split -l 100 hw5_14_train.in
rm -rf hw5_15_*
touch hw5_15_1_train.in
touch hw5_15_2_train.in
touch hw5_15_3_train.in
touch hw5_15_4_train.in
touch hw5_15_5_train.in

cat ./xaa >> hw5_15_1_test.in
cat ./xab >> hw5_15_1_train.in
cat ./xac >> hw5_15_1_train.in
cat ./xad >> hw5_15_1_train.in
cat ./xae >> hw5_15_1_train.in

cat ./xab >> hw5_15_2_test.in
cat ./xaa >> hw5_15_2_train.in
cat ./xac >> hw5_15_2_train.in
cat ./xad >> hw5_15_2_train.in
cat ./xae >> hw5_15_2_train.in

cat ./xac >> hw5_15_3_test.in
cat ./xab >> hw5_15_3_train.in
cat ./xaa >> hw5_15_3_train.in
cat ./xad >> hw5_15_3_train.in
cat ./xae >> hw5_15_3_train.in

cat ./xad >> hw5_15_4_test.in
cat ./xab >> hw5_15_4_train.in
cat ./xac >> hw5_15_4_train.in
cat ./xaa >> hw5_15_4_train.in
cat ./xae >> hw5_15_4_train.in

cat ./xae >> hw5_15_5_test.in
cat ./xab >> hw5_15_5_train.in
cat ./xac >> hw5_15_5_train.in
cat ./xad >> hw5_15_5_train.in
cat ./xaa >> hw5_15_5_train.in
