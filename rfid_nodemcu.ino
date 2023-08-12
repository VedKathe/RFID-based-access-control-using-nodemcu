
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = D3;  // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;   // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class
MFRC522::MIFARE_Key key;

int flag = 0;
String tag;
bool accessDenied = false;

//funtion to check if the id is registed or not
bool check_id(String tag) {
  WiFiClient client;
  HTTPClient http;
  http.begin(client, "http://192.168.0.110:3000/users"); // change link with your api link and hostname/ip address
  int httpResponseCode = http.GET();

  if (httpResponseCode == HTTP_CODE_OK) {
    // Parse the JSON response
    String response = http.getString();
    Serial.println("Response data:");
    Serial.println(response);

    // Parse the JSON array
    const size_t capacity = JSON_ARRAY_SIZE(10);  // Change 10 to the expected size of the array
    StaticJsonDocument<capacity> jsonDoc;
    DeserializationError jsonError = deserializeJson(jsonDoc, response);

    if (jsonError) {
      Serial.print("JSON parsing error: ");
      Serial.println(jsonError.c_str());
    } else {
      // Assuming the JSON response is an array of strings
      JsonArray jsonArray = jsonDoc.as<JsonArray>();
      for (JsonVariant value : jsonArray) {
        if (tag == value.as<String>()) {
          return true;
        }
        Serial.println(value.as<String>());
      }
    }
  } else {
    Serial.print("HTTP GET request failed, error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  return false;
}

//funtion to send post request for storing entries into database
void send_data(String tag) {
  WiFiClient client;
  HTTPClient http;
  // Create a JSON document
  StaticJsonDocument<512> data;
  data["id"] = tag;

  // Serialize the JSON object to a string
  String jsonStringData;
  serializeJson(data, jsonStringData);

  // Make a POST request
  http.begin(client, "http://192.168.0.110:3000/setusers"); // change link with your api link and hostname/ip address
  http.addHeader("Content-Type", "application/json");  // Set the content type to JSON
  int httpResponseCode = http.POST(jsonStringData);    // Send the JSON data in the POST request
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
  http.end();
}

void setup() {
  WiFi.begin("Kathe M E", "mh15ds1241");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.begin(115200);
  SPI.begin();          // Init SPI bus
  rfid.PCD_Init();      // Init MFRC522
  pinMode(D8, OUTPUT);  // Green Led
  pinMode(D1, OUTPUT);  // Red led and buzzer
  pinMode(D0, INPUT);   // pushdown button
}

void loop() {
  accessDenied = false;


  if (!rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }

    Serial.println(tag);
    // check access
    if (check_id(tag)) {
      Serial.println("Access Granted!");
      digitalWrite(D8, HIGH);
      delay(500);
      digitalWrite(D8, LOW);
      delay(500);
      digitalWrite(D8, HIGH);
      delay(500);
      digitalWrite(D8, LOW);
      delay(500);
      digitalWrite(D8, HIGH);
      delay(500);
      digitalWrite(D8, LOW);
      delay(500);
      send_data(tag); // store entry
      accessDenied = false;  // Reset the accessDenied flag
    } else {
      Serial.println("Access Denied!");
      //logic to continusously blink red led and buzzer until button is clicked
      if (!accessDenied) {
        accessDenied = true;  // Set the accessDenied flag to true
        while (digitalRead(D0) == LOW) {
          digitalWrite(D1, HIGH);
          delay(100);
          digitalWrite(D1, LOW);
          delay(100);
        }
        digitalWrite(D1, LOW);  // Turn off the buzzer when the button is pressed
      }
    }
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}
