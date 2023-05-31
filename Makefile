alice:
	g++ -fexec-charset=GBK Alice.cpp DH.o main.o -o Alice.exe -lws2_32

bob:
	g++ -fexec-charset=GBK Bob.cpp DH.o main.o -o Bob.exe -lws2_32

eve:
	g++ -fexec-charset=GBK Eve.cpp DH.o main.o -o Eve.exe -lws2_32

des:
	g++ -fexec-charset=GBK -c DES.h DES.cpp

dh:
	g++ -fexec-charset=GBK -c DH.h DH.cpp

main:
	g++ -fexec-charset=GBK -c main.h main.cpp DES.cpp