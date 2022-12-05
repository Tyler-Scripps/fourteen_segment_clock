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
<script>
function setTime() {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/time?value="+new Date().toLocaleTimeString(), true);
    xhr.send();
}
</script>
</body>
</html>
)rawliteral";