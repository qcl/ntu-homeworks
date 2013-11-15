all: trans agent

trans: b97501046_hw2_trans.c b97501046_hw2.o
	gcc b97501046_hw2_trans.c b97501046_hw2.o -o trans

agent: b97501046_hw2_agent.c b97501046_hw2.o
	gcc b97501046_hw2_agent.c b97501046_hw2.o -o agent

b97501046_hw2.o: b97501046_hw2.h b97501046_hw2.c
	gcc -c b97501046_hw2.c

clean:
	rm -f *~
	rm -f trans
	rm -f agent
	rm -f *.o
