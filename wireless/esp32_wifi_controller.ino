#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>

#define MPU_ADDR 0x68

#define SDA_PIN 21
#define SCL_PIN 22
#define TOUCH_PIN 4

// ==============================
// Wi-Fi SETTINGS
// ==============================

const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// Laptop IP address on the same Wi-Fi network
IPAddress LAPTOP_IP(192, 168, 1, 100);

const unsigned int UDP_PORT = 4210;

WiFiUDP udp;

// ==============================
// MPU6050 VARIABLES
// ==============================

int16_t ax, ay, az;
int16_t gx, gy, gz;

float gyroXOffset = 0;
float gyroYOffset = 0;

float pitch = 0;
float roll = 0;

unsigned long lastTime;

const float ALPHA = 0.96;

// ==============================
// WRITE MPU6050 REGISTER
// ==============================

void writeMPU(byte reg, byte value)
{
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

// ==============================
// READ MPU6050
// ==============================

bool readMPU()
{
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);

  if (Wire.endTransmission(false) != 0)
  {
    return false;
  }

  uint8_t received = Wire.requestFrom(MPU_ADDR, 14, true);

  if (received != 14)
  {
    return false;
  }

  ax = (Wire.read() << 8) | Wire.read();
  ay = (Wire.read() << 8) | Wire.read();
  az = (Wire.read() << 8) | Wire.read();

  // Temperature - ignored
  Wire.read();
  Wire.read();

  gx = (Wire.read() << 8) | Wire.read();
  gy = (Wire.read() << 8) | Wire.read();
  gz = (Wire.read() << 8) | Wire.read();

  return true;
}

// ==============================
// CHECK MPU6050
// ==============================

bool checkMPU()
{
  Wire.beginTransmission(MPU_ADDR);

  if (Wire.endTransmission() != 0)
  {
    return false;
  }

  return true;
}

// ==============================
// GYRO CALIBRATION
// ==============================

void calibrateGyro()
{
  Serial.println();
  Serial.println("================================");
  Serial.println("GYRO CALIBRATION");
  Serial.println("KEEP YOUR HAND COMPLETELY STILL");
  Serial.println("================================");

  long sumX = 0;
  long sumY = 0;

  const int samples = 500;

  for (int i = 0; i < samples; i++)
  {
    if (readMPU())
    {
      sumX += gx;
      sumY += gy;
    }

    delay(3);
  }

  gyroXOffset = sumX / (float)samples;
  gyroYOffset = sumY / (float)samples;

  Serial.println("Calibration complete.");
}

// ==============================
// SET INITIAL ANGLE
// ==============================

void setInitialAngle()
{
  if (!readMPU())
  {
    return;
  }

  float accX = ax / 16384.0;
  float accY = ay / 16384.0;
  float accZ = az / 16384.0;

  pitch =
    atan2(
      -accX,
      sqrt((accY * accY) + (accZ * accZ))
    )
    * 180.0 / PI;

  roll =
    atan2(accY, accZ)
    * 180.0 / PI;
}

// ==============================
// CONNECT TO WI-FI
// ==============================

void connectWiFi()
{
  Serial.println();
  Serial.println("Connecting to Wi-Fi...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi connected.");

  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Laptop IP Address: ");
  Serial.println(LAPTOP_IP);

  udp.begin(UDP_PORT);

  Serial.print("UDP Port: ");
  Serial.println(UDP_PORT);
}

// ==============================
// SETUP
// ==============================

void setup()
{
  Serial.begin(115200);

  delay(1000);

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);

  pinMode(TOUCH_PIN, INPUT);

  Serial.println();
  Serial.println("================================");
  Serial.println("ESP32 WIRELESS GLOVE CONTROLLER");
  Serial.println("================================");

  if (!checkMPU())
  {
    Serial.println("ERROR: MPU6050 NOT FOUND!");
    Serial.println("Check SDA, SCL, VCC and GND.");

    while (1)
    {
      delay(1000);
    }
  }

  Serial.println("MPU6050 detected.");

  // Wake MPU6050
  writeMPU(0x6B, 0x00);

  delay(100);

  // Gyroscope ±500°/s
  writeMPU(0x1B, 0x08);

  // Accelerometer ±2g
  writeMPU(0x1C, 0x00);

  delay(100);

  calibrateGyro();

  setInitialAngle();

  connectWiFi();

  lastTime = micros();

  Serial.println();
  Serial.println("================================");
  Serial.println("WIRELESS CONTROLLER READY");
  Serial.println("================================");
}

// ==============================
// LOOP
// ==============================

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wi-Fi disconnected. Reconnecting...");
    connectWiFi();
  }

  if (!readMPU())
  {
    Serial.println("MPU READ ERROR");
    delay(10);
    return;
  }

  unsigned long now = micros();

  float dt =
    (now - lastTime) / 1000000.0;

  lastTime = now;

  if (dt <= 0 || dt > 0.1)
  {
    dt = 0.01;
  }

  // ==============================
  // ACCELEROMETER
  // ==============================

  float accX = ax / 16384.0;
  float accY = ay / 16384.0;
  float accZ = az / 16384.0;

  float accPitch =
    atan2(
      -accX,
      sqrt((accY * accY) + (accZ * accZ))
    )
    * 180.0 / PI;

  float accRoll =
    atan2(accY, accZ)
    * 180.0 / PI;

  // ==============================
  // GYROSCOPE
  // ==============================

  float gyroX =
    (gx - gyroXOffset) / 65.5;

  float gyroY =
    (gy - gyroYOffset) / 65.5;

  // ==============================
  // COMPLEMENTARY FILTER
  // ==============================

  pitch =
    ALPHA *
    (pitch + gyroX * dt)
    +
    (1.0 - ALPHA) *
    accPitch;

  roll =
    ALPHA *
    (roll + gyroY * dt)
    +
    (1.0 - ALPHA) *
    accRoll;

  // ==============================
  // TOUCH
  // ==============================

  int touchState = digitalRead(TOUCH_PIN);

  // ==============================
  // SEND UDP DATA
  // ==============================

  udp.beginPacket(LAPTOP_IP, UDP_PORT);

  udp.print("DATA,");
  udp.print(pitch, 2);
  udp.print(",");
  udp.print(roll, 2);
  udp.print(",");
  udp.println(touchState);

  udp.endPacket();

  delay(5);
}

⚠️ Before uploading
Change these three values:
const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

IPAddress LAPTOP_IP(192, 168, 1, 100);
For example:
const char* WIFI_SSID = "MyWiFi";
const char* WIFI_PASSWORD = "mypassword";

IPAddress LAPTOP_IP(192, 168, 1, 105);
Don't copy my example IP blindly. Your laptop's actual local IP must be entered.


