import socket
import pyautogui

# ==============================
# SETTINGS
# ==============================

UDP_IP = "0.0.0.0"
UDP_PORT = 4210

# Cursor sensitivity
SENSITIVITY_X = 0.4
SENSITIVITY_Y = 0.4

# Ignore tiny movements
DEADZONE = 2.0

# Maximum cursor speed
MAX_SPEED = 25

# Change these if direction is reversed
INVERT_X = False
INVERT_Y = False

# ==============================

pyautogui.PAUSE = 0
pyautogui.FAILSAFE = False

print("Starting Wi-Fi cursor controller...")

sock = socket.socket(
    socket.AF_INET,
    socket.SOCK_DGRAM
)

sock.bind((UDP_IP, UDP_PORT))

print("Waiting for ESP32...")
print(f"Listening on UDP port {UDP_PORT}")

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

        data, address = sock.recvfrom(1024)

        line = data.decode(
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
                "ESP32 connected:",
                address
            )

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

  
