% time
t = linspace(-1,1,2048);
 
% message signal
ms = 1;
mt = cos(2*pi*ms*t);
 
% carrier signal
cs = 10;
ct = cos(2*pi*cs*t);
 
% DSB-AM
%10 modulation
K1 = 0.1;
DSBAM1 = (1 + K1.*mt).*ct;
[H1,w1] = spectra(DSBAM1,20,0.01);

%40 modulation
K2 = 0.4;
DSBAM2 = (1 + K2.*mt).*ct;
[H2,w2] = spectra(DSBAM2,20,0.01);

%60 modulation
K3 = 0.6;
DSBAM3 = (1 + K3.*mt).*ct;
[H3,w3] = spectra(DSBAM3,20,0.01);

%110 modulation
K4 = 1.1;
DSBAM4 = (1 + K4.*mt).*ct;
[H4,w4] = spectra(DSBAM4,20,0.01);

%100 modulation
K5 = 1.0;
DSBAM5 = (1 + K5.*mt).*ct;
[H5,w5] = spectra(DSBAM5,20,0.01);
 
subplot(2,1,1);
plot(t,DSBAM1);
title(['Plot of Modulated Signal with MI of ' num2str(K1)]);
xlabel({'Time',''});
ylabel('Amplitude');
 
subplot(2,1,2);
plot(w1,abs(fftshift(H1)));
title('Plot of Magnitude Spectrum');
xlabel('Frequency');
ylabel('Amplitude');
