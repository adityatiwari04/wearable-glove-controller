# 📋 Changelog
All notable changes to the Wearable Glove Controller project are documented here.

## [1.0.0] - Initial Release

### Added
- ESP32-based wearable glove controller
- MPU6050 accelerometer and gyroscope integration
- TTP223 touch sensor integration
- I2C communication between ESP32 and MPU6050
- Gyroscope calibration
- Accelerometer-based orientation estimation
- Gyroscope-based motion tracking
- Complementary filter for smoother pitch and roll estimation
- USB serial communication between ESP32 and computer
- Python-based computer-side controller
- PySerial integration
- PyAutoGUI integration
- Hand-movement-based cursor control
- Touch-based left-click interaction
- Configurable cursor sensitivity
- Configurable dead zone
- Maximum cursor-speed limitation
- X/Y cursor direction inversion
- Hardware wiring documentation
- System architecture documentation
- Operation guide
- Troubleshooting guide

## Planned Improvements

Future versions may include:

- Wireless communication
- Wi-Fi-based operation
- Bluetooth HID support
- Advanced gesture recognition
- Double-click gestures
- Right-click gestures
- Triple-click gestures
- Scroll gestures
- Pinch gestures
- Smartphone control
- Multi-device interaction
- Improved sensor fusion
- Custom PCB
- Battery optimization
- Improved wearable design
- Gesture-based application control
- Machine-learning-based gesture recognition

---

## Versioning

The project follows a simple versioning approach:

```text
MAJOR.MINOR.PATCH
Example:
1.0.0
MAJOR — Major redesign or breaking change
MINOR — New features
PATCH — Bug fixes and small improvements
