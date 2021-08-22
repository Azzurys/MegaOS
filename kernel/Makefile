KERNEL := kernel.elf


# Use our cross-compiling toolchain
CC = x86_64-elf-g++
LD = x86_64-elf-ld

CXXFLAGS = -Wall -Wextra -O2 -pipe
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
	-ffreestanding         \
	-fno-stack-protector   \
	-fno-pic -fpie         \
	-fno-exceptions        \
	-fno-rtti              \
	-mno-80387             \
	-mno-mmx               \
	-mno-3dnow             \
	-mno-sse               \
	-mno-sse2              \
	-mno-red-zone


# List our .cpp files
CXXFILES := $(shell find ./ -type f -name '*.cpp')

# Compile each source file
OBJ := $(CXXFILES:.cpp=.o)

.PHONY: all clean

all: $(KERNEL)

stivale2.h:
	wget https://github.com/stivale/stivale/raw/master/stivale2.h

$(KERNEL): $(OBJ)
	$(LD) $(OBJ) $(LDFLAGS) $(INTERNALLDFLAGS) -o $@

%.o: %.cpp stivale2.h
	$(CC) $(CXXFLAGS) $(INTERNALCXXFLAGS) -c $< -o $@

clean:
	rm -rf $(KERNEL) $(OBJ)
