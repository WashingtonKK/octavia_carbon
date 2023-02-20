import serial.tools.list_ports
import serial
import paho.mqtt.client as mqtt

# Scan for the Arduino serial port
arduino_port = None
ports = serial.tools.list_ports.comports()

for p in ports:
    if 'Arduino' in p.description:
        arduino_port = p.device

if arduino_port is None:
    print("Arduino not found!")
    exit()

# Set up the serial connection
#ser = serial.Serial(arduino_port, 9600) -- Works
ser = serial.Serial('/dev/ttyACM0', 9600)

# Set up the MQTT client
#client = mqtt.Client()
#client.connect("broker.hivemq.com", 1883)

while True:
    # Read a line from the serial port
    line = ser.readline().decode('utf-8').strip()
    
    # Split the line into individual numbers
    numbers = line.split(',')
    
    # Send each number via MQTT
    for i in range(len(numbers)):
        #topic = "mytopic/{}".format(i)
        #message = str(numbers[i])
        #client.publish(topic, message)
        print(numbers[i])
