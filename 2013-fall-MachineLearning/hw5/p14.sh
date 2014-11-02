echo "cv"
svm-train -s 0 -t 2 -c ${1} -g ${2} -v 5 hw5_14_train.in
echo "train"
svm-train -s 0 -t 2 -c ${1} -g ${2} hw5_14_train.in
echo "predict"
svm-predict hw5_14_train.in hw5_14_train.in.model output
