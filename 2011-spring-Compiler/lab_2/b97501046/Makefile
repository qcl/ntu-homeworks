# target run 用來測試自創測試檔，驗收的 Makefile 不會用到
# 參數 $(GRAM) 是不含 path 的測試檔案名稱
# run 的效果是由這一層的 $(GRAM) 產生這一層的 $(GRAM).csv 
run: main
	./main $(GRAM) $(GRAM).csv

# target judgeRun 一定要有，驗收的 Makefile 會用到
# 參數 $(GRAM) 是不含 path 的測試檔案名稱
# judgeRun 的效果是由上一層的 $(GRAM) 產生這一層的 $(GRAM).csv 
judgeRun: main
	./main ../$(GRAM) $(GRAM).csv

main: main.cc scanner.o rules.o token.o
	g++ main.cc rules.o scanner.o token.o -o main

rules.o: rules.cc rules.h
	g++ -c rules.cc

scanner.o: scanner.cc scanner.h
	g++ -c scanner.cc

token.o: token.cc token.h
	g++ -c token.cc
	
# target clean 一定要有，驗收的 Makefile 會用到
# clean 的效果必須清掉程式執行檔與執行結果
clean:
	rm -f *.o
	rm -f *.csv
	rm -f main
