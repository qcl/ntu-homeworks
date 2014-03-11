clear all;

inputfile = './welcome.wav';

myEncript(inputfile,'tmp.wav');
myEncript('tmp.wav','tmp2.wav');
[ya,fa] = wavread(inputfile);
[yb,fb] = wavread('tmp.wav');
[yc,fc] = wavread('tmp2.wav');
time = (1:length(ya))/fa;

subplot(3,1,1);
plot(time,ya(:,1));
subplot(3,1,2);
plot(time,yb(:,1));
subplot(3,1,3);
plot(time,yc(:,1));

diff = [ya(:,1) - yc(:,1) ya(:,1) yb(:,1) yc(:,1)];

unique(ya-yc)
