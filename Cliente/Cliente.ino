
#include "esp_camera.h"
#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h> 
#include "Pines.h"

#define CAMERA_MODEL_AI_THINKER

const char* ssid = "Totalplay-7E9D";
const char* password = "7E9DE6FE6Xm3qJjn";
const char* server_ip = "192.168.0.1";
const int server_port = 80;

//static const char *TAG = "camera_example";

void setup() {

  Serial.begin(115200);

  if(!SPIFFS.begin(true)){
    Serial.println("Error al montar SPIFFS");
    return;
  }

  // Conéctate a la red WiFi
  //WiFi.begin(ssid, password);
  //while (WiFi.status() != WL_CONNECTED) {
   // delay(1000);
   // Serial.println("Conectando a WiFi...");
  //}
  //Serial.println("Conectado a la red WiFi");

  // Configura la cámara
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_VGA;
  //config.pixel_format = PIXFORMAT_JPEG; // for streaming
  config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // Inicializa la cámara
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Error al inicializar la cámara (%d)", err);
    return;
  }

}

void loop() {
  if (capturarFotoRemotamente()){
    Serial.println("Foto capturada y guardada como 'foto.bmp'");
  } else {
    Serial.println("Error al capturar la foto");
  }

  // Espera 5 segundos antes de tomar otra foto
  delay(10000);
  }

bool capturarFotoRemotamente() {
  HTTPClient http;

  // URL del servidor
  String url = "http://" + String(server_ip) + ":" + String(server_port) + "/tomar-foto";

  // Realiza la solicitud HTTP POST al servidor
  int httpResponseCode = http.POST(url);

  if (httpResponseCode == HTTP_CODE_OK) {
    // La orden se envió correctamente, captura la foto
    camera_fb_t* fb = esp_camera_fb_get();

    if (!fb) {
      Serial.println("Error al capturar el frame de la cámara");
      return false;
    }

    // Guarda la foto en formato BMP
    fs::File file = SPIFFS.open("/foto.bmp", FILE_WRITE);
    if (!file) {
      Serial.println("Error al abrir el archivo");
      esp_camera_fb_return(fb);
      return false;
    }

    file.write((const uint8_t*)fb->buf, fb->len);
    file.close();

    // Libera la memoria utilizada por el búfer de la cámara
    esp_camera_fb_return(fb);

    return true;
  } else {
    Serial.printf("Error en la solicitud HTTP: %d\n", httpResponseCode);
    return false;
  }
}