
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

cmake_minimum_required(VERSION 3.5)

project(AvangardOS:Initializer C CXX ASM_NASM)

if(NOT DEFINED ARCH)
        set(ARCH "x86_64")
endif()

set(CMAKE_CXX_LINK_FLAGS "-ffreestanding -z max-page-size=0x1000 -T${CMAKE_CURRENT_SOURCE_DIR}/link.ld -ffreestanding -nostdlib")
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_CXX_COMPILER> <CMAKE_CXX_LINK_FLAGS> -o <TARGET> <OBJECTS> <LINK_LIBRARIES> <LINK_FLAGS>")
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> -I${CMAKE_CURRENT_SOURCE_DIR}/include/arch/${ARCH}/asm/ -f ${CMAKE_ASM_NASM_OBJECT_FORMAT} -o <OBJECT> <SOURCE>")
add_compile_options(-g -m64 -ffreestanding -nostdlib -mno-red-zone -fno-rtti -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-3dnow -nodefaultlibs -Wall -Wextra -Wtype-limits -Wmaybe-uninitialized -fno-exceptions)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)

file(GLOB CXX_SOURCES "*/*.cpp")
file(GLOB CXX_SUB_SOURCES "*/*/*.cpp")
file(GLOB ASM_ARCH_SRCS "arch/${ARCH}/asm/*.asm")

add_executable(initializer ${CXX_SOURCES} ${CXX_SUB_SOURCES} ${CXX_ARCH_SRCS} ${ASM_ARCH_SRCS})

#install(TARGETS initializer DESTINATION bin)

set(ISO_FILE ${CMAKE_CURRENT_BINARY_DIR}/initializer.iso)

add_custom_command(OUTPUT ${ISO_FILE}
        DEPENDS initializer
        COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/image ${CMAKE_CURRENT_BINARY_DIR}/image/
        COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/initializer ${CMAKE_CURRENT_BINARY_DIR}/image/iso/boot/initializer.bin
        COMMAND grub-mkrescue -o ${ISO_FILE} ${CMAKE_CURRENT_BINARY_DIR}/image/iso
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(RunQEMU
        COMMAND qemu-system-${ARCH} -cdrom ${ISO_FILE}
        COMMENT Run OS image in QEMU
)

add_custom_target(DebugQEMU
        COMMAND qemu-system-${ARCH} -s -cdrom ${ISO_FILE} > qemu-debug.log 2>&1 &
        COMMENT Debug OS image in QEMU
)

add_custom_target(RunBOCHS
        COMMAND xterm -e bochs -f ${CMAKE_CURRENT_SOURCE_DIR}/bochs.config -q
        COMMENT Run OS image in BOCHS
)

add_custom_target(iso DEPENDS ${ISO_FILE})

add_dependencies(RunQEMU iso)
add_dependencies(DebugQEMU iso)
add_dependencies(RunBOCHS iso)