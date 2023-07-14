#include <WiFi.h>
#include <WiFiClient.h>
#include <DHT.h>
#include <LiquidCrystal.h>

#define DHTPIN 14        // DHT11 data pin connected to GPIO14
#define DHTTYPE DHT11    // DHT11 sensor type

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Vsr";
const char* password = "vishal1234";
const char* receiverIP = "192,168,210,230"; // IP address of the receiver ESP32
const int receiverPort = 8888;    
uint16_t senderPort = 8888;        // Port number used by the receiver ESP32
WiFiClient client;

LiquidCrystal lcd(27, 26, 25, 33, 32, 14); // RS, EN, D4, D5, D6, D7

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2);
  lcd.print("Temp:");
  lcd.setCursor(8, 0);
  lcd.print("Humidity:");
  lcd.setCursor(0, 1);
  lcd.print("Connecting...");
  WiFi.begin("Vsr", "vishal1234");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  lcd.clear();
  lcd.print("Connected to WiFi");
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  lcd.setCursor(5, 0);
  lcd.print(temperature);
  lcd.print((char)223); // Degree symbol
  lcd.print("C");

  lcd.setCursor(17, 0);
  lcd.print(humidity);
  lcd.print("%");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("C, Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  if (client.connect("192.168.210.230", atoi("8888"))) {
    client.print("Temperature: ");
    client.print(temperature);
    //delay(3000);
    client.print("C, Humidity: ");
    client.print(humidity);
    client.println("%");
    client.stop();
    Serial.println("Data sent to receiver");
  } else {
    Serial.println("Connection to receiver failed");
  }

  delay(1000);
}
