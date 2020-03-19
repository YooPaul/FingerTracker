import json
import numpy as np

mm_per_step = 78.0 / 4000.0
z_real = -73.4 # in mm, depends on data used

# data order is 
# 0: (0,0,1)
# 1: (0,0,0)
# 2: (1,-1,0)
# 3: (1,1,0)

def compute_pos_from_samples(calibrated_samples):
    sensor_mag = {'0': [], '1': [], '2': [], '3': []}
    for sample in calibrated_samples:
        # print(sample)
        sample_split = sample.split(' ')
        for i in range(0, 4):
            mag_at_i = np.sqrt(float(sample_split[i]))
            sensor_mag[str(i)].append(mag_at_i)
    # print(sensor_mag['0'])
    # Test it on sensor 0
    signal = np.array(sensor_mag['1'], dtype=float)
    
    sp = np.fft.fft(signal)
    import matplotlib.pyplot as plt
    t = np.linspace(0, 1.7, 1000)
    s = signal # np.sin(40 * 2 * np.pi * t) + 0.5 * np.sin(90 * 2 * np.pi * t)

    plt.ylabel("Amplitude")
    plt.xlabel("Time [s]")
    plt.plot(t, s)
    plt.show()
    
    fft = np.fft.fft(s)
    T = t[1] - t[0]  # sampling interval 
    N = s.size

    # 1/T = frequency
    f = np.linspace(0, 1 / T, N)

    plt.ylabel("Amplitude")
    plt.xlabel("Frequency [Hz]")
    # plt.bar(f[:N // 2], np.abs(fft)[:N // 2] * 1 / N, width=1)  # 1 / N is a normalization factor
    # plt.show()
    
    fcoord = f[:N // 2]
    fcoord2 = fcoord[10:]
    fmag = np.abs(fft)[:N // 2] * 1 / N
    fmag2 = fmag[10:]
    maxA = np.amax(fmag2)
    fmax = np.where(fmag2 == np.amax(fmag2))
    print("max A=",maxA)
    # print("Index=",fmax)
    print("max freq=",fcoord2[fmax])
    
    plt.bar(fcoord, fmag, width=1)  # 1 / N is a normalization factor
    plt.show()
    
    
    
    
    # t = np.arange(256)
    # sp = np.fft.fft(np.sin(t))
    # freq = np.fft.fftfreq(500)
    # plt.plot(freq, sp.real, freq, sp.imag)
    # plt.show()
    # exit()
            


with open('xy_8_inches_80hz.json') as json_file:
    data = json.load(json_file)
    step_size = data['stepsize']
    for x in range(-2, 3):
        for y in range(-2, 3):
            x_coord = -x * step_size
            y_coord = -y * step_size
            samples_list = data['points'][str(x_coord) + ' ' + str(y_coord)]
            
            x_real = x_coord * mm_per_step
            y_real = y_coord * mm_per_step
            
            # print('x_real: ' + str(x_real))
            # print('y_real: ' + str(y_real))
            # print('z_real: ' + str(z_real))
            
            # print(samples_list[0])
            # calibrated = calirate_data(samples_list[0])
            # print(calibrated)

            compute_pos_from_samples(samples_list)


print('done!')