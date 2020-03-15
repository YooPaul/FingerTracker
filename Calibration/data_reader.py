import serial
import numpy as np

ser = serial.Serial('COM4', 115200)
ser.flushInput()

data_samples = []

while True:
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

print('Collected ' + str(len(data_samples)) + ' data samples!')
print(np.var(data_samples))