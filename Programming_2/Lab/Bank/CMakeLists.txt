cmake_minimum_required(VERSION 3.27.6)

project(bank)

# set a 32Mb stack size
set(STACK_SIZE_BYTES 32000000)

if(WIN32)
    # append the stack size linker option to the CMAKE_EXE_LINKER_FLAGS
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,${STACK_SIZE_BYTES}")
else()
    # append the stack size linker option to the CMAKE_EXE_LINKER_FLAGS
    set(CMAKE_EXE_LINKER_FlAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,stack-size=${STACK_SIZE_BYTES}")
endif()

if(MINGW)
    link_libraries(ws2_32 wsock32)
endif()

add_executable(bank ./src/main.cpp)

add_subdirectory(./src/lib/merge_sort)
add_subdirectory(./src/lib/search_ops)
add_subdirectory(./src/lib/file_ops)
add_subdirectory(./src/lib/transaction)
#add_subdirectory(./src/lib/list)

target_include_directories(${PROJECT_NAME}
    PUBLIC ./src/lib
    PUBLIC ./src/lib/merge_sort
    PUBLIC ./src/lib/search_ops
    PUBLIC ./src/lib/file_ops
    PUBLIC ./src/lib/transaction
    PUBLIC ./src/lib/list
)

target_link_directories(${PROJECT_NAME}
    PRIVATE merge_sort
    PRIVATE search_ops
    PRIVATE file_ops
    PRIVATE transaction
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE merge_sort
    PRIVATE search_ops
    PRIVATE file_ops
    PRIVATE transaction
)