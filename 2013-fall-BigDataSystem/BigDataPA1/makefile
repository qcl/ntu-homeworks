pagerank: PageRank.jar PageRankRun.sh
	sh PageRankRun.sh 10 /big.graph /output
	hadoop fs -cat /output/*

PageRank.jar: PageRank.java PageRankCompile.sh
	sh PageRankCompile.sh

sort: Sort.jar SortRun.sh
	sh SortRun.sh /big.list /output
	hadoop fs -cat /output/*

Sort.jar: Sort.java SortCompile.sh
	sh SortCompile.sh
