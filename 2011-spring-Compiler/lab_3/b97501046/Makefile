# target run 用來測試自創測試檔，驗收的 Makefile 不會用到
# 參數 $(PROG) 是不含 path 的測試檔案名稱
# run 的效果是由這一層的 $(PROG) 產生這一層的 $(PROG).out 
run: main
	./main $(PROG) $(PROG).out

# target judgeRun 一定要有，驗收的 Makefile 會用到
# 參數 $(PROG) 是不含 path 的測試檔案名稱
# judgeRun 的效果是由上一層的 $(PROG) 產生這一層的 $(PROG).out 
judgeRun: main
	./main ../$(PROG) $(PROG).out

main: main.cc parser.o scanner.o token.o
	g++ main.cc parser.o scanner.o token.o -o main

parser.o: parser.cc parser.h
	g++ -c parser.cc

scanner.o: scanner.cc scanner.h
	g++ -c scanner.cc

token.o: token.cc token.h
	g++ -c token.cc

# target clean 一定要有，驗收的 Makefile 會用到
# clean 的效果必須清掉程式執行檔與執行結果
clean:
	rm -f *.o
	rm -f *.out
	rm -f *~
	rm -f main
