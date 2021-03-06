set(EXECUTABLE_NAME rs)
SET(EXECUTABLE_LINK_LIBS "")
SET(EXECUTABLE_LINK_FLAGS ${CMAKE_EXE_LINKER_FLAGS_DEBUG})
SET(MODUL_NAME ${EXECUTABLE_NAME})

list(APPEND EXECUTABLE_LINK_LIBS
    boost
    ZLIB
 )

list(APPEND EXECUTABLE_LINK_LIBS
    MetaRuntime
    CFramework
 ) 

file(GLOB_RECURSE EXECUTABLE_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE MODUL_HEADER_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h *.hpp)

meta_parser_prebuild(
    TARGET ${MODUL_NAME}
    GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/.Assembly"
    SOURCE_ROOT "${CMAKE_CURRENT_LIST_DIR}"
    HEADER_FILES ${MODUL_HEADER_SRCS}
    MODULE_HEADER "${MODUL_NAME}AssemblyModul.h"
    OUT_MODULE_SOURCE META_MODULE_SOURCE
    OUT_GENERATED_FILES META_GENERATED_FILES
    OUT_INC META_GENERATED_HEADERS
    OUT_SRC META_GENERATED_SOURCES
)

LIST(APPEND EXECUTABLE_SRCS ${MODUL_HEADER_SRCS})

group_source(${EXECUTABLE_SRCS})
AppendDefaultIncludeDir()

LIST(APPEND EXECUTABLE_SRCS ${META_GENERATED_HEADERS})
LIST(APPEND EXECUTABLE_SRCS ${META_GENERATED_SOURCES})

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/XamlCPP
    )

CreateExecutable("${EXECUTABLE_NAME}" "${EXECUTABLE_SRCS}" "${EXECUTABLE_LINK_LIBS}" "${EXECUTABLE_LINK_FLAGS}")
CreateCxxPch("${EXECUTABLE_NAME}" "${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.h" "${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.cpp")
add_dependencies("${MODUL_NAME}" MetaParser)

meta_parser_build(
    TARGET ${MODUL_NAME}
    GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/.Assembly"
    SOURCE_ROOT "${CMAKE_CURRENT_LIST_DIR}"
    GENERATED_FILES ${META_GENERATED_FILES}
    SOURCE_FILE "${CMAKE_CURRENT_BINARY_DIR}/.Assembly/Assembly_${MODUL_NAME}.dsc"
    MODULE_HEADER "${MODUL_NAME}AssemblyModul.h"
    MODULE_SOURCE_FILE ${META_MODULE_SOURCE}
    HEADER_FILES ${MODUL_HEADER_SRCS}
    PARSER_EXECUTABLE ${MetaParserExecutable}
)

get_property(OUTPUTDIR TARGET ${EXECUTABLE_NAME} PROPERTY RUNTIME_OUTPUT_DIRECTORY)
set(ResourceCompilerExecutable "${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE:${EXECUTABLE_NAME}>" CACHE STRING "" FORCE)

if( WIN32 )
    if( MSVC )
      install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${BUILD_TYPE}/${EXECUTABLE_NAME}.pdb DESTINATION ${BIN_DIR} CONFIGURATIONS Debug)
    endif( MSVC )
endif(WIN32)

# we don't want to copy if we're building in the source dir
if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)

    # list of files for which we add a copy rule
    file(GLOB_RECURSE MODUL_RESOURCE_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.mustache)

    foreach(item IN LISTS MODUL_RESOURCE_SRCS)
        message(${LibExSTATUS} "Add Resource: ${item}")
        add_custom_command(TARGET ${EXECUTABLE_NAME} PRE_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy_if_different
                               "${CMAKE_CURRENT_SOURCE_DIR}/${item}"
                               "$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/${item}"
                           DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${item}")
    endforeach()
endif()

set_target_properties(${EXECUTABLE_NAME}
    PROPERTIES
      FOLDER
        "Resource")

# copy resources on post build
add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    # mustache templates directory
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_LIST_DIR}/../../Resources"
        $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)