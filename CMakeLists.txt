cmake_minimum_required(VERSION 3.9)
project(UDPReciever CXX)

include(FetchContent)
include(ExternalProject)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories for header files
include_directories(include)
include_directories(".")

# Add all .cpp files from src as sources for the executable
file(GLOB SRC_FILES *.cpp)
file(GLOB HEADER_FILES *.h *.hpp)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.14.1  # Specify the version you want
)
FetchContent_MakeAvailable(spdlog)


# Fetch ASIO (the standalone version)
FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG asio-1-31-0  # Specify the version you want
)
FetchContent_MakeAvailable(asio)


# Create the executable from all .cpp files + add better ide visibility
add_executable(UDPReciever ${SRC_FILES} ${HEADER_FILES})
target_include_directories(UDPReciever PRIVATE ${asio_SOURCE_DIR})
target_link_libraries(UDPReciever PRIVATE spdlog::spdlog)