CXX = g++ -std=c++20  -pthread

all: clean CoffeeMaker

CoffeeMaker: main.cpp CoffeeMaker.cpp
	$(CXX) $^ -o $@

clean:
	rm -f CoffeeMaker