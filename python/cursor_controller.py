import serial
import pyautogui
import time

# ==============================
# SETTINGS
# ==============================

COM_PORT = "COM5"
BAUD_RATE = 115200

# Cursor sensitivity
SENSITIVITY_X = 0.1
SENSITIVITY_Y = 0.1

# Ignore tiny movements
DEADZONE = 8.0

# Maximum cursor speed
MAX_SPEED = 5

# Change these if direction is reversed
INVERT_X = False
INVERT_Y = False

# ==============================

pyautogui.PAUSE = 0

# Prevent PyAutoGUI corner fail-safe
pyautogui.FAILSAFE = False

screen_w, screen_h = pyautogui.size()

print("Connecting to ESP32...")

ser = serial.Serial(
    COM_PORT,
    BAUD_RATE,
    timeout=1
)

time.sleep(2)

print("Connected!")
print()
print("Keep glove still...")
print("Starting cursor control...")

# Flush old data
ser.reset_input_buffer()

# ==============================
# VARIABLES
# ==============================

zero_pitch = None
zero_roll = None

last_touch = 0

# ==============================
# MAIN LOOP
# ==============================

while True:

    try:

        line = ser.readline().decode(
            "utf-8",
            errors="ignore"
        ).strip()

        if not line:
            continue

        if not line.startswith("DATA,"):
            continue

        parts = line.split(",")

        if len(parts) != 4:
            continue

        pitch = float(parts[1])
        roll = float(parts[2])
        touch = int(parts[3])

        # --------------------------
        # SET NEUTRAL POSITION
        # --------------------------

        if zero_pitch is None:

            zero_pitch = pitch
            zero_roll = roll

            print(
                "Neutral position:",
                zero_pitch,
                zero_roll
            )

            continue

        # --------------------------
        # RELATIVE ANGLE
        # --------------------------

        pitch_diff = pitch - zero_pitch
        roll_diff = roll - zero_roll

        # --------------------------
        # DEADZONE
        # --------------------------

        if abs(pitch_diff) < DEADZONE:
            pitch_diff = 0

        if abs(roll_diff) < DEADZONE:
            roll_diff = 0

        # --------------------------
        # CURSOR SPEED
        # --------------------------

        move_x = roll_diff * SENSITIVITY_X
        move_y = pitch_diff * SENSITIVITY_Y

        # --------------------------
        # LIMIT SPEED
        # --------------------------

        move_x = max(
            -MAX_SPEED,
            min(MAX_SPEED, move_x)
        )

        move_y = max(
            -MAX_SPEED,
            min(MAX_SPEED, move_y)
        )

        # --------------------------
        # INVERT DIRECTION
        # --------------------------

        if INVERT_X:
            move_x = -move_x

        if INVERT_Y:
            move_y = -move_y

        # --------------------------
        # MOVE CURSOR
        # --------------------------

        if move_x != 0 or move_y != 0:

            pyautogui.moveRel(
                int(move_x),
                int(move_y),
                duration=0
            )

        # --------------------------
        # TOUCH = LEFT CLICK
        # --------------------------

        if touch == 1 and last_touch == 0:

            pyautogui.click()

            print("CLICK")

        last_touch = touch

    except KeyboardInterrupt:

        print("Stopped.")
        break

    except Exception as e:

        print("Error:", e)
