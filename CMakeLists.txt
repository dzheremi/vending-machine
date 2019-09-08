cmake_minimum_required(VERSION 3.3)
project(Code)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -std=c99")

set(SOURCE_FILES
    ppd_coin.c
    ppd_coin.h
    ppd_main.c
    ppd_main.h
    ppd_menu.c
    ppd_menu.h
    ppd_options.c
    ppd_options.h
    ppd_stock.c
    ppd_stock.h
    ppd_utility.c
    ppd_utility.h)

add_executable(ppd ${SOURCE_FILES})