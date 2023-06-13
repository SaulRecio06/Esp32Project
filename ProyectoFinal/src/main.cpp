#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <ESP32Servo.h> 
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <token.h>
#include <string>
Servo myservo;  // crea el objeto servo

// GPIO de salida del servo
static const int servoPin = 13;
int dhtPin = 26;
DHT dht(dhtPin, DHT11);

boolean estadoLed1 = false;

WiFiClientSecure client;

UniversalTelegramBot bot(BOTtoken, client);


void bot_setup()
{
   const String commands = F("["
                            "{\"command\":\"Ayuda\",  \"description\":\"Despliega las opciones del Bot.\"},"
                            "{\"command\":\"Comida\", \"description\":\"Activa el dispensador de comida\"},"
                            "{\"command\":\"Temperatura\",\"description\":\"Despliega la temperatura\"}" // no comma on last command
                            "]");
  bot.setMyCommands(commands);

}


void handleNewMessages(int numNewMessages) {
  

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
   
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Usuario no autorizado", "");
      continue;
    }
    String text = bot.messages[i].text;
    if (text == "/Comida") {
      bot.sendMessage(chat_id, "Alimentando", "");
      myservo.write(90);
      digitalWrite(led1,HIGH);             
      delay(500);                       
      myservo.write(0);
      digitalWrite(led1,LOW);             

    }

     if (text == "/Temperatura") {
      String temperatura ="";
        float temp = dht.readTemperature();
    float humidity = dht.readHumidity();
    temperatura+="Temperatura: "+String(temp)+"C° \n";
    temperatura+="Humedad: "+String(humidity)+"% \n";
      bot.sendMessage(chat_id, temperatura, "");    
    }
 
    if (text == "/Ayuda") {
      String keyboardJson = F("[[{ \"text\" : \"Comida\", \"callback_data\" : \"/Comida\" },{ \"text\" : \"Temperatura\", \"callback_data\" : \"/Temperatura\" }],");

        keyboardJson += F("[{ \"text\" : \"Ayuda\", \"callback_data\" : \"/Ayuda\" }]]");
      String ayuda ="Bienvenido al sistema de control con Esp32 " ".\n";
      String keyboardJson2 = F("[[{ \"text\" : \"/Comida\", \"callback_data\" : \"/Comida\" },{ \"text\" : \"/Temperatura\", \"callback_data\" : \"/Temperatura\" }],");
        keyboardJson2 += F("[ { \"text\" : \"/Ayuda\", \"callback_data\" : \"/Ayuda\" }]]");
      ayuda += "Estas son tus opciones.\n\n";
      ayuda += "/Comida : Activa el dispensador de comida.\n";
      ayuda += "/Temperatura : Muestra la Temperatura.\n";
      ayuda += "/Ayuda : Despliega las opciones del Bot.\n";
      // bot.sendMessageWithInlineKeyboard(chat_id, ayuda, "",keyboardJson);
      bot.sendMessageWithReplyKeyboard(chat_id,ayuda, "",keyboardJson2);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(led1,OUTPUT);
  myservo.attach(servoPin);  // vincula el servo en el servoPin

  // Conecta a red WiFi con SSID y password
  Serial.print("Conectado a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Muestra IP local 
  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  bot_setup();
  bot.sendMessage(CHAT_ID, "Presione o Escriba /Ayuda para empezar", "");
}

void loop() {
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

  while(numNewMessages) {
    handleNewMessages(numNewMessages);
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }
}