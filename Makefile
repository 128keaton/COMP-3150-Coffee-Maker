CXX = g++ -std=c++2a

all: clean CoffeeMaker

CoffeeMaker: sleep.cpp main.cpp CoffeeMaker.cpp
	$(CXX) $^ -o $@

clean:
	rm -f CoffeeMaker