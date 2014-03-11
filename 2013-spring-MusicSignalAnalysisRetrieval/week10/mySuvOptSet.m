function suvOpt=suvOptSet
% suvOptSet: Set up options for SUV-detection
%	Usage: suvOpt=suvOptSet

%	Roger Jang, 20130509

%% ====== Fixed options
suvOpt.frameSize=640;
suvOpt.overlap=480;
suvOpt.feaExtractFun='mySuvFeaExtract';		% Function for feature extraction
suvOpt.mainClassifier='mySuvClassifier';		% Function for classification
%% ====== Modifiable options
% === Opts for input selection
suvOpt.useInputSelect=1;		% Use input selection
suvOpt.input.bestIndex=[1 2 3 4 5 6 7 8 9];
% === Opts for input normalization
suvOpt.useInputNormalize=0;		% Use input normalization
suvOpt.input.mu=[1496167.70393109;145.693762575453];
suvOpt.input.sigma=[2181371.68543704;49.6206785165336];
% === Opts for the classifier
suvOpt.baseClassifier='nbcEval';
load cPrm.mat
suvOpt.cPrm=cPrm;
