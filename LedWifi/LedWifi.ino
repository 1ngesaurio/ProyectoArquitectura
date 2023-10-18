#include <WiFi.h>
#include <DHT.h>

const char* ssid = "Familia Batz";         // Cambia por tu SSID
const char* password = "Familiabatz123";   // Cambia por tu contraseña

const int ledPin1 = 2;   // Primer LED
const int ledPin2 = 4;   // Segundo LED
const int ledPin3 = 5;   // Tercer LED
const int ledPin4 = 18;  // Cuarto LED
const int ledPin5 = 19;  // Quinto LED

const int dhtPin = 14;   // Pin al que está conectado el sensor DHT11

WiFiServer server(80);
DHT dht(dhtPin, DHT11);

float humidity = 0.0;
float temperature = 0.0;

bool ledState1 = false;
bool ledState2 = false;
bool ledState3 = false;
bool ledState4 = false;
bool ledState5 = false;

String getLEDStateText(int ledNumber) {
  switch (ledNumber) {
    case 1:
      return ledState1 ? "Encendido" : "Apagado";
    case 2:
      return ledState2 ? "Encendido" : "Apagado";
    case 3:
      return ledState3 ? "Encendido" : "Apagado";
    case 4:
      return ledState4 ? "Encendido" : "Apagado";
    case 5:
      return ledState5 ? "Encendido" : "Apagado";
    default:
      return "Desconocido";
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a la red Wi-Fi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledPin5, OUTPUT);

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);
  digitalWrite(ledPin5, LOW);

  dht.begin();

  server.begin();
}

void updateSensorData() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("Cliente conectado");
  
  String request = client.readStringUntil('\r');
  client.flush();

  if (request.indexOf("/encender1") != -1) {
    digitalWrite(ledPin1, HIGH);
    ledState1 = true;
  } else if (request.indexOf("/apagar1") != -1) {
    digitalWrite(ledPin1, LOW);
    ledState1 = false;
  }

  if (request.indexOf("/encender2") != -1) {
    digitalWrite(ledPin2, HIGH);
    ledState2 = true;
  } else if (request.indexOf("/apagar2") != -1) {
    digitalWrite(ledPin2, LOW);
    ledState2 = false;
  }

  if (request.indexOf("/encender3") != -1) {
    digitalWrite(ledPin3, HIGH);
    ledState3 = true;
  } else if (request.indexOf("/apagar3") != -1) {
    digitalWrite(ledPin3, LOW);
    ledState3 = false;
  }

  if (request.indexOf("/encender4") != -1) {
    digitalWrite(ledPin4, HIGH);
    ledState4 = true;
  } else if (request.indexOf("/apagar4") != -1) {
    digitalWrite(ledPin4, LOW);
    ledState4 = false;
  }

  if (request.indexOf("/encender5") != -1) {
    digitalWrite(ledPin5, HIGH);
    ledState5 = true;
  } else if (request.indexOf("/apagar5") != -1) {
    digitalWrite(ledPin5, LOW);
    ledState5 = false;
  }

  updateSensorData();

  String response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n";
  response += "<html><head>";
  response += "<meta charset=\"UTF-8\">";  // Especifica la codificación de caracteres UTF-8
  response += "<style>";
  response += "body { font-family: Arial, sans-serif; text-align: center; }";
  response += "h1 { color: #333; }";
  response += "button { padding: 10px 20px; font-size: 18px; margin: 10px; border-radius: 25px; }";
  response += ".encender { background-color: green; color: white; }";
  response += ".apagar { background-color: red; color: white; }";
  response += ".estado { font-size: 20px; margin-top: 10px; }";
  response += ".progress-bar { width: 200px; background-color: #f3f3f3; border: 1px solid #ccc; border-radius: 5px; display: inline-block; margin-top: 10px; }";
  response += ".progress-bar .progress { width: 0; height: 20px; border-radius: 5px; text-align: right; }";
  response += ".progress-bar .progress .value { padding-right: 5px; color: #fff; }";  // Color del texto
  response += ".progress-bar .progress.temperature { background-color: #3498db; }";  // Color de la barra para temperatura
  response += ".progress-bar .progress.humidity { background-color: #e74c3c; }";  // Color de la barra para humedad
  response += "</style>";
  response += "</head><body>";
  response += "<h1>Control de Luces</h1>";

  // Encender/Apagar LED y mostrar estado
  for (int i = 1; i <= 5; i++) {
    response += "<div>";
    response += "<button class=\"encender\" onclick=\"encenderLED(" + String(i) + ")\">Encender Luz " + String(i) + "</button>";
    response += "<button class=\"apagar\" onclick=\"apagarLED(" + String(i) + ")\">Apagar Luz " + String(i) + "</button>";
    response += "<div id=\"estado" + String(i) + "\" class=\"estado\">Estado: " + getLEDStateText(i) + "</div>";
    response += "</div>";
  }

  // Mostrar datos de sensor y barras de progreso
  response += "<div id=\"dhtInfo\">";
  response += "<h1>Temperatura y humedad de la casa</h1>";
  response += "<h2>Temperatura</h2>";
  response += "<div class=\"progress-bar\"><div class=\"progress temperature\" style=\"width: " + String(temperature) + "%\"><span class=\"value\">" + String(temperature) + "</span>°C</div></div>";
  response += "<h2>Humedad</h2>";
  response += "<div class=\"progress-bar\"><div class=\"progress humidity\" style=\"width: " + String(humidity) + "%\"><span class=\"value\">" + String(humidity) + "</span>%</div></div>";
  response += "</div>";
  
  response += "<script>";
  response += "function encenderLED(led) {";
  response += "  var xhttp = new XMLHttpRequest();";
  response += "  xhttp.open(\"GET\", \"/encender\" + led, true);";
  response += "  xhttp.send();";
  response += "  updateLEDState(led, true);";
  response += "}";
  
  response += "function apagarLED(led) {";
  response += "  var xhttp = new XMLHttpRequest();";
  response += "  xhttp.open(\"GET\", \"/apagar\" + led, true);";
  response += "  xhttp.send();";
  response += "  updateLEDState(led, false);";
  response += "}";
  
  response += "function updateLEDState(led, state) {";
  response += "  var element = document.getElementById('estado' + led);";
  response += "  element.innerHTML = 'Estado: ' + (state ? 'Encendido' : 'Apagado');";
  response += "}";
  response += "setInterval(function() {";
  response += "  var xhttp = new XMLHttpRequest();";
  response += "  xhttp.onreadystatechange = function() {";
  response += "    if (this.readyState == 4 && this.status == 200) {";
  response += "      var data = JSON.parse(this.responseText);";
  response += "      document.querySelector('.temperature .progress').style.width = data.temperature + '%';";
  response += "      document.querySelector('.temperature .value').innerHTML = data.temperature;";
  response += "      document.querySelector('.humidity .progress').style.width = data.humidity + '%';";
  response += "      document.querySelector('.humidity .value').innerHTML = data.humidity;";
  response += "    }";
  response += "  };";
  response += "  xhttp.open('GET', '/data', true);";
  response += "  xhttp.send();";
  response += "}, 5000);";  // Actualizar cada 5 segundos
  response += "</script>";
  response += "</body></html>";

  client.print(response);
  delay(1);
  Serial.println("Cliente desconectado");
}
