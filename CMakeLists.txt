cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)
project(writer)

if (NOT ParaView_BINARY_DIR)
  find_package(ParaView REQUIRED)
  include(${PARAVIEW_USE_FILE})
endif()

# Set a consistent MACOSX_RPATH default across all CMake versions.
# When CMake 2.8.12 is required, change this default to 1.
# When CMake 3.0.0 is required, remove this block (see CMP0042).
if(NOT DEFINED CMAKE_MACOSX_RPATH)
  set(CMAKE_MACOSX_RPATH 0)
endif()

include(ParaViewPlugins)

# for the code to be aware of the debug mode
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DWRITER_DEBUG")

# create a paraview plugin containing server manager xml and the server
# manager classes to build
# this plugin can be loaded on the server side

add_paraview_plugin(Writer "1.0"
    SERVER_MANAGER_XML Writer.xml
    SERVER_MANAGER_SOURCES vtkGenericWriter.cxx)

