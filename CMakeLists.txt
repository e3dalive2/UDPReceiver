cmake_minimum_required(VERSION 3.9)
project(UDPReceiver CXX)

include(FetchContent)
include(ExternalProject)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories for header files
include_directories(".") # for spdlog

# Add all .cpp files from src as sources for the executable
file(GLOB SRC_FILES *.cpp)
file(GLOB HEADER_FILES *.h *.hpp)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.14.1 
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG asio-1-31-0  
)
FetchContent_MakeAvailable(asio)

FetchContent_Declare(
  utfcpp
  GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git
  GIT_TAG v4.0.5 
)
FetchContent_MakeAvailable(utfcpp)

FetchContent_Declare(
  concurrentqueue
  GIT_REPOSITORY https://github.com/cameron314/concurrentqueue.git
  GIT_TAG v1.0.4
)
FetchContent_MakeAvailable(concurrentqueue)


# Create the executable from all .cpp files + add better ide visibility
add_executable(UDPReciever ${SRC_FILES} ${HEADER_FILES})
target_include_directories(UDPReciever PRIVATE ${asio_SOURCE_DIR})
include_directories(${asio_SOURCE_DIR}/asio/include)

target_include_directories(UDPReciever PRIVATE ${utfcpp_SOURCE_DIR})
target_include_directories(UDPReciever PRIVATE ${concurrentqueue_SOURCE_DIR})



if(WIN32)
    target_compile_definitions(UDPReciever PRIVATE SPDLOG_WCHAR_TO_UTF8_SUPPORT)
endif()
target_compile_definitions(UDPReciever PRIVATE SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 
_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS _WINSOCK_DEPRECATED_NO_WARNINGS ASIO_STANDALONE)
 

target_link_libraries(UDPReciever PRIVATE spdlog::spdlog)