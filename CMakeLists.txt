CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

PROJECT(micro-organism-monitor LANGUAGES C )

IF (WIN32)
	option(CMAKE_USE_WIN32_THREADS_INIT "using WIN32 threads" ON)
ENDIF ()

#################################
# GDB 设置
#################################
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(SRC_LIST ${PROJECT_SOURCE_DIR}/src/main.c
				${PROJECT_SOURCE_DIR}/src/lib/DHTXXWorkerThread.c
            	${PROJECT_SOURCE_DIR}/src/lib/DHTXX.c
				${PROJECT_SOURCE_DIR}/src/lib/RelayThread.c
            	${PROJECT_SOURCE_DIR}/src/lib/StepperMotor.c
				${PROJECT_SOURCE_DIR}/src/lib/I2C16X2.c
				${PROJECT_SOURCE_DIR}/src/lib/MarqueueThread.c)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++0x")

include_directories(${PROJECT_SOURCE_DIR}/src/include)
link_directories(/usr/lib)

message("PROJECT_NAME: ${PROJECT_NAME}")
message("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}/src")
message("PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
#################################
# 生成可执行文件
#################################
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} 
					pthread
					wiringPi
					wiringPiDev)
