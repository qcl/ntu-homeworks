function [minDist, bestPitch, allDist]=myLinearScaling4qbsh(queryPitch,dbPitch,lowerRatio,upperRatio,resolution,distanceType)
% myLinearScaling4qbsh
% r01922024

queryLen = length(queryPitch);
dbPitchLen = length(dbPitch);
ratio = linspace(lowerRatio,upperRatio,resolution);

allDist = zeros(1,resolution) + 2.147483647000000e+09;

for i=1:resolution
    % Find the length of the scaled query
    scaledQueryLen = floor(queryLen*ratio(i));

    % Get out of the loop id scaledQuery is longer than dbPitch
    if scaledQueryLen > dbPitchLen
        break
    end

    % Obitan scaledQuery by interp1
    scaledQuery = interp1(1:queryLen,queryPitch,linspace(1,queryLen,scaledQueryLen));

    % Find the difference (diffPitch) between scaledQuery and dbPath
    diffPitch = dbPitch(1:scaledQueryLen) - scaledQuery;

    % Find the median of diffPitch
    if distanceType == 1
        med = median(diffPitch);
    else
        med = mean(diffPitch);
    end

    % allDist(i) = norm(diffPitch-median,distanceType)/length(scaledQuery)
    if distanceType == 1
        allDist(i) = norm(diffPitch-med,distanceType)/scaledQueryLen;
    else
        allDist(i) = sum((diffPitch-med).*(diffPitch-med))/scaledQueryLen;
    end

    % scaledTransposedQuery{i}=...
    scaledTransposedQuery{i} = scaledQuery+med;

end

[minDist,minIndex] = min(allDist);
bestPitch = scaledTransposedQuery{minIndex};

return

