#include <WiFi.h>
#include <HTTPClient.h>
#include "esp_camera.h"

const char* ssid = "Esp32";
const char* password = "Tonatiuh";
const char* serverAddress = "192.168.4.1"; // Reemplaza con la dirección IP del servidor en modo AP

#define CAMERA_MODEL_XIAO_ESP32S3
#include "Pines.h"

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a la red Wi-Fi...");
  }

  Serial.println("Conexión Wi-Fi establecida");

  // camara

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
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;
  
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(config.pixel_format == PIXFORMAT_JPEG){
    if(psramFound()){
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Error al inicializar la cámara: %s\n", esp_err_to_name(err));
    return;
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
        // Captura una imagen con la cámara
        camera_fb_t *fb = esp_camera_fb_get();
        if (fb) {
            WiFiClient Cliente;
            HTTPClient http;
            http.begin(Cliente,serverAddress,80, "/upload");
            // Configura la cabecera de la solicitud HTTP para el archivo JPEG
            http.addHeader("ContentType","image/jpeg");

           Serial.print(http.header("ContentType"));
            // Envía la imagen al servidor
            int httpResponseCode = http.POST(fb->buf, fb->len);

            if (httpResponseCode == 200) {
                Serial.println("Imagen enviada con éxito al servidor");
            } else {
                Serial.print("Error al enviar la imagen. Código de respuesta: ");
                Serial.println(httpResponseCode);
            }

            http.end();
            esp_camera_fb_return(fb);
        } else {
            Serial.println("Error al capturar la imagen.");
        }
    }
  
  // Espera un tiempo antes de capturar otra imagen
 
  delay(10000);
}
