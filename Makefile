.PHONY: all clean
all: otp
otp: main.o File.o
	g++ main.o File.o -o otp
main.o: main.cpp
	g++ -c -lpthread main.cpp
File.o: File.cpp
	g++ -c -lpthread File.cpp
clean:
	rm -rf *.o otp