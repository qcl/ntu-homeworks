function B = myShape(A)
% R01922024
% Qing-Cheng Li
% MSAR Week #1

% myShape

m = length(A(1,:,1));
n = length(A(:,1,1));

index = [1:m*n:3*m*n]'*ones(1,m*n)+ones(1,3)'*[0:m*n-1];
B = A(index);

return;
