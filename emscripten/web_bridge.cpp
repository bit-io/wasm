#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "hsharp_rt.hpp"

extern "C" {

extern double hsharp_js_canvas_set_size(double, double, double);
extern double hsharp_js_canvas_clear(double);
extern double hsharp_js_canvas_fill_style(double, const char*);
extern double hsharp_js_canvas_stroke_style(double, const char*);
extern double hsharp_js_canvas_line_width(double, double);
extern double hsharp_js_canvas_fill_rect(double, double, double, double, double);
extern double hsharp_js_canvas_stroke_rect(double, double, double, double, double);
extern double hsharp_js_canvas_clear_rect(double, double, double, double, double);
extern double hsharp_js_canvas_begin_path(double);
extern double hsharp_js_canvas_move_to(double, double, double);
extern double hsharp_js_canvas_line_to(double, double, double);
extern double hsharp_js_canvas_arc(double, double, double, double, double, double);
extern double hsharp_js_canvas_close_path(double);
extern double hsharp_js_canvas_fill(double);
extern double hsharp_js_canvas_stroke(double);
extern double hsharp_js_canvas_font(double, const char*);
extern double hsharp_js_canvas_fill_text(double, const char*, double, double);
extern void hsharp_js_console_log(const char*);
extern void hsharp_js_console_info(const char*);
extern void hsharp_js_console_warn(const char*);
extern void hsharp_js_console_error(const char*);
extern void hsharp_js_console_debug(const char*);
extern void hsharp_js_console_clear(void);
extern void hsharp_js_console_group(const char*);
extern void hsharp_js_console_group_end(void);
extern double hsharp_js_dom_get_by_id(const char*);
extern double hsharp_js_dom_query_selector(const char*);
extern const char* hsharp_js_dom_query_selector_all(const char*);
extern double hsharp_js_dom_create_element(const char*);
extern double hsharp_js_dom_body(void);
extern double hsharp_js_dom_head(void);
extern double hsharp_js_dom_append_child(double, double);
extern double hsharp_js_dom_prepend_child(double, double);
extern double hsharp_js_dom_remove(double);
extern double hsharp_js_dom_set_inner_html(double, const char*);
extern const char* hsharp_js_dom_get_inner_html(double);
extern double hsharp_js_dom_set_text(double, const char*);
extern const char* hsharp_js_dom_get_text(double);
extern double hsharp_js_dom_set_attribute(double, const char*, const char*);
extern const char* hsharp_js_dom_get_attribute(double, const char*);
extern double hsharp_js_dom_remove_attribute(double, const char*);
extern double hsharp_js_dom_add_class(double, const char*);
extern double hsharp_js_dom_remove_class(double, const char*);
extern double hsharp_js_dom_toggle_class(double, const char*);
extern double hsharp_js_dom_has_class(double, const char*);
extern double hsharp_js_dom_set_style(double, const char*, const char*);
extern const char* hsharp_js_dom_get_style(double, const char*);
extern double hsharp_js_dom_set_value(double, const char*);
extern const char* hsharp_js_dom_get_value(double);
extern double hsharp_js_dom_is_valid(double);
extern double hsharp_js_dom_parent(double);
extern double hsharp_js_events_listen(double, const char*);
extern double hsharp_js_events_unlisten(double);
extern double hsharp_js_fetch_get(const char*);
extern double hsharp_js_fetch_post(const char*, const char*, const char*);
extern double hsharp_js_fetch_status(double);
extern double hsharp_js_random_f64(void);
extern double hsharp_js_random_int(double, double);
extern const char* hsharp_js_random_hex(double);
extern const char* hsharp_js_random_uuid_v4(void);
extern const char* hsharp_js_storage_local_get(const char*);
extern double hsharp_js_storage_local_set(const char*, const char*);
extern double hsharp_js_storage_local_remove(const char*);
extern double hsharp_js_storage_local_clear(void);
extern double hsharp_js_storage_local_has(const char*);
extern const char* hsharp_js_storage_session_get(const char*);
extern double hsharp_js_storage_session_set(const char*, const char*);
extern double hsharp_js_storage_session_remove(const char*);
extern double hsharp_js_storage_session_clear(void);
extern double hsharp_js_storage_session_has(const char*);
extern double hsharp_js_time_now_ms(void);
extern double hsharp_js_time_perf_now_ms(void);
extern double hsharp_js_timers_set_timeout(double);
extern void hsharp_js_timers_clear_timeout(double);
extern double hsharp_js_timers_set_interval(double);
extern void hsharp_js_timers_clear_interval(double);
extern double hsharp_js_timers_request_animation_frame(void);
extern void hsharp_js_timers_cancel_animation_frame(double);

int8_t hweb_canvas_set_size(int64_t handle, int64_t width, int64_t height) { return (int8_t)(hsharp_js_canvas_set_size((double)handle, (double)width, (double)height) != 0); }
int8_t hweb_canvas_clear(int64_t handle) { return (int8_t)(hsharp_js_canvas_clear((double)handle) != 0); }
int8_t hweb_canvas_fill_style(int64_t handle, const char* css_color) { return (int8_t)(hsharp_js_canvas_fill_style((double)handle, css_color) != 0); }
int8_t hweb_canvas_stroke_style(int64_t handle, const char* css_color) { return (int8_t)(hsharp_js_canvas_stroke_style((double)handle, css_color) != 0); }
int8_t hweb_canvas_line_width(int64_t handle, double width) { return (int8_t)(hsharp_js_canvas_line_width((double)handle, width) != 0); }
int8_t hweb_canvas_fill_rect(int64_t handle, double x, double y, double w, double h) { return (int8_t)(hsharp_js_canvas_fill_rect((double)handle, x, y, w, h) != 0); }
int8_t hweb_canvas_stroke_rect(int64_t handle, double x, double y, double w, double h) { return (int8_t)(hsharp_js_canvas_stroke_rect((double)handle, x, y, w, h) != 0); }
int8_t hweb_canvas_clear_rect(int64_t handle, double x, double y, double w, double h) { return (int8_t)(hsharp_js_canvas_clear_rect((double)handle, x, y, w, h) != 0); }
int8_t hweb_canvas_begin_path(int64_t handle) { return (int8_t)(hsharp_js_canvas_begin_path((double)handle) != 0); }
int8_t hweb_canvas_move_to(int64_t handle, double x, double y) { return (int8_t)(hsharp_js_canvas_move_to((double)handle, x, y) != 0); }
int8_t hweb_canvas_line_to(int64_t handle, double x, double y) { return (int8_t)(hsharp_js_canvas_line_to((double)handle, x, y) != 0); }
int8_t hweb_canvas_arc(int64_t handle, double x, double y, double radius, double start_rad, double end_rad) { return (int8_t)(hsharp_js_canvas_arc((double)handle, x, y, radius, start_rad, end_rad) != 0); }
int8_t hweb_canvas_close_path(int64_t handle) { return (int8_t)(hsharp_js_canvas_close_path((double)handle) != 0); }
int8_t hweb_canvas_fill(int64_t handle) { return (int8_t)(hsharp_js_canvas_fill((double)handle) != 0); }
int8_t hweb_canvas_stroke(int64_t handle) { return (int8_t)(hsharp_js_canvas_stroke((double)handle) != 0); }
int8_t hweb_canvas_font(int64_t handle, const char* css_font) { return (int8_t)(hsharp_js_canvas_font((double)handle, css_font) != 0); }
int8_t hweb_canvas_fill_text(int64_t handle, const char* text, double x, double y) { return (int8_t)(hsharp_js_canvas_fill_text((double)handle, text, x, y) != 0); }
void hweb_console_log(const char* msg) { hsharp_js_console_log(msg); }
void hweb_console_info(const char* msg) { hsharp_js_console_info(msg); }
void hweb_console_warn(const char* msg) { hsharp_js_console_warn(msg); }
void hweb_console_error(const char* msg) { hsharp_js_console_error(msg); }
void hweb_console_debug(const char* msg) { hsharp_js_console_debug(msg); }
void hweb_console_clear(void) { hsharp_js_console_clear(); }
void hweb_console_group(const char* label) { hsharp_js_console_group(label); }
void hweb_console_group_end(void) { hsharp_js_console_group_end(); }
int64_t hweb_dom_get_by_id(const char* id) { return (int64_t)hsharp_js_dom_get_by_id(id); }
int64_t hweb_dom_query_selector(const char* selector) { return (int64_t)hsharp_js_dom_query_selector(selector); }
const char* hweb_dom_query_selector_all(const char* selector) { return hsharp_js_dom_query_selector_all(selector); }
int64_t hweb_dom_create_element(const char* tag) { return (int64_t)hsharp_js_dom_create_element(tag); }
int64_t hweb_dom_body(void) { return (int64_t)hsharp_js_dom_body(); }
int64_t hweb_dom_head(void) { return (int64_t)hsharp_js_dom_head(); }
int8_t hweb_dom_append_child(int64_t parent, int64_t child) { return (int8_t)(hsharp_js_dom_append_child((double)parent, (double)child) != 0); }
int8_t hweb_dom_prepend_child(int64_t parent, int64_t child) { return (int8_t)(hsharp_js_dom_prepend_child((double)parent, (double)child) != 0); }
int8_t hweb_dom_remove(int64_t handle) { return (int8_t)(hsharp_js_dom_remove((double)handle) != 0); }
int8_t hweb_dom_set_inner_html(int64_t handle, const char* html) { return (int8_t)(hsharp_js_dom_set_inner_html((double)handle, html) != 0); }
const char* hweb_dom_get_inner_html(int64_t handle) { return hsharp_js_dom_get_inner_html((double)handle); }
int8_t hweb_dom_set_text(int64_t handle, const char* text) { return (int8_t)(hsharp_js_dom_set_text((double)handle, text) != 0); }
const char* hweb_dom_get_text(int64_t handle) { return hsharp_js_dom_get_text((double)handle); }
int8_t hweb_dom_set_attribute(int64_t handle, const char* name, const char* value) { return (int8_t)(hsharp_js_dom_set_attribute((double)handle, name, value) != 0); }
const char* hweb_dom_get_attribute(int64_t handle, const char* name) { return hsharp_js_dom_get_attribute((double)handle, name); }
int8_t hweb_dom_remove_attribute(int64_t handle, const char* name) { return (int8_t)(hsharp_js_dom_remove_attribute((double)handle, name) != 0); }
int8_t hweb_dom_add_class(int64_t handle, const char* class_name) { return (int8_t)(hsharp_js_dom_add_class((double)handle, class_name) != 0); }
int8_t hweb_dom_remove_class(int64_t handle, const char* class_name) { return (int8_t)(hsharp_js_dom_remove_class((double)handle, class_name) != 0); }
int8_t hweb_dom_toggle_class(int64_t handle, const char* class_name) { return (int8_t)(hsharp_js_dom_toggle_class((double)handle, class_name) != 0); }
int8_t hweb_dom_has_class(int64_t handle, const char* class_name) { return (int8_t)(hsharp_js_dom_has_class((double)handle, class_name) != 0); }
int8_t hweb_dom_set_style(int64_t handle, const char* prop, const char* value) { return (int8_t)(hsharp_js_dom_set_style((double)handle, prop, value) != 0); }
const char* hweb_dom_get_style(int64_t handle, const char* prop) { return hsharp_js_dom_get_style((double)handle, prop); }
int8_t hweb_dom_set_value(int64_t handle, const char* value) { return (int8_t)(hsharp_js_dom_set_value((double)handle, value) != 0); }
const char* hweb_dom_get_value(int64_t handle) { return hsharp_js_dom_get_value((double)handle); }
int8_t hweb_dom_is_valid(int64_t handle) { return (int8_t)(hsharp_js_dom_is_valid((double)handle) != 0); }
int64_t hweb_dom_parent(int64_t handle) { return (int64_t)hsharp_js_dom_parent((double)handle); }
int64_t hweb_events_listen(int64_t handle, const char* event_type) { return (int64_t)hsharp_js_events_listen((double)handle, event_type); }
int8_t hweb_events_unlisten(int64_t listener_id) { return (int8_t)(hsharp_js_events_unlisten((double)listener_id) != 0); }
int64_t hweb_fetch_get(const char* url) { return (int64_t)hsharp_js_fetch_get(url); }
int64_t hweb_fetch_post(const char* url, const char* body, const char* content_type) { return (int64_t)hsharp_js_fetch_post(url, body, content_type); }
int64_t hweb_fetch_status(int64_t request_id) { return (int64_t)hsharp_js_fetch_status((double)request_id); }
double hweb_random_f64(void) { return hsharp_js_random_f64(); }
int64_t hweb_random_int(int64_t min, int64_t max) { return (int64_t)hsharp_js_random_int((double)min, (double)max); }
const char* hweb_random_hex(int64_t n_bytes) { return hsharp_js_random_hex((double)n_bytes); }
const char* hweb_random_uuid_v4(void) { return hsharp_js_random_uuid_v4(); }
const char* hweb_storage_local_get(const char* key) { return hsharp_js_storage_local_get(key); }
int8_t hweb_storage_local_set(const char* key, const char* value) { return (int8_t)(hsharp_js_storage_local_set(key, value) != 0); }
int8_t hweb_storage_local_remove(const char* key) { return (int8_t)(hsharp_js_storage_local_remove(key) != 0); }
int8_t hweb_storage_local_clear(void) { return (int8_t)(hsharp_js_storage_local_clear() != 0); }
int8_t hweb_storage_local_has(const char* key) { return (int8_t)(hsharp_js_storage_local_has(key) != 0); }
const char* hweb_storage_session_get(const char* key) { return hsharp_js_storage_session_get(key); }
int8_t hweb_storage_session_set(const char* key, const char* value) { return (int8_t)(hsharp_js_storage_session_set(key, value) != 0); }
int8_t hweb_storage_session_remove(const char* key) { return (int8_t)(hsharp_js_storage_session_remove(key) != 0); }
int8_t hweb_storage_session_clear(void) { return (int8_t)(hsharp_js_storage_session_clear() != 0); }
int8_t hweb_storage_session_has(const char* key) { return (int8_t)(hsharp_js_storage_session_has(key) != 0); }
double hweb_time_now_ms(void) { return hsharp_js_time_now_ms(); }
double hweb_time_perf_now_ms(void) { return hsharp_js_time_perf_now_ms(); }
int64_t hweb_timers_set_timeout(int64_t delay_ms) { return (int64_t)hsharp_js_timers_set_timeout((double)delay_ms); }
void hweb_timers_clear_timeout(int64_t id) { hsharp_js_timers_clear_timeout((double)id); }
int64_t hweb_timers_set_interval(int64_t delay_ms) { return (int64_t)hsharp_js_timers_set_interval((double)delay_ms); }
void hweb_timers_clear_interval(int64_t id) { hsharp_js_timers_clear_interval((double)id); }
int64_t hweb_timers_request_animation_frame(void) { return (int64_t)hsharp_js_timers_request_animation_frame(); }
void hweb_timers_cancel_animation_frame(int64_t id) { hsharp_js_timers_cancel_animation_frame((double)id); }

// ── runtime plumbing used by emscripten/hsharp-web.js ─────────────────────────
// Shared buffer for string results: the H# wrapper copies the returned `const char*` at once, so a
// single reusable buffer is enough (and nothing leaks per call).
static char*  g_retbuf = nullptr;
static size_t g_retcap = 0;
char* hsharp_web_retbuf(int n) {
    size_t need = n > 0 ? (size_t)n : 1;
    if (need > g_retcap) {
        size_t cap = g_retcap ? g_retcap : 256;
        while (cap < need) cap *= 2;
        char* p = (char*)realloc(g_retbuf, cap);
        if (!p) return g_retbuf;
        g_retbuf = p; g_retcap = cap;
    }
    return g_retbuf;
}
void* hsharp_web_alloc(int n) { return malloc(n > 0 ? (size_t)n : 1); }
void  hsharp_web_free(void* p) { free(p); }

// The single entry point for every asynchronous event. Calls the program's
//   pub fn wasm_on_event(kind: int, id: int, num: f64, text: string)
// (a weak reference: programs that never register events need not define it).
}  // extern "C"

void f_wasm_on_event(int64_t kind, int64_t id, double num, hs::Str text) __attribute__((weak));

extern "C" void hsharp_web_event(int kind, int id, double num, const char* text) {
    if (!f_wasm_on_event) return;
    f_wasm_on_event((int64_t)kind, (int64_t)id, num, hs::Str(std::string(text ? text : "")));
}
