# PYTHON 2
# I can port to python3, if desired
import pycuda.autoinit
import pycuda.driver as cuda
import numpy
import matplotlib.pyplot as plt
import ishne
from pycuda.compiler import SourceModule
import sys
from timer import Timer

with open("wavelet2.cu") as wavelet_file:
    mod = SourceModule(wavelet_file.read())

mexican_hat = mod.get_function("mexican_hat")
cross_correlate_with_wavelet = mod.get_function("cross_correlate_with_wavelet")

# Read the ISHNE file
ecg = ishne.ISHNE(sys.argv[1])
ecg.read()

# Convert to floats (optimization: can we use half floats?)
lead = ecg.leads[0].astype(numpy.float32)
lead_size = len(lead)

# number of samples: 0.06 - 0.1 * SAMPLING_RATE (QRS Time: 60-100ms)
num_samples = int(0.08 * ecg.sampling_rate)
# The math suggests 16 samples is the width of the QRS complex
# Measuring the QRS complex for 9004 gives 16 samples
# Measured correlated peak 7 samples after start of QRS
hat = numpy.zeros(num_samples).astype(numpy.float32)
# Mexican hats seem to hold a nonzero value between -4 and 4 w/ sigma=1
sigma = 1.0
maxval = 4 * sigma
minval = -maxval

# Sampled chunk of ECG @ first QRS
# TODO find dynamically
qrs_wavelet = numpy.asarray([0.0, -0.23, -0.69999999, -0.89999998, -0.58999997, -0.12, 0.41, 0.94, 1.0, 0.92000002, 0.75999999, 0.52999997, 0.37, 0.20999999, 0.12, 0.1], numpy.float32)
with Timer() as hat_time:
    mexican_hat(cuda.Out(hat), numpy.float32(sigma), numpy.float32(minval), numpy.float32((maxval - minval)/num_samples), grid=(1, 1), block=(int(ecg.sampling_rate), 1, 1))
print "Daubechies 4 (db4) Generation: {} sec".format(hat_time.interval)

# Output values
correlated_hat = numpy.zeros(lead_size).astype(numpy.float32)
correlated_qrs = numpy.zeros(lead_size).astype(numpy.float32)

# Kernel Parameters
threads_per_block = 200
num_blocks = lead_size / threads_per_block

with Timer() as cor_time_1:
    cross_correlate_with_wavelet(cuda.Out(correlated_qrs), cuda.In(lead), cuda.In(qrs_wavelet), numpy.int32(lead_size), numpy.int32(num_samples), grid=(num_blocks, 1), block=(threads_per_block, 1, 1))
with Timer() as cor_time_2:
    cross_correlate_with_wavelet(cuda.Out(correlated_hat), cuda.In(lead), cuda.In(hat), numpy.int32(lead_size), numpy.int32(num_samples), grid=(num_blocks, 1), block=(threads_per_block, 1, 1))

print "Correlation w/ Mexican Hat: {} sec".format(cor_time_2.interval)
print "Correlation w/ QRS: {} sec".format(cor_time_1.interval)

# How many points to plot
chunk = 10000

plt.figure(1)
x = numpy.linspace(0, chunk, num=chunk)
plt.plot(x, correlated_hat[:chunk], 'b', x, correlated_qrs[:chunk], 'r', x, lead[:chunk], 'g')
plt.show()
