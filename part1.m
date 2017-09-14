% time
t = linspace(-1,1,2048);
 
% message signal (1Hz cosine message)
ms = 1;
mt = cos(2*pi*ms*t);
 
% carrier signal (10Hz cosine wave)
cs = 10;
ct = cos(2*pi*cs*t);
 
% DSB-SC
DSBSC = mt.*ct;
 
% Magnitude Spectrum
[H,w] = spectra(DSBSC,20,0.01);
 
subplot(2,1,1);
plot(t,DSBSC);
title('Plot of Modulated Signal');
xlabel({'Time',''});
ylabel('Amplitude');
 
subplot(2,1,2);
plot(w,abs(fftshift(H)));
title('Plot of Magnitude Spectrum');
xlabel('Frequency');
ylabel('Amplitude');
