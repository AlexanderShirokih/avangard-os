# Toolchain for x86_64 PC

SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   /home/alexander/opt/cross/bin/x86_64-elf-gcc)
SET(CMAKE_CXX_COMPILER /home/alexander/opt/cross/bin/x86_64-elf-g++)

# where is the target environment
SET(CMAKE_FIND_ROOT_PATH  /usr/lib64/)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
