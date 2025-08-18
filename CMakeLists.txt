cmake_minimum_required(VERSION 3.16)
project(OrderBook)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GTest
find_package(GTest REQUIRED)

# Main library
add_library(OrderBookLib
    src/OrderBook.cpp
    src/OrderBook.hpp
    src/LevelInfo.hpp
    src/Order.hpp
    src/constants.hpp
    src/Usings.hpp
    src/OrderType.hpp
    src/TradeInfo.hpp
    src/ModifyOrder.hpp
    src/OrderSide.hpp
)

# Test executable
add_executable(OrderbookTest
    OrderbookTest/test.cpp
    OrderbookTest/pch.cpp
    OrderbookTest/pch.h
)

target_link_libraries(OrderbookTest
    OrderBookLib
    GTest::gtest
    GTest::gtest_main
)

# Copy test files to build directory
file(COPY OrderbookTest/TestFiles DESTINATION ${CMAKE_BINARY_DIR})

# Enable testing
enable_testing()
add_test(NAME OrderbookTests COMMAND OrderbookTest)

# Set working directory for tests
set_tests_properties(OrderbookTests PROPERTIES
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
