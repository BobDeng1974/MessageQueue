powerpc:
	powerpc-e300c3-linux-gnu-g++ test.cpp -lpthread
arm:
	arm-linux-g++ test.cpp -lpthread
pc:
	g++ test.cpp -lpthread -g
clean:
	rm -rf a.out
