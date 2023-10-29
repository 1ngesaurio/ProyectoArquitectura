#include <ESP32_Servo.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Familia Batz";
const char* password = "Familiabatz123";

const char* PARAM_SPEED = "speed";
Servo servo1;
Servo servo2;

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  servo1.attach(13); // Pin del primer servo (cambia según tu conexión)
  servo2.attach(14); // Pin del segundo servo (cambia según tu conexión)

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conexión exitosa.");

  // Muestra la dirección IP en el monitor serial
  Serial.print("Dirección IP del ESP32: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><head>";
    html += "<style>";
    html += "body { text-align: center; }";
    html += "h2, h3 { margin: 20px; }";
    html += "input[type='range'] { width: 80%; }";
    html += "button { margin: 10px; }";
    html += "</style>";
    html += "</head><body>";
    html += "<h2>Control de puertas</h2>";
    
    // Interfaz para controlar el primer servo
    html += "<h3>Techo</h3>";
    html += "<p>Posicion del Servo 1: <span id='servo1Val'>90</span></p>";
    html += "<input type='range' id='servo1' name='servo1' min='0' max='180' oninput='updateServo1(value)'><br>";
    
    // Interfaz para controlar el segundo servo
    html += "<h3>Puerta</h3>";
    html += "<p>Posicion del Servo 2: <span id='servo2Val'>90</span></p>";
    html += "<input type='range' id='servo2' name='servo2' min='0' max='180' oninput='updateServo2(value)'><br>";
    
    html += "<button type='button' onclick='sendServo1()'>Enviar Techo</button>";
    html += "<button type='button' onclick='sendServo2()'>Enviar Puerta</button>";
    html += "<script>function updateServo1(val) {document.getElementById('servo1Val').innerText = val;}";
    html += "function updateServo2(val) {document.getElementById('servo2Val').innerText = val;}";
    html += "function sendServo1() {var servo1 = document.getElementById('servo1').value;";
    html += "var xhr = new XMLHttpRequest(); xhr.open('GET', '/setservo1?servo1=' + servo1, true); xhr.send();}";
    html += "function sendServo2() {var servo2 = document.getElementById('servo2').value;";
    html += "var xhr = new XMLHttpRequest(); xhr.open('GET', '/setservo2?servo2=' + servo2, true); xhr.send();}</script>";
    
    html += "</body></html>";
    request->send(200, "text/html", html);
  });

  server.on("/setservo1", HTTP_GET, [](AsyncWebServerRequest *request){
    String servo1Pos = request->arg("servo1");
    int servo1Position = servo1Pos.toInt();
    servo1.write(servo1Position);
    request->send(200, "text/plain", "Posición del Servo 1 actualizada a " + servo1Pos);
  });

  server.on("/setservo2", HTTP_GET, [](AsyncWebServerRequest *request){
    String servo2Pos = request->arg("servo2");
    int servo2Position = servo2Pos.toInt();
    servo2.write(servo2Position);
    request->send(200, "text/plain", "Posición del Servo 2 actualizada a " + servo2Pos);
  });

  server.begin();
}

void loop() {
  // Tu código loop aquí
}
