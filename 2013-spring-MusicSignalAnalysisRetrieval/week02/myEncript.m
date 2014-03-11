function myEncript(inputFile,outputFile)

[y,fs,nbits] = wavread(inputFile);

% for each chanel
for i = (1:length(y(1,:)))

    % for each sample in chanel
    for j = (1:length(y(:,i)))
        if y(j,i) > 0
            ny(j,i) = 1  - y(j,i);
        elseif y(j,i) < 0
            ny(j,i) = -1 - y(j,i);
        else
            ny(j,i) = y(j,i);
        end
    end
end

ny = flipud(ny);

wavwrite(ny,fs,nbits,outputFile);

return;
