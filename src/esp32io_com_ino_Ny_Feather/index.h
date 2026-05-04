/*
 * Styled web interface for ESP32 car controller
 */

const char *HTML_CONTENT = R"=====(
<!DOCTYPE html>
<html>
<head>
<title>ESP32 Car Controller</title>
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
<style>
  *, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }

  body {
    font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif;
    background: #0f0f0f;
    color: #e0e0e0;
    min-height: 100vh;
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    gap: 1.5rem;
    padding: 2rem 1rem;
  }

  h2 {
    font-size: 20px;
    font-weight: 500;
    color: #ffffff;
    letter-spacing: -0.01em;
    text-align: center;
  }

  .status-bar {
    display: flex;
    align-items: center;
    gap: 8px;
    font-size: 13px;
    color: #888;
  }

  .ws-dot {
    width: 8px;
    height: 8px;
    border-radius: 50%;
    background: #E24B4A;
    display: inline-block;
    transition: background 0.3s;
  }

  .ws-dot.open { background: #639922; }

  #ws_state {
    font-weight: 500;
    color: #ccc;
  }

  .grid {
    display: grid;
    grid-template-columns: repeat(3, 100px);
    grid-template-rows: repeat(3, 100px);
    gap: 8px;
  }

  .btn {
    border-radius: 10px;
    border: 1px solid #2a2a2a;
    background: #1a1a1a;
    cursor: pointer;
    display: flex;
    align-items: center;
    justify-content: center;
    font-size: 30px;
    color: #e0e0e0;
    transition: background 0.1s, transform 0.1s, border-color 0.1s;
    user-select: none;
    -webkit-user-select: none;
    touch-action: none;
  }

  .btn:active, .btn.active {
    background: #2a2a2a;
    border-color: #444;
    transform: scale(0.93);
  }

  .btn-stop {
    font-size: 13px;
    font-weight: 600;
    letter-spacing: 0.05em;
    color: #888;
    background: #161616;
  }

  .btn-stop.active {
    background: #2a0a0a;
    border-color: #7a2020;
    color: #e05555;
  }

  .empty { visibility: hidden; }

  .connect-btn {
    padding: 10px 32px;
    border-radius: 10px;
    border: 1px solid #2a2a2a;
    background: #1a1a1a;
    font-size: 14px;
    font-weight: 500;
    color: #ccc;
    cursor: pointer;
    font-family: inherit;
    transition: background 0.15s, border-color 0.15s;
  }

  .connect-btn:hover { background: #222; }

  .connect-btn.connected {
    border-color: #3B6D11;
    color: #7bc142;
  }

  .credit {
    font-size: 12px;
    color: #444;
  }
</style>
</head>
<body>

<h2>ESP32 Car Controller</h2>

<div class="status-bar">
  <span class="ws-dot" id="ws_dot"></span>
  WebSocket: <span id="ws_state">closed</span>
</div>

<div class="grid">
  <div class="empty"></div>
  <div class="btn" id="btn-fwd">&#9650;</div>
  <div class="empty"></div>
  <div class="btn" id="btn-left">&#9668;</div>
  <div class="btn btn-stop" id="btn-stop">STOP</div>
  <div class="btn" id="btn-right">&#9658;</div>
  <div class="empty"></div>
  <div class="btn" id="btn-bwd">&#9660;</div>
  <div class="empty"></div>
</div>

<button class="connect-btn" id="wc_conn" onclick="wc_onclick()">Connect</button>

<script>
var CMD_STOP=0, CMD_FORWARD=1, CMD_BACKWARD=2, CMD_LEFT=4, CMD_RIGHT=8;
var ws = null;

function send_command(cmd) {
  if (ws && ws.readyState === 1) ws.send(cmd + "\r\n");
}

function bindBtn(el, cmd) {
  ['mousedown','touchstart'].forEach(function(e) {
    el.addEventListener(e, function(ev) {
      ev.preventDefault();
      el.classList.add('active');
      send_command(cmd);
    });
  });
  ['mouseup','mouseleave','touchend','touchcancel'].forEach(function(e) {
    el.addEventListener(e, function(ev) {
      ev.preventDefault();
      el.classList.remove('active');
      send_command(CMD_STOP);
    });
  });
}

bindBtn(document.getElementById('btn-fwd'),  CMD_FORWARD);
bindBtn(document.getElementById('btn-bwd'),  CMD_BACKWARD);
bindBtn(document.getElementById('btn-left'), CMD_LEFT);
bindBtn(document.getElementById('btn-right'),CMD_RIGHT);
bindBtn(document.getElementById('btn-stop'), CMD_STOP);

function ws_onopen() {
  document.getElementById('ws_state').textContent = 'open';
  document.getElementById('ws_dot').classList.add('open');
  document.getElementById('wc_conn').textContent = 'Disconnect';
  document.getElementById('wc_conn').classList.add('connected');
}

function ws_onclose() {
  document.getElementById('ws_state').textContent = 'closed';
  document.getElementById('ws_dot').classList.remove('open');
  document.getElementById('wc_conn').textContent = 'Connect';
  document.getElementById('wc_conn').classList.remove('connected');
  ws = null;
}

function wc_onclick() {
  if (!ws) {
    ws = new WebSocket("ws://" + window.location.host + ":81");
    document.getElementById('ws_state').textContent = 'connecting...';
    ws.onopen = ws_onopen;
    ws.onclose = ws_onclose;
  } else {
    ws.close();
  }
}
</script>
</body>
</html>
)=====";