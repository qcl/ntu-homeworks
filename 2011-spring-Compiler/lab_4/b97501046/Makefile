# target run 用來測試自創測試檔，驗收的 Makefile 不會用到
# 參數 $(PROG) -- 不含 path，不含副檔名的測試程式名稱 (如：swap)
# 參數 $(DATA) -- 整數，代表測試資料編號 (如：1)
# run 的效果是：
#   (1) 由這一層的 swap.cmm 先產生其 assembly，然後組譯成執行檔
#   (2) 餵入這一層的 swap.cmm.1，產生這一層的 swap.cmm.1.out
run: 
	mkdir -p genAsm
	$(MAKE) run -C compiler PROG=$(PROG)
	java -jar ../bass/bass.jar genAsm/$(PROG).bass
	java -cp ".:compiler/lib" $(PROG) < $(PROG).cmm.$(DATA) > $(PROG).cmm.$(DATA).out

# target judgeRun 一定要有，驗收的 Makefile 會用到
# 參數 $(PROG) -- 不含 path，不含副檔名的測試程式名稱 (如：swap)
# 參數 $(DATA) -- 整數，代表測試資料編號 (如：1)
# judgeRun 的效果是：
#   (1) 由上一層的 swap.cmm 先產生其 assembly，然後組譯成執行檔
#   (2) 餵入上一層的 swap.cmm.1，產生這一層的 swap.cmm.1.out
judgeRun: $(PROG).class
	java -cp ".:compiler/lib" $(PROG) < ../$(PROG).cmm.$(DATA) > $(PROG).cmm.$(DATA).out

# 這個 target 是用來達成上述效果 (1) 的
$(PROG).class:
	mkdir -p genAsm
	$(MAKE) judgeRun -C compiler PROG=$(PROG)
	java -jar ../bass/bass.jar genAsm/$(PROG).bass
	
# target clean 一定要有，驗收的 Makefile 會用到
# clean 的效果必須清掉 compiler 程式執行檔、測試程式 assembly 檔、
#                      測試程式執行檔、測試程式執行結果
clean:
	$(MAKE) clean -C compiler
	rm -rf genAsm
	rm -f *.class
	rm -f *~
	rm -f *.out

