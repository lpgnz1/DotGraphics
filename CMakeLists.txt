# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Name of the project
project(DotGraphics)

# Set Clang as the compiler explicitly (optional, but recommended)
set(CMAKE_C_COMPILER clang-cl)
set(CMAKE_CXX_COMPILER clang-cl)

# Set the executable to be a Win32 GUI app (Global)
# set(CMAKE_WIN32_EXECUTABLE TRUE)

# Add the main.cpp source file
add_executable(DotGraphics 
main.cpp
Draw.cpp)

# Link against GDI32 library, which is needed for basic window functions
target_link_libraries(DotGraphics PRIVATE gdi32)

# Specify that this is a Win32 GUI app, not a console app (executable dependant)
set_target_properties(DotGraphics PROPERTIES WIN32_EXECUTABLE TRUE)

# Set linker so clang can use Microsoft Visual C++
set(CMAKE_LINKER "link.exe")