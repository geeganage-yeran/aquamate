#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ESP32Servo.h>

// Firebase configuration
#define FIREBASE_HOST "https://aquamate-4ad2a-default-rtdb.firebaseio.com"  // Firebase URL
#define FIREBASE_AUTH "a3jsXLBsTyrlTYzwansn3Ec6SvGqhfGtUFUCB5jO"           // Firebase Auth Secret

#define SERVO_PIN 13  // GPIO pin connected to the servo signal wire
Servo myServo;        // Create a Servo object

// Wi-Fi credentials
const char* ssid = "CodeDev";
const char* password = "yeran124";

// Firebase instances
FirebaseData firebaseData;
FirebaseConfig firebaseConfig;  // FirebaseConfig object
FirebaseAuth firebaseAuth;      // FirebaseAuth object

void setup() {
  Serial.begin(9600);
  // Clear any leftover data in the buffer
  while (Serial.available() > 0) {
    Serial.read();  // Clear the buffer
  }

    // Attach the servo to the specified pin
  myServo.attach(SERVO_PIN);
  myServo.write(0);  // Set the servo to 0° initially

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Set up Firebase configuration
  firebaseConfig.database_url = FIREBASE_HOST;  // Use database_url instead of host
  firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;  // Use legacy_token instead of api_key

  // Initialize Firebase with the FirebaseConfig and FirebaseAuth objects
  Firebase.begin(&firebaseConfig, &firebaseAuth);
  Firebase.reconnectWiFi(true);

  // Check Firebase connection
  if (Firebase.ready()) {
    Serial.println("Firebase connected successfully!");
  } else {
    Serial.println("Failed to connect to Firebase!");
    Serial.println("Error Reason: " + firebaseData.errorReason());
    Serial.println("Error Code: " + String(firebaseData.errorCode()));
    return; // Stop further execution if Firebase connection fails
  }


}

void loop() {
    // Fetch current status for feed and light (before we send any new data)
  fetchStatus();
  
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');  // Read the incoming data
    Serial.print("Received data: ");
    Serial.println(data);  // Print the raw data received from Serial

    int firstCommaIndex = data.indexOf(',');
    int secondCommaIndex = data.indexOf(',', firstCommaIndex + 1);

    String tdsString = data.substring(0, firstCommaIndex);
    String tempString = data.substring(firstCommaIndex + 1, secondCommaIndex);
    String turbidityString = data.substring(secondCommaIndex + 1);

    float tds = tdsString.toFloat();
    float temperature = tempString.toFloat();
    float turbidity = turbidityString.toFloat();

    // Print the parsed sensor values
    Serial.println("Parsed Sensor Values:");
    Serial.print("TDS: ");
    Serial.println(tds);
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Turbidity: ");
    Serial.println(turbidity);

    // Send this data to Firebase
    updateSensorData(tds, temperature, turbidity);
  }
  delay(5000);
}

void fetchStatus() {
  // Fetch current status for feed and light
  if (Firebase.getInt(firebaseData, "/aquarium/status/feedStatus")) {
    if (firebaseData.dataType() == "int") {
      int feedStatus = firebaseData.intData();  // Get the integer data (0 or 1)
      Serial.print("Current Feed Status: ");
      Serial.println(feedStatus == 1 ? "ON" : "OFF");

      // Control the servo based on feedStatus
      if (feedStatus == 1) {
        myServo.write(90);  // Rotate the servo to 90°
        Serial.println("Servo rotated to 90°");
      } else {
        myServo.write(0);   // Rotate the servo back to 0°
        Serial.println("Servo returned to 0°");
      }
    }
  } else {
    Serial.println("Failed to fetch feed status: " + firebaseData.errorReason());
    Serial.println("Error Code: " + String(firebaseData.errorCode()));
  }

  if (Firebase.getInt(firebaseData, "/aquarium/status/lightStatus")) {
    if (firebaseData.dataType() == "int") {
      int lightStatus = firebaseData.intData();  // Get the integer data (0 or 1)
      Serial.print("Current Light Status: ");
      Serial.println(lightStatus == 1 ? "ON" : "OFF");
    }
  } else {
    Serial.println("Failed to fetch light status: " + firebaseData.errorReason());
    Serial.println("Error Code: " + String(firebaseData.errorCode()));
  }
}

void updateSensorData(float tds, float temperature, float turbidity) {
  // Update Firebase with the sensor data
  if (Firebase.setFloat(firebaseData, "/aquarium/status/tds", tds)) {
    Serial.println("TDS data updated to Firebase");
  } else {
    Serial.println("Failed to update TDS data: " + firebaseData.errorReason());
    Serial.println("Error Code: " + String(firebaseData.errorCode()));
  }

  if (Firebase.setFloat(firebaseData, "/aquarium/status/temperature", temperature)) {
    Serial.println("Temperature data updated to Firebase");
  } else {
    Serial.println("Failed to update temperature data: " + firebaseData.errorReason());
    Serial.println("Error Code: " + String(firebaseData.errorCode()));
  }

  if (Firebase.setFloat(firebaseData, "/aquarium/status/turbidity", turbidity)) {
    Serial.println("Turbidity data updated to Firebase");
  } else {
    Serial.println("Failed to update turbidity data: " + firebaseData.errorReason());
    Serial.println("Error Code: " + String(firebaseData.errorCode()));
  }
}