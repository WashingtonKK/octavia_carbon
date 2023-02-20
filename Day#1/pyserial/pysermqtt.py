import serial
import paho.mqtt.client as mqtt

# Set up the serial connection
ser = serial.Serial('/dev/ttyACM0', 9600) # Replace '/dev/ttyACM0' with the appropriate serial port for your Arduino

# Set up the MQTT client
client = mqtt.Client()
client.connect("broker.hivemq.com", 1883) # Replace with your own MQTT broker address and port number

while True:
    # Read a line from the serial port
    line = ser.readline().decode('utf-8').strip()
    
    # Split the line into individual numbers
    numbers = line.split(',')
    
    # Send each number via MQTT
    for i in range(len(numbers)):
        topic = "mytopic/{}".format(i)
        message = str(numbers[i])
        client.publish(topic, message)
