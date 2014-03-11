function ptOpt=myPtOptSet
%myPtOptSet: Returns the best parameters for PT

ptOpt.frameDuration=32;		% Duration (in ms) of a frame
ptOpt.overlapDuration=0;	% Duration (in ms) of overlap
ptOpt.useVolThreshold=0;	% Pitch with small volume is set to zero

ptOpt.frame2pitchOpt=frame2pitch('defaultOpt');

% ====== You can change options for frame2pitch
ptOpt.frame2pitchOpt.pdf='acf';
ptOpt.frame2pitchOpt.freqRange = [50,500];
ptOpt.frame2pitchOpt.maxShift = 512;

ptOpt.vthr = 1;
ptOpt.pth = 5;
