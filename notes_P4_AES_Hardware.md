# Lab P4: AES in Hardware (FPGA and CSU)

## Overview

This lab demonstrates hardware-accelerated AES encryption using two different approaches on the Xilinx Zynq UltraScale+ MPSoC:
- **Part 1 (P4.1)**: AES implementation in FPGA using memory-mapped I/O
- **Part 2 (P4.2)**: AES implementation in Configuration Security Unit (CSU) using socket communication

Both provide hardware acceleration but with different characteristics, trade-offs, and use cases.

---

## Technical Terms & Definitions

### Part 1: FPGA Implementation

**FPGA (Field-Programmable Gate Array)**
- Reconfigurable digital logic device
- Can implement custom hardware circuits
- Configured at boot time with a bitstream
- Acts as custom peripherals once configured

**Hardware Accelerator**
- Specialized hardware designed to perform specific computations faster than general-purpose CPU
- Dedicated logic gates and circuits for specific algorithms
- Parallel processing capabilities

**Memory-Mapped I/O (MMIO)**
- Technique where hardware registers appear as memory addresses
- Software reads/writes to memory addresses to control hardware
- No special I/O instructions needed

**AXI Interface (Advanced eXtensible Interface)**
- Standard interface protocol for connecting IP blocks in FPGA
- Base address for FPGA region: 0x8000'0000
- Used for communication between processor and FPGA logic

**mmap() (Memory Mapping)**
- System call to request kernel permission for hardware access
- Creates virtual address mapping to physical hardware addresses
- Required due to Memory Protection Unit (MPU) in Linux

**Memory Protection Unit (MPU)**
- Hardware that prevents unauthorized memory access
- User programs cannot directly access physical addresses
- Kernel manages permissions through mmap()

**Virtual Address**
- Address used by software (provided by mmap)
- Kernel transparently translates to physical address
- Value differs from requested physical address

**Physical Address**
- Actual hardware address (e.g., 0x8002'0000 for AES block)
- Only accessible through kernel-managed mapping
- Direct access blocked by MPU

**Write-Only Register**
- Hardware register that can only be written, not read
- Reading returns undefined values
- Cannot use read-modify-write operations (|= won't work)

**Read-Only Register**
- Hardware register that can only be read, not written
- Used for status information and results
- Writing has no effect or causes errors

**Key Expansion**
- Process of generating round keys from main encryption key
- AES-256 requires 15 rounds, thus 15 round keys
- Hardware performs this automatically in FPGA

**Polling (Busy Waiting)**
- Continuously checking a status bit until condition met
- Used to wait for hardware operations to complete
- Simple but CPU-intensive

**In-Place Encryption**
- Result overwrites input buffer
- Same buffer used for input and output
- Saves memory but destroys original data

**Register Offset**
- Distance from base address to specific register
- AES block offset: 0x2'0000 from FPGA base
- Individual registers have offsets from block base (e.g., 0x08 for Control)

### Part 2: CSU Implementation

**Configuration Security Unit (CSU)**
- Dedicated security subsystem in Zynq UltraScale+ MPSoC
- Available from boot time for secure operations
- Isolated from main system for security

**Secure Processor Block (SPB)**
- Triple-redundant processor for fault tolerance
- Protected RAM and ROM
- Runs independently from main CPU

**Crypto Interface Block (CIB)**
- Hardware blocks for cryptographic algorithms (AES, SHA, RSA)
- Dedicated crypto engines
- Connected to protected key storage

**eFUSE**
- One-time programmable memory for storing keys
- Limited write cycles (approximately 10)
- Cannot be read back once written
- Physically protected against tampering
- Once exhausted, device unusable for security purposes

**Socket**
- Communication endpoint abstracting low-level details
- Can be for network communication or local system
- Provides standard interface for data exchange

**AF_ALG (Address Family for ALGorithms)**
- Special socket type for cryptographic operations
- Linux-specific interface to kernel crypto API
- Analogous to AF_INET (network) or AF_UNIX (local IPC)

**Socket Descriptor (sd)**
- File handle for socket
- Used for configuration operations
- Similar to file descriptor but for sockets

**File Descriptor (fd)**
- Handle for actual data communication
- Obtained from accept() after socket configuration
- Used for sending/receiving encrypted data

**bind() (Socket Binding)**
- Associates socket with specific algorithm/address
- Configures which crypto algorithm to use
- Must be done before using socket

**sockaddr_alg Structure**
- Configuration structure for algorithm socket
- Contains: family (AF_ALG), type (skcipher), name (cbc(aes))
- Passed to bind() to configure socket

**skcipher**
- Symmetric Key Cipher
- Type specification for AF_ALG socket
- Indicates algorithm uses same key for encrypt/decrypt

**setsockopt() (Set Socket Option)**
- Function to configure socket parameters
- Used to pass encryption key to CSU
- Sets various socket-level options

**accept() (Socket Accept)**
- Creates actual data channel for communication
- Returns file descriptor for data transfer
- Like accepting a network connection

**msghdr (Message Header)**
- Structure containing control information and data
- Includes both ancillary data (headers) and payload
- Used with sendmsg() for complex messages

**cmsghdr (Control Message Header)**
- Ancillary data header in message
- Can contain multiple headers (operation type, IV)
- Accessed via CMSG_FIRSTHDR and CMSG_NXTHDR macros

**iovec (I/O Vector)**
- Structure describing data buffer
- Contains pointer and length
- Used for scatter-gather I/O

**sendmsg()**
- Sends message with headers and data
- More complex than simple write()
- Allows sending control information along with data

**ALG_SET_OP**
- Control message type to set operation (encrypt/decrypt)
- Value: ALG_OP_ENCRYPT or ALG_OP_DECRYPT
- Part of message headers

**ALG_SET_IV**
- Control message type to set initialization vector
- Contains IV length and data
- Required for CBC mode

**af_alg_iv Structure**
- Structure for passing IV to CSU
- Contains IV length (ivlen) and IV data array
- Embedded in control message

**CMSG_SPACE()**
- Macro to calculate space needed for control message
- Includes header and data
- Used to allocate control buffer

**CMSG_DATA()**
- Macro to get pointer to data portion of control message
- Follows the header structure
- Where actual data is written

**Secure Boot**
- Process of verifying bootloader authenticity before execution
- CSU can verify signatures (RSA) and decrypt bootloader (AES)
- Ensures only authorized code runs on device

**Triple Redundancy**
- Running same computation on three processors
- Detects tampering or faults by comparing results
- Security feature of CSU's Secure Processor Block

---

## Core Concepts

### 1. Hardware Acceleration Fundamentals

**Why Hardware is Faster:**
- **Parallel Operations**: Multiple gates working simultaneously vs sequential CPU instructions
- **No Instruction Overhead**: Direct logic implementation, no fetch-decode-execute cycle
- **Pipelined Processing**: Multiple stages executing concurrently
- **Dedicated Resources**: Optimized specifically for one algorithm
- **No OS Overhead**: No interrupts, context switches, or scheduling delays

**Typical Performance:**
- Software (P3): ~1-10 ms per operation
- FPGA (P4.1): ~0.01-0.1 ms per operation (10-100x faster)
- CSU (P4.2): ~0.1-1 ms per operation (middle ground)

### 2. Memory-Mapped I/O (FPGA Approach)

**Address Space Layout:**
```
0x0000'0000        - DDR Memory Controller
0x8000'0000        - AXI Interface to FPGA (base)
0x8002'0000        - AES Block (base + 0x2'0000 offset)
0x000A0000'0000    - Other regions (PCIe, QSPI, RPU)
```

**Why mmap() is Required:**
- Linux Memory Protection Unit prevents direct hardware access
- Security measure to prevent unauthorized access
- Kernel manages permissions and address translation
- User receives virtual address pointer, kernel handles physical mapping

**Virtual vs Physical Addresses:**
- Physical: Actual hardware location (0x8002'0000)
- Virtual: Address visible to program (arbitrary value from mmap)
- Kernel MMU translates virtual to physical transparently
- Programmer uses virtual address, hardware sees physical

### 3. FPGA Register Interface

**Complete Register Map (from base 0x8002'0000):**
- **0x08**: Control Register (write-only) - Start operations
- **0x09**: Status Register (read-only) - Check completion
- **0x0A**: Config Register (write-only) - Set mode/key length
- **0x10-0x17**: Key Registers (write-only) - 256-bit key (8×32 bits)
- **0x20-0x23**: Block Registers (write-only) - Input data (4×32 bits)
- **0x30-0x33**: Result Registers (read-only) - Output data (4×32 bits)
- **0x40-0x43**: IV Registers (write-only) - Initialization vector (4×32 bits)

**Write-Only Register Implications:**
```c
// WRONG - Cannot read-modify-write!
*control_reg |= 0x01;  // Reads undefined value first

// CORRECT - Write complete value
*control_reg = 0x01;   // Direct write
```

### 4. FPGA Operation Sequence

**Step-by-Step Process:**
1. **Load Key**: Write 32 bytes to KEY0-KEY7 registers
2. **Configure**: Write config register (encrypt/decrypt, key length)
3. **Init Key Expansion**: Write 0x01 to control register (INIT bit)
4. **Wait Ready**: Poll status register until READY bit = 1
5. **Load IV**: Write 16 bytes to IV0-IV3 registers
6. **Load Block**: Write 16 bytes to BLOCK0-BLOCK3 registers
7. **Start Processing**: Write 0x02 to control register (NEXT bit)
8. **Wait Valid**: Poll status register until VALID bit = 1
9. **Read Result**: Read 16 bytes from RES0-RES3 registers

**Status Bits:**
- **READY (bit 0)**: Key expansion complete, ready for encryption
- **VALID (bit 1)**: Result is ready to read

### 5. Configuration Security Unit Architecture

**Two Main Components:**

**Secure Processor Block (SPB):**
- Triple-redundant processor (fault detection)
- Protected RAM and ROM (isolated memory)
- Independent from main CPU
- Available during boot process

**Crypto Interface Block (CIB):**
- Dedicated AES engine (hardware acceleration)
- SHA engine (hashing)
- RSA engine (asymmetric crypto)
- eFUSE key storage (protected, one-time programmable)

### 6. Socket Communication Model

**Why Sockets Instead of Registers:**
- CSU registers not publicly documented (security through obscurity)
- Prevents direct hardware manipulation by untrusted code
- OS provides controlled, validated access
- Standard Linux crypto API interface

**Socket Workflow:**
```
Application → socket(AF_ALG) → Kernel Crypto API → CSU Driver → CSU Hardware
```

**Configuration Sequence:**
1. Create socket with AF_ALG family
2. Bind to algorithm (AES-CBC)
3. Set key via setsockopt()
4. Accept to get data file descriptor
5. Send data with operation headers (encrypt/decrypt, IV)
6. Read encrypted result
7. Close file descriptor

### 7. Message Structure for CSU

**Complete Message Components:**
```
┌─────────────────┐
│  msghdr         │  ← Main message structure
├─────────────────┤
│ msg_control ────┼──┐  ← Points to control headers
│ msg_iov ────────┼──│───┐  ← Points to data buffer
└─────────────────┘  │   │
                     ▼   ▼
          ┌──────────────┐  ┌──────────┐
          │ Control Hdrs │  │   Data   │
          ├──────────────┤  │          │
          │ 1. Operation │  │ Plaintext│
          │    (Enc/Dec) │  │    or    │
          ├──────────────┤  │Ciphertext│
          │ 2. IV (16B)  │  │          │
          └──────────────┘  └──────────┘
```

**First Header (Operation):**
- Type: ALG_SET_OP
- Data: ALG_OP_ENCRYPT or ALG_OP_DECRYPT
- Size: 4 bytes (uint32_t)

**Second Header (IV):**
- Type: ALG_SET_IV
- Data: af_alg_iv structure (ivlen + IV bytes)
- Size: 4 bytes (length) + 16 bytes (IV) = 20 bytes

### 8. Security Features Comparison

**FPGA (P4.1):**
- Speed: Very fast (~0.01-0.1 ms)
- Security: None (registers visible, key readable)
- Isolation: None (part of main system)
- Key Storage: Volatile registers
- Use Case: High-performance applications without security requirements

**CSU (P4.2):**
- Speed: Fast (~0.1-1 ms, socket overhead)
- Security: Very high (isolated processor, protected keys)
- Isolation: Full (separate processor, protected memory)
- Key Storage: eFUSE (non-volatile, protected, one-time programmable)
- Use Case: Production devices, secure boot, key management

### 9. Performance Trade-offs

**FPGA Advantages:**
- Direct register access (minimal overhead)
- Fastest possible execution
- Simple programming model

**FPGA Disadvantages:**
- No security features
- Requires root privileges (sudo)
- Keys visible in memory
- No tamper protection

**CSU Advantages:**
- Protected key storage (eFUSE)
- Secure boot capability
- Tamper resistance (triple redundancy)
- Memory isolation
- No root privileges needed

**CSU Disadvantages:**
- Socket overhead (slower than FPGA)
- More complex programming model
- Limited eFUSE write cycles
- Message passing overhead

---

## Important Diagrams & Graphics

### Diagram 1: Memory Map Layout

```
┌────────────────────────────┐
│  0x0000'0000               │
│  ┌──────────────────────┐  │
│  │  DDR Memory          │  │
│  │  Controller          │  │
│  └──────────────────────┘  │
│  0x8000'0000               │
│  ┌──────────────────────┐  │
│  │  AXI Interface       │  │
│  │  to FPGA             │  │
│  │                      │  │
│  │  0x8002'0000         │  │
│  │  ┌────────────────┐  │  │
│  │  │  AES Block     │  │  │
│  │  │  (+0x2'0000)   │  │  │
│  │  └────────────────┘  │  │
│  └──────────────────────┘  │
│  0x000A0000'0000           │
│  ┌──────────────────────┐  │
│  │  Other Regions       │  │
│  │  (PCIe, QSPI, RPU)   │  │
│  └──────────────────────┘  │
│  0x1000'0000'0000          │
└────────────────────────────┘
```

**Purpose**: Shows how AES block is located in memory address space
**Key Points**:
- FPGA accessible from 0x8000'0000
- AES at offset +0x2'0000
- Must use mmap() to access

### Diagram 2: AES FPGA Register Layout

```
Offset    Register        Access    Size      Description
────────────────────────────────────────────────────────
0x08      Control         WO        32-bit    INIT, NEXT bits
0x09      Status          RO        32-bit    READY, VALID bits
0x0A      Config          WO        32-bit    Enc/Dec, KeyLen
0x10-17   KEY0-KEY7       WO        8×32-bit  256-bit key
0x20-23   BLOCK0-BLOCK3   WO        4×32-bit  128-bit input
0x30-33   RES0-RES3       RO        4×32-bit  128-bit output
0x40-43   IV0-IV3         WO        4×32-bit  128-bit IV
```

**Purpose**: Complete register map for FPGA AES block
**Key Points**:
- Three types: Write-Only, Read-Only
- 32-bit registers accessed as uint32_t
- Multiple registers for key/block/result/IV

### Diagram 3: Key Register Bit Layout (256-bit)

```
        256-bit Key               128-bit Key
┌──────────────────┐      ┌──────────────────┐
│ KEY0 │ 255..224  │      │ KEY0 │ 127..96   │
├──────┼───────────┤      ├──────┼───────────┤
│ KEY1 │ 223..192  │      │ KEY1 │  95..64   │
├──────┼───────────┤      ├──────┼───────────┤
│ KEY2 │ 191..160  │      │ KEY2 │  63..32   │
├──────┼───────────┤      ├──────┼───────────┤
│ KEY3 │ 159..128  │      │ KEY3 │  31..0    │
├──────┼───────────┤      ├──────┼───────────┤
│ KEY4 │ 127..96   │      │ KEY4 │  unused   │
├──────┼───────────┤      ├──────┼───────────┤
│ KEY5 │  95..64   │      │ KEY5 │  unused   │
├──────┼───────────┤      ├──────┼───────────┤
│ KEY6 │  63..32   │      │ KEY6 │  unused   │
├──────┼───────────┤      ├──────┼───────────┤
│ KEY7 │  31..0    │      │ KEY7 │  unused   │
└──────┴───────────┘      └──────┴───────────┘
```

**Purpose**: Shows how 256-bit key is distributed across 8 registers
**Key Points**:
- MSB in KEY0, LSB in KEY7
- 128-bit key uses only KEY0-KEY3
- Each register is 32 bits

### Diagram 4: FPGA Operation Flow

```
Start
  │
  ├─► Load Key (KEY0-KEY7)
  │
  ├─► Configure (Config Register)
  │     - Set Encrypt/Decrypt
  │     - Set Key Length (256-bit)
  │
  ├─► Init Key Expansion (Control = 0x01)
  │
  ├─► Poll Status until READY = 1
  │
  ├─► Load IV (IV0-IV3)
  │
  ├─► Load Block (BLOCK0-BLOCK3)
  │
  ├─► Start Processing (Control = 0x02)
  │
  ├─► Poll Status until VALID = 1
  │
  ├─► Read Result (RES0-RES3)
  │
  └─► Done
```

**Purpose**: Step-by-step operation sequence
**Key Points**:
- Key expansion done once
- Must wait for status bits
- Sequential operations

### Diagram 5: Configuration Security Unit Architecture

```
┌─────────────────────────────────────────┐
│  Configuration Security Unit (CSU)      │
├──────────────────┬──────────────────────┤
│ Secure Processor │ Crypto Interface     │
│ Block (SPB)      │ Block (CIB)          │
├──────────────────┼──────────────────────┤
│ - Triple         │ - AES Engine         │
│   Redundant CPU  │ - SHA Engine         │
│ - Protected RAM  │ - RSA Engine         │
│ - Protected ROM  │ - Protected Key      │
│                  │   Storage (eFUSE)    │
└──────────────────┴──────────────────────┘
```

**Purpose**: CSU internal architecture
**Key Points**:
- Two main blocks: SPB and CIB
- Triple redundancy for security
- Multiple crypto engines

### Diagram 6: Socket Communication Layers

```
┌─────────────────┐
│ User Application│  ← Your program
└────────┬────────┘
         │ AF_ALG Socket API
┌────────▼────────┐
│ Linux Kernel    │  ← Crypto API
│ Crypto API      │
└────────┬────────┘
         │ Driver Interface
┌────────▼────────┐
│ CSU Driver      │  ← Kernel module
└────────┬────────┘
         │ Hardware Access
┌────────▼────────┐
│ CSU Hardware    │  ← AES/SHA/RSA
│ (AES/SHA/RSA)   │
└─────────────────┘
```

**Purpose**: Shows abstraction layers for CSU access
**Key Points**:
- Multiple layers for security
- Socket API abstracts hardware
- Kernel manages permissions

### Diagram 7: Socket Setup Sequence

```
socket(AF_ALG)
    ↓
  [sd] created
    ↓
bind(sa) ─→ Configure: family=AF_ALG
             type=skcipher, name=cbc(aes)
    ↓
setsockopt(key) ─→ Load 256-bit key
    ↓
accept()
    ↓
  [fd] obtained ─→ Data channel ready
    ↓
close(sd) ─→ Config socket no longer needed
    ↓
Ready for data transfer
```

**Purpose**: Socket configuration steps
**Key Points**:
- Two descriptors: sd (config), fd (data)
- Configuration before data transfer
- Close sd after accept

### Diagram 8: Message Structure (sendmsg)

```
┌──────────────────────────────┐
│       msghdr                 │
├──────────────────────────────┤
│ msg_control ──────────┐      │
│ msg_controllen        │      │
│ msg_iov ─────────┐    │      │
│ msg_iovlen       │    │      │
└──────────────────┼────┼──────┘
                   │    │
        ┌──────────┘    └──────────────┐
        │                               │
        ▼                               ▼
┌──────────────────┐          ┌────────────────┐
│ Control Buffer   │          │  Data Buffer   │
├──────────────────┤          │  (iovec)       │
│ cmsghdr #1       │          │                │
│ ┌──────────────┐ │          │  Plaintext or  │
│ │cmsg_level    │ │          │  Ciphertext    │
│ │cmsg_type     │ │          │                │
│ │cmsg_len      │ │          │  (length bytes)│
│ │ALG_SET_OP    │ │          │                │
│ │  (4 bytes)   │ │          └────────────────┘
│ └──────────────┘ │
│ cmsghdr #2       │
│ ┌──────────────┐ │
│ │ALG_SET_IV    │ │
│ │  (20 bytes)  │ │
│ │ - ivlen: 16  │ │
│ │ - iv: 16B    │ │
│ └──────────────┘ │
└──────────────────┘
```

**Purpose**: Complete message structure for sendmsg
**Key Points**:
- Control headers separate from data
- Multiple control headers possible
- IV structure contains length + data

### Diagram 9: Complete CSU Message Flow

```
Your Program      Linux Kernel      CSU Hardware
     │                 │                  │
     │ socket(AF_ALG)  │                  │
     ├────────────────►│                  │
     │ sd              │                  │
     │                 │                  │
     │ bind(sa)        │                  │
     ├────────────────►│                  │
     │                 │ Configure        │
     │                 ├─────────────────►│
     │                 │  AES-CBC         │
     │                 │                  │
     │ setsockopt(key) │                  │
     ├────────────────►│                  │
     │                 │ Load Key         │
     │                 ├─────────────────►│
     │                 │ Key Expansion    │
     │                 │◄─────────────────┤
     │                 │                  │
     │ accept()        │                  │
     ├────────────────►│                  │
     │ fd              │                  │
     │                 │                  │
     │ sendmsg(data+IV)│                  │
     ├────────────────►│                  │
     │                 │ Encrypt/Decrypt  │
     │                 ├─────────────────►│
     │                 │   Processing...  │
     │                 │◄─────────────────┤
     │                 │   Result         │
     │ read()          │                  │
     ├────────────────►│                  │
     │◄────────────────┤                  │
     │  Result Data    │                  │
     │                 │                  │
     │ close(fd)       │                  │
     ├────────────────►│                  │
     │                 │ Release          │
     │                 ├─────────────────►│
```

**Purpose**: Complete communication sequence
**Key Points**:
- Multiple round-trips for setup
- Data processing happens in CSU
- Kernel mediates all access

### Diagram 10: eFUSE Key Storage

```
┌─────────────────────────────────┐
│  eFUSE Key Storage              │
├─────────────────────────────────┤
│  Properties:                    │
│  ✓ One-time programmable        │
│  ✓ Limited writes (~10 cycles)  │
│  ✓ Cannot be read back          │
│  ✓ Physically protected         │
│  ✓ Survives power off           │
│  ✓ Tamper resistant             │
│                                 │
│  Use Cases:                     │
│  - Device-unique keys           │
│  - License verification         │
│  - Secure boot keys             │
│  - Factory provisioning         │
└─────────────────────────────────┘
```

**Purpose**: eFUSE characteristics and use cases
**Key Points**:
- One-time programmable (permanent)
- Limited write cycles
- Critical for production security

### Diagram 11: Performance Comparison

```
         Software (P3)
            ~1-10 ms
         ═══════════════════════
               ▼
         CSU (P4.2)
            ~0.1-1 ms
         ═══════════
               ▼
         FPGA (P4.1)
          ~0.01-0.1 ms
         ═══════

Speed:   FPGA > CSU > Software
Security: CSU >> FPGA = Software
```

**Purpose**: Relative performance and security
**Key Points**:
- FPGA fastest but no security
- CSU balances speed and security
- Software slowest and least secure

### Diagram 12: Hardware vs Software Comparison

```
                Software        FPGA         CSU
              ┌─────────┐   ┌────────┐   ┌──────┐
Speed         │  Slow   │   │  Fast  │   │ Fast │
              └─────────┘   └────────┘   └──────┘

              ┌─────────┐   ┌────────┐   ┌──────┐
Security      │  None   │   │  None  │   │ High │
              └─────────┘   └────────┘   └──────┘

              ┌─────────┐   ┌────────┐   ┌──────┐
Isolation     │  None   │   │  None  │   │ Full │
              └─────────┘   └────────┘   └──────┘

              ┌─────────┐   ┌────────┐   ┌──────┐
Key Storage   │   RAM   │   │  Regs  │   │eFUSE │
              └─────────┘   └────────┘   └──────┘

              ┌─────────┐   ┌────────┐   ┌──────┐
Setup Cost    │   Low   │   │  Low   │   │Medium│
              └─────────┘   └────────┘   └──────┘
```

**Purpose**: Multi-dimensional comparison
**Key Points**:
- Each approach has trade-offs
- Choose based on requirements
- CSU best for security-critical

### Diagram 13: Block Register Data Flow (FPGA)

```
Input Buffer (16 bytes)
┌────┬────┬────┬────┬────┬────┬────┬────┐
│ 15 │ 14 │ 13 │ 12 │ 11 │ 10 │  9 │  8 │
└────┴────┴────┴────┴────┴────┴────┴────┘
┌────┬────┬────┬────┬────┬────┬────┬────┐
│  7 │  6 │  5 │  4 │  3 │  2 │  1 │  0 │
└────┴────┴────┴────┴────┴────┴────┴────┘
         │
         │ Cast to uint32_t*
         ▼
┌─────────────┬─────────────┬─────────────┬─────────────┐
│  BLOCK0     │  BLOCK1     │  BLOCK2     │  BLOCK3     │
│ bytes 15-12 │ bytes 11-8  │ bytes 7-4   │ bytes 3-0   │
│  127..96    │   95..64    │   63..32    │   31..0     │
└─────────────┴─────────────┴─────────────┴─────────────┘
         │
         │ Write to registers
         ▼
      FPGA AES Block
         │
         │ Encryption
         ▼
┌─────────────┬─────────────┬─────────────┬─────────────┐
│   RES0      │   RES1      │   RES2      │   RES3      │
│  127..96    │   95..64    │   63..32    │   31..0     │
└─────────────┴─────────────┴─────────────┴─────────────┘
         │
         │ Read from registers
         ▼
Output Buffer (16 bytes)
```

**Purpose**: Shows data flow through FPGA registers
**Key Points**:
- 16 bytes split into 4×32-bit registers
- Data processed in hardware
- Result read back to buffer

### Diagram 14: Security Comparison Matrix

```
                    FPGA          CSU
┌─────────────────┬────────┬────────────┐
│ Key Visibility  │ Visible│  Protected │
├─────────────────┼────────┼────────────┤
│ Tamper Detect   │   No   │    Yes     │
├─────────────────┼────────┼────────────┤
│ Secure Boot     │   No   │    Yes     │
├─────────────────┼────────┼────────────┤
│ Memory Isolation│   No   │    Yes     │
├─────────────────┼────────┼────────────┤
│ Key Storage     │Volatile│Non-volatile│
├─────────────────┼────────┼────────────┤
│ Root Required   │  Yes   │     No     │
├─────────────────┼────────┼────────────┤
│ Production Use  │   No   │    Yes     │
└─────────────────┴────────┴────────────┘
```

**Purpose**: Security feature comparison
**Key Points**:
- CSU superior in all security aspects
- FPGA suitable only for non-security apps
- CSU designed for production

---

## Formulas & Procedures

### Formula 1: Register Address Calculation

**Physical Address = Base Address + Block Offset + Register Offset**

For AES Control Register:
```
Physical Address = 0x8000'0000 + 0x2'0000 + 0x08
                 = 0x8002'0008
```

For AES KEY0 Register:
```
Physical Address = 0x8000'0000 + 0x2'0000 + 0x10
                 = 0x8002'0010
```

**Variables:**
- Base Address = 0x8000'0000 (FPGA AXI base)
- Block Offset = 0x2'0000 (AES block within FPGA)
- Register Offset = varies by register (0x08, 0x10, etc.)

### Procedure 1: FPGA Memory Mapping

**Step-by-step mmap() process:**

```c
// 1. Open /dev/mem to access physical memory
int m_mfd = open("/dev/mem", O_RDWR);
if (m_mfd == -1) {
    perror("Cannot open /dev/mem");
    exit(1);
}

// 2. Define physical address and page size
#define AES_BASE_ADDR 0x80020000
size_t page_size = sysconf(_SC_PAGE_SIZE);  // Usually 4096 bytes

// 3. Map physical address to virtual address
void *virtual_aes_base = mmap(
    NULL,                       // Let kernel choose virtual address
    page_size,                  // Size to map (4KB)
    PROT_READ | PROT_WRITE,    // Read and write permissions
    MAP_SHARED,                 // Shared with other processes/devices
    m_mfd,                      // File descriptor from /dev/mem
    AES_BASE_ADDR              // Physical address to map
);

if (virtual_aes_base == MAP_FAILED) {
    perror("mmap failed");
    close(m_mfd);
    exit(1);
}

// 4. Cast to register pointer
uint32_t *aes_reg = (uint32_t *)virtual_aes_base;

// 5. Now can access registers using offsets
aes_reg[0x08/4] = value;  // Write to control register
// (Divide by 4 because pointer arithmetic on uint32_t)
```

### Procedure 2: FPGA Key Loading

```c
// Convert key bytes to 32-bit words
uint32_t *key_word = (uint32_t *)key;

// Load key into 8 registers (256-bit key)
#define KEY_SIZE 32        // 32 bytes = 256 bits
#define DATA_BUS_SIZE 4    // 4 bytes per register

for (uint32_t i = 0; i < KEY_SIZE/DATA_BUS_SIZE; i++) {
    aes_reg[AES_KEY_REG(i)] = key_word[i];
}

// AES_KEY_REG(i) expands to: (0x10 + i)
// This writes to registers at offsets:
// 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17
```

### Procedure 3: FPGA Configuration

```c
// Build configuration value
uint32_t config = 0;

// Set encryption/decryption mode
#define CONFIG_ENCDEC_BIT 0
#define CONFIG_KEYLEN_BIT 1
#define AES_ENC 1
#define AES_256_BIT_KEY 1

if (enc == AES_ENC) {
    config |= (1u << CONFIG_ENCDEC_BIT);  // Bit 0 = 1 for encrypt
}

// Set key length to 256-bit
config |= (AES_256_BIT_KEY << CONFIG_KEYLEN_BIT);  // Bit 1 = 1

// Write complete configuration (must write all at once!)
aes_reg[AES_CONFIG_REG] = config;
```

### Procedure 4: FPGA Key Expansion

```c
// Start key expansion by setting INIT bit
#define CTRL_INIT_BIT 0
aes_reg[AES_CTRL_REG] = (1u << CTRL_INIT_BIT);

// Wait for key expansion to complete
#define STATUS_READY_BIT 0
while ((aes_reg[AES_STATUS_REG] & (1u << STATUS_READY_BIT)) == 0) {
    // Busy wait - hardware is working
    // This typically takes only a few clock cycles
}

// Now ready bit is set, can proceed with encryption
```

### Procedure 5: FPGA Block Encryption

```c
// Load block data (16 bytes = 4 registers)
uint32_t *buf_word = (uint32_t *)buf;
for (uint32_t i = 0; i < (16u / DATA_BUS_SIZE); i++) {
    aes_reg[AES_BLOCK_REG(i)] = buf_word[i];
}

// Load initialization vector (16 bytes = 4 registers)
uint32_t *iv_word = (uint32_t *)iv;
for (uint32_t i = 0; i < (IV_SIZE / DATA_BUS_SIZE); i++) {
    aes_reg[AES_IV_REG(i)] = iv_word[i];
}

// Start block processing by setting NEXT bit
#define CTRL_NEXT_BIT 1
aes_reg[AES_CTRL_REG] = (1u << CTRL_NEXT_BIT);

// Wait for operation to complete
#define STATUS_VALID_BIT 1
while ((aes_reg[AES_STATUS_REG] & (1u << STATUS_VALID_BIT)) == 0) {
    // Busy wait - hardware is processing
}

// Read result (4 registers back to buffer)
for (uint32_t i = 0; i < (16u / DATA_BUS_SIZE); i++) {
    buf_word[i] = aes_reg[AES_RESULT_REG(i)];
}
```

### Procedure 6: CSU Socket Setup

```c
// 1. Create AF_ALG socket
int sd = socket(AF_ALG, SOCK_SEQPACKET, 0);
if (sd == -1) {
    perror("socket");
    return;
}

// 2. Configure algorithm selection
struct sockaddr_alg sa = {
    .salg_family = AF_ALG,          // Algorithm family
    .salg_type = "skcipher",        // Symmetric key cipher
    .salg_name = "cbc(aes)"         // AES in CBC mode
};

// 3. Bind socket to algorithm
int ret = bind(sd, (struct sockaddr *)&sa, sizeof(sa));
if (ret == -1) {
    perror("bind");
    close(sd);
    return;
}

// 4. Set encryption key
#define AES_KEY_LENGTH 32  // 256-bit key
ret = setsockopt(sd, SOL_ALG, ALG_SET_KEY, key, AES_KEY_LENGTH);
if (ret == -1) {
    perror("setsockopt");
    close(sd);
    return;
}

// 5. Accept to get data file descriptor
int fd = accept(sd, NULL, 0);
if (fd == -1) {
    perror("accept");
    close(sd);
    return;
}

// 6. Close configuration socket (no longer needed)
close(sd);

// fd is now ready for data transfer
```

### Procedure 7: CSU Message Preparation

```c
// Allocate control buffer (space for 2 headers)
char cbuf[CMSG_SPACE(4) + CMSG_SPACE(20)] = {0};

// Initialize message structure
struct msghdr msg = {};
msg.msg_control = cbuf;
msg.msg_controllen = sizeof(cbuf);

// ─── First Header: Set Operation (Encrypt/Decrypt) ───

struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
cmsg->cmsg_level = SOL_ALG;
cmsg->cmsg_type = ALG_SET_OP;
cmsg->cmsg_len = CMSG_LEN(4);  // 4 bytes for uint32

// Write operation value
*(__u32 *)CMSG_DATA(cmsg) = enc_dir;
// enc_dir = ALG_OP_ENCRYPT or ALG_OP_DECRYPT

// ─── Second Header: Set IV ───

cmsg = CMSG_NXTHDR(&msg, cmsg);  // Move to next header
cmsg->cmsg_level = SOL_ALG;
cmsg->cmsg_type = ALG_SET_IV;
cmsg->cmsg_len = CMSG_LEN(20);  // 4 (ivlen) + 16 (iv data)

// Fill IV structure
struct af_alg_iv *ivp = (void *)CMSG_DATA(cmsg);
ivp->ivlen = 16;              // IV length for AES
memcpy(ivp->iv, iv, 16);      // Copy IV data

// ─── Data Buffer ───

struct iovec iov = {buf, length};
msg.msg_iov = &iov;
msg.msg_iovlen = 1;
```

### Procedure 8: CSU Data Transfer

```c
// Send message with headers and data
int ret = sendmsg(fd, &msg, 0);
if (ret == -1) {
    perror("sendmsg");
    close(fd);
    return;
}

// Read encrypted/decrypted result
ret = read(fd, buf, length);
if (ret == -1) {
    perror("read");
    close(fd);
    return;
}

// Result is now in buf (in-place operation)
// Close file descriptor when done
close(fd);
```

### Formula 2: Control Buffer Size Calculation

**Total Size = CMSG_SPACE(operation_size) + CMSG_SPACE(iv_size)**

```c
// For CSU message:
operation_size = 4 bytes (uint32_t for ALG_OP_ENCRYPT/DECRYPT)
iv_size = 20 bytes (4 bytes ivlen + 16 bytes IV data)

Total = CMSG_SPACE(4) + CMSG_SPACE(20)

// CMSG_SPACE adds header overhead and alignment padding
// Actual total is typically ~64-80 bytes depending on platform
```

### Procedure 9: Multi-Block Processing (FPGA)

```c
// For messages larger than 16 bytes
size_t block_count = length / 16;
uint8_t *current_block = buf;
uint8_t current_iv[16];

// Copy initial IV
memcpy(current_iv, iv, 16);

// Key expansion done only once before loop
aes_reg[AES_CTRL_REG] = (1u << CTRL_INIT_BIT);
while ((aes_reg[AES_STATUS_REG] & (1u << STATUS_READY_BIT)) == 0);

for (size_t block = 0; block < block_count; block++) {
    // Load current IV
    uint32_t *iv_word = (uint32_t *)current_iv;
    for (uint32_t i = 0; i < 4; i++) {
        aes_reg[AES_IV_REG(i)] = iv_word[i];
    }

    // Load block
    uint32_t *block_word = (uint32_t *)current_block;
    for (uint32_t i = 0; i < 4; i++) {
        aes_reg[AES_BLOCK_REG(i)] = block_word[i];
    }

    // Process block
    aes_reg[AES_CTRL_REG] = (1u << CTRL_NEXT_BIT);
    while ((aes_reg[AES_STATUS_REG] & (1u << STATUS_VALID_BIT)) == 0);

    // Read result
    for (uint32_t i = 0; i < 4; i++) {
        block_word[i] = aes_reg[AES_RESULT_REG(i)];
    }

    // For CBC mode: next IV is previous ciphertext
    if (block < block_count - 1) {
        memcpy(current_iv, current_block, 16);
    }

    // Move to next block
    current_block += 16;
}
```

---

## Comparisons & Contrasts

### Comparison 1: FPGA vs CSU - Speed

| Aspect | FPGA (P4.1) | CSU (P4.2) |
|--------|-------------|------------|
| **Typical Time** | ~0.01-0.1 ms | ~0.1-1 ms |
| **Setup Overhead** | Minimal (mmap once) | Medium (socket each time) |
| **Data Transfer** | Direct register write | Message passing through kernel |
| **Hardware Access** | Immediate | Through kernel driver |
| **Best For** | Maximum performance | Balanced performance + security |

**Why FPGA is Faster:**
- Direct register access without kernel mediation
- No socket creation/teardown overhead
- No message marshalling/unmarshalling
- Immediate hardware response

**Why CSU is Slower:**
- Socket creation overhead
- Kernel message passing
- Driver validation and security checks
- Context switches between user/kernel space

### Comparison 2: FPGA vs CSU - Security

| Feature | FPGA (P4.1) | CSU (P4.2) |
|---------|-------------|------------|
| **Key Storage** | Volatile registers (visible) | eFUSE (protected, permanent) |
| **Key Readback** | Possible (keys in memory) | Impossible (eFUSE cannot be read) |
| **Memory Isolation** | None (shared memory space) | Full (separate protected memory) |
| **Tamper Detection** | None | Triple redundancy detects faults |
| **Secure Boot** | No capability | Full capability |
| **Root Required** | Yes (direct hardware access) | No (socket API) |
| **Production Use** | Unsuitable for security | Designed for security |

**Winner: CSU for any security-critical application**

### Comparison 3: Write-Only vs Read-Only Registers

| Characteristic | Write-Only | Read-Only |
|----------------|------------|-----------|
| **Examples** | Control, Config, Key, Block, IV | Status, Result |
| **Purpose** | Configure hardware, provide input | Check status, retrieve output |
| **Read Behavior** | Returns undefined/garbage values | Returns actual hardware state |
| **Write Behavior** | Updates hardware | No effect or error |
| **Common Mistake** | Using `\|=` (read-modify-write) | Attempting to write |
| **Correct Usage** | Always write complete value | Only read |

**Critical Rule:** Write-only registers must be written completely, never use read-modify-write!

### Comparison 4: Socket vs Direct Register Access

| Aspect | Socket (CSU) | Direct Register (FPGA) |
|--------|--------------|------------------------|
| **Access Method** | System calls (socket, bind, etc.) | Memory writes via mmap() |
| **Abstraction Level** | High (kernel handles details) | Low (direct hardware control) |
| **Security** | Kernel enforces access control | No security (direct access) |
| **Portability** | Standard Linux API | Hardware-specific |
| **Setup Complexity** | High (multiple steps) | Low (single mmap) |
| **Debugging** | Harder (kernel black box) | Easier (direct observation) |
| **Error Handling** | Standard errno values | Hardware-specific behavior |

**Trade-off:** Sockets provide security and abstraction at cost of complexity and performance

### Comparison 5: Virtual vs Physical Addresses

| Aspect | Virtual Address | Physical Address |
|--------|----------------|------------------|
| **Who Uses** | User program | Hardware |
| **Assigned By** | Kernel (mmap) | Hardware design |
| **Value** | Arbitrary (chosen by kernel) | Fixed (0x8002'0000 for AES) |
| **Translation** | MMU handles transparently | No translation |
| **Security** | Protected (MPU enforced) | Unprotected (direct) |
| **Example** | Varies per process | Always 0x8002'0000 |

**Programmer View:** Use virtual address, kernel handles translation to physical

### Comparison 6: Software vs Hardware AES (All Three)

| Metric | Software (P3) | FPGA (P4.1) | CSU (P4.2) |
|--------|---------------|-------------|------------|
| **Speed** | Slowest (~1-10ms) | Fastest (~0.01-0.1ms) | Fast (~0.1-1ms) |
| **Security** | None | None | Very High |
| **Flexibility** | High (easy to change) | Low (fixed hardware) | Low (fixed hardware) |
| **Power Efficiency** | Poor (CPU active) | Good (dedicated logic) | Good (dedicated logic) |
| **Complexity** | Simple code | Medium (registers) | High (sockets) |
| **Best Use Case** | Testing, flexibility needed | High-performance non-secure | Production secure systems |

**Decision Criteria:**
- Need security? → CSU
- Need maximum speed, no security? → FPGA
- Need flexibility, can accept slow? → Software

### Comparison 7: Socket Types (AF_ALG vs Others)

| Socket Type | Purpose | Typical Use |
|-------------|---------|-------------|
| **AF_INET** | Internet Protocol (TCP/IP) | Network communication |
| **AF_UNIX** | Unix domain | Local inter-process communication |
| **AF_ALG** | Algorithm | Kernel cryptographic functions |

**AF_ALG Unique Features:**
- Access to hardware crypto accelerators
- Kernel manages security and permissions
- Standard interface for various algorithms

---

## Examples & Applications

### Example 1: Simple FPGA Encryption (16 bytes)

**Scenario:** Encrypt a single 16-byte message "abcdefghijklmno\\0"

**Code:**
```c
// Test data
uint8_t key[32] = {0x60, 0x3d, 0xeb, 0x10, ...};  // 256-bit key
uint8_t iv[16] = {0x00, 0x01, 0x02, 0x03, ...};   // IV
uint8_t plaintext[16] = "abcdefghijklmno";

// 1. Map FPGA memory
int m_mfd = open("/dev/mem", O_RDWR);
void *base = mmap(NULL, 4096, PROT_READ|PROT_WRITE,
                   MAP_SHARED, m_mfd, 0x80020000);
uint32_t *aes_reg = (uint32_t *)base;

// 2. Load key (8 registers for 256-bit)
uint32_t *key_word = (uint32_t *)key;
for (int i = 0; i < 8; i++) {
    aes_reg[0x10 + i] = key_word[i];
}

// 3. Configure: Encrypt + 256-bit key
aes_reg[0x0A] = 0x03;  // Both bits set

// 4. Init key expansion
aes_reg[0x08] = 0x01;  // INIT bit
while (!(aes_reg[0x09] & 0x01));  // Wait READY

// 5. Load IV
uint32_t *iv_word = (uint32_t *)iv;
for (int i = 0; i < 4; i++) {
    aes_reg[0x40 + i] = iv_word[i];
}

// 6. Load plaintext
uint32_t *plain_word = (uint32_t *)plaintext;
for (int i = 0; i < 4; i++) {
    aes_reg[0x20 + i] = plain_word[i];
}

// 7. Start encryption
aes_reg[0x08] = 0x02;  // NEXT bit
while (!(aes_reg[0x09] & 0x02));  // Wait VALID

// 8. Read ciphertext
for (int i = 0; i < 4; i++) {
    plain_word[i] = aes_reg[0x30 + i];
}

// plaintext now contains encrypted result
printf("Ciphertext: ");
for (int i = 0; i < 16; i++) {
    printf("%02x", plaintext[i]);
}
```

**Expected Output:**
```
Ciphertext: 25972ebbc121fec478d25e7de8c8f91d
```

### Example 2: CSU Socket-Based Encryption

**Scenario:** Same encryption using CSU

**Code:**
```c
// Test data (same as FPGA example)
uint8_t key[32] = {0x60, 0x3d, 0xeb, 0x10, ...};
uint8_t iv[16] = {0x00, 0x01, 0x02, 0x03, ...};
uint8_t plaintext[16] = "abcdefghijklmno";

// 1. Create socket
int sd = socket(AF_ALG, SOCK_SEQPACKET, 0);

// 2. Bind to AES-CBC
struct sockaddr_alg sa = {
    .salg_family = AF_ALG,
    .salg_type = "skcipher",
    .salg_name = "cbc(aes)"
};
bind(sd, (struct sockaddr *)&sa, sizeof(sa));

// 3. Set key
setsockopt(sd, SOL_ALG, ALG_SET_KEY, key, 32);

// 4. Get data channel
int fd = accept(sd, NULL, 0);
close(sd);

// 5. Prepare message
char cbuf[CMSG_SPACE(4) + CMSG_SPACE(20)] = {0};
struct msghdr msg = {};
msg.msg_control = cbuf;
msg.msg_controllen = sizeof(cbuf);

// Set operation
struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
cmsg->cmsg_level = SOL_ALG;
cmsg->cmsg_type = ALG_SET_OP;
cmsg->cmsg_len = CMSG_LEN(4);
*(__u32 *)CMSG_DATA(cmsg) = ALG_OP_ENCRYPT;

// Set IV
cmsg = CMSG_NXTHDR(&msg, cmsg);
cmsg->cmsg_level = SOL_ALG;
cmsg->cmsg_type = ALG_SET_IV;
cmsg->cmsg_len = CMSG_LEN(20);
struct af_alg_iv *ivp = (void *)CMSG_DATA(cmsg);
ivp->ivlen = 16;
memcpy(ivp->iv, iv, 16);

// Set data
struct iovec iov = {plaintext, 16};
msg.msg_iov = &iov;
msg.msg_iovlen = 1;

// 6. Encrypt
sendmsg(fd, &msg, 0);
read(fd, plaintext, 16);
close(fd);

// plaintext now contains encrypted result
```

**Output:** Same as FPGA example

**Key Difference:** CSU requires more setup but provides security

### Example 3: Performance Measurement

**Scenario:** Measure encryption time for comparison

**Code:**
```c
#include <time.h>

struct timespec start, stop;

// ─── FPGA Timing ───
clock_gettime(CLOCK_REALTIME, &start);
AES_FPGA_encrypt_buffer(key, iv, buffer, 16);
clock_gettime(CLOCK_REALTIME, &stop);

double fpga_time = (stop.tv_sec - start.tv_sec) * 1000.0 +
                   (stop.tv_nsec - start.tv_nsec) / 1000000.0;
printf("FPGA Time: %.3f ms\n", fpga_time);

// ─── CSU Timing ───
clock_gettime(CLOCK_REALTIME, &start);
AES_CSU_encrypt_buffer(key, iv, buffer, 16);
clock_gettime(CLOCK_REALTIME, &stop);

double csu_time = (stop.tv_sec - start.tv_sec) * 1000.0 +
                  (stop.tv_nsec - start.tv_nsec) / 1000000.0;
printf("CSU Time: %.3f ms\n", csu_time);

printf("FPGA is %.1fx faster\n", csu_time / fpga_time);
```

**Typical Output:**
```
FPGA Time: 0.035 ms
CSU Time: 0.280 ms
FPGA is 8.0x faster
```

**Analysis:** FPGA faster due to direct hardware access, but CSU provides security

### Example 4: Multi-Block Message Processing

**Scenario:** Encrypt 64-byte message (4 AES blocks)

**Challenge:** Must process in 16-byte chunks for CBC mode

**Code:**
```c
uint8_t message[64] = "This is a longer message that spans multiple AES blocks...";
uint8_t current_iv[16];
memcpy(current_iv, iv, 16);

// Do key expansion once
// ... (setup code)

// Process each block
for (int block = 0; block < 4; block++) {
    uint8_t *current_block = &message[block * 16];

    // Load IV (first block uses provided IV, rest use previous ciphertext)
    load_iv(current_iv);

    // Load and process block
    load_block(current_block);
    start_processing();
    wait_valid();
    read_result(current_block);

    // Save ciphertext as next IV (CBC chaining)
    memcpy(current_iv, current_block, 16);
}
```

**Key Point:** IV for block N is ciphertext from block N-1 (CBC mode)

### Example 5: Error Handling (Wrong Register Access)

**Scenario:** Common mistake with write-only registers

**Wrong Code:**
```c
// MISTAKE: Using read-modify-write on write-only register
uint32_t config = aes_reg[AES_CONFIG_REG];  // Read (gets garbage!)
config |= (1 << CONFIG_ENCDEC_BIT);         // Modify
aes_reg[AES_CONFIG_REG] = config;           // Write (garbage + our bit)

// Result: Undefined behavior, encryption may not work
```

**Correct Code:**
```c
// CORRECT: Build value from scratch, write once
uint32_t config = 0;                        // Start with 0
config |= (1 << CONFIG_ENCDEC_BIT);         // Set encrypt bit
config |= (1 << CONFIG_KEYLEN_BIT);         // Set 256-bit key
aes_reg[AES_CONFIG_REG] = config;           // Write complete value

// Result: Works correctly
```

### Example 6: Secure Key Storage with eFUSE

**Scenario:** Production device needs permanent device-unique key

**Problem:** Keys in registers (FPGA) lost on power-off and readable

**Solution:** Use CSU eFUSE (one-time operation)

**Concept (not actual implementation):**
```c
// During manufacturing/provisioning (one-time):
1. Generate device-unique key
2. Program key into eFUSE (irreversible)
3. Verify key programmed correctly
4. Device can now use key forever

// During normal operation:
1. CSU reads key from eFUSE internally
2. Key never exposed to software or main CPU
3. Encryption/decryption uses protected key
4. Even with physical access, key cannot be read

// Security benefits:
- Key survives power-off (non-volatile)
- Key cannot be extracted (read-protected)
- Tamper detection (triple redundancy)
- Secure boot capability
```

**Use Cases:**
- License verification keys
- Device authentication certificates
- Firmware decryption keys
- Root of trust for secure boot

### Example 7: Comparing Execution Paths

**FPGA Path:**
```
Application Code
      ↓
  mmap() [one-time setup]
      ↓
  Write to registers [~nanoseconds]
      ↓
  FPGA Hardware [microseconds]
      ↓
  Read from registers [~nanoseconds]
      ↓
  Result in buffer

Total: ~0.01-0.1 ms
```

**CSU Path:**
```
Application Code
      ↓
  socket() [create socket]
      ↓
  bind() [configure algorithm]
      ↓
  setsockopt() [set key]
      ↓
  accept() [get data channel]
      ↓
  sendmsg() [marshal data, switch to kernel]
      ↓
  Kernel Crypto API [validate, route]
      ↓
  CSU Driver [format for hardware]
      ↓
  CSU Hardware [microseconds]
      ↓
  Driver → Kernel → User [multiple copies]
      ↓
  read() [unmarshal result]
      ↓
  Result in buffer

Total: ~0.1-1 ms
```

**Analysis:** Multiple abstraction layers add overhead but provide security and portability

---

## Connections to Other Topics

### Connection to Lecture 08: Energy

**Energy Consumption:**
- **Software AES**: CPU continuously active, high energy per operation
- **FPGA AES**: Dedicated logic more efficient, but still powered
- **CSU AES**: Optimized crypto engines, lowest energy per bit

**Measurement Approach:**
- Similar to P2 energy measurement lab
- Could measure current through measurement resistor
- Compare energy cost: Software > FPGA > CSU (per operation)

**Trade-off:**
- FPGA faster but setup cost exists
- CSU socket overhead adds initial energy cost
- For many operations, hardware amortizes setup cost

### Connection to Lecture 09: Performance

**Performance Metrics:**
- **Throughput**: Hardware >> Software (10-100x)
- **Latency**: FPGA lowest, CSU has socket overhead
- **Scalability**: Hardware pipelines for multiple operations

**Bottleneck Analysis:**
- Software: CPU instruction decode/execute
- FPGA: Polling overhead (could use interrupts)
- CSU: Socket/kernel message passing

**Optimization Opportunities:**
- Keep FPGA connection open (single mmap)
- Reuse CSU socket for multiple operations
- Batch process multiple blocks

### Connection to Lecture 10: Introduction FPGA

**FPGA Concepts Applied:**
- Custom logic implementation (AES rounds in hardware)
- Register-based interface (memory-mapped I/O)
- Reconfigurable logic (could load different crypto algorithms)
- Pipeline architecture (parallel round processing)

**Bitstream Configuration:**
- AES block configured at boot time
- Part of FPGA fabric
- Could be reconfigured for different algorithms

### Connection to Lab P3: AES on Multi-Processor

**Comparison:**
- P3: Software AES on APU/RPU
- P4.1: Hardware AES on FPGA
- P4.2: Hardware AES on CSU

**Performance Progression:**
- APU (software): Slowest but flexible
- RPU (software): Similar, different processor
- FPGA (hardware): Much faster, no security
- CSU (hardware): Fast + secure

**Common Elements:**
- Same AES-CBC algorithm
- Same test vectors (NIST)
- Same key/IV/block sizes
- Different implementation approaches

### Connection to Lecture 11-12: DSE (Design Space Exploration)

**Design Space:**
- **Software**: Implementation choice (CPU, algorithm)
- **FPGA**: Logic design choice (area, speed, power)
- **CSU**: System integration choice

**Pareto Front:**
- Security vs Speed: CSU optimal
- Speed vs Flexibility: FPGA optimal
- Flexibility vs Complexity: Software optimal

**Trade-off Analysis:**
```
          Security
              ↑
      CSU ●   │
              │
              │   ● FPGA
              │   ● Software
              └─────────────→ Speed
```

### Connection to Lecture 05: Development Processes

**Testing Requirements:**
- **FPGA**: Hardware-in-loop testing required
- **CSU**: Security validation needed
- Both: Regression testing with known vectors

**V-Model Application:**
- Requirements: AES-256 CBC encryption
- Design: Choose hardware implementation
- Implementation: FPGA/CSU logic + driver
- Testing: Verify against NIST vectors
- Validation: Performance and security verification

### Connection to Lecture 06: Non-Functional Requirements

**NFRs Addressed:**
- **Performance**: Hardware 10-100x faster than software
- **Security**: CSU provides tamper resistance, key protection
- **Real-time**: FPGA deterministic timing
- **Reliability**: CSU triple redundancy
- **Maintainability**: Socket API more maintainable than register access

**Requirements Tracing:**
```
NFR: "Encryption must complete in <1ms"
  → Design Decision: Use hardware acceleration
  → Implementation: FPGA or CSU
  → Verification: Timing measurements
```

### Connection to Lecture 13: RTOS Scheduling

**Interrupt-Driven Approach (potential improvement):**
- Current: Polling READY/VALID bits (busy waiting)
- Alternative: Use interrupts when operation complete
- RTOS could schedule other tasks while waiting

**Priority Inversion:**
- High-priority encryption task waiting for hardware
- Could use priority inheritance with interrupt-driven approach

### Connection to Lecture 14: Multi-Processor Systems

**Multiple Processors in Zynq:**
- **APU (Cortex-A53)**: Runs Linux, manages FPGA/CSU
- **FPGA**: Custom AES logic
- **CSU**: Dedicated security processor

**Communication:**
- APU ↔ FPGA: Memory-mapped registers (AXI)
- APU ↔ CSU: Socket communication through kernel
- Similar to RPMsg in P3 but different interface

**Resource Sharing:**
- FPGA: Could be shared by multiple processes (with proper locking)
- CSU: Kernel manages concurrent access
- Memory: Shared DDR for buffers

### Connection to Lecture 03: Requirements

**Functional Requirements:**
- FR1: Encrypt data using AES-256 CBC mode
- FR2: Support encryption and decryption
- FR3: Support different message sizes

**Derived Requirements:**
- FPGA: Must handle register access
- CSU: Must handle socket communication
- Both: Must validate against NIST test vectors

### Connection to Lecture 04: Modelling

**Component Diagram:**
```
┌─────────────┐         ┌──────────────┐
│ Application │────────→│ AES Hardware │
│  (User)     │ Encrypt │  (FPGA/CSU)  │
└─────────────┘ ←────── └──────────────┘
                Decrypt
```

**State Machine (FPGA AES):**
```
[IDLE] → Load Key → [KEY_LOADED]
       → Configure → [CONFIGURED]
       → Init → [KEY_EXPANDING]
       → Ready → [READY]
       → Load Block → [BLOCK_LOADED]
       → Process → [PROCESSING]
       → Valid → [RESULT_READY]
       → Read → [IDLE]
```

### Connection to Lecture 02: Software Paradigms

**Programming Paradigms:**
- **Procedural**: Step-by-step register writes (FPGA)
- **Object-Oriented**: Could wrap socket/register access in classes
- **Event-Driven**: Interrupt-based (improvement opportunity)

**Abstraction Levels:**
- Low: Direct register manipulation
- Medium: Socket API
- High: Crypto library (using hardware underneath)

### Connection to Lab P2: Energy Measurement

**Similar Measurement Approach:**
- P2: Measured AES energy on Cortex-M4
- P4: Could measure FPGA/CSU energy similarly
- Expected: Hardware more energy-efficient per operation

**Comparison:**
```
Software (P2): High energy, slow
FPGA (P4.1): Medium energy, very fast
CSU (P4.2): Low energy, fast
```

---

## Summary

This lab demonstrates two distinct approaches to hardware-accelerated AES encryption:

**Part 1 - FPGA Implementation:**
- Direct register access via memory-mapped I/O
- Fastest possible performance (~0.01-0.1 ms)
- No security features
- Requires understanding of hardware register map
- Suitable for high-performance, non-security-critical applications

**Part 2 - CSU Implementation:**
- Socket-based access via Linux crypto API
- Fast with security (~0.1-1 ms)
- Protected key storage (eFUSE)
- Standard software interface
- Suitable for production security applications

**Key Learnings:**
1. Memory-mapped I/O allows software to control hardware peripherals
2. mmap() provides safe hardware access in protected operating systems
3. Write-only registers require special handling (no read-modify-write)
4. Sockets abstract hardware details and provide security
5. Hardware acceleration provides massive speedup (10-100x)
6. Security features add overhead but are essential for production
7. Different approaches suit different requirements

**Critical Concepts:**
- Virtual vs physical addresses
- Write-only vs read-only registers
- Polling for hardware completion
- Socket communication model
- Trade-offs between speed and security

**Practical Skills:**
- Using mmap() for hardware access
- Creating and configuring AF_ALG sockets
- Preparing complex message structures
- Measuring performance accurately
- Choosing appropriate implementation for requirements

---

## Possible Exam Questions

### Basic Concepts & Definitions

1. **What is an FPGA and how is it used as a hardware accelerator?**
   - Expected: Reconfigurable logic, custom circuits, configured at boot, acts as peripheral

2. **Explain the difference between virtual and physical addresses in the context of hardware access.**
   - Expected: Virtual from mmap, physical is actual hardware, kernel translates via MMU

3. **What does mmap() do and why is it necessary in Linux?**
   - Expected: Requests kernel permission, creates virtual mapping, required due to MPU

4. **Define memory-mapped I/O (MMIO).**
   - Expected: Hardware registers accessible as memory addresses, read/write to control hardware

5. **What is a write-only register and what special considerations does it require?**
   - Expected: Cannot be read, returns undefined, must write complete value, no |= operations

6. **What is the Configuration Security Unit (CSU)?**
   - Expected: Dedicated security subsystem, SPB + CIB, available at boot, for secure operations

7. **What is eFUSE and why is it important for security?**
   - Expected: One-time programmable, limited writes, cannot read back, permanently stores keys

8. **Explain what AF_ALG means and its purpose.**
   - Expected: Address Family for Algorithms, socket type for cryptographic operations

9. **What is the difference between socket descriptor (sd) and file descriptor (fd) in the CSU implementation?**
   - Expected: sd for configuration, fd for data transfer, fd obtained from accept(sd)

10. **Define "polling" or "busy waiting" in the context of hardware operations.**
    - Expected: Continuously checking status bit until condition met, simple but CPU-intensive

### Register Operations & FPGA

11. **What is the base address of the FPGA AXI interface and what is the offset for the AES block?**
    - Expected: Base 0x8000'0000, AES offset +0x2'0000, total 0x8002'0000

12. **List all register types in the FPGA AES block and their access permissions.**
    - Expected: Control (WO), Status (RO), Config (WO), Key (WO), Block (WO), Result (RO), IV (WO)

13. **What are the two control bits in the Control Register and what do they do?**
    - Expected: INIT (bit 0) starts key expansion, NEXT (bit 1) starts block processing

14. **What are the two status bits in the Status Register and what do they indicate?**
    - Expected: READY (bit 0) = key expansion done, VALID (bit 1) = result ready

15. **What does the Config Register control?**
    - Expected: Encrypt/decrypt mode (bit 0), key length 128/256 bit (bit 1)

16. **How many registers are needed to store a 256-bit AES key and why?**
    - Expected: 8 registers (KEY0-KEY7), each 32 bits, 8×32=256 bits

17. **Why can't you use `*reg |= 0x01` on a write-only register?**
    - Expected: Reading returns undefined value, modifying undefined, writes garbage

18. **Explain the complete sequence of operations to encrypt one block in FPGA.**
    - Expected: Load key, configure, init + wait ready, load IV/block, process + wait valid, read result

19. **What is key expansion and when does it happen in the FPGA?**
    - Expected: Generating round keys from main key, triggered by INIT bit, must wait for READY

20. **How do you know when a hardware operation is complete in FPGA?**
    - Expected: Poll status register for appropriate bit (READY or VALID)

### CSU & Socket Communication

21. **What are the two main components of the CSU and what do they do?**
    - Expected: SPB (triple-redundant processor, protected memory), CIB (crypto engines, eFUSE)

22. **Why does CSU use sockets instead of direct register access?**
    - Expected: Registers not documented for security, prevents manipulation, kernel controls access

23. **List the steps to configure a socket for CSU communication.**
    - Expected: socket() → bind() → setsockopt() → accept() → close(sd)

24. **What information is in the sockaddr_alg structure?**
    - Expected: salg_family (AF_ALG), salg_type (skcipher), salg_name (cbc(aes))

25. **What does "skcipher" mean in the socket configuration?**
    - Expected: Symmetric Key Cipher, same key for encrypt/decrypt

26. **What are the two control headers required in a sendmsg() for CSU?**
    - Expected: ALG_SET_OP (encrypt/decrypt), ALG_SET_IV (initialization vector)

27. **What is the af_alg_iv structure and what does it contain?**
    - Expected: ivlen (IV length, 16 for AES), iv[] (actual IV data array)

28. **How do you send data to the CSU for encryption?**
    - Expected: sendmsg() with control headers (operation, IV) and data buffer (iovec)

29. **How do you receive the encrypted result from CSU?**
    - Expected: read() from file descriptor, result written to same buffer (in-place)

30. **Why do you close(sd) after accept() in CSU implementation?**
    - Expected: Configuration socket (sd) no longer needed, fd handles data transfer

### Performance & Comparisons

31. **Compare the typical execution times for software, FPGA, and CSU AES encryption.**
    - Expected: Software ~1-10ms, FPGA ~0.01-0.1ms (fastest), CSU ~0.1-1ms (middle)

32. **Why is FPGA AES faster than CSU AES?**
    - Expected: Direct register access, no socket overhead, no kernel mediation, immediate hardware access

33. **List three reasons why hardware AES is faster than software AES.**
    - Expected: Parallel operations, no instruction fetch/decode, pipelined processing, dedicated logic, no OS overhead

34. **Compare the security features of FPGA vs CSU.**
    - Expected: FPGA: no security, keys visible; CSU: eFUSE storage, memory isolation, tamper detection

35. **What are the trade-offs between FPGA and CSU implementations?**
    - Expected: FPGA faster but no security, CSU slower but secure, choose based on requirements

36. **Why does FPGA require sudo but CSU doesn't?**
    - Expected: FPGA accesses /dev/mem (physical memory) needs root, CSU uses socket API (standard user)

37. **Compare virtual vs physical address in terms of security.**
    - Expected: Virtual protected by MPU, isolated per process; physical is direct hardware, unprotected

38. **When would you choose FPGA over CSU for AES?**
    - Expected: Maximum performance needed, no security requirements, non-production environment

39. **When would you choose CSU over FPGA for AES?**
    - Expected: Security critical, production device, key protection needed, secure boot required

40. **What is the performance impact of socket overhead in CSU?**
    - Expected: Socket creation, bind, accept add ~0.05-0.5ms overhead, message passing through kernel

### Implementation Details

41. **Write the correct way to set the encrypt bit in the Config register (assume bit 0).**
    - Expected: `uint32_t config = 0; config |= (1 << 0); aes_reg[CONFIG] = config;`

42. **Write the polling loop to wait for key expansion to complete.**
    - Expected: `while (!(aes_reg[STATUS] & (1 << READY_BIT)));`

43. **How do you load a 256-bit key into the FPGA key registers?**
    - Expected: Cast key to uint32_t*, loop 8 times, write to registers 0x10-0x17

44. **What is the size of the control buffer for CSU message headers?**
    - Expected: CMSG_SPACE(4) + CMSG_SPACE(20) ≈ 64-80 bytes depending on platform

45. **How do you access the second control message header in a msghdr?**
    - Expected: `cmsg = CMSG_NXTHDR(&msg, cmsg);`

46. **What is the difference between CMSG_SPACE() and CMSG_LEN()?**
    - Expected: CMSG_SPACE includes alignment padding, CMSG_LEN is just header + data

47. **How do you handle multi-block messages in FPGA (e.g., 64 bytes)?**
    - Expected: Loop 4 times for 4 blocks, key expansion once, IV for block N is ciphertext from N-1 (CBC)

48. **What happens to the input buffer after encryption in both FPGA and CSU?**
    - Expected: In-place encryption, result overwrites input, original data destroyed

49. **How would you measure the execution time of encryption?**
    - Expected: clock_gettime(CLOCK_REALTIME) before and after, calculate difference in ms

50. **What needs to be done before the first FPGA encryption can start?**
    - Expected: mmap() memory, load key, configure, init key expansion, wait ready

### Memory & Addressing

51. **Calculate the physical address of the AES Result Register 0 (RES0).**
    - Expected: Base 0x8000'0000 + offset 0x2'0000 + register 0x30 = 0x8002'0030

52. **What is the size of one AES block and how many registers does it occupy?**
    - Expected: 16 bytes = 128 bits, occupies 4 registers (each 32 bits)

53. **How many bytes can be stored in registers KEY0-KEY7 total?**
    - Expected: 8 registers × 4 bytes = 32 bytes = 256 bits

54. **What is the purpose of the page size parameter in mmap()?**
    - Expected: Specifies how much memory to map, typically 4096 bytes (one page)

55. **Why doesn't the virtual address equal the physical address after mmap()?**
    - Expected: Kernel assigns arbitrary virtual address, MMU translates to physical transparently

### Security & CSU Features

56. **Explain how triple redundancy in CSU's SPB enhances security.**
    - Expected: Three processors run same computation, compare results, detects tampering or faults

57. **What happens if all eFUSE write cycles are exhausted?**
    - Expected: Device cannot write new keys, unusable for security purposes, permanent limitation

58. **List three security advantages of CSU over FPGA.**
    - Expected: Protected key storage, memory isolation, tamper detection, secure boot capability

59. **How does CSU support secure boot?**
    - Expected: Verifies bootloader signature (RSA), decrypts bootloader (AES), ensures only authentic code

60. **Why can't eFUSE keys be read back even by software?**
    - Expected: Hardware design prevents readback, keys only usable by crypto engines internally

61. **What is the security risk of using FPGA for encryption?**
    - Expected: Keys visible in memory, no tamper protection, keys lost on power-off, vulnerable to attacks

62. **How does kernel crypto API enhance security?**
    - Expected: Validates access, enforces permissions, prevents unauthorized hardware access

63. **Why is CSU available from boot time?**
    - Expected: Required for secure boot, must verify bootloader before OS starts

64. **What is "security through obscurity" in the context of CSU?**
    - Expected: Not documenting register details publicly, but not sole security measure

65. **How does memory isolation in CSU protect against attacks?**
    - Expected: CSU memory separate from main system, keys never exposed to main CPU

### Conceptual Understanding

66. **Explain the concept of "in-place encryption."**
    - Expected: Result overwrites input buffer, same buffer for input and output, saves memory

67. **What is the difference between key expansion and encryption rounds?**
    - Expected: Key expansion generates round keys (done once), rounds use those keys to encrypt (done per block)

68. **Why is polling inefficient and what's a better alternative?**
    - Expected: CPU busy waiting wastes cycles, interrupts better (sleep until hardware done)

69. **Explain why hardware can perform AES faster than software.**
    - Expected: Parallel logic gates, no instruction overhead, dedicated circuits, pipelined stages

70. **What does "hardware acceleration" mean in general?**
    - Expected: Using dedicated hardware for specific tasks instead of general-purpose CPU

71. **Explain the relationship between AXI interface and FPGA peripherals.**
    - Expected: AXI is communication protocol, connects CPU to FPGA logic, defines addressing

72. **What is the purpose of the initialization vector (IV) in CBC mode?**
    - Expected: Randomizes first block encryption, prevents pattern detection, subsequent blocks use previous ciphertext

73. **Why must you wait for READY bit before encrypting?**
    - Expected: Key expansion must complete, round keys must be ready before encryption can start

74. **Why must you wait for VALID bit before reading result?**
    - Expected: Encryption takes time (multiple rounds), reading early gets incomplete/wrong result

75. **Explain message marshalling in the context of CSU sockets.**
    - Expected: Converting data to standard format for transmission, includes headers and payload

### Debugging & Troubleshooting

76. **What would happen if you forgot to wait for READY bit in FPGA?**
    - Expected: Encryption starts before key expansion done, uses incomplete keys, wrong result

77. **What error occurs if you try to encrypt without setting the key?**
    - Expected: Undefined behavior, encryption uses uninitialized key, garbage output

78. **Why might CSU socket creation fail?**
    - Expected: Algorithm not available in kernel, insufficient permissions, kernel module not loaded

79. **What happens if you pass wrong key length to setsockopt()?**
    - Expected: Error (errno = EINVAL), socket not configured, encryption fails

80. **How do you verify encryption is working correctly?**
    - Expected: Compare output against NIST test vectors with known key/IV/plaintext

81. **What could cause mmap() to fail?**
    - Expected: Not running as root, wrong address, /dev/mem not accessible, permissions denied

82. **Why might FPGA encryption return wrong results?**
    - Expected: Wrong key, forgot to wait for status bits, read-modify-write on write-only register

83. **What debugging approach would you use for CSU socket errors?**
    - Expected: Check errno values, verify algorithm available (/proc/crypto), use strerror()

84. **How can you tell if the AES block is properly mapped?**
    - Expected: Check mmap return for MAP_FAILED, try reading status register, verify access works

85. **What happens if message size is not multiple of 16 bytes?**
    - Expected: AES requires 16-byte blocks, will fail or need padding to multiple of 16

### System Architecture

86. **Draw the memory map showing DDR, AXI base, and AES block.**
    - Expected: Diagram with addresses: 0x0 (DDR), 0x8000'0000 (AXI), 0x8002'0000 (AES)

87. **Describe the communication path from application to FPGA AES hardware.**
    - Expected: App → mmap() → virtual address → MMU translation → physical address → AXI → FPGA

88. **Describe the communication path from application to CSU AES hardware.**
    - Expected: App → socket API → kernel crypto API → CSU driver → CSU hardware

89. **What processors/components are involved in the complete Zynq UltraScale+ system?**
    - Expected: APU (Cortex-A53), RPU (Cortex-R5), FPGA, CSU, DDR controller, peripherals

90. **How does Linux Memory Protection Unit (MPU) affect hardware access?**
    - Expected: Blocks direct physical memory access, requires mmap() for permission, enforces security

### Design Decisions & Trade-offs

91. **What factors would you consider when choosing between FPGA and CSU for a project?**
    - Expected: Performance requirements, security needs, cost, development time, production vs prototype

92. **Why might you keep the socket open for multiple operations in CSU?**
    - Expected: Amortize socket setup overhead, improve throughput, reduce latency

93. **What are the advantages and disadvantages of in-place encryption?**
    - Expected: Advantage: saves memory; Disadvantage: destroys original data, no recovery

94. **How does CBC mode affect multi-block processing?**
    - Expected: Sequential (not parallel), IV for block N is ciphertext N-1, must process in order

95. **What improvements could be made to reduce FPGA polling overhead?**
    - Expected: Use interrupts instead of polling, sleep/wake instead of busy wait

96. **Why is socket overhead acceptable for CSU despite being slower?**
    - Expected: Security benefits outweigh performance cost, still much faster than software

97. **What happens to development complexity as you move from software to hardware?**
    - Expected: Increases (hardware knowledge needed, register details, timing, setup)

98. **How would you optimize for power consumption in hardware AES?**
    - Expected: Use CSU (most efficient), clock gating when idle, minimize unnecessary operations

99. **What testing strategy would you use for hardware AES implementation?**
    - Expected: NIST test vectors, different message sizes, encrypt + decrypt verification, performance measurement

100. **If you had to explain to a manager why CSU is worth the complexity over FPGA, what would you say?**
     - Expected: Production security requirements (eFUSE, tamper detection, secure boot), regulatory compliance, customer trust, long-term key protection, prevents device cloning

---

## Additional Resources

**Documentation:**
- Xilinx UG1137: Zynq UltraScale+ MPSoC Software Developer Guide
- Linux Crypto API Documentation
- AF_ALG Socket Interface Documentation
- NIST AES Specification (FIPS 197)

**System Calls:**
- mmap(2) man page
- socket(2) man page
- bind(2) man page
- sendmsg(2) man page

**Key Files:**
- `/dev/mem` - Physical memory access
- `/proc/crypto` - Available kernel crypto algorithms

**Build Commands:**
```bash
# Part 1 - FPGA
cd 06_P4/P4.1_AES_in_FPGA
make
make install
sudo ./fpga.elf

# Part 2 - CSU
cd 06_P4/P4.2_AES_in_CSU
make
make install
./csu.elf  # No sudo needed!
```

**Important Notes:**
- FPGA requires root (sudo) for /dev/mem access
- CSU does not require root (standard socket API)
- Both use same NIST test vectors for verification
- Expected performance: FPGA fastest, CSU fast + secure, software slow
