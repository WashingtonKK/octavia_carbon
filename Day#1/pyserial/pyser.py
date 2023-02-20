import serial

# Set up the serial connection
ser = serial.Serial('/dev/ttyACM0', 9600) # Replace '/dev/ttyACM0' with the appropriate serial port for your Arduino

while True:
    # Read a line from the serial port
    line = ser.readline().decode('utf-8').strip()
    
    # Split the line into individual numbers
    numbers = line.split(',')
    
    # Print the numbers to the console
    print(numbers)
