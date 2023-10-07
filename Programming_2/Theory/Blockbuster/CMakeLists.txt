cmake_minimum_required(VERSION 3.27.6)

project(blockbuster)
set(BOOST_ROOT "C:/Boost")
set(BOOST_INCLUDEDIR "C:/Boost/include/boost-1_83/")
set(Boost_USE_STATIC_RUNTIME ON)

if(MINGW)
    link_libraries(ws2_32 wsock32)
endif()

find_package(Boost REQUIRED COMPONENTS locale)

add_executable(blockbuster ./src/main.cpp)

if(Boost_FOUND)
    target_include_directories(${PROJECT_NAME}
        PUBLIC ${Boost_INCLUDE_DIR}
    )
    target_link_libraries(${PROJECT_NAME}
        PRIVATE Boost::locale
    )
    message(STATUS "Found BOOST ${Boost_VERSION_STRING}")
else()
    message(STATUS "WEBSOCKET BOOST NOT FOUND")
endif()