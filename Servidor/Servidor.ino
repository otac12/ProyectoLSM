//ESP32 que funge como servidor

#include <WiFi.h>
#include <WebServer.h>

// Se crea una red propia -> SSID y Password

const char* SSID = "Maestro";
const char* PASS = "Master123"; //Opcional

// Se declaran los parametros para crear una IP fija

IPAddress ip (192,168,4,1);
IPAddress gateway (192,168,4,2);
IPAddress subnet (255,255,255,0);

// Declaramos un objeto de la libreria WebServer para poder acceder a sus funciones
// Y como parametro 80, que es el puerto estandar de todos los servicios WEB HTTP

WebServer Server(80);

void setup() {
  // Iniciamos el monitor Serial

  Serial.begin(115200);
  delay(50);

  // Creamos el punto de acceso
  WiFi.softAP(SSID,PASS); // Tiene mas parametros opcionales

  // Para crear una IP fija utilizamos 
  WiFi.softAPConfig(ip, gateway, subnet);

  // Para utilizar la IP que viene por defecto utilizamos
  // IPAddress ip = WiFi.softAPIP();

  Serial.print("Nombre de la red ESP32: ");
  Serial.println(SSID);
  Serial.print("La IP es: ");
  Serial.println(ip);

  // Declarar los dispositivos que se pueden conectar al servidor

  Server.on("/", handleConnectionRoot);
  Server.on("/device1", handleDevice1);
  Server.on("/device2", handleDevice2);
  Server.on("/device3", handleDevice3);
  Server.onNotFound(handleNotFound);

  // Inicializar el servidor

  Server.begin();
  Serial.println("El servidor HTTP iniciado");
  Serial.println("\nDispositivos Contactados: ");
}

void loop() {
  // Recibimos las peticiones de los clientes

  Server.handleClient();
}

// Creamos una variable para el dispositivo y para su respuesta

String Device = "";
String Answer = "";

void setAnswer(){
  // Cargamos un esqueleto en HTML como respuesta de conexión
  
  Answer =  "<!DOCTYPE html>\
            <html>\
            <body>\
            <h1>Bienvenido \"" + Device +"\"! </h1>\
            </body>\
            </html>";
}

// Asignamos una respuesta cuando un dispositivo se conecte a la raiz

void handleConnectionRoot(){
  // El código de respuesta de estado satisfactorio HTTP 200 OK indica que la solicitud ha tenido éxito.

  Server.send(200, "text/html", "Conexión exitosa");
}

void handleDevice1(){
  // Nombramos al Dispositivo
  Device = "ESP32-CAM-1";

  // Imprimimos el nombre para identificarlo
  Serial.println(Device);

  // Mandamos a llamar la función respuesta
  setAnswer();

  // Mandamos el código de respuesta satisfactorio
  Server.send(200, "text/html", Answer);
}

void handleDevice2(){
  // Nombramos al Dispositivo
  Device = "ESP32-CAM-2";
  
  // Imprimimos el nombre para identificarlo
  Serial.println(Device);

  // Mandamos a llamar la función respuesta
  setAnswer();

  // Mandamos el código de respuesta satisfactorio
  Server.send(200, "text/html", Answer);
}

void handleDevice3(){
  // Nombramos al Dispositivo
  Device = "Mobile";
  
  // Imprimimos el nombre para identificarlo
  Serial.println(Device);

  // Mandamos a llamar la función respuesta
  setAnswer();

  // Mandamos el código de respuesta satisfactorio
  Server.send(200, "text/html", Answer);
}

void handleNotFound(){
  // El código de respuesta de estado insatisfactorio HTTP 404 ERROR indica que la solicitud no ha tenido éxito.

  Server.send(404, "text/plain", "Conexión fallida");
}