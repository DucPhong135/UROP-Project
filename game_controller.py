import serial
from pynput.keyboard import Controller, Key

# Initialize the serial port for communication with Arduino
ser = serial.Serial(
    port='COM8',  # Replace with your COM port (e.g., COM3, COM8, etc.)
    baudrate=9600,
    timeout=1  # Timeout for serial read operations
)

# Initialize the keyboard controller to simulate keypresses
keyboard = Controller()

def map_command_to_key(command):
    """
    Map received serial command to keyboard arrow key presses.
    
    Args:
        command (str): Single-character command from the serial input.
    """
    if command == 'U':  # Up command
        keyboard.press(Key.up)
        keyboard.release(Key.up)
        print("Up arrow pressed")
    elif command == 'D':  # Down command
        keyboard.press(Key.down)
        keyboard.release(Key.down)
        print("Down arrow pressed")
    elif command == 'L':  # Left command
        keyboard.press(Key.left)
        keyboard.release(Key.left)
        print("Left arrow pressed")
    elif command == 'R':  # Right command
        keyboard.press(Key.right)
        keyboard.release(Key.right)
        print("Right arrow pressed")
    else:
        print(f"Unknown command: {command}")

def main():
    """
    Main function to listen for serial data and process it.
    """
    print("Listening for serial data...")
    begin_mapping = 0  # Flag to start mapping commands to keys
    try:
        while True:
            # Check if there is incoming serial data
            if ser.in_waiting > 0:
                data = ser.read(1).decode('utf-8').strip()  # Read one byte and decode
                if data == "Z":  # Signal to begin command mapping
                    begin_mapping = 1
                if begin_mapping == 1:
                    map_command_to_key(data)  # Map valid commands to key presses
                else:
                    print(data)  # Print raw data before mapping starts
    except KeyboardInterrupt:
        print("Program stopped by user.")  # Gracefully exit on Ctrl+C
    except Exception as e:
        print(f"Error: {e}")  # Print any other exceptions
    finally:
        ser.close()  # Close the serial port when the program ends

if __name__ == "__main__":
    main()
