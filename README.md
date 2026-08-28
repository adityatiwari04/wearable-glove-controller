# 🖐️ Wearable Glove Controller

A wearable, sensor-based glove that allows users to control a computer using natural hand movements and touch gestures — without relying on a traditional mouse.

The system uses an **ESP32**, **MPU6050 accelerometer/gyroscope**, and **TTP223 touch sensor** to translate hand movements into cursor movements and touch gestures into mouse actions.

---

## 🚀 Project Overview

Traditional computer mice require a physical surface and keep the user relatively close to the interaction area.

This project explores a different approach:

> **What if natural hand movements could become the interface?**

The glove tracks the orientation of the user's hand using the MPU6050 and sends the calculated movement data to a computer through the ESP32.

The computer-side Python program converts this data into cursor movement and click actions.

---

## ✨ Features

- 🖐️ Hand-movement-based cursor control
- 🎯 MPU6050-based orientation tracking
- 👆 Touch-based left click
- 🖱️ Gesture-based mouse interaction
- 📐 Adjustable cursor sensitivity
- 🚫 Dead-zone filtering to reduce unwanted movement
- ⚡ Maximum cursor-speed limitation
- 🔄 Adjustable cursor direction
- 📡 ESP32-based wireless communication can be used for extended interaction distance
- 🔋 Can be adapted for battery-powered wearable operation
- 💻 Designed as an alternative interaction method to a traditional mouse
- 🤖 Hardware + software integration

---

## 🧩 Hardware Components

| Component | Purpose |
|---|---|
| ESP32 | Main microcontroller and communication unit |
| MPU6050 | Accelerometer and gyroscope for hand orientation |
| TTP223 Touch Sensor | Detects touch gestures |
| Glove | Wearable platform |
| Battery / Power Source | Portable power for the glove |
| Connecting Wires | Electrical connections |

---

## 🔌 Hardware Connections

### ESP32 ↔ MPU6050

| MPU6050 | ESP32 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

### ESP32 ↔ TTP223

| TTP223 | ESP32 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| OUT | GPIO 4 |

### Simplified Connection


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
## 🧠 How It Works

The system works by combining hand-motion sensing, touch input, and computer-side software.

### Working Flow


Hand Movement
      ↓
   MPU6050
      ↓
Accelerometer + Gyroscope Data
      ↓
Complementary Filter
      ↓
Pitch & Roll Angles
      ↓
     ESP32
      ↓
Serial / Wireless Communication
      ↓
Python Program
      ↓
PyAutoGUI
      ↓
Computer Cursor


1. Motion Detection
The MPU6050 provides:
Accelerometer measurements
Gyroscope measurements
The accelerometer helps estimate the hand orientation relative to gravity.
The gyroscope measures angular velocity.

2. Sensor Fusion
The ESP32 combines accelerometer and gyroscope information using a complementary filter.
This produces smoother pitch and roll estimates for cursor control.

3. Data Transmission
The ESP32 sends the calculated data to the computer using serial communication.
The data format is:
DATA,pitch,roll,touch
Example:
DATA,12.45,-8.21,0
Where:
pitch = calculated pitch angle
roll = calculated roll angle
touch = TTP223 touch state

4. Computer-Side Processing
The Python program:
1. receives the ESP32 data and:
2. Establishes the initial hand position.
3. Calculates the difference from the neutral position.
4. Applies a dead zone.
5. Calculates cursor movement.
6. Limits the maximum cursor speed.
7. Applies direction settings.
8. Moves the computer cursor using PyAutoGUI.
9. Detects touch events.

🖱️ Cursor Control
The current implementation uses changes in hand orientation to control the cursor.
Hand Action                                              Result
Rotate/tilt hand horizontally                   Horizontal cursor movement
Rotate/tilt hand vertically                     Vertical cursor movement
Touch TTP223                                        Left-click event
The exact cursor response can be adjusted using the Python configuration settings.

👆 Touch Interaction
The TTP223 touch sensor is connected to:
TTP223 OUT → ESP32 GPIO 4
When the sensor detects a touch, the ESP32 sends the touch state to the Python program.
The current Python implementation detects a new touch and performs a left click.
Additional gestures such as double-click, long-click/right-click, or triple-click can be implemented by extending the gesture-detection logic.

🧮 Dead Zone
A dead zone is used to ignore very small changes in hand orientation around the neutral position.
For example:
DEADZONE = 2.0
This means small movements below the configured threshold are ignored.
Increasing the dead zone makes the controller less sensitive to small unwanted movements.

🎯 Cursor Sensitivity
Cursor sensitivity can be changed in:
python/cursor_controller.py
Current settings:
SENSITIVITY_X = 0.4
SENSITIVITY_Y = 0.4
Lower sensitivity
Use lower values when you want to rotate your hand more before the cursor moves significantly.
Example:
SENSITIVITY_X = 0.25
SENSITIVITY_Y = 0.25
Higher sensitivity
Use higher values when you want the cursor to respond more quickly to smaller hand movements.

⚡ Maximum Cursor Speed
The maximum cursor movement can be limited using:
MAX_SPEED = 25
Reducing this value can make cursor movement more controlled.

🔄 Cursor Direction
If the cursor moves in the opposite direction, change:
INVERT_X = False
INVERT_Y = False
For example:
INVERT_X = True
will reverse the horizontal direction.

🛠️ Software Requirements
Arduino IDE
Required for programming the ESP32.
You need:
Arduino IDE
ESP32 board support
Wire library
The Wire library is normally included with the Arduino environment.
Python
Install Python 3.
Required Python packages:
pyserial
pyautogui
Install them with:
pip install pyserial pyautogui

📥 Installation
Step 1 — Install Arduino IDE
Install Arduino IDE on your computer.
Step 2 — Install ESP32 Board Support
Open Arduino IDE.
Go to:
File → Preferences
Add the ESP32 board manager URL:
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
Then go to:
Tools → Board → Boards Manager
Search for:
ESP32
Install the ESP32 board package.
Step 3 — Connect the ESP32
Connect the ESP32 to your computer using a USB data cable.
Step 4 — Select the ESP32 Board
In Arduino IDE:
Tools → Board
Select the appropriate ESP32 board.
Step 5 — Select the COM Port
Go to:
Tools → Port
Select the COM port assigned to your ESP32.
Step 6 — Upload the ESP32 Code
Open:
esp32/glove_controller.ino
Copy the code into Arduino IDE.
Click:
Upload

🐍 Python Setup
Open Command Prompt or Terminal.
Install the required packages:
pip install pyserial pyautogui
Or install them from the requirements file:
pip install -r python/requirements.txt

⚙️ Configure the COM Port
Open:
python/cursor_controller.py
Find:
COM_PORT = "COM5"
Change COM5 to the COM port assigned to your ESP32.
For example:
COM_PORT = "COM3"

▶️ How to Run
1. Connect the Glove
Connect:
MPU6050
TTP223
ESP32
according to the wiring diagram.
2. Connect ESP32 to Computer
Connect the ESP32 using a USB data cable.
3. Upload the Firmware
Upload:
esp32/glove_controller.ino
using Arduino IDE.
4. Keep the Glove Still
The ESP32 performs gyroscope calibration when it starts.
Keep the glove completely still during calibration.
5. Start Python
Open a terminal in the Python directory.
Run:
python cursor_controller.py
6. Set the Neutral Position
The initial pitch and roll values are treated as the neutral position.
Keep your hand in your preferred starting position when the Python program begins.
7. Control the Cursor
Move or rotate your hand.
The cursor will respond to the change in hand orientation.

🧤 Recommended Sensor Placement
MPU6050
Place the MPU6050 on the back of the hand.
This provides a convenient position for detecting overall hand orientation.
TTP223
Place the TTP223 on a convenient area of the palm or finger where it can easily be touched.
ESP32
Place the ESP32 on the back of the hand or wrist area.
Secure the components properly and keep wires away from frequently bending joints.

📊 System Data Flow
┌──────────────────────┐
│       MPU6050        │
│ Accelerometer/Gyro   │
└──────────┬───────────┘
           │
           ▼
┌──────────────────────┐
│        ESP32         │
│ Sensor Processing    │
│ Pitch/Roll           │
│ Touch Detection      │
└──────────┬───────────┘
           │
           │ USB Serial
           ▼
┌──────────────────────┐
│   Python Controller  │
│       PySerial       │
│       PyAutoGUI      │
└──────────┬───────────┘
           │
           ▼
┌──────────────────────┐
│    Computer Cursor   │
└──────────────────────┘

🔌 Why This Approach?
This project explores an alternative interaction method that does not require continuously moving a physical mouse across a surface.
Potential advantages include:
Wearable interaction
Reduced dependence on a mouse surface
More freedom of hand movement
Alternative computer input method
Potential accessibility applications
Potential for future wireless operation

📡 Wireless Extension
The current implementation uses USB serial communication.
The ESP32 can potentially be extended to support wireless communication such as:
Wi-Fi
Bluetooth
Bluetooth HID
A wireless implementation could remove the USB cable and provide greater freedom of movement.
The usable range depends on the communication technology, environment, network configuration, and power source.

🔋 Portable Operation
The wearable system can be adapted for battery-powered operation.
A suitable rechargeable battery and power-management solution can make the glove more portable.
The power system should be selected according to the voltage and current requirements of the ESP32 and connected sensors.

🎥 Project Demonstration
A demonstration video of the project will be added here.
https://lnkd.in/p/gGEYcJfd

▶️ Watch Project Demonstration
The demonstration can showcase:
Wearable glove operation
Hand-controlled cursor movement
Touch interaction
Computer control

📂 Repository Structure
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
├── docs/
│   ├── operation.md
│   ├── architecture.md
│   └── troubleshooting.md
│
├── CHANGELOG.md
├── .gitignore
└── LICENSE

🔮 Future Improvements
Possible future improvements include:
📡 Wireless operation
🔋 Integrated rechargeable battery
📱 Improved smartphone control
🖱️ Advanced mouse gestures
👆 Long-touch/right-click
👆 Double-click
👆 Triple-click
📜 Scroll gestures
🤏 Pinch gestures
🎮 Gaming-oriented controls
🧠 Machine-learning-based gesture recognition
🔧 Custom PCB design
📦 Smaller wearable enclosure
♿ Accessibility-focused interaction
🔗 Multi-device interaction
🎯 Potential Applications
This concept can be explored for:
Human-computer interaction
Wearable technology
Robotics
Automation
Accessibility
Hands-free interaction
Experimental input devices
Smart interfaces
Educational electronics projects

⚠️ Current Limitations
This project is currently a prototype.
Performance can depend on:
Sensor placement
Sensor noise
Calibration
Hand orientation
User movement
Computer configuration
Python environment
Communication method
Different users may require different sensitivity and dead-zone settings.

🤝 Contributing
Contributions and suggestions are welcome.
If you want to improve the project:
Fork the repository.
Create a new branch.
Make your changes.
Test the changes.
Submit a Pull Request.
Ideas for improving the hardware, software, gesture recognition, wireless communication, and wearable design are welcome.

👨‍💻 Author
Aditya Kumar Tiwari
Mechatronics Engineering Student
Interests
Robotics
AI & Computer Vision
Embedded Systems
Automation
Human-Computer Interaction

⭐ Support
If you find this project interesting, consider giving the repository a ⭐ Star.
Feedback, suggestions, and contributions are welcome.

📜 License
This project is released under the MIT License.
See the LICENSE file for details
