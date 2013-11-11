# 這是驗收時用的，不要更改

# 執行學生的 make judgeRun 後，比較學生答案與參考答案
# 學生答案位在下一層，參考答案位在這一層
judge:
	$(MAKE) judgeRun -C $(STU) GRAM=$(GRAM)
	diff -s --strip-trailing-cr $(STU)/$(GRAM).csv $(GRAM).csv

# 執行學生的 make clean
clean:
	$(MAKE) clean -C $(STU)