const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Big Clock</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
  </style>
</head>
<body>
  <h2>Big Clock</h2>
  <button onclick="setTime()">Update Time</button>
  <br>
  <br>
  <select oninput="setMode()" id="modeSelect">
    <option value="0">Debug</option>
    <option value="1">Numeric Time</option>
    <option value="2">Message</option>
    <option value="3">Roman Numeral Time</option>
  </select>
  <br><br>
  <input type="color" id="colorInput" value="#320000" oninput="setColor()">
  <br><br>
  <label for="baseInput">Numeric Base (2-36):</label>
  <input type="number" id="baseInput" min="2" max="36" step="1" oninput="setBase()">
  <br><br>
  <label for="messageInput">Message:</label>
  <input type="text" id="messageInput" oninput="setMessage()">
  <br><br>
  <input type="checkbox" id="militaryCheckbox" oninput="setMilitary()">
  <label for="militaryCheckbox">24 Hour Time</label>
  <br><br>
  <input type="checkbox" id="wiresCheckbox" oninput="setWires()">
  <label for="wiresCheckbox">Wires on Top</label>
  <br><br>
<script>
function setTime() {
    var xhr = new XMLHttpRequest();
    const time = new Date();
    xhr.open("GET", "/time?value="+ time.getSeconds().toString().padStart(2, '0') + ',' + time.getMinutes().toString().padStart(2, '0') + ',' + time.getHours().toString().padStart(2, '0') + ',' + time.getDate().toString().padStart(2, '0') + ',' + time.getMonth().toString().padStart(2, '0') + ',' + time.getFullYear(), true);
    xhr.send();
}

function setMode() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/mode?value=" + document.getElementById("modeSelect").value, true);
  xhr.send();
}

function setColor() {
  let color = document.getElementById("colorInput").value;
  var xhr = new XMLHttpRequest();
  let requestString = "/color?red=" + Number("0x" + color.slice(1,3)) + "&green=" + Number("0x" + color.slice(3,5)) + "&blue=" + Number("0x" + color.slice(5));
  xhr.open("GET", requestString, true);
  xhr.send();
}

function setBase() {
	let base = document.getElementById("baseInput").value;
	base = Math.min(Math.max(base, 2), 36);
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/base?value=" + base, true);
    xhr.send();
}

function setMilitary() {
  let requestString = "/24h?value=";
  requestString += document.getElementById("militaryCheckbox").checked;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", requestString, true);
  xhr.send();
}

function setMessage() {
  let messageStr = "/message?value=" + document.getElementById("messageInput").value;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", messageStr, true);
  xhr.send();
}

function setWires() {
    let requestString = "/wires?value=";
    requestString += document.getElementById("wiresCheckbox").checked;
    var xhr = new XMLHttpRequest();
    xhr.open("GET", requestString, true);
    xhr.send();
}
</script>
</body>
</html>
)rawliteral";