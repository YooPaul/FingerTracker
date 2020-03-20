import json
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import fsolve

mm_per_step = 78.0 / 4000.0
z_real = -48 # in mm, depends on data used

# data order is 
# 0: (0,0,1)
# 1: (0,0,0)
# 2: (1,-1,0)
# 3: (1,1,0)



import numpy as np
from scipy.optimize import minimize

def mag_error(em,ps,Bsdata):
#
#   cost function parameters
#   pm: guessed position of electromagnet
#   ps: list of sensor coordinates (4 sensors)
#   Bsdata: B sensor data vector
#
    rc = 2.0e-3  # 2 mm radius core ?
    Nc = 100     # 100 turns ?
    I = .3      # 1 amp ?
    mu0 = 1.256e-7  # vacuum permeability in air (H/m)
    muc = 1.0e3     # relative core permeability (guess)
    K = mu0*muc*I*Nc*rc*rc/4.0
    K = K / (4/1.13) # Correction factor
    K2 = K*K
    # print('K is ', K)
    something = K * 2 * ((.05) ** -3)
    # print('something is ', something)
    # wait = input()
    """The magnetic error function"""
    xm,ym,zm = em  # guesed electromagnet coordinates
    error = 0.0
    for i in range(0,4):  # for all sensors 
        # get coordinates of sensor
        xs,ys,zs = ps[i]
        # calculate radius
        r = np.sqrt((xs-xm)*(xs-xm)+(ys-ym)*(ys-ym)+(zs-zm)*(zs-zm))
        # calculate cosine
        c = (zm-zs)/r
        #calculate the magnitue of expected B square
        Bc = (K/r/r/r)*np.sqrt(3.0*c*c+1.0)
        # print('ps is ', ps)
        # print('xm is ', xm)
        # print('radius is ', r)
        # print(Bc)
        error = error + (Bc-Bsdata[i]) * (Bc-Bsdata[i])
    # puase = input()
    # print('error is ', error)
    error2 = np.sqrt(error) / 17e-6
    # print('error is ', error2)
    return error2


def compute_amplitude_from_FFT(sensor_data, i):
    # Numpy FFT computation, based on this tutorial https://www.ritchievink.com/blog/2017/04/23/understanding-the-fourier-transform-by-example/
    signal = np.array(sensor_data, dtype=float)
    
    sp = np.fft.fft(signal)

    t = np.linspace(0, 1.7, 1000)
    s = signal 

    # plt.ylabel("Amplitude")
    # plt.xlabel("Time [s]")
    # plt.plot(t, s)
    # plt.show()
    
    fft = np.fft.fft(s)
    T = t[1] - t[0]  # sampling interval 
    N = s.size

    # 1/T = frequency
    f = np.linspace(0, 1 / T, N)

    # plt.ylabel("Amplitude")
    # plt.xlabel("Frequency [Hz]")
    # plt.bar(f[:N // 2], np.abs(fft)[:N // 2] * 1 / N, width=1)  # 1 / N is a normalization factor
    # plt.show()
    
    fcoord = f[:N // 2]
    fcoord2 = fcoord[10:]
    fmag = np.abs(fft)[:N // 2] * 1 / N
    fmag2 = fmag[10:]
    maxA = np.amax(fmag2)
    fmax = np.where(fmag2 == np.amax(fmag2))
    # print("max A=",maxA)
    # print("Index=",fmax)
    if (False): # fcoord2[fmax] < 80):
        print("max freq=",fcoord2[fmax])
        print(i)
    
        plt.ylabel("Amplitude")
        plt.xlabel("Time [s]")
        plt.plot(t, s)
        plt.show()
        plt.bar(fcoord, fmag, width=1)  # 1 / N is a normalization factor
        plt.show()

    return maxA



def compute_pos_from_samples(calibrated_samples, ref_pos):
    # print(ref_pos)
    sensor_data = {}
    for i in range(0, 12):
        sensor_data[str(i)] = []
    
    for sample in calibrated_samples:
        # print(sample)
        sample_split = sample.split(' ')
        if (len(sample_split) != 13):
            print('wtf!')
            print(sample_split)
            exit()
        for i in range(0, 12):
            sensor_data[str(i)].append(float(sample_split[i]))

    componenet_amp = {}
    for i in range(0, 12):
        componenet_amp[i] = compute_amplitude_from_FFT(sensor_data[str(i)], i)

    amplitude_map = {}
    for i in range(0, 4):
        x_comp = componenet_amp[i * 3]
        y_comp = componenet_amp[(i * 3) + 1]
        z_comp = componenet_amp[(i * 3) + 2]
        amplitude_map[i] = np.sqrt((x_comp * x_comp) + (y_comp * y_comp) + (z_comp * z_comp)) / 1000000 # Put into Tesla
        
    if (x == 0):
        # print(str(amplitude_map[0]) + ' : ' + str(amplitude_map[1]) + ' : ' + str(amplitude_map[2]) + ' : ' + str(amplitude_map[3]))
        pass

    
    em = np.array([1/1000,0.0,-60.0/1000]) # guess of coordinate
    Bsdata = np.array([ amplitude_map[0],amplitude_map[1],amplitude_map[2],amplitude_map[3] ]) # enter B amplitude for each sensor

    # put the actual sensor coordinates

    ps0 = np.array([0,0,21.6/1000])
    ps1 = np.array([0,0,0])
    ps2 = np.array([20.4/1000,-20.4/1000,0])
    ps3 = np.array([20.4/1000,20.4/1000,0])

    ps = [ps0, ps1, ps2, ps3]

    bnds = ((-20/1000, 20/1000), (-20/1000, 20/1000), (-90/1000, -30/1000))
    # here do the optimization
    res = minimize(mag_error, em, args=(ps,Bsdata), method='BFGS',jac=None, bounds=bnds,tol=1.0e-6,options={'disp': True})
    
    # print(res.x)
    
    
    #  print(Bsdata)
    # print(ref_pos)
    # INSERT POSITION CODE HERE
    return res.x
    
# def computeEstimate(mag_pos, )
    
            


with open('crap_2.json') as json_file:
    data = json.load(json_file)
    step_size = data['stepsize']
    for x in range(-2, 3):
        for y in range(-2, 3):
            x_coord = -x * step_size
            y_coord = -y * step_size
            samples_list = data['points'][str(x_coord) + ' ' + str(y_coord)]
            
            x_real = -(x_coord * mm_per_step) # correction
            y_real = -y_coord * mm_per_step
            
            # print('x_real: ' + str(x_real))
            # print('y_real: ' + str(y_real))
            # print('z_real: ' + str(z_real))
            
            # print(samples_list[0])
            # calibrated = calirate_data(samples_list[0])
            # print(calibrated)
            real = (y_real / 1000, x_real / 1000, z_real / 1000)
            estimate = compute_pos_from_samples(samples_list, (x_real, y_real, z_real))
            print('real vs estimate')
            print(real)
            print(estimate)


print('done!')