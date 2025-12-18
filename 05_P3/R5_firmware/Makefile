TARGET = ese@10.0.0.1
NAME = aes_rpu_rtos
OBJ_DIR = Release
SRC_DIR = src
DIRS = $(OBJ_DIR)/$(SRC_DIR)
LINKER = ./src/lscript.ld
CC = arm-none-eabi-gcc

L_I_DIR = $(SYS_ROOT)
IDIR = $(L_I_DIR)/../freertos10_xilinx_psu_cortexr5_0/bspinclude/include
LDIR = $(L_I_DIR)/../freertos10_xilinx_psu_cortexr5_0/bsplib/lib

LIBS = -Wl,--start-group,-lxil,-lfreertos,-lgcc,-lc,--end-group -Wl,--start-group,-lxil,-lmetal,-lopen_amp,-lgcc,-lc,--end-group -Wl,--start-group,-lxil,-lmetal,-lgcc,-lc,--end-group
CFLAGS = -W -Wall -Wextra -mfloat-abi=hard -mcpu=cortex-r5 -mfpu=vfpv3-d16 -mthumb

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

ifdef OS
    RM = del /Q
    FixPath = $(subst /,\,$1)
	MKDIR = mkdir $(subst /,\,$(1)) > nul 2>&1 || (exit 0)
else
    ifeq ($(shell uname), Linux)
        RM = rm -f
        FixPath = $1
		MKDIR = mkdir -p $1
    endif
endif


$(OBJ_DIR)/%.o: %.c $(wildcard $(SRC_DIR)/*.h)
	$(call MKDIR,$(call FixPath,$(DIRS)))
	$(CC) $(CFLAGS) -c $< -o $@ -I $(IDIR)

$(NAME).elf: $(OBJ_FILES) $(wildcard $(LDIR)/*.a)
	$(CC) $(CFLAGS) $^ -o $@ -L $(LDIR) $(LIBS) -Wl,-T -Wl,$(LINKER) 

clean:
	$(RM) $(call FixPath,$(OBJ_FILES))
	$(RM) $(call FixPath,$(NAME).elf)

install: $(NAME).elf
	pscp -scp -pw ese $(NAME).elf $(TARGET):/home/ese/
	ssh $(TARGET) "chmod +x /home/ese/$(NAME).elf"

