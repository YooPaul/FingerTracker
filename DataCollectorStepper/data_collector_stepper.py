import serial
import time
import json

mag_port = 'COM11'
stepper_port = 'COM12'
mag = serial.Serial(mag_port, 1000000)
stepper = serial.Serial(stepper_port, 115200)


def print_line(serial_port):
    ser_bytes = serial_port.readline()
    decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode('utf-8')
    print(decoded_bytes)


print_line(mag)
mag.write('8'.encode('ascii'))
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

data_json['points'] = {}

current_x_pos = 0
current_y_pos = 0
for x in range(-2, 3):
    for y in range(-2, 3):
        step_dif_x = (x - current_x_pos) * step_size
        step_dif_y = (y - current_y_pos) * step_size
        current_x_pos = x
        current_y_pos = y
        
        print('Moving to ' + str(x) + ' ' + str(y))
        # First move the stepper motors
        stepper.write(('moveTo ' + str(-step_dif_x) + ' ' + str(-step_dif_y)).encode('ascii'))
        stepper.flushInput()
        print_line(stepper)
        
        time.sleep(1)
        # Then make the measurements
        print('Measuring data at ' + str(x) + ' ' + str(y))
        samples_list = []
        # Clear the serial buffer of old data
        mag.reset_input_buffer()
        mag.readline()
        for i in range(0, 1000):
            ser_bytes = mag.readline()
            decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode('utf-8')
            samples_list.append(decoded_bytes)
        
        data_json['points'][str(-x * step_size) + ' ' + str(-y * step_size)] = samples_list
        

# Record the end time
data_json['end'] = time.time()
# Save the data        
with open('data_' + str(data_json['end']) + '.json', 'w') as outfile:
    json.dump(data_json, outfile)
print('Data rate of ' + str(len(data_json['samples']) / (data_json['end'] - data_json['start'])))
