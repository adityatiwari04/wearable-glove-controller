# 🖐️ How to Operate the Wearable Glove Controller

This guide explains how to set up and operate the Wearable Glove Controller.

---

## 📋 Requirements

Before starting, make sure you have:

### Hardware

- ESP32 Development Board
- MPU6050
- TTP223 Touch Sensor
- Glove
- Connecting wires
- USB cable
- Computer

### Software

- Arduino IDE
- ESP32 board support for Arduino IDE
- Python 3
- Required Python libraries

---

# 1. Assemble the Hardware

Connect the components according to the wiring diagram in:

hardware/wiring.md

#🖐️ How to Operate the Wearable Glove Controller

This guide explains how to set up and operate the Wearable Glove Controller.

## 📋 Requirements
Before starting, make sure you have:

### Hardware

- ESP32 Development Board
- MPU6050
- TTP223 Touch Sensor
- Glove
- Connecting wires
- USB cable
- Computer

### Software

- Arduino IDE
- ESP32 board support for Arduino IDE
- Python 3
- Required Python libraries

---

# 1. Assemble the Hardware
Connect the components according to the wiring diagram in:
hardware/wiring.md
MPU6050
VCC → 3.3V
GND → GND
SDA → GPIO 21
SCL → GPIO 22
TTP223
VCC → 3.3V
GND → GND
OUT → GPIO 4
Make sure all connections are secure before powering the ESP32.

2. Upload the ESP32 Firmware
Open the following file from this repository:
esp32/glove_controller.ino
Open the code in Arduino IDE.
Select your ESP32 board
In Arduino IDE:
Tools → Board → ESP32 → ESP32 Dev Module
Select the appropriate COM port:
Tools → Port → Your ESP32 COM Port
Then click:
Upload
Wait until the upload is completed.

3. Check the ESP32
Open the Arduino Serial Monitor.
Set the baud rate to:
115200
The ESP32 should display messages similar to:
================================
ESP32 GLOVE CONTROLLER
================================
MPU6050 detected.
During startup, the system performs gyroscope calibration.

4. Keep the Glove Still During Calibration
When the ESP32 displays:
GYRO CALIBRATION
KEEP YOUR HAND COMPLETELY STILL
Keep your hand and the glove completely still.
After calibration, the system establishes the initial hand position.
This position becomes the neutral cursor position.

5. Connect ESP32 to the Computer
Connect the ESP32 to the computer using a USB cable.
The ESP32 will continuously send sensor data through the serial connection.
The data format is:
DATA,pitch,roll,touch
Example:
DATA,2.35,-4.21,0

6. Configure the Python Program
Open:
python/cursor_controller.py
Find:
COM_PORT = "COM5"
Replace COM5 with the COM port assigned to your ESP32.
For example:
COM_PORT = "COM3"
Keep the baud rate at:
BAUD_RATE = 115200

7. Install Python Dependencies
Open Command Prompt or PowerShell in the project directory.
Run:
pip install -r python/requirements.txt
The required libraries are:
PySerial
PyAutoGUI

8. Run the Cursor Controller
Run:
python python/cursor_controller.py
You should see:
Connecting to ESP32...
Connected!

Keep glove still...
Starting cursor control...
The program will establish the initial hand orientation as the neutral position.

9. Move the Cursor
Move or tilt your hand while wearing the glove.
The MPU6050 detects changes in hand orientation.
The Python program converts these changes into cursor movement.
The basic mapping is:
Hand tilt → MPU6050 → ESP32 → Serial → Python → Mouse Cursor

10. Perform a Click
Touch the TTP223 sensor.
The ESP32 detects the touch and sends the touch state to the Python program.
The Python program converts the touch event into a left mouse click.
Basic interaction:
Hand movement → Cursor movement

Touch sensor → Left click

11. Adjust Cursor Sensitivity
Inside:
python/cursor_controller.py
You can change:
SENSITIVITY_X = 0.1
SENSITIVITY_Y = 0.1
Higher values make the cursor move faster.
Lower values make the cursor move more slowly.
Example:
SENSITIVITY_X = 0.6
SENSITIVITY_Y = 0.6

12. Adjust Dead Zone
The dead zone prevents very small sensor movements from moving the cursor.
Current value:
DEADZONE = 2.0
Increasing the value can reduce unwanted small movements.
For example:
DEADZONE = 3.0

13. Adjust Maximum Cursor Speed
The maximum cursor speed is controlled by:
MAX_SPEED = 25
You can increase or decrease this value according to your preference.
Example:
MAX_SPEED = 15

14. Reverse Cursor Direction
If the cursor moves in the opposite direction, change:
INVERT_X = False
INVERT_Y = False
For example:
INVERT_X = True
or:
INVERT_Y = True

🔄 Complete Working Flow
              HAND MOVEMENT
                    ↓
                MPU6050
                    ↓
                 ESP32
                    ↓
              Serial Data
                    ↓
            Python Program
                    ↓
              Mouse Cursor


              TOUCH INPUT
                    ↓
                 TTP223
                    ↓
                 ESP32
                    ↓
              Serial Data
                    ↓
            Python Program
                    ↓
               LEFT CLICK

🛑 Stopping the Program
To stop the Python program, press:
Ctrl + C
The program will stop the cursor-control loop.

⚠️ Important
Keep the glove still whenever the system is establishing its initial neutral position.
If the cursor behaves incorrectly, check:
MPU6050 wiring
TTP223 wiring
ESP32 COM port
USB connection
Python dependencies
Sensor orientation
Sensitivity and dead-zone settings
For troubleshooting, see:
docs/troubleshooting.md
