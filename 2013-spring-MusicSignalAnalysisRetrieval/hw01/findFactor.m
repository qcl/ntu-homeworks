function factor = findFactor(number)
% R01922024
% Qing-Cheng Li
% MSAR Homework #1

% findFactor find all factor of number.

    factor = [];
    if number > 0;
        for i = 1:number
            if mod(number,i) == 0;
                factor = [factor i];
            end
        end
    end
    return;
