%high
waveFile='HighPitch.wav';
[y, fs, nbits]=wavread(waveFile);
index1=94200;
frameSize=2000;
index2=index1+frameSize-1;
frame=y(index1:index2);

subplot(4,1,1); plot(y); grid on
line(index1*[1 1], [-1 1], 'color', 'r');
line(index2*[1 1], [-1 1], 'color', 'r');
subplot(4,1,2); plot(frame, '.-'); grid on
point=[809, 1408];
line(point, frame(point), 'marker', 'o', 'color', 'red');

periodCount=4;
fp=((point(2)-point(1))/periodCount)/fs;	% fundamental period
ff=fs/((point(2)-point(1))/periodCount);	% fundamental frequency
pitch=69+12*log2(ff/440);
fprintf('HighPitch');
fprintf('Fundamental period = %g second\n', fp);
fprintf('Fundamental frequency = %g Hertz\n', ff);
fprintf('Pitch = %g semitone\n', pitch);


%low
waveFile='lowPitch.wav';
[y, fs, nbits]=wavread(waveFile);
index1=76000;
frameSize=3000;
index2=index1+frameSize-1;
frame=y(index1:index2);

subplot(4,1,3); plot(y); grid on
line(index1*[1 1], [-1 1], 'color', 'r');
line(index2*[1 1], [-1 1], 'color', 'r');
subplot(4,1,4); plot(frame, '.-'); grid on
point=[666, 2436];
line(point, frame(point), 'marker', 'o', 'color', 'red');

periodCount=4;
fp=((point(2)-point(1))/periodCount)/fs;	% fundamental period
ff=fs/((point(2)-point(1))/periodCount);	% fundamental frequency
pitch=69+12*log2(ff/440);
fprintf('LowPitch');
fprintf('Fundamental period = %g second\n', fp);
fprintf('Fundamental frequency = %g Hertz\n', ff);
fprintf('Pitch = %g semitone\n', pitch);
