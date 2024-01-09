#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>


const char* ssid = "Esp32"; // Nombre de la red Wi-Fi del punto de acceso
const char* password = "Tonatiuh"; // Contraseña de la red del punto de acceso
<<<<<<< HEAD
=======

>>>>>>> 6f351563ee280edf08666757c04f070506ffc6fb
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

    if(!SPIFFS.begin(true)){
    Serial.println("Error al montar SPIFFS");
    return;
    }
    
    if (server.hasHeader("ContentType")) {
      String contentType = server.header("ContentType");
      Serial.print("Content-Type received: ");
      Serial.println(contentType);
    } 
    else {
      Serial.println("No Content-Type header received.");
    }   
  
   if (server.hasArg("file")) {
    // Obtiene el archivo cargado
    File file = SPIFFS.open("/image.jpeg", "w");
    if (!file) {
      server.send(500, "text/plain", "Error al abrir el archivo");
      return;
    }

    // Escribe los datos del archivo
    size_t contentLength = server.arg("file").length();
    for (size_t i = 0; i < contentLength; i++) {
      file.write((uint8_t)server.arg("file")[i]);
    }
    file.close();

    server.send(200, "text/plain", "Archivo recibido y guardado");
  } else {
    server.send(400, "text/plain", "No se proporcionó un archivo");
  }
  });

  server.begin();
<<<<<<< HEAD
  Serial.println(WiFi.softAPIP());
  Serial.println(WiFi.localIP());
=======

  Serial.println(WiFi.softAPIP());

  Serial.println(WiFi.localIP());

>>>>>>> 6f351563ee280edf08666757c04f070506ffc6fb
}

void loop() {

  server.handleClient();

}
