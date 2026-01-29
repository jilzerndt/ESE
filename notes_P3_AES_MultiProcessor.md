# Lab P3: AES Algorithm on ARM Cortex-A53 and ARM Cortex-R5

## Technical Terms & Definitions

### Encryption & AES

**AES (Advanced Encryption Standard)**: Symmetric block cipher encryption algorithm selected by NIST in 2001 to replace DES. Features:
- Symmetric: Same key for encryption and decryption
- Block cipher: Operates on fixed-size blocks (128 bits = 16 bytes)
- Variable key length: 128, 192, or 256 bits
- This lab uses AES-256 (256-bit key = 32 bytes)

**DES (Data Encryption Standard)**: Older encryption algorithm with 56-bit key, cracked in 56 hours in 1998, now crackable in ~5 minutes due to increased parallel computing power.

**Brute-Force Attack**: Attempting all possible keys to decrypt data without knowing the correct key. AES-256 makes this computationally infeasible (2^256 possible keys).

**Symmetric Encryption**: Encryption method where the same key is used for both encryption and decryption (unlike asymmetric/public-key cryptography).

**Block Cipher**: Encryption algorithm that operates on fixed-size blocks of data (AES uses 16-byte blocks).

**Plaintext**: Unencrypted, readable data before encryption.

**Ciphertext**: Encrypted, unreadable data after encryption.

**Key**: Secret value used by the encryption algorithm to transform plaintext to ciphertext and vice versa.

**Initialization Vector (IV)**: Random starting value used in CBC mode to ensure identical plaintexts produce different ciphertexts. Must be:
- Unpredictable
- Unique for each encryption session
- 16 bytes for AES (one block size)

### AES Modes

**CBC (Cipher Block Chaining)**: AES encryption mode where:
- Each plaintext block is XORed with the previous ciphertext block before encryption
- First block is XORed with the Initialization Vector (IV)
- Creates dependency chain: each ciphertext block depends on all previous plaintext blocks
- **Encryption**: Must be sequential (single-threaded) due to dependencies
- **Decryption**: Can be parallelized (only needs key and two adjacent ciphertext blocks)

**XOR (Exclusive OR)**: Bitwise operation where output is 1 if inputs differ, 0 if same:
```
0 XOR 0 = 0
0 XOR 1 = 1
1 XOR 0 = 1
1 XOR 1 = 0
```
Used in CBC to chain blocks together.

### Multi-Processor Architecture

**Xilinx Zynq UltraScale+ MPSoC**: Multi-processor system-on-chip containing:
- **APU (Application Processing Unit)**: 4× ARM Cortex-A53 cores
- **RPU (Realtime Processing Unit)**: 2× ARM Cortex-R5 cores
- **GPU (Graphics Processing Unit)**: ARM Mali-400 MP2
- Shared memory and peripherals

**ARM Cortex-A53**: 64-bit application processor core designed for:
- High performance
- Running complex operating systems (Linux, Android)
- General-purpose computing
- Higher clock speeds (typically 1.2-1.5 GHz on ZynqMP)

**ARM Cortex-R5**: 32-bit real-time processor core designed for:
- Deterministic real-time performance
- Low latency
- Safety-critical applications
- Lower clock speeds (typically 500-600 MHz on ZynqMP)

**APU (Application Processing Unit)**: Processor subsystem running Linux, handling:
- User applications
- Operating system services
- Network stack, file system
- Non-real-time tasks

**RPU (Realtime Processing Unit)**: Processor subsystem running RTOS, handling:
- Hard real-time tasks
- Deterministic operations
- Low-latency control loops
- Safety-critical functions

**Heterogeneous Multi-Processing**: System with different types of processors working together, each optimized for specific tasks (A53 for applications, R5 for real-time).

### Remote Procedure Calls

**Remote Procedure Call (RPC)**: Function executed on a separate processor from the caller. Requires:
- Setup: Loading firmware on remote processor
- Communication: Transferring input and output data
- Notification: Informing caller when execution completes

**RPMsg (Remote Processor Messaging)**: Linux kernel framework for communication between processors:
- Message-based communication
- Endpoint concept (channels)
- Built on virtio transport layer
- Enables user-space access via character devices

**RPMsg Virtio**: Virtual I/O device providing transport layer for RPMsg, using shared memory for efficient inter-processor communication.

**Remoteproc (Remote Processor)**: Linux kernel framework for:
- Loading firmware onto remote processors
- Starting and stopping remote processors
- Managing lifecycle of remote cores
- Providing control interface via sysfs (`/sys/class/remoteproc/`)

**Firmware**: Binary executable loaded onto the remote processor (R5), containing:
- Application code (AES algorithm)
- RTOS (FreeRTOS)
- RPMsg handlers
- Resource table (memory mapping)

**FreeRTOS**: Real-Time Operating System providing:
- Task scheduling
- Interrupt management
- Minimal memory footprint
- Deterministic behavior
- Runs on R5 in this lab

**Callback Function**: Function passed as argument to another function, to be called when an event occurs (e.g., when RPMsg receives data).

**Resource Table**: Data structure in firmware describing:
- Memory regions
- Communication channels
- Peripheral access
- Required by remoteproc framework

**Name Service**: Protocol step where remote processor announces its communication channels to the master processor.

**Endpoint**: Communication channel in RPMsg, like a socket, with:
- Source address
- Destination address
- Message handler callback

**Master Processor**: Processor controlling the remote processor (A53 with Linux in this lab).

**Remote Processor**: Processor being controlled (R5 with FreeRTOS in this lab).

### Linux/System Concepts

**Device Tree**: Data structure describing hardware configuration, presented as:
- Virtual filesystem at `/sys/`
- Allows querying and controlling hardware
- Used to access remoteproc devices

**sysfs**: Virtual filesystem in Linux at `/sys/` exposing kernel objects and device information.

**Character Device**: Device accessed through file operations (read/write), appears in `/dev/`. RPMsg creates `/dev/rpmsgX` devices.

**Kernel Module**: Loadable code extending kernel functionality (e.g., rpmsg_char driver).

**Superuser (root/sudo)**: Administrator account with unrestricted system access, required for:
- Loading firmware
- Controlling remote processors
- Accessing remoteproc sysfs interface

**clock_gettime()**: Linux system call for high-precision time measurement:
- Nanosecond resolution
- Various clock types (REALTIME, MONOTONIC, etc.)
- Returns timespec struct with seconds and nanoseconds

**CLOCK_REALTIME**: Wall-clock time, can be adjusted by system administrator, suitable for measuring absolute time.

**timespec**: Structure containing:
```c
struct timespec {
    time_t tv_sec;   // seconds
    long tv_nsec;    // nanoseconds
};
```

### Communication Protocol

**RPMsg Message Structure (aes_datatype)**:
```c
struct _aes_datatype {
    unsigned char dec;          // 1 byte: 0=encrypt, 1=decrypt
    unsigned char key[32];      // 32 bytes: AES-256 key
    unsigned char iv[16];       // 16 bytes: Initialization vector
    unsigned char text_length;  // 1 byte: Length of text
    unsigned char text[];       // Variable: Plaintext/ciphertext
};
```
Total fixed overhead: 50 bytes + text

**Shutdown Message**: Special message (0xEF56A55A) sent to gracefully terminate remote procedure.

**File Descriptor**: Integer representing an open file or device, used with read/write operations.

**write() System Call**: Sends data to a file descriptor (used to send RPMsg).

**read() System Call**: Receives data from a file descriptor (used to receive RPMsg results).

### Performance Measurement

**Timestamp**: Recorded time at a specific point in program execution, used to calculate elapsed time.

**Execution Time**: Total time from start to completion of a function or code section.

**Communication Overhead**: Additional time required for inter-processor communication beyond the actual computation time.

**Setup Overhead**: One-time initialization cost (loading firmware, establishing channel) before remote procedure can be used.

**Throughput**: Amount of data processed per unit time (e.g., bytes/second of encryption).

**Latency**: Time delay between initiating an operation and receiving the result.

---

## Core Concepts

### AES-256 CBC Mode Encryption

**How CBC Mode Works:**

**Encryption Process:**
1. **First Block:**
   - Take first 16-byte plaintext block (P₁)
   - XOR with Initialization Vector: `P₁ ⊕ IV`
   - Encrypt with AES: `C₁ = AES_Encrypt(P₁ ⊕ IV, Key)`

2. **Subsequent Blocks:**
   - Take next plaintext block (Pᵢ)
   - XOR with previous ciphertext: `Pᵢ ⊕ Cᵢ₋₁`
   - Encrypt: `Cᵢ = AES_Encrypt(Pᵢ ⊕ Cᵢ₋₁, Key)`

3. **Chaining Effect:**
   - Each ciphertext depends on all previous plaintext blocks
   - Identical plaintext blocks produce different ciphertexts
   - Prevents pattern detection in ciphertext

**Decryption Process:**
1. **Any Block:**
   - Decrypt ciphertext: `Temp = AES_Decrypt(Cᵢ, Key)`
   - XOR with previous ciphertext: `Pᵢ = Temp ⊕ Cᵢ₋₁`
   - First block uses IV: `P₁ = AES_Decrypt(C₁, Key) ⊕ IV`

2. **Parallelization Opportunity:**
   - Decryption only needs Cᵢ and Cᵢ₋₁
   - Can decrypt multiple blocks simultaneously
   - No dependency chain like encryption

**Why CBC Over Simpler Modes:**
- **ECB (Electronic Codebook)** Problem: Same plaintext always produces same ciphertext
  - Reveals patterns in data
  - Security vulnerability
- **CBC Solution**: Chaining ensures identical plaintexts produce different ciphertexts
  - No 1:1 plaintext-ciphertext relationship
  - More secure

**AES Context Initialization:**
```c
AES_init_ctx_iv(&ctx, key, iv);
```
This function:
1. **Key Expansion**: Converts 256-bit key into round keys for AES rounds
2. **IV Setup**: Stores initialization vector for CBC mode
3. **Context Creation**: Prepares structure for encryption/decryption operations

### Remote Procedure Call Architecture

**System Architecture:**
```
┌─────────────────────────────────────────┐
│   Linux (Cortex-A53)                    │
│                                         │
│   ┌─────────────────────────────────┐   │
│   │   User Application (main.c)     │   │
│   │   - Prepares data               │   │
│   │   - Calls AES_RPU_encrypt()     │   │
│   └───────────┬─────────────────────┘   │
│               │                         │
│   ┌───────────▼─────────────────────┐   │
│   │   Linux Kernel                  │   │
│   │   ┌──────────┐  ┌────────────┐  │   │
│   │   │ rpmsg    │  │ remoteproc │  │   │
│   │   │ driver   │  │ framework  │  │   │
│   │   └──────────┘  └────────────┘  │   │
│   └───────────┬─────────────────────┘   │
└───────────────┼─────────────────────────┘
                │ Shared Memory (virtio)
┌───────────────▼─────────────────────────┐
│   FreeRTOS (Cortex-R5)                  │
│                                         │
│   ┌─────────────────────────────────┐   │
│   │   RPMsg Handler                  │   │
│   │   - Receives aes_datatype        │   │
│   │   - Calls rpmsg_endpoint_cb()    │   │
│   └───────────┬─────────────────────┘   │
│               │                         │
│   ┌───────────▼─────────────────────┐   │
│   │   AES Implementation             │   │
│   │   - AES_CBC_encrypt_buffer()     │   │
│   │   - AES_CBC_decrypt_buffer()     │   │
│   └─────────────────────────────────┘   │
└─────────────────────────────────────────┘
```

**Complete RPC Lifecycle:**

**1. Setup Phase (One-time, in AES_RPU_start()):**
```
Master (A53)                          Remote (R5)
    │                                     │
    │ 1. Write firmware name              │
    │    to /sys/.../firmware             │
    │                                     │
    │ 2. Write "start" to                 │
    │    /sys/.../state                   │
    ├──────────────────────────────────→  │
    │                                     │ 3. R5 boots, loads firmware
    │                                     │ 4. FreeRTOS starts
    │                                     │ 5. RPMsg initializes
    │                                     │
    │  ←────────── Name Service ────────  │
    │              Announcement           │
    │                                     │
    │ 6. Create callback endpoint         │
    │                                     │
    │  ─────── Name Service ACK ───────→  │
    │                                     │
    │ 7. Load rpmsg_char driver           │
    │ 8. Open /dev/rpmsgX                 │
    │                                     │
    └──── Channel Ready for Use ──────────┘
```

**2. Execution Phase (Per encryption call):**
```
Application                           Remote Procedure
    │                                     │
    │ Prepare aes_datatype:               │
    │ - Set dec flag (0=encrypt)          │
    │ - Copy 256-bit key                  │
    │ - Copy 16-byte IV                   │
    │ - Copy plaintext                    │
    │                                     │
    │ write(fd, data, size)               │
    ├──────────────────────────────────→  │
    │                                     │
    │                                     │ rpmsg_endpoint_cb() called
    │                                     │ - Parse aes_datatype
    │                                     │ - Init AES context
    │                                     │ - Encrypt/decrypt
    │                                     │
    │  ←───────── Result (text) ────────  │
    │                                     │
    │ read(fd, result, size)              │
    │                                     │
    └─────────────────────────────────────┘
```

**3. Shutdown Phase (in AES_RPU_stop()):**
```
Application                           Remote Procedure
    │                                     │
    │ Send SHUTDOWN_MSG                   │
    ├──────────────────────────────────→  │
    │                                     │
    │                                     │ Recognize shutdown
    │                                     │ Clean up resources
    │                                     │
    │ close(fd)                           │
    │ sleep(2) - graceful shutdown        │
    │                                     │
    │ Write "stop" to                     │
    │ /sys/.../state                      │
    ├──────────────────────────────────→  │
    │                                     │ R5 halts
    │                                     │
    └─────────────────────────────────────┘
```

### Implementation on Cortex-A53 (APU)

**File Structure:**
- `APU/src/main.c`: Main application, timing measurements
- `APU/src/aes_apu.c`: AES wrapper functions for A53
- `APU/src/aes.c`: Core AES algorithm implementation

**Key Functions:**

**1. AES_APU_init_ctx_iv(key, iv):**
```c
void AES_APU_init_ctx_iv(uint8_t *key, uint8_t *iv);
```
- **Purpose**: Initialize AES context for encryption/decryption
- **Steps**:
  1. Key expansion: Generate round keys from 256-bit key
  2. IV setup: Store initialization vector for CBC mode
- **Must be called** before encryption/decryption

**2. AES_APU_encrypt_buffer(buffer, length):**
```c
void AES_APU_encrypt_buffer(uint8_t *buffer, uint32_t length);
```
- **Purpose**: Encrypt data in-place using CBC mode
- **Requirements**:
  - Context must be initialized
  - Length must be multiple of 16 bytes
- **Behavior**: Buffer contents replaced with ciphertext

**3. AES_APU_decrypt_buffer(buffer, length):**
```c
void AES_APU_decrypt_buffer(uint8_t *buffer, uint32_t length);
```
- **Purpose**: Decrypt data in-place using CBC mode
- **Behavior**: Buffer contents replaced with plaintext

**Timing Measurement Strategy:**
```c
struct timespec time_start, time_stop;

// Start timing BEFORE initialization
clock_gettime(CLOCK_REALTIME, &time_start);

// Initialize AES context (part of measured work)
AES_APU_init_ctx_iv(key, iv);

// Perform encryption (main measured work)
AES_APU_encrypt_buffer(buffer, length);

// Stop timing AFTER encryption completes
clock_gettime(CLOCK_REALTIME, &time_stop);

// Calculate elapsed time
long seconds = time_stop.tv_sec - time_start.tv_sec;
long nanoseconds = time_stop.tv_nsec - time_start.tv_nsec;
double elapsed = seconds + nanoseconds*1e-9;
```

**What to Measure:**
- Include initialization: Key expansion is part of the work
- Include encryption: The main computation
- Exclude: I/O, setup, test validation

### Implementation on Cortex-R5 (RPU)

**File Structure:**
- `RPU/src/main.c`: Main application on A53 side
- `RPU/src/aes_rpu.c`: RPC setup/teardown, communication
- `R5_firmware/src/rpmsg_aes.c`: Firmware running on R5
- `R5_firmware/src/aes.c`: Core AES algorithm

**APU Side Functions:**

**1. AES_RPU_start(firmware_name):**
```c
int AES_RPU_start(const char *firmware);
```
**Purpose**: Initialize remote processor and establish communication

**Implementation:**
```c
// 1. Specify firmware
FILE *fp = fopen("/sys/class/remoteproc/remoteproc0/firmware", "w");
fprintf(fp, "%s", firmware);  // Write "aes_rpu_rtos.elf"
fclose(fp);

// 2. Start remote processor
fp = fopen("/sys/class/remoteproc/remoteproc0/state", "w");
fprintf(fp, "start");
fclose(fp);

// 3. Load RPMsg character device driver
system("modprobe rpmsg_char");

// 4. Wait for and find the RPMsg channel
// (channel appears in /sys/bus/rpmsg/devices/)

// 5. Bind channel to rpmsg_chrdev driver
// 6. Open rpmsg_ctrl device
// 7. Create endpoint (ioctl calls)
// 8. Open /dev/rpmsgX device

return fd;  // File descriptor for communication
```

**What It Does:**
- Loads firmware into R5 memory
- Starts R5 processor executing FreeRTOS
- Establishes RPMsg communication channel
- Returns file descriptor for sending/receiving messages

**2. AES_RPU_encrypt_buffer(key, iv, buffer, length):**
```c
void AES_RPU_encrypt_buffer(uint8_t *key, uint8_t *iv,
                             uint8_t *buffer, uint32_t length);
```

**Implementation:**
```c
// 1. Prepare message structure
aes_datatype *aes_data = malloc(50 + length);
aes_data->dec = 0;  // 0 = encrypt, 1 = decrypt
memcpy(aes_data->key, key, 32);
memcpy(aes_data->iv, iv, 16);
aes_data->text_length = length;
memcpy(aes_data->text, buffer, length);

// 2. Send to R5
size_t data_size = 50 + length;
write(fd_glob, aes_data, data_size);

// 3. Wait for and receive result
read(fd_glob, buffer, length);  // Result written back to buffer

free(aes_data);
```

**Communication Details:**
- **Blocking write**: Sends data, waits for R5 to receive
- **Blocking read**: Waits for R5 to send result back
- **In-place operation**: Result overwrites input buffer

**3. AES_RPU_stop(firmware_name):**
```c
int AES_RPU_stop(const char *firmware);
```

**Implementation:**
```c
// 1. Send shutdown message
uint32_t shutdown_msg = SHUTDOWN_MSG;  // 0xEF56A55A
write(fd_glob, &shutdown_msg, sizeof(shutdown_msg));

// 2. Close communication channel
close(fd_glob);

// 3. Wait for graceful shutdown
sleep(2);  // Important! Let R5 clean up

// 4. Stop remote processor
FILE *fp = fopen("/sys/class/remoteproc/remoteproc0/state", "w");
fprintf(fp, "stop");
fclose(fp);

return 0;
```

**Why sleep(2)?**
- R5 needs time to process shutdown message
- Ensures all pending messages are handled
- Prevents abrupt termination mid-operation
- Graceful resource cleanup

**RPU Side (Firmware):**

**Callback Function:**
```c
int rpmsg_endpoint_cb(struct rpmsg_endpoint *ept, void *data,
                      size_t len, uint32_t src, void *priv)
{
    // 1. Check for shutdown
    if (*((int *)data) == SHUTDOWN_MSG) {
        // Shutdown requested
        return 0;
    }

    // 2. Parse incoming data
    aes_datatype *aes_data = (aes_datatype *)data;

    // 3. Initialize AES context
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, aes_data->key, aes_data->iv);

    // 4. Encrypt or decrypt based on flag
    if (aes_data->dec) {
        AES_CBC_decrypt_buffer(&ctx, aes_data->text,
                               aes_data->text_length);
    } else {
        AES_CBC_encrypt_buffer(&ctx, aes_data->text,
                               aes_data->text_length);
    }

    // 5. Send result back to master
    rpmsg_send(ept, aes_data->text, aes_data->text_length);

    return 0;
}
```

**Execution Flow:**
1. RPMsg framework calls callback when message arrives
2. Callback runs in FreeRTOS task context on R5
3. Performs AES operation
4. Sends result back through same channel
5. Returns (callback complete, ready for next message)

### Performance Measurement Strategy

**What to Measure on APU:**
```c
clock_gettime(CLOCK_REALTIME, &time_start);
AES_APU_init_ctx_iv(key, iv);          // Include
AES_APU_encrypt_buffer(buffer, length); // Include
clock_gettime(CLOCK_REALTIME, &time_stop);
// Exclude: Verification, printing, test setup
```

**What to Measure on RPU:**
```c
// Do NOT time setup
AES_RPU_start(firmware);  // One-time overhead

// Time only the encryption
clock_gettime(CLOCK_REALTIME, &time_start);
AES_RPU_encrypt_buffer(key, iv, buffer, length);
clock_gettime(CLOCK_REALTIME, &time_stop);

// Do NOT time teardown
AES_RPU_stop(firmware);  // Includes sleep(2)
```

**Why Different Strategies?**
- **APU**: Initialization is part of per-encryption cost
- **RPU**: Setup is one-time, amortized over many calls
- **sleep(2)** in stop: Not part of encryption performance
- Goal: Compare actual encryption performance fairly

**Multiple Measurements:**
- Run each test ≥5 times
- Calculate average execution time
- Record results for three message sizes:
  - 16 bytes (1 AES block, minimum)
  - 64 bytes (4 AES blocks, medium)
  - 240 bytes (15 AES blocks, large)

**Expected Performance Factors:**

**A53 Advantages:**
- Higher clock speed (~1.2 GHz vs ~500 MHz)
- Better branch prediction
- Larger caches

**R5 Advantages:**
- Dedicated processor (no OS interference)
- Deterministic performance
- Lower latency interrupts

**RPU Disadvantages:**
- Communication overhead (50-byte message + data copying)
- Setup/teardown cost for small messages
- Shared memory bandwidth limits

**Typical Results:**
- Small messages (16 bytes): **A53 faster** (RPU overhead dominates)
- Large messages (240 bytes): **Could be either** (computation vs overhead tradeoff)
- Consistency: **R5 more consistent** (no OS scheduling variability)

---

## Important Diagrams & Graphics

### 1. **CBC Encryption Flow Diagram (Page 2)**
```
Plaintext:      P₁          P₂          P₃          Pₙ
                │           │           │           │
         IV ───XOR   ┌─────XOR   ┌─────XOR   ┌─────XOR
                │    │      │    │      │    │      │
                ▼    │      ▼    │      ▼    │      ▼
            ┌───────┐│  ┌───────┐│  ┌───────┐│  ┌───────┐
         K─→│Encrypt││K→│Encrypt││K→│Encrypt││K→│Encrypt│
            └───┬───┘│  └───┬───┘│  └───┬───┘│  └───┬───┘
                │    │      │    │      │    │      │
Ciphertext:    C₁───┘     C₂───┘     C₃───┘     Cₙ
```
**Description**: CBC encryption chain. Each plaintext block is XORed with the previous ciphertext (or IV for first block), then encrypted. Shows dependency chain preventing parallelization. Draw with XOR symbols (⊕), encryption boxes, and data flow arrows showing how Cᵢ₋₁ feeds into next block.

### 2. **CBC Decryption Flow Diagram (Page 2)**
```
Ciphertext:     C₁          C₂          C₃          Cₙ
                │           │           │           │
                ├───────┐   ├───────┐   ├───────┐   │
                ▼       │   ▼       │   ▼       │   ▼
            ┌───────┐  │┌───────┐  │┌───────┐  │┌───────┐
         K─→│Decrypt│  ││Decrypt│  ││Decrypt│  ││Decrypt│
            └───┬───┘  │└───┬───┘  │└───┬───┘  │└───┬───┘
                │      │    │      │    │      │    │
         IV ───XOR     └───XOR     └───XOR  Cₙ₋₁──XOR
                │           │           │           │
                ▼           ▼           ▼           ▼
Plaintext:     P₁          P₂          P₃          Pₙ
```
**Description**: CBC decryption. Each ciphertext block is decrypted, then XORed with previous ciphertext (or IV for first block). Shows independence - can decrypt blocks in parallel since each only needs Cᵢ and Cᵢ₋₁. Draw with parallel paths possible.

### 3. **Multi-Processor System Architecture (Page 3)**
```
┌────────────────────────────────────────────────────────┐
│              Xilinx Zynq UltraScale+ MPSoC             │
│                                                        │
│  ┌──────────────────────┐  ┌──────────────────────┐   │
│  │   APU (Cortex-A53)   │  │   RPU (Cortex-R5)    │   │
│  │   ┌──────────────┐   │  │   ┌──────────────┐   │   │
│  │   │   Core 0     │   │  │   │   Core 0     │   │   │
│  │   │   (Linux)    │   │  │   │  (FreeRTOS)  │   │   │
│  │   └──────────────┘   │  │   └──────────────┘   │   │
│  │   ┌──────────────┐   │  │   ┌──────────────┐   │   │
│  │   │   Cores 1-3  │   │  │   │   Core 1     │   │   │
│  │   │              │   │  │   │  (unused)    │   │   │
│  │   └──────────────┘   │  │   └──────────────┘   │   │
│  │                      │  │                      │   │
│  │   L1 Cache (32KB)    │  │   L1 Cache (32KB)    │   │
│  │   L2 Cache (1MB)     │  │   TCM (128KB)        │   │
│  └──────────┬───────────┘  └──────────┬───────────┘   │
│             │                          │               │
│             └──────────┬───────────────┘               │
│                        │                               │
│          ┌─────────────▼──────────────┐                │
│          │   Shared DDR Memory        │                │
│          │   (2GB, for communication) │                │
│          └────────────────────────────┘                │
│                                                        │
└────────────────────────────────────────────────────────┘
```
**Description**: ZynqMP architecture showing APU (4× A53) and RPU (2× R5) with separate caches but shared memory for inter-processor communication. This lab uses A53 Core 0 (Linux) and R5 Core 0 (FreeRTOS). Draw as block diagram with processor cores, caches, and shared memory connection.

### 4. **RPMsg Communication Architecture (Page 3)**
```
┌─────────────────────────────────────────────────────────┐
│                 User Space (A53)                        │
│  ┌──────────────────────────────────────────────────┐   │
│  │         Application (main.c)                      │   │
│  │  - Prepares aes_datatype                         │   │
│  │  - write(fd, data, size)                         │   │
│  │  - read(fd, result, size)                        │   │
│  └───────────────────┬──────────────────────────────┘   │
│                      │ /dev/rpmsgX                      │
├──────────────────────┼──────────────────────────────────┤
│    Linux Kernel      │                                  │
│  ┌──────────────────▼───────────────┐                   │
│  │   rpmsg_char driver              │                   │
│  └──────────────────┬───────────────┘                   │
│  ┌──────────────────▼───────────────┐                   │
│  │   RPMsg Framework                │                   │
│  └──────────────────┬───────────────┘                   │
│  ┌──────────────────▼───────────────┐                   │
│  │   VirtIO Transport               │                   │
│  └──────────────────┬───────────────┘                   │
│  ┌──────────────────▼───────────────┐                   │
│  │   Remoteproc Framework           │                   │
│  │   - Load firmware                │                   │
│  │   - Start/stop R5                │                   │
│  └──────────────────┬───────────────┘                   │
└─────────────────────┼─────────────────────────────────┘
                      │ Shared Memory (virtqueues)
┌─────────────────────▼─────────────────────────────────┐
│            FreeRTOS (R5 Core 0)                        │
│  ┌──────────────────────────────────────────────────┐  │
│  │   OpenAMP Library                                │  │
│  │   ┌────────────────────────────────────────────┐ │  │
│  │   │   RPMsg Endpoint Callback                  │ │  │
│  │   │   - rpmsg_endpoint_cb()                    │ │  │
│  │   │   - Parses aes_datatype                    │ │  │
│  │   └────────────┬───────────────────────────────┘ │  │
│  └────────────────┼─────────────────────────────────┘  │
│  ┌────────────────▼─────────────────────────────────┐  │
│  │   AES Implementation                             │  │
│  │   - AES_init_ctx_iv()                            │  │
│  │   - AES_CBC_encrypt_buffer()                     │  │
│  └──────────────────────────────────────────────────┘  │
└────────────────────────────────────────────────────────┘
```
**Description**: Complete software stack showing user application → kernel drivers → RPMsg → shared memory → R5 firmware. Draw as layered architecture with arrows showing data flow from application to AES execution on R5.

### 5. **RPC Setup Sequence (Page 4)**
```
Master (A53)                                    Remote (R5)
     │                                               │
     │ Load firmware and Boot Remote                │
     ├──────────────────────────────────────────────►│
     │                                               │ FreeRTOS boots
     │                                               │ OpenAMP initializes
     │                                               │ RPMsg endpoint created
     │                                               │
     │          ◄────── Name Service ──────────────  │
     │                  Announcement                 │
     │                                               │
Channel │                                            │ Channel
created │                                            │ created
Callback│                                            │ Callback()
     │                                               │
     │ ─────────  Name Service  ──────────────────► │
     │            Acknowledgement                    │
     │                                               │
     │ ═════════ Send Message ═══════════════════►  │
     │ rpmsg_send()                                  │ Callback executes
     │                                               │ (rpmsg_endpoint_cb)
     │                                               │ Performs AES
     │                                               │
     │  ◄═══════ Send Message ══════════════════════ │
     │                    rpmsg_send() returns       │
     │                                               │
     │ ═════════ Shutdown Message ═══════════════►  │
     ├──────────────────────────────────────────────►│
     │                                               │ Cleanup
     │  ◄────── Shutdown Acknowledge ──────────────  │
     │                                               │
     │ Stop Remote                                   │ R5 halts
     ├──────────────────────────────────────────────►│
     │                                               ■
```
**Description**: Sequence diagram showing complete RPC lifecycle: setup (firmware load, channel creation), communication (name service, message exchange), and shutdown. Draw with time flowing downward, showing message exchanges between processors.

### 6. **aes_datatype Structure Layout (Page 7)**
```
Offset: 0        1                               33
        ├────────┼───────────────────────────────┼───────────────────
Bytes:  │  dec   │      key[32]                 │    iv[16]
        │ (1B)   │   (32 bytes)                 │  (16 bytes)
        └────────┴───────────────────────────────┴───────────────────

Offset: 49       50                              50+N
        ┼────────┼───────────────────────────────────────────────────
        │ length │         text[]
        │  (1B)  │      (variable, N bytes)
        └────────┴───────────────────────────────────────────────────

Total Size: 50 bytes (fixed) + N bytes (text length)

Field Details:
┌────────────┬──────────┬─────────────────────────────────────────┐
│   Field    │   Size   │           Description                   │
├────────────┼──────────┼─────────────────────────────────────────┤
│ dec        │  1 byte  │ 0 = encrypt, 1 = decrypt               │
│ key[]      │ 32 bytes │ AES-256 encryption key                 │
│ iv[]       │ 16 bytes │ Initialization vector for CBC          │
│ text_length│  1 byte  │ Length of text (max 255 bytes)         │
│ text[]     │ N bytes  │ Plaintext (encrypt) or ciphertext      │
│            │          │ (decrypt) - modified in place          │
└────────────┴──────────┴─────────────────────────────────────────┘

Return Data:
┌────────────┬──────────┬─────────────────────────────────────────┐
│   Field    │   Size   │           Description                   │
├────────────┼──────────┼─────────────────────────────────────────┤
│ text[]     │ N bytes  │ Result: ciphertext or plaintext        │
│ text_length│  1 byte  │ Length of result (same as input)       │
└────────────┴──────────┴─────────────────────────────────────────┘
```
**Description**: Memory layout of RPMsg data structure showing 50-byte fixed header plus variable-length text. Draw as byte-level memory diagram with offsets and field descriptions.

### 7. **Performance Comparison Chart (Example Data)**
```
Execution Time (ms)
  ^
30│
  │                                                     ■ APU (A53)
25│                                                     □ RPU (R5)
  │
20│
  │                                          ■
15│                               □
  │                    ■
10│         □
  │  ■
 5│  □
  │
 0└──────────────────────────────────────────────────►
    16 bytes    64 bytes    240 bytes    Message Size
    (1 block)   (4 blocks)  (15 blocks)

Observations:
┌──────────────┬─────────────────────────────────────────────┐
│ Message Size │          Performance Comparison             │
├──────────────┼─────────────────────────────────────────────┤
│ 16 bytes     │ A53 faster - RPU comm overhead dominates   │
│ 64 bytes     │ Similar - overhead vs computation balanced  │
│ 240 bytes    │ Variable - depends on specific system       │
└──────────────┴─────────────────────────────────────────────┘
```
**Description**: Example performance comparison showing execution time vs message size for both processors. Small messages favor A53 (low overhead), larger messages show tradeoffs. Draw as bar chart or line graph with two series.

---

## Formulas & Procedures

### 1. **Time Measurement Calculation**

**Formula:**
```c
struct timespec time_start, time_stop;
clock_gettime(CLOCK_REALTIME, &time_start);
// ... code to measure ...
clock_gettime(CLOCK_REALTIME, &time_stop);

// Calculate elapsed time in seconds
double elapsed_seconds = (time_stop.tv_sec - time_start.tv_sec) +
                         (time_stop.tv_nsec - time_start.tv_nsec) / 1e9;

// Or in milliseconds
double elapsed_ms = (time_stop.tv_sec - time_start.tv_sec) * 1000.0 +
                    (time_stop.tv_nsec - time_start.tv_nsec) / 1e6;

// Or in microseconds
double elapsed_us = (time_stop.tv_sec - time_start.tv_sec) * 1e6 +
                    (time_stop.tv_nsec - time_start.tv_nsec) / 1e3;
```

**Variables:**
- `tv_sec`: Seconds component (time_t, typically long)
- `tv_nsec`: Nanoseconds component (long, 0-999,999,999)
- `1e9`: 10^9 nanoseconds per second
- `1e6`: 10^6 nanoseconds per millisecond
- `1e3`: 10^3 nanoseconds per microsecond

**Example:**
```
Start:  tv_sec = 1234567890, tv_nsec = 500000000
Stop:   tv_sec = 1234567891, tv_nsec = 750000000

Elapsed: (1234567891 - 1234567890) + (750000000 - 500000000) / 1e9
       = 1 + 250000000 / 1000000000
       = 1 + 0.25
       = 1.25 seconds
       = 1250 milliseconds
```

### 2. **Communication Overhead Calculation**

**Total Time Breakdown:**
```
T_total = T_setup + T_encrypt + T_teardown

Where:
T_setup = Time for AES_RPU_start() (one-time)
T_encrypt = Time for AES_RPU_encrypt_buffer()
          = T_prepare + T_send + T_compute + T_receive
T_teardown = Time for AES_RPU_stop() (one-time, includes sleep(2))
```

**Per-encryption Time (what we measure):**
```
T_encrypt = T_prepare + T_comm_overhead + T_aes_actual

Where:
T_prepare = Time to build aes_datatype structure
T_comm_overhead = T_send + T_receive + context_switches
T_aes_actual = Actual AES computation time on R5
```

**Communication Overhead Percentage:**
```
Overhead% = (T_comm_overhead / T_encrypt) × 100%

For small messages: Overhead% can be 50-80%
For large messages: Overhead% drops to 10-30%
```

**Example:**
```
16-byte message:
T_encrypt = 0.5 ms (total measured)
T_aes_actual = 0.1 ms (estimated pure computation)
T_comm_overhead = 0.4 ms (send/receive/switching)
Overhead% = (0.4 / 0.5) × 100% = 80%

240-byte message:
T_encrypt = 2.0 ms (total measured)
T_aes_actual = 1.5 ms (estimated pure computation)
T_comm_overhead = 0.5 ms (send/receive/switching)
Overhead% = (0.5 / 2.0) × 100% = 25%
```

### 3. **AES CBC Block Processing**

**Number of Blocks:**
```
N_blocks = ⌈message_length / 16⌉

Where:
message_length must be multiple of 16
16 = AES block size in bytes
⌈⌉ = ceiling function (round up)
```

**Examples:**
```
16 bytes  → 16/16 = 1 block
64 bytes  → 64/16 = 4 blocks
240 bytes → 240/16 = 15 blocks
255 bytes → Invalid! Not multiple of 16
256 bytes → 256/16 = 16 blocks
```

**Procedure for Padding (if needed):**
```
1. Calculate padding needed:
   padding_bytes = 16 - (message_length % 16)

2. If padding_bytes == 16, no padding needed

3. Otherwise, append padding_bytes worth of padding:
   - PKCS#7: Each padding byte contains value padding_bytes
   - Example: Need 5 bytes padding → append 0x05 0x05 0x05 0x05 0x05
```

### 4. **Remote Processor Control Procedure**

**Starting Remote Processor:**
```bash
# 1. Write firmware name
echo "aes_rpu_rtos.elf" > /sys/class/remoteproc/remoteproc0/firmware

# 2. Start the processor
echo "start" > /sys/class/remoteproc/remoteproc0/state

# 3. Verify state
cat /sys/class/remoteproc/remoteproc0/state
# Should show: "running"

# 4. Check for rpmsg device
ls /dev/rpmsg*
# Should see: /dev/rpmsg0, /dev/rpmsg_ctrl0
```

**Stopping Remote Processor:**
```bash
# 1. Send shutdown message (done by application)
# 2. Wait for graceful shutdown (sleep(2))

# 3. Stop the processor
echo "stop" > /sys/class/remoteproc/remoteproc0/state

# 4. Verify state
cat /sys/class/remoteproc/remoteproc0/state
# Should show: "offline"
```

**State Transitions:**
```
offline → (echo "start") → running
running → (echo "stop") → offline

States:
- offline: R5 not running
- running: R5 executing firmware
- crashed: R5 encountered error (rare)
```

### 5. **Performance Data Collection Procedure**

**For Each Processor (A53, R5):**

**Step 1: Test Setup**
```
1. Select message size: 16, 64, or 240 bytes
2. Prepare test data (NIST test vectors)
3. Verify program built and deployed
4. SSH to Ultra96 board
```

**Step 2: Run Multiple Trials**
```
for trial in 1..5:
    1. Execute ./apu.elf (or ./rpu.elf)
    2. Record printed execution time
    3. Verify "SUCCESS!" message
    4. Wait 1-2 seconds between trials
```

**Step 3: Calculate Statistics**
```
measurements = [t1, t2, t3, t4, t5]

average = sum(measurements) / 5
min_time = minimum(measurements)
max_time = maximum(measurements)

// Optional: Standard deviation
variance = sum((ti - average)² for ti in measurements) / 5
std_dev = √variance
```

**Step 4: Tabulate Results**
```
Processor | Message Size | Avg Time | Min | Max | Std Dev
----------|--------------|----------|-----|-----|--------
A53       | 16 bytes     | X.XX ms  | ... | ... | ...
A53       | 64 bytes     | X.XX ms  | ... | ... | ...
A53       | 240 bytes    | X.XX ms  | ... | ... | ...
R5        | 16 bytes     | X.XX ms  | ... | ... | ...
R5        | 64 bytes     | X.XX ms  | ... | ... | ...
R5        | 240 bytes    | X.XX ms  | ... | ... | ...
```

**Step 5: Analysis**
```
// Calculate speedup
speedup = Time_slower / Time_faster

// Calculate efficiency (for RPU)
efficiency = Theoretical_speedup / Actual_speedup
           = (if R5 is slower due to overhead)

// Identify crossover point
// At what message size does R5 become faster/slower?
```

### 6. **AES Encryption/Decryption Procedure**

**Complete Encryption Process:**
```
1. Prepare key (32 bytes for AES-256)
2. Generate random IV (16 bytes)
3. Ensure message length is multiple of 16
   (pad if necessary)
4. Initialize context:
   AES_init_ctx_iv(&ctx, key, iv)
5. Encrypt in place:
   AES_CBC_encrypt_buffer(&ctx, buffer, length)
6. Result: buffer now contains ciphertext
7. Store IV with ciphertext (needed for decryption)
```

**Complete Decryption Process:**
```
1. Use same key as encryption (32 bytes)
2. Retrieve IV that was used for encryption (16 bytes)
3. Initialize context:
   AES_init_ctx_iv(&ctx, key, iv)
4. Decrypt in place:
   AES_CBC_decrypt_buffer(&ctx, buffer, length)
5. Result: buffer now contains plaintext
6. Remove padding if PKCS#7 was used
```

**Key Differences:**
- Encryption: Sequential, cannot parallelize blocks
- Decryption: Parallelizable, can process blocks independently
- Same key and IV must be used for both operations
- IV does not need to be secret, but should be unpredictable

---

## Comparisons & Contrasts

### 1. **ARM Cortex-A53 vs ARM Cortex-R5**

| Feature | Cortex-A53 (APU) | Cortex-R5 (RPU) |
|---------|------------------|-----------------|
| **Architecture** | 64-bit ARMv8-A | 32-bit ARMv7-R |
| **Design Goal** | High performance | Hard real-time |
| **Typical Clock** | 1.2-1.5 GHz | 500-600 MHz |
| **Pipeline** | 8-stage | 8-stage |
| **Cache L1** | 32 KB I + 32 KB D | 32 KB I + 32 KB D |
| **Cache L2** | Up to 2 MB (shared) | None (has TCM) |
| **TCM** | None | 128 KB (fast local memory) |
| **OS Support** | Linux, Android | FreeRTOS, bare-metal |
| **Memory Protection** | MMU (virtual memory) | MPU (regions) |
| **Best For** | Complex apps, multitasking | Real-time control |
| **Power** | Higher | Lower |
| **Determinism** | Variable (OS scheduling) | High (minimal OS) |
| **This Lab** | Runs Linux + application | Runs FreeRTOS + AES |

**When to Use Each:**
- **A53**: User interfaces, networking, file systems, complex algorithms
- **R5**: Motor control, sensor fusion, safety-critical tasks, low-latency processing

### 2. **Local vs Remote AES Execution**

| Aspect | APU (Local on A53) | RPU (Remote on R5) |
|--------|-------------------|-------------------|
| **Execution Location** | Same processor as caller | Different processor |
| **Communication** | Function call (nanoseconds) | RPMsg (microseconds to milliseconds) |
| **Setup Cost** | Context init (~microseconds) | Load firmware, establish channel (~seconds) |
| **Per-Call Overhead** | Minimal (function call) | High (50-byte header + copies) |
| **Data Transfer** | None (same memory) | Copy to shared memory, notify, copy back |
| **Context Switches** | None or minimal (Linux scheduler) | Multiple (Linux → R5 → Linux) |
| **Determinism** | Low (Linux interrupts) | High (dedicated RTOS) |
| **Resource Usage** | Uses A53 time | Offloads to R5 |
| **Best For** | Small, frequent operations | Large, infrequent operations or dedicated task |
| **Scalability** | Limited by A53 cores | Can use multiple R5 cores independently |
| **Latency** | Very low | Higher due to communication |
| **Throughput** | High for small messages | Better for large batches |

**Performance Crossover:**
- **Small messages** (< 64 bytes): Local faster (overhead dominates)
- **Large messages** (> 256 bytes): May favor remote if R5 less loaded
- **Sustained load**: Remote better (frees A53 for other tasks)
- **Sporadic use**: Local better (avoid setup overhead)

### 3. **CBC Encryption vs Decryption**

| Characteristic | Encryption | Decryption |
|----------------|------------|------------|
| **Dependencies** | Each block depends on previous | Each block independent (except for XOR) |
| **Parallelization** | Must be sequential | Can be parallelized |
| **Processing Order** | Must go forward (P₁→P₂→P₃→...) | Can process any order |
| **Input** | Plaintext blocks | Ciphertext blocks |
| **XOR Input** | Previous ciphertext (or IV) | Previous ciphertext (or IV) |
| **AES Operation** | Encrypt(Pᵢ ⊕ Cᵢ₋₁) | Decrypt(Cᵢ), then ⊕ Cᵢ₋₁ |
| **Performance** | Linear with blocks | Could be faster with parallelization |
| **This Lab** | Sequential implementation | Sequential implementation |
| **Typical Use** | Sender side | Receiver side |

**Why Decryption Can Parallelize:**
```
Decrypt P₁: Only needs C₁ and IV
Decrypt P₂: Only needs C₂ and C₁
Decrypt P₃: Only needs C₃ and C₂
// All Cᵢ values known at start → can process simultaneously
```

### 4. **Communication Methods**

| Method | Description | Speed | Use Case |
|--------|-------------|-------|----------|
| **Function Call** | Direct call within same processor | Fastest (ns) | Same-process functions |
| **System Call** | User space → kernel | Fast (μs) | Access kernel services |
| **IPC (pipes, sockets)** | Between processes on same CPU | Medium (μs-ms) | Process communication |
| **Shared Memory** | Direct memory access | Fast (μs) | Same-SoC processors |
| **RPMsg** | Message-based shared memory | Medium (ms) | Different CPU cores |
| **Network** | TCP/IP over Ethernet/WiFi | Slow (ms-s) | Different devices |

**This Lab Uses:**
- Function calls (within A53 or R5)
- System calls (open/read/write to /dev/rpmsg)
- RPMsg (A53 ↔ R5 communication)
- Shared memory (underlying RPMsg transport)

### 5. **AES Key Sizes**

| Key Size | Security Level | Speed | Use Case |
|----------|----------------|-------|----------|
| **AES-128** | 128 bits (16 bytes) | Fastest | Most common, sufficient for most uses |
| **AES-192** | 192 bits (24 bytes) | Medium | Higher security margin |
| **AES-256** | 256 bits (32 bytes) | Slowest | Maximum security, government/military |

**This Lab Uses: AES-256**

**Computational Cost:**
- AES-128: 10 rounds
- AES-192: 12 rounds
- AES-256: 14 rounds (40% more computation than AES-128)

**Security:**
- AES-128: 2^128 possible keys (sufficient until ~2030s)
- AES-256: 2^256 possible keys (computationally infeasible to break)

### 6. **Operating Systems**

| Feature | Linux (on A53) | FreeRTOS (on R5) |
|---------|----------------|------------------|
| **Type** | General-purpose OS | Real-time OS |
| **Size** | Large (MB) | Tiny (KB) |
| **Scheduling** | Complex (CFS) | Simple (priority-based) |
| **Determinism** | Low | High |
| **Memory Management** | Virtual (MMU) | Physical (MPU) |
| **Processes** | Multiple, isolated | Tasks, shared memory |
| **File System** | Yes | No (optional) |
| **Network Stack** | Full TCP/IP | Minimal (optional) |
| **Drivers** | Extensive | Limited |
| **Development** | Standard tools | Embedded toolchain |
| **Context Switch** | Slow (~10 μs) | Fast (~1 μs) |
| **Best For** | Complex applications | Real-time control |

**This Lab:**
- Linux handles user interaction, file access, development tools
- FreeRTOS provides deterministic AES execution

### 7. **Timing Measurement Methods**

| Method | Resolution | Accuracy | Overhead | Use Case |
|--------|-----------|----------|----------|----------|
| **clock_gettime(CLOCK_REALTIME)** | Nanosecond | Good | Low | Wall-clock time |
| **clock_gettime(CLOCK_MONOTONIC)** | Nanosecond | Excellent | Low | Relative time (immune to time adjustments) |
| **gettimeofday()** | Microsecond | Good | Very low | Legacy code |
| **rdtsc** (x86) | CPU cycle | Excellent | Minimal | High-precision profiling |
| **PMU counters** | CPU cycle | Excellent | Minimal | Hardware profiling |
| **GPIO toggle** | Hardware-dependent | Excellent | None | External measurement |

**This Lab Uses: clock_gettime(CLOCK_REALTIME)**
- Sufficient resolution for millisecond-scale measurements
- Easy to use
- Portable across platforms
- Low overhead

---

## Examples & Applications

### Example 1: **Complete APU Execution Flow**

**Scenario:** Encrypt 64-byte message on ARM Cortex-A53

**Code:**
```c
#include <time.h>
#include "aes_apu.h"

int main(void) {
    // Test data (NIST test vectors)
    uint8_t key[32] = {
        0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
        0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
        0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
        0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4
    };

    uint8_t iv[16] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    };

    uint8_t plaintext[64] = "This is a 64-byte test message for AES encryption using CBC mode!";

    struct timespec time_start, time_stop;

    // Start timing
    clock_gettime(CLOCK_REALTIME, &time_start);

    // Initialize AES context (key expansion + IV setup)
    AES_APU_init_ctx_iv(key, iv);

    // Encrypt in place
    AES_APU_encrypt_buffer(plaintext, 64);

    // Stop timing
    clock_gettime(CLOCK_REALTIME, &time_stop);

    // Calculate elapsed time
    double elapsed_ms = (time_stop.tv_sec - time_start.tv_sec) * 1000.0 +
                        (time_stop.tv_nsec - time_start.tv_nsec) / 1e6;

    printf("Encryption time: %.3f ms\n", elapsed_ms);

    // At this point, plaintext[] contains ciphertext
    // Verify against expected NIST result...

    return 0;
}
```

**Execution Flow:**
```
1. Initialize test data (key, IV, plaintext)
2. Get start timestamp
3. Expand 256-bit key into round keys (key expansion)
4. Store IV in context
5. For each 16-byte block:
   a. XOR with previous ciphertext (or IV for first)
   b. Apply AES encryption (14 rounds for AES-256)
   c. Store result as ciphertext
6. Get stop timestamp
7. Calculate elapsed time: ~0.05-0.1 ms typical
8. Verify result matches expected ciphertext
```

**Typical Output:**
```
Encryption time: 0.087 ms
AES encrypt: SUCCESS!
```

### Example 2: **Complete RPU Execution Flow**

**Scenario:** Encrypt same 64-byte message using remote R5 processor

**Code:**
```c
int main(void) {
    // Same test data as Example 1
    uint8_t key[32] = { /* ... */ };
    uint8_t iv[16] = { /* ... */ };
    uint8_t plaintext[64] = "This is a 64-byte test message...";

    struct timespec time_start, time_stop;

    // Setup: Load firmware and establish communication
    // (Only done once, not timed)
    int success = AES_RPU_start("aes_rpu_rtos.elf");
    if (success < 0) {
        printf("Failed to start RPU\n");
        return -1;
    }

    // Measure only the encryption, not setup
    clock_gettime(CLOCK_REALTIME, &time_start);

    // Call remote procedure
    AES_RPU_encrypt_buffer(key, iv, plaintext, 64);

    clock_gettime(CLOCK_REALTIME, &time_stop);

    double elapsed_ms = (time_stop.tv_sec - time_start.tv_sec) * 1000.0 +
                        (time_stop.tv_nsec - time_start.tv_nsec) / 1e6;

    printf("RPU encryption time: %.3f ms\n", elapsed_ms);

    // Teardown: Stop firmware (not timed, includes sleep(2))
    AES_RPU_stop("aes_rpu_rtos.elf");

    return 0;
}
```

**Behind the Scenes (AES_RPU_encrypt_buffer):**
```c
void AES_RPU_encrypt_buffer(uint8_t *key, uint8_t *iv,
                             uint8_t *buffer, uint32_t length) {
    // 1. Allocate message structure
    aes_datatype *msg = malloc(50 + length);

    // 2. Populate structure
    msg->dec = 0;  // 0 = encrypt
    memcpy(msg->key, key, 32);
    memcpy(msg->iv, iv, 16);
    msg->text_length = length;
    memcpy(msg->text, buffer, length);

    // 3. Send to R5 (blocking write)
    write(fd_glob, msg, 50 + length);
    // At this point, data copied to shared memory
    // R5 is notified via interrupt
    // R5 processes in rpmsg_endpoint_cb()
    // R5 sends result back

    // 4. Receive result (blocking read)
    read(fd_glob, buffer, length);
    // Buffer now contains ciphertext

    free(msg);
}
```

**R5 Side (rpmsg_endpoint_cb):**
```c
int rpmsg_endpoint_cb(struct rpmsg_endpoint *ept, void *data,
                      size_t len, uint32_t src, void *priv) {
    aes_datatype *msg = (aes_datatype *)data;

    // 1. Initialize AES context
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, msg->key, msg->iv);

    // 2. Encrypt (msg->dec == 0)
    AES_CBC_encrypt_buffer(&ctx, msg->text, msg->text_length);

    // 3. Send result back
    rpmsg_send(ept, msg->text, msg->text_length);

    return 0;
}
```

**Typical Output:**
```
RPU encryption time: 1.234 ms
AES encrypt: SUCCESS!
```

**Time Breakdown:**
```
Total 1.234 ms:
- Prepare message: ~0.01 ms
- Send to R5: ~0.4 ms (copy + notify + context switch)
- R5 encryption: ~0.1 ms (actual AES)
- Receive from R5: ~0.4 ms (context switch + copy)
- Overhead: ~0.3 ms (various system calls)

Communication overhead: ~1.1 ms out of 1.234 ms = 89%!
```

### Example 3: **Performance Comparison Analysis**

**Measured Data:**

| Message Size | APU Time (ms) | RPU Time (ms) | Winner | Speedup |
|--------------|---------------|---------------|--------|---------|
| 16 bytes (1 block) | 0.042 | 0.856 | APU | 20.4× faster |
| 64 bytes (4 blocks) | 0.087 | 1.234 | APU | 14.2× faster |
| 240 bytes (15 blocks) | 0.298 | 2.145 | APU | 7.2× faster |

**Analysis:**

**Why APU is Faster for All Sizes:**
1. **No Communication Overhead**: Function call vs RPMsg
2. **Higher Clock**: A53 @ 1.2 GHz vs R5 @ 500 MHz = 2.4× clock advantage
3. **Better Cache**: L2 cache on A53 helps with repeated operations
4. **Compiler Optimization**: A53 toolchain may produce better code

**Why Speedup Decreases with Size:**
```
16 bytes:
- APU: Mostly setup overhead (context init)
- RPU: Mostly communication overhead
- Ratio: 20.4× (communication dominates)

240 bytes:
- APU: More actual computation time
- RPU: Same communication overhead, more computation
- Ratio: 7.2× (computation becomes significant)
```

**Break-even Analysis:**
```
For RPU to be competitive:
- Communication overhead must be amortized
- Either:
  a) Much larger messages (MB range)
  b) Sustained high load (A53 saturated)
  c) Multiple concurrent operations (parallel R5 cores)
  d) Real-time guarantees needed (R5 determinism)
```

**When RPU Would Win:**
```
Scenario 1: A53 fully loaded
- A53 running: Web server, UI, network stack
- Offload AES to R5: Frees A53 for other tasks
- Total system throughput: Higher with RPU

Scenario 2: Large batch processing
- Encrypt 1000 messages back-to-back
- Setup overhead: Amortized over all operations
- Setup (1× 2s) + 1000× encrypt (1.2 ms each) = 3.2s total
- vs A53: 1000× 0.087 ms = 87 ms
- RPU still slower, but...

Scenario 3: Real-time requirements
- Need guaranteed 2ms response time
- A53: Variable (Linux scheduling) - might miss deadline
- R5: Consistent 1.2ms - always meets deadline
- RPU wins for determinism!
```

### Example 4: **Memory Layout and Data Transfer**

**Scenario:** Understanding the 50-byte overhead

**RPMsg Structure:**
```c
// On A53 (sender)
uint8_t key[32] = { /* 256-bit key */ };
uint8_t iv[16] = { /* 128-bit IV */ };
uint8_t text[16] = "Hello, World!!! ";  // 16 bytes (1 block)

// Prepare message
aes_datatype *msg = malloc(50 + 16);  // 66 bytes total

msg->dec = 0;                    // Byte 0
memcpy(msg->key, key, 32);       // Bytes 1-32
memcpy(msg->iv, iv, 16);         // Bytes 33-48
msg->text_length = 16;           // Byte 49
memcpy(msg->text, text, 16);     // Bytes 50-65

// Send (copies to shared memory)
write(fd, msg, 66);
```

**Memory Layout in Shared Memory:**
```
Offset  Content                              Bytes
------  ------------------------------------ -----
0       dec flag (0x00 = encrypt)            1
1-32    key[32]                              32
33-48   iv[16]                               16
49      text_length (0x10 = 16)              1
50-65   text[16] "Hello, World!!! "          16
        ------------------------------------ ----
        Total                                66
```

**Data Transfer Path:**
```
1. User buffer (plaintext) in A53 RAM
   ↓ memcpy
2. aes_datatype structure in A53 heap
   ↓ write() system call
3. RPMsg buffer in kernel space
   ↓ virtio transport
4. Shared DDR memory (virtqueue)
   ↓ R5 interrupt & DMA
5. R5 local memory
   ↓ Processing
6. R5 result buffer
   ↓ rpmsg_send()
7. Shared DDR memory (virtqueue)
   ↓ A53 interrupt
8. Kernel RPMsg buffer
   ↓ read() system call
9. User buffer (ciphertext) in A53 RAM

Total copies: 5-6 memory copy operations!
```

**Overhead Breakdown:**
```
Operation                    Time (estimated)
---------------------------- ----------------
Prepare message (memcpy×3)   ~5 μs
write() syscall overhead     ~10 μs
Copy to kernel buffer        ~20 μs
Virtio transport             ~50 μs
R5 interrupt latency         ~10 μs
R5 processing (AES)          ~100 μs
R5 rpmsg_send                ~30 μs
Virtio transport back        ~50 μs
A53 interrupt latency        ~20 μs
Copy from kernel buffer      ~20 μs
read() syscall overhead      ~10 μs
                             --------
Total                        ~325 μs

Actual computation: ~100 μs
Communication: ~225 μs
Overhead ratio: 69%
```

### Example 5: **NIST Test Vector Verification**

**Purpose:** Ensure AES implementation is correct using official test vectors

**Test Vector (AES-256 CBC):**
```c
// From NIST FIPS-197 test vectors
uint8_t key[32] = {
    0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
    0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
    0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
    0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4
};

uint8_t iv[16] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};

uint8_t plaintext[16] = {
    0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
    0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a
};

// Expected result after encryption
uint8_t expected_ciphertext[16] = {
    0xf5, 0x8c, 0x4c, 0x04, 0xd6, 0xe5, 0xf1, 0xba,
    0x77, 0x9e, 0xab, 0xfb, 0x5f, 0x7b, 0xfb, 0xd6
};
```

**Verification Code:**
```c
// Encrypt
AES_APU_init_ctx_iv(key, iv);
AES_APU_encrypt_buffer(plaintext, 16);

// Compare result
int success = 1;
for (int i = 0; i < 16; i++) {
    if (plaintext[i] != expected_ciphertext[i]) {
        printf("Mismatch at byte %d: got 0x%02x, expected 0x%02x\n",
               i, plaintext[i], expected_ciphertext[i]);
        success = 0;
    }
}

if (success) {
    printf("AES encrypt: SUCCESS!\n");
} else {
    printf("AES encrypt: FAILED!\n");
}

// Decrypt to verify reversibility
AES_APU_init_ctx_iv(key, iv);  // Re-init with same key/IV
AES_APU_decrypt_buffer(plaintext, 16);  // plaintext contains ciphertext now

// Should get original plaintext back
uint8_t original[16] = {
    0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
    0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a
};

for (int i = 0; i < 16; i++) {
    if (plaintext[i] != original[i]) {
        printf("Decrypt mismatch at byte %d\n", i);
        success = 0;
    }
}

if (success) {
    printf("AES decrypt: SUCCESS!\n");
}
```

**Why This Matters:**
- Proves implementation matches official standard
- Detects endianness issues
- Catches key expansion bugs
- Verifies CBC chaining logic
- Essential for interoperability with other AES implementations

---

## Connections to Other Topics

### To Lecture 01: Embedded Systems
- **Multi-processor SoC** exemplifies heterogeneous embedded systems
- **Resource constraints** drive need for processor selection (A53 vs R5)
- **Real-time requirements** motivate use of R5 for deterministic tasks
- **Energy considerations** (though Ultra96 power-hungry, principle applies)

### To Lecture 02: Software Paradigms
- **Modular design**: aes.c, aes_apu.c, aes_rpu.c separation
- **Event-driven**: RPMsg callback (rpmsg_endpoint_cb) as event handler
- **Layered architecture**: Application → System calls → Kernel → Hardware
- **Interface design**: Same AES API regardless of processor choice

### To Lecture 05: Development Processes
- **Cross-compilation**: Different toolchains for A53 (Linux) and R5 (FreeRTOS)
- **Testing with NIST vectors**: Verification against known-good results
- **Iterative optimization**: Measure, analyze, improve (performance tuning)
- **Makefile automation**: Build and deployment process

### To Lecture 06: Non-Functional Requirements
- **Performance**: Execution time measurements for different processors
- **Determinism**: R5 provides predictable timing (real-time NFR)
- **Scalability**: Can add more R5 cores or distribute load
- **Reliability**: NIST test vectors ensure correct implementation
- **Maintainability**: Modular code structure

### To Lecture 08: Energy
- **Performance-energy tradeoff**: Faster A53 uses more power per operation
- **Offloading**: R5 might be more energy-efficient if A53 can sleep
- **Sleep modes**: R5 can sleep between encryptions, A53 runs full Linux
- **Dynamic power**: Higher clock A53 has P = α×C×V²×F energy cost

### To Lecture 09: Performance
- **Profiling**: Timing measurements identify bottlenecks (communication overhead)
- **Amdahl's Law**: Communication overhead limits RPU speedup
- **Algorithm optimization**: AES implementation (this lab uses software; future labs use hardware)
- **Cache effects**: A53's L2 cache vs R5's TCM
- **Parallelization potential**: CBC encryption sequential, decryption parallelizable

### To Future Lecture 10: FPGAs
- **Hardware acceleration**: Next step beyond software AES
- **Performance comparison**: FPGA will be much faster than both A53 and R5
- **Energy efficiency**: Dedicated hardware more efficient than software
- **Design space**: Software (flexible) vs Hardware (fast) tradeoff

### To Future Lecture 11-12: Design Space Exploration
- **Processor selection**: A53 vs R5 vs FPGA for AES algorithm
- **Performance/cost/energy tradeoffs**: This lab provides data points
- **Pareto frontiers**: Plotting performance vs energy vs cost
- **Design decisions**: When to use remote procedures vs local execution

### To Future Lecture 13: RTOS Scheduling
- **FreeRTOS**: Running on R5 in this lab
- **Task priorities**: RPMsg callback as high-priority task
- **Deterministic scheduling**: R5 provides consistent response times
- **Interrupt handling**: RPMsg uses interrupts for inter-processor communication

### To Future Lecture 14: Multi-Processor Systems
- **Heterogeneous multiprocessing**: A53 + R5 working together
- **Inter-processor communication**: RPMsg and virtio
- **Shared memory**: Communication mechanism between processors
- **Load distribution**: Offloading computation to specialized cores
- **Coherency issues**: Shared memory requires careful synchronization

### To Lab P2: Energy Analysis
- **Performance measurement**: Similar timing methodology (clock_gettime)
- **Energy-performance tradeoff**: Faster execution often means more power
- **State machines**: P2 used FSM for idle/AES states; similar concept here
- **AES algorithm**: Same algorithm, different measurement focus

### To Future Lab P4: AES in Hardware
- **Same algorithm**: Software (this lab) vs Hardware (P4)
- **Performance comparison**: Will compare all implementations (A53, R5, FPGA, CSU)
- **Design space data**: This lab provides software performance baseline
- **Hardware acceleration**: Dramatic speedup expected in P4

### To Future Lab P5: Design Space Exploration
- **Data collection**: Performance measurements from this lab used in P5
- **Comparison**: A53 vs R5 vs FPGA vs CSU hardware crypto
- **Optimization**: Use DSE to find optimal implementation
- **Trade-off analysis**: Performance, energy, cost considerations

---

## Summary

This lab demonstrates **heterogeneous multiprocessing** by implementing AES-256 encryption on two different ARM processors within the Zynq UltraScale+ MPSoC:

**Key Implementations:**
1. **Local execution on A53**: Direct function calls under Linux
2. **Remote execution on R5**: RPMsg-based inter-processor communication

**Main Findings:**
- **Communication overhead dominates** for remote procedure calls with small messages
- **A53 typically faster** for this workload due to higher clock and no communication penalty
- **R5 provides determinism** valuable for real-time applications despite lower performance
- **Design tradeoffs**: Performance vs determinism vs resource utilization

**Technical Skills Developed:**
1. AES-256 CBC encryption/decryption
2. High-precision timing with clock_gettime()
3. Remote procedure call setup and execution
4. Linux remoteproc and RPMsg frameworks
5. Multi-processor system programming
6. Performance measurement and analysis

**Important Concepts:**
- **CBC mode**: Chaining prevents pattern detection, but limits parallelization
- **RPMsg protocol**: Message-based communication between heterogeneous processors
- **Firmware management**: Loading and controlling remote processors from Linux
- **Performance profiling**: Identifying and measuring overhead vs computation time
- **NIST test vectors**: Validation against cryptographic standards

**Real-World Applications:**
- IoT devices with crypto workload distribution
- Automotive systems (application + safety-critical processors)
- Industrial control (HMI on A53, control loops on R5)
- Medical devices (user interface + real-time monitoring)

---

## Possible Exam Questions

### Basic Knowledge (Definitions & Concepts)

1. **What is AES and what key sizes does it support?**

2. **Explain what CBC (Cipher Block Chaining) mode is and why it's used.**

3. **Define the Initialization Vector (IV) and explain its purpose in CBC mode.**

4. **What is the difference between ARM Cortex-A53 and ARM Cortex-R5 processors?**

5. **Define "Remote Procedure Call" (RPC) in the context of embedded systems.**

6. **What is RPMsg and what problem does it solve?**

7. **Explain the purpose of the Linux remoteproc framework.**

8. **What is FreeRTOS and why is it suitable for the R5 processor?**

9. **Define "callback function" and give an example from this lab.**

10. **What is the purpose of the aes_datatype structure?**

11. **Explain what clock_gettime() does and what CLOCK_REALTIME means.**

12. **What is shared memory and how is it used for inter-processor communication?**

13. **Define TCM (Tightly-Coupled Memory) and explain its advantage.**

14. **What is a "heterogeneous multiprocessing system"?**

15. **Explain the purpose of NIST test vectors in cryptographic implementations.**

16. **What does "in-place encryption" mean?**

17. **Define communication overhead in the context of remote procedure calls.**

18. **What is the virtio transport layer?**

19. **Explain the difference between plaintext and ciphertext.**

20. **What is the significance of the SHUTDOWN_MSG (0xEF56A55A)?**

### Conceptual Understanding

21. **Explain why CBC encryption must be sequential but decryption can be parallelized.**

22. **Describe the complete lifecycle of a remote procedure call from setup to shutdown.**

23. **Why does the RPU implementation include a sleep(2) in the stop function?**

24. **Explain the trade-offs between running AES on A53 vs R5.**

25. **Describe what happens when you write "start" to /sys/class/remoteproc/remoteproc0/state.**

26. **Why is communication overhead more significant for small messages than large ones?**

27. **Explain the two steps that AES_init_ctx_iv() performs and why they're necessary.**

28. **Describe the flow of data from A53 user space to R5 execution and back.**

29. **Why might the R5 processor be preferred despite being slower than the A53?**

30. **Explain how the RPMsg callback function (rpmsg_endpoint_cb) works on the R5 side.**

31. **Describe the memory layout of the aes_datatype structure sent via RPMsg.**

32. **Why must AES message lengths be multiples of 16 bytes?**

33. **Explain the role of the device tree in accessing the remote processor.**

34. **Describe how the "name service" protocol establishes communication between processors.**

35. **Why is it important to measure only the encryption time and not the setup/teardown?**

### Implementation & Practical

36. **Show how to measure execution time using clock_gettime() in C code.**

37. **Write the code to initialize AES context and encrypt a 16-byte message on the APU.**

38. **Describe the steps to manually start the remote processor using sysfs commands.**

39. **How would you modify the code to decrypt instead of encrypt on the RPU?**

40. **Show how to calculate the total size of an RPMsg message for a 64-byte payload.**

41. **Explain how to verify AES encryption results using NIST test vectors.**

42. **Describe the procedure for collecting performance measurements for different message sizes.**

43. **How would you calculate communication overhead percentage from measured times?**

44. **Show how to calculate elapsed time in milliseconds from two timespec structs.**

45. **Write pseudo-code for the rpmsg_endpoint_cb callback function on the R5.**

### Analysis & Comparison

46. **Compare the execution characteristics of APU vs RPU AES implementations.**

47. **Analyze why small messages favor local execution while large messages might benefit from remote execution.**

48. **Compare CBC encryption and decryption in terms of parallelization potential.**

49. **Contrast the roles of Linux (on A53) and FreeRTOS (on R5) in this system.**

50. **Compare function calls, system calls, and RPMsg in terms of communication overhead.**

51. **Analyze the performance impact of the 50-byte fixed overhead in RPMsg messages.**

52. **Compare AES-128 vs AES-256 in terms of security and performance trade-offs.**

53. **Contrast MMU (on A53) vs MPU (on R5) memory protection mechanisms.**

54. **Compare L2 cache (A53) vs TCM (R5) for fast memory access.**

55. **Analyze the path data takes from user application to R5 execution - how many copies?**

### Problem Solving & Calculations

56. **Given measurements: A53 = 0.087 ms, R5 = 1.234 ms for 64 bytes. Calculate speedup and overhead.**

57. **If R5 takes 0.1 ms for pure AES and total time is 1.2 ms, what percentage is communication overhead?**

58. **Calculate the total message size for encrypting 240 bytes via RPMsg.**

59. **How many AES blocks are in a 192-byte message? Is this valid for AES?**

60. **Given 5 measurements: [0.084, 0.087, 0.089, 0.085, 0.090] ms, calculate average and range.**

61. **If the A53 clock is 1.2 GHz and R5 is 500 MHz, what is the theoretical clock advantage?**

62. **Calculate elapsed time: start = {tv_sec: 100, tv_nsec: 750000000}, stop = {tv_sec: 101, tv_nsec: 250000000}.**

63. **How many times must the R5 be called to amortize a 2-second setup overhead down to 1%?**

64. **If APU uses 200 mW during AES and takes 0.1 ms, calculate energy. If RPU uses 50 mW but takes 1.2 ms, which uses less energy?**

65. **At what message size might RPU become competitive if communication overhead is fixed at 1 ms and pure computation scales linearly?**

### Critical Thinking & Design Decisions

66. **When would you choose to implement AES on the R5 instead of the A53?**

67. **How could you reduce communication overhead for the RPU implementation?**

68. **Design a scenario where using the R5 for AES would improve overall system performance.**

69. **What are the security implications of sending encryption keys via shared memory?**

70. **How would you modify the system to encrypt multiple messages in a batch?**

71. **Propose optimizations to the RPMsg protocol for this specific use case.**

72. **Discuss the trade-offs between determinism and throughput in choosing A53 vs R5.**

73. **How would caching affect repeated AES operations on the A53?**

74. **Design an experiment to determine the exact communication overhead vs computation time breakdown.**

75. **What would happen if you tried to encrypt a 17-byte message? How would you handle it?**

### Integration & Connections

76. **How does this lab relate to the energy concepts from Lecture 08?**

77. **Explain how Amdahl's Law (from Lecture 09) applies to the RPU implementation.**

78. **Connect the FSM concept from Lab P2 to the state management in remote processor control.**

79. **How will this lab's measurements be used in the future Design Space Exploration lab?**

80. **Relate the modular code structure (aes.c, aes_apu.c, aes_rpu.c) to software paradigms from Lecture 02.**

81. **How does the performance measurement methodology compare to Lab P2's energy measurements?**

82. **Explain how this lab demonstrates the heterogeneous multi-processor concepts that will be covered in Lecture 14.**

83. **Connect the RTOS concepts (FreeRTOS on R5) to what will be learned in Lecture 13.**

84. **How does this software AES implementation provide a baseline for the hardware FPGA implementation in future labs?**

### Debugging & Troubleshooting

85. **The RPU code fails with "No rpmsg device found". What could be wrong?**

86. **Encryption results don't match NIST test vectors. List 3 possible causes.**

87. **The remote processor won't start (state remains "offline"). Describe troubleshooting steps.**

88. **RPU timing measurements seem inconsistent. What factors could cause variability?**

89. **The application crashes when calling AES_RPU_encrypt_buffer(). Diagnose potential issues.**

90. **Why might you see "Permission denied" when accessing /sys/class/remoteproc/?**

### Advanced Analysis

91. **Analyze the complete data flow for a 64-byte encryption showing all memory copies and transitions.**

92. **Calculate the effective throughput (bytes/second) for APU vs RPU given the measured times.**

93. **Discuss how CBC mode's sequential nature impacts multi-core parallelization strategies.**

94. **Analyze the security implications of the Ultra96's shared memory architecture.**

95. **Evaluate whether hardware acceleration (future lab) would eliminate the advantages of using R5.**

96. **Propose a hybrid strategy using both A53 and R5 for optimal system throughput.**

97. **Analyze how Linux scheduler interference affects A53 performance consistency compared to R5.**

98. **Discuss the implications of DES being crackable in 5 minutes (2021) vs 56 hours (1998).**

99. **Evaluate the trade-off between modular code structure and performance in this lab's implementation.**

100. **Design a complete embedded system using this lab's concepts: specify which tasks run on A53 vs R5 and justify your choices.**

