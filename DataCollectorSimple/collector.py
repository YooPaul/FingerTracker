import serial
import time
import json

mag_port = 'COM11'
mag = serial.Serial(mag_port, 500000)


def print_line(serial_port):
    ser_bytes = serial_port.readline()
    decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode('utf-8')
    print(decoded_bytes)


def read_mag_data(serial_port, data_list):
    ser_bytes = serial_port.readline()
    decoded_str = ser_bytes[0:len(ser_bytes)-2].decode('utf-8')
    measurements = decoded_str.split(':')
    
    data_map = {}
    for i in range(0, 4):
        values = measurements[i].split(',')
        data_map[i] = (float(values[1]), float(values[2]), float(values[3]))
    
    data_list.append(data_map)
    # print(measurements[0] + ':' + measurements[1] + ':' + measurements[2] + ':' + measurements[3])


print_line(mag)

mag.write('9'.encode('ascii'))

mag.flushInput()


data_samples = []
data_json = {}
data_json['samples'] = data_samples;
data_json['start'] = time.time()
while True:
    try:
        read_mag_data(mag, data_samples);
    except KeyboardInterrupt as e:
        # Record the end time
        data_json['end'] = time.time()
        # Save the data        
        with open('data_' + str(data_json['end']) + '.json', 'w') as outfile:
            json.dump(data_json, outfile)
        print('Data rate of ' + str(len(data_json['samples']) / (data_json['end'] - data_json['start'])))
        exit()
    except:
        pass