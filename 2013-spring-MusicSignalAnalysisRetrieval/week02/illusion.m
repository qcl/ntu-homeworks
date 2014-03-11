%R01922024
%Week #2

clear all;

[y,fs] = wavread('qcl.wav');
time = ( 1:length(y) )/fs;

tp = 1;
vp = 1;

w = [y.*sin(time*tp)'*vp y.*cos(time*tp)'*vp];

soundsc(w,fs);
