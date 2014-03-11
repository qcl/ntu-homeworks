function [distVec, minIndexVec] = myQbsh(songDb, pitch, qbshOpt)
%myQbsh: Match a given pitch vector for QBSH [對一個使用者輸入的音高向量進行 QBSH]
%
%	Usage:
%		[distVec, minIndexVec, scoreVec] = myQbsh(songDb, pitch, qbshOpt)
%
%	Description:
%		[distVec, minIndexVec, scoreVec] = myQbsh(songDb, pitch, qbshOpt)
%			songDb: song collection
%			pitch: input pitch vector
%			qbshOpt: MR parameters

%	Roger Jang, 20130122

if any(isnan(pitch)), error('The given pitch cannot contain NaN!'); end
if sum(pitch)==0, error('The given pitch is all zero!'); end

pitch=pitch(:)';					% Change to a row vector [改為列向量]
pitch=pvRestHandle(pitch, qbshOpt.useRest);		% Handle rests [處理休止符]
pitch=pitch-mean(pitch);				% Shift the pitch to have zero mean [平移到平均值為零]
pitchLen = length(pitch);				% Length of the pitch vector [音高向量長度（點數）]
songNum=length(songDb);
distVec=zeros(1, songNum);
minIndexVec=zeros(1, songNum);

switch qbshOpt.method
	case 'ls'
		% ====== Create scaled version of the input query pitch
		[scaledVecSet, scaledVecLen]=scaledVecCreate(pitch, qbshOpt.lowerRatio, qbshOpt.upperRatio, qbshOpt.resolution);
		for i=1:length(songDb)	% Compare pitch to each song
		%	fprintf('\t%d/%d: songName=%s\n', i, length(songDb), songDb(i).songName);
		%	songDb(i).anchorPvIndex=1;
			anchorNum=length(songDb(i).anchorPvIndex);
			distVecInSong=zeros(anchorNum, 1);
			minIndexInSong=zeros(anchorNum, 1);
			for j=1:anchorNum
			%	fprintf('\t\t%d/%d:\n', j, anchorNum); pause;
			%	song=songDb(i).pv(songDb(i).noteStartIndex(j):end);		% A song from the note start index
				song=songDb(i).pv(songDb(i).anchorPvIndex(j):end);		% A song from the anchor index
				% ====== Compute LS (計算 LS) at each anchor point
				[distVecInSong(j), minIndexInSong(j)]=linScaling2Mex(scaledVecSet, scaledVecLen, song, qbshOpt.lsDistanceType);
			end
			[distVec(i), minIndex]=min(distVecInSong);	% distVec(i): distance to song i. minIndex: anchor index
			minIndexVec(i)=minIndexInSong(minIndex);	% 
		end
	case 'dtw1'
		for i=1:length(songDb),
			song=songDb(i).pv;
			% ====== Shift song to have zero mean [平移歌曲，盡量使其和輸入音高向量有相同的基準（在此為平移至平均值為零）]
			songMean=mean(song(1:min(pitchLen, length(song))));	% Get mean of the song with the same length as the pitch vector [抓出歌曲的平均值（取其長度和輸入音高向量相同）]
			song=song-songMean;					% Shift to mean [平移到平均值]
			distVec(i)=dtw4qbsh(pitch, song, qbshOpt);
		end
	case 'dtw2'
		for i=1:length(songDb),
			song=songDb(i).pv;
			% ====== Shift song to have zero mean [平移歌曲，盡量使其和輸入音高向量有相同的基準（在此為平移至平均值為零）]
			songMean=mean(song(1:min(pitchLen, length(song))));	% Get mean of the song with the same length as the pitch vector [抓出歌曲的平均值（取其長度和輸入音高向量相同）]
			song=song-songMean;					% Shift to mean [平移到平均值]
			distVec(i)=dtw4qbsh(pitch, song, qbshOpt);
		end
	otherwise
		error('Unknown method!');
end