cmake_minimum_required(VERSION 3.20)
project(CoffeeMaker)

set(CMAKE_CXX_STANDARD 20)


add_executable(CoffeeMaker main.cpp CoffeeMaker.cpp CoffeeMaker.h component/Boiler.h component/Carafe.h state/CoffeeMakerState.h type/Heater.h type/Tank.h component/HotPlate.h helpers/StringFormatter.h helpers/Indicators.hpp  "sleep.h" "sleep.cpp")
add_compile_definitions(_USE_MATH_DEFINES)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(CoffeeMaker PRIVATE Threads::Threads)