#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

const char* ssid = "MiRedAP"; // Nombre de la red Wi-Fi del punto de acceso
const char* password = "MiContraseñaAP"; // Contraseña de la red del punto de acceso
WebServer server(80); // Crea el servidor en el puerto 80

void setup() {
  Serial.begin(115200);

  // Conecta el ESP32 al punto de acceso (AP)
  WiFi.softAP(ssid, password);

  Serial.println("Servidor en funcionamiento en modo AP");

  server.on("/", HTTP_GET, [](){
    server.send(200, "text/plain", "¡Bienvenido al servidor ESP32!");
  });

  server.on("/upload", HTTP_POST, [](){
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Recibiendo archivo: %s\n", upload.filename.c_str());
      SPIFFS.remove(upload.filename);
      File file = SPIFFS.open(upload.filename, "w");
      if (!file) {
        Serial.println("Error al abrir el archivo para escritura");
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      Serial.printf("Escribiendo %d bytes\n", upload.currentSize);
      File file = SPIFFS.open(upload.filename, "a");
      if (file) {
        file.write(upload.buf, upload.totalSize);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      Serial.printf("Archivo recibido: %s, %u bytes\n", upload.filename.c_str(), upload.totalSize);
      server.send(200, "text/plain", "Archivo recibido");
    }
  });

  server.begin();
}

void loop() {
  server.handleClient();
  // WiFiClient client = server.available();

  // if (client) {
  //   String currentLine = "";
  //   while (client.connected()) {
  //     if (client.available()) {
  //       char c = client.read();
  //       if (c == '\n') {
  //         if (currentLine.length() == 0) {
  //           // Fin de la solicitud HTTP
  //           if (currentLine.startsWith("POST")) {
  //             // Procesa la solicitud POST aquí
  //             // Captura y procesa los datos POST del cliente
  //             // Ejemplo: busca "Content-Length" para obtener el tamaño de los datos
  //           }
  //           client.println("HTTP/1.1 200 OK");
  //           client.println("Content-Type: text/html");
  //           client.println("Connection: close");
  //           client.println();
  //           client.println("Solicitud procesada con éxito");
  //           break;
  //         } else {
  //           currentLine = "";
  //         }
  //       } else if (c != '\r') {
  //         currentLine += c;
  //       }
  //     }
  //   }
  //   client.stop();
  // }


}
