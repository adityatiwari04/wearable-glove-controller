# 🛠️ Troubleshooting Guide
This guide helps diagnose common problems with the Wearable Glove Controller.
## 1. MPU6050 Not Found
### Problem
The Serial Monitor displays:

```text
ERROR: MPU6050 NOT FOUND!
Possible Causes
Incorrect SDA connection
Incorrect SCL connection
VCC not connected correctly
GND not connected correctly
Loose wires
Incorrect I2C address
Faulty sensor
Check the Wiring
MPU6050 → ESP32

VCC → 3.3V
GND → GND
SDA → GPIO 21
SCL → GPIO 22
The firmware expects the MPU6050 at:
0x68

2. Cursor Does Not Move
Check
Make sure:
ESP32 is connected to the computer.
The correct COM port is selected.
The Python program is running.
The ESP32 is sending data.
The MPU6050 is connected correctly.
The Serial Monitor should show data similar to:
DATA,2.35,-4.21,0

3. Python Cannot Connect to ESP32
Problem
You may see an error related to the serial port.
Solution
Open:
python/cursor_controller.py
Check:
COM_PORT = "COM5"
Replace COM5 with the actual COM port of your ESP32.
On Windows, you can check the port through:
Device Manager
→ Ports (COM & LPT)

4. Python Module Not Found
Problem
Python displays an error such as:
ModuleNotFoundError
Solution
Install the required dependencies:
pip install -r python/requirements.txt
Or install them individually:
pip install pyserial pyautogui

5. Cursor Moves Too Slowly
Open:
python/cursor_controller.py
Increase:
SENSITIVITY_X = 0.4
SENSITIVITY_Y = 0.4
For example:
SENSITIVITY_X = 0.6
SENSITIVITY_Y = 0.6
You can experiment with different values until the movement feels comfortable.

6. Cursor Moves Too Fast
Decrease the sensitivity:
SENSITIVITY_X = 0.3
SENSITIVITY_Y = 0.3
You can also reduce:
MAX_SPEED = 25
For example:
MAX_SPEED = 15

7. Cursor Moves When the Hand Is Still
This can happen because of small sensor variations or natural hand movement.
Increase:
DEADZONE = 2.0
For example:
DEADZONE = 3.0
A larger dead zone ignores smaller changes in orientation.

8. Cursor Moves in the Wrong Direction
Open:
python/cursor_controller.py
Check:
INVERT_X = False
INVERT_Y = False
If horizontal movement is reversed:
INVERT_X = True
If vertical movement is reversed:
INVERT_Y = True

9. Touch Sensor Does Not Work
Check the Wiring
TTP223 → ESP32

VCC → 3.3V
GND → GND
OUT → GPIO 4
Make sure the touch sensor output is connected to:
GPIO 4
The ESP32 reads the sensor using:
digitalRead(TOUCH_PIN);

10. Touch Is Triggering Repeated Clicks
The Python program detects the transition from:
0 → 1
rather than continuously clicking while the sensor remains active.
The relevant logic is:
if touch == 1 and last_touch == 0:
    pyautogui.click()
This prevents continuous clicking while the sensor remains touched.

11. Calibration Problems
During startup, the ESP32 performs gyroscope calibration.
The Serial Monitor displays:
GYRO CALIBRATION
KEEP YOUR HAND COMPLETELY STILL
Keep the glove completely still during this process.
Movement during calibration can affect the calculated gyroscope offsets.

12. Cursor Jumps After Starting Python
When the Python controller starts, it records the current orientation as the neutral position.
Therefore:
Put your hand in a comfortable neutral position.
Keep it still.
Start the Python controller.
Wait for the neutral position message.
Then begin moving the glove.

13. Serial Data Looks Incorrect
The expected format is:
DATA,pitch,roll,touch
Example:
DATA,5.21,-3.42,0
The Python program expects exactly four comma-separated values.
If the format is changed in the ESP32 firmware, the Python parser may need to be updated as well.

14. Arduino Upload Problem
If Arduino IDE cannot upload the firmware:
Check
Correct ESP32 board selected
Correct COM port selected
USB cable supports data transfer
ESP32 is connected properly
No other application is using the COM port
Try reconnecting the ESP32 and selecting the correct port again.

15. Python Program Stops Unexpectedly
Run the program from Command Prompt or PowerShell:
python python/cursor_controller.py
Read the error displayed in the terminal.
Common causes include:
Incorrect COM port
Missing Python package
ESP32 disconnected
Invalid serial data
Python environment configuration

16. ESP32 Disconnects During Operation
Check:
USB cable
USB port
ESP32 power
Loose sensor connections
Damaged wires
For a wearable battery-powered version, use an appropriate regulated power source for the ESP32 and connected sensors.

17. General Diagnostic Checklist
Before asking for help, check the following:
☐ ESP32 powers on
☐ MPU6050 is detected
☐ MPU6050 wiring is correct
☐ TTP223 wiring is correct
☐ Correct COM port selected
☐ Baud rate is 115200
☐ Python dependencies installed
☐ ESP32 is sending DATA messages
☐ Glove is still during calibration
☐ Neutral position is established
☐ Sensitivity is appropriate
☐ Dead zone is appropriate

🧪 Recommended Testing Order
If the complete system is not working, test it in this order:
1. Check ESP32
       ↓
2. Check MPU6050
       ↓
3. Check TTP223
       ↓
4. Upload ESP32 firmware
       ↓
5. Verify Serial Monitor data
       ↓
6. Install Python dependencies
       ↓
7. Check COM port
       ↓
8. Run Python controller
       ↓
9. Establish neutral position
       ↓
10. Test cursor movement
       ↓
11. Test touch click
Testing each layer separately makes troubleshooting easier.

📌 Important
Do not change multiple settings at the same time while troubleshooting.
Change one parameter, test the system, and then adjust the next parameter.
This makes it easier to identify the cause of a problem.
