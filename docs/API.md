# `web-wasm` — dokumentacja API i polityka stabilności (v0.1.0)

## Polityka stabilności

`web-wasm` istnieje po to, żeby dawało się na nim polegać. Zasady:

1. **Żadna publiczna funkcja/struktura/stała z `src/*.h#` nie zmieni nazwy,
   liczby parametrów ani ich typów bez zmiany wersji MAJOR** (semver:
   `MAJOR.MINOR.PATCH`, mimo że jesteśmy jeszcze przed 1.0.0 — dla tej
   biblioteki 0.x.y traktujemy zmianę pierwszej cyfry po zerze jak MAJOR).
2. Nowe funkcje mogą być dodawane w wersji MINOR. Nigdy nie usuwamy ani
   nie zmieniamy istniejącej sygnatury w MINOR/PATCH — najwyżej oznaczamy
   ją jako przestarzałą w komentarzu i utrzymujemy dalej.
3. Wartości liczbowe `KIND_*` (`web_events.h#`) są częścią zamrożonego
   ABI między H# a `emscripten/hsharp-web.js` — traktowane jak MAJOR.
4. Nazwy eksportów wasm używane przez `emscripten/hsharp-web.js` (`main`, `hsharp_web_event`,
   `hsharp_web_alloc`, `hsharp_web_free`, `hsharp_web_retbuf`) oraz sygnatura `wasm_on_event` są
   również częścią zamrożonego ABI.
5. Każda zmiana łamiąca którykolwiek z powyższych punktów trafia do
   `CHANGELOG.md` z jawnym nagłówkiem „BREAKING”.

## Kontrakt ABI (jak program łączy się z przeglądarką)

Kompilator H# tłumaczy każde `fn hweb_x(...)` z bloku `extern static [rust]` na deklarację
`extern "C" <ret> hx_x(<args>) __asm__("hweb_x");` — typy po stronie C są ustalone przez kompilator
(`cg::c_abi_ty`), potwierdzone na wygenerowanym C++:

| typ H#     | typ C                |
|------------|----------------------|
| `int`      | `int64_t`            |
| `bool`     | `int8_t`             |
| `f64`/`f32`| `double`/`float`     |
| `string`   | `const char*` (UTF-8, zakończony zerem) |

`emscripten/web_bridge.cpp` (generowany przez `scripts/gen-emscripten-bridge.py`) definiuje wszystkie
`hweb_*` dokładnie tymi typami. Do JS liczby przechodzą jako `double` (bez BigInt, działa na każdej
wersji emscripten), napisy jako wskaźniki. Napis zwracany z JS trafia do jednego, współdzielonego bufora
(`hsharp_web_retbuf`) — opakowanie wygenerowane przez kompilator kopiuje go od razu.

Punkt wejścia zdarzeń: `hsharp_web_event(kind, id, num, text)` (eksportowany z wasm, wołany z JS)
wywołuje Twoje `pub fn wasm_on_event(kind: int, id: int, num: f64, text: string)`; gdy go nie ma,
zdarzenia są ignorowane (słabe odwołanie).

Po zmianie jakiejkolwiek deklaracji `fn hweb_*` w `src/` uruchom
`python3 scripts/gen-emscripten-bridge.py` — skrypt odświeży `web_bridge.cpp` i zgłosi błąd, jeśli
któraś funkcja nie ma odpowiednika `hsharp_js_*` w `emscripten/hsharp-web.js`.

## Moduły

| Moduł            | Użycie (po `use "bit -> wasm" from "wasm"`)                                              | Zakres |
|-------------------|-----------------------------------------------------|--------|
| `web_console`      | `wasm::web_console`      | `console.*` |
| `web_dom`          | `wasm::web_dom`              | DOM: `Element`, tworzenie/usuwanie, atrybuty, klasy, style, `value` |
| `web_events`       | `wasm::web_events`        | `listen`/`unlisten`, stałe `KIND_*`, punkt wejścia `wasm_on_event` |
| `web_timers`       | `wasm::web_timers`        | `setTimeout`/`setInterval`/`requestAnimationFrame` |
| `web_storage`      | `wasm::web_storage`      | `localStorage`/`sessionStorage` |
| `web_fetch`        | `wasm::web_fetch`          | `fetch()` (GET/POST), asynchronicznie przez `wasm_on_event` |
| `web_canvas`       | `wasm::web_canvas`        | Canvas 2D — prostokąty, ścieżki, tekst |
| `web_random`       | `wasm::web_random`        | `crypto.getRandomValues`-backed losowość |
| `web_time`         | `wasm::web_time`            | `Date.now()` / `performance.now()` (zamiennik `std -> time` na wasm32) |
| `web_types`        | `wasm::web_types`          | `Response` używany przez `web_fetch` |

Pełne sygnatury — patrz nagłówki poszczególnych plików `src/*.h#`, każdy
jest w pełni udokumentowany komentarzami i celowo krótki (jeden plik =
jedna odpowiedzialność), żeby dało się go przeczytać w całości w minutę.

## Model zdarzeń — dlaczego jeden punkt wejścia, nie domknięcia

H#'owy `extern static [rust]` ma dziś dobrze zdefiniowaną, stabilną
marshalling tylko dla typów płaskich (int/bool/float) i `string`/`bytes`
(patrz `ffi_rust_native.rs`: `RsTy::Opaque` dla czegokolwiek innego, w
tym typów funkcyjnych/domknięć, jest **odrzucane** przez generator
shimów — nie ma jeszcze stabilnego ABI dla przekazania domknięcia przez
tę granicę). Zamiast prowizorki opartej o niestabilny/niepotwierdzony
mechanizm, `web-wasm` celowo używa jednego, przewidywalnego wzorca:
zarejestruj coś (`events::listen`, `timers::set_timeout`, ...), odbierz
wynik w Twojej własnej `pub fn wasm_on_event(kind, id, num, text)`.
Jedna, stabilna granica ABI zamiast N niestabilnych.

## Stan aplikacji: H# nie ma mutowalnych zmiennych globalnych

Top-level `let`/`const` w H# jest zawsze **niezmienny** i liczony raz
przed `main()` (potwierdzone w AST kompilatora: `Item::ConstDef`, jedyny
top-level-binding, z komentarzem wprost mówiącym, że nie jest
przeliczany ponownie jak lokalny `let`). W praktyce oznacza to, że stanu
aplikacji między `main()` a `wasm_on_event()` **nie da się** trzymać w
zwykłej zmiennej modułu. Dwa poprawne miejsca na taki stan:

- **DOM** (atrybuty `data-*`, `value` pól formularzy) — stan nie musi
  przetrwać odświeżenia strony (patrz `examples/hello-page`).
- **`web_storage`** (`localStorage`/`sessionStorage`) — stan ma przetrwać
  odświeżenie/zamknięcie karty.

## Czego ta biblioteka NIE dostarcza (świadomie)

`fs::`, `proc::`, `env::`, `net_tcp`/`net_udp`, `sqlite`, `python`,
`crypto_rsa` (OpenSSL) — wszystko to, co w `std` zależy od POSIX/systemu
plików/procesów. W przeglądarce ich po prostu nie ma — to ograniczenie platformy,
nie tej biblioteki (w node część z nich działa dzięki libc Emscripten). `web_random` i
`web_time` istnieją właśnie jako w pełni funkcjonalne, webowe zamienniki
dla tych dwóch akurat przypadków, gdzie użytkownik najczęściej i tak
sięgnąłby po `std`.
