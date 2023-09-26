#include <Arduino.h>
#include <ArduinoJson.hpp>
#include <HTTPClient.h>
#include <WiFi.h>

const char* ssid     = "WH-iot"; // Change this to your WiFi SSID
const char* password = "wh213215"; // Change this to your WiFi password

int analogPin = GPIO_NUM_34; // potentiometer wiper (middle terminal) connected to analog pin 3
                    // outside leads to ground and +5V
int val = 0;  // variable to store the value read

using namespace ArduinoJson::ARDUINOJSON_VERSION_NAMESPACE;

void setup() {
    Serial.begin(115200);
    // pinMode(analogPin, GPIO_MODE_INPUT);
    while(!Serial){delay(100);}

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println("******************************************************");
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop(){
    val = analogRead(analogPin);  // read the input pin
    Serial.println(val);  
    delay(250);

    HTTPClient http;
    http.begin("http://192.168.2.142:8080");
    const int capacity = JSON_OBJECT_SIZE(2);
    StaticJsonDocument<capacity> doc;
    doc["value"].set(val);
    char output[127];
    auto len = serializeJson(doc, output);
    http.POST(reinterpret_cast<uint8_t*>(output), len);
    delay(999);
    ESP_LOGI("main", "output: %s", output);
}

extern "C" void app_main(void){
    initArduino();
    setup();
    while (true){
        loop();
    }
}

