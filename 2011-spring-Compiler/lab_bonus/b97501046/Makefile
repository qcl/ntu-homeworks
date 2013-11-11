# target run 用來測試自創測試檔，驗收的 Makefile 不會用到
# 參數 $(INPUT) 是不含 path 的測試檔案名稱
# run 的效果是由這一層的 $(INPUT) 產生這一層的 $(INPUT).out 
run: bonus.py
	python bonus.py $(INPUT) $(INPUT).out

# target judgeRun 一定要有，驗收的 Makefile 會用到
# 參數 $(INPUT) 是不含 path 的測試檔案名稱
# judgeRun 的效果是由上一層的 $(INPUT) 產生這一層的 $(INPUT).out 
judgeRun: bonus.py
	python bonus.py ../$(INPUT) $(INPUT).out
	
# target clean 一定要有，驗收的 Makefile 會用到
# clean 的效果必須清掉程式執行檔與執行結果
clean:
	rm -f *.out
