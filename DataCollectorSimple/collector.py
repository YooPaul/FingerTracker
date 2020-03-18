import serial
import numpy as np

mag_port = 'COM11'

mag = serial.Serial(mag_port, 115200)


def print_line(serial_port):
    ser_bytes = serial_port.readline()
    decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode('utf-8')
    print(decoded_bytes)


def read_mag_data(serial_port):
    ser_bytes = serial_port.readline()
    decoded_str = ser_bytes[0:len(ser_bytes)-2].decode('utf-8')
    measurements = decoded_str.split(':')
    print(measurements[0] + ':' + measurements[1] + ':' + measurements[2] + ':' + measurements[3])


print_line(mag)

mag.write('9'.encode('ascii'))

mag.flushInput()

while True:
    try:
        read_mag_data(mag);
    except KeyboardInterrupt as e:
        exit()
    except:
        pass