
% Part A
% Generate sequence of uniformly distributed random variable (-0.5,0.5)
X_n = -0.5 + rand(1,1000);
% Generate output
Y_n(1) = 0;
for i = 2:1000
Y_n(i) = 0.7*Y_n(i-1) + X_n(i);

end

% Plot input and output plot(1:1000,X_n,'r'); hold on; plot(1:1000,Y_n,'b'); xlabel('N (bin)'); 
ylabel('Magnitude');
title('White Noise and Output after LPF');
legend('Input White Noise','Output');

% Part B
% Calculate the autocorrelation of X_n and Y_n
R_x = AUTOCORRELATION(X_n,49);
R_y = AUTOCORRELATION(Y_n,49);

% Plot the autocorrelations subplot(1,2,1);
plot(R_x);
xlabel({'N (bin)',' '});
ylabel('Magnitude');
title('Autocorrelation of X_n : R_x');
subplot(1,2,2);
plot(R_y); xlabel('N (bin)'); ylabel('Magnitude');
title('Autocorrelation of Y_n : R_y');

% Part C
% Calculate the power spectra
S_x = fftshift(abs(fft(Rx)));
S_y = fftshift(abs(fft(Ry)));

% Plot the power spectra subplot(1,2,1); plot(S_x);
xlabel({'N (bin)',' '});
ylabel('Magnitude');
title('Power Spectra of R_x : S_x');
subplot(1,2,2);
plot(S_y);
xlabel('N (bin)');
ylabel('Magnitude');
title('Power Spectra of R_y : S_y');
