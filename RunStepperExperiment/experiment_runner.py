import serial
import numpy as np

mag_port = 'COM_X'
stepper_port = 'COM12'

# mag = serial.Serial(mag_port, 115200)
stepper = serial.Serial(stepper_port, 115200)


def print_line(serial_port):
    ser_bytes = stepper.readline()
    decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode('utf-8')
    print(decoded_bytes)


print_line(stepper)

stepper.write('moveToCenter'.encode('ascii'))

stepper.flushInput()

print_line(stepper)

print('done!')

while False:
    try:
        ser_bytes = ser.readline()
        decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode('utf-8')
        numbers = decoded_bytes.split('\t')
        if len(numbers) == 3:
            print(decoded_bytes)
            print(float(numbers[0]))
            print(float(numbers[1]))
            print(float(numbers[2]))
            
            # data_samples.append((float(numbers[0]), float(numbers[1]), float(numbers[2])))
            data_samples.append((float(numbers[2])))
            if len(data_samples) >= 1000:
                break;
    except:
        print("Keyboard Interrupt")
        break

# print('Collected ' + str(len(data_samples)) + ' data samples!')
# print(np.var(data_samples))