# FindNVTT.cmake 
# locate NVIDIA Texture Tools (NVTT)
# Referenced https://github.com/nvpro-samples/nvtt_samples/blob/main/CMakeLists.txt

find_path(NVTT_INCLUDE_DIR 
  NAMES nvtt.h nvtt_lowlevel.h nvtt_wrapper.h
  HINTS
    "/usr/local/include"
    "/usr/include"
    "${CMAKE_CURRENT_LIST_DIR}/../include"
    "$ENV{NVTT_DIR}/include"
    "C:/Program Files/NVIDIA Corporation/NVIDIA Texture Tools/include"
    "C:/Program Files (x86)/NVIDIA Corporation/NVIDIA Texture Tools/include"
)

find_library(NVTT_LIBRARY
  NAMES nvtt30205 nvtt nvtt*
  HINTS
    "/usr/local/lib"
    "/usr/lib"
    "${CMAKE_CURRENT_LIST_DIR}/../lib"
    "$ENV{NVTT_DIR}/lib"
    "C:/Program Files/NVIDIA Corporation/NVIDIA Texture Tools/lib"
    "C:/Program Files (x86)/NVIDIA Corporation/NVIDIA Texture Tools/lib"
    NO_DEFAULT_PATH
)

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
