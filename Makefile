CXX = g++ -std=c++2a  -pthread

all: clean CoffeeMaker

CoffeeMaker: main.cpp CoffeeMaker.cpp
	$(CXX) $^ -o $@

clean:
	rm -f CoffeeMaker