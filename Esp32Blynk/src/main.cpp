#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL58iV3dlo"
#define BLYNK_TEMPLATE_NAME "Esp32Blynk"
#define BLYNK_AUTH_TOKEN "9DjTiVtewun8J5TFJfTSJuR9oWZpGR7u"


// #define WIFI_NETWORK "NokiaSaul";
// #define WIFI_PASSWORD "123Esp32asd";

#define WIFI_NETWORK  "Totalplay-82A3"
#define WIFI_PASSWORD "82A3DC71ArDj6abQ"



void setup() {

  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN,WIFI_NETWORK,WIFI_PASSWORD,"blynk.cloud",80);
 
}

void loop() {
  Blynk.run();
}