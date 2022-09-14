#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>;
#include <ArduinoJson.h>;
const char* ssid = "DESKTOP 5160";
const char* password = "12345678";




#define ONE_WIRE_BUS 12

OneWire oneWire(ONE_WIRE_BUS);  
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DallasTemperature sensors(&oneWire);

void setup(void)
{
  sensors.begin();  // Start up the library
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while(WiFi.status()!=WL_CONNECTED){
  Serial.print(".");
  delay(500);


}

void loop(void)
{ 
  // Send the command to get temperatures
  sensors.requestTemperatures(); 
  sensor_value = sensors.getTempCByIndex(0);
  //print the temperature in Celsius
  Serial.print("Temperature: ");
  Serial.print(sensor_value);Serial.print('\r');
  Serial.print(" Degrees ");//shows degrees character
  Serial.print("C  |  ");
  
  //print the temperature in Fahrenheit
  Serial.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);Serial.print('\r');
  Serial.print(" Degrees ");//shows degrees character
  Serial.println("F");

  lcd.setCursor(0,0);
  lcd.print("Celcius =  ");lcd.print(sensors.getTempCByIndex(0));lcd.print(" deg C");
  lcd.setCursor(0,1);
  lcd.print("Farenh  =  ");lcd.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
  if((WiFi.status()==WL_CONNECTED)){
    Serial.println("Connected");
    HTTPClient client;
    client.begin("http://68.183.129.143:5000/api/temp");
    client.addHeader("Content-Type", "application/json");
    const size_t CAPACITY = JSON_OBJECT_SIZE(1);
    StaticJsonDocument<CAPACITY> doc;
    JsonObject object = doc.to<JsonObject>();
    object["cowID"] = "jaslkdj";
    object["Temperature"] = sensor_value;
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
  
}
