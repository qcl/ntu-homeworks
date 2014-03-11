function [epInSampleIndex, epInFrameIndex] = myEpd(wObj, epdPrm, showPlot)
% myEpd: modify from Roger Jang's myEpd.m and endPointDetect.m 
%
% R01922024
% Qing-Cheng Li

if nargin<1, selfdemo; return; end
if nargin<2, epdPrm=myEpdPrmSet; end
if nargin<3, showPlot=0; end

wave=wObj.signal;
fs=wObj.fs;

frameSize=epdPrm.frameSize; 
overlap=epdPrm.overlap;

wave=double(wave);
wave=wave-mean(wave);
frameMat=enframe(wave, frameSize, overlap);

%For speed up
frameMat=frameZeroMean(frameMat, 2);

frameNum=size(frameMat, 2);

minSegment=round(epdPrm.minSegment*fs/(frameSize-overlap));

volume=frame2volume(frameMat);		% compute volume
temp=sort(volume);
index=round(frameNum*epdPrm.vMinMaxPercentile/100); if index==0, index=1; end
volMin=temp(index);
volMax=temp(frameNum-index+1);			% To avoid unvoiced sounds
volTh1=(volMax-volMin)*epdPrm.volRatio+volMin;	% compute volume threshold
volTh2=(volMax-volMin)*epdPrm.volRatio2+volMin;	% compute volume threshold

% ====== Compute ZCR
[minVol, index]=min(volume);
shiftAmount=epdPrm.zcrShiftGain*max(abs(frameMat(:,index)));		% shiftAmount is equal to epdPrm.zcrShiftGain times the max. abs. sample within the frame of min. volume
shiftAmount=max(shiftAmount, max(frameMat(:))/100);
zcr=frame2zcr(frameMat, 1, shiftAmount);
zcrTh=max(zcr)*epdPrm.zcrRatio;


% ====== Merge Expansion 1 and Identify voiced part that's larger than volTh2

soundSegment = [];
tmpSeg = segmentFind(volume>volTh1);
for i=1:length(tmpSeg),
    tmpV = sum(volume(tmpSeg(i).begin:tmpSeg(i).end)>volTh2);
    if tmpV > 0, soundSegment = [soundSegment tmpSeg(i)]; end
end


%tmpSeg = segmentFind(zcr>zcrTh);
%for i=1:length(tmpSeg),
%    tmpZ = sum()
%end

% ====== Expansion 2: zcr
for i=1:length(soundSegment),
	head = soundSegment(i).begin;
	while (head-1)>=1 & zcr(head-1)>zcrTh			% Extend at beginning
		head=head-1;
	end
	soundSegment(i).begin = head;
	tail = soundSegment(i).end;
	while (tail+1)<=length(zcr) & zcr(tail+1)>zcrTh		% Extend at ending
		tail=tail+1;
	end
	soundSegment(i).end = tail;
end
% ====== Delete repeated sound segments
index = [];
for i=1:length(soundSegment)-1,
	if soundSegment(i).begin==soundSegment(i+1).begin & soundSegment(i).end==soundSegment(i+1).end,
		index=[index, i];
	end
end
soundSegment(index) = [];
% ====== Delete short sound clips
index = [];
for i=1:length(soundSegment)
	soundSegment(i).duration=soundSegment(i).end-soundSegment(i).begin+1;	% This is necessary since the duration is changed due to expansion
	if soundSegment(i).duration<=minSegment
		index = [index, i];
	end
end
soundSegment(index) = [];
zeroOneVec=0*volume;
for i=1:length(soundSegment)
	for j=soundSegment(i).begin:soundSegment(i).end
		zeroOneVec(j)=1;
	end
end

if isempty(soundSegment)
	epInSampleIndex=[];
	epInFrameIndex=[];
	fprintf('Warning: No sound segment found in %s.m.\n', mfilename);
else
	epInFrameIndex=[soundSegment(1).begin, soundSegment(end).end];
	epInSampleIndex=frame2sampleIndex(epInFrameIndex, frameSize, overlap);		% conversion from frame index to sample index
end

if showPlot,
    fprintf('This method was not implemented.\n');
end

% ====== Self demo
function selfdemo
    fprintf('This method was not implemented.\n');

% ====== SpeedUpFunctions
function frameMat = frameZeroMean(frameMat, polyOrder)
[frameSize, frameNum]=size(frameMat);
for i=1:frameNum
	frameMat(:,i)=frameMat(:,i)-polyval(polyfit((1:frameSize)', frameMat(:,i), polyOrder), (1:frameSize)');
end

function p = polyfit(x,y,n)
x = x(:);
y = y(:);
V(:,n+1) = ones(length(x),1,class(x));
for j = n:-1:1
    V(:,j) = x.*V(:,j+1);
end
[Q,R] = qr(V,0);
p = R\(Q'*y);
p = p.';

function y = polyval(p,x)
nc = length(p);
siz_x = size(x);
y = zeros(siz_x, superiorfloat(x,p));
if nc>0, y(:) = p(1); end
for i=2:nc
    y = x .* y + p(i);
end


