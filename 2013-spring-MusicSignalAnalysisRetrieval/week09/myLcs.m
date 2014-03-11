function [lcsLength,lcsPath,lcsStr,lcsTable] = myLcs(str1,str2,showOpt)
% LCS 
% R01922024
if nargin<3, showOpt=0;end;

str1 = str1(:)';
str2 = str2(:)';

m = length(str1);
n = length(str2);

dpTable = zeros(m,n);

if str1(1) == str2(1)
    dpTable(1,1) = 1;
end

for i=2:m
    if str1(i)==str2(1)
        dpTable(i,1) = 1;
    else
        dpTable(i,1) = dpTable(i-1,1);
    end
end

for j=2:n
    if str1(1)==str2(j)
        dpTable(1,j) = 1;
    else
        dpTable(1,j) = dpTable(1,j-1);
    end
end

for i=2:m
    for j=2:n
        if str1(i) == str2(j)
            dpTable(i,j)=dpTable(i-1,j-1)+1;
        else
            [dpTable(i,j), index] = max([dpTable(i-1,j), dpTable(i,j-1)]);

        end
    end
end

lcsTable = dpTable;
lcsLength = dpTable(m,n);

% find multiple path.
aws = {};
aws = findAllPath(str1,str2,dpTable,aws,[m,n],[]);
b = {};
for i=1:size(aws,2)
    b = [b getStrByPath(str1,str2,aws{i},dpTable,lcsLength)];
end

lcsStr = unique(b);
lcsPath = cell(1,size(lcsStr,2));
for i=1:size(lcsStr,2)
    p = find(ismember(b,lcsStr{i}));
    lcsPath{i} = aws{p(1)};
    for j=2:size(p,2)
        lcsPath{i} = getLeftUpPath(lcsPath{i},aws{p(j)});
    end
end

if showOpt == 1
    dpPathPlot(str1,str2,lcsPath,dpTable);
    title(sprintf('multiple LCS'));
end

function a=findAllPath(str1,str2,dpTable,aws,now,path)
path = [now;path];
if now(1) == 1 && now(2) == 1
    %path = removeZero(path,dpTable);
    a = [aws {path}];
    return
elseif now(1) == 1 && str1(now(1)) == str2(now(2))
    path = removeZero(path,dpTable);
    a = [aws {path}];
    return
elseif now(2) == 1 && str1(now(1)) == str2(now(2))
    path = removeZero(path,dpTable);
    a = [aws {path}];
    return
else
    if now(1) == 1
        aws = findAllPath(str1,str2,dpTable,aws,[now(1),now(2)-1],path);
    elseif now(2) == 1
        aws = findAllPath(str1,str2,dpTable,aws,[now(1)-1,now(2)],path);
    else
        if str1(now(1)) == str2(now(2))
            aws = findAllPath(str1,str2,dpTable,aws,[now(1)-1,now(2)-1],path);
        else

            if dpTable(now(1)-1,now(2)) == dpTable(now(1),now(2))
                aws = findAllPath(str1,str2,dpTable,aws,[now(1)-1,now(2)],path);
            end

            if dpTable(now(1),now(2)-1) == dpTable(now(1),now(2))
                aws = findAllPath(str1,str2,dpTable,aws,[now(1),now(2)-1],path);
            end

        end
    end
    a = aws;
end

function newPath=removeZero(path,dpTable)
    newPath = [];
    for i=1:size(path,1)
        c = path(i,:);
        if dpTable(c(1),c(2)) ~= 0
            newPath = [newPath; c];
        end
    end
    return 

function str = getStrByPath(str1,str2,path,dpTable,l)
    indexs = [];
    for i=1:size(path,1)
        c = path(i,:);
        if str1(c(1)) == str2(c(2))
            indexs = [indexs c(1)];
            l = l - 1;
            if l == 0
                break
            end
        end    
    end
    str = str1(indexs);
    return

function path = getLeftUpPath(path1,path2)
    path = path1;
    l1 = size(path1,1);
    l2 = size(path2,1);
    k = 0;
    while k < min(l1,l2)
        p1 = path1(l1-k,:);
        p2 = path2(l2-k,:);
        if p1(1) ~= p2(1) || p1(2) ~= p2(2)
            if p1(1) <= p2(1) && p1(2) >= p2(2)
                path = path1;
                return
            end

            if p2(1) <= p1(1) && p2(2) >= p1(2)
                path = path2;
                return
            end

        end
        k = k + 1;
    end
    
function dpPathPlot(str1, str2, lcsPath, lcsTable)

m = length(str1);
n = length(str2);
plot(nan, nan); axis([0 m+1 0 n+1]); box on; axis image
set(gca, 'xtick', 1:m);
set(gca, 'ytick', 1:n);
set(gca, 'xticklabel', str1(:));
set(gca, 'yticklabel', str2(:));
xlabel(['String1 = ', str1]);
ylabel(['String2 = ', str2]);
% === Plot bounding boxes for LCS table element
for i = 1:m
	for j = 1:n
		line(i, j, 'marker', 'square', 'markersize', 15, 'color', 'g', 'linestyle', 'none');
	end
end
% === Plot prevPos
for i=1:m
	for j=1:n
		now=i+j*sqrt(-1);
        if i==1 && j==1
        
        elseif i==1
            next = i+ (j-1)*sqrt(-1);
            start=now+(next-now)*0.2;
    		stop =now+(next-now)*0.5;
            arrowPlot(start, stop, [0 0 1]);
        elseif j==1
            next = (i-1) + j*sqrt(-1);
            start=now+(next-now)*0.2;
    		stop =now+(next-now)*0.5;
            arrowPlot(start, stop, [0 0 1]);
        else
            if str1(i) == str2(j)
                next = (i-1) + (j-1)*sqrt(-1);
                start=now+(next-now)*0.2;
                stop =now+(next-now)*0.5;
                arrowPlot(start, stop, [0 0 1]);
            else
                if lcsTable(i,j) == lcsTable(i-1,j)
                    next = (i-1) + (j)*sqrt(-1);
                    start=now+(next-now)*0.2;
                    stop =now+(next-now)*0.5;
                    arrowPlot(start, stop, [0 0 1]);
                end
                if lcsTable(i,j) == lcsTable(i,j-1)
                    next = (i) + (j-1)*sqrt(-1);
                    start=now+(next-now)*0.2;
                    stop =now+(next-now)*0.5;
                    arrowPlot(start, stop, [0 0 1]);
                end
            end
        end
	end
end
% === Plot DP path
for k=1:size(lcsPath,2)
    for i=1:size(lcsPath{k},1)-1
        line(lcsPath{k}(i:i+1, 1)+0.2, lcsPath{k}(i:i+1, 2)-0.2, 'color', 'm', 'linewidth', 2);
    end
end
% === Circle matched elements
for k=1:size(lcsPath,2)
    for i=1:size(lcsPath{k},1)
    	if str1(lcsPath{k}(i,1))==str2(lcsPath{k}(i,2))
    		line(lcsPath{k}(i,1), lcsPath{k}(i,2), 'marker', 'o', 'markersize', 15, 'color', 'r', 'linestyle', 'none', 'linewidth', 2);
    	end
    end
end
% === Plot LCS table element
for i = 1:m
	for j = 1:n
		text(i, j, int2str(lcsTable(i,j)), 'hori', 'center');
	end
end

