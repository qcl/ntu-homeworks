echo "cv"
svm-train -s 4 -t 2 -c ${1} -g ${2} ./hw5_15_1_train.in
svm-predict hw5_15_1_test.in hw5_15_1_train.in.model hw5_15_1.out
svm-train -s 4 -t 2 -c ${1} -g ${2} ./hw5_15_2_train.in
svm-predict hw5_15_2_test.in hw5_15_2_train.in.model hw5_15_2.out
svm-train -s 4 -t 2 -c ${1} -g ${2} ./hw5_15_3_train.in
svm-predict hw5_15_3_test.in hw5_15_3_train.in.model hw5_15_3.out
svm-train -s 4 -t 2 -c ${1} -g ${2} ./hw5_15_4_train.in
svm-predict hw5_15_4_test.in hw5_15_4_train.in.model hw5_15_4.out
svm-train -s 4 -t 2 -c ${1} -g ${2} ./hw5_15_5_train.in
svm-predict hw5_15_5_test.in hw5_15_5_train.in.model hw5_15_5.out
echo "train"
svm-train -s 4 -t 2 -c ${1} -g ${2} ./hw5_14_train.in
svm-predict hw5_14_train.in hw5_14_train.in.model hw5_15_0.out
python calacc.py
