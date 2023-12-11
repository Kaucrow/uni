cmake_minimum_required(VERSION 3.27.6)

project(blockbuster)

# set a 32Mb stack size
set(STACK_SIZE_BYTES 32000000)

if(WIN32)
    set(BOOST_ROOT "C:/Boost")
    set(BOOST_INCLUDEDIR "C:/Boost/include/boost-1_83/")
    set(Boost_USE_STATIC_RUNTIME ON)
    # append the stack size linker option to the CMAKE_EXE_LINKER_FLAGS
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,${STACK_SIZE_BYTES}")
else()
    set(BOOST_INCLUDEDIR "/usr/include/boost/")
    # append the stack size linker option to the CMAKE_EXE_LINKER_FLAGS
    set(CMAKE_EXE_LINKER_FlAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,stack-size=${STACK_SIZE_BYTES}")
endif()

if(MINGW)
    link_libraries(ws2_32 wsock32)
endif()

find_package(Boost REQUIRED COMPONENTS locale)

add_executable(blockbuster ./src/main.cpp)

add_subdirectory(./src/lib/merge_sort)
add_subdirectory(./src/lib/store_frag)
add_subdirectory(./src/lib/rent_movie)
add_subdirectory(./src/lib/search_ops)
add_subdirectory(./src/lib/file_ops)
add_subdirectory(./src/lib/misc)
add_subdirectory(./src/test)

target_include_directories(${PROJECT_NAME}
    PUBLIC ./src/lib
    PUBLIC ./src/lib/merge_sort
    PUBLIC ./src/lib/store_frag
    PUBLIC ./src/lib/rent_movie
    PUBLIC ./src/lib/search_ops
    PUBLIC ./src/lib/file_ops
    PUBLIC ./src/lib/list
    PUBLIC ./src/lib/misc
    PUBLIC ./src/test
)

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

target_link_directories(${PROJECT_NAME}
    PRIVATE merge_sort
    PRIVATE store_frag
    PRIVATE rent_movie
    PRIVATE file_ops
    PRIVATE search_ops
    PRIVATE misc
    PRIVATE test
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE merge_sort
    PRIVATE store_frag
    PRIVATE rent_movie
    PRIVATE search_ops
    PRIVATE file_ops
    PRIVATE misc
    PRIVATE rand_gen
    PRIVATE test
)