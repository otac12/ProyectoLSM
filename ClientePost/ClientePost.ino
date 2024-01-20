
#include "esp_camera.h"
#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h> 
#include <HTTPClient.h>
#define CAMERA_MODEL_AI_THINKER
#include "Pines.h"

const char* ssid = "MiRedESP32";  // Nombre de tu red WiFi
const char* password = "ClaveSecreta";  // Contraseña de tu red WiFi
const char* esp32Server = "http://192.168.4.1/foto";

String responsePayload;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Conectando");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Conectado a la red con la IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

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
  String answer = getRequest(esp32Server);
  Serial.println("Respuesta del ESP32 Servidor:");
  Serial.println(responsePayload);
  delay(5000);

  if (capturarFotoRemotamente()){
    Serial.println("Foto capturada y guardada como 'foto.bmp'");
  } else {
    Serial.println("Error al capturar la foto");
  }

  // Espera 5 segundos antes de tomar otra foto
  delay(10000);
}

bool capturarFotoRemotamente() {
  // Realiza la solicitud HTTP GET al servidor
  responsePayload = getRequest("http://192.168.4.1/foto");

  // Verifica la respuesta del servidor
  if (!responsePayload.isEmpty()) {
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
    Serial.println("Error en la solicitud HTTP");
    return false;
  }
}

String getRequest(const char* url) {
  HTTPClient http;
  http.begin(url);

  int httpResponseCode = http.GET();
  String payload = "";

  if (httpResponseCode > 0) {
    payload = http.getString();
  } else {
    Serial.print("Error en la solicitud HTTP. Código de respuesta: ");
    Serial.println(httpResponseCode);
  }

  http.end();
  return payload;
}