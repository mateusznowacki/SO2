cmake_minimum_required(VERSION 3.30)
project(SO2)

set(CMAKE_CXX_STANDARD 20)

# Ustaw rozmiar stosu
math(EXPR stack_size "16 * 1024 * 1024")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,${stack_size}")

# Statyczne biblioteki C++ i GCC
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

# Pliki źródłowe
add_executable(SO2
    main.cpp
    client_handler.cpp
    user_manager.cpp
    chat_room.cpp
    chat_room_manager.cpp
)

# Jeśli masz nagłówki w osobnym katalogu (np. headers/)
# include_directories(headers) ← tylko jeśli faktycznie istnieje

# Dołącz bibliotekę WinSock2 (dla Windows socket API)
target_link_libraries(SO2 ws2_32)
