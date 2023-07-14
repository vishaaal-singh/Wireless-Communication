#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WiFiMulti.h>

const char* ssid = "Vsr";          // Replace with your network credentials
const char* password = "vishal1234";

WiFiClient client;
WiFiServer server(8888);
WiFiMulti wifiMulti;

LiquidCrystal_I2C lcd(0x27, 16, 2);    // Initialize 16x2 LCD display

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  wifiMulti.addAP(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected");
    while (client.connected()) {
      String data = "";
      while (client.available() > 0) {
        char c = client.read();
        if (c == '\n') {
          break;
        }
        data += c;
      }
      if (data != "") {
        int pos = data.indexOf(",");
        if (pos != -1) {
          float temperature = data.substring(0, pos).toFloat();
          float humidity = data.substring(pos + 1).toFloat();
          // Print temperature and humidity on LCD display
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Temp: ");
          lcd.print(temperature);
          Serial.println("Temp: ");
          Serial.print(temperature);
          Serial.println("humidity: ");
          Serial.print(humidity);
          lcd.print(" C");
          lcd.setCursor(0, 1);
         // lcd.print("Humidity: ");
         // lcd.print(humidity);
         // lcd.print(" %");
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}
