all: ping

ping: b97501046_hw3_pingtalk.cpp cnping.o
	g++ b97501046_hw3_pingtalk.cpp cnping.o -lpthread -o ping

cnping.o: cnping.h cnping.cc
	g++ cnping.cc -c

clean:
	rm -rf *.o
	rm -rf ping
