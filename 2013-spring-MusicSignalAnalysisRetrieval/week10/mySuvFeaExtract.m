function [inData, outData, inputName, annotation]=mySuvFeaExtract(waveFile, showPlot)
% mySuvFeaExtract: Wave to feature conversion for su/v detection
%
%	Usage:
%		[inData, outData, inputName, annotation]=mySuvFeaExtract(waveFile, showPlot)
%
%	Example:
%		waveFile = 'test.wav';
%		showPlot=1;
%		[inData, outData, inputName, annotation]=mySuvFeaExtract(waveFile, showPlot);

% Roger Jang, 20040910, 20070417

if nargin<1, selfdemo; return; end
if nargin<2, showPlot=0; end

[y, fs, nbits]=wavReadInt(waveFile);
% ====== Frame blocking
frameSize=640;		% This should be the same as the one use for pitch labeling
overlap=480;		% This should be the same as the one use for pitch labeling
frameMat=buffer2(y, frameSize, overlap);
frameNum=size(frameMat, 2);
% ====== zero-mean for each frame
frameMat=frameZeroMean(frameMat, 0);
inData=[];
inputName={};
% ====== Feature 1: absolute volume
volume = frame2volume(frameMat);
inData=[inData; volume];
inputName={inputName{:}, 'volume'};
% ====== Feature 2: zero crossing rate
zcr=frame2zcr(frameMat, 2);
inData=[inData; zcr];
inputName={inputName{:}, 'zcr'};
% ====== Feature 3: number of frame local min
frameLocalMinCount = zeros(1,frameNum);
for i = 1:frameNum
	frameLocalMinCount(i) = sum(localMin(frameMat(:, i)));
end
inData=[inData; frameLocalMinCount];
inputName={inputName{:}, 'frameLocalMinCount'};
% ====== Feature 4: relative volume
relVol=volume/median(volume);
inData=[inData; relVol];
inputName={inputName{:}, 'relVol'};
% ====== Feature 5 and 6: number of AMDF local min, and AMDF max difference
amdfLocalMinCount = zeros(1,frameNum);
amdfMaxDiff = zeros(1,frameNum);
for i=1:frameNum
	amdf = frame2amdf(frameMat(:, i));
	localMinIndex=find(localMin(amdf));
	amdfLocalMinCount(i)=length(localMinIndex);
	if amdfLocalMinCount>0
		amdfMaxDiff(i)=max(amdf)-min(amdf(localMinIndex));
	else
		amdfMaxDiff(i)=0;
	end
end
inData=[inData; amdfLocalMinCount];
inputName={inputName{:}, 'amdfLocalMinCount'};
inData=[inData; amdfMaxDiff];
inputName={inputName{:}, 'amdfMaxDiff'};

% ====== Feature 7: Absolute sum of high-order difference
ashod = frame2ashod(frameMat);
inData = [inData; ashod];
inputName={inputName{:}, 'ashod'};

% ====== Feature 8: Clarity
clarity = frame2clarity(frameMat,fs);
inData = [inData; clarity];
inputName={inputName{:}, 'clarity'};

% ====== Feature 9: Pitch
pitches = frame2pitches(frameMat,fs);
inData = [inData; pitches];
inputName={inputName{:}, 'pitches'};

% ====== add annotation for debugging
for i=1:size(inData,2)
	annotation{i}=strPurify(sprintf('%s\nframe index = %d', waveFile, i));
end

% ====== Read human-labeled pitch file
pvFile = strrep(waveFile, '.wav', '.pv');
targetPitch = asciiRead(pvFile);
if length(targetPitch)>frameNum, targetPitch=targetPitch(1:frameNum); end	% Due to the difference between buffer.m (used before) and buffer2.m (used now)
outData = targetPitch>0;
outData = outData(:)';

if showPlot
	DS.input=inData;
	DS.output=outData+1;	% Make the output 1 or 2
	DS.inputName=inputName;
	DS.annotation=annotation;
	subplot(2,1,1);
	dsScatterPlot(DS); title('Raw data');
	DS.input=inputNormalize(DS.input);
	subplot(2,1,2);
	dsScatterPlot(DS); title('Normalized data');
end

% ====== Self demo
function selfdemo
mObj=mFileParse(which(mfilename));
strEval(mObj.example);

function pitches=frame2pitches(frameMat,fs)
[frameSize, frameNum]=size(frameMat);
pitches=zeros(1, frameNum);
for i=1:frameNum
	frame=frameMat(:,i);
	pitch=frame2pitch(frame,fs);
    pitches(i) = pitch;
end