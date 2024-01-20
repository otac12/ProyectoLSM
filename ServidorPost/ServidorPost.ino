#include <ESPAsyncWebServer.h>

const char* ssid = "Totalplay-7E9D";
const char* password = "7E9DE6FE6Xm3qJjn";

AsyncWebServer server(8080);

void setup() {
  Serial.begin(115200);

  // Conéctate a la red WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a la red WiFi");

  // Muestra la dirección IP en el puerto serie
  Serial.print("Dirección IP asignada: ");
  Serial.println(WiFi.localIP());

  // Configura la ruta "/tomar-foto" para enviar la orden al cliente
  server.on("/foto", HTTP_POST, [](AsyncWebServerRequest *request) {
    // Validar si hay algún dispositivo conectado
    if (WiFi.softAPgetStationNum() > 0 || WiFi.scanComplete() == WIFI_SCAN_FAILED) {
      // Envía la orden al cliente solo si hay algún dispositivo conectado
      // Puedes personalizar esta parte según tu implementación específica
      // Por ejemplo, puedes enviar un JSON con instrucciones al cliente
      request->send(200, "text/plain", "Foto Tomada");
    } else {
      // Si no hay dispositivos conectados, envía un mensaje de error
      request->send(400, "text/plain", "No hay dispositivos conectados");
    }
  });

  // Inicia el servidor
  server.begin();
}

void loop() {
  // No es necesario realizar ninguna tarea en el bucle principal
}

