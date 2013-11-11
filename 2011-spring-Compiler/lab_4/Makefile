# 這是驗收時用的，不要更改

# 執行學生的 make judgeRun 後，比較學生答案與參考答案
# 學生答案位在下一層，參考答案位在這一層
# 參數 $(STU) -- 學生子目錄，即學號，英文字母小寫
# 參數 $(PROG) -- 不含 path，不含副檔名的測試程式名稱
# 參數 $(DATA) -- 整數，代表測試資料編號
# 範例： make judge STU=b97902888 PROG=swap DATA=1
judge:
	$(MAKE) judgeRun -C $(STU) PROG=$(PROG) DATA=$(DATA)
	diff -s --strip-trailing-cr $(STU)/$(PROG).cmm.$(DATA).out $(PROG).cmm.$(DATA).out

# 執行學生的 make clean
clean:
	$(MAKE) clean -C $(STU)