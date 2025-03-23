#define RT0 10000   // Ω
#define B 3950      // K
#define VCC 5       // Supply voltage
#define R 10000     // R = 10KΩ

const int TDS_Pin = A0;  // Pin where the TDS sensor is connected
const int Temp_Pin = A1;  // Pin where the temperature sensor is connected
const int turbidityPin = A2;  // Pin where the turbidity sensor is connected

const float V_ref = 5.0;  // Reference voltage for Arduino Uno (5V)
const float calibration_factor = 368.1;  // Calibration factor for TDS

// Variables for temperature calculation
float RT, VR, ln, Temp, T0, Read;
float voltage, ntu;

void setup() {
  Serial.begin(9600);  // Start serial communication with ESP32
  pinMode(TDS_Pin, INPUT);
  pinMode(Temp_Pin, INPUT);
  pinMode(turbidityPin, INPUT);

  T0 = 25 + 273.15;  // Temperature T0 from datasheet, conversion from Celsius to Kelvin
}

void loop() {
  // 1. TDS Sensor Reading
  int sensorValue = analogRead(TDS_Pin);  // Read the analog value from the TDS sensor
  float voltageTDS = (sensorValue / 1023.0) * V_ref;  // Convert to voltage (for 5V system)
  float TDS_ppm = voltageTDS * calibration_factor;  // Convert voltage to TDS (ppm)

  // 2. Temperature Sensor Reading
  Read = analogRead(Temp_Pin);  // Read the analog value from the temperature sensor
  Read = (5.00 / 1023.00) * Read;  // Convert to voltage
  VR = VCC - Read;
  RT = Read / (VR / R);  // Resistance of RT
  ln = log(RT / RT0);
  Temp = (1 / ((ln / B) + (1 / T0))); // Temperature from sensor
  Temp = Temp - 273.15;  // Convert to Celsius

  // 3. Turbidity Sensor Reading
  int turbidityValue = analogRead(turbidityPin);  // Read the analog value from the turbidity sensor
  voltage = turbidityValue * (5.0 / 1023.0);   // Convert the analog value to voltage (for 5V system)

  // Map voltage to NTU based on the calibration (adjust as necessary)
  if (voltage >= 3.83) {
    ntu = 0;  // Clear water
  } else if (voltage >= 2.83 && voltage < 3.83) {
    ntu = map(voltage * 1000, 2830, 3830, 0, 50);  // Map 2.83V (50 NTU) to 3.83V (0 NTU)
  } else if (voltage >= 1.57 && voltage < 2.83) {
    ntu = map(voltage * 1000, 1570, 2830, 50, 100);  // Map 1.57V (100 NTU) to 2.83V (50 NTU)
  } else {
    ntu = 100;  // Assume very high turbidity for lower voltage values
  }

  // Output all values (TDS, Temperature, Turbidity) in one line
  Serial.print(TDS_ppm);  // Print TDS value
  Serial.print(", ");     // Add a separator (comma)
  Serial.print(Temp);     // Print Temperature value
  Serial.print(", ");     // Add a separator (comma)
  Serial.println(ntu);    // Print Turbidity value

  delay(1000);  // Wait for 1 second before the next reading
}
