powerpc:
	powerpc-e300c3-linux-gnu-g++ *.cpp -lpthread -lm
arm:
	arm-linux-g++ *.cpp -lpthread -lm
pc:
	g++ *.cpp -lpthread -lm -g -O3
clean:
	rm -rf a.out
