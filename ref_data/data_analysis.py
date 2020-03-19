import json
import numpy as np

# Magnetometer calibration constants
mag_off_4 = (-14.85, 34.21, -50.03)# (0,0,1)
mag_map_4 = (0.980, 0.048, -0.005, 0.048, 1.011, -0.003, -0.005, -0.003, 1.012)
mag_mag_4 = 45.26

mag_off_5 = (6.90, -24.74, 3.78) # (0,0,0)
mag_map_5 = (1.002, 0.037, -.008, 0.037, 1.020, -0.001, -0.008, -.001, .979) # Magnetic Mapping, read from left to right top to bottom (Row Major)
mag_mag_5 = 44.33

mag_off_6 = (-15.92, -8.41, -7.70) # (1,-1,0)
mag_map_6 = (1.000, 0.052, -0.016, 0.052, 1.006, -0.008, -0.016, -0.008, 0.997)
mag_mag_6 = 44.91

mag_off_7 = (3.81, -25.75, -14.32) # (1,1,0)
mag_map_7 = (0.988, 0.039, -0.011, 0.039, 1.051, 0.006, -0.011, 0.006, 0.965)
mag_mag_7 = 42.97

off_map = {'0': mag_off_4, '1': mag_off_5, '2': mag_off_6, '3': mag_off_7}
transform_map = {'0': mag_map_4, '1': mag_map_5, '2': mag_map_6, '3': mag_map_7}
mag_map = {'0': mag_mag_4, '1': mag_mag_5, '2': mag_mag_6, '3': mag_mag_7}

mm_per_step = 78.0 / 4000.0

z_real = -73.4 # in mm, depends on data used

def apply_transform(data_vector, matrix):
    new_x = data_vector[0] * matrix[0] + data_vector[1] * matrix[3] + data_vector[2] * matrix[6]
    new_y = data_vector[0] * matrix[1] + data_vector[1] * matrix[4] + data_vector[2] * matrix[7]
    new_z = data_vector[0] * matrix[2] + data_vector[1] * matrix[5] + data_vector[2] * matrix[8]
    return (new_x, new_y, new_z)
    

def compute_calibration(data_vector, id):
    offsets = off_map[id]
    transform = transform_map[id]
    magnitude = mag_map[id]
    
    new_x = data_vector[0] - offsets[0]
    new_y = data_vector[1] - offsets[1]
    new_z = data_vector[2] - offsets[2]
    transformed = apply_transform((new_x, new_y, new_z), transform)
    scaled = (transformed[0] / magnitude, transformed[1] / magnitude, transformed[2] / magnitude)
        
    return scaled

def calirate_data(original_data):
    new_data = {}
    new_data['time'] = original_data['time']
    for i in range(0,4):
        data = original_data[str(i)]
        calibrated = compute_calibration(data, str(i))
        new_data[str(i)] = calibrated
        
    return new_data


def calibrate_samples(raw_samples):
    calibrated_samples = []
    for i in range(0, len(raw_samples)):
        calibrated_data = calirate_data(raw_samples[i])
        calibrated_samples.append(calibrated_data)
    return calibrated_samples


def compute_pos_from_samples(calibrated_samples):
    sensor_mag = {'0': [], '1': [], '2': [], '3': []}
    for sample in calibrated_samples:
        for i in range(0, 4):
            field_at_i = sample[str(i)]
            mag_of_i = np.sqrt((field_at_i[0] * field_at_i[0]) + (field_at_i[1] * field_at_i[1]) + (field_at_i[2] * field_at_i[2]))
            sensor_mag[str(i)].append(mag_of_i)
    print(sensor_mag['0'])
    # Test it on sensor 0
    signal = np.array(sensor_mag['0'], dtype=float)
    
    sp = np.fft.fft(signal)
    import matplotlib.pyplot as plt
    t = np.linspace(0, 0.5, 500)
    s = signal # np.sin(40 * 2 * np.pi * t) + 0.5 * np.sin(90 * 2 * np.pi * t)

    plt.ylabel("Amplitude")
    plt.xlabel("Time [s]")
    plt.plot(t, s)
    plt.show()
    # t = np.arange(256)
    # sp = np.fft.fft(np.sin(t))
    # freq = np.fft.fftfreq(500)
    # plt.plot(freq, sp.real, freq, sp.imag)
    # plt.show()
    # exit()
            


with open('xy_8_inches_should_work.json') as json_file:
    data = json.load(json_file)
    step_size = data['stepsize']
    for x in range(-2, 3):
        for y in range(-2, 3):
            x_coord = -x * step_size
            y_coord = -y * step_size
            samples_list = data[str(x_coord) + ' ' + str(y_coord)]
            
            x_real = x_coord * mm_per_step
            y_real = y_coord * mm_per_step
            
            # print('x_real: ' + str(x_real))
            # print('y_real: ' + str(y_real))
            # print('z_real: ' + str(z_real))
            
            # print(samples_list[0])
            # calibrated = calirate_data(samples_list[0])
            # print(calibrated)
            calibrated_list = calibrate_samples(samples_list)
            # print(len(calibrated_list))
            compute_pos_from_samples(calibrated_list)


print('done!')