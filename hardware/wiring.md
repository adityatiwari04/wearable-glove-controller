# 🔌 Hardware Wiring

This document explains how the sensors are connected to the ESP32 in the Wearable Glove Controller.

---

## 🧩 Components

- ESP32 Development Board
- MPU6050 Accelerometer + Gyroscope
- TTP223 Capacitive Touch Sensor
- Wearable glove
- Connecting wires
- Power source

---

# 1. MPU6050 → ESP32

The MPU6050 communicates with the ESP32 using the I2C communication protocol.

| MPU6050 Pin | ESP32 Pin |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

### Connection

MPU6050                  ESP32
────────                  ─────
VCC       ─────────────→  3.3V
GND       ─────────────→  GND
SDA       ─────────────→  GPIO 21
SCL       ─────────────→  GPIO 22

2. TTP223 → ESP32
The TTP223 is used to detect touch input.
TTP223 Pin
ESP32 Pin
VCC
3.3V
GND
GND
OUT
GPIO 4
Connection
TTP223                   ESP32
──────                   ─────
VCC       ─────────────→ 3.3V
GND       ─────────────→ GND
OUT       ─────────────→ GPIO 4

3. Complete Wiring
                         ┌─────────────────┐
                         │      ESP32      │
                         │                 │
                         │ 3.3V ───────────┼──── VCC → MPU6050
                         │                 │
                         │ GND ────────────┼──── GND → MPU6050
                         │                 │
                         │ GPIO 21 ────────┼──── SDA → MPU6050
                         │ GPIO 22 ────────┼──── SCL → MPU6050
                         │                 │
                         │ 3.3V ───────────┼──── VCC → TTP223
                         │                 │
                         │ GND ────────────┼──── GND → TTP223
                         │                 │
                         │ GPIO 4 ─────────┼──── OUT → TTP223
                         │                 │
                         └─────────────────┘

4. Sensor Functions
MPU6050
The MPU6050 provides:
Accelerometer measurements
Gyroscope measurements
Hand orientation information
The ESP32 reads the sensor through I2C.
The project uses the measured orientation to calculate pitch and roll.
TTP223
The TTP223 provides touch detection.
When the touch sensor is activated, the ESP32 reads the signal on GPIO 4.
The Python controller can then convert this signal into a mouse action.

5. Important Notes
Power
The sensors are operated from the ESP32's 3.3V supply.
Make sure that:
VCC and GND are connected correctly.
All devices share a common ground.
The sensor wiring is secure.
I2C
The project uses:
SDA → GPIO 21
SCL → GPIO 22
The MPU6050 I2C address used by the firmware is:
0x68

6. Glove Placement
A practical arrangement is:
        ┌───────────────┐
        │   MPU6050     │
        │   Hand Area   │
        └───────────────┘

             ✋ GLOVE

              [TTP223]
              [ PALM ]
The MPU6050 should be mounted firmly on the glove so that it follows the movement of the hand.
The TTP223 can be positioned on the palm or another convenient location where it can be easily touched.


⚠️ Safety
Disconnect the power source before changing the wiring.
Double-check the connections before powering the ESP32.
Do not connect sensor VCC to a voltage higher than the module's supported voltage.
