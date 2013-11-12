wpr: WeightedPageRank.jar run.sh testdata/small.graph.13.out ansdiff.py
	sh run.sh 13 /small.graph /output
	hadoop fs -cat /output/*
	rm -rf small.13.qcl.out
	hadoop fs -get /output/part-00000 small.13.qcl.out
	python ansdiff.py testdata/small.graph.13.out small.13.qcl.out

big: WeightedPageRank.jar run.sh
	sh run.sh 6 /big.graph /output
	hadoop fs -cat /output/*
	rm -rf big.6.qcl.out
	hadoop fs -get /output/part-00000 big.6.qcl.out
	python ansdiff.py testdata/big.graph.6.out big.6.qcl.out

biga: WeightedPageRank.jar run.sh
	sh run.sh 1 /big.graph /output
	hadoop fs -cat /output/*
	rm -rf big.1.qcl.out
	hadoop fs -get /output/part-00000 big.1.qcl.out
	python ansdiff.py testdata/big.graph.1.out big.1.qcl.out

WeightedPageRank.jar: WeightedPageRank.java compile.sh
	sh compile.sh

