# Copy files with a specific extension in a src directory to the dst directory
function(copy_files_by_extension src dst extension)
  # Find the spv files in the directory
  file(GLOB_RECURSE FILES "${src}/*.${extension}")

  foreach(FILE ${FILES})
    cmake_path(GET FILE FILENAME FILE_NAME)
  
    set(COPIED_FILE "${dst}/${FILE_NAME}")
  
    message(STATUS "${dst}/${FILE_NAME}")
    add_custom_command(
      OUTPUT ${COPIED_FILE}
      COMMAND ${CMAKE_COMMAND} -E copy ${FILE} ${dst}
      DEPENDS ${FILE}
    )

    list(APPEND COPIED_FILES ${COPIED_FILE})
  endforeach()

  add_custom_target(
    CopyFileByExtension
    ALL
    DEPENDS ${COPIED_FILES}
  )

  add_custom_command(
    TARGET CopyFileByExtension
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${dst}
  )
endfunction()
