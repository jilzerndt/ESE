# Praktikum 3 - AES Encryption on ARM Cortex-A53 and Cortex-R5

## Overview

This lab exercise demonstrates implementing the AES (Advanced Encryption Standard) encryption algorithm on two different processors within the Xilinx Zynq UltraScale+ MPSoC:
- **ARM Cortex-A53** (Application Processing Unit - APU) running Linux
- **ARM Cortex-R5** (Realtime Processing Unit - RPU) running FreeRTOS via Remote Procedure Calls

## Key Concepts

### 1. AES Encryption Algorithm

**AES (Advanced Encryption Standard)** is a symmetric encryption algorithm that uses the same key for both encryption and decryption. Key features:
- Uses 128-bit, 192-bit, or 256-bit keys (this lab uses 256-bit)
- Operates on 16-byte blocks of data
- Much more secure than the older DES algorithm

**CBC Mode (Cipher Block Chaining):**
- Each plaintext block is XORed with the previous ciphertext block before encryption
- The first block uses an Initialization Vector (IV)
- Prevents patterns in the plaintext from appearing in the ciphertext
- Encryption must be sequential, but decryption can be parallelized

### 2. Remote Procedure Calls (RPC)

A **Remote Procedure** is a function that executes on a separate processor. In this system:
- Main application runs on Cortex-A53 (Linux)
- AES computation runs on Cortex-R5 (FreeRTOS)
- Communication happens via RPMsg (Remote Processor Messaging)

**Why use Remote Procedures?**
- Offload computation to specialized processors
- Better resource utilization
- Potential for better real-time performance

### 3. Communication Architecture

```
┌─────────────────────────────────────┐
│   ARM Cortex-A53 (Linux)            │
│   ┌─────────────────────────────┐   │
│   │  User Application           │   │
│   └──────────┬──────────────────┘   │
│              │                       │
│   ┌──────────▼──────────────────┐   │
│   │  RPMsg/RPRoc Kernel Driver  │   │
│   └──────────┬──────────────────┘   │
└──────────────┼───────────────────────┘
               │ Shared Memory
┌──────────────▼───────────────────────┐
│   ARM Cortex-R5 (FreeRTOS)          │
│   ┌─────────────────────────────┐   │
│   │  RPMsg Handler              │   │
│   └──────────┬──────────────────┘   │
│              │                       │
│   ┌──────────▼──────────────────┐   │
│   │  AES Remote Procedure       │   │
│   └─────────────────────────────┘   │
└─────────────────────────────────────┘
```

## Implementation Details

### Part 1: AES on Cortex-A53 (APU)

**File:** `05_P3/APU/src/main.c`

**What was implemented:**
```c
// Before initialization and encryption
clock_gettime(CLOCK_REALTIME, &time_start);

AES_APU_init_ctx_iv(key, iv);
AES_APU_encrypt_buffer(enc_a, text_length);

// After encryption completes
clock_gettime(CLOCK_REALTIME, &time_stop);
```

**Explanation:**
1. `clock_gettime()` captures high-precision timestamps
2. `CLOCK_REALTIME` provides wall-clock time in seconds and nanoseconds
3. Measurements include:
   - AES context initialization (key expansion)
   - Actual encryption operation

**Key Functions:**
- `AES_APU_init_ctx_iv(key, iv)` - Prepares the AES context:
  1. Expands the encryption key
  2. Sets the initialization vector
- `AES_APU_encrypt_buffer()` - Performs CBC-mode encryption
- `AES_APU_decrypt_buffer()` - Performs CBC-mode decryption

### Part 2: AES on Cortex-R5 (RPU) via Remote Procedure

**Files:**
- `05_P3/RPU/src/aes_rpu.c` - Remote procedure setup/teardown
- `05_P3/RPU/src/main.c` - Main application
- `05_P3/R5_firmware/src/rpmsg_aes.c` - Firmware running on R5

#### Starting the Remote Processor

**File:** `aes_rpu.c` function `AES_RPU_start()`

**Implementation:**
```c
// 1. Write firmware name to the remoteproc system
fp = fopen("/sys/class/remoteproc/remoteproc0/firmware", "w");
fprintf(fp, "%s", firmware);
fclose(fp);

// 2. Start the firmware
fp = fopen("/sys/class/remoteproc/remoteproc0/state", "w");
fprintf(fp, "start");
fclose(fp);
```

**What happens:**
1. Linux's remoteproc framework loads the firmware into R5 memory
2. The R5 processor starts executing the firmware
3. Firmware registers the RPMsg channel
4. Communication channel is established

#### The Communication Protocol

**Data Structure sent to R5:**
```c
typedef struct _aes_datatype {
    unsigned char dec;              // 0 = encrypt, 1 = decrypt
    unsigned char key[32];          // 256-bit AES key
    unsigned char iv[16];           // Initialization vector
    unsigned char text_length;      // Length of data
    unsigned char text[];           // Flexible array for the data
} aes_datatype;
```

**Process:**
1. Prepare data structure with key, IV, and plaintext
2. Write to `/dev/rpmsgX` device (total: 50 bytes + text)
3. R5 receives message, performs encryption
4. Read encrypted result back from `/dev/rpmsgX`

#### Stopping the Remote Processor

**File:** `aes_rpu.c` function `AES_RPU_stop()`

**Implementation:**
```c
// 1. Send shutdown message to firmware
send_shutdown(fd_glob);
close(fd_glob);

// 2. Wait for graceful shutdown
sleep(2);

// 3. Stop the firmware
FILE *fp = fopen("/sys/class/remoteproc/remoteproc0/state", "w");
fprintf(fp, "stop");
fclose(fp);
```

**Why the sleep(2)?**
- Gives the R5 firmware time to clean up resources
- Ensures all pending messages are processed
- Prevents abrupt termination

#### Timing Measurements for RPU

**File:** `main.c`

**Implementation:**
```c
int success = AES_RPU_start("aes_rpu_rtos.elf");

// Measure only the encryption, not setup/teardown
clock_gettime(CLOCK_REALTIME, &time_start);
AES_RPU_encrypt_buffer(key, iv, enc_a, text_length);
clock_gettime(CLOCK_REALTIME, &time_stop);

AES_RPU_decrypt_buffer(key, iv, dec_a, text_length);
success = AES_RPU_stop("aes_rpu_rtos.elf");
```

**Important:** Timing excludes `AES_RPU_start()` and `AES_RPU_stop()` because:
- Start includes firmware loading and channel setup (one-time cost)
- Stop includes a 2-second sleep for graceful shutdown
- We want to measure just the encryption performance

## RPMsg Setup Sequence

The complete setup sequence in `AES_RPU_start()`:

```
1. Write firmware name → /sys/class/remoteproc/remoteproc0/firmware
2. Write "start" → /sys/class/remoteproc/remoteproc0/state
3. Load rpmsg_char kernel module
4. Look up the RPMsg channel (wait up to 300ms if needed)
5. Bind the channel to rpmsg_chrdev driver
6. Open the rpmsg control device
7. Create an endpoint for communication
8. Open the endpoint device (/dev/rpmsgX)
```

## Understanding the Firmware Side

**File:** `R5_firmware/src/rpmsg_aes.c`

The firmware running on the R5 contains:

```c
// Callback function executed when data arrives
int rpmsg_endpoint_cb(struct rpmsg_endpoint *ept, void *data,
                      size_t len, uint32_t src, void *priv)
{
    aes_datatype *aes_data = (aes_datatype *)data;

    if (*((int *)data) == SHUTDOWN_MSG) {
        // Shutdown requested
        return 0;
    }

    // Initialize AES context
    AES_init_ctx_iv(&ctx, aes_data->key, aes_data->iv);

    // Encrypt or decrypt based on flag
    if (aes_data->dec) {
        AES_CBC_decrypt_buffer(&ctx, aes_data->text, aes_data->text_length);
    } else {
        AES_CBC_encrypt_buffer(&ctx, aes_data->text, aes_data->text_length);
    }

    // Send result back
    rpmsg_send(ept, aes_data->text, aes_data->text_length);

    return 0;
}
```

## Testing Different Message Sizes

The code includes three test cases (uncomment to select):

1. **Small (16 bytes):** `"abcdefghijklmno"`
   - Minimum AES block size
   - Tests basic functionality

2. **Medium (64 bytes):** 4 AES blocks
   - Tests CBC chaining across multiple blocks

3. **Large (240 bytes):** 15 AES blocks
   - Tests performance with larger data

## Expected Test Results

The code verifies encryption/decryption by comparing against NIST test vectors:

```
Input:  [plaintext in hex]
Output: [encrypted result]
Soll:   [expected NIST result]

AES encrypt: SUCCESS!
AES decrypt: SUCCESS!
```

## Secret Message

Both versions decrypt a secret message at the end:
```c
uint8_t secret_a[] = { 0x4c, 0x55, 0x44, 0x6f, ... };
```
When decrypted with the NIST test key and IV, this reveals a hidden message!

## Performance Comparison

You should measure and compare:

| Processor | Message Size | Setup Time | Encrypt Time | Stop Time |
|-----------|--------------|------------|--------------|-----------|
| A53 (APU) | 16 bytes     | ~Xms       | ~Xms         | N/A       |
| A53 (APU) | 64 bytes     | ~Xms       | ~Xms         | N/A       |
| A53 (APU) | 240 bytes    | ~Xms       | ~Xms         | N/A       |
| R5 (RPU)  | 16 bytes     | ~Xms       | ~Xms         | N/A       |
| R5 (RPU)  | 64 bytes     | ~Xms       | ~Xms         | N/A       |
| R5 (RPU)  | 240 bytes    | ~Xms       | ~Xms         | N/A       |

**Factors affecting performance:**
- **A53:** Higher clock speed, but also runs Linux and other processes
- **R5:** Lower clock speed, but dedicated to the task with RTOS
- **Communication overhead:** RPMsg adds latency for small messages
- **CBC mode:** Sequential encryption prevents parallelization

## Building and Running

### APU Version:
```bash
cd 05_P3/APU
make
make install  # Copies to Ultra96 board
ssh ese@10.0.0.1
./apu.elf
```

### RPU Version:
```bash
cd 05_P3/RPU
make
make install  # Copies to Ultra96 board

# Copy firmware to /lib/firmware
ssh ese@10.0.0.1
sudo mv aes_rpu_rtos.elf /lib/firmware/
./rpu.elf
```

## Key Takeaways

1. **AES-256 CBC** provides strong encryption using block chaining
2. **Remote Procedures** allow offloading computation to other processors
3. **RPMsg** enables efficient inter-processor communication
4. **Performance trade-offs** exist between different processors:
   - Communication overhead can dominate for small operations
   - Dedicated processors can offer consistent performance
5. **Linux remoteproc framework** simplifies managing remote processors
6. **Timing measurements** must account for setup/teardown costs

## Common Issues and Solutions

**Problem:** "No rpmsg device found"
- **Solution:** Firmware may not have started. Check `/sys/class/remoteproc/remoteproc0/state`

**Problem:** Encryption results don't match
- **Solution:** Ensure you're using the correct NIST test key and IV
- Check that message length is a multiple of 16 bytes

**Problem:** Permission denied when writing to /sys/class/remoteproc
- **Solution:** Need sudo/root privileges for remote processor control

**Problem:** Timing results seem inconsistent
- **Solution:** Run multiple iterations and average the results
- First run may include caching effects

## Study Questions

1. **What are the two steps that `AES_APU_init_ctx_iv()` performs?**
   - Expand/prepare the encryption key
   - Set the initialization vector

2. **How many bytes of data are transferred in the RPMsg communication?**
   - 50 bytes fixed overhead (dec flag + key + IV + length)
   - Plus the text length (variable)

3. **How is encryption vs decryption determined in the remote procedure?**
   - The `dec` flag: 0 = encrypt, 1 = decrypt

4. **How can the remote procedure be shut down?**
   - Send the SHUTDOWN_MSG (0xEF56A55A)
   - Write "stop" to `/sys/class/remoteproc/remoteproc0/state`

5. **Why is there a `sleep(2)` in the stop function?**
   - Allows graceful shutdown of the R5 firmware
   - Ensures all pending messages are processed

## References

- [NIST AES Specification](https://csrc.nist.gov/publications/detail/fips/197/final)
- [CBC Mode Explanation](https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#Cipher_block_chaining_(CBC))
- [Linux Remoteproc Framework](https://www.kernel.org/doc/html/latest/staging/remoteproc.html)
- [RPMsg Protocol](https://www.kernel.org/doc/html/latest/staging/rpmsg.html)

Next Steps                                                                                                                                                                                                                                                                
                                                                                                                                                                                                                                                                            
  To run the lab:                                                                                                                                                                                                                                                           
                                                                                                                                                                                                                                                                            
  1. Build APU version: cd 05_P3/APU && make && make install                                                                                                                                                                                                                
  2. Build RPU version: cd 05_P3/RPU && make && make install                                                                                                                                                                                                                
  3. Copy firmware: sudo mv aes_rpu_rtos.elf /lib/firmware/                                                                                                                                                                                                                 
  4. Run tests with different message sizes (16, 64, 240 bytes)                                                                                                                                                                                                             
  5. Record measurements for comparison 