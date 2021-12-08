cmake_minimum_required(VERSION 3.20)
project(CoffeeMaker)

set(CMAKE_CXX_STANDARD 20)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG f7c6bf91a7adf9eddc32593c0151caea54c17434
        )

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------------------------

add_executable(CoffeeMaker main.cpp CoffeeMaker.cpp CoffeeMaker.h component/Boiler.h component/Carafe.h state/CoffeeMakerState.h type/Heater.h type/Tank.h component/HotPlate.h helpers/StringFormatter.h helpers/Indicators.hpp  "sleep.h" "sleep.cpp")
add_compile_definitions(_USE_MATH_DEFINES)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_include_directories(CoffeeMaker PRIVATE src)
target_link_libraries(CoffeeMaker
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
        PRIVATE Threads::Threads)