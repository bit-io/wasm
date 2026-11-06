# `wasm` — pisz strony w H#, kompiluj do WebAssembly

Biblioteka H# (bit.io) z bindingami przeglądarki: konsola, DOM, zdarzenia, timery, storage, fetch,
canvas, losowość, czas. **API jest w 100% czystym H#** (`src/*.h#`). Do WebAssembly program kompiluje
sam kompilator: H# tłumaczy się do C++, a `h# compile --target wasm` woła `emcc` z Emscripten
(emsdk). Ta biblioteka dostarcza to, co łączy program z przeglądarką (`emscripten/`).

## Użycie

`Bit.hk` projektu:

```
[default-build]
-> emit   => bin
-> target => wasm

[dependencies]
-> wasm => newest
```

Kod:

```hsharp
use "bit -> wasm" from "wasm"

fn main() is
    wasm::web_console::log("Cześć z H#!")
    let h: wasm::web_dom::Element = wasm::web_dom::create("h1")
    h.set_text("Działa!")
    wasm::web_dom::body().append(h)
end
```

Budowanie: `bit` (albo `bit --target wasm`, albo `h# compile src/main.h# --target wasm -o build/app.js`).
Za pierwszym razem bit sklonuje emsdk do `cache/env/emsdk` i wykona tam `./emsdk install latest` oraz
`./emsdk activate latest` — bez żadnych zmian w `.bashrc` ani innych plikach środowiska.
Wynik: `build/app.js` + `build/app.wasm` (`-o build/app.html` dodaje gotową stronę).
Strona ładuje `<script src="build/app.js"></script>`; WASM wymaga HTTP (`python3 -m http.server`), nie file://.

## Zawartość

```
wasm/
├── Bit.hk                    manifest pakietu
├── src/                      BIBLIOTEKA — 100% H# (web_console, web_dom, web_events, web_timers,
│                             web_storage, web_fetch, web_canvas, web_random, web_time, web_types)
├── emscripten/
│   ├── web_bridge.cpp        GENEROWANY: definicje hweb_* (C), wołają hsharp_js_*
│   └── hsharp-web.js         emcc --js-library: strona JS (DOM, timery, fetch, ...)
├── scripts/gen-emscripten-bridge.py   generuje web_bridge.cpp z deklaracji `fn hweb_*` w src/
├── examples/hello-page/      działający przykład (licznik kliknięć + timer + UUID)
└── docs/API.md               dokumentacja i polityka stabilności
```

## Jak to działa

Każda funkcja `fn hweb_x(...)` w `extern static [rust] is ... end` jest dla kompilatora zwykłym symbolem C
(`int` → `int64_t`, `bool` → `int8_t`, `f64` → `double`, `string` → `const char*`). `web_bridge.cpp`
definiuje je dokładnie tymi typami i przekazuje do `hsharp_js_x` z `hsharp-web.js`. Kompilator H#
dołącza oba pliki do linkowania (`em++ --js-library`), ilekroć program woła którekolwiek `hweb_*`;
runtime H# (`write()`, stringi, tablice) kompiluje ten sam `emcc` z libc Emscripten, więc nie ma
osobnego runtime'u.

Zdarzenia asynchroniczne (DOM, timery, fetch) wracają jednym punktem wejścia, który piszesz sam:

```hsharp
pub fn wasm_on_event(kind: int, id: int, num: f64, text: string) is ... end
```

## Czego nie ma

`fs::`, `proc::`, `net_*`, `sqlite`, `python`, `crypto_rsa` — to zależy od systemu, którego w przeglądarce
nie ma (w node część `std` działa, bo Emscripten ma własną libc i emulację plików).

## Licencja

MIT
