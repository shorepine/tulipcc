import math
import sys
sys.path.append('.')

from snippets import scipy
from ulab import numpy as np

np.set_printoptions(threshold=100)

a = [4, 3, 1, 2-2j, 2+2j, 2-1j, 2+1j, 2-1j, 2+1j, 1+1j, 1-1j]
#print('_cplxreal: ', scipy.cplxreal(a))
f = np.array([-1.0, -1.0, -1.0, -1.0, 1.0, 1.0, 1.0], dtype=np.float)
t = (0.9984772174419884+0.01125340518638924j)
w = 'real'
#print('nearest_real_complex_idx: ', scipy.nearest_real_complex_idx(f,t,w))


nyquistRate = 48000 * 2
centerFrequency_Hz = 480.0
lowerCutoffFrequency_Hz = centerFrequency_Hz/math.sqrt(2)
upperCutoffFrequenc_Hz = centerFrequency_Hz*math.sqrt(2)
wn = np.array([ lowerCutoffFrequency_Hz, upperCutoffFrequenc_Hz])/nyquistRate

z = [] 
p = np.array([-0.1564344650402309+0.9876883405951379j, -0.4539904997395468+0.8910065241883679j, 
-0.7071067811865476+0.7071067811865475j, -0.8910065241883679+0.4539904997395467j, -0.9876883405951379+0.1564344650402309j, 
-0.9876883405951379-0.1564344650402309j, -0.8910065241883679-0.4539904997395467j, -0.7071067811865476-0.7071067811865475j, 
-0.4539904997395468-0.8910065241883679j, -0.1564344650402309-0.9876883405951379j], dtype=np.complex) 
k = 1
wo = 0.1886352115099219

print(scipy.lp2hp_zpk(z,p,k,wo))

z = np.array([0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0], dtype=np.float)
p = np.array([-0.02950904840030544-0.1863127990340476j, -0.08563859394186457-0.1680752041469931j, 
-0.1333852372292245-0.1333852372292244j, -0.1680752041469931-0.08563859394186453j, -0.1863127990340476-0.02950904840030543j,
 -0.1863127990340476+0.02950904840030543j, -0.1680752041469931+0.08563859394186453j, -0.1333852372292245+0.1333852372292244j,
  -0.08563859394186457+0.1680752041469931j, -0.02950904840030544+0.1863127990340476j], dtype=np.complex)
k = 1.0
fs = 2.0

print(scipy.bilinear_zpk(z,p,k,fs)) 

z = np.array([], dtype=np.float)
p = np.array([-0.3826834323650898+0.9238795325112868j, 
-0.9238795325112868+0.3826834323650898j, -0.9238795325112868-0.3826834323650898j, 
-0.3826834323650898-0.9238795325112868j], dtype=np.complex)
k = 1
wo = 0.03141673402115484
bw = 0.02221601345771878
print(scipy.lp2bs_zpk(z, p, k, wo=wo, bw=bw))

print(scipy.butter(N=4, Wn=wn, btype='bandpass', analog=False, output='ba'))
print(scipy.butter(N=4, Wn=wn, btype='bandpass', analog=False, output='zpk'))
print(scipy.butter(N=4, Wn=wn, btype='bandpass', analog=False, output='sos'))
print(scipy.butter(N=4, Wn=wn, btype='bandstop', analog=False, output='ba'))
print(scipy.butter(10, 15, 'lp', fs=1000, output='sos'))
print(scipy.butter(10, 15, 'hp', fs=1000, output='sos'))

