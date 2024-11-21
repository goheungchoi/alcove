# FindNVTT.cmake 
# locate NVIDIA Texture Tools (NVTT)
# Referenced https://github.com/nvpro-samples/nvtt_samples/blob/main/CMakeLists.txt

find_path(NVTT_INCLUDE_DIR 
  NAMES nvtt.h nvtt_lowlevel.h nvtt_wrapper.h
  PATHS
  "/usr/local/"
  "/usr/"
  "$ENV{NVTT_DIR}/"
  "C:/Program Files/NVIDIA Corporation/NVIDIA Texture Tools/"
  "C:/Program Files (x86)/NVIDIA Corporation/NVIDIA Texture Tools/"
  "${CMAKE_CURRENT_SOURCE_DIR}/"
  PATH_SUFFIXES include include/nvtt
)

cmake_print_variables(NVTT_INCLUDE_DIR)

find_library(NVTT_LIBRARY
  NAMES nvtt30205.dll nvtt30205.lib nvtt.dll nvtt*.dll nvtt*.lib libnvtt.so.*
  PATHS
    "/usr/local/lib"
    "/usr/lib"
    "$ENV{NVTT_DIR}"
    "C:/Program Files/NVIDIA Corporation/NVIDIA Texture Tools"
    "C:/Program Files/NVIDIA Corporation/NVIDIA Texture Tools"
    "C:/Program Files (x86)/NVIDIA Corporation/NVIDIA Texture Tools"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/nvtt"
    NO_DEFAULT_PATH
  PATH_SUFFIXES lib
)

cmake_print_variables(NVTT_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NVTT DEFAULT_MSG NVTT_LIBRARY NVTT_INCLUDE_DIR)

mark_as_advanced(NVTT_INCLUDE_DIR NVTT_LIBRARY)

if(NVTT_FOUND)
  set(NVTT_LIBRARIES ${NVTT_LIBRARY})
  set(NVTT_INCLUDE_DIRS ${NVTT_INCLUDE_DIR})

  # Set RPATH for Linux/Mac to find shared libraries in the executable directory
  if(UNIX AND NOT APPLE)
    set(CMAKE_SKIP_BUILD_RPATH  FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
  endif()

else()
  message(WARNING "NVTT not found! Please install NVTT and set the NVTT_DIR environment variable to the root directory of NVTT.")
endif()
