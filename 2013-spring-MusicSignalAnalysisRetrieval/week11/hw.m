% R01922024
% Qing-Cheng Li (qcl)

dataSetName = 'wine';
maxClusterNum = 50;



dataSet = prData(dataSetName);
[dim,dataNum] = size(dataSet.input);

% LOO
foldNum = dataNum;

% KNN
method = 'knnc';
knncTrainPrm.method = 'kMeans';

vRr = [];
tRr = [];

for c=1:maxClusterNum
    knncTrainPrm.centerNum4eachClass = c;
    [vRrAll,tRrAll] = perfCv(dataSet,method,knncTrainPrm,foldNum,0);
    
    vRr = [vRr vRrAll*100];
    tRr = [tRr tRrAll*100];
    fprintf('c = %d, tRr = %.2f%%, vRr = %.2f%%\n',c,tRrAll*100,vRrAll*100);
end

[maxVRr,maxC] = max(vRr);
fprintf('\nmax vRr = %.2f%% @ c = %d\n',maxVRr,maxC);

plot(1:maxClusterNum,tRr,'.-',1:maxClusterNum,vRr,'.-',maxC,maxVRr,'O');
xlabel('No. of clusters for each class');
ylabel('RR(%)');
legend('Training RR','Validating RR','Max. Validation RR','location','northOutside','orientation','horizontal');

