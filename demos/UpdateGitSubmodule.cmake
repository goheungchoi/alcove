function(update_git_submodule path)
  message(STATUS "Updating Git submodules in ${path}")

  find_package(Git REQUIRED)
  if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
  # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
      message(STATUS "${path}: Submodule update")
      execute_process(
        COMMAND           ${GIT_EXECUTABLE} submodule update --init --recursive ${path}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE   GIT_SUBMODULE_RESULT
      )
    
      if (NOT GIT_SUBMODULE_RESULT EQUAL "0")
        message(FATAL_ERROR "
          `git submodule update --init --recursive ${path}` 
          failed with ${GIT_SUBMODULE_RESULT}, 
          please checkout this submodule."
        )
      endif()
    endif()

  endif()

  if (EXISTS ${path}/CMakeLists.txt)
    message(STATUS "${path}: Add subdirectory.")
    add_subdirectory(${path})
  endif()
endfunction()