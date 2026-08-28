# 📱 Phone Screen on Laptop
This project can also be used to interact with an Android smartphone from a laptop.
The phone screen is displayed on the laptop using **scrcpy**.

This allows the wearable glove to be used as an alternative interaction method while the smartphone screen is visible on the computer.
## 🔗 How It Works

The phone-to-laptop setup works through the following flow:

```text
Android Phone
      │
      │ USB Connection
      ▼
USB Debugging
      │
      ▼
    scrcpy
      │
      ▼
    Laptop
      │
      ▼
Phone Screen Displayed
on Laptop
The glove controller and phone-screen mirroring are separate components.
The glove provides the interaction method, while scrcpy provides the phone-screen display and computer-side interaction with the Android device.

🧩 Requirements
You need:
Android smartphone
Windows laptop/PC
USB data cable
USB debugging enabled on the phone
scrcpy

⚙️ 1. Enable Developer Options
On your Android phone:
Open Settings.
Go to About phone.
Find Build number.
Tap Build number several times until Developer Options are enabled.
The exact location may differ depending on the Android manufacturer.

🔧 2. Enable USB Debugging
Open:
Settings
→ Developer Options
→ USB Debugging
Enable USB Debugging.
When the phone asks for permission after connecting it to the computer, allow the computer to access the device.

💻 3. Install scrcpy
Download scrcpy from its official GitHub repository:
https://github.com/Genymobile/scrcpy
Download the appropriate release for Windows.
Extract the downloaded ZIP file to a convenient location.

🔌 4. Connect the Phone
Connect the Android phone to the laptop using a USB data cable.
Make sure:
The cable supports data transfer.
USB debugging is enabled.
The phone is unlocked.
The USB debugging authorization request is accepted.

▶️ 5. Start scrcpy
Open the extracted scrcpy folder.
Run:
scrcpy.exe
A window containing the phone screen should appear on the laptop.
You can now view and interact with the Android phone through the laptop.

🖥️ 6. Phone Screen on Laptop
The basic setup is:
       ANDROID PHONE
             │
             │ USB
             ▼
       USB DEBUGGING
             │
             ▼
           SCRCPY
             │
             ▼
          LAPTOP
             │
             ▼
     PHONE SCREEN WINDOW
scrcpy transfers the Android device's screen to the computer and allows interaction with the device using the computer.

🖐️ 7. Integration With the Glove Controller
The overall project can combine both systems:
                 WEARABLE GLOVE
                       │
              ┌────────┴────────┐
              │                 │
              ▼                 ▼
           MPU6050            TTP223
              │                 │
              └────────┬────────┘
                       ▼
                     ESP32
                       │
                       ▼
                  Laptop Input
                       │
                       ▼
              ┌─────────────────┐
              │   Interaction   │
              │                 │
              │ Laptop / Phone  │
              └─────────────────┘
                       ▲
                       │
                  scrcpy
                       │
                       │ USB
                       ▼
                Android Phone
The glove can therefore act as an alternative input interface while the smartphone screen is displayed on the laptop.

📡 Wireless Extension
The current phone-screen setup described here uses a USB connection.
scrcpy also supports other connection methods depending on the Android device and configuration.
A future version of this project can explore wireless phone mirroring so that:
Android Phone
      │
      │ Wireless
      ▼
    Laptop
      ▲
      │
    Glove
This could help create a more completely wireless interaction system.

🔐 USB Debugging Security
USB debugging provides powerful access between the computer and Android device.
Only authorize computers that you trust.
When finished, USB debugging can be disabled from Developer Options if it is not needed.

🛠️ Troubleshooting
Phone is not detected
Check:
USB cable supports data transfer.
USB debugging is enabled.
Phone is unlocked.
USB debugging authorization was accepted.
Try reconnecting the USB cable.
scrcpy does not open the phone screen
Try:
Disconnecting the phone.
Reconnecting it.
Unlocking the phone.
Checking the USB debugging authorization prompt.
Running scrcpy again.
USB debugging authorization does not appear
Check that Developer Options and USB Debugging are enabled.
You can also reconnect the USB cable and watch for the authorization prompt on the phone.

🎯 Purpose in This Project
The purpose of using scrcpy is to make the Android smartphone screen accessible from the laptop.
This enables the project to demonstrate a broader interaction concept:
Use a wearable glove as an alternative input interface while interacting with a smartphone through the laptop.
The phone mirroring component can be further extended in future versions with wireless communication and more advanced gesture-based controls.

🚀 Future Improvements
Possible future improvements include:
Wireless phone mirroring
Gesture-based phone navigation
Touch-free smartphone interaction
Phone scrolling using glove movements
Gesture-based app control
Multi-device interaction
Bluetooth HID integration
Wi-Fi-based communication
Completely wireless glove operation
