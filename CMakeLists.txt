cmake_minimum_required(VERSION 3.12)
project(stest VERSION 0.0.1)

set(DEBUG_MODE TRUE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_PREFIX_PATH C:/Users/pepit/libraries)

# Include the source directory
include_directories(src)

# Link FMT library
set(FMT_PATH ${CMAKE_PREFIX_PATH}/fmt-10.1.0)
set(FMT_LIBRARIES ${FMT_PATH}/build/libfmt.a)
include_directories(${FMT_PATH}/include)

set(JKR_SOURCE_FILES
  # src/main.cpp

  src/jkr/sort/quicksort.hpp

  src/jkr/type.hpp

  src/jkr/type/bitmap.cpp
  src/jkr/type/bitmap.hpp
)

file(GLOB TEST_FILES "src/test/*.cpp")

foreach(TEST_FILE ${TEST_FILES})
  get_filename_component(EXECUTABLE_NAME ${TEST_FILE} NAME_WE)

  add_executable(${EXECUTABLE_NAME} ${TEST_FILE} ${JKR_SOURCE_FILES})

  target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${FMT_LIBRARIES})
endforeach()

# Console Build (DEBUG)
if (${DEBUG_MODE})
  add_compile_definitions(JKR_DEBUG_PRINT) # Enables .print() in classes using fmt
endif()
