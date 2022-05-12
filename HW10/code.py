import time
from ulab import numpy as np

# Initialize signals
signal1 = np.zeros(1024)
signal2 = np.zeros(1024)
signal3 = np.zeros(1024)

# Initialize time array
t = np.linspace(0, 1024*0.05, 1024)
print(t)

# Modify Signals
signal1 = np.sin(2*3.14*1000*t)
signal2 = np.sin(2*3.14*2000*t)
signal3 = np.sin(2*3.14*3000*t)

# Summate and Produce FFT
signal = signal1+signal2+signal3
real, imaginary = np.fft.fft(signal)

# Plotting
for i in real:
    print("("+str(i)+",)")  # print with plotting format
    time.sleep(0.05)
