cmake_minimum_required(VERSION 3.22)

# SUPPORTS_OS_Windows
# SUPPORTS_OS_Linux
# SUPPORTS_OS_Darwin

#set(PROJECT_ID "boppy") # <<<<= override default project name if desired
set(PROJECT_DESCRIPTION "Hello World Project")
set(PROJECT_VERSION 0.0.1.0)
set(PROJECT_COMPANY_NAME "My Company")
set(PROJECT_COMPANY_NAMESPACE "com.mycompany")  # Reverse domain name notation

set(LIBRARIES mssm_graphics_nanovg) # <<<<= Choose the libraries to include

#include(${PROJECT_SOURCE_DIR}/cmake/glsl-shaders.cmake)

# -----------  call ProjectInit.cmake before project()  ----------------
include(${CMAKE_SOURCE_DIR}/../libraries/cmake/ProjectInit.cmake)

add_executable(${PROJECT_NAME}
  main.cpp
)

add_subdirectory(packaging)
add_subdirectory(src)


# -----------  call ProjectFinalize.cmake last thing ----------------
include(${CMAKE_SOURCE_DIR}/../libraries/cmake/ProjectFinalize.cmake)


