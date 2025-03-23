# AquaMate - Automated Aquarium Care System

## Overview
AquaMate is an IoT-based automated aquarium management system that simplifies the maintenance of aquariums. By monitoring critical water parameters, controlling feeding schedules, and adjusting lighting, AquaMate helps aquarium owners maintain a healthy environment for aquatic life with minimal effort. The system is powered by an ESP32 microcontroller, integrating Wi-Fi capabilities for remote monitoring and control via a mobile app.

## Features
- **Water Quality Monitoring:** Real-time monitoring of aquarium water parameters such as temperature, turbidity, and Total Dissolved Solids (TDS).
- **Automated Feeding:** Schedule or manually control feeding through a servo motor to ensure consistent feeding times.
- **Lighting Control:** Automate aquarium lighting with a relay module for optimal day-night cycles or manually control via the app.
- **Mobile App Integration:** Monitor and control aquarium parameters (feeding, lighting, water quality) remotely via a user-friendly mobile app.
  
## Hardware Components
- **ESP32 Microcontroller**: The heart of the system that controls sensors and actuators, and enables Wi-Fi connectivity.
- **TDS Sensor**: Measures the concentration of dissolved solids in the aquarium water.
- **Turbidity Sensor**: Detects the cloudiness of water to monitor water quality.
- **Temperature Sensor**: Ensures the aquarium temperature is optimal for aquatic life.
- **Relay Module**: Controls higher voltage devices such as aquarium lights.
- **Servo Motor (SG90)**: Automated feeding control.
- **Additional Components**: Buck converters, breadboard, jumper wires.

## Software Requirements
- **Arduino IDE**: For programming the ESP32.
- **Visual Studio Code**: For developing the mobile app.
- **Proteus**: For circuit simulation (optional).

## How It Works
1. **Water Quality Monitoring**: The system continuously measures water quality parameters such as temperature, turbidity, and TDS. This data is transmitted to the ESP32 for real-time monitoring.
2. **Automated Feeding**: The system allows automatic feeding at scheduled intervals or manual activation via the app. The servo motor dispenses food accordingly.
3. **Lighting Control**: Users can automate or manually control the aquarium lighting based on a pre-set schedule.
4. **Remote Access**: Through the AquaMate mobile app, users can monitor water quality and control feeding and lighting remotely.

##Libraries for arduino
1.https://github.com/arduino-libraries/WiFi
2.https://github.com/mobizt/Firebase-ESP32

## Getting Started

### 1. Clone the repository
```bash
git clone https://github.com/geeganage-yeran/aquamate.git
