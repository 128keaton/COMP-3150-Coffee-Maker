cmake_minimum_required(VERSION 3.20)
project(CoffeeMaker)

set(CMAKE_CXX_STANDARD 20)

add_executable(CoffeeMaker main.cpp type/Readable.h CoffeeMaker.cpp CoffeeMaker.h type/Fillable.h component/Boiler.h state/BoilerState.h component/Carafe.h state/CarafeState.h type/StateChanges.h state/CoffeeMakerState.h type/Heatable.h state/HeaterState.h helpers/ProgressBar.h)
