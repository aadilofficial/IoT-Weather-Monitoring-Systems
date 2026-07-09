#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "DHT.h"

// ---------------- WIFI ----------------
const char* ssid = "ZTE_2.4G_7q6kHb";
const char* password = "PNfmc4Kd";

// ---------------- FIREBASE ----------------
#define API_KEY "AIzaSyAv_AuCLtPbwcZMHrbx_D2a3KwIkFE9TxE"

#define DATABASE_SECRET "q5iTnv7Vj8dd7rc9LSNIAlgYZD42PcYtwg4Q3jbE"

#define DATABASE_URL "weather-monitoring-b5ac2-default-rtdb.firebaseio.com"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// ---------------- DHT ----------------
#define DHTPIN D2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(115200);

  dht.begin();

  // WIFI
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi Connected");

  // FIREBASE CONFIG
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Use the legacy token (Database Secret) for master admin access
  config.signer.tokens.legacy_token = DATABASE_SECRET;

  // Initialize Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("Firebase Connected");
}

void loop()
{
  float humidity = dht.readHumidity();

  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println("DHT Error");
    return;
  }

  Serial.print("Humidity: ");
  Serial.println(humidity);

  Serial.print("Temperature: ");
  Serial.println(temperature);

  // Upload Humidity
  if (Firebase.RTDB.setFloat(&fbdo, "/Weather/Humidity", humidity))
  {
    Serial.println("Humidity Uploaded");
  }
  else
  {
    Serial.println(fbdo.errorReason());
  }

  // Upload Temperature
  if (Firebase.RTDB.setFloat(&fbdo, "/Weather/Temperature", temperature))
  {
    Serial.println("Temperature Uploaded");
  }
  else
  {
    Serial.println(fbdo.errorReason());
  }

  Serial.println("--------------------");

  delay(5000);
}