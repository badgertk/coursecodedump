% Generate the message signal

tstep = 0.0001;
tv = (0:tstep:0.15);
tin1 = (0:tstep:0.05-tstep);
tin2 = (0.05:tstep:0.1-tstep);
tin3 = (0.1:tstep:0.15);
mess = [ones(1,length(tin1)) -2*ones(1,length(tin2));
zeros(1,lenth(tin3));

% Compute the integral of the message signal using cummulative trapezoid integration
intmess = cumtrapz(tv,mess);

% Plots
% message signal
subplot(2,1,1);
plot(tv,mess,'r','LineWidth',2);
axis([0 0.15 -2.1 1.1])
title ('');
xlabel('time');
ylabel('message[n]');

% integral of message signal
subplot(2,1,2);
plot(tv,intmess,'b','LineWidth',2);
axis([0 0.15 -0.1 0.1])
title ('');
xlabel('time');
ylabel('int message[n]');

% Plot the FM signal using the expression from part A
ut = cos((2*pi*300.*tv)+(2*pi*50).*mt_int);

plot(tv,ut,'k');
axis([0 0.15 -1.1 1.1]);
title('Plot of FM Signal');
xlabel('time');
ylabel('Amplitude');


% Plot the Spectra of m(t) and u(t)
[Hmt,freqmt] = spectra(mt,20000,0.25);
[Hut,frequt] = spectra(ut,20000,0.25);

subplot(2,1,1); plot(freqmt,abs(fftshift(Hmt))); title('Spectra of Message Signal'); 
xlabel({'Frequency (Hz)',''}); ylabel('Amplitude');

subplot(2,1,2); plot(frequt,abs(fftshift(Hut))); title('Spectra of FM Signal'); xlabel({'Frequency 
(Hz)',''});
ylabel('Amplitude');
