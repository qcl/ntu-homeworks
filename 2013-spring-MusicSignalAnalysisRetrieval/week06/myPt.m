function pitch=myPt(wObj, opt, showPlot);
% myPt: My pitch tracking
%
%	Usage:
%		pitch=myPt(wObj, opt, showPlot);
%			opt: options for pitch
%			showPlot: 1 for plotting, 0 for not plotting
%
%			pitch: result of pitch tracking
%
%	Example:
%		waveFile='�۴��K�����U��_����_0.wav';
%		wObj=waveFile2obj(waveFile);
%		opt=myPtOptSet;
%		showPlot=1;
%		myPt(wObj, opt, showPlot);
%
%	Modify from Roger Jang
%   Qing-Cheng Li
%   r01922024 at csie dot ntu dot edu dot tw

if nargin<1, selfdemo; return; end
if nargin<2, opt=myPtOptSet; end
if nargin<3, showPlot=0; end

if isstr(wObj), wObj=waveFile2obj(wObj); end	% If the give wObj is a file name

y=wObj.signal;  % get wave signal 
fs=wObj.fs;     % get fs
y=y-mean(y);    % zero mean?


frameSize=round(opt.frameDuration*fs/1000);
overlap=round(opt.overlapDuration*fs/1000);
frameMat=enframe(y, frameSize, overlap);
frameNum=size(frameMat, 2);
pitch=zeros(1, frameNum);
pdf  =[];
for i=1:frameNum
	frame=frameMat(:, i);
	[pitch(i),tmpdf]=frame2pitch(frame, fs, opt.frame2pitchOpt);
    pdf = [pdf tmpdf];
    
end

volume=frame2volume(frameMat);
volumeTh=max(volume)*opt.vthr/10;
pitch0=pitch;



for i=2:length(pitch)-1
    
    a = pitch(i-1);
    b = pitch(i);
    c = pitch(i+1);
    %if abs(pitch(i)-pitch(i-1))>5 && abs(pitch(i+1) - pitch(i)) > 5
    %    pitch(i) = (pitch(i+1)+pitch(i-1))/2;
    %end
    
    if abs(a-b)>opt.pth && abs(b-c)>opt.pth && volume(i) > volumeTh && volume(i+1) > volumeTh && volume(i-1) > volumeTh
        pitch(i) = (a+c)/2;
    end
    
    
        
end

if showPlot
	fprintf('No implementation.\n');
end

% ====== Self demo
function selfdemo
    fprintf('No selfdemo.\n');