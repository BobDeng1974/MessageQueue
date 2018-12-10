powerpc:
	powerpc-e300c3-linux-gnu-g++ ztest.cpp -lpthread -lm
arm:
	arm-linux-g++ ztest.cpp -lpthread -lm
pc:
	g++ ztest.cpp -lpthread -lm -g
clean:
	rm -rf a.out
