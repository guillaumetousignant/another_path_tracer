find_package(Doxygen REQUIRED dot)
if (DOXYGEN_FOUND)
    # set input file
    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/docs/Doxyfile)

    # note the option ALL which allows to build the docs together with the application
    add_custom_target(documentation ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen."
        VERBATIM)
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the documentation.")
endif (DOXYGEN_FOUND)