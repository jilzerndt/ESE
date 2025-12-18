TARGET = ese@10.0.0.1
NAME = apu
OBJ_DIR = Release
SRC_DIR = src
DIRS = $(OBJ_DIR)/$(SRC_DIR)

CFLAGS  = -std=gnu99
CFLAGS += --sysroot="$(SYS_ROOT)\cortexa72-cortexa53-xilinx-linux" -lm # Linking to library
CFLAGS += -mcpu=cortex-a53 -O0 #-mfpu=neon -Ofast -mfloat-abi=hard 					# Optimizations
CFLAGS += -Wall -Wextra #-fopt-info-vec-optimized -fopt-info-missed=tmp/msd.txt	# Compiler Messages

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

ifdef OS
	RM = del /Q
	FixPath = $(subst /,\,$1)
	MKDIR = mkdir $(subst /,\,$(1)) > nul 2>&1 || (exit 0)
	CC = aarch64-none-linux-gnu-gcc
else
	ifeq ($(shell uname), Linux)
		RM = rm -f
		FixPath = $1
		MKDIR = mkdir -p $1
		CC = aarch64-linux-gnu-gcc
	endif
endif


$(OBJ_DIR)/%.o: %.c $(wildcard $(SRC_DIR)/*.h)
	$(call MKDIR,$(DIRS))
	$(CC) $(CFLAGS) -c $(call FixPath,$<) -o $(call FixPath,$@)

$(NAME).elf: $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

all: $(NAME).elf

clean:
	$(RM) $(call FixPath,$(OBJ_FILES))
	$(RM) $(call FixPath,$(NAME).elf)

install: $(NAME).elf
	scp -O $(NAME).elf $(TARGET):/home/ese/
	ssh $(TARGET) "chmod +x /home/ese/$(NAME).elf"

test:
	$(CC) -v


#ssh-keygen -t rsa -b 4096 -C "ese_key"
#pscp -scp -pw ese $(NAME).elf $(TARGET):/home/ese/
#plink -pw ese $(TARGET) -t "chmod +x /home/ese/$(NAME).elf"
