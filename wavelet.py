import pycuda.autoinit
import pycuda.driver as cuda
import numpy
import matplotlib.pyplot as plt
import ishne
from pycuda.compiler import SourceModule
import sys

with open("wavelet_mod.cu") as wavelet_file:
    mod = SourceModule(wavelet_file.read())

calculate_hats = mod.get_function("calculate_hats")
cross_correlate_with_hat = mod.get_function("cross_correlate_with_hat")

ecg = ishne.ISHNE(sys.argv[1])
ecg.read()
lead = ecg.leads[0].astype(numpy.float32)[:1000]

hats = numpy.zeros(170 * 21).astype(numpy.float32)
correlated = numpy.zeros(1000).astype(numpy.float32)

calculate_hats(cuda.Out(hats), grid=(170, 1), block=(21, 1, 1))

cross_correlate_with_hat(cuda.Out(correlated), cuda.In(lead), cuda.In(hats[:21]), numpy.int32(10000), grid=(1, 1), block=(1000, 1, 1))

plt.figure(1)
x = numpy.linspace(0, 100, num=1000)
plt.plot(x,correlated, 'r', x, lead, 'g')
plt.show()

# print hats[:21]
