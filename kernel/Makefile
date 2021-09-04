KERNEL := kernel.elf


# Use our cross-compiling toolchain
CC = x86_64-elf-g++
AS = nasm
LD = x86_64-elf-ld

CXXFLAGS = -Wall -Wextra -O2 -pipe
ASMFLAGS = -f elf64 -F dwarf -g
LDFLAGS =

INTERNALLDFLAGS :=         \
	-Tlinker.ld            \
	-nostdlib              \
	-zmax-page-size=0x1000 \
	-static                \
	-pie                   \
	--no-dynamic-linker    \
	-ztext

INTERNALCXXFLAGS  :=       \
	-I.                    \
	-I../                  \
	-ggdb                  \
	-ffreestanding         \
	-fno-stack-protector   \
	-fno-pic               \
	-fpie                  \
	-fno-exceptions        \
	-fno-rtti              \
	-mno-80387             \
	-mno-mmx               \
	-mno-3dnow             \
	-mno-sse               \
	-mno-sse2              \
	-mno-red-zone


PRINTF_IMPLEMENTATION_DEFINES =          \
 	-DPRINTF_DISABLE_SUPPORT_FLOAT       \
 	-DPRINTF_DISABLE_SUPPORT_EXPONENTIAL

COMPILER_DEFINES = $(PRINTF_IMPLEMENTATION_DEFINES)


# List our .cpp and .asm files
CXXFILES := $(shell find ./ -type f -name '*.cpp')
ASMFILES := $(shell find ./ -type f -name '*.asm')

# Compile each source file
OBJ := $(CXXFILES:.cpp=.o) $(ASMFILES:.asm=.o)

.PHONY: all clean

all: $(KERNEL)

stivale2.h:
	wget https://github.com/stivale/stivale/raw/master/stivale2.h

$(KERNEL): $(OBJ)
	$(LD) $(OBJ) $(LDFLAGS) $(INTERNALLDFLAGS) -o $@

%.o: %.cpp stivale2.hpp
	$(CC) $(CXXFLAGS) $(INTERNALCXXFLAGS) $(COMPILER_DEFINES) -c $< -o $@

%.o: %.asm
	$(AS) $(ASMFLAGS) $< -o $@

clean:
	rm -rf $(KERNEL) $(OBJ)
