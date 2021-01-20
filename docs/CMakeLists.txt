find_package(Doxygen REQUIRED dot)
if (DOXYGEN_FOUND)
    # Find all the public headers
    get_target_property(APTRACER_PUBLIC_HEADER_DIR APTracer INTERFACE_INCLUDE_DIRECTORIES)
    file(GLOB_RECURSE APTRACER_PUBLIC_HEADERS ${APTRACER_PUBLIC_HEADER_DIR}/*.h)

    set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/include)
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
    set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
    set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # Replace variables inside @@ with the current values
    configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

    # Doxygen won't create this for us
    file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR}/html)

    # Only regenerate Doxygen when the Doxyfile or public headers change
    add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
                      DEPENDS ${APTRACER_PUBLIC_HEADERS}
                      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                      MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
                      COMMENT "Generating docs"
                      VERBATIM)

    # Nice named target so we can run the job easily
    add_custom_target(documentation ALL DEPENDS ${DOXYGEN_INDEX_FILE})

    # Add an install target to install the docs
    include(GNUInstallDirs)
    install(DIRECTORY ${DOXYGEN_OUTPUT_DIR}/html
    DESTINATION ${CMAKE_INSTALL_DOCDIR})

else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the documentation.")
endif (DOXYGEN_FOUND)