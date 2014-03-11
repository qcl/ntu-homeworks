DS=prData('iris');
cMax=30;
showPlot=0;
vRrAll = zeros(1,cMax);
tRrAll = zeros(1,cMax);
DSNum= size(DS.input, 2);

for c=1:cMax
   knncPrm.method='kMeans';
   knncPrm.centerNum4eachClass=c;
   knncPrm.k=1;
   [vRrAll(c),tRrAll(c)]=perfCv(DS,'knnc', knncPrm, DSNum, showPlot);
   fprintf('When c=%2d , Training RR=%.2f%%, Validating RR=%.2f%%\n', c, tRrAll(c)*100, vRrAll(c)*100);
end

x = 1:cMax;
plot(x,tRrAll,x,vRrAll);

% ===================================

DS=prData('WINE');
cMax=50;
showPlot=0;
vRrAll = zeros(1,cMax);
tRrAll = zeros(1,cMax);
DSNum= size(DS.input, 2);

for c=1:cMax
   knncPrm.method='kMeans';
   knncPrm.centerNum4eachClass=c;
   knncPrm.k=1;
   [vRrAll(c),tRrAll(c)]=perfCv(DS,'knnc', knncPrm, DSNum, showPlot);
   fprintf('When c=%2d , Training RR=%.2f%%, Validating RR=%.2f%%\n', c, tRrAll(c)*100, vRrAll(c)*100);
end

x = 1:cMax;
plot(x,tRrAll,x,vRrAll);