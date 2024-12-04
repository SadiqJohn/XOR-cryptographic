all:
	g++ -std=c++17 -pthread encryptUtil.cpp -o encryptUtil

clean:
	rm -f encryptUtil
