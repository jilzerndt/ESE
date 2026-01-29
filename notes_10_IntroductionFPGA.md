# Lecture 10: Introduction to FPGA

## Technical Terms & Definitions

### FPGA Architecture

- **FPGA (Field Programmable Gate Array)**: Integrated circuit containing programmable logic cells and programmable connections between them that can be configured in the field to implement custom digital circuits
- **CLB (Configurable Logic Block)**: Basic building block of an FPGA containing LUTs, flip-flops, multiplexers, and routing resources
- **LUT (Lookup Table)**: Memory-based logic element that stores output values for every possible input combination; implements arbitrary logic functions by acting as a truth table
- **6-input LUT**: Can implement any logic function with up to 6 inputs, requires 2^6 = 64 bits of configuration memory
- **Flip-Flop**: Sequential logic storage element that stores one bit and is triggered by a clock signal
- **Routing**: Configurable connections between CLBs implemented as multiplexers controlled by configuration memory
- **Configuration Memory**: SRAM or Flash cells that store the FPGA's configuration (LUT contents, routing connections, multiplexer select signals)
- **Bitstream**: Binary file containing configuration data that is loaded into the FPGA to define its functionality
- **Programmable Interconnect**: Network of wires and multiplexers that can be configured to connect CLBs in arbitrary patterns
- **PLL (Phase-Locked Loop)**: On-chip clock generation circuit that can multiply/divide input clock frequencies

### System Integration

- **SoC (System on Chip)**: Single chip integrating multiple components like processors, FPGA fabric, memory controllers, and peripherals
- **AXI (Advanced eXtensible Interface)**: ARM bus protocol for on-chip communication between processors and FPGA blocks
- **APB (ARM Peripheral Bus)**: Simpler bus protocol used to connect peripheral blocks with control/status registers
- **Memory-Mapped I/O**: Technique where hardware registers are accessed using memory addresses via the system bus
- **Control Register**: Memory-mapped register written by CPU to configure or start FPGA operations
- **Status Register**: Memory-mapped register read by CPU to check FPGA operation results or state
- **Hardcore Processor**: Fixed silicon processor integrated into the FPGA chip (e.g., ARM Cortex-A53, Cortex-R5)
- **Softcore Processor**: Processor implemented using FPGA logic fabric (e.g., MicroBlaze, NIOS II, RISC-V)

### FPGA vs Other Architectures

- **ASIC (Application-Specific Integrated Circuit)**: Custom chip designed for specific application; faster and more efficient than FPGA but not reconfigurable
- **Von Neumann Architecture**: Traditional computer architecture with CPU, memory, and stored-program concept
- **General Purpose Processor**: CPU optimized for wide variety of tasks with fixed instruction set
- **GPU (Graphics Processing Unit)**: Highly parallel processor optimized for graphics and data-parallel computations
- **Reconfigurability**: Ability to reprogram FPGA functionality in the field by loading new bitstream
- **Pipelining**: Technique where multiple operations are overlapped in execution, with new data entering every clock cycle
- **Parallelism**: Simultaneous execution of multiple operations using independent hardware resources

### AES-Specific Terms

- **AES (Advanced Encryption Standard)**: Symmetric block cipher encrypting 128-bit blocks using 128/192/256-bit keys
- **Symmetric Encryption**: Same key used for both encryption and decryption
- **Block Cipher**: Encrypts fixed-size blocks of data (16 bytes for AES)
- **Round**: One iteration of AES transformations (10/12/14 rounds depending on key size)
- **SubBytes**: AES transformation replacing each byte using S-box lookup table
- **S-box (Substitution Box)**: Lookup table implementing non-linear byte substitution in AES
- **ShiftRows**: AES transformation cyclically shifting rows of state matrix
- **MixColumns**: AES transformation performing matrix multiplication in Galois Field GF(256) on state columns
- **AddRoundKey**: AES transformation XORing state with round key
- **GF(256) (Galois Field)**: Finite field with 256 elements used for AES arithmetic operations
- **Round Key**: Key material derived from main encryption key for each AES round
- **CBC (Cipher Block Chaining)**: AES mode where each plaintext block is XORed with previous ciphertext before encryption

### Performance Metrics

- **Latency**: Time delay from input to output for single operation
- **Throughput**: Amount of data processed per unit time
- **Clock Frequency**: Rate at which FPGA logic is clocked (50-1000 MHz depending on design complexity)
- **Clock Cycle**: Time period of one clock pulse; inverse of clock frequency
- **Deterministic Execution**: Guaranteed consistent execution time regardless of data or system state
- **Data Throughput**: Rate of data processing, typically measured in MB/s or Gb/s
- **Resource Utilization**: Percentage of available FPGA resources (LUTs, flip-flops, RAM blocks) used by design

## Core Concepts

### 1. FPGA Fundamental Principle

**What makes FPGA different from CPU:**
- **No fixed instruction set**: FPGA implements functions directly in hardware, not by executing instructions
- **No ALU or control unit**: Instead has configurable logic cells that become the function
- **Spatial computation**: Operations distributed across chip area vs temporal (sequential) in CPU
- **Configuration vs programming**: FPGA configured with bitstream defining hardware structure, not software instructions

**Key architectural elements:**
- Programmable logic cells (LUTs) for implementing any logic function
- Programmable routing network connecting cells via multiplexers
- Configuration memory (SRAM/Flash) storing connection patterns and LUT contents
- Clock distribution network providing synchronized timing

### 2. Lookup Table (LUT) Implementation

**How LUTs work:**
- Input signals act as address to select output value from stored truth table
- N-input LUT requires 2^N configuration bits
- Can implement ANY logic function with N or fewer inputs
- Example: 6-input LUT needs 64 bits, can be one 6-input function OR two 5-input functions

**Advantages for certain operations:**
- Logic functions: Implemented directly as truth table → 1 clock cycle
- No instruction fetch/decode overhead
- Parallel evaluation of multiple independent functions

**Example: 7-segment decoder**
- 4 inputs (BCD), 8 outputs (segments)
- Each output implemented as separate 4-input LUT
- All 8 outputs evaluated simultaneously in 1 cycle
- In CPU: Would require multiple instructions per output

### 3. Sequential Logic with Flip-Flops

**Flip-flop functionality:**
- Stores one bit of state
- Updates on clock edge (typically rising)
- Enables sequential circuits: counters, shift registers, state machines
- Can be connected to LUT output or direct inputs

**Clock distribution:**
- Generated on-chip by configurable PLL
- Distributed via dedicated low-skew routing
- Multiple clock domains possible
- Clock speed limited by: logic complexity, routing delay, component limits

### 4. Routing and Connections

**Implementation:**
- Multiplexers select which signal routes to each destination
- Configuration memory controls multiplexer select signals
- Multiple routing levels: local (within CLB), intermediate, long-distance

**Key characteristic:**
- Pure data movement operations (like AES ShiftRows) require NO clock cycles
- Just routing, no logic gates needed
- This is major advantage over CPU where data movement requires MOV instructions

### 5. Arithmetic Operations

**Approaches:**
- **LUT-based**: Each bit of addition/multiplication in separate LUT, cascade for multi-bit
  - Slow but uses only general resources
- **Hard blocks**: Fixed silicon adders/multipliers (like in CPU)
  - Fast but consumes fixed resources
  - Example: DSP blocks with dedicated 18x18 multipliers

**Trade-off:**
- Simple operations (XOR, AND): Ideal for FPGA
- Complex operations (division, floating-point): Better in CPU unless high throughput needed

### 6. Parallelism and Pipelining

**Spatial parallelism:**
- Multiple independent operations execute simultaneously in different CLBs
- Example: All 16 S-box lookups in AES happen in parallel
- Limited only by available FPGA resources

**Pipelining:**
- Break operation into stages with flip-flops between
- New input every clock cycle, output every cycle after initial latency
- Example: AES with 14-stage pipeline can encrypt one block per cycle

**Sequential execution:**
- Reuse same hardware for multiple operations
- Lower resource usage, higher latency
- Example: AES with one round module used 14 times sequentially

### 7. System-on-Chip Integration

**Heterogeneous architecture:**
- **Hardcore processors**: ARM Cortex-A53 (application), Cortex-R5 (realtime)
  - Fixed in silicon, high performance
  - Run Linux, FreeRTOS, or bare-metal
- **FPGA fabric**: Configurable logic for acceleration
- **System bus**: Connects all components

**Division of labor:**
- CPU: Control logic, complex algorithms, OS, user interface
- FPGA: Data-intensive operations, parallel processing, custom interfaces

### 8. CPU-FPGA Communication

**AXI interface mechanism:**
- Memory-mapped registers in FPGA accessible from CPU
- CPU writes control registers (start, configuration)
- CPU reads status registers (done, results)
- Shared memory for bulk data transfer

**Example interaction sequence:**
1. CPU writes configuration to FPGA registers
2. CPU writes start bit to control register
3. FPGA processes data from shared memory
4. FPGA sets done bit in status register
5. CPU reads results from shared memory

### 9. FPGA Advantages for Specific Applications

**Ideal workloads:**
- **High data throughput**: Process data streams at wire speed (networking, video)
- **Highly independent parallelism**: Many operations with no dependencies
- **Deterministic execution**: Guaranteed fixed timing (industrial control, safety systems)
- **Low latency**: Minimal delay from input to output (high-frequency trading, signal processing)
- **Many interfaces**: Handle multiple data streams simultaneously (oscilloscopes, test equipment)
- **Custom protocols**: Implement non-standard interfaces not available in standard chips

**Poor fit:**
- Complex sequential algorithms with many dependencies
- Floating-point heavy computations (unless using hard DSP blocks)
- Frequent algorithm changes during operation
- Simple control tasks better suited to microcontroller

## Important Diagrams & Graphics

### 1. Design Space Triangle with FPGA Position

**Description:**
- Triangle with vertices: Cost, Energy, Performance
- Shows trade-off space for embedded systems
- FPGA positioned between "1 CPU" and "2 CPUs"
- Indicates FPGA offers: moderate cost, good performance, moderate energy
- Alternative options shown: Low-Energy CPU, 1 CPU, 1 CPU + FPGA, 2 CPUs
- **Key insight**: FPGA fills gap when single CPU insufficient but full ASIC or multi-CPU too expensive

**When to draw:**
- Show FPGA's position in design space
- Explain why FPGA chosen over alternatives
- Illustrate energy-performance-cost trade-offs

### 2. Simple CPU Architecture (Recap)

**Description:**
- Core registers (R0-R15) including SP, LR, PC
- 32-bit ALU with inputs A, B and output C
- Control Unit with Instruction Register (IR)
- Flags (APSR)
- Bus Interface connecting to Address and Data buses
- Von Neumann architecture with shared instruction/data path

**Key contrast with FPGA:**
- CPU: Fixed components, instruction-driven
- FPGA: Configurable components, structure-driven

### 3. CPU ALU Detail

**Description:**
- Shows internal structure of 32-bit ALU
- Multiple operation blocks: +, -, *, &, ~, /, ...
- Multiplexer selects operation based on instruction
- Inputs A and B (32 bits each)
- Output C (32 bits)
- Illustrates how CPU has fixed set of operations

**Comparison to FPGA:**
- FPGA: Can implement custom operations, multiple ALUs in parallel

### 4. CPU Die Photo vs GPU Die Photo

**CPU characteristics (from die photo):**
- Few large cores
- Large caches (L2, L3)
- Memory controller visible
- Much area dedicated to control logic

**GPU characteristics (from die photo):**
- Many small parallel processing units
- Minimal cache per unit
- Optimized for throughput not latency
- Visual contrast shows parallelism vs complexity trade-off

### 5. FPGA Die Photo

**Description:**
- Regular array structure visible
- Repeating pattern of CLBs
- Routing channels between blocks
- Periphery: I/O blocks
- **Key observation**: Homogeneous structure unlike CPU/GPU with specialized blocks

**What to note:**
- Fabric regularity enables flexible function mapping
- No visible "function-specific" blocks (contrast with CPU's cache, ALU)
- Routing area significant portion of die

### 6. Configurable Logic Block (CLB) Internal Structure

**Components shown:**
- **LUT block**: Contains 6:1 multiplexer, inputs I0-I5, outputs O6 and O5
  - Can implement one 6-input function or two 5-input functions
- **Flip-flops**: Q1 and Q2 for sequential logic
- **Multiplexers**: Select between LUT output, direct input X or I
- **Configuration**: "Note" indicators showing multiplexer inputs include carry and wide multiplexers

**Data flow:**
- Inputs → LUT → (optionally) Flip-flop → Output
- Alternative: Direct input → Flip-flop → Output (bypass LUT)

**What this enables:**
- Combinational logic (LUT only)
- Sequential logic (LUT + FF)
- Cascading (connect multiple CLBs)

### 7. LUT Implementation Example (7-Segment Decoder)

**Left side - Truth table:**
- 4 inputs (x3, x2, x1, x0) representing BCD digit
- 8 outputs (a-g segments + decimal point)
- Shows how input pattern maps to output pattern
- Example: 0000 → display "0" → segments a,b,c,e,f,g ON

**Right side - Hardware implementation:**
- Block diagram showing 4 inputs entering "Decoder" block
- 8 outputs going to 7-segment display
- **Key point**: This entire truth table fits in LUTs

**FPGA advantage:**
- All outputs computed simultaneously in 1 cycle
- CPU would need multiple instructions per output

### 8. Routing Network Between CLBs

**Three views shown:**

**Wide view:**
- Grid of CLBs (visible as repeated blocks)
- Vertical and horizontal routing channels
- Cross-connections at intersections

**Medium zoom:**
- Individual CLB blocks visible
- Routing multiplexers at edges
- Connection points where signals can turn

**Close-up:**
- SRAM cells (configuration memory) controlling routing
- Multiplexers selecting which route to take
- Shows how SRAM bit = 0 or 1 determines connection

**Analogy:**
- Like highway system with on-ramps, off-ramps, and interchanges
- Each CLB can connect to nearby or distant CLBs
- Configuration determines which "roads" are active

### 9. System-on-Chip Block Diagram (Zynq UltraScale+)

**Major components numbered:**
1. **PL (Programmable Logic)**: FPGA fabric with CLBs
2. **APU (Application Processing Unit)**: ARM Cortex-A53 quad-core, runs Linux
3. **RPU (Real-time Processing Unit)**: ARM Cortex-R5 dual-core, runs FreeRTOS
4. **Peripherals**: GPIO, USB, UART, SPI, CAN, I2C, etc.
5. **CSU (Configuration Security Unit)**: Handles FPGA configuration and security
6. **System Bus**: Interconnect (CCI) between all components
7. **AXI Interfaces**: Multiple AXI ports connecting PL to system

**Additional blocks:**
- DDR Controller: Interface to external DRAM
- GPU Mali-400: Graphics acceleration
- OCM (On-Chip Memory): Fast scratchpad memory
- PMU (Platform Management Unit): Power and system management
- PCIe, SATA, Display Port: External interfaces

**Key insight:**
- Heterogeneous system: multiple processor types + FPGA on single chip
- Flexible partitioning: Choose CPU vs FPGA per function

### 10. AXI Address Map

**Shows:**
- Memory-mapped address space for FPGA blocks
- Example: CBC (AES) block at address 0x8002'0000
- Address range: 64K (0x8002'0000 to 0x8002'FFFF)
- Within this range: individual registers at specific offsets

**How CPU accesses:**
- Request from kernel to map physical address
- Receive virtual address pointer
- Read/write memory locations → accesses FPGA registers

### 11. AES Encryption Round Diagram

**Flow shown:**
- Plaintext (16 bytes) → Initial transformation
- State after initial → Round 1 (4 transformations)
- Round 1 output → Round N-1
- Round N-1 output → Round N (3 transformations, no MixColumns)
- Final State → Ciphertext (16 bytes)

**Side: Key expansion:**
- Input key (M bytes, M=16/24/32)
- Expanded to round keys
- One round key per round

**Indicates:**
- Multiple rounds of same operations
- Opportunity for: sequential (reuse hardware) or pipelined (replicate hardware)

### 12. AES SubBytes Operation with S-box

**Visualization:**
- Input block: 4x4 matrix of bytes (00, 04, 08, 12, ...)
- S-box: Lookup table shown as block
- Each byte → S-box → Substituted byte
- Output: Transformed block

**Example shown:**
- Byte at position (row=0, col=2) with value 0x0F
- Goes through LUT
- Output: Substituted value

**FPGA implementation insight:**
- LUT perfect for this: truth table is S-box
- All 16 bytes substituted in parallel
- 1 clock cycle for entire block

### 13. AES ShiftRows Operation

**Before (Block):**
```
00  04  08  12
01  05  09  13
02  06  10  14
03  07  11  15
```

**After (Shifted Rows):**
```
00  04  08  12      (row 0: no shift)
05  09  13  01      (row 1: shift left 1)
10  14  02  06      (row 2: shift left 2)
15  03  07  11      (row 3: shift left 3)
```

**Implementation shown:**
- Arrows showing data movement from input to output positions
- Pure routing, no logic gates

**FPGA advantage:**
- No clock cycles needed!
- Just configure routing to connect appropriate positions
- CPU needs at least one instruction per row (4 instructions minimum)

### 14. AES MixColumns Operation

**Matrix multiplication shown:**
```
┌─────┐   ┌────┐       ┌──────┐
│ 2 3 1 1│   │ 04 │       │ M04  │
│ 1 2 3 1│ * │ 05 │   =   │ M05  │
│ 1 1 2 3│   │ 06 │       │ M06  │
│ 3 1 1 2│   │ 07 │       │ M07  │
└─────┘   └────┘       └──────┘
```

**Applied to each column of state independently:**
- Column 0: (04,05,06,07) → (M04,M05,M06,M07)
- Repeat for columns 1, 2, 3

**FPGA implementation diagram:**
- Shows parallel computation paths
- Matrix multiplication decomposed to XOR and multiply operations in GF(256)
- All 4 columns processed simultaneously

**Key:** Multiplication by 1, 2, 3 in GF(256) → specific logic functions, not arithmetic multiply

### 15. AES AddRoundKey (XOR) Operation

**Visualization:**
```
Block:              Round Key:          Result:
┌────────────┐      ┌────────────┐      ┌────────────┐
│ 00 04 08 12│      │ 00 04 08 12│      │ 00 04 08 12│
│ 01 05 09 13│  ⊕   │ 01 05 09 13│  =   │ 01 05 09 13│
│ 02 06 10 14│      │ 02 06 10 14│      │ 02 06 10 14│
│ 03 07 11 15│      │ 03 07 11 15│      │ 03 07 11 15│
└────────────┘      └────────────┘      └────────────┘
```

**Implementation shown:**
- 16 parallel XOR gates (one per byte)
- Each pair of bytes goes through XOR ($)
- All XORs happen simultaneously

**FPGA implementation:**
- Single clock cycle for entire block
- CPU would need at least 4 instructions (one per 32-bit word)

### 16. Sequential vs Pipelined AES Implementation

**Sequential (top):**
```
input → [subst][shift][mix][xor] → output
         └─────── repeat n times ────────┘
```
- One set of hardware, reused N times (10/12/14 rounds)
- Lower resource usage
- Latency: N × 3 cycles

**Pipelined (bottom):**
```
input → [round 1, 3 cycles] → [round 2, 3 cycles] → ... → [round n, 3 cycles] → output
```
- N copies of hardware, each processing different round
- Higher resource usage (N times more)
- Latency: N × 3 cycles (same)
- **Throughput**: New block every 3 cycles (N/3 times faster throughput!)

**Trade-off:**
- Sequential: Low resources, low throughput
- Pipelined: High resources, high throughput
- Choice depends on: application requirements, available FPGA resources

### 17. Vivado Resource Utilization Report

**Bar chart showing:**
- LUT: 11% utilized
- LUTRAM: 1% utilized
- FF (Flip-Flops): 5% utilized
- BUFG: 1% utilized

**Indicates:**
- Two AES instances in FPGA
- Plenty of resources remaining (89% LUTs free)
- Could add more AES instances or other functions

### 18. Vivado Implemented Design Layout

**Visualization:**
- Top view of FPGA die with placed logic
- Green/cyan blocks: Configured CLBs with AES logic
- Purple/pink blocks: I/O and interface blocks
- Yellow blocks: System interconnect
- Black areas: Unused FPGA fabric

**Shows:**
- Spatial distribution of AES implementation
- Concentration of logic in specific regions
- Routing between blocks
- Efficient use of local resources

## Formulas & Procedures

### 1. LUT Configuration Size Formula

**Formula:**
```
Configuration_bits = 2^N
```

Where:
- N = number of LUT inputs
- Configuration_bits = memory bits needed to store truth table

**Examples:**
- 4-input LUT: 2^4 = 16 bits
- 5-input LUT: 2^5 = 32 bits
- 6-input LUT: 2^6 = 64 bits

**Implications:**
- LUT size grows exponentially with inputs
- 6-input LUTs are practical limit in most FPGAs
- Larger functions require cascading multiple LUTs

### 2. FPGA Clock Period Calculation

**Formula:**
```
Clock_period = Logic_delay + Routing_delay + Setup_time

Max_frequency = 1 / Clock_period
```

**Components:**
- Logic_delay: Time for signals to propagate through LUTs
- Routing_delay: Time for signals to travel through interconnect
- Setup_time: Required time before clock edge for flip-flop

**Typical values:**
- Simple design: 200-1000 MHz possible
- Complex design: 50-200 MHz typical
- Routing often dominates in large designs

### 3. Pipeline Throughput Calculation

**Formula:**
```
Throughput = Clock_frequency × Block_size / Pipeline_stages

Latency = Pipeline_stages × Clock_period
```

**Example for pipelined AES:**
- Clock_frequency = 200 MHz
- Block_size = 128 bits (16 bytes)
- Pipeline_stages = 14 (for AES-256)

```
Throughput = 200 MHz × 128 bits / 1 = 25.6 Gb/s
            = 3.2 GB/s

Latency = 14 × 5ns = 70ns
```

**Insight:** Pipelining trades latency for throughput

### 4. Resource Utilization Percentage

**Formula:**
```
Utilization% = (Used_resources / Available_resources) × 100%
```

**Example from lecture:**
- Used LUTs: ~6,000
- Available LUTs: ~54,000
```
LUT_utilization = (6,000 / 54,000) × 100% = 11%
```

**Design rule:**
- Target 60-80% utilization for best results
- >90%: Routing becomes difficult
- <50%: Possibly oversized FPGA

### 5. Parallel Speedup Calculation

**Formula:**
```
Speedup = T_sequential / T_parallel

For N parallel operations:
Speedup_ideal = N
Speedup_actual = N / (1 + Overhead_factor)
```

**Example: AES S-box lookups:**
- Sequential CPU: 16 lookups × T_lookup = 16T
- Parallel FPGA: 1 lookup time = T
```
Speedup = 16T / T = 16× theoretical
```

**Reality:** Communication overhead reduces actual speedup

### 6. FPGA Design Flow Procedure

**Standard flow:**
1. **Specification**: Define requirements (functionality, performance, resources)
2. **HDL coding**: Write Verilog/VHDL or use HLS (High-Level Synthesis)
3. **Simulation**: Verify functionality with testbenches
4. **Synthesis**: Convert HDL to netlist of logic gates
5. **Implementation**:
   - Translate: Convert netlist to FPGA primitives
   - Map: Assign logic to specific LUTs/FFs
   - Place: Position logic elements on FPGA
   - Route: Connect placed elements
6. **Timing analysis**: Verify timing constraints met
7. **Bitstream generation**: Create configuration file
8. **Program FPGA**: Load bitstream onto device
9. **Hardware verification**: Test on actual FPGA

### 7. AXI Register Access Procedure

**CPU accessing FPGA register:**
```c
// 1. Map physical address to virtual
void *base_addr;
int fd = open("/dev/mem", O_RDWR);
base_addr = mmap(NULL, 0x10000, PROT_READ|PROT_WRITE,
                 MAP_SHARED, fd, 0x80020000);

// 2. Access registers via pointer arithmetic
volatile uint32_t *control_reg = (uint32_t*)(base_addr + 0x00);
volatile uint32_t *status_reg = (uint32_t*)(base_addr + 0x04);
volatile uint32_t *data_reg = (uint32_t*)(base_addr + 0x08);

// 3. Write configuration
*control_reg = 0x01;  // Start operation

// 4. Wait for completion
while((*status_reg & 0x01) == 0);  // Poll done bit

// 5. Read result
uint32_t result = *data_reg;

// 6. Cleanup
munmap(base_addr, 0x10000);
close(fd);
```

**Key steps:**
- Use /dev/mem to access physical memory
- mmap() creates virtual mapping
- Volatile prevents compiler optimization
- Memory barriers may be needed for ordering

## Comparisons & Contrasts

### 1. CPU vs FPGA vs GPU vs ASIC

| Aspect | CPU | FPGA | GPU | ASIC |
|--------|-----|------|-----|------|
| **Architecture** | Fixed, general-purpose | Configurable logic fabric | Fixed, massively parallel | Fixed, application-specific |
| **Programming** | Software (C/C++/assembly) | Hardware description (HDL) or HLS | GPU languages (CUDA/OpenCL) | Not programmable |
| **Flexibility** | High (any algorithm) | High (reconfigurable) | Medium (parallel algorithms) | None (single function) |
| **Performance** | Good for sequential | Excellent for parallel logic | Excellent for data parallel | Best for specific task |
| **Power Efficiency** | Moderate | Moderate | Lower (high power) | Highest |
| **Development Time** | Short | Long | Medium | Very long |
| **Cost** | Low-Medium | Medium-High | Medium | High NRE, low volume cost |
| **Clock Speed** | High (GHz) | Medium (100-500MHz) | High (GHz) | Highest (GHz+) |
| **Latency** | Good | Excellent (deterministic) | Higher | Excellent |
| **Best Use Case** | Control, OS, general compute | Custom hardware accelerators | Parallel numerical compute | Mass production products |

**Decision criteria:**
- Volume < 10k units: FPGA
- Volume > 100k units: Consider ASIC
- Need reconfiguration: FPGA
- Sequential algorithm: CPU
- Data parallel: GPU
- Custom interfaces: FPGA

### 2. Sequential vs Pipelined FPGA Implementation

| Aspect | Sequential | Pipelined |
|--------|-----------|-----------|
| **Hardware Reuse** | Same hardware for each iteration | Separate hardware for each stage |
| **Resource Usage** | Low (1× base unit) | High (N× base units) |
| **Latency** | N × stage_delay | N × stage_delay (same) |
| **Throughput** | 1 / (N × stage_delay) | 1 / stage_delay (N× better) |
| **Clock Frequency** | Can be higher (simpler timing) | May be lower (complex routing) |
| **Power Consumption** | Lower (less active logic) | Higher (all stages active) |
| **Design Complexity** | Lower | Higher (pipeline control) |
| **Best For** | Single operations, limited resources | Streaming data, high throughput |

**Example: AES with 14 rounds:**
- Sequential: 14 × 3 cycles = 42 cycles per block
- Pipelined: 3 cycles per block (after initial 42 cycle fill)
- Throughput improvement: 14× for streaming data

### 3. SRAM-based vs Flash-based FPGA Configuration

| Property | SRAM-based (Xilinx, Altera/Intel) | Flash-based (Microchip, Lattice) |
|----------|----------------------------------|----------------------------------|
| **Volatility** | Volatile (loses config on power-off) | Non-volatile (retains config) |
| **Boot Time** | Slower (reload from external memory) | Instant-on |
| **External Boot Storage** | Required (SPI Flash, SD card) | Optional (for updates) |
| **Power Consumption** | Higher (due to SRAM leakage) | Lower |
| **Reconfiguration** | Fast (milliseconds) | Slower (seconds) |
| **Density** | Higher (more logic per area) | Lower |
| **Cost** | Lower for large devices | Higher |
| **Security** | Needs encryption for bitstream | Inherently more secure |
| **Partial Reconfiguration** | Supported | Limited support |
| **Use Cases** | High-performance applications | Power-sensitive, secure applications |

**Hybrid approach:** SRAM FPGA + encrypted external Flash

### 4. Softcore vs Hardcore Processors on FPGA

| Aspect | Softcore (MicroBlaze, NIOS V) | Hardcore (ARM Cortex-A53/R5) |
|--------|-------------------------------|-------------------------------|
| **Implementation** | Uses FPGA logic fabric | Fixed silicon blocks |
| **Flexibility** | Configurable features | Fixed features |
| **Performance** | Lower (50-200 MHz) | Higher (up to 1.5 GHz) |
| **Resource Usage** | Consumes LUTs/FFs | No fabric resources used |
| **Power** | Higher per instruction | Lower per instruction |
| **ISA Options** | Various (MicroBlaze, RISC-V) | ARM only |
| **Peripherals** | Fully customizable | Fixed + customizable via fabric |
| **Software Ecosystem** | Limited | Mature (Linux, RTOS, compilers) |
| **Cost** | Free (uses existing fabric) | Included in SoC, higher chip cost |
| **When to Use** | Simple control, custom ISA | OS support, high performance |

**Common strategy:** Hardcore for main application, softcore for simple control tasks

### 5. FPGA Logic Functions vs CPU Instructions

| Operation | FPGA Implementation | CPU Implementation | FPGA Advantage |
|-----------|---------------------|-------------------|----------------|
| **Logic (AND, OR, XOR)** | 1 LUT, 0 cycles (combinational) | 1 instruction, 1 cycle | Parallel: many simultaneous |
| **Lookup Table** | Direct LUT, 1 cycle | Memory load, 3-5 cycles + cache | Built-in, faster |
| **Data Reordering** | Pure routing, 0 cycles | MOV instructions, N cycles | No time cost |
| **Bit Manipulation** | Logic gates, 1 cycle | Shift/mask instructions, multiple cycles | Arbitrary patterns in 1 cycle |
| **Addition (32-bit)** | Dedicated or LUT-based, 1 cycle | ADD instruction, 1 cycle | Comparable |
| **Multiplication** | DSP block or LUT-cascade, 1-3 cycles | MUL instruction, 3-5 cycles | Slightly better |
| **Division** | LUT-cascade, many cycles | DIV instruction, 10-40 cycles | Worse than CPU |
| **Floating-Point** | Large LUT resources or hard FP blocks | FPU, 3-10 cycles | Worse unless hard blocks |
| **State Machine** | LUTs + FFs, 1 cycle/state | Multiple instructions per state | Much faster |

**Key insight:** FPGA excels at logic, lookup, and bitwise; struggles with complex arithmetic

### 6. FPGA Clock Frequencies vs Other Technologies

| Implementation | Typical Clock Speed | Reason |
|----------------|-------------------|---------|
| **Modern CPU** | 2-5 GHz | Advanced process node, optimized critical paths, deep pipelines |
| **GPU** | 1-2 GHz | Parallel design trades frequency for throughput |
| **Simple FPGA Design** | 200-1000 MHz | Good routing, short logic paths |
| **Complex FPGA Design** | 50-200 MHz | Long routing delays, deep logic |
| **FPGA DSP Blocks** | 400-600 MHz | Hardened arithmetic, optimized timing |
| **FPGA Block RAM** | 400-600 MHz | Hardened memory, regular structure |
| **ASIC** | 1-3 GHz | Optimized for specific design, custom routing |

**Why FPGA is slower:**
- Configurable routing has more delay than fixed wiring
- LUTs slower than direct logic gates
- General-purpose fabric vs optimized design

**Compensation:** Parallelism makes up for lower frequency

### 7. Memory Types in FPGA Systems

| Memory Type | Size | Speed | Location | Usage |
|-------------|------|-------|----------|-------|
| **LUT RAM** | Small (KB) | Fastest (1 cycle) | Within CLBs | Small lookup tables, FIFOs |
| **Block RAM (BRAM)** | Medium (MB) | Very fast (1-2 cycles) | Dedicated blocks in fabric | Buffers, small arrays, packet storage |
| **Ultra RAM** | Large (tens of MB) | Fast (few cycles) | Dedicated blocks (newer FPGAs) | Large buffers, frame buffers |
| **On-Chip Memory (OCM)** | Medium (MB) | Very fast | Processor subsystem | CPU scratchpad, DMA buffers |
| **DDR DRAM** | Large (GB) | Slower (10-30 cycles) | External chip | Main memory, large datasets |
| **Flash** | Largest (GB) | Slowest (100s of cycles) | External chip | Non-volatile storage, bitstream |

**Access pattern optimization:**
- Frequent access, small data: BRAM
- Large datasets: DDR with caching strategy
- Configuration data: LUT RAM

## Examples & Applications

### Example 1: AES S-box Substitution in FPGA

**Problem:** Implement AES SubBytes operation that replaces each of 16 input bytes with corresponding S-box value.

**Software approach:**
```c
uint8_t sbox[256] = { /* 256 byte lookup table */ };
uint8_t state[16];

for(int i = 0; i < 16; i++) {
    state[i] = sbox[state[i]];  // 16 iterations
}
// Total: ~16 cycles (assuming 1 cycle per lookup)
```

**FPGA approach:**
- Implement 16 LUTs, each storing 256-byte S-box
- All 16 bytes feed into separate LUTs simultaneously
- All 16 outputs available in 1 clock cycle

**FPGA advantage:**
- 16× faster for this operation
- Parallelism vs sequential execution

**Resource cost:**
- 16 LUTs × 256 bytes = 4096 bytes of configuration memory
- In 6-input LUT: Need 8-input function, use LUT cascade: ~128 LUTs total
- Still parallel, 1-2 cycle latency

### Example 2: Implementing 16-bit Counter in FPGA

**Requirements:** 16-bit counter that increments every clock cycle.

**FPGA implementation:**
```verilog
always @(posedge clk) begin
    if (reset)
        counter <= 16'h0000;
    else
        counter <= counter + 1;
end
```

**Hardware mapping:**
- 16 flip-flops for storing counter value
- Carry chain for increment (dedicated fast carry logic)
- Resources: ~16 LUTs + 16 FFs

**Operation:**
- Each clock cycle: Read counter → Add 1 → Store new value
- Deterministic 1-cycle operation
- Can run multiple independent counters in parallel

**Comparison to CPU:**
- CPU: LOAD, ADD, STORE (3 instructions)
- FPGA: Direct hardware update (0 instructions)
- Multiple counters in FPGA: No additional time cost

### Example 3: Custom Protocol Interface

**Scenario:** Implement custom serial protocol with:
- 10 MHz clock
- 8-bit data + 2-bit header + 6-bit CRC
- Specific timing requirements: data valid 50ns before clock edge

**Why FPGA ideal:**
- Guaranteed deterministic timing (hard to achieve in software)
- Can bit-bang at exact timing
- Parallel CRC calculation while receiving data
- No OS jitter or interrupt latency

**Implementation:**
```verilog
// State machine for protocol
case(state)
    IDLE: if (start) state <= HEADER;
    HEADER: /* capture 2 bits, state <= DATA */
    DATA: /* capture 8 bits, compute CRC, state <= CRC */
    CRC: /* verify 6-bit CRC, state <= IDLE */
endcase
```

**Advantage over CPU:**
- CPU must poll or use interrupts (timing uncertainty)
- FPGA directly implements timing in hardware
- Can handle multiple protocols simultaneously

### Example 4: Real-Time Image Processing Filter

**Task:** Apply 3×3 convolution filter to video stream (1920×1080 @ 60 fps).

**Requirements calculation:**
```
Pixel rate = 1920 × 1080 × 60 = 124.4 million pixels/second
Per-pixel ops = 9 multiplies + 8 adds = 17 ops
Total ops = 124.4M × 17 = 2.1 billion ops/second
```

**CPU approach:**
- Sequential processing of each pixel
- ~20 instructions per pixel
- Needs ~2.5 billion instructions/second
- Difficult to achieve real-time on modest CPU

**FPGA approach:**
- Pipeline design: 3 rows buffered in BRAM
- Parallel multiply-accumulate for 9 pixels
- New result every clock cycle
```
Clock requirement = 124.4 MHz
FPGA easily achieves 200+ MHz
→ Real-time processing guaranteed
```

**Implementation:**
- 9 multipliers (use DSP blocks)
- Adder tree for accumulation
- Line buffers (3× 1920 pixels)
- Total: ~10% of medium FPGA

### Example 5: Network Packet Processing

**Requirements:**
- Process incoming packets at 10 Gbps
- Extract header fields
- Compute checksum
- Make forwarding decision

**Calculation:**
```
Minimum packet size: 64 bytes
Maximum packet rate = 10 Gbps / (64 bytes × 8) = 19.5 million packets/second
Time per packet = 51 ns
```

**Challenge for CPU:**
- 51 ns = ~100 clock cycles at 2 GHz
- Must parse, process, decide in 100 cycles
- Difficult with OS overhead

**FPGA solution:**
```
Pipeline stages:
1. Receive packet (streaming)
2. Parse header (combinational logic, 0 cycles)
3. Lookup routing table (BRAM, 1 cycle)
4. Compute checksum (parallel XOR tree, 1 cycle)
5. Make decision (lookup, 1 cycle)
6. Forward packet (streaming)

Total latency: ~10 cycles = 50 ns at 200 MHz
Throughput: 1 packet/cycle after pipeline fill
```

**FPGA advantage:**
- Deterministic low latency
- Wire-speed processing
- No packet loss under load

### Example 6: Design Space Exploration - Choosing AES Implementation

**Scenario:** Need AES encryption for IoT device communication.

**Option 1: Software on ARM Cortex-M4 @ 100 MHz**
- Performance: ~200 cycles/block = 2 μs/block = 500k blocks/sec = 64 MB/s
- Energy: ~40 μJ/block (estimated)
- Cost: CPU already present
- Flexibility: Easy to update algorithm

**Option 2: FPGA Sequential (14 rounds sequential, 100 MHz clock)**
- Performance: 42 cycles/block = 420 ns/block = 2.4M blocks/sec = 307 MB/s
- Energy: ~5 μJ/block (estimated)
- Cost: + $10 for FPGA chip
- Flexibility: Requires FPGA reprogramming to update

**Option 3: FPGA Pipelined (14-stage pipeline, 100 MHz clock)**
- Performance: 1 block/cycle (after fill) = 10 ns/block = 100M blocks/sec = 12.8 GB/s
- Energy: ~1 μJ/block (estimated)
- Cost: + $10 for FPGA chip (same), uses 14× more FPGA resources
- Flexibility: Requires FPGA reprogramming to update

**Decision matrix:**
| Requirement | Best Option |
|-------------|-------------|
| Cost-sensitive, moderate performance | Software |
| Power-critical, good performance | FPGA Sequential |
| Maximum throughput needed | FPGA Pipelined |
| Frequent algorithm changes | Software |
| Fixed algorithm, volume production | FPGA or ASIC |

**Actual choice depends on:**
- Data rate requirements
- Power budget
- Cost constraints
- Development time available
- Future flexibility needs

### Example 7: Debugging FPGA Design

**Common issue:** Design works in simulation but fails on hardware.

**Debugging procedure:**
1. **Check clock domains:**
   - Ensure proper synchronizers between clock domains
   - Use FPGA vendor's clock domain crossing (CDC) analysis tools

2. **Verify timing:**
   - Check timing report for negative slack
   - Ensure setup/hold times met
   ```
   Timing report shows:
   Slack = -0.5 ns (VIOLATION)
   Required: 10.0 ns
   Actual: 10.5 ns

   Fix: Add pipeline stage or reduce clock frequency
   ```

3. **Use chipscope/ILA (Integrated Logic Analyzer):**
   - Insert debug cores to capture internal signals
   - Trigger on error conditions
   - Analyze waveforms on actual hardware

4. **Check initialization:**
   - FPGAs need proper reset strategy
   - Ensure all flip-flops have defined initial state

5. **Verify I/O standards:**
   - Check voltage levels match (3.3V, 2.5V, 1.8V, etc.)
   - Ensure termination resistors present if needed

**Example fix:**
```verilog
// Problem: Missing synchronizer on async input
input async_signal;

// Fix: Add two-stage synchronizer
reg [1:0] sync;
always @(posedge clk) begin
    sync <= {sync[0], async_signal};
end
wire sync_signal = sync[1];
```

## Connections to Other Topics

### 1. Connection to Lecture 01 (Embedded Systems)

**Embedded system classifications:**
- FPGA fits in "high-performance embedded" category
- Combines flexibility with performance
- Used in aerospace, medical, automotive where reliability and performance critical

**System constraints:**
- Real-time requirements: FPGA offers deterministic timing
- Energy constraints: Can be more efficient than general-purpose CPU for specific tasks
- Cost constraints: Higher NRE but lower volume cost than ASIC

### 2. Connection to Lecture 02 (Software Paradigms)

**Hardware-Software co-design:**
- Partition algorithm between CPU and FPGA
- Software handles control, FPGA handles data-intensive operations
- Example: Video processing - CPU handles UI, FPGA handles pixel operations

**Interface paradigms:**
- Memory-mapped I/O: FPGA registers accessed like memory
- Interrupt-driven: FPGA signals CPU on completion
- DMA: Direct memory access for bulk data transfer

### 3. Connection to Lecture 06 (Non-Functional Requirements)

**Performance:**
- FPGA accelerates throughput-intensive operations
- Predictable latency for real-time systems

**Energy:**
- Custom hardware more efficient than general purpose for specific tasks
- Dynamic reconfiguration for power management

**Reliability:**
- Deterministic behavior improves testability
- Can implement redundancy for fault tolerance
- SEU (Single Event Upset) considerations for aerospace

**Cost:**
- Higher unit cost but lower development cost than ASIC
- Reconfigurability reduces inventory costs

### 4. Connection to Lecture 08 (Energy)

**Energy consumption factors:**
- **Static power**: SRAM-based FPGAs have leakage current
- **Dynamic power**: P = α × C × V² × f
  - α = activity factor (what percentage of gates switching)
  - C = capacitance
  - V = voltage
  - f = frequency

**Energy optimization strategies:**
- Clock gating: Disable clock to unused blocks
- Dynamic frequency scaling: Lower frequency when full performance not needed
- Partial reconfiguration: Reconfigure only needed portions
- Choose appropriate FPGA implementation (sequential vs pipelined)

**Comparison to P2 lab:**
- Measured energy consumption of AES on Cortex-M4
- FPGA AES would show different characteristics: higher throughput, different energy profile

### 5. Connection to Lecture 09 (Performance)

**Amdahl's Law applies to FPGA acceleration:**
```
If 20% of application can be accelerated 100× in FPGA:
S_overall = 1 / (0.8 + 0.2/100) = 1 / 0.802 = 1.25×

Overall speedup limited by non-accelerated portion!
```

**Performance optimization techniques:**
- **Parallelism**: Multiple operations simultaneously (data parallelism)
- **Pipelining**: Overlap operations (temporal parallelism)
- **Loop unrolling**: Eliminate loop overhead by replicating logic
- **Caching**: Use BRAM to cache frequently accessed data

**Profiling FPGA designs:**
- Use timing reports to find critical paths
- Resource utilization shows bottlenecks
- Identify hot spots for optimization

### 6. Connection to Lab P3 (AES Multi-Processor)

**Lab P3 compared three implementations:**
1. AES on APU (Cortex-A53 software)
2. AES on RPU (Cortex-R5 via RPC)
3. **Could add**: AES on FPGA fabric

**Expected performance:**
- Software APU: ~1-2 μs per block
- Software RPU: Similar, but with RPC overhead
- FPGA sequential: ~0.5 μs per block
- FPGA pipelined: 0.01-0.1 μs per block (100-10× faster)

**Communication overhead still applies:**
- CPU→FPGA data transfer via AXI takes time
- For very small messages, overhead dominates
- For streaming data, FPGA excels

**Lesson:** Choose implementation based on data characteristics and performance requirements

### 7. Connection to Lab P4 (AES in Hardware)

**Lab P4 explores:**
- Implementing AES directly in FPGA fabric (this lecture's focus)
- Using CSU (Configuration Security Unit) hardware acceleration
- Performance comparison of different hardware approaches

**Key concepts from this lecture applied in P4:**
- LUTs implement S-box lookups
- Routing implements ShiftRows
- Parallel XOR for AddRoundKey
- Sequential vs pipelined trade-offs

**Expected results:**
- FPGA implementation much faster than software
- Hardware accelerator (CSU) fastest but less flexible

### 8. Connection to Lecture 11-12 (Design Space Exploration)

**FPGA in design space:**
- Variable point in cost-performance-energy triangle
- Can explore different implementations (sequential vs pipelined) within FPGA
- Trade resource usage vs throughput

**Design parameters to explore:**
- Clock frequency
- Pipeline depth
- Parallelism level
- Resource allocation (BRAMs vs logic, DSP blocks vs LUTs)

**Pareto frontier:**
- Different FPGA implementations form points on frontier
- No single "best" - depends on constraints

### 9. Connection to Lecture 13 (RTOS Scheduling)

**FPGA for real-time systems:**
- Deterministic execution eliminates scheduling jitter
- Can implement hard real-time functions in FPGA
- CPU handles non-critical tasks, FPGA handles time-critical

**Example:** Motor control
- FPGA: PWM generation, encoder reading (deterministic timing)
- CPU: Control algorithm, UI, communication (handled by RTOS)

**Advantage:** Offloading to FPGA reduces CPU real-time burden

### 10. Connection to Lecture 14 (Multi-Processor Systems)

**Heterogeneous multiprocessing:**
- FPGA as specialized co-processor
- Similar to GPU but customizable
- Communication via shared memory or message passing

**Zynq UltraScale+ MPSoC (covered in this lecture):**
- APU: 4× Cortex-A53 (application)
- RPU: 2× Cortex-R5 (realtime)
- GPU: Mali-400 (graphics)
- FPGA: Programmable logic (custom acceleration)

**Task partitioning strategy:**
- Control flow: Application processor
- Real-time: Realtime processor
- Data-intensive: FPGA fabric
- Graphics: GPU

**Communication mechanisms:**
- AXI interconnect for control/status
- DMA for bulk data transfer
- Shared memory for large datasets
- Cache coherency considerations (CCI - Cache Coherent Interconnect)

### 11. Connection to Lecture 04 (Modeling)

**FPGA design flow uses models:**
- **Behavioral model**: High-level algorithm (C/C++, Python)
- **RTL model**: Register Transfer Level (Verilog/VHDL)
- **Structural model**: Netlist of gates and connections
- **Physical model**: Placed and routed on actual FPGA

**Design entry methods:**
- Traditional: Write HDL (Verilog/VHDL)
- Modern: High-Level Synthesis (HLS) from C/C++
- Visual: Block diagrams (Vivado IPI, Quartus Platform Designer)

**Verification:**
- Simulation at each abstraction level
- Formal verification for critical paths
- Hardware-in-the-loop testing

### 12. Connection to Lecture 05 (Development Processes)

**FPGA development differs from pure software:**
- Longer compilation times (minutes to hours)
- Hardware bugs harder to debug
- Iteration cycles longer

**Process implications:**
- More emphasis on upfront design and simulation
- Version control for HDL and constraints
- Continuous integration more challenging
- Need hardware prototyping boards

**Agile in FPGA development:**
- Difficult to apply traditional agile (long iteration times)
- Hybrid approach: Agile for software, waterfall for FPGA
- Use HLS for faster prototyping

### 13. Connection to Lab P5 (Design Space Exploration)

**Lab P5 explores design alternatives:**
- Different partitioning of algorithm across processors and accelerators
- This lecture provides FPGA as additional implementation option

**Exploration dimensions:**
- CPU only vs CPU+FPGA vs CPU+GPU vs multi-CPU
- Within FPGA: Sequential vs pipelined, clock frequency, resource usage
- Energy-performance trade-offs

**Automated DSE for FPGA:**
- Tools can generate multiple FPGA implementations
- Explore Pareto frontier automatically
- Choose based on specific requirements

## Summary

FPGAs provide configurable hardware acceleration for embedded systems through:
- **Programmable logic cells (LUTs)** implementing arbitrary logic functions
- **Programmable routing** connecting cells in custom patterns
- **Sequential elements (flip-flops)** enabling state machines and pipelining
- **Parallel execution** of independent operations
- **Deterministic timing** for real-time applications

Key advantages: reconfigurability, parallel processing, custom interfaces, deterministic behavior
Key disadvantages: lower clock speeds, higher cost than microcontrollers, longer development time, complexity of math operations

FPGAs fill the gap between general-purpose processors and ASICs, ideal for:
- High-throughput data processing (networking, video, signal processing)
- Custom protocols and interfaces
- Hardware acceleration of specific algorithms
- Low/medium volume applications needing performance beyond standard processors

Integration with CPUs in SoC creates powerful heterogeneous systems leveraging strengths of both: CPU for control and complex algorithms, FPGA for data-intensive and parallel operations.

## Possible Exam Questions

### Basic Definitions (20 questions)

1. **What does FPGA stand for and what is its key characteristic?**
   - Field Programmable Gate Array; configurable logic fabric that can be programmed in the field to implement custom digital circuits

2. **What is a LUT (Lookup Table) and how does it implement logic functions?**
   - Memory-based element that stores output values for every input combination; acts as programmable truth table

3. **How many configuration bits does a 6-input LUT require?**
   - 2^6 = 64 bits to store all possible output values

4. **What is the purpose of flip-flops in an FPGA CLB?**
   - Store state for sequential logic; enable counters, shift registers, state machines; update on clock edge

5. **How are connections between CLBs implemented in an FPGA?**
   - Via multiplexers controlled by configuration memory (SRAM or Flash cells)

6. **What is the difference between SRAM-based and Flash-based FPGAs?**
   - SRAM: volatile, faster reconfiguration, higher density; Flash: non-volatile, instant-on, more secure

7. **What is AXI and what is it used for?**
   - Advanced eXtensible Interface; ARM bus protocol for on-chip communication between processors and FPGA blocks

8. **What is the difference between a hardcore and softcore processor?**
   - Hardcore: fixed silicon processor (ARM); Softcore: processor implemented in FPGA fabric (MicroBlaze, NIOS)

9. **What is memory-mapped I/O?**
   - Technique where hardware registers accessed using memory addresses via system bus

10. **What is a bitstream in FPGA context?**
    - Binary configuration file loaded into FPGA to define its functionality (LUT contents, routing, etc.)

11. **What does "Field Programmable" mean in FPGA?**
    - Can be configured/programmed by end user in the field (not just at factory)

12. **What is pipelining in FPGA design?**
    - Breaking operation into stages with flip-flops between; new input accepted every cycle

13. **What is a CLB (Configurable Logic Block)?**
    - Basic building block of FPGA containing LUTs, flip-flops, multiplexers, and routing

14. **What does "reconfigurable" mean for FPGAs?**
    - Functionality can be changed by loading new bitstream; same hardware implements different circuits

15. **What is routing delay in FPGA?**
    - Time for signals to propagate through configurable interconnect between logic blocks

16. **What is an S-box in AES?**
    - Substitution box; lookup table performing non-linear byte substitution

17. **What is the purpose of a PLL in an FPGA?**
    - Phase-Locked Loop; generates clock signals at different frequencies from input clock

18. **What is meant by "spatial computation" in FPGAs?**
    - Operations distributed across physical chip area simultaneously vs sequential execution in CPU

19. **What is a System-on-Chip (SoC)?**
    - Single integrated circuit combining processors, FPGA fabric, memory controllers, and peripherals

20. **What is deterministic execution?**
    - Guaranteed consistent execution time regardless of data or system state

### Conceptual Understanding (20 questions)

21. **Why is FPGA slower in clock frequency than modern CPUs but can still be faster for certain operations?**
    - Configurable routing adds delay vs fixed wiring; compensated by massive parallelism executing many operations simultaneously

22. **Explain why data movement operations (like AES ShiftRows) are "free" in FPGAs.**
    - Pure routing without logic gates; configuration sets up direct wire connections; no clock cycles needed

23. **How does an FPGA implement a logic function without having a defined instruction set?**
    - LUT stores truth table directly; input combination selects corresponding output; becomes the function in hardware

24. **Why are LUTs ideal for implementing AES S-box substitution?**
    - S-box is lookup table by nature; LUT stores entire S-box; direct mapping, single cycle access

25. **Explain the trade-off between sequential and pipelined FPGA implementations.**
    - Sequential: low resources, low throughput, reuses hardware; Pipelined: high resources, high throughput, parallel stages

26. **How does FPGA achieve deterministic execution time?**
    - No OS scheduling, cache effects, or interrupts; pure hardware with fixed propagation delays; same path every time

27. **Why would you choose FPGA over ASIC for a product?**
    - Lower NRE cost; reconfigurability for updates; faster time-to-market; suitable for low-medium volume production

28. **Explain how CPU and FPGA communicate via AXI memory-mapped registers.**
    - FPGA blocks assigned address range; CPU writes/reads addresses like memory; accesses FPGA control/status registers

29. **Why are complex math operations (division, floating-point) challenging in FPGAs?**
    - Require many LUTs to implement; iterative algorithms; unless hard blocks present, less efficient than dedicated CPU units

30. **How does pipelining increase throughput without reducing latency?**
    - Latency = total stages × cycle time (same); but new result every cycle after fill; throughput = 1/cycle not 1/latency

31. **Explain why FPGAs are well-suited for network packet processing.**
    - Deterministic low latency; parallel header parsing; wire-speed processing without OS overhead; custom protocol support

32. **How does FPGA's architecture enable parallel execution of operations?**
    - Independent CLBs operate simultaneously; no shared ALU; spatial distribution of operations across fabric

33. **Why is development time longer for FPGA than software?**
    - Hardware design requires timing analysis; long compilation (synthesis, place, route); harder debugging; steeper learning curve

34. **Explain the concept of "configuration memory" in FPGAs.**
    - SRAM/Flash cells storing LUT contents and routing selections; defines FPGA functionality; loaded from bitstream

35. **How does FPGA's reconfigurability benefit system design?**
    - Update algorithms in field; bug fixes without hardware change; adapt to new standards; reduce development risk

36. **Why might FPGA use more power than CPU for same operation?**
    - Configurable routing less efficient than direct wiring; SRAM leakage current; less optimized for general operations

37. **Explain why AES encryption is well-suited for FPGA implementation.**
    - Logic operations (XOR, substitution); parallel execution of all bytes; lookup tables map to LUTs; pipeline-friendly structure

38. **How does heterogeneous SoC leverage both CPU and FPGA strengths?**
    - CPU: control, complex algorithms, OS; FPGA: data-intensive, parallel, custom interfaces; optimal task partitioning

39. **Why is timing analysis critical in FPGA design?**
    - Must ensure signals propagate within clock period; routing delays vary; violations cause functional errors

40. **Explain the role of flip-flops in enabling pipelining.**
    - Store intermediate results between pipeline stages; synchronize to clock; enable overlapped execution

### Implementation & Practical (15 questions)

41. **How many LUTs would be needed to implement 16 parallel AES S-box lookups?**
    - Each S-box: 8-bit input → 8-bit output requires ~8 LUTs (cascaded); Total: ~128 LUTs for 16 parallel S-boxes

42. **If an FPGA design uses 11% of LUTs and 5% of flip-flops, what does this indicate?**
    - Plenty of resources available; could add more functionality; design likely dominated by combinational logic (LUTs > FFs)

43. **Calculate throughput of pipelined AES with 14 stages at 200 MHz clock.**
    - After pipeline fill: 1 block per cycle; Throughput = 200M blocks/sec × 128 bits = 25.6 Gb/s = 3.2 GB/s

44. **How would you verify FPGA timing constraints are met?**
    - Run static timing analysis; check for negative slack; ensure all paths meet setup/hold times; review timing report

45. **Describe the steps to access FPGA register from Linux application.**
    - Open /dev/mem; mmap() physical address; cast to pointer; read/write via pointer; use volatile keyword

46. **Why is clock distribution important in FPGA design?**
    - Ensures all flip-flops see clock edge simultaneously; minimizes skew; enables synchronous operation; uses dedicated routing

47. **How would you debug an FPGA design that works in simulation but fails on hardware?**
    - Check timing report for violations; use ILA/chipscope to capture internal signals; verify clock domain crossings; check I/O standards and resets

48. **What factors limit maximum clock frequency in FPGA design?**
    - Logic depth between registers; routing delay; complexity of combinational paths; hard block limits (BRAM, DSP)

49. **Describe how to implement a simple state machine in FPGA.**
    - Use enumerated type for states; flip-flops store current state; combinational logic determines next state; update on clock edge

50. **How would you partition AES encryption between CPU and FPGA?**
    - CPU: key management, mode control, data transfer; FPGA: core AES rounds (SubBytes, ShiftRows, MixColumns, AddRoundKey)

51. **Why might you use Block RAM instead of LUTs for data storage?**
    - BRAM denser for large arrays; faster access; dedicated resources don't consume logic fabric; designed for memory

52. **How do you interface custom FPGA logic to ARM processor in SoC?**
    - Create AXI slave interface; define register map; connect via AXI interconnect; implement control/status registers

53. **What determines whether to use sequential or pipelined implementation?**
    - Throughput requirements (pipelined for high); resource constraints (sequential for limited); latency tolerance; power budget

54. **How would you optimize FPGA design that fails timing?**
    - Add pipeline stages; reduce logic depth; use faster blocks (BRAM, DSP); optimize critical path; lower clock frequency if acceptable

55. **Describe process of programming FPGA with new functionality.**
    - Design HDL or HLS; simulate and verify; synthesize to netlist; place and route; generate bitstream; load via JTAG or flash

### Analysis & Comparison (15 questions)

56. **Compare resource usage of 16-bit addition in FPGA vs CPU.**
    - FPGA: ~16 LUTs + carry chain, always present; CPU: single ADD instruction using fixed ALU; CPU more efficient for general arithmetic

57. **Why might software AES on CPU be preferred over FPGA despite lower performance?**
    - Lower cost (no FPGA); easier updates; shorter development time; sufficient for low data rates; more portable

58. **Contrast how CPU and FPGA execute a for-loop.**
    - CPU: loop instructions, branch prediction, sequential iterations; FPGA: unroll loop into parallel hardware or sequential state machine

59. **Compare latency vs throughput for sequential and pipelined FPGA designs.**
    - Latency: same (N×delay); Throughput: pipelined N× higher; sequential: 1/(N×delay), pipelined: 1/delay

60. **How does FPGA's power consumption compare to CPU for AES encryption?**
    - Lower energy per operation (custom hardware); but higher static power (leakage); overall depends on utilization and technology node

61. **Compare FPGA and GPU for parallel processing.**
    - GPU: fixed parallel architecture, high throughput, mature tools; FPGA: customizable parallelism, lower latency, custom interfaces

62. **Why is FPGA better than CPU for packet processing but worse for web server?**
    - Packet: deterministic, parallel, low latency needed; Web: complex logic, irregular patterns, sequential processing suitable

63. **Contrast SRAM-based (Xilinx) vs Flash-based (Microchip) FPGA applications.**
    - SRAM: high-performance, reconfigurable apps; Flash: power-critical, secure, instant-on embedded systems

64. **How does clock frequency limitation affect FPGA vs CPU comparison?**
    - FPGA: 100-500 MHz typical; CPU: 2-5 GHz; FPGA compensates with parallelism; CPU limited to sequential execution

65. **Compare development time for software on CPU vs FPGA implementation.**
    - Software: faster (hours to days), easier debugging; FPGA: slower (days to weeks), complex timing analysis, longer compilation

66. **Why might ASIC be chosen over FPGA for high-volume product?**
    - Lower unit cost at volume; higher performance; lower power; optimized for single function; but high NRE and inflexible

67. **How does FPGA's determinism compare to RTOS on CPU?**
    - FPGA: absolute determinism (hardware timing); RTOS: bounded latency but with jitter; FPGA better for hard real-time

68. **Compare AES implementation complexity: CPU software vs FPGA hardware.**
    - Software: easier to write and debug, portable; FPGA: requires HDL knowledge, timing analysis, but higher performance

69. **Why are lookup operations faster in FPGA than CPU?**
    - FPGA: LUT is direct truth table, 1 cycle; CPU: memory access, cache miss possible, 3-5+ cycles

70. **How does FPGA routing flexibility compare to ASIC fixed wiring?**
    - FPGA: fully configurable, slower, general purpose; ASIC: optimized fixed paths, faster, specific function only

### Problem Solving & Calculations (15 questions)

71. **Calculate AES throughput: 100 MHz clock, sequential implementation, 42 cycles/block.**
    - Throughput = 100M cycles/s ÷ 42 cycles/block = 2.38M blocks/s × 16 bytes = 38.1 MB/s

72. **Estimate LUT utilization for 8-input multiplexer.**
    - 8:1 MUX has 8 data inputs + 3 select = 11 signal inputs; Needs multiple 6-input LUTs cascaded; ~4 LUTs required

73. **If design uses 60% of LUTs, how many more AES blocks can fit if each uses 5%?**
    - Available: 100% - 60% = 40%; Number: 40% ÷ 5% = 8 more AES blocks

74. **Calculate energy per AES block: 1.5W power, 0.5 μs per block.**
    - Energy = Power × Time = 1.5W × 0.5μs = 0.75 μJ per block

75. **What clock frequency needed for 1 Gbps throughput with 8-bit datapath?**
    - Frequency = 1 Gb/s ÷ 8 bits = 125 MHz minimum

76. **Calculate speedup if 30% of app accelerated 50× in FPGA.**
    - S = 1 / (0.7 + 0.3/50) = 1 / 0.706 = 1.42× overall speedup

77. **Estimate BRAM usage for 1024-byte FIFO buffer.**
    - Typical BRAM block: 36 Kb = 4608 bytes; Need: 1024 bytes ÷ 4608 = 0.22 blocks; Use 1 BRAM block

78. **Calculate latency for 10-stage pipeline at 200 MHz.**
    - Latency = 10 stages × (1/200MHz) = 10 × 5ns = 50ns

79. **How many parallel operations to saturate 10 Gbps link with 64-byte packets?**
    - Packet rate = 10 Gb/s ÷ (64 bytes × 8 bits) = 19.5M packets/s; If 1 operation per packet: 19.5M ops/s needed

80. **If FPGA uses 40 mW idle, 200 mW active, calculate average power at 50% utilization.**
    - Average = 0.5 × 40mW + 0.5 × 200mW = 20mW + 100mW = 120mW

81. **Compare cost: FPGA ($10) vs ASIC ($50k NRE + $2/unit) at 10k units.**
    - FPGA: 10k × $10 = $100k; ASIC: $50k + 10k × $2 = $70k; ASIC cheaper at this volume

82. **Calculate bits needed for 32:1 multiplexer select signal.**
    - 32 inputs requires log₂(32) = 5 select bits

83. **If implementation fails timing by 2ns, what clock frequency is achievable?**
    - If violated 10ns constraint by 2ns: actual is 12ns; Max freq = 1/12ns = 83.3 MHz

84. **How many 6-input LUTs to implement 4×4 matrix multiply?**
    - 16 outputs, each sum of 4 products; Each product needs multiply+add; Complex, estimate ~200-300 LUTs with optimization

85. **Calculate improvement: sequential 42 cycles vs pipelined (after fill) 1 cycle.**
    - For single block: same latency; For streaming: throughput improves 42×

### Critical Thinking & Design Decisions (15 questions)

86. **When would you choose FPGA over multi-core CPU for embedded system?**
    - Need deterministic real-time; custom interfaces; higher throughput than CPU can provide; specific algorithm acceleration; parallel data processing

87. **What factors should guide decision between SRAM and Flash FPGA?**
    - Boot time (Flash instant-on); power (Flash lower); cost (SRAM lower); security (Flash better); performance (SRAM higher)

88. **How would you decide whether to pipeline an FPGA design?**
    - Check throughput requirements; assess available resources; consider latency tolerance; evaluate power budget; analyze data arrival pattern

89. **What considerations for partitioning algorithm between CPU and FPGA?**
    - Data movement overhead; algorithm complexity; parallelizability; real-time requirements; development effort; maintainability

90. **Why might hardware accelerator not improve overall system performance?**
    - Communication overhead (Amdahl's Law); small fraction accelerated; bottleneck elsewhere; data transfer limits; setup/teardown costs

91. **How do you choose between implementing logic in LUTs vs DSP blocks?**
    - DSP for arithmetic (multiply, MAC); LUTs for logic, bit operations; DSP faster and more efficient for math; LUTs more flexible

92. **When is reconfigurability advantage vs drawback?**
    - Advantage: field updates, multiple functions, prototyping; Drawback: boot time, security risk, potential instability, added complexity

93. **What determines whether FPGA or GPU better for application?**
    - FPGA: custom algorithms, low latency, deterministic, custom interfaces; GPU: standard parallel patterns, high throughput, mature tools

94. **How would you validate FPGA design meets functional requirements?**
    - Comprehensive testbenches; known-good test vectors; corner cases; hardware verification; formal verification for critical paths; compare to reference

95. **What trade-offs in choosing FPGA clock frequency?**
    - Higher: better performance, harder timing closure, more power; Lower: easier timing, less power, lower performance; sweet spot balance

96. **When should you use hardcore vs softcore processor in SoC?**
    - Hardcore: need high performance, OS support, complex software; Softcore: simple control, custom ISA, resource constrained

97. **How do you decide on FPGA vs ASIC for product?**
    - Volume (high → ASIC); flexibility needs (updates → FPGA); development time (fast → FPGA); cost (calculate breakeven); risk tolerance

98. **What factors determine if FPGA solution is cost-effective?**
    - Development cost vs unit savings; volume; software/hardware skill availability; time-to-market value; maintenance costs

99. **How would you approach debugging complex FPGA timing violation?**
    - Identify critical path from timing report; analyze fanout; consider pipelining; check routing congestion; simplify logic; use timing constraints effectively

100. **When is it acceptable for FPGA to have lower clock speed than CPU?**
    - When parallelism compensates; deterministic timing more important than raw speed; throughput vs latency priority; power constraints

### Integration & Connections (5 questions)

101. **How does FPGA relate to energy-performance-cost design space from Lecture 01?**
    - Fills gap between single CPU and multiple CPUs; trades moderate cost increase for significant performance gain; energy efficiency varies by application

102. **Connect FPGA acceleration to Amdahl's Law from Lecture 09.**
    - Only parallelizable portion accelerated; communication overhead reduces speedup; overall speedup limited by sequential fraction; must consider entire system

103. **How does FPGA implementation compare to multi-processor approach in P3 lab?**
    - Different trade-off: FPGA higher performance for specific task; multi-processor more flexible; FPGA deterministic; CPU easier to program

104. **Relate FPGA design flow to software development processes from Lecture 05.**
    - Similar phases but longer iterations; more emphasis on upfront design; verification more critical; harder to apply agile; waterfall more common

105. **How does FPGA fit into heterogeneous multi-processor systems from Lecture 14?**
    - Specialized accelerator alongside general CPUs; task-specific processing; communication via shared bus; optimal workload distribution
