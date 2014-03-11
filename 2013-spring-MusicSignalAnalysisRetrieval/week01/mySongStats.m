function [out1,out2] = mySongStats(songObj)
% R01922024
% Qing-Cheng Li
% MSAR Week #1

% Encoding: big-5

%clear all;
%load songObj.mat;

%oldlang = feature('DefaultCharacterSet');
%feature('DefaultCharacterSet', 'UTF8');

artist = { songObj.artist };
uniqueArtist = unique(artist);

tobeDel = find(strcmp('unknown',uniqueArtist));
if length(tobeDel) == 1
    uniqueArtist(tobeDel) = [];
end


tobeDel = find(strcmp('¦Ñºq',uniqueArtist));
if length(tobeDel) == 1
    uniqueArtist(tobeDel) = [];
end

tobeDel = find(strcmp('¤£¸Ô',uniqueArtist));
if length(tobeDel) == 1
    uniqueArtist(tobeDel) = [];
end


for i=1:length(uniqueArtist)
    artistObj(i).name = uniqueArtist{i};
    artistObj(i).songCount = length(find(strcmp(uniqueArtist{i},artist)));
end

[orz,index] = sort([artistObj.songCount],'descend');
artistObj = artistObj(index);
out1 = artistObj(1:10);

chinese = unique(artist(find(strcmp('Chinese',{songObj.language}))));
taiwanese = unique(artist(find(strcmp('Taiwanese',{songObj.language}))));
[out2,orz] = sort(intersect(intersect(chinese,taiwanese),uniqueArtist));

%feature('DefaultCharacterSet', oldlang);

return;
