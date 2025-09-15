OUTPUT ?= output.txt
GREEN := \033[0;32m
RED := \033[0;31m
.PHONY: all clean test
all: otp
otp: main.o File.o
	g++ main.o File.o -o otp
main.o: main.cpp
	g++ -c -lpthread main.cpp
File.o: File.cpp
	g++ -c -lpthread File.cpp
clean:
	rm -rf *.o otp
	
test: otp $(INPUT)
	@echo "Running a test with parameters:"
	@echo "  X=$(X), A=$(A), C=$(C), M=$(M)"
	@echo "  Input: $(INPUT), Output: $(OUTPUT)"
	./otp -i $(INPUT) -o $(OUTPUT) -x $(X) -a $(A) -c $(C) -m $(M)
	./otp -i $(OUTPUT) -o  decrypted_$(INPUT) -x $(X) -a $(A) -c $(C) -m $(M)
	@if diff $(INPUT) decrypted_$(INPUT) > /dev/null; then \
		echo " $(GREEN) TEST PASSED"; \
	else \
		echo " $(RED) TEST FAILED"; \
		exit 1; \
	fi



