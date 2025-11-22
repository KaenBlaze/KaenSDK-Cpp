cmake_minimum_required(VERSION 3.20)
project(roboos-sdk-cpp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(BUILD_EXAMPLES "Build example programs" ON)
option(BUILD_TESTS "Build test programs" OFF)

# Include directories
include_directories(include)

# Find packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

# Source files
set(SOURCES
    src/config/RoboOSConfig.cpp
    src/utils/errors.cpp
    src/utils/logger.cpp
    src/utils/http_client.cpp
    src/wallet/RobotWallet.cpp
    src/wallet/WalletStorage.cpp
    src/wallet/x402/x402.cpp
    src/fpr/FPRClient.cpp
    src/fpr/PaymentChannel.cpp
    src/verification/ZKVerification.cpp
    src/reputation/ReputationClient.cpp
    src/marketplace/TaskMarketplace.cpp
    src/marketplace/TaskTypes.cpp
    src/verification/ZKVerification.cpp
    src/reputation/ReputationClient.cpp
    src/robots/BaseRobot.cpp
    src/robots/ForkliftRobot.cpp
    src/robots/AMRRobot.cpp
    src/robots/CleaningRobot.cpp
    src/robots/HospitalRobot.cpp
    src/robots/DroneRobot.cpp
    src/robots/RoboticArm.cpp
    src/robots/AMRRobot.cpp
    src/robots/CleaningRobot.cpp
    src/robots/HospitalRobot.cpp
    src/robots/DroneRobot.cpp
    src/robots/RoboticArm.cpp
    src/RobotSDK.cpp
)

# Header files
set(HEADERS
    include/roboos/config/RoboOSConfig.h
    include/roboos/utils/errors.h
    include/roboos/utils/logger.h
    include/roboos/utils/http_client.h
    include/roboos/wallet/RobotWallet.h
    include/roboos/wallet/WalletStorage.h
    include/roboos/wallet/x402/x402.h
    include/roboos/fpr/FPRClient.h
    include/roboos/fpr/PaymentChannel.h
    include/roboos/verification/ZKVerification.h
    include/roboos/reputation/ReputationClient.h
    include/roboos/marketplace/TaskMarketplace.h
    include/roboos/marketplace/TaskTypes.h
    include/roboos/verification/ZKVerification.h
    include/roboos/reputation/ReputationClient.h
    include/roboos/robots/BaseRobot.h
    include/roboos/robots/ForkliftRobot.h
    include/roboos/robots/AMRRobot.h
    include/roboos/robots/CleaningRobot.h
    include/roboos/robots/HospitalRobot.h
    include/roboos/robots/DroneRobot.h
    include/roboos/robots/RoboticArm.h
    include/roboos/robots/AMRRobot.h
    include/roboos/robots/CleaningRobot.h
    include/roboos/robots/HospitalRobot.h
    include/roboos/robots/DroneRobot.h
    include/roboos/robots/RoboticArm.h
    include/roboos/RobotSDK.h
)

# Create library
add_library(roboos-sdk STATIC ${SOURCES} ${HEADERS})

target_include_directories(roboos-sdk PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(roboos-sdk PUBLIC
    ${CURL_LIBRARIES}
    ${JSONCPP_LIBRARIES}
)

target_compile_options(roboos-sdk PRIVATE
    ${CURL_CFLAGS_OTHER}
    ${JSONCPP_CFLAGS_OTHER}
)

# Examples
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Installation
install(TARGETS roboos-sdk
    EXPORT roboos-sdk-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT roboos-sdk-targets
    FILE roboos-sdk-targets.cmake
    NAMESPACE roboos-sdk::
    DESTINATION lib/cmake/roboos-sdk
)

