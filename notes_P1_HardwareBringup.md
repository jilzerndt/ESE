# Lab P1: Hardware Bring-up (Inbetriebnahme)

## Technical Terms & Definitions

- **Cross-Toolchain / Cross-Compiler**: A compiler that runs on one system (host) but generates executables for a different system (target). Required when the development machine has a different architecture than the embedded platform.

- **Host System**: The development computer where code is written and compiled (e.g., Windows x86-64, Linux x86-64).

- **Target System**: The embedded platform where the compiled code will execute (e.g., Ultra96 with ARM Cortex-A53, AArch64 architecture).

- **Ultra96 Board**: Development board featuring a Xilinx Zynq UltraScale+ MPSoC (Multi-Processor System-on-Chip) combining FPGA fabric with multiple ARM processors.

- **SoC (System on Chip)**: Integrated circuit that combines multiple components (processors, memory, peripherals, FPGA fabric) on a single chip.

- **FPGA (Field-Programmable Gate Array)**: Reconfigurable hardware that can be programmed to implement custom digital logic circuits.

- **ARM Cortex-A53**: 64-bit ARM processor core used in the Ultra96 board, part of the ARMv8-A architecture (AArch64).

- **Sysroot**: A directory containing the target system's libraries, headers, and other files needed for cross-compilation. Allows the cross-compiler to link against the correct target libraries.

- **AArch64**: 64-bit ARM architecture (ARM Architecture 64-bit), used in modern ARM processors like Cortex-A53.

- **GNU/Linux Toolchain**: Collection of development tools including compiler (GCC), linker, assembler, and libraries for building Linux applications.

- **Makefile**: A file containing rules and instructions for the `make` build automation tool, defining how to compile and link source code.

- **Build Target**: A named recipe in a makefile that specifies what to build and how to build it.

- **USB Gadget Ethernet**: USB protocol that creates a network interface over a USB connection, allowing the embedded board to appear as a network device to the host computer.

- **SSH (Secure Shell)**: Encrypted network protocol for secure remote access to computer systems over a network.

- **SCP (Secure Copy Protocol)**: Protocol for securely transferring files between computers using SSH authentication and encryption.

- **Embedded Linux**: A Linux operating system optimized for embedded systems with constrained resources.

- **Boot Process**: The sequence of operations that occur when an embedded system powers on, including hardware initialization, bootloader execution, and OS loading.

- **RNDIS (Remote Network Driver Interface Specification)**: Microsoft protocol for USB network devices, used for USB Gadget Ethernet on some systems.

- **ELF (Executable and Linkable Format)**: Standard file format for executables, object code, and shared libraries on Linux systems.

- **PS POR (Processing System Power-On Reset)**: Hardware reset signal that reinitializes the ARM processor to its initial state, simulating a fresh power-on.

- **FPGA Configuration**: The process of loading a bitstream into the FPGA to define its hardware functionality.

- **PATH Environment Variable**: System variable containing directories where the operating system looks for executable programs.

- **Environment Variable**: A dynamic named value that affects the behavior of processes on a computer (e.g., SYS_ROOT, PATH).

## Core Concepts

### Cross-Compilation Workflow
The development workflow for embedded systems involves:
1. **Writing code** on the host system (Windows/Linux PC)
2. **Cross-compiling** using a toolchain that targets the embedded platform architecture
3. **Transferring** the compiled executable to the target system
4. **Executing** and testing on the actual hardware

This is necessary because:
- The embedded platform (ARM) has a different instruction set than the development machine (x86-64)
- The embedded platform may lack resources (storage, memory, power) for native compilation
- Native compilation on embedded hardware would be extremely slow

### Ultra96 Hardware Architecture
The Ultra96 board contains:
- **Xilinx Zynq UltraScale+ MPSoC**: Combines programmable logic (FPGA) with processing system (ARM cores)
- **Processing System (PS)**: Contains ARM Cortex-A53 processors running embedded Linux
- **Programmable Logic (PL)**: FPGA fabric for custom hardware acceleration
- **Dual nature**: Software runs on ARM processors while custom hardware can be implemented in FPGA

Key components:
- **4 USER LEDs**: Controllable from ARM processor software via GPIO
- **Status LEDs (DONE, 3V3, INIT)**: Indicate FPGA configuration status
- **Power Button (SW4)**: Manual start button (board doesn't auto-start on power connection)
- **User Button (SW1)**: Programmable button for user input
- **USB 3 Device Port**: Creates Gadget Ethernet connection to host computer
- **SD Card**: Contains bootloader, FPGA configuration, and Linux filesystem

### Toolchain Components
A complete cross-development environment requires:

1. **Text Editor/IDE**: Visual Studio Code with C/C++ extensions
2. **Cross-Compiler**: `aarch64-none-linux-gnu-gcc` (Windows) or `aarch64-linux-gnu-gcc` (Linux)
   - Generates ARM AArch64 machine code from C/C++ source
   - Version 12.3.Rel1 based on GCC 12.3
3. **Build System**: GNU Make for automating compilation
4. **Sysroot Libraries**: Platform-specific libraries and headers from Ultra96 Linux (~4.6 GB)
5. **Transfer Tools**: SCP for file transfer, SSH for remote shell access

### Boot Process Sequence
When the Ultra96 powers on:
1. **Power Connection**: Connect 12V power supply, Power LED illuminates
2. **Power Button Press**: Press SW4 to start boot sequence
3. **FPGA Configuration** (~5 seconds):
   - INIT LED (red): FPGA initializing
   - DONE LED (blue): FPGA configured successfully
4. **Linux Boot** (~25 seconds): ARM processors load and start embedded Linux from SD card
5. **USB Gadget Ethernet Ready**: Network interface available at IP 10.0.0.1

### Makefile Structure and Build Targets
Build targets define recipes for different operations:

```makefile
target_name: dependencies
	commands to execute
```

The P1 makefile defines:
- **all**: Default target, builds main.elf (the executable)
- **clean**: Removes compiled object files and executable
- **test**: Verifies cross-compiler installation by running `gcc -v`

The makefile automatically:
- Detects host OS (Windows vs Linux) and selects appropriate cross-compiler
- Compiles all .c files in src/ directory
- Creates object files in Release/ directory
- Links object files into final executable

### Compiler Flags and Options
Important flags used in cross-compilation:

- **--sysroot=$(SYS_ROOT)/cortexa72-cortexa53-xilinx-linux**: Points to target system libraries
- **-mcpu=cortex-a53**: Optimizes code for Cortex-A53 processor architecture
- **-std=gnu99**: Uses C99 standard with GNU extensions
- **-O0**: No optimization (easier debugging)
- **-Wall -Wextra**: Enable all standard warnings
- **-lm**: Link against math library

### USB Gadget Ethernet Connection
The Ultra96 uses USB Device mode to create a network connection:
- **No separate network cable needed**: Uses USB cable for both power signaling and networking
- **Fixed IP address**: Board always available at 10.0.0.1
- **Automatic RNDIS driver**: Windows/Linux recognize as "Remote NDIS Compatible Device"
- **Enables SSH and SCP**: Full network connectivity over USB

### Development Workflow Cycle
The typical embedded development cycle:

1. **Edit**: Modify source code in `src/main.c` using VS Code
2. **Build**: Run `make` to cross-compile for ARM target
3. **Transfer**: Use `scp -O main.elf ese@10.0.0.1:/home/ese/` to copy executable
4. **Connect**: Use `ssh ese@10.0.0.1` to access board's shell
5. **Execute**: Run `chmod +x main.elf` (if needed) and `./main.elf`
6. **Test**: Verify functionality and debug
7. **Iterate**: Return to step 1 for fixes or improvements

## Important Diagrams & Graphics

### Ultra96 Board Component Layout
**Description**: Top view photograph of Ultra96 board with labeled components:
- **Left side**: User Button (SW1, magenta), Status LEDs (DONE/3V3/INIT in green box, bottom left corner)
- **Center**: USB 3 Device port (Gadget Ethernet, cyan box in center-bottom), large SoC chip on underside
- **Right side**: Power Button SW4 (magenta, center-right), 4 USER LEDs (green box, bottom-right), PS POR button (magenta, right side), Power LED 12 Vin (green, top-right corner)
- **Layout**: Compact single-board design with all essential components accessible from top surface

**Key Takeaway**: All interaction points (buttons, LEDs, USB) are clearly marked and accessible for hardware bring-up.

### FPGA Boot Status LED Sequence
**Description**: Two-stage LED indicator showing FPGA configuration progress:

**Stage 1 - Initializing**:
- DONE: OFF (---)
- 3V3: GREEN (on)
- INIT: RED (on)
- Meaning: FPGA currently being configured with bitstream

**Stage 2 - Configured**:
- DONE: BLUE (on)
- 3V3: GREEN (on)
- INIT: OFF (---)
- Meaning: FPGA successfully configured and ready

**Key Takeaway**: DONE LED turning blue signals successful FPGA configuration; wait additional 25 seconds for Linux boot completion.

### USB 3 Device Connection
**Description**: Close-up photograph showing USB Micro-B cable connected to USB 3 Device port:
- **Location**: Center-bottom of board between SD card slot and USB Type-A ports
- **Cable**: USB Micro-B connector, labeled in photo
- **Purpose**: Creates Gadget Ethernet network interface to host computer

**Key Takeaway**: This single USB connection provides network access (IP 10.0.0.1) for SSH and SCP without requiring separate Ethernet cable.

### Cross-Toolchain Architecture Diagram
**Conceptual layout**:
```
Host System                           Target System
(x86-64 PC)                          (Ultra96 ARM)
┌─────────────────┐                  ┌─────────────┐
│  Source Code    │                  │             │
│   (.c files)    │                  │   ARM CPU   │
└────────┬────────┘                  │ (Cortex-A53)│
         │                           └──────┬──────┘
         ▼                                  │
┌─────────────────┐                        │
│ Cross-Compiler  │                        │
│ (aarch64-gcc)   │───────────────────────┐│
└────────┬────────┘                       ││
         │                                 ││
         ▼                                 ▼▼
┌─────────────────┐    Transfer      ┌─────────────┐
│  ARM Executable │──────(SCP)──────▶│ Executable  │
│   (main.elf)    │                  │  Runs Here  │
└─────────────────┘                  └─────────────┘
         ▲
         │
    ┌────┴────┐
    │ Sysroot │ (Target libraries)
    │ Libraries│
    └─────────┘
```

**Key Takeaway**: Cross-compiler runs on host but generates code for target architecture, requiring target libraries (sysroot) for proper linking.

### Development Toolchain Installation Flow
**Conceptual sequence diagram**:
```
1. Install VS Code
        ↓
2. Install C/C++ Extension
        ↓
3. Download ARM Cross-Toolchain
   (aarch64-none-linux-gnu)
        ↓
4. Install Toolchain
   ☑ Add to PATH variable
        ↓
5. Install GNU Make
   Add to PATH variable
        ↓
6. Download Ultra96 Sysroots
   (cross-compile.zip, 4.6 GB)
        ↓
7. Set SYS_ROOT Environment Variable
        ↓
8. Restart VS Code
        ↓
9. Test: aarch64-none-linux-gnu-gcc -v
```

**Key Takeaway**: Environment variables (PATH, SYS_ROOT) are critical for toolchain to function; VS Code restart required to pick up changes.

### Makefile Build Target Dependency Graph
**Conceptual diagram**:
```
        [all] (default target)
           ↓
      [main.elf]
           ↓
    ┌──────┴──────┐
    ↓             ↓
[main.o]      [other .o files]
    ↓             ↓
[main.c]      [other .c files]
    ↓             ↓
[headers: *.h files]

[clean] ──→ Removes .o files and .elf
[test] ──→ Runs compiler with -v flag
```

**Key Takeaway**: Make automatically tracks dependencies; changing a .c or .h file triggers recompilation of affected objects and relinking.

### SSH/SCP Communication Flow
**Network diagram**:
```
Host PC                      Ultra96 Board
───────                      ─────────────
         USB Gadget Ethernet
         (Network over USB)
┌────────┐                  ┌───────────┐
│ No IP  │◄────USB Cable───►│ 10.0.0.1  │
│assigned│                  │           │
│(Link-  │                  │ SSH Server│
│ local) │                  │   Port 22 │
└───┬────┘                  └─────┬─────┘
    │                             │
    │  ssh ese@10.0.0.1          │
    │  (Remote shell access)      │
    ├────────────────────────────►│
    │                             │
    │  scp -O main.elf ese@...   │
    │  (File transfer)            │
    ├────────────────────────────►│
    │                             │
```

**Key Takeaway**: USB Gadget Ethernet creates automatic point-to-point network; host doesn't need IP configuration, only target has fixed IP 10.0.0.1.

## Formulas & Procedures

### Cross-Compilation Command Structure
**General cross-compiler invocation**:
```bash
aarch64-none-linux-gnu-gcc [OPTIONS] -o output_file source_files.c
```

**Key options**:
- `--sysroot=<path>`: Specify target system root directory for libraries
- `-mcpu=<cpu-type>`: Optimize for specific ARM processor
- `-std=<standard>`: Specify C language standard
- `-O<level>`: Optimization level (0=none, 1-3=increasing optimization)
- `-Wall -Wextra`: Enable comprehensive warnings
- `-c`: Compile only, don't link (generate .o file)
- `-o`: Specify output file name

**Example from makefile**:
```bash
aarch64-none-linux-gnu-gcc -std=gnu99 --sysroot=$(SYS_ROOT)/cortexa72-cortexa53-xilinx-linux \
  -mcpu=cortex-a53 -O0 -Wall -Wextra -lm -o main.elf src/main.c
```

### Makefile Target Execution Flow
**Procedure for building a target**:
1. Make reads the makefile
2. User specifies target (or 'all' by default): `make <target>`
3. Make checks target dependencies
4. If dependencies are files, checks if they're newer than target
5. If dependencies are targets, recursively builds those first
6. Executes commands for the target (indented with TAB)
7. Target considered up-to-date

**Dependency resolution example**:
```makefile
main.elf: main.o utils.o     # main.elf depends on object files
	gcc main.o utils.o -o main.elf

main.o: main.c headers.h      # main.o depends on source and header
	gcc -c main.c -o main.o
```

### File Transfer Procedure (SCP)
**Basic SCP syntax**:
```bash
scp [OPTIONS] source_file user@host:destination_path
```

**Procedure for transferring executable to Ultra96**:
```bash
scp -O main.elf ese@10.0.0.1:/home/ese/
```

**Flags**:
- `-O`: Use legacy SCP protocol (more compatible with embedded systems)
- `ese@10.0.0.1`: Username 'ese' at IP address 10.0.0.1
- `:/home/ese/`: Destination directory on target system

### Remote Execution Procedure (SSH)
**SSH connection**:
```bash
ssh ese@10.0.0.1
# Password: ese
```

**Making file executable and running it**:
```bash
chmod +x main.elf    # Add execute permission
./main.elf           # Run executable
```

### Environment Variable Configuration
**Windows procedure**:
1. Open "System Properties" → "Advanced" → "Environment Variables"
2. Add new variable `SYS_ROOT` with value `C:\arm\cross-compile\sysroots`
3. Edit `PATH` variable to include:
   - `C:\Program Files (x86)\Arm GNU Toolchain\bin`
   - `C:\Program Files (x86)\GnuWin32\bin`
4. Click OK to save
5. Restart applications (especially VS Code) to load new variables

**Linux procedure**:
```bash
export SYS_ROOT=/path/to/sysroots
export PATH=$PATH:/path/to/toolchain/bin
```

Add to `~/.bashrc` or `~/.profile` for persistence.

### Verification Procedures
**Test cross-compiler installation**:
```bash
aarch64-none-linux-gnu-gcc -v
```
Expected output: Version information showing GCC 12.3, ARM GNU Toolchain 12.3.Rel1

**Test make installation**:
```bash
make test
```
Executes the 'test' target which runs `gcc -v`

**Test board connectivity**:
```bash
ping 10.0.0.1          # Should respond if board is booted and connected
ssh ese@10.0.0.1       # Should prompt for password
```

## Comparisons & Contrasts

### Native vs Cross-Compilation

| Aspect | Native Compilation | Cross-Compilation |
|--------|-------------------|-------------------|
| **Compiler Location** | Runs on target system | Runs on host system |
| **Target Architecture** | Same as host | Different from host |
| **Use Case** | Desktop/server development | Embedded development |
| **Speed** | Depends on target performance | Uses full host PC performance |
| **Resources Required** | Need complete toolchain on target | Only need runtime on target |
| **Typical Example** | Compiling Linux app on Linux PC | Compiling ARM app on x86 PC |
| **Advantages** | Simpler setup, no architecture mismatch | Faster builds, less target resources |
| **Disadvantages** | Requires powerful target system | More complex setup, sysroot needed |

### Host System vs Target System

| Property | Host System | Target System |
|----------|-------------|---------------|
| **Architecture** | x86-64 (Intel/AMD) | ARM AArch64 (Cortex-A53) |
| **Operating System** | Windows 10/11, Linux desktop | Embedded Linux (Petalinux/Yocto) |
| **Purpose** | Development, compilation | Execution, deployment |
| **Resources** | High (GB RAM, fast CPU) | Limited (MB-GB RAM, embedded CPU) |
| **Storage** | HDD/SSD (TB) | SD card (GB) |
| **Tools Available** | Full IDE, debuggers, compilers | Runtime libraries, basic shell |
| **Typical Tasks** | Edit, build, test | Run, collect data, control hardware |

### Windows vs Linux Toolchain Differences

| Component | Windows | Linux |
|-----------|---------|-------|
| **Cross-Compiler Name** | `aarch64-none-linux-gnu-gcc` | `aarch64-linux-gnu-gcc` |
| **Installation Method** | Download .exe installer from ARM website | Package manager: `apt-get install` |
| **PATH Separator** | Semicolon (;) | Colon (:) |
| **Path Format** | Backslash (C:\path\to\file) | Forward slash (/path/to/file) |
| **Make Command** | Requires separate GnuWin32 installation | Already included in system |
| **File Operations** | `del`, `mkdir` with backslashes | `rm`, `mkdir` with forward slashes |
| **Shell** | PowerShell, Command Prompt | Bash, sh |

### Build Targets Comparison

| Target | Purpose | When to Use | What It Does |
|--------|---------|-------------|--------------|
| **all** | Build complete project | Default, general build | Compiles all sources, links executable |
| **clean** | Remove build artifacts | Before fresh build, cleanup | Deletes .o files and .elf executable |
| **test** | Verify toolchain | After installation | Runs `gcc -v` to check compiler |
| **main.elf** | Build executable only | Explicit executable build | Links object files into ELF binary |
| **%.o** | Build single object file | Implicit (called by dependencies) | Compiles one .c file to .o |

### SCP vs SSH

| Feature | SCP (Secure Copy) | SSH (Secure Shell) |
|---------|-------------------|-------------------|
| **Primary Purpose** | File transfer | Remote shell access |
| **Protocol Base** | SSH (uses SSH connection) | SSH |
| **Interactive** | No (command completes) | Yes (persistent session) |
| **Typical Usage** | Copy files to/from target | Execute commands on target |
| **Command Example** | `scp main.elf ese@10.0.0.1:~/` | `ssh ese@10.0.0.1` |
| **Direction** | Bidirectional (push/pull) | Host → Target commands |
| **Session** | One-time operation | Remains open until exit |

### FPGA vs Processor (on Zynq)

| Aspect | FPGA (Programmable Logic - PL) | Processor (Processing System - PS) |
|--------|--------------------------------|-----------------------------------|
| **Nature** | Reconfigurable hardware | Fixed hardware (ARM CPU) |
| **Programming** | Hardware Description Languages (VHDL, Verilog) | Software (C, C++, assembly) |
| **Execution** | Parallel, custom circuits | Sequential instructions |
| **Speed** | Can be extremely fast for specific tasks | General-purpose, GHz clock |
| **Flexibility** | Reconfigurable, different circuits | Fixed instruction set |
| **Power** | Higher for equivalent tasks | Lower for sequential processing |
| **Use Case** | Hardware acceleration, custom interfaces | OS, control logic, general computation |
| **Configuration** | Loaded at boot from bitstream | Runs code from memory |
| **In P1 Lab** | Not used (pre-configured) | Runs embedded Linux and user code |

### Development Workflow: Embedded vs Desktop

| Phase | Embedded System (Ultra96) | Desktop Application |
|-------|--------------------------|---------------------|
| **Edit** | On host PC (VS Code) | On same machine |
| **Compile** | Cross-compile on host | Native compile on same machine |
| **Link** | Link against target sysroot | Link against local libraries |
| **Transfer** | SCP to target | No transfer needed |
| **Execute** | SSH to target and run | Run directly |
| **Debug** | Remote debugging or printf | Local debugger (GDB, IDE) |
| **Iteration** | Multiple steps (build→transfer→run) | Single step (build→run) |

## Examples & Applications

### Example 1: "Hello World" Scrolling Animation
The provided `main.c` implements a scrolling text animation:

```c
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    char message[] = "Hello, World!                           ";
    while (1) {
        printf("\r");              // Return to start of line
        printf("%s", message);     // Print the message

        // Shift message by one character, wrapping around
        char first_char = message[0];
        memmove(message, message + 1, strlen(message) - 1);
        message[strlen(message) - 1] = first_char;

        fflush(stdout);
        usleep(100000);           // 100 ms delay
    }
    return 0;
}
```

**Key concepts demonstrated**:
- **Standard I/O**: Uses `printf()` for output on embedded Linux
- **String manipulation**: `memmove()` for efficient memory copying
- **Timing control**: `usleep()` for precise delays (100ms = 100,000μs)
- **Terminal control**: `\r` (carriage return) for in-place updates
- **Buffer flushing**: `fflush(stdout)` ensures immediate output
- **Infinite loop**: Typical pattern for embedded systems (no exit condition)

**Real-world relevance**: Demonstrates basic embedded programming patterns used in status displays, user feedback, and animation on resource-constrained systems.

### Example 2: Makefile Automatic Source Detection
The makefile automatically finds and compiles all C files:

```makefile
SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
```

**How it works**:
1. `$(wildcard $(SRC_DIR)/*.c)` finds all .c files in src/ directory
2. `$(patsubst %.c,$(OBJ_DIR)/%.o,...)` converts `src/main.c` → `Release/src/main.o`
3. Make automatically compiles each .c file to .o using pattern rule
4. All .o files linked together into final executable

**Benefit**: Add new source files to src/ and they're automatically included in build without modifying makefile.

### Example 3: Platform-Specific Makefile Configuration
The makefile adapts to Windows vs Linux automatically:

```makefile
ifdef OS                      # OS variable set on Windows
	RM = del /Q
	FixPath = $(subst /,\,$1)
	CC = aarch64-none-linux-gnu-gcc
else
	ifeq ($(shell uname), Linux)
		RM = rm -f
		FixPath = $1
		CC = aarch64-linux-gnu-gcc
	endif
endif
```

**Platform adaptations**:
- **Windows**: Uses `del`, backslash paths, ARM's cross-compiler naming
- **Linux**: Uses `rm`, forward slash paths, different compiler naming
- **Path conversion**: `FixPath` function converts forward to backslash on Windows

**Real-world relevance**: Enables same makefile to work across different development environments, critical for team collaboration.

### Example 4: Sysroot Linking
Cross-compiler must link against target system libraries:

```makefile
CFLAGS += --sysroot=$(SYS_ROOT)/cortexa72-cortexa53-xilinx-linux -lm
```

**Why this is necessary**:
- Target uses different C library (glibc for AArch64) than host
- System calls interface differs between architectures
- Library ABIs (Application Binary Interfaces) are architecture-specific
- Math library (`-lm`) compiled for ARM, not x86

**Without sysroot**: Code would compile but fail to link or crash at runtime due to ABI mismatches.

### Example 5: Hardware Boot Sequence
Actual boot timing for Ultra96:

```
Time 0s:     Press Power Button (SW4)
Time 0-5s:   FPGA Configuration
             - INIT LED red (configuring)
             - DONE LED turns blue when complete
Time 5-30s:  Linux Boot Process
             - Kernel loads from SD card
             - Filesystem mounts
             - Services start (including SSH server)
Time 30s:    System Ready
             - USB Gadget Ethernet active at 10.0.0.1
             - SSH accessible
```

**Practical implication**: Always wait full 30 seconds after pressing power button before attempting SSH connection.

### Example 6: File Transfer Workflow
Complete workflow for updating code on target:

```bash
# On host (Windows PowerShell or Command Prompt):
cd P1                                    # Navigate to project directory
make clean                               # Remove old build artifacts
make                                     # Build for ARM target
scp -O main.elf ese@10.0.0.1:/home/ese/ # Transfer to Ultra96
ssh ese@10.0.0.1                        # Connect to board

# On target (Ultra96 SSH session):
ese@10.0.0.1's password: ese            # Enter password
chmod +x main.elf                        # Make executable (first time only)
./main.elf                              # Run program
# Press Ctrl+C to stop if in infinite loop
exit                                     # Disconnect from board
```

**Time savings**: After initial setup, update cycle takes <10 seconds (build→transfer→run).

### Example 7: Verifying Cross-Compiler Configuration
Testing that toolchain is correctly installed:

```bash
# Test 1: Compiler accessible
PS C:\> aarch64-none-linux-gnu-gcc -v
# Should output: gcc version 12.3.1... (Arm GNU Toolchain 12.3.Rel1)

# Test 2: Make accessible
PS C:\> make test
# Should run and output compiler version info

# Test 3: SYS_ROOT set correctly
PS C:\> echo $env:SYS_ROOT
# Should output: C:\arm\cross-compile\sysroots (or your path)

# Test 4: Build succeeds
PS C:\> cd P1
PS C:\Users\...\P1> make
# Should compile without errors and create main.elf
```

**Troubleshooting**: If any test fails, revisit installation steps for that component.

### Example 8: USB Gadget Ethernet Setup
Automatic network configuration via USB:

**Host side** (Windows):
- Plug in USB Micro-B cable to Ultra96 USB 3 Device port
- Windows detects "Remote NDIS Compatible Device"
- Network adapter appears with link-local address (169.254.x.x)
- No IP configuration needed on host side

**Target side** (Ultra96):
- USB gadget driver creates `usb0` network interface
- Interface configured with static IP 10.0.0.1/24
- Routing configured for point-to-point link
- SSH server listens on all interfaces (including usb0)

**Result**: Direct network communication over USB cable without requiring separate Ethernet hardware or network infrastructure.

## Connections to Other Topics

### Forward Connections (Topics Covered Later)

**→ Lab P2 (Energy Analysis)**:
- Cross-compilation workflow established here will be used for building energy measurement code
- Ultra96 hardware platform continues to be used for power profiling
- Makefile techniques extend to more complex multi-file projects

**→ Lab P3 (AES Multi-Processor)**:
- SSH/SCP workflow for deploying parallel processing applications
- Understanding of ARM Cortex-A53 architecture essential for multi-core programming
- Build system complexity increases with multi-threaded code

**→ Lab P4 (AES in FPGA)**:
- First lab will use both PS (ARM) and PL (FPGA) parts of Zynq
- Hardware/software co-design builds on understanding of SoC architecture
- FPGA configuration process (DONE/INIT LEDs) becomes relevant

**→ Lecture 08 (Energy)**:
- Cross-compilation with different optimization flags (-O0, -O1, -O2, -O3) affects energy consumption
- Embedded Linux platform enables energy measurement tooling

**→ Lecture 09 (Performance)**:
- Compiler optimization flags (-mcpu, -O flags) introduced here are critical for performance
- ARM Cortex-A53 architecture details become important for optimization

**→ Lecture 10 (FPGA Introduction)**:
- FPGA part of Zynq UltraScale+ (currently unused) will be programmed
- Understanding of PL vs PS distinction becomes essential

**→ Lecture 13 (RTOS Scheduling)**:
- Embedded Linux used here is a full OS; contrast with RTOS will be explored
- Understanding of ARM processor and boot process provides foundation

**→ Lecture 14 (Multi-Processor Systems)**:
- Zynq contains multiple ARM cores (Cortex-A53 quad-core)
- Cross-compilation for multi-core targets builds on single-core workflow

### Backward Connections (Prerequisites from Earlier Topics)

**← Lecture 01 (Embedded Systems)**:
- Ultra96 is an example of the cyber-physical embedded systems discussed
- Design space (energy-cost-performance) manifests in hardware choices
- Resource constraints motivate cross-compilation approach

**← Lecture 02 (Software Paradigms)**:
- Example code uses procedural programming paradigm (C with functions)
- Infinite loop pattern typical of time-driven or event-driven embedded software
- Could be rewritten in different paradigms (OOP, event-driven)

### Related Concepts from Broader CS Knowledge

**Operating Systems**:
- Embedded Linux is a specialized OS distribution
- Boot process involves bootloader, kernel loading, init system
- SSH server is a userspace daemon providing remote access service

**Computer Architecture**:
- ARM vs x86 instruction set architecture (ISA) differences
- RISC (ARM) vs CISC (x86) design philosophies
- 64-bit addressing (AArch64) vs 32-bit (ARMv7)

**Computer Networks**:
- TCP/IP networking over USB interface
- SSH protocol (port 22) for encrypted communication
- Static IP addressing for embedded systems

**Software Engineering**:
- Build automation with Make
- Version control (Git repository for lab files)
- Cross-platform development considerations

**Systems Programming**:
- C language for low-level system access
- Direct hardware control via GPIO (USER LEDs)
- System calls interface through C standard library

## Summary

### Key Takeaways

1. **Cross-compilation is essential** for embedded development when host and target architectures differ (x86-64 host → ARM target).

2. **Complete toolchain** requires: cross-compiler, build system (make), target libraries (sysroot), and transfer tools (SSH/SCP).

3. **Ultra96 is a heterogeneous SoC** combining ARM processors (software) and FPGA fabric (hardware), though P1 only uses ARM side.

4. **Development workflow** follows edit→build→transfer→execute cycle, introducing latency compared to native development.

5. **USB Gadget Ethernet** provides convenient point-to-point networking over USB cable, enabling SSH/SCP without separate network infrastructure.

6. **Boot process takes ~30 seconds**: FPGA configuration (5s) + Linux boot (25s) before system ready for SSH access.

7. **Makefile automation** handles cross-compilation complexity, platform differences, and automatic source file detection.

8. **Environment variables** (PATH, SYS_ROOT) are critical for toolchain operation and must be set system-wide.

9. **Sysroot libraries** must match target system exactly for correct linking and runtime behavior.

10. **Embedded Linux** provides familiar POSIX environment on ARM, allowing standard C programs with minor modifications.

### Essential Facts for Exam Discussion

- **Cross-toolchain target naming**: `aarch64-none-linux-gnu` = AArch64 architecture, bare-metal/none environment, Linux OS, GNU toolchain
- **Ultra96 SoC**: Xilinx Zynq UltraScale+ MPSoC with ARM Cortex-A53 + FPGA fabric
- **Fixed IP address**: Ultra96 always accessible at 10.0.0.1 via USB Gadget Ethernet
- **Login credentials**: Username `ese`, Password `ese`
- **Build targets**: `all` (build), `clean` (remove artifacts), `test` (verify toolchain)
- **Key compiler flags**: `--sysroot` (target libraries), `-mcpu=cortex-a53` (architecture optimization)
- **Boot indicators**: DONE LED blue = FPGA configured, wait 25s more for Linux
- **File permissions**: May need `chmod +x` before executing transferred files

## Possible Exam Questions

### Basic Recall / Definitions (7 questions)

1. What is a cross-compiler and why is it necessary for embedded development?

2. What does "SoC" stand for and what components does the Ultra96 SoC contain?

3. What is a sysroot and what purpose does it serve in cross-compilation?

4. What IP address is the Ultra96 board accessible at via USB Gadget Ethernet?

5. What are the three build targets defined in the P1 makefile and what does each do?

6. What do the DONE, 3V3, and INIT LEDs indicate during the boot process?

7. What does the compiler flag `--sysroot` do?

### Comprehension / Explanation (8 questions)

8. Explain the complete boot sequence of the Ultra96 from power-on to SSH accessibility, including approximate timings.

9. Why must the cross-compiler link against a sysroot containing target system libraries rather than using the host system's libraries?

10. Describe the complete development workflow cycle for updating code on the Ultra96, from editing source to executing on hardware.

11. How does USB Gadget Ethernet enable network communication between host and target without requiring a separate network cable?

12. Explain how the makefile automatically detects and compiles all source files in the `src/` directory.

13. Why does the makefile use different cross-compiler names (`aarch64-none-linux-gnu-gcc` vs `aarch64-linux-gnu-gcc`) on Windows vs Linux?

14. What is the purpose of the `chmod +x` command when first executing a transferred file on the Ultra96?

15. Explain the difference between the Processing System (PS) and Programmable Logic (PL) in the Zynq UltraScale+ architecture.

### Application (6 questions)

16. Given a new C source file `utils.c` added to the `src/` directory, what changes (if any) must be made to the makefile for it to be included in the build?

17. If the `make` command fails with "command not found," what are two possible causes and how would you diagnose which one?

18. You run `ssh ese@10.0.0.1` immediately after pressing the Ultra96 power button and get "connection refused." Is this an error? How long should you wait and why?

19. Design a modification to the "Hello World" program that blinks one of the USER LEDs instead of scrolling text. What additional knowledge would you need?

20. If you wanted to compile the code with maximum optimization instead of no optimization, what change would you make to the makefile?

21. You need to transfer multiple files (main.elf, config.txt, data.bin) to the Ultra96. Write the complete sequence of scp commands.

### Analysis / Comparison (7 questions)

22. Compare and contrast native compilation (compiling on the same system where code will run) with cross-compilation. When is each approach appropriate?

23. Analyze the advantages and disadvantages of using SSH/SCP for embedded development versus having a full development environment directly on the target system.

24. Compare the embedded Linux development environment on Ultra96 with a bare-metal (no OS) embedded development approach. What trade-offs exist?

25. The makefile uses `-O0` (no optimization). Contrast this with `-O3` (maximum optimization) in terms of: compilation time, code size, execution speed, debugging difficulty.

26. Compare the Ultra96's heterogeneous architecture (ARM + FPGA) with a traditional microcontroller (single processor, no FPGA). What capabilities does each approach enable or restrict?

27. Analyze why the sysroot files require 4.6 GB of storage. What types of files are included and why are they necessary?

28. Compare the USB Gadget Ethernet approach used by Ultra96 with traditional Ethernet-based embedded board communication. What are the pros and cons of each?

### Synthesis / Design Decisions (6 questions)

29. Design a strategy for minimizing the development cycle iteration time (edit → execute) when making frequent small changes to embedded code.

30. Propose a method for automatically transferring and executing code on the Ultra96 immediately after a successful build, requiring only a single command.

31. If you needed to develop for multiple different ARM-based platforms (e.g., Ultra96 and Raspberry Pi), how would you structure your development environment and makefiles?

32. Design a debugging strategy for embedded code that crashes immediately on execution. What tools and techniques would you use without a GUI debugger?

33. Propose modifications to the development workflow that would allow multiple developers to work on the same Ultra96 hardware simultaneously.

34. How would you modify the makefile to support building for both the target (ARM) and the host (x86-64) for testing purposes?

### Critical Thinking / Trade-offs (6 questions)

35. Why does the Ultra96 require a manual power button press rather than automatically starting when power is applied? What are the implications for deployment scenarios?

36. The lab uses an older GCC version (12.3 from 2023). Discuss potential risks and benefits of using a newer vs. older, more stable toolchain version.

37. Evaluate the security implications of using SSH with known credentials (ese/ese) for embedded system access. In what scenarios is this acceptable vs. problematic?

38. Analyze the trade-off between using a complete makefile (like in P1) versus a simpler build script. When does the added complexity become worthwhile?

39. The Ultra96 boots from an SD card containing both the OS and FPGA configuration. Discuss reliability, performance, and deployment implications compared to built-in flash storage.

40. Discuss why embedded Linux distributions are often custom-built (Yocto/Petalinux) rather than using standard desktop distributions. What are the trade-offs?

### Scenario-Based Questions (6 questions)

41. Scenario: You run `make` and receive error "cannot find -lm". The source code includes `<math.h>` and uses `sqrt()`. What is the likely cause and how would you fix it?

42. Scenario: After transferring main.elf to Ultra96, it runs correctly. You make a small change, rebuild, transfer again, but the board still runs the old version. What could have happened?

43. Scenario: The Ultra96 Power LED is on, but DONE LED stays off and INIT LED is red for several minutes. Diagnose the problem and propose solutions.

44. Scenario: Your host computer is running Mac OS. The lab instructions mention that Mac requires additional steps. What specific challenges exist and what solutions are available?

45. Scenario: You need to use a third-party library (e.g., OpenSSL) in your embedded code. Describe the complete process from obtaining the library to successfully linking it into your program.

46. Scenario: Multiple students share one Ultra96 board. Propose a system for managing concurrent development without interfering with each other's work.

### Connections & Integration (5 questions)

47. How does the procedural programming paradigm (Lecture 02) manifest in the P1 example code? How might the code look different if using an event-driven paradigm?

48. Relate the resource constraints discussed in Lecture 01 (Embedded Systems) to the design decisions in the Ultra96 platform and cross-compilation workflow.

49. Anticipating future labs: How might the cross-compilation workflow need to change when FPGA acceleration is added in later practicals?

50. Connect the concept of design space (performance-energy-cost) to the choice of using an ARM Cortex-A53 processor versus a smaller microcontroller or larger desktop processor.

51. How does the boot process of the Ultra96 (FPGA configuration → Linux boot) relate to the cyber-physical nature of embedded systems discussed in Lecture 01?

### Code Recognition & Debugging (5 questions)

52. In the provided main.c, explain what would happen if the `fflush(stdout)` call were removed. Why is it necessary?

53. Identify the potential issue: The makefile uses `$(wildcard $(SRC_DIR)/*.c)` to find sources. What would happen if a source file with extension `.C` (uppercase) were added?

54. The makefile includes `-mcpu=cortex-a53` but the board contains a "Zynq UltraScale+". Explain why this is correct and not a mismatch.

55. Analyze this compiler invocation: `aarch64-none-linux-gnu-gcc -O3 --sysroot=/wrong/path -o test test.c`. It compiles successfully but crashes on target. Why?

56. In the provided main.c, the message array includes many trailing spaces. Explain why this design choice was made for the scrolling animation.

### Advanced Understanding (4 questions)

57. The cross-compiler is named "aarch64-none-linux-gnu". Decode each component of this name and explain what each part signifies about the toolchain's capabilities and target.

58. Explain how Make's dependency tracking works. If you modify a header file included by multiple source files, which files get recompiled and why?

59. Discuss the role of the SD card in the Ultra96 architecture. What would be required to boot the system without an SD card, and why might this be desirable?

60. Analyze the implications of the Zynq being a "heterogeneous" system. How does this affect: development complexity, debugging difficulty, potential performance, and power efficiency?

### Meta-Questions About Lab Practice (3 questions)

61. What are the key learning objectives of P1, and how do they prepare you for subsequent labs in the course?

62. Reflect on the evaluation criteria: "Cross-Toolchain is functional and can build for ultra96" and "Hello world executable can be transferred and run." Why are these fundamental prerequisites for embedded development?

63. This lab establishes a development workflow that will be reused throughout the course. Identify three aspects of this workflow that will become more complex in later labs involving FPGA programming or multi-core processing.
