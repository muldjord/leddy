cmake_minimum_required(VERSION 3.16)

project(leddy VERSION 1.1.1 LANGUAGES C CXX)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_definitions(-DVERSION=${VERSION})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

# Set build type defined with '-DBUILD_TYPE=type'
if(DEFINED BUILD_TYPE)
  string(TOLOWER "${BUILD_TYPE}" BUILD_TYPE)
  if(BUILD_TYPE STREQUAL "debug")
    set(CMAKE_BUILD_TYPE debug CACHE STRING "Build type" FORCE)
  elseif(BUILD_TYPE STREQUAL "release")
    set(CMAKE_BUILD_TYPE release CACHE STRING "Build type" FORCE)
  else()
    message(FATAL_ERROR "Unknown build type: ${BUILD_TYPE}")
  endif()
endif()

# Default to 'release' type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/debug CACHE PATH "Install path prefix" FORCE)
  else()
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/release CACHE PATH "Install path prefix" FORCE)
  endif()
endif()

set(MATRIX_IMPL_ALLOWED
  MATRIXSIM
  MATRIXUNI
  MATRIXADA
)

# Set LED Matrix implementation with 'cmake .. -DMATRIX_IMPL=MATRIXEXAMPLE'.
if(NOT DEFINED MATRIX_IMPL)
  message(FATAL_ERROR
    "MATRIX_IMPL undefined!\n"
    "Please set with 'cmake -DMATRIX_IMPL=MATRIXEXAMPLE ..'\n"
    "Valid values are: ${MATRIX_IMPL_ALLOWED}.\n"
    "Check the documentation for more details."
  )
endif()

if(NOT MATRIX_IMPL IN_LIST MATRIX_IMPL_ALLOWED)
  message(FATAL_ERROR
    "Invalid MATRIX_IMPL='${MATRIX_IMPL}'."
    "Valid values are: MATRIXSIM, MATRIXUNI, MATRIXADA"
  )
endif()

message(STATUS "Building for LED matrix type: ${MATRIX_IMPL}")

set(SOURCES
  src/animation.cpp
  src/backgrounds.cpp
  src/commandhandler.cpp
  src/commandqueue.cpp
  src/EasyGifReader.cpp
  src/gallery.cpp
  src/gameoflife.cpp
  src/leddy.cpp
  src/loader.cpp
  src/main.cpp
  src/matrixabstract.cpp
  src/netcomm.cpp
  src/pixelfont.cpp
  src/rssscroll.cpp
  src/runcommand.cpp
  src/scene.cpp
  src/snowfall.cpp
  src/timedate.cpp
  src/transition.cpp
  src/weather.cpp
)

if(MATRIX_IMPL STREQUAL "MATRIXSIM")
  find_package(Qt6 REQUIRED COMPONENTS Widgets Core Network Xml)
  list(APPEND SOURCES
    src/matrixsim.cpp
  )
elseif(MATRIX_IMPL STREQUAL "MATRIXUNI")
  find_package(Qt6 REQUIRED COMPONENTS Gui Core Network Xml)
  list(APPEND SOURCES
    src/matrixuni.cpp
  )
elseif(MATRIX_IMPL STREQUAL "MATRIXADA")
  find_package(Qt6 REQUIRED COMPONENTS Gui Core Network Xml)
  list(APPEND SOURCES
    src/matrixada.cpp
  )
endif()

qt_standard_project_setup()

find_package(GIF REQUIRED)
message(STATUS "LibGif include directory: ${GIF_INCLUDE_DIRS}")
message(STATUS "LibGif libraries: ${GIF_LIBRARIES}")

qt_add_executable(leddy
  ${SOURCES}
)

if(MATRIX_IMPL STREQUAL "MATRIXADA")
  add_subdirectory(rpi-rgb-led-matrix)
  target_link_libraries(leddy
    PRIVATE ledmatrix
  )
endif()

target_compile_definitions(leddy PRIVATE ${MATRIX_IMPL})

include_directories(${GIF_INCLUDE_DIRS})

if(MATRIX_IMPL STREQUAL "MATRIXSIM")
  target_link_libraries(leddy PRIVATE Qt6::Widgets Qt6::Network Qt6::Xml ${GIF_LIBRARIES})
else()
  target_link_libraries(leddy PRIVATE Qt6::Gui Qt6::Network Qt6::Xml ${GIF_LIBRARIES})
endif()

install(TARGETS leddy
  RUNTIME DESTINATION .
)

install(FILES
  ${CMAKE_SOURCE_DIR}/AUTHORS
  ${CMAKE_SOURCE_DIR}/LICENSE
  ${CMAKE_SOURCE_DIR}/config.ini.example
  ${CMAKE_SOURCE_DIR}/README.md
  DESTINATION .
)

install(CODE "
  execute_process(
    COMMAND \"${CMAKE_COMMAND}\" -E create_symlink
            \"${CMAKE_INSTALL_PREFIX}/../themes\"
            \"${CMAKE_INSTALL_PREFIX}/themes\"
  )
")
