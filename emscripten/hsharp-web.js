(function () {
  var lib = {};

  // ── shared state ───────────────────────────────────────────────────────
  lib.$HSW = {
    handles: [],      // handle:int -> Element
    listeners: [],    // listener_id:int -> {el, type, fn}
    timeouts: {}, intervals: {}, rafs: {}, fetchStatus: {},
    ctx: {},
    nextTimer: 1, nextFetch: 1,

    str: function (p) { return p ? UTF8ToString(p) : ''; },
    ret: function (s) {
      s = (s === null || s === undefined) ? '' : String(s);
      var n = lengthBytesUTF8(s) + 1;
      var p = _hsharp_web_retbuf(n);
      stringToUTF8(s, p, n);
      return p;
    },
    reg: function (el) {
      if (!el) return -1;
      var h = HSW.handles.indexOf(el);
      if (h >= 0) return h;
      HSW.handles.push(el);
      return HSW.handles.length - 1;
    },
    el: function (h) { return (h >= 0 && h < HSW.handles.length) ? HSW.handles[h] : null; },
    event: function (kind, id, num, text) {
      text = (text === null || text === undefined) ? '' : String(text);
      var n = lengthBytesUTF8(text) + 1;
      var p = _hsharp_web_alloc(n);
      stringToUTF8(text, p, n);
      _hsharp_web_event(kind, id, num, p);
      _hsharp_web_free(p);
    },
    c2d: function (h) {
      if (HSW.ctx[h]) return HSW.ctx[h];
      var el = HSW.el(h);
      if (!el || el.tagName !== 'CANVAS') return null;
      return (HSW.ctx[h] = el.getContext('2d'));
    },
    store: function (area) { return area === 0 ? localStorage : sessionStorage; },
    KIND_TIMEOUT: 1, KIND_INTERVAL: 2, KIND_FRAME: 3, KIND_DOM_EVENT: 4, KIND_FETCH_OK: 5, KIND_FETCH_ERR: 6
  };

  var fns = {
    // ── console ──
    console_log:   function (m) { console.log(HSW.str(m)); },
    console_info:  function (m) { console.info(HSW.str(m)); },
    console_warn:  function (m) { console.warn(HSW.str(m)); },
    console_error: function (m) { console.error(HSW.str(m)); },
    console_debug: function (m) { (console.debug || console.log).call(console, HSW.str(m)); },
    console_clear: function () { if (console.clear) console.clear(); },
    console_group: function (l) { (console.group || console.log).call(console, HSW.str(l)); },
    console_group_end: function () { if (console.groupEnd) console.groupEnd(); },

    // ── dom ──
    dom_get_by_id:  function (p) { return HSW.reg(document.getElementById(HSW.str(p))); },
    dom_query_selector: function (p) { return HSW.reg(document.querySelector(HSW.str(p))); },
    dom_query_selector_all: function (p) {
      var found = document.querySelectorAll(HSW.str(p)), ids = [];
      for (var i = 0; i < found.length; i++) ids.push(HSW.reg(found[i]));
      return HSW.ret(ids.join(','));
    },
    dom_create_element: function (p) { return HSW.reg(document.createElement(HSW.str(p))); },
    dom_body: function () { return HSW.reg(document.body); },
    dom_head: function () { return HSW.reg(document.head); },
    dom_append_child:  function (a, b) { var p = HSW.el(a), c = HSW.el(b); if (!p || !c) return 0; p.appendChild(c); return 1; },
    dom_prepend_child: function (a, b) { var p = HSW.el(a), c = HSW.el(b); if (!p || !c) return 0; p.insertBefore(c, p.firstChild); return 1; },
    dom_remove: function (h) { var e = HSW.el(h); if (!e) return 0; e.remove(); return 1; },
    dom_set_inner_html: function (h, s) { var e = HSW.el(h); if (!e) return 0; e.innerHTML = HSW.str(s); return 1; },
    dom_get_inner_html: function (h) { var e = HSW.el(h); return HSW.ret(e ? e.innerHTML : ''); },
    dom_set_text: function (h, s) { var e = HSW.el(h); if (!e) return 0; e.textContent = HSW.str(s); return 1; },
    dom_get_text: function (h) { var e = HSW.el(h); return HSW.ret(e ? e.textContent : ''); },
    dom_set_attribute: function (h, n, v) { var e = HSW.el(h); if (!e) return 0; e.setAttribute(HSW.str(n), HSW.str(v)); return 1; },
    dom_get_attribute: function (h, n) {
      var e = HSW.el(h); var v = e ? e.getAttribute(HSW.str(n)) : '';
      return HSW.ret(v === null ? '' : v);
    },
    dom_remove_attribute: function (h, n) { var e = HSW.el(h); if (!e) return 0; e.removeAttribute(HSW.str(n)); return 1; },
    dom_add_class:    function (h, c) { var e = HSW.el(h); if (!e) return 0; e.classList.add(HSW.str(c)); return 1; },
    dom_remove_class: function (h, c) { var e = HSW.el(h); if (!e) return 0; e.classList.remove(HSW.str(c)); return 1; },
    dom_toggle_class: function (h, c) { var e = HSW.el(h); if (!e) return 0; return e.classList.toggle(HSW.str(c)) ? 1 : 0; },
    dom_has_class:    function (h, c) { var e = HSW.el(h); return (e && e.classList.contains(HSW.str(c))) ? 1 : 0; },
    dom_set_style: function (h, p, v) { var e = HSW.el(h); if (!e) return 0; e.style.setProperty(HSW.str(p), HSW.str(v)); return 1; },
    dom_get_style: function (h, p) { var e = HSW.el(h); return HSW.ret(e ? e.style.getPropertyValue(HSW.str(p)) : ''); },
    dom_set_value: function (h, v) { var e = HSW.el(h); if (!e) return 0; e.value = HSW.str(v); return 1; },
    dom_get_value: function (h) { var e = HSW.el(h); return HSW.ret(e && ('value' in e) ? e.value : ''); },
    dom_is_valid:  function (h) { var e = HSW.el(h); return (e && e.isConnected !== false) ? 1 : 0; },
    dom_parent:    function (h) { var e = HSW.el(h); return (e && e.parentElement) ? HSW.reg(e.parentElement) : -1; },

    // ── events ──
    events_listen: function (h, t) {
      var el = HSW.el(h); if (!el) return -1;
      var type = HSW.str(t), id = HSW.listeners.length;
      var fn = function (ev) { HSW.event(HSW.KIND_DOM_EVENT, id, 0, ev.type); };
      HSW.listeners.push({ el: el, type: type, fn: fn });
      el.addEventListener(type, fn);
      return id;
    },
    events_unlisten: function (id) {
      var l = HSW.listeners[id]; if (!l) return 0;
      l.el.removeEventListener(l.type, l.fn);
      HSW.listeners[id] = null;
      return 1;
    },

    // ── timers ──
    timers_set_timeout: function (ms) {
      var id = HSW.nextTimer++;
      HSW.timeouts[id] = setTimeout(function () { delete HSW.timeouts[id]; HSW.event(HSW.KIND_TIMEOUT, id, 0, ''); }, ms);
      return id;
    },
    timers_clear_timeout: function (id) { if (HSW.timeouts[id] !== undefined) { clearTimeout(HSW.timeouts[id]); delete HSW.timeouts[id]; } },
    timers_set_interval: function (ms) {
      var id = HSW.nextTimer++;
      HSW.intervals[id] = setInterval(function () { HSW.event(HSW.KIND_INTERVAL, id, 0, ''); }, ms);
      return id;
    },
    timers_clear_interval: function (id) { if (HSW.intervals[id] !== undefined) { clearInterval(HSW.intervals[id]); delete HSW.intervals[id]; } },
    timers_request_animation_frame: function () {
      var id = HSW.nextTimer++;
      HSW.rafs[id] = requestAnimationFrame(function (ts) { delete HSW.rafs[id]; HSW.event(HSW.KIND_FRAME, id, ts, ''); });
      return id;
    },
    timers_cancel_animation_frame: function (id) { if (HSW.rafs[id] !== undefined) { cancelAnimationFrame(HSW.rafs[id]); delete HSW.rafs[id]; } },

    // ── storage ──
    storage_local_get:  function (k) { var v = localStorage.getItem(HSW.str(k)); return HSW.ret(v === null ? '' : v); },
    storage_local_set:  function (k, v) { try { localStorage.setItem(HSW.str(k), HSW.str(v)); return 1; } catch (e) { return 0; } },
    storage_local_remove: function (k) { localStorage.removeItem(HSW.str(k)); return 1; },
    storage_local_clear:  function () { localStorage.clear(); return 1; },
    storage_local_has:  function (k) { return localStorage.getItem(HSW.str(k)) !== null ? 1 : 0; },
    storage_session_get:  function (k) { var v = sessionStorage.getItem(HSW.str(k)); return HSW.ret(v === null ? '' : v); },
    storage_session_set:  function (k, v) { try { sessionStorage.setItem(HSW.str(k), HSW.str(v)); return 1; } catch (e) { return 0; } },
    storage_session_remove: function (k) { sessionStorage.removeItem(HSW.str(k)); return 1; },
    storage_session_clear:  function () { sessionStorage.clear(); return 1; },
    storage_session_has:  function (k) { return sessionStorage.getItem(HSW.str(k)) !== null ? 1 : 0; },

    // ── fetch ──
    fetch_get:  function (u) { return HSW.doFetch('GET', HSW.str(u), null, null); },
    fetch_post: function (u, b, c) { return HSW.doFetch('POST', HSW.str(u), HSW.str(b), HSW.str(c)); },
    fetch_status: function (id) { return HSW.fetchStatus[id] || 0; },

    // ── random / time ──
    random_f64: function () { return Math.random(); },
    random_int: function (min, max) {
      if (max <= min) return min;
      return min + Math.floor(Math.random() * (max - min + 1));
    },
    random_hex: function (n) {
      var out = '';
      for (var i = 0; i < Math.max(0, n); i++) out += ('0' + Math.floor(Math.random() * 256).toString(16)).slice(-2);
      return HSW.ret(out);
    },
    random_uuid_v4: function () {
      var b = [];
      for (var i = 0; i < 16; i++) b.push(Math.floor(Math.random() * 256));
      b[6] = (b[6] & 0x0f) | 0x40; b[8] = (b[8] & 0x3f) | 0x80;
      var h = b.map(function (x) { return ('0' + x.toString(16)).slice(-2); }).join('');
      return HSW.ret(h.slice(0, 8) + '-' + h.slice(8, 12) + '-' + h.slice(12, 16) + '-' + h.slice(16, 20) + '-' + h.slice(20, 32));
    },
    time_now_ms: function () { return Date.now(); },
    time_perf_now_ms: function () { return (typeof performance !== 'undefined' ? performance.now() : Date.now()); },

    // ── canvas ──
    canvas_set_size: function (h, w, ht) { var e = HSW.el(h); if (!e) return 0; e.width = w; e.height = ht; return 1; },
    canvas_clear: function (h) { var c = HSW.c2d(h); if (!c) return 0; c.clearRect(0, 0, c.canvas.width, c.canvas.height); return 1; },
    canvas_fill_style:   function (h, s) { var c = HSW.c2d(h); if (!c) return 0; c.fillStyle = HSW.str(s); return 1; },
    canvas_stroke_style: function (h, s) { var c = HSW.c2d(h); if (!c) return 0; c.strokeStyle = HSW.str(s); return 1; },
    canvas_line_width:   function (h, w) { var c = HSW.c2d(h); if (!c) return 0; c.lineWidth = w; return 1; },
    canvas_fill_rect:    function (h, x, y, w, ht) { var c = HSW.c2d(h); if (!c) return 0; c.fillRect(x, y, w, ht); return 1; },
    canvas_stroke_rect:  function (h, x, y, w, ht) { var c = HSW.c2d(h); if (!c) return 0; c.strokeRect(x, y, w, ht); return 1; },
    canvas_clear_rect:   function (h, x, y, w, ht) { var c = HSW.c2d(h); if (!c) return 0; c.clearRect(x, y, w, ht); return 1; },
    canvas_begin_path: function (h) { var c = HSW.c2d(h); if (!c) return 0; c.beginPath(); return 1; },
    canvas_move_to:    function (h, x, y) { var c = HSW.c2d(h); if (!c) return 0; c.moveTo(x, y); return 1; },
    canvas_line_to:    function (h, x, y) { var c = HSW.c2d(h); if (!c) return 0; c.lineTo(x, y); return 1; },
    canvas_arc:        function (h, x, y, r, a0, a1) { var c = HSW.c2d(h); if (!c) return 0; c.arc(x, y, r, a0, a1); return 1; },
    canvas_close_path: function (h) { var c = HSW.c2d(h); if (!c) return 0; c.closePath(); return 1; },
    canvas_fill:   function (h) { var c = HSW.c2d(h); if (!c) return 0; c.fill(); return 1; },
    canvas_stroke: function (h) { var c = HSW.c2d(h); if (!c) return 0; c.stroke(); return 1; },
    canvas_font:   function (h, s) { var c = HSW.c2d(h); if (!c) return 0; c.font = HSW.str(s); return 1; },
    canvas_fill_text: function (h, s, x, y) { var c = HSW.c2d(h); if (!c) return 0; c.fillText(HSW.str(s), x, y); return 1; }
  };

  // fetch is shared by get/post; kept on HSW so both entry points see it
  lib.$HSW.doFetch = function (method, url, body, ctype) {
    var id = HSW.nextFetch++;
    var init = { method: method };
    if (method === 'POST') { init.body = body; init.headers = { 'Content-Type': ctype || 'text/plain' }; }
    fetch(url, init).then(function (resp) {
      HSW.fetchStatus[id] = resp.status;
      return resp.text().then(function (text) {
        HSW.event(resp.ok ? HSW.KIND_FETCH_OK : HSW.KIND_FETCH_ERR, id, resp.status, text);
      });
    }).catch(function (err) {
      HSW.fetchStatus[id] = 0;
      HSW.event(HSW.KIND_FETCH_ERR, id, 0, String(err));
    });
    return id;
  };

  Object.keys(fns).forEach(function (name) {
    lib['hsharp_js_' + name] = fns[name];
    lib['hsharp_js_' + name + '__deps'] = ['$HSW'];
  });

  mergeInto(LibraryManager.library, lib);
})();
