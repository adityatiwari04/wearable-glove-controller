# 🖐️ Wearable Glove Controller

A wearable, sensor-based glove that allows users to control a computer using natural hand movements and touch input.

The system uses an **ESP32**, **MPU6050 accelerometer/gyroscope**, and **TTP223 touch sensor** to translate hand movements into cursor movement and touch input into mouse actions.

The project also explores **wireless Wi-Fi operation** and **Android phone-to-laptop interaction** using scrcpy.

---

## 🎥 Project Demonstration

Watch the project demonstration on LinkedIn:

[▶️ Watch the Wearable Glove Controller Demo](https://lnkd.in/p/gGEYcJfd)

---

## 🚀 Project Overview

Traditional computer mice require a physical surface and keep the user relatively close to the interaction area.

This project explores an alternative:

> **What if natural hand movements could become the interface?**

The glove tracks the orientation of the user's hand using the MPU6050.

The ESP32 processes the sensor data and sends it to the computer.

A Python program then converts the received data into cursor movement and mouse actions.

---

# ✨ Features

- 🖐️ Hand-movement-based cursor control
- 🎯 MPU6050-based orientation tracking
- 👆 Touch-based left click
- 🖱️ Alternative mouse interaction
- 📐 Adjustable cursor sensitivity
- 🚫 Dead-zone filtering
- ⚡ Maximum cursor-speed limitation
- 🔄 Adjustable cursor direction
- 🔌 USB serial communication
- 📡 Alternative Wi-Fi-based communication
- 🔋 Battery-powered operation can be explored
- 📱 Android phone-to-laptop integration
- 🖥️ Phone screen mirroring using scrcpy
- 🤖 Hardware + software integration
- 🔧 Modular architecture for future improvements

---

# 🧩 Hardware Components

| Component | Purpose |
|---|---|
| ESP32 | Main microcontroller and communication unit |
| MPU6050 | Accelerometer and gyroscope for hand orientation |
| TTP223 | Capacitive touch sensor |
| Glove | Wearable platform |
| Battery / Power Source | Portable power |
| Connecting Wires | Electrical connections |

---

# 🔌 Hardware Connections

## MPU6050 → ESP32

| MPU6050 | ESP32 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

## TTP223 → ESP32

| TTP223 | ESP32 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| OUT | GPIO 4 |

### Simplified Wiring

```text
                 ESP32
              ┌──────────┐
              │          │
     SDA ─────│ GPIO 21  │──── SDA → MPU6050
     SCL ─────│ GPIO 22  │──── SCL → MPU6050
              │          │
     Touch ───│ GPIO 4   │──── OUT → TTP223
              │          │
     3.3V ────│ 3V3      │──── VCC
     GND ─────│ GND      │──── GND
              └──────────┘

For detailed wiring information:
hardware/wiring.md

💻 Software Requirements
Arduino IDE
Arduino IDE is required to upload the ESP32 firmware.
ESP32 Board Support
In Arduino IDE:
File → Preferences
Add the following URL to Additional Boards Manager URLs:
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
Then:
Tools → Board → Boards Manager
Search for:
ESP32
Install:
ESP32 by Espressif Systems
🔌 USB Mode
The USB version is the basic and currently documented wired implementation.
Data Flow
MPU6050 + TTP223
        ↓
      ESP32
        ↓
    USB Serial
        ↓
      Laptop
        ↓
Python Controller
        ↓
   PyAutoGUI
        ↓
Computer Cursor
ESP32 Firmware
Open:
esp32/glove_controller.ino
Upload it to the ESP32 using Arduino IDE.
Select the appropriate:
Board → ESP32 Dev Module
and the correct COM port.

🐍 Python Setup
Python 3 is required for the computer-side controller.
Install the required packages:
pip install -r python/requirements.txt
The project uses:
PySerial
PyAutoGUI

🖱️ Running the USB Controller
Open:
python/cursor_controller.py
Before running it, check:
COM_PORT = "COM5"
Replace COM5 with the COM port assigned to your ESP32.
Then run:
python python/cursor_controller.py
Keep the glove still when the program starts so the initial orientation can be established as the neutral position.

🎯 Cursor Configuration
The following settings can be adjusted in:
python/cursor_controller.py
Sensitivity
SENSITIVITY_X = 0.4
SENSITIVITY_Y = 0.4
Higher values increase cursor movement.
Dead Zone
DEADZONE = 2.0
This helps ignore very small movements.
Maximum Speed
MAX_SPEED = 25
This limits the maximum cursor movement.
Direction
INVERT_X = False
INVERT_Y = False
Change these to True if an axis moves in the opposite direction.

📡 Wireless Wi-Fi Mode
The project also includes an alternative Wi-Fi implementation.
Instead of sending data through USB serial, the ESP32 sends data to the laptop using UDP over Wi-Fi.
Wireless Data Flow
MPU6050 + TTP223
        ↓
      ESP32
        ↓
    Wi-Fi / UDP
        ↓
      Laptop
        ↓
Python Controller
        ↓
   PyAutoGUI
        ↓
Computer Cursor
Wireless Files
wireless/
├── esp32_wifi_controller.ino
└── wifi_cursor_controller.py

⚙️ Wi-Fi Setup
Open:
wireless/esp32_wifi_controller.ino
Configure:
const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

IPAddress LAPTOP_IP(192, 168, 1, 100);
Replace the Wi-Fi credentials with your network details.
Replace LAPTOP_IP with the laptop's actual local IPv4 address.
On Windows, use:
ipconfig
in Command Prompt to find the laptop's IPv4 address.
The ESP32 and laptop should be able to communicate over the same Wi-Fi network.

▶️ Run the Wi-Fi Controller
Run:
python wireless/wifi_cursor_controller.py
The Python program listens on UDP port:
4210
If the cursor does not respond, check the laptop IP, Wi-Fi connection, and firewall settings.
USB vs Wi-Fi
Mode
Communication
Main Advantage
USB
Serial
Simple and reliable
Wi-Fi
UDP
Wireless operation
The practical operating distance depends on the Wi-Fi network, signal strength, obstacles, and environment.

📱 Phone-to-Laptop Integration
The project can also demonstrate interaction with an Android smartphone while viewing its screen on a laptop.
For this, the project uses scrcpy.
scrcpy allows an Android device's screen to be displayed and controlled from a computer.
Data Flow
Android Phone
      ↓
USB Debugging
      ↓
    scrcpy
      ↓
    Laptop
      ↓
Phone Screen + Interaction

🔧 Android Phone Setup
1. Enable Developer Options
On the Android phone:
Settings → About Phone → Build Number
Tap Build Number several times until Developer Options are enabled.
The exact location can vary between Android manufacturers.
2. Enable USB Debugging
Go to:
Settings → Developer Options → USB Debugging
Enable USB Debugging.
3. Install scrcpy
Official repository:

🔗 Genymobile/scrcpy⁠�
Download the appropriate version for your operating system and extract it.
4. Connect the Phone
Connect the Android phone to the laptop using a USB data cable.
Unlock the phone and accept the USB debugging authorization prompt if it appears.
5. Start scrcpy
Open the extracted scrcpy folder and run:
scrcpy.exe
The phone screen should appear on the laptop.

🖐️ Glove + Phone Interaction
The glove provides an alternative input interface while scrcpy makes the smartphone screen visible on the laptop.
Conceptually:
Wearable Glove
      ↓
     ESP32
      ↓
Laptop Interaction
      ↓
    scrcpy
      ↓
Android Phone
This allows the project to demonstrate interaction with both a computer and a smartphone using a wearable interface.
Detailed phone setup:
phone/phone_to_laptop.md

🏗️ System Architecture
                  USER
                   │
          ┌────────┴────────┐
          │                 │
          ▼                 ▼
   Hand Movement          Touch
          │                 │
          ▼                 ▼
      MPU6050             TTP223
          │                 │
          └────────┬────────┘
                   ▼
                 ESP32
                   │
          ┌────────┴────────┐
          │                 │
          ▼                 ▼
       USB Serial        Wi-Fi / UDP
          │                 │
          └────────┬────────┘
                   ▼
             Laptop / PC
                   │
                   ▼
            Python Controller
                   │
                   ▼
               PyAutoGUI
                   │
                   ▼
             User Interface

Detailed architecture:
docs/architecture.md

🧮 Orientation Processing
The MPU6050 provides accelerometer and gyroscope measurements.
The project uses a complementary filter to combine the two sources and estimate hand orientation.
The firmware uses:
ALPHA = 0.96
The accelerometer provides a long-term reference while the gyroscope provides short-term motion information.

👆 Touch Interaction
The TTP223 is connected to:
GPIO 4
The current Python implementation uses a touch transition to trigger a left-click.
Touch
  ↓
TTP223
  ↓
ESP32
  ↓
Python
  ↓
Left Click
Additional gesture recognition can be added in future versions.

📁 Project Structure
wearable-glove-controller/
│
├── README.md
│
├── esp32/
│   └── glove_controller.ino
│
├── python/
│   ├── cursor_controller.py
│   └── requirements.txt
│
├── hardware/
│   └── wiring.md
│
├── wireless/
│   ├── esp32_wifi_controller.ino
│   └── wifi_cursor_controller.py
│
├── phone/
│   └── phone_to_laptop.md
│
├── docs/
│   ├── operation.md
│   ├── architecture.md
│   └── troubleshooting.md
│
├── CHANGELOG.md
├── .gitignore
└── LICENSE

🛠️ Troubleshooting
For common hardware, Python, USB, and sensor problems, see:
docs/troubleshooting.md
Common checks:
☐ ESP32 is powered
☐ MPU6050 is connected correctly
☐ TTP223 is connected correctly
☐ Correct COM port selected
☐ Baud rate is 115200
☐ Python dependencies installed
☐ Glove is still during calibration
☐ Neutral position established
☐ Wi-Fi credentials are correct for wireless mode
☐ Laptop IP is correct for wireless mode

📖 Documentation
Detailed documentation is available here:
Hardware
hardware/wiring.md
Operation Guide
docs/operation.md
System Architecture
docs/architecture.md
Troubleshooting
docs/troubleshooting.md
Phone Integration
phone/phone_to_laptop.md
Changelog
CHANGELOG.md

🔮 Future Development
Possible future improvements include:
Wireless phone interaction
Bluetooth HID support
Advanced gesture recognition
Double-click gestures
Right-click gestures
Triple-click gestures
Scroll gestures
Pinch gestures
Smartphone navigation
Multi-device interaction
Improved sensor fusion
Custom PCB
Better battery optimization
Improved wearable design
Machine-learning-based gesture recognition
Completely wireless operation

🤝 Contributing
Contributions, improvements, ideas, and experiments are welcome.
If you want to contribute:
Fork the repository.
Create a new branch.
Make your changes.
Test the changes.
Submit a pull request.

📄 License
This project is licensed under the MIT License.
See:
LICENSE

🔗 Project Repository
GitHub: https://github.com/adityatiwari04/wearable-glove-controller
⭐ If You Find This Project Interesting
Feel free to explore the code, build your own version, experiment with new interaction methods, and contribute improvements to the project.
