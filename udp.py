import socket
import serial

# Serial COM port (Arduino)
COM_PORT = "COM6"
BAUD_RATE = 115200

# UDP settings (must match FlyWithLua)
UDP_IP = "127.0.0.1"
UDP_PORT = 49000

# Open Serial port
ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=1)

# Set up UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print(f"Listening on UDP {UDP_IP}:{UDP_PORT}, forwarding to {COM_PORT}")

def sendValue(dataref, value, lastValue):
    if value != lastValue:
        valToSend = f"{dataref}={value}"
        ser.write((valToSend + "\n").encode('utf-8'))
        #print(dataref + ":" + value)
        lastValue = value

lastIAS = None  # Track IAS
lastALT = None  # Track ALT
lastHDG = None  # Track HDG
lastBeacon = None  # Track Beacon state

while True:
    data, addr = sock.recvfrom(1024)  # buffer size 1024 bytes
    line = data.decode('utf-8').strip()
    #IAS = int(line)
    #print("Received:", line)
    dataref, value = line.split('=')

    # IAS
    if dataref == "IAS":
        value = int(round(float(value), 0))
        if value >= 0  and value != lastIAS:
            value = str(value)
            sendValue(dataref, value, lastIAS)
            lastIAS = int(value)
            value = None
    elif dataref == "ALT":
        value = int(round(float(value), 0))
        if value >= 0  and value != lastALT:
            #value = value = f"{value:05}"
            value = str(value)
            sendValue(dataref, value, lastALT)
            lastALT = int(value)
            value = None
    elif dataref == "HDG":
        value = int(round(float(value), 0))
        if value >= 0  and value != lastHDG:
            value = value = f"{value:03}"
            value = str(value)
            sendValue(dataref, value, lastHDG)
            lastHDG = int(value)
            value = None
    elif dataref == "BEACON":
        if value != lastBeacon:
            sendValue(dataref, value, lastBeacon)
            lastBeacon = value
            value = None
 
