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
  </select>
  <br>
  <br>
  <input type="color" id="colorInput" value="#320000" oninput="setColor()">
  <br>
  <br>
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
</script>
</body>
</html>
)rawliteral";