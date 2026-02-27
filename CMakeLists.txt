cmake_minimum_required(VERSION 3.10)
project(ZeroElo_Bot)

set(CMAKE_CXX_STANDARD 17)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

# --- Common Source Files ---
set(SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/PieceUtil.cpp
    ${SRC_DIR}/Board.cpp
    ${SRC_DIR}/MoveUtil.cpp
    ${SRC_DIR}/MoveList.cpp
    ${SRC_DIR}/Evaluation.cpp
    ${SRC_DIR}/uci.cpp
    ${SRC_DIR}/SearchUtil.cpp
    ${SRC_DIR}/Transposition.cpp
    ${SRC_DIR}/TimeManager.cpp
)

# --- Build Logic ---
if(EMSCRIPTEN)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/binWasm)
    
    add_executable(ZeroElo_Bot ${SOURCES})
    set_target_properties(ZeroElo_Bot PROPERTIES OUTPUT_NAME "ZeroEloWasm" SUFFIX ".js" LINK_FLAGS "-fexceptions -frtti --no-entry -s MODULARIZE=1 -s EXPORT_ES6=1 -s ENVIRONMENT=web,worker -s EXPORTED_FUNCTIONS=['_main','_send_uci_command'] -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap','FS'] -s ALLOW_MEMORY_GROWTH=1 -s STACK_SIZE=16777216 -s INITIAL_MEMORY=134217728 -O3 -flto -s SINGLE_FILE=1")
   
else()
    # Native (Desktop) Specific Flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -march=native -funroll-loops -flto -fomit-frame-pointer -ffast-math -ftree-vectorize -fstrict-aliasing -D_FORTIFY_SOURCE=2 -fno-exceptions -w")
    add_executable(ZeroElo_Bot ${SOURCES})
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
endif()

include_directories(${SRC_DIR})