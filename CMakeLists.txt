cmake_minimum_required(VERSION 3.20)
project(CoffeeMaker)

set(CMAKE_CXX_STANDARD 20)

add_executable(CoffeeMaker main.cpp CoffeeMaker.cpp CoffeeMaker.h component/Boiler.h component/Carafe.h state/CoffeeMakerState.h helpers/ProgressBar.h type/Heater.h type/Tank.h component/HotPlate.h helpers/StringFormatter.h)
