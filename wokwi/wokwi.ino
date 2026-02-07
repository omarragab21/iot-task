#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT22

// Wokwi credentials
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// REPLACE THESE WITH YOUR ACTUAL VALUES
const String baseUrl = "YOUR_PUBLIC_URL_HERE"; // e.g., "https://xyz.ngrok-free.app"
const String postPath = "/readings";         // e.g., "/readings"

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Wait 3 seconds
  delay(3000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Check WiFi connection status
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    
    // Construct full URL
    String fullUrl = baseUrl + postPath;
    http.begin(fullUrl);
    http.addHeader("Content-Type", "application/json");

    // Manually construct JSON payload
    // Payload: {"sensor":"dht22","temp":25.4,"humidity":61.2}
    String payload = "{\"sensor\":\"dht22\",\"temp\":";
    payload += String(t);
    payload += ",\"humidity\":";
    payload += String(h);
    payload += "}";

    Serial.print("Sending to " + fullUrl + " | Payload: ");
    Serial.println(payload);

    int httpResponseCode = http.POST(payload);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(response);
    } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
    WiFi.reconnect();
  }
}
