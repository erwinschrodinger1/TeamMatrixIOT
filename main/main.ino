#include <WiFi.h>
#include <HTTPClient.h>;
#include <ArduinoJson.h>;

const char* ssid = "DESKTOP 5160";
const char* password = "12345678";

char jsonOutput[128];

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  

  
}

void loop(){
  if((WiFi.status()==WL_CONNECTED)){
    Serial.println("Connected");
    HTTPClient client;
    client.begin("http://68.183.129.143:5000/api/temp");
    client.addHeader("Content-Type", "application/json");
    const size_t CAPACITY = JSON_OBJECT_SIZE(1);
    StaticJsonDocument<CAPACITY> doc;
    JsonObject object = doc.to<JsonObject>();
    object["cowID"] = "jaslkdj";
   
    serializeJson(doc, jsonOutput);
    Serial.println("doc is "+ String(jsonOutput));
    
    int httpCode = client.POST(String(jsonOutput));
    if(httpCode>0){
      String payload = client.getString();
      Serial.println("Status Code"+String(httpCode));
      Serial.println("Payload is: ");
      Serial.println(payload);
      client.end();
    }
    delay(1000);
  }
  else{
    Serial.println("Now connection");
  }

  delay(10000);
}
