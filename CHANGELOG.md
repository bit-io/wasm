# Changelog

Format: [Keep a Changelog](https://keepachangelog.com/), wersjonowanie
opisane w `docs/API.md`.

## [0.2.0] — wasm przez emcc

### BREAKING
- Cel `wasm32` kompilatora H# buduje teraz przez `emcc` (Emscripten): `h# compile --target wasm`
  daje `app.js` + `app.wasm`. Stara ścieżka (`--emit object` + `cargo` + `wasm-ld`) nie istnieje.
- Usunięte: `runtime/` (Rust: `core_runtime.rs`, `web_bridge.rs`) i `glue/hsharp-web-loader.js` —
  runtime H# kompiluje sam emcc, a stronę JS dostarcza `emscripten/hsharp-web.js`.
- Importy: `use "bit -> wasm" from "wasm"`, potem `wasm::web_dom::...` (poprzednie
  `use "web-wasm -> ..."` nie było poprawną składnią H#).

### Dodano
- `emscripten/web_bridge.cpp` (generowany) i `emscripten/hsharp-web.js` (emcc `--js-library`).
- `scripts/gen-emscripten-bridge.py` — generator mostu z deklaracji `fn hweb_*`.
- `src/lib.h#` wylicza moduły (wcześniej był pusty), wersja w `wasm::VERSION`.

### Naprawiono
- Kontrakt ABI: `bool` to `int8_t` (nie `i64`), `int` to `int64_t` — potwierdzone na kodzie
  wygenerowanym przez kompilator.
- Przykład `hello-page` przepisany pod nową ścieżkę (`Bit.hk` zamiast `Bytes.hk`).

## [0.1.0] — pierwsze wydanie

### Dodano
- `web_console`, `web_dom`, `web_events`, `web_timers`, `web_storage`,
  `web_fetch`, `web_canvas`, `web_random`, `web_time`, `web_types` —
  100% H#, zero zależności w `bytes.hk`.
- `runtime-rust/` — reimplementacja podzbioru `runtime/core.c` (`hsh_*`)
  na `wasm32-unknown-unknown` + most do przeglądarki (`hweb_*` / import
  `hsharp_web`), jedyny nie-H#-owy fragment pakietu.
- `glue/hsharp-web-loader.js` — jedyny plik JS, implementuje most
  `hsharp_web`, ładuje i uruchamia skompilowany moduł `.wasm`.
- `scripts/build-wasm.sh` — pipeline `hsharp compile --emit object
  --target wasm32` → `cargo build --target wasm32-unknown-unknown` →
  `wasm-ld`.
- `examples/hello-page` — działający przykład (licznik kliknięć + timer +
  losowy UUID), pokazuje wzorzec przechowywania stanu w DOM.

### Znane ograniczenia (patrz `docs/API.md`)
- Brak `fs::`/`proc::`/`env::`/`net_*`/`sqlite`/`python`/`crypto_rsa` —
  świadomie, zależą od POSIX.
- Brak przekazywania domknięć H# przez `extern static [rust]` — patrz
  „Model zdarzeń” w `docs/API.md`.
- Kontrakt ABI (typy parametrów `extern static [rust]`) opisany w
  `docs/API.md` nie został zweryfikowany uruchomieniowo w tym środowisku
  (brak toolchainu LLVM 21 + H# w sandboxie, w którym ta biblioteka
  powstała) — zweryfikuj przed produkcją zgodnie z instrukcją tamże.
