# Files
FILE(
    GLOB SRCList
    LIST_DIRECTORIES false
    "*.cpp"
)

FILE(
    GLOB HeaderList
    LIST_DIRECTORIES false
    "*.h"
)
SET(CML "CMakeLists.txt")

# Executable target
add_executable(${ExeName} ${SRCList} ${HeaderList} ${CML})
IF (WIN32)
    source_group("/" FILES  ${SRCList} ${HeaderList})
    source_group("/CMakeLists/" FILES  ${CML})
ENDIF()

# (Windows) Compile options
IF (WIN32)
    INCLUDE(${PROJECT_SOURCE_DIR}/cmake_config/windows.cmake)
ENDIF()
