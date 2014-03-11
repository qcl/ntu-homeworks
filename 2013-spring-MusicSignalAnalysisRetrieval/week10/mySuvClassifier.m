function [computedClass, DS2]=mySuvClassifier(DS, suvOpt)
% mySuvClassifier: My classifier for SU/V detection

DS2=DS;
% ====== Input selection
if suvOpt.useInputSelect
	DS2.input=DS2.input(suvOpt.input.bestIndex, :);		% Use the selected features
	DS2.inputName=DS2.inputName(suvOpt.input.bestIndex);	% Update the input names based on the selected features
end
% ====== Input normalization
if suvOpt.useInputNormalize
	DS2.input=inputNormalize(DS2.input, suvOpt.input.mu, suvOpt.input.sigma);	% Input normalization based on the given mu and sigma
end
% ====== Classifier evaluation
% In this example, we use the quadratic classifier. You can change it to other classifiers of your choice.
computedClass=feval(suvOpt.baseClassifier, DS2, suvOpt.cPrm);
