//ESP32S3 que funge como cliente

#include <WiFi.h>

// Se declaran las credenciales del servidor

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
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  initWiFi();
}

void loop() {
  
}