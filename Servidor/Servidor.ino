/*/Users/tonatiuh/Desktop/Proyecto_LSM/Cliente/Cliente.ino
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"/Users/tonatiuh/Desktop/Proyecto_LSM/Servidor/Libreria/Funciones_nucleos.h
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "Libreria/Funciones_nucleos.h"

#define LED_BUILTIN 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Set these to your desired credentials.
const char *ssid = "yourAP";
const char *password = "yourPassword";

WiFiServer server(80);


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");
  WiFiClient Clientes[3];
  int contador=0;
  String respuesta;

  // You can remove the password parameter if you want the AP to be open.
  // a valid password must have more than 7 characters
  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while(1);
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() {

  Clientes[0] = server.available();   // Cliente 1

  // se incializa los thread para cada camara 

  xTaskCreatePinnedToCore(camara,"camara1",10000,&Clientes[0],0,&camara1,0);

  Clientes[1] = server.available();   // Cliente 2

  respuesta = Cliente[1].readStringUntil('\r');

  Serial.println(respuesta);
}