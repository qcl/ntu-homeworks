% R01922024
% Qing-Cheng Li

clear all;

t=0:0.1:4*pi;
y=sin(t)+rand(1, length(t));

[value,local] = findpeaks(y);
local = t(local);

p = plot(t, y, '.-',local,value,'ro');
set(p(2),'markerSize',10);
for i=1:length(value)
    text(local(i),value(i),sprintf('   (%.2f,%.2f)',local(i),value(i)));
end

