function qbshOpt=qbshOptSet
% qbshOptSet: Set up options for QBSH
%	Usage: qbshOpt=qbshOptSet

%	Roger Jang, 20130425

%% ====== Add toolbox paths
%addpath /users/jang/matlab/toolbox/utility
%addpath /users/jang/matlab/toolbox/sap
%% ====== Fixed options
qbshOpt.songDb='childSong';
qbshOpt.anchorPos='songStart';		% 'songStart', 'sentenceStart', or 'noteStart' [比對位置]
qbshOpt.matchFun='myQbsh';		% Function for matching the query pitch
qbshOpt.usePv=1;			% Use human-labeled pitch vector instead of doing pitch tracking on wave files
qbshOpt.ptOpt=ptOptSet(8000, 8);	% For pitch tracking
qbshOpt.matchType='wave2midi';		% Match type: wave against midi
%% ====== Modifiable options
qbshOpt.method='ls';		% Match method, used in myQbsh.m [比對方法，請見 myQbsh.m]
qbshOpt.useRest=1;		% Use rest (1: extend previous nonzero note, 0: delete rest) [是否使用休止符（1：使用前一個非零音符來取代，0：砍掉休止符）]
%% === LS options
qbshOpt.lowerRatio=0.5;
qbshOpt.upperRatio=2.0;
qbshOpt.resolution=101;		% Resolution of LS [線性伸縮的次數]
qbshOpt.lsDistanceType=1;
%% === DTW options
qbshOpt.beginCorner=1;		% Anchored beginning [頭固定]
qbshOpt.endCorner=0;		% Free end [尾浮動]
qbshOpt.dtwCount=5;		% No of key transposition [每次比對需進行幾次 DTW]
