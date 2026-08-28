# 🏗️ System Architecture
This document explains the architecture and data flow of the Wearable Glove Controller.
## 📌 System Overview

The project consists of three main layers:

1. **Sensing Layer** — MPU6050 and TTP223
2. **Processing & Communication Layer** — ESP32
3. **Computer Control Layer** — Python + PyAutoGUI

The complete system converts physical hand movement and touch input into computer interaction.

# 🔷 High-Level Architecture
┌─────────────────────────────┐
│        USER'S HAND          │
│                             │
│   Hand Movement + Touch     │
└──────────────┬──────────────┘
               │
               ▼
┌─────────────────────────────┐
│        SENSING LAYER        │
│                             │
│  ┌──────────┐  ┌─────────┐ │
│  │ MPU6050  │  │ TTP223  │ │
│  │          │  │         │ │
│  │ Acc/Gyro │  │  Touch  │ │
│  └────┬─────┘  └────┬────┘ │
└───────┼─────────────┼──────┘
        │             │
        └──────┬──────┘
               ▼
┌─────────────────────────────┐
│         ESP32               │
│                             │
│  • Read sensor data         │
│  • Calculate pitch/roll     │
│  • Apply complementary      │
│    filter                   │
│  • Read touch state         │
│  • Send serial data         │
└──────────────┬──────────────┘
               │
               │ USB Serial
               ▼
┌─────────────────────────────┐
│      PYTHON CONTROLLER      │
│                             │
│  • PySerial                 │
│  • Receive ESP32 data       │
│  • Calculate cursor motion  │
│  • Apply sensitivity        │
│  • Apply dead zone          │
│  • Limit cursor speed       │
│  • Process touch events     │
└──────────────┬──────────────┘
               │
               ▼
┌─────────────────────────────┐
│       PyAutoGUI             │
│                             │
│   Computer Mouse Control    │
└──────────────┬──────────────┘
               │
               ▼
┌─────────────────────────────┐
│       COMPUTER              │
│                             │
│      Cursor Movement        │
│      Mouse Interaction      │
└─────────────────────────────┘

🧩 1. Sensing Layer
The sensing layer consists of:
1. MPU6050
2. TTP223

MPU6050
The MPU6050 contains:
>axis accelerometer
>3-axis gyroscope

It is connected to the ESP32 using I2C.
I2C Connections

SDA → ESP32 GPIO 21
SCL → ESP32 GPIO 22

The accelerometer provides information about the hand's orientation relative to gravity.
The gyroscope provides angular velocity measurements.
The project uses these measurements to estimate:
>Pitch
>Roll

TTP223
The TTP223 is a capacitive touch sensor.
It provides a digital signal when the user touches the sensor.
Connection:
TTP223 OUT → ESP32 GPIO 4
The touch state is included in the data sent to the computer.

⚙️ 2. ESP32 Processing Layer
The ESP32 acts as the central controller.
Its main responsibilities are:
Initialize the MPU6050.
Initialize the TTP223 input.
Calibrate the gyroscope.
Read accelerometer data.
Read gyroscope data.
Calculate orientation.
Apply the complementary filter.
Read touch state.
Send data to the computer.

🧮 3. Orientation Estimation
The project combines accelerometer and gyroscope measurements using a complementary filter.
The accelerometer provides a stable long-term orientation reference.
The gyroscope provides faster short-term motion information.
The complementary filter combines both sources to obtain smoother orientation estimates.
The filter coefficient used in the firmware is:
ALPHA = 0.96
A simplified representation is:
Accelerometer ────────┐
                      │
                      ▼
                Complementary
                   Filter
                      ▲
                      │
Gyroscope ────────────┘
                      │
                      ▼
                 Pitch / Roll

📡 4. Serial Communication
The ESP32 communicates with the Python program using USB serial communication.
The configured baud rate is:
115200
The ESP32 sends data in the following format:
DATA,pitch,roll,touch
Example:
DATA,12.45,-8.21,0
The values represent:
DATA
 ↓
Pitch
 ↓
Roll
 ↓
Touch State

🐍 5. Python Processing Layer
The Python program receives the serial data using PySerial.
It then performs several processing steps.
Step 1 — Read Data
The program reads one serial line at a time.
Step 2 — Parse Data
The received data is divided into:
Pitch
Roll
Touch
Step 3 — Establish Neutral Position
When the program starts, the current pitch and roll are stored as the neutral position.
Current Position
       ↓
Neutral Position
       ↓
Calculate Difference
Step 4 — Calculate Relative Movement
The difference between the current and neutral orientation determines cursor movement.
Step 5 — Apply Dead Zone
Small movements are ignored to reduce unwanted cursor movement.
Step 6 — Apply Sensitivity
The movement is multiplied by configurable sensitivity values.
Step 7 — Limit Speed
The maximum cursor movement is limited using MAX_SPEED.
Step 8 — Move Cursor
PyAutoGUI sends the resulting movement to the operating system.

🖱️ 6. Cursor Control Pipeline
Hand Rotation
      ↓
MPU6050
      ↓
Pitch / Roll
      ↓
ESP32
      ↓
USB Serial
      ↓
Python
      ↓
Neutral Position Difference
      ↓
Dead Zone
      ↓
Sensitivity
      ↓
Maximum Speed
      ↓
PyAutoGUI
      ↓
Computer Cursor

👆 7. Touch Control Pipeline
Finger / Hand Touch
        ↓
     TTP223
        ↓
     GPIO 4
        ↓
       ESP32
        ↓
   USB Serial
        ↓
     Python
        ↓
   Mouse Action
The current implementation uses a touch transition to trigger a left-click event.
Additional gesture recognition can be implemented in the Python controller.

🔄 8. Complete Data Flow
The complete system can be represented as:
                   USER
                    │
          ┌─────────┴─────────┐
          │                   │
          ▼                   ▼
   Hand Orientation        Touch
          │                   │
          ▼                   ▼
      MPU6050              TTP223
          │                   │
          └─────────┬─────────┘
                    │
                    ▼
                  ESP32
                    │
          ┌─────────┴─────────┐
          │                   │
          ▼                   ▼
     Pitch / Roll         Touch State
          │                   │
          └─────────┬─────────┘
                    │
                    ▼
              USB Serial
                    │
                    ▼
             Python Program
                    │
          ┌─────────┴─────────┐
          │                   │
          ▼                   ▼
     Cursor Motion        Touch Event
          │                   │
          └─────────┬─────────┘
                    │
                    ▼
                PyAutoGUI
                    │
                    ▼
                 COMPUTER

🔋 9. Power Architecture
During USB operation, the ESP32 receives power through the USB connection.
The sensors receive their supply from the ESP32's 3.3V rail.
USB Power
    ↓
  ESP32
    ↓
3.3V Supply
 ┌──┴──────┐
 ↓         ↓
MPU6050   TTP223
For a portable version, a suitable rechargeable battery and power-management circuit can be added.

📡 10. Future Wireless Architecture
The current implementation uses USB serial communication.
A future wireless version could replace the USB serial link with:
ESP32
  │
  ├── Wi-Fi
  │
  └── Bluetooth
        │
        ▼
     Computer
This could make the glove completely wireless.
Possible wireless implementations include:
Wi-Fi communication
Bluetooth communication
Bluetooth HID
The actual communication range depends on the selected technology and operating environment.

🧠 11. Software Architecture
ESP32 Firmware
      │
      ├── MPU6050 Driver
      │
      ├── Gyroscope Calibration
      │
      ├── Orientation Calculation
      │
      ├── Complementary Filter
      │
      └── Touch Detection
               │
               ▼
        Serial Data Output
               │
               ▼
      Python Controller
               │
      ├── Serial Input
      ├── Data Parsing
      ├── Neutral Calibration
      ├── Dead Zone
      ├── Sensitivity
      ├── Speed Limiting
      └── Mouse Control
               │
               ▼
            PyAutoGUI
               │
               ▼
          Operating System

🎯 Design Goal
The main goal of the architecture is to separate:
Sensing
Embedded processing
Communication
Computer-side processing
User interaction
This modular structure makes it easier to improve individual parts of the system without redesigning the entire project.

🔮 Future Development
> The architecture can be extended with:
>  Wireless communication
>  Advanced touch gestures
>  Double-click detection
>  Long-touch/right-click detection
>  Triple-click detection
>  Scroll gestures
>  Pinch gestures
>  Smartphone control
>  Multi-device control
>  Bluetooth HID
>  Custom PCB
>  Improved sensor fusion
>  Machine-learning-based gesture recognition
