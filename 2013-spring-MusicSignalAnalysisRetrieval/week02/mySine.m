function outputSignal = mySine(duration, freq)
%R01922024
fs = 16000;
time = (0:duration*fs-1)/fs;
freq(2) = ( freq(1) + freq(2) )/2;
f    = (freq(1):(freq(2)-freq(1))/(length(time)-1):freq(2));
q    = time.*f;

outputSignal = sin(2*pi*q)';

return;
