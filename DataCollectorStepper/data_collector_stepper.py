import serial
import time
import json

mag_port = 'COM11'
stepper_port = 'COM12'
mag = serial.Serial(mag_port, 500000)
stepper = serial.Serial(stepper_port, 115200)


def print_line(serial_port):
    ser_bytes = serial_port.readline()
    decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode('utf-8')
    print(decoded_bytes)


def read_mag_data(serial_port):
    ser_bytes = serial_port.readline()
    decoded_str = ser_bytes[0:len(ser_bytes)-2].decode('utf-8')
    measurements = decoded_str.split(':')
    
    data_map = {}
    for i in range(0, 4):
        values = measurements[i].split(',')
        try:
            data_map[i] = (float(values[1]), float(values[2]), float(values[3]))
        except:
            print('FAILED WITH ' + measurements[i])
            exit()
    
    data_map['time'] = time.time()
    return data_map
    # print(measurements[0] + ':' + measurements[1] + ':' + measurements[2] + ':' + measurements[3])


print_line(mag)
mag.write('9'.encode('ascii'))
mag.flushInput()


print_line(stepper)
stepper.write('moveToCenter'.encode('ascii'))
stepper.flushInput()
print_line(stepper)


data_samples = []
data_json = {}
data_json['samples'] = data_samples;
data_json['start'] = time.time()


step_size = 500

data_json['stepsize'] = step_size

current_x_pos = 0
current_y_pos = 0
for x in range(-2, 3):
    for y in range(-2, 3):
        step_dif_x = (x - current_x_pos) * step_size
        step_dif_y = (y - current_y_pos) * step_size
        current_x_pos = x
        current_y_pos = y
        
        # First move the stepper motors
        stepper.write(('moveTo ' + str(-step_dif_x) + ' ' + str(-step_dif_y)).encode('ascii'))
        stepper.flushInput()
        print_line(stepper)
        
        # Then make the measurements
        print('Measuring position at ' + str(x) + ' ' + str(y))
        samples_list = []
        for i in range(0, 500):
            data_map = read_mag_data(mag)
            samples_list.append(data_map)
        
        data_json[str(-x * step_size) + ' ' + str(-y * step_size)] = samples_list
        

# Record the end time
data_json['end'] = time.time()
# Save the data        
with open('data_' + str(data_json['end']) + '.json', 'w') as outfile:
    json.dump(data_json, outfile)
print('Data rate of ' + str(len(data_json['samples']) / (data_json['end'] - data_json['start'])))
