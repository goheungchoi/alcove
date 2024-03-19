if(BUILD_TESTING)
  find_package(Catch2 REQUIRED)
  cmake_print_variables(Catch2_VERSION)

  set(TEST_MAIN     "ctest_test"                                  )
  set(TEST_SOURCES  "main.cpp"                                    )
  set(TEST_INCLUDES "./" ${Alcove_SOURCE_DIR} ${Alcove_INCLUDE_DIR} ${Catch2_INCLUDE_DIR} )

  cmake_print_variables(TEST_INCLUDES)

  add_executable(             ${TEST_MAIN}         ${TEST_SOURCES}               )
  target_include_directories( ${TEST_MAIN} PRIVATE ${TEST_INCLUDES}              )
  target_link_libraries(      ${TEST_MAIN} PRIVATE "fact" Catch2::Catch2WithMain )

  add_test(NAME ${TEST_MAIN} COMMAND ${TEST_MAIN})

  # Add Subdirectories
  # add_subdirectory(core)
endif()