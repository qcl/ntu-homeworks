function epdPrm=myEpdPrmSet
% myEpdPrmSet: Returns the best parameters for EPD
%
% MSAR 2013 Homework
% R01922024 Qing-Cheng Li

epdPrm.frameSize = 8000/(16000/256);
epdPrm.overlap = 0;

epdPrm.volRatio = 0.1;
epdPrm.vMinMaxPercentile = 3;

epdPrm.volRatio2 = 0.2;
epdPrm.zcrRatio = 0.1;  % TODO try 0.15 0.16 0.17 0.2
epdPrm.zcrShiftGain = 4;

epdPrm.minSegment = 0.068;
