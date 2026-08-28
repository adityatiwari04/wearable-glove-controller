#include <Wire.h>

#define MPU_ADDR 0x68

#define SDA_PIN 21
#define SCL_PIN 22

#define TOUCH_PIN 4

// MPU6050 raw values
int16_t ax, ay, az;
int16_t gx, gy, gz;

// Gyroscope offsets
float gyroXOffset = 0;
float gyroYOffset = 0;

// Filtered angles
float pitch = 0;
float roll = 0;

unsigned long lastTime;

// Complementary filter
const float ALPHA = 0.96;

// --------------------------------------------------
// WRITE MPU6050 REGISTER
// --------------------------------------------------

void writeMPU(byte reg, byte value)
{
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

// --------------------------------------------------
// READ MPU6050
// --------------------------------------------------

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

  // Temperature - ignore
  Wire.read();
  Wire.read();

  gx = (Wire.read() << 8) | Wire.read();
  gy = (Wire.read() << 8) | Wire.read();
  gz = (Wire.read() << 8) | Wire.read();

  return true;
}

// --------------------------------------------------
// CHECK MPU6050
// --------------------------------------------------

bool checkMPU()
{
  Wire.beginTransmission(MPU_ADDR);

  if (Wire.endTransmission() != 0)
  {
    return false;
  }

  return true;
}

// --------------------------------------------------
// GYRO CALIBRATION
// --------------------------------------------------

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

// --------------------------------------------------
// SET INITIAL ANGLE
// --------------------------------------------------

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

// --------------------------------------------------
// SETUP
// --------------------------------------------------

void setup()
{
  Serial.begin(115200);

  delay(1000);

  // I2C
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);

  // Touch sensor
  pinMode(TOUCH_PIN, INPUT);

  Serial.println();
  Serial.println("================================");
  Serial.println("ESP32 GLOVE CONTROLLER");
  Serial.println("================================");

  // Check MPU
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

  // Calibrate
  calibrateGyro();

  // Establish starting position
  setInitialAngle();

  lastTime = micros();

  Serial.println();
  Serial.println("================================");
  Serial.println("READY");
  Serial.println("================================");
}

// --------------------------------------------------
// LOOP
// --------------------------------------------------

void loop()
{
  if (!readMPU())
  {
    Serial.println("MPU READ ERROR");
    delay(10);
    return;
  }

  // Time
  unsigned long now = micros();

  float dt =
    (now - lastTime) / 1000000.0;

  lastTime = now;

  if (dt <= 0 || dt > 0.1)
  {
    dt = 0.01;
  }

  // -----------------------------------------------
  // ACCELEROMETER
  // -----------------------------------------------

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

  // -----------------------------------------------
  // GYROSCOPE
  // -----------------------------------------------

  float gyroX =
    (gx - gyroXOffset) / 65.5;

  float gyroY =
    (gy - gyroYOffset) / 65.5;

  // -----------------------------------------------
  // COMPLEMENTARY FILTER
  // -----------------------------------------------

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

  // -----------------------------------------------
  // TOUCH SENSOR
  // -----------------------------------------------

  int touchState = digitalRead(TOUCH_PIN);

  // -----------------------------------------------
  // SEND TO LAPTOP
  // -----------------------------------------------

  Serial.print("DATA,");
  Serial.print(pitch, 2);
  Serial.print(",");
  Serial.print(roll, 2);
  Serial.print(",");
  Serial.println(touchState);

  // ~100-150 readings per second
  delay(5);
}
