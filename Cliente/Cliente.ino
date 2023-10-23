//ESP32S3 que funge como cliente

#include <WiFi.h>
#include <WiFiClient.h>

// Se declaran las cre/Users/tonatiuh/Desktop/Proyecto_LSM/Cliente/Cliente.inodenciales del servidor

// const char* SSID = "Totalplay-97AA";
// const char* PASS = "97AA4E1DzJ9kk5Hy";

const char *SSID = "yourAP";
const char *PASS= "yourPassword";

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println();
  Serial.println(WiFi.localIP());
}

void setup() {
  int Ip_server; // se pone la ip del servidor 
  Serial.begin(115200);
  WiFiClient cliente;
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  initWiFi();
}

void loop() {
 
  if (cliente.connect(Ip_server))
  {
    String informacion = "Informacion_imagen";  // Datos a enviar en el cuerpo del POST
    String solicitud = "POST /ruta_del_servidor HTTP/1.1\r\n" +
                             "Host: " + serverIP + "\r\n" +
                             "Content-Type: application/x-www-form-urlencoded\r\n" +
                             "Content-Length: " + String(informacion.length()) + "\r\n\r\n" +
                             informacion;
                             
    client.print(solicitud);
  }
}