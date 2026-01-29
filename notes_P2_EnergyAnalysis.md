# Lab P2: Energy Analysis - AES Encryption Measurement

## Technical Terms & Definitions

**AES (Advanced Encryption Standard)**
- Symmetric block cipher encryption algorithm
- Standardized by NIST in 2001
- Used in this lab: AES-256 (256-bit key length)
- Block size: 128 bits (16 bytes)
- Computationally intensive, making it suitable for energy measurement experiments

**Finite State Machine (FSM)**
- Mathematical model of computation consisting of finite number of states
- In this lab: Used to structure the measurement program with distinct operational states
- Transitions between states based on events (timer expiration, operation completion)
- Enables clear separation of different energy consumption profiles

**Sleep Mode (STM32F429)**
- Low-power mode where CPU is stopped but peripherals can continue running
- Function: `power_enter_sleep()`
- Entered using WFI (Wait For Interrupt) instruction
- Fast wake-up time (no re-initialization needed)
- Typical current: ~10-50 μA (depending on active peripherals)

**Stop Mode (STM32F429)**
- Deeper low-power mode than sleep
- Function: `power_enter_stop()`
- All clocks in 1.2V domain stopped
- Voltage regulator in low-power mode
- Contents of SRAM and registers retained
- Typical current: ~1-10 μA
- Wake-up time: longer than sleep mode (clock restart required)

**Wakeup Timer**
- Hardware timer that can wake MCU from sleep/stop mode
- Configured with `wakeup_init(0x3E80)` where 0x3E80 = 16,000 decimal
- With appropriate clock prescaler: results in ~1 second wake-up interval
- Counts down while MCU sleeps
- Generates interrupt when reaching zero

**Measurement Resistor (Shunt Resistor)**
- Low-value precision resistor placed in series with power supply
- In this lab: Replaces jumper JP3 on MC1-Board
- Voltage drop across resistor proportional to current: V = I × R
- Allows non-intrusive current measurement
- Typical value: 0.1Ω to 1Ω (low enough to minimize voltage drop)

**Oscilloscope Probe Configuration**
- Two probe heads required for differential voltage measurement
- Probe 1: Measures voltage on one side of shunt resistor
- Probe 2: Measures voltage on other side of shunt resistor
- Differential measurement: V_shunt = V_probe1 - V_probe2
- Important: Oscilloscope must be configured for differential mode or subtract channels

**NOP (No Operation) Instruction**
- Assembly instruction that does nothing (no operation performed)
- In C: `__asm volatile ("nop");`
- Purpose in this lab: Create baseline active state with minimal computational work
- `volatile` keyword prevents compiler optimization (ensures NOPs execute)
- Used to measure "idle" power consumption when CPU active but not doing useful work

**Clock Speed Configuration**
- STM32F429 supports multiple clock frequencies
- Available in this lab: 60 MHz, 84 MHz, 120 MHz, 168 MHz
- Higher clock → Higher power consumption but faster execution
- Set using: `power_set_clock(CLOCK_168MHZ)`
- Affects both active power and execution time (thus total energy)

**Energy Calculation Formula**
```
E = P × t = V_supply × I × t
where I = V_measured / R_shunt
```
- E: Energy in Joules (J)
- P: Power in Watts (W)
- t: Time in seconds (s)
- V_supply: Supply voltage (typically 3.3V for STM32)
- I: Current in Amperes (A)
- V_measured: Voltage across shunt resistor (from oscilloscope)
- R_shunt: Resistance of measurement resistor (Ω)

**Jumper JP3**
- Jumper on MC1-Board in power supply path
- Must be removed and replaced with measurement resistor for energy measurement
- Critical: Flash programming only works with JP3 or measurement resistor installed
- Without either: No power to MCU, programming fails

**Volatile Keyword (in C)**
- Tells compiler that variable can change unexpectedly
- Prevents optimization that might eliminate code
- Essential for: `for (volatile uint32_t i = 0; i < 8000000; i++)`
- Without volatile: Compiler might optimize away the entire loop
- With volatile: Each iteration executed as written

**Block Cipher**
- Encryption algorithm that operates on fixed-size blocks of data
- AES block size: 128 bits (16 bytes)
- Message must be multiple of block size (padding if necessary)
- Each block encrypted/decrypted independently (in ECB mode) or with chaining (CBC, etc.)

**Cryptographic Overhead**
- Additional computational cost of encryption/decryption
- Includes: Key expansion, substitution, permutation, mixing operations
- Measured as: Extra energy consumed vs. unencrypted data transfer
- In this lab: Comparing AES state vs. IDLE state shows overhead

**Power Profile**
- Graph showing power consumption over time
- Distinct levels visible on oscilloscope for different states
- In this lab: Three levels expected (sleep, idle, AES)
- Duration and height of each level indicates energy consumption

**Energy Profiling**
- Process of measuring energy consumption of different operations
- In this lab: Profiling AES encryption for various message sizes
- Goal: Understand relationship between workload and energy consumption
- Results used for battery life estimation and optimization decisions

**Message Length Dependency**
- AES energy consumption scales with amount of data encrypted
- More blocks → More computational rounds → More energy
- Expected relationship: Linear (Energy ∝ message_size)
- Slope of relationship indicates energy per byte encrypted

**State Transition**
- Change from one FSM state to another
- In this lab: IDLE ↔ AES transitions
- Each transition occurs after completing state operation
- Transitions visible on oscilloscope as power level changes

## Core Concepts

### Lab Overview and Objectives

This lab focuses on practical energy measurement of cryptographic operations on an embedded system. Students implement a finite state machine that alternates between three distinct operational modes (sleep, idle, AES encryption) to measure and compare their energy consumption.

**Primary Learning Goals:**
1. **Practical FSM Implementation**: Design and code a state machine in C for structured program flow
2. **Energy Measurement Methodology**: Use oscilloscope and shunt resistor to measure real-world energy consumption
3. **Low-Power Mode Management**: Configure and utilize MCU sleep modes for power optimization
4. **Oscilloscope Skills**: Configure and interpret voltage waveforms for energy analysis
5. **Cryptography Energy Cost**: Quantify computational cost of AES encryption

**Why This Matters:**
- Battery-operated devices must optimize energy consumption
- Encryption is computationally expensive
- Understanding energy profiles enables informed design decisions
- Real measurements validate theoretical models

### Hardware Measurement Setup

**Configuration Steps:**
1. **Remove Jumper JP3**: Standard jumper on MC1-Board that provides power to MCU
2. **Install Measurement Resistor**: Precision low-value resistor replaces JP3
3. **Connect Oscilloscope Probes**:
   - Two probes needed for differential measurement
   - Probe across resistor (both sides)
   - Differential signal = voltage drop = I × R
4. **Oscilloscope Configuration**:
   - Trigger on voltage level changes (state transitions)
   - Appropriate time scale to capture full cycles (~3-5 seconds)
   - Voltage scale to see all three levels clearly
   - Cursor measurement for precise voltage and time readings

**Measurement Principle:**
```
Power Supply (3.3V) → [Resistor] → MCU
                         ↑
                    Oscilloscope measures V_drop
```

When current flows through resistor to power MCU:
- Voltage drop appears across resistor
- V_drop = I_MCU × R_shunt
- Current calculation: I_MCU = V_drop / R_shunt
- Power calculation: P_MCU = V_supply × I_MCU
- Energy calculation: E = P × duration

**Critical Consideration:**
Measurement resistor must be:
- **Low enough**: Minimize voltage drop (avoid starving MCU of voltage)
- **High enough**: Create measurable voltage drop for oscilloscope
- **Precise**: Accurate resistance value essential for current calculation
- **Low inductance**: Avoid ringing/artifacts in fast current changes

### Finite State Machine Design

**FSM Structure:**
The lab requires a 2-state FSM that cycles continuously:

```
STATE_IDLE → STATE_AES → STATE_IDLE → STATE_AES → ...
```

**State Definitions:**

**1. STATE_IDLE** (Baseline Active State)
- **Purpose**: Measure power when CPU active but doing minimal work
- **Implementation**: Execute NOP instructions in tight loop for ~1 second
- **Code**:
  ```c
  for (volatile uint32_t i = 0; i < 8000000; i++) {
      __asm volatile ("nop");
  }
  ```
- **Why**: Provides baseline for comparison with computational work (AES)
- **Duration**: ~1 second (8,000,000 NOPs at 168 MHz)
- **Power Level**: Medium (CPU running, caches active, but minimal computation)

**2. STATE_AES** (Computational Workload State)
- **Purpose**: Measure power during cryptographic operations
- **Implementation**: Execute AES encryption/decryption 2000 times
- **Code**:
  ```c
  for (uint32_t i = 0; i < 2000u; i++) {
      runAES(aes_message, sizeof(aes_message));
  }
  ```
- **Why**: Represents realistic computational workload
- **Duration**: Variable (depends on message size, typically 1-10 seconds)
- **Power Level**: High (intensive CPU, memory, and ALU usage)

**State Transition Logic:**
```c
switch(state) {
    case STATE_IDLE:
        // Execute NOP loop
        for (volatile uint32_t i = 0; i < 8000000; i++) {
            __asm volatile ("nop");
        }
        state = STATE_AES;  // Transition to AES
        break;

    case STATE_AES:
        // Execute AES operations
        for (uint32_t i = 0; i < 2000u; i++) {
            runAES(aes_message, sizeof(aes_message));
        }
        state = STATE_IDLE;  // Transition back to IDLE
        break;
}
```

**Complete Cycle Sequence:**
```
1. Sleep Mode (1 second)
   ↓ [Wakeup Timer Interrupt]
2. Wake Up
   ↓
3. STATE_IDLE (NOP loop, ~1 second)
   ↓
4. Sleep Mode (1 second)
   ↓ [Wakeup Timer Interrupt]
5. Wake Up
   ↓
6. STATE_AES (2000× AES, variable duration)
   ↓
7. Repeat from step 1
```

**State Diagram:**
```
        ┌──────────────┐
        │  STATE_IDLE  │
        │              │
        │ • Sleep 1s   │
        │ • Wake up    │
        │ • NOP loop   │
        └──────┬───────┘
               │
               │ After NOP loop
               ↓
        ┌──────────────┐
        │  STATE_AES   │
        │              │
        │ • Sleep 1s   │
        │ • Wake up    │
        │ • 2000× AES  │
        └──────┬───────┘
               │
               │ After AES ops
               └───→ Back to IDLE
```

### Sleep Mode Integration

**Why Sleep Mode is Critical:**
- Embedded systems spend most time waiting (sensors, user input, timers)
- Active waiting (polling) wastes enormous energy
- Sleep modes reduce power by 90-99%
- Automatic wakeup enables periodic operations without constant CPU activity

**Sleep Mode in This Lab:**
```c
wakeup_init(0x3E80);     // Configure 1-second wakeup
power_enter_sleep();      // Enter sleep mode
// CPU halts here
// Wakeup interrupt occurs after 1 second
// Execution continues here
```

**What Happens During Sleep:**
1. CPU stops executing instructions
2. Most clocks stopped or reduced
3. Voltage regulator may enter low-power mode (in stop mode)
4. Wakeup timer continues counting
5. When timer expires → Interrupt generated → CPU wakes
6. Execution resumes after `power_enter_sleep()` call

**Power Comparison:**
- **Active (168 MHz)**: ~100-200 mA
- **Sleep Mode**: ~10-50 mA (some peripherals active)
- **Stop Mode**: ~1-10 mA (most peripherals off)
- **Standby Mode**: ~0.001-0.01 mA (not used in this lab)

### AES Encryption Measurement

**AES Algorithm Characteristics:**
- **Symmetric cipher**: Same key for encryption and decryption
- **Block cipher**: Operates on fixed 128-bit blocks
- **Rounds**: AES-256 uses 14 rounds of transformations
- **Operations per round**: SubBytes, ShiftRows, MixColumns, AddRoundKey
- **Computational intensity**: Each operation involves lookups, XOR, multiplication

**runAES() Function:**
```c
uint32_t runAES(char *message, size_t message_length);
```
- Performs both encryption AND decryption (full cycle)
- Message length must be multiple of 16 bytes (AES block size)
- Returns 0 on success, 1 on cipher error, 2 on length error
- Implemented in P02_library.lib (precompiled)

**Why 2000 Iterations?**
- Single AES operation too fast to measure accurately
- 2000 iterations provide sufficient duration (1-10 seconds depending on message size)
- Multiple iterations average out measurement noise
- Still reasonable duration for oscilloscope capture

**Energy Scaling with Message Size:**

For a message of N bytes:
- Number of blocks = N / 16
- AES operations = 2000 × (N / 16) encrypt + 2000 × (N / 16) decrypt
- Total operations ∝ message size
- **Expected**: Linear relationship between message size and energy

**Example Calculation:**
- Message: 256 bytes = 16 blocks
- Iterations: 2000
- Total blocks processed: 2000 × 16 × 2 (enc+dec) = 64,000 blocks
- If 128-byte message: 32,000 blocks (half the energy)

### Energy Calculation Methodology

**Step-by-Step Process:**

**1. Oscilloscope Measurement:**
```
Observe voltage waveform with three distinct levels:
- Level 1 (lowest): Sleep mode
- Level 2 (medium): IDLE state (NOP loop)
- Level 3 (highest): AES state
```

**2. For Each State, Measure:**
- **Voltage**: V_measured across shunt resistor (use cursors)
- **Duration**: t (time state persists)
- **Supply voltage**: V_supply = 3.3V (typical for STM32)

**3. Calculate Current:**
```
I = V_measured / R_shunt
```
Example: V_measured = 0.033V, R_shunt = 0.1Ω
```
I = 0.033V / 0.1Ω = 0.33A = 330 mA
```

**4. Calculate Power:**
```
P = V_supply × I = 3.3V × 0.33A = 1.089 W
```

**5. Calculate Energy:**
```
E = P × t
```
Example: If AES state lasts 4.4 seconds:
```
E = 1.089W × 4.4s = 4.79 J
```

**Complete Example:**

Given measurements from oscilloscope:
- **Sleep**: V = 0.018V, t = 1s, R = 0.1Ω
- **IDLE**: V = 0.030V, t = 1s, R = 0.1Ω
- **AES**: V = 0.033V, t = 4.4s, R = 0.1Ω

Calculations:
```
Sleep:
I = 0.018 / 0.1 = 0.18A = 180 mA
P = 3.3V × 0.18A = 0.594 W
E = 0.594W × 1s = 0.594 J

IDLE:
I = 0.030 / 0.1 = 0.30A = 300 mA
P = 3.3V × 0.30A = 0.99 W
E = 0.99W × 1s = 0.99 J

AES:
I = 0.033 / 0.1 = 0.33A = 330 mA
P = 3.3V × 0.33A = 1.089 W
E = 1.089W × 4.4s = 4.79 J
```

**Per Cycle Energy:**
```
E_total = E_sleep + E_idle + E_sleep + E_aes
E_total = 0.594 + 0.99 + 0.594 + 4.79 = 6.97 J per cycle
```

### Message Size Scaling Analysis

**Experimental Approach:**
Test 5 different message sizes to establish relationship between data size and energy:

**Suggested Message Sizes:**
1. 32 bytes (2 blocks)
2. 64 bytes (4 blocks)
3. 128 bytes (8 blocks)
4. 256 bytes (16 blocks)
5. 512 bytes (32 blocks)

**Implementation:**
Change array size in code:
```c
static char aes_message[32] = {0};   // Test 1
static char aes_message[64] = {0};   // Test 2
static char aes_message[128] = {0};  // Test 3
static char aes_message[256] = {0};  // Test 4
static char aes_message[512] = {0};  // Test 5
```

**Expected Results:**

| Message Size (bytes) | Blocks | Execution Time (s) | Energy (J) |
|---------------------|--------|-------------------|------------|
| 32                  | 2      | ~1.2              | ~1.3       |
| 64                  | 4      | ~2.3              | ~2.5       |
| 128                 | 8      | ~4.4              | ~4.8       |
| 256                 | 16     | ~8.9              | ~9.7       |
| 512                 | 32     | ~17.8             | ~19.4      |

**Linear Relationship:**
```
E_AES = E_baseline + k × message_size

where:
- E_baseline: Setup overhead (small, ~0.1J)
- k: Energy per byte (slope of line)
- Typical k ≈ 0.03-0.04 J/byte for 2000 iterations
```

**Graph:**
```
Energy (J)
    │
 20 │                              ●
    │
 15 │
    │
 10 │                     ●
    │
  5 │            ●
    │      ●
  0 │  ●──────────────────────────────> Message Size (bytes)
    0    64   128   192  256  320  384  448  512
```

**Why Linear?**
- Each block requires same amount of computation
- No caching benefits (message encrypted once then discarded)
- Energy per block approximately constant
- Total energy = (number of blocks) × (energy per block)

**Deviations from Linearity:**
Possible reasons for non-linear behavior:
1. **Cache effects**: First blocks may be slower (cache misses)
2. **Memory alignment**: Unaligned access adds overhead
3. **Measurement noise**: Small messages have higher relative error
4. **Thermal effects**: Prolonged operation may cause temperature-dependent slowdown

## Important Diagrams & Graphics

### 1. Hardware Measurement Setup (Slide/Page Context)
**Description:** Circuit diagram showing measurement configuration

```
VDD (3.3V) ──┬─── Power Supply
             │
          [JP3 removed]
             │
          [Shunt Resistor R]
             │
   Probe+ ───┤
             │
   Probe- ───┤
             │
         STM32F429
         MCU Board
             │
           GND
```

**Components:**
- **Power Supply**: 3.3V from USB or external source
- **JP3 Location**: Jumper position on MC1-Board (normally closed)
- **Shunt Resistor**: 0.1Ω typical, precision resistor
- **Oscilloscope Probes**: Differential connection across resistor
- **MCU**: STM32F429 Discovery Kit on MC1-Board

**Measurement Points:**
- Probe+: Before resistor (higher potential)
- Probe-: After resistor (lower potential)
- V_diff = V_probe+ - V_probe-
- This voltage drop is proportional to current

**Draw:** Circuit schematic with labeled components, showing current path and probe connections

### 2. FSM State Diagram (Task 2)
**Description:** Visual representation of 2-state finite state machine

```
        ┌─────────────────────┐
        │                     │
        │    STATE_IDLE       │
        │                     │
        │  Actions:           │
        │  1. Sleep 1 second  │
        │  2. Wake up         │
        │  3. NOP loop ~1s    │
        │                     │
        └──────────┬──────────┘
                   │
                   │ Condition: After NOP loop completes
                   │ (i >= 8,000,000)
                   ↓
        ┌─────────────────────┐
        │                     │
        │    STATE_AES        │
        │                     │
        │  Actions:           │
        │  1. Sleep 1 second  │
        │  2. Wake up         │
        │  3. 2000× AES ops   │
        │                     │
        └──────────┬──────────┘
                   │
                   │ Condition: After AES operations complete
                   │ (i >= 2000)
                   └─────→ Back to STATE_IDLE
```

**Annotations:**
- Each state includes sleep period (energy saving)
- Transitions are deterministic (completion of operation)
- No external events trigger transitions (unlike typical FSMs)
- Cycle repeats indefinitely

**Draw:** Rectangular boxes for states, arrows for transitions, labeled conditions

### 3. Oscilloscope Waveform - Expected Power Profile
**Description:** Time-domain voltage waveform showing three power levels

```
V (volts)
   │
0.35│                    ┌─────────────┐
    │                    │  AES State  │
0.33│                    │             │
    │                    │             │
0.31│                    └─────────────┘
    │           ┌───┐                       ┌───┐
0.30│           │ I │                       │ I │
    │           │ D │                       │ D │
    │           │ L │                       │ L │
0.28│           │ E │                       │ E │
    │           └───┘                       └───┘
    │  ┌───┐                       ┌───┐
0.18│  │ S │                       │ S │
    │  │ L │                       │ L │
    │  │ E │                       │ E │
    │  │ E │                       │ E │
    │  │ P │                       │ P │
0.00│──┴───┴───────────────────────┴───┴────────────> Time (s)
    0   1   2   3   4   5   6   7   8   9  10  11
```

**Key Features:**
- **Baseline (Sleep)**: Lowest voltage, ~0.18V, duration 1s
- **IDLE (NOP)**: Medium voltage, ~0.30V, duration 1s
- **AES**: Highest voltage, ~0.33V, duration variable (depends on message size)
- **Pattern Repeats**: Sleep → IDLE → Sleep → AES → Sleep → IDLE → ...

**Time Markers:**
- 0-1s: Sleep before IDLE
- 1-2s: IDLE (NOP loop)
- 2-3s: Sleep before AES
- 3-7s: AES (example with 128-byte message)
- 7-8s: Sleep before next IDLE
- Pattern continues...

**Draw:** Line graph with labeled regions, time scale, voltage scale, clear transitions

### 4. Energy vs. Message Size Graph (Task 2.5)
**Description:** Linear relationship between message size and AES energy consumption

```
Energy (J)
    │
 20 │                                        ● (512 bytes, 19.4 J)
    │                                      ╱
    │                                    ╱
 15 │                                  ╱
    │                                ╱
    │                              ╱
 10 │                     ● (256 bytes, 9.7 J)
    │                   ╱
    │                 ╱
  5 │            ● (128 bytes, 4.8 J)
    │          ╱
    │      ● (64 bytes, 2.5 J)
    │    ╱
    │  ● (32 bytes, 1.3 J)
  0 │──┴──────────────────────────────────────────────> Message Size (bytes)
    0   64   128   192   256   320   384   448   512
```

**Linear Fit:**
```
E = 0.038 × message_size + 0.1
```
- Slope: 0.038 J/byte
- Y-intercept: 0.1 J (setup overhead)
- R² ≈ 0.999 (excellent linear fit)

**Interpretation:**
- Each byte adds approximately 0.038 J (for 2000 iterations)
- Per iteration: 0.038 / 2000 = 19 μJ per byte
- Doubling message size doubles energy
- Predictable, allows battery life estimation

**Draw:** Scatter plot with data points, linear trend line, labeled axes, equation shown

### 5. Power Hierarchy Comparison (Conceptual)
**Description:** Bar chart comparing power consumption in different states

```
Power (mW)
    │
1200│                       ┌─────┐
    │                       │     │
1000│                       │     │
    │                       │     │
 800│                       │ AES │
    │                       │     │
 600│                       │     │
    │                       │     │
 400│                       └─────┘
    │          ┌─────┐
 200│          │IDLE │
    │          └─────┘
    │ ┌─────┐
  50│ │Sleep│
    │ └─────┘
  0 └─────────────────────────────────> State
      Sleep   IDLE    AES
```

**Approximate Values:**
- **Sleep**: ~60 mW (180 mA × 3.3V)
- **IDLE**: ~990 mW (300 mA × 3.3V)
- **AES**: ~1090 mW (330 mA × 3.3V)

**Ratios:**
- IDLE vs Sleep: 16.5× more power
- AES vs Sleep: 18× more power
- AES vs IDLE: 1.1× more power (modest increase)

**Key Insight:**
- Sleep mode dramatically reduces power (>90% savings)
- Difference between IDLE and AES is relatively small
- Most energy savings come from sleeping, not optimizing active code

**Draw:** Vertical bar chart with logarithmic or linear scale, labeled bars

### 6. Code Structure - Main Loop Flowchart
**Description:** Program flow diagram showing execution sequence

```
         START
           │
           ↓
    ┌─────────────┐
    │ Initialize  │
    │ Peripherals │
    │ & Variables │
    └──────┬──────┘
           │
           ↓
    ┌─────────────┐
    │ Set Clock   │
    │ to 168 MHz  │
    └──────┬──────┘
           │
           ↓
    ┌─────────────┐
    │ Initialize  │
    │ State = IDLE│
    └──────┬──────┘
           │
           ↓
    ╔═════════════╗
    ║  Main Loop  ║
    ║  (while 1)  ║
    ╚══════┬══════╝
           │
           ↓
    ┌─────────────┐
    │Configure    │
    │Wakeup Timer │
    │  (1 second) │
    └──────┬──────┘
           │
           ↓
    ┌─────────────┐
    │Enter Sleep  │
    │    Mode     │
    └──────┬──────┘
           │
           │ [Wait 1 second]
           │
           ↓
    ┌─────────────┐
    │ Wake Up     │
    │ (Interrupt) │
    └──────┬──────┘
           │
           ↓
    ╔═════════════╗
    ║   Switch    ║
    ║   (state)   ║
    ╚══════┬══════╝
           │
      ┌────┴────┐
      │         │
 STATE_IDLE  STATE_AES
      │         │
      ↓         ↓
  ┌───────┐ ┌────────┐
  │  NOP  │ │  AES   │
  │ Loop  │ │ 2000×  │
  │  ~1s  │ │        │
  └───┬───┘ └────┬───┘
      │         │
      ↓         ↓
  state=AES state=IDLE
      │         │
      └────┬────┘
           │
           └───→ Loop back to Configure Wakeup Timer
```

**Key Decision Points:**
- State determines which operation executes
- After operation, state transitions
- Sleep periods separate active states

**Draw:** Flowchart with standard symbols (rectangles, diamonds, arrows)

### 7. AES Block Processing Visualization
**Description:** How message is divided into blocks for encryption

```
Message (256 bytes)
├─────────┬─────────┬─────────┬─────────┬──────────────────┬─────────┐
│Block 0  │Block 1  │Block 2  │Block 3  │  ...   ...  ...  │Block 15 │
│16 bytes │16 bytes │16 bytes │16 bytes │                  │16 bytes │
└─────────┴─────────┴─────────┴─────────┴──────────────────┴─────────┘
    ↓         ↓         ↓         ↓                             ↓
  ┌───┐     ┌───┐     ┌───┐     ┌───┐                       ┌───┐
  │AES│     │AES│     │AES│     │AES│          ...          │AES│
  └─┬─┘     └─┬─┘     └─┬─┘     └─┬─┘                       └─┬─┘
    ↓         ↓         ↓         ↓                             ↓
Encrypted  Encrypted Encrypted Encrypted                   Encrypted
Block 0    Block 1   Block 2   Block 3                     Block 15
    ↓         ↓         ↓         ↓                             ↓
  ┌───┐     ┌───┐     ┌───┐     ┌───┐                       ┌───┐
  │AES│     │AES│     │AES│     │AES│          ...          │AES│
  │⁻¹ │     │⁻¹ │     │⁻¹ │     │⁻¹ │                       │⁻¹ │
  └─┬─┘     └─┬─┘     └─┬─┘     └─┬─┘                       └─┬─┘
    ↓         ↓         ↓         ↓                             ↓
Decrypted  Decrypted Decrypted Decrypted                   Decrypted
Block 0    Block 1   Block 2   Block 3                     Block 15
```

**For 2000 Iterations:**
```
Total Operations = 2000 × 16 blocks × 2 (encrypt + decrypt)
                 = 64,000 block operations
```

**Energy per Block Operation:**
```
If total energy = 9.7 J for 256 bytes:
Energy per block op = 9.7 J / 64,000 = 0.15 mJ per block operation
```

**Draw:** Block diagram showing message segmentation and processing pipeline

## Formulas & Procedures

### Current Measurement Using Shunt Resistor

**Ohm's Law Application:**
```
V = I × R
Therefore: I = V / R
```

**Given:**
- V_measured: Voltage across shunt resistor (from oscilloscope)
- R_shunt: Resistance of measurement resistor (typically 0.1Ω or 1Ω)

**Calculate:**
```
I_MCU = V_measured / R_shunt
```

**Example 1:**
```
V_measured = 0.033V (from oscilloscope)
R_shunt = 0.1Ω (measurement resistor)

I_MCU = 0.033V / 0.1Ω = 0.33A = 330 mA
```

**Example 2:**
```
V_measured = 0.018V (sleep mode)
R_shunt = 0.1Ω

I_MCU = 0.018V / 0.1Ω = 0.18A = 180 mA
```

### Power Calculation

**Definition:**
```
P = V × I
```

For MCU:
```
P_MCU = V_supply × I_MCU
```

**Given:**
- V_supply: MCU supply voltage (typically 3.3V for STM32)
- I_MCU: Current through MCU (calculated from shunt resistor)

**Calculate:**
```
P_MCU = V_supply × (V_measured / R_shunt)
P_MCU = (V_supply / R_shunt) × V_measured
```

**Example:**
```
V_supply = 3.3V
V_measured = 0.030V (IDLE state)
R_shunt = 0.1Ω

I_MCU = 0.030 / 0.1 = 0.30A

P_MCU = 3.3V × 0.30A = 0.99 W = 990 mW
```

### Energy Calculation

**Definition:**
```
E = P × t = V × I × t
```

**For MCU:**
```
E_MCU = P_MCU × t_duration
```

**Given:**
- P_MCU: Power consumption (calculated above)
- t_duration: Time spent in state (from oscilloscope)

**Calculate:**
```
E_MCU = V_supply × I_MCU × t
E_MCU = V_supply × (V_measured / R_shunt) × t
```

**Example: AES State**
```
V_supply = 3.3V
V_measured = 0.033V
R_shunt = 0.1Ω
t_duration = 4.4s (from oscilloscope)

I = 0.033 / 0.1 = 0.33A
P = 3.3 × 0.33 = 1.089 W
E = 1.089 × 4.4 = 4.79 J
```

### Complete Energy Measurement Procedure

**Step 1: Configure Oscilloscope**
```
1. Channel 1: Connect to one side of shunt resistor
2. Channel 2: Connect to other side of shunt resistor
3. Configure: Math function = CH1 - CH2 (differential)
4. Time scale: 1-2 seconds/division
5. Voltage scale: 10-50 mV/division (depends on R_shunt)
6. Trigger: Rising edge on math channel, ~halfway between levels
```

**Step 2: Run Program and Capture Waveform**
```
1. Flash program to STM32
2. Remove JP3, install measurement resistor
3. Start oscilloscope capture
4. Wait for stable waveform (at least 2-3 cycles)
5. Freeze display (stop acquisition)
```

**Step 3: Measure Each State**
```
For each state (Sleep, IDLE, AES):
1. Use cursor 1: Mark start of state
2. Use cursor 2: Mark end of state
3. Record: ΔV (voltage level) and Δt (duration)
4. Repeat for 3-5 cycles, average results
```

**Step 4: Calculate Energy Per State**
```
For each state:
I = V_measured / R_shunt
P = 3.3V × I
E = P × t_duration

Record in table:
State  | V(mV) | I(mA) | P(mW) | t(s) | E(mJ)
-------|-------|-------|-------|------|------
Sleep  |  18   |  180  |  594  | 1.0  | 594
IDLE   |  30   |  300  |  990  | 1.0  | 990
AES    |  33   |  330  | 1089  | 4.4  | 4790
```

**Step 5: Calculate Total Cycle Energy**
```
E_cycle = E_sleep1 + E_idle + E_sleep2 + E_aes

Example:
E_cycle = 594 + 990 + 594 + 4790 = 6968 mJ = 6.97 J
```

### Message Size Variation Procedure

**Step 1: Prepare Test Cases**
```
Create 5 versions of main.c with different message sizes:
- Version 1: static char aes_message[32] = {0};
- Version 2: static char aes_message[64] = {0};
- Version 3: static char aes_message[128] = {0};
- Version 4: static char aes_message[256] = {0};
- Version 5: static char aes_message[512] = {0};
```

**Step 2: For Each Message Size**
```
1. Compile and flash program
2. Install measurement resistor
3. Capture oscilloscope waveform
4. Measure AES state: V_measured and t_duration
5. Calculate: I, P, E_aes
6. Record in table
```

**Step 3: Data Collection Table**
```
Msg Size | Blocks | V(mV) | t(s) | I(mA) | P(mW) | E(J)
---------|--------|-------|------|-------|-------|------
  32     |   2    |  33   | 1.2  |  330  | 1089  | 1.31
  64     |   4    |  33   | 2.3  |  330  | 1089  | 2.50
 128     |   8    |  33   | 4.4  |  330  | 1089  | 4.79
 256     |  16    |  33   | 8.9  |  330  | 1089  | 9.69
 512     |  32    |  33   | 17.8 |  330  | 1089  | 19.38
```

**Step 4: Linear Regression Analysis**
```
Fit line: E = m × size + b

Using least squares:
m = Σ(size × E) - n×mean(size)×mean(E) / Σ(size²) - n×mean(size)²

From data above:
m ≈ 0.038 J/byte
b ≈ 0.1 J

Therefore: E = 0.038 × size + 0.1
```

**Step 5: Validate Linear Model**
```
Calculate R² (coefficient of determination):
R² = 1 - (SS_res / SS_tot)

where:
SS_res = Σ(E_measured - E_predicted)²
SS_tot = Σ(E_measured - mean(E))²

Expected: R² > 0.99 (excellent fit)
```

### Battery Life Estimation Formula

**Given:**
- Battery capacity: C (mAh)
- Battery voltage: V_bat (V)
- Average power: P_avg (W)

**Convert Battery Capacity to Energy:**
```
E_battery = C × V_bat × 3600 (seconds per hour)
E_battery (in Joules) = (C / 1000) × V_bat × 3600
```

**Example:**
```
C = 2000 mAh
V_bat = 3.7V (Li-ion)

E_battery = (2000/1000) × 3.7 × 3600 = 26,640 J
```

**Calculate Average Power:**
```
From one complete cycle:
E_cycle = 6.97 J (calculated earlier)
T_cycle = 1 + 1 + 1 + 4.4 = 7.4 s

P_avg = E_cycle / T_cycle = 6.97 / 7.4 = 0.942 W
```

**Estimate Battery Life:**
```
Lifetime = E_battery / P_avg

Lifetime (seconds) = 26,640 / 0.942 = 28,280 s
Lifetime (hours) = 28,280 / 3600 = 7.86 hours
```

**Alternatively, Using Current:**
```
I_avg = P_avg / V_supply = 0.942 / 3.3 = 0.285 A = 285 mA

Lifetime = C / I_avg
Lifetime (hours) = 2000 mAh / 285 mA = 7.02 hours

Note: Difference due to voltage conversion (3.7V battery vs 3.3V system)
```

### NOP Loop Duration Calibration

**Purpose:** Determine iterations needed for ~1 second NOP loop

**Formula:**
```
Iterations = Clock_frequency / Cycles_per_iteration

For STM32F429 at 168 MHz:
- 1 NOP instruction ≈ 1 clock cycle
- Overhead (loop, comparison, increment) ≈ ~20 cycles
- Net: ~21 cycles per iteration

Iterations for 1 second:
N = 168,000,000 cycles / 21 cycles ≈ 8,000,000 iterations
```

**Verification:**
```
Measure actual duration on oscilloscope:
t_measured = (end time - start time) for IDLE state

If t_measured ≠ 1.0s:
Adjust iterations: N_new = N_old × (1.0 / t_measured)

Example:
N_old = 8,000,000
t_measured = 1.05s

N_new = 8,000,000 × (1.0 / 1.05) ≈ 7,619,000
```

### AES Iteration Count Selection

**Goal:** Choose iteration count for adequate measurement duration

**Considerations:**
- Too few: Duration too short for accurate oscilloscope measurement (<0.5s)
- Too many: Excessive time, unnecessary (>30s)
- Sweet spot: 2-10 seconds for various message sizes

**Calculation:**
```
For message size M bytes, iterations N:
Total blocks = (M / 16) × N × 2 (encrypt + decrypt)

Execution time estimation:
t_AES ≈ (Total blocks) × (Time per block)

Time per block ≈ 250 μs (typical for STM32F429 at 168 MHz)

Example (256 bytes, 2000 iterations):
Total blocks = (256/16) × 2000 × 2 = 64,000
t_AES = 64,000 × 250μs = 16s (but actual ≈8.9s due to optimizations)
```

**Selection Criteria:**
```
Choose N such that:
2s < t_AES < 15s for largest message size

For 512 bytes max:
Blocks = 32 × 2 = 64 per iteration
Time per iteration ≈ 16 ms
N = 10s / 0.016s ≈ 625 iterations minimum

In lab: N = 2000 chosen for good margin and clear measurement
```

## Comparisons & Contrasts

### Sleep Mode vs. Stop Mode

| Aspect | Sleep Mode | Stop Mode |
|--------|-----------|-----------|
| **Function Call** | `power_enter_sleep()` | `power_enter_stop()` |
| **CPU State** | Halted (WFI) | Halted |
| **Clock Status** | Most clocks running | All 1.2V domain clocks stopped |
| **Voltage Regulator** | Normal mode | Low-power mode |
| **Peripheral State** | Active (many continue) | Stopped (most disabled) |
| **SRAM Retention** | Fully retained | Fully retained |
| **Register Retention** | Fully retained | Fully retained |
| **Wake-up Time** | Very fast (~μs) | Slower (clocks must restart, ~100μs) |
| **Typical Current (STM32F429)** | 10-50 mA | 1-10 mA |
| **Wake-up Sources** | Any interrupt | Limited interrupts, RTC, external |
| **Use Case** | Brief delays with peripheral activity | Long sleep, maximize savings |

**In This Lab:**
- Either mode can be used (`power_enter_sleep` or `power_enter_stop`)
- Both reduce consumption drastically vs. active
- Stop mode saves more energy but wake-up overhead slightly higher
- For 1-second sleep: Stop mode preferred (savings >> overhead)

### IDLE State (NOP) vs. AES State

| Aspect | IDLE (NOP Loop) | AES Encryption |
|--------|----------------|----------------|
| **Purpose** | Baseline active power | Computational workload power |
| **Operation** | Execute NOP instructions | Execute AES algorithm |
| **CPU Usage** | 100% (but minimal work) | 100% (intensive work) |
| **ALU Usage** | Minimal | Heavy (arithmetic, logical ops) |
| **Memory Access** | Minimal (instruction fetch only) | Heavy (data reads/writes, lookups) |
| **Cache Activity** | Low | High (S-box lookups, round keys) |
| **Typical Voltage** | ~0.030V across 0.1Ω shunt | ~0.033V across 0.1Ω shunt |
| **Typical Current** | ~300 mA | ~330 mA |
| **Typical Power** | ~990 mW | ~1089 mW |
| **Duration** | Fixed (~1 second) | Variable (depends on message size) |
| **Energy** | ~990 mJ (fixed) | ~1-20 J (variable) |

**Power Difference:**
- AES is only 10% higher power than IDLE
- Both much higher than sleep
- Main energy cost is being active, not what you're doing

**Key Insight:**
"Hurry up and sleep" strategy:
- Execute AES as fast as possible
- Return to sleep quickly
- Faster execution → Less total energy (less time at high power)

### Message Size: 32 bytes vs. 512 bytes

| Aspect | 32 Bytes | 512 Bytes | Ratio |
|--------|----------|-----------|-------|
| **Blocks** | 2 | 32 | 16× |
| **Operations (2000 iter)** | 8,000 | 128,000 | 16× |
| **Execution Time** | ~1.2s | ~17.8s | 14.8× |
| **Power During AES** | ~1089 mW | ~1089 mW | 1× (same) |
| **Energy (AES only)** | ~1.31 J | ~19.38 J | 14.8× |
| **Energy Efficiency** | 41 mJ/byte | 38 mJ/byte | 0.93× |

**Analysis:**
- Execution time and energy scale almost linearly with message size
- Power consumption remains constant (CPU always running full speed)
- Slight efficiency improvement for larger messages (better amortization of overhead)
- Predictable scaling enables accurate battery life estimation

**Why Linear?**
- Each 16-byte block requires same computation
- No significant caching benefits (message different each time)
- No batch optimization (blocks processed sequentially)
- Fixed-time operations (no data-dependent branches)

### Hardware AES vs. Software AES (Conceptual Comparison)

| Aspect | Software AES (This Lab) | Hardware AES (STM32 Crypto) |
|--------|------------------------|----------------------------|
| **Implementation** | C code in library | Dedicated silicon |
| **Performance** | ~250 μs/block | ~10 μs/block (25× faster) |
| **Power** | Full CPU power (~1089 mW) | Lower (~500 mW, CPU idle) |
| **Energy per Block** | ~270 μJ | ~5 μJ (50× better) |
| **Flexibility** | Can modify algorithm | Fixed algorithm |
| **CPU Availability** | Blocked during AES | Free for other tasks |
| **Code Size** | Large (~10 KB) | Small (driver only) |
| **Cost** | Free (SW only) | Requires HW support |

**Trade-off:**
- Software: Flexible, works on any MCU, but energy-expensive
- Hardware: Efficient, fast, but requires specific MCU with crypto accelerator

**For Battery-Powered IoT:**
- Hardware AES can extend battery life 50× for crypto-heavy applications
- Worth paying premium for MCU with hardware crypto

### Measurement: Multimeter vs. Oscilloscope

| Aspect | Multimeter | Oscilloscope |
|--------|-----------|--------------|
| **Measurement Type** | Average/RMS | Real-time waveform |
| **Time Resolution** | Poor (seconds) | Excellent (μs) |
| **Accuracy** | High (±0.1%) | Moderate (±2-5%) |
| **Dynamic Range** | Limited | Wide (with auto-scaling) |
| **State Separation** | Difficult (averages all) | Easy (visual, cursors) |
| **Setup Complexity** | Simple | Moderate |
| **Data Analysis** | Manual calculation | Visual + cursor measurements |
| **Cost** | Low ($50-500) | Higher ($500-5000) |
| **Best For** | Steady-state, single value | Dynamic, multiple states |

**For This Lab:**
- **Oscilloscope essential**: Need to distinguish sleep/idle/AES states
- Multimeter would only show average (~600 mW), not individual states
- Oscilloscope enables per-state energy calculation

**Measurement Strategy:**
1. Use oscilloscope for dynamic profiling (this lab)
2. Use multimeter for verification of average power
3. Cross-check: Oscilloscope average ≈ Multimeter reading

### Clock Frequency Impact (Conceptual)

| Clock Speed | Execution Time | Power | Energy |
|-------------|---------------|-------|--------|
| **60 MHz** | 2.8× baseline | 0.6× baseline | 1.68× baseline |
| **84 MHz** | 2.0× baseline | 0.75× baseline | 1.5× baseline |
| **120 MHz** | 1.4× baseline | 0.9× baseline | 1.26× baseline |
| **168 MHz** | 1.0× baseline | 1.0× baseline | 1.0× baseline |

**Analysis:**
- Lower clock: Less power but longer execution
- Higher clock: More power but shorter execution
- Energy trade-off: Lower clock usually worse (time increase > power decrease)
- **Optimal for battery**: Highest clock (finish fast, sleep sooner)

**Formula:**
```
Power ∝ Clock frequency (approximately)
Time ∝ 1 / Clock frequency
Energy = Power × Time ∝ Clock × (1/Clock) = constant (theoretically)

In practice: Energy slightly lower at higher clock due to fixed overhead
```

**Conclusion:**
"Hurry up and get back to sleep" strategy wins for energy efficiency.

## Examples & Applications

### Example 1: Complete Energy Measurement Calculation

**Scenario:** Measuring AES energy consumption for 128-byte message with 2000 iterations

**Step 1: Oscilloscope Measurements**
```
Given measurements:
- Shunt resistor: R = 0.1Ω
- Supply voltage: V_supply = 3.3V
- Number of iterations: 2000

Oscilloscope readings:
State  | Voltage across shunt | Duration
-------|---------------------|----------
Sleep  | 18 mV               | 1.0 s
IDLE   | 30 mV               | 1.0 s
Sleep  | 18 mV               | 1.0 s
AES    | 33 mV               | 4.4 s
```

**Step 2: Current Calculations**
```
Sleep:
I_sleep = V / R = 0.018V / 0.1Ω = 0.18 A = 180 mA

IDLE:
I_idle = 0.030V / 0.1Ω = 0.30 A = 300 mA

AES:
I_aes = 0.033V / 0.1Ω = 0.33 A = 330 mA
```

**Step 3: Power Calculations**
```
Sleep:
P_sleep = V_supply × I = 3.3V × 0.18A = 0.594 W = 594 mW

IDLE:
P_idle = 3.3V × 0.30A = 0.99 W = 990 mW

AES:
P_aes = 3.3V × 0.33A = 1.089 W = 1089 mW
```

**Step 4: Energy Calculations**
```
Sleep (1st):
E_sleep1 = P × t = 0.594W × 1.0s = 0.594 J = 594 mJ

IDLE:
E_idle = 0.99W × 1.0s = 0.99 J = 990 mJ

Sleep (2nd):
E_sleep2 = 0.594W × 1.0s = 0.594 J = 594 mJ

AES:
E_aes = 1.089W × 4.4s = 4.79 J = 4790 mJ
```

**Step 5: Total Cycle Energy**
```
E_total = E_sleep1 + E_idle + E_sleep2 + E_aes
E_total = 594 + 990 + 594 + 4790 = 6968 mJ
E_total = 6.97 J per cycle
```

**Step 6: Energy Per AES Operation**
```
2000 iterations performed
Energy attributable to AES (excluding overhead):
E_aes_only = 4.79 J (AES state energy)

Energy per iteration:
E_per_iteration = 4.79 J / 2000 = 2.395 mJ per AES encrypt+decrypt cycle

Energy per block (128 bytes = 8 blocks):
E_per_block = 4.79 J / (2000 × 8 × 2) = 149 μJ per block operation
```

**Step 7: Battery Life Projection**
```
Assuming device encrypts 1 KB every 10 minutes:
- Blocks per day: (1024 / 16) × (60 / 10) × 24 = 9216 blocks
- Energy per day: 9216 × 149 μJ = 1.37 J/day
- Plus sleep energy: Assume 99.9% sleep time
  - Sleep: 86400s × 0.594W × 0.999 = 51,300 J/day
- Total: ~51.3 kJ/day

Battery: 2000 mAh at 3.7V:
- Capacity: 2.0 Ah × 3.7V × 3600s = 26,640 J
- Lifetime: 26,640 / 51,300 = 0.52 days = 12.5 hours

Conclusion: Need larger battery or less frequent encryption!
```

### Example 2: Comparing Two Message Sizes

**Scenario:** Measuring 64-byte vs 256-byte message energy consumption

**64-Byte Message Measurement:**
```
Oscilloscope:
- AES state voltage: 33 mV
- AES state duration: 2.3 s
- Shunt resistor: 0.1Ω

Calculation:
I = 0.033 / 0.1 = 0.33 A
P = 3.3 × 0.33 = 1.089 W
E_64 = 1.089 × 2.3 = 2.50 J
```

**256-Byte Message Measurement:**
```
Oscilloscope:
- AES state voltage: 33 mV (same power level)
- AES state duration: 8.9 s (4× longer, as expected)
- Shunt resistor: 0.1Ω

Calculation:
I = 0.033 / 0.1 = 0.33 A
P = 3.3 × 0.33 = 1.089 W
E_256 = 1.089 × 8.9 = 9.69 J
```

**Comparison:**
```
Ratio of energies: E_256 / E_64 = 9.69 / 2.50 = 3.88×

Expected ratio: 256 bytes / 64 bytes = 4.0×

Difference: 3% (excellent agreement, within measurement error)

Conclusion: Energy scales linearly with message size, confirming theory
```

**Energy Per Byte:**
```
64 bytes:  E_per_byte = 2.50 J / 64 = 39.1 mJ/byte
256 bytes: E_per_byte = 9.69 J / 256 = 37.9 mJ/byte

Small efficiency improvement for larger messages (better amortization of setup overhead)
```

### Example 3: FSM Implementation Walkthrough

**Scenario:** Implementing the 2-state FSM step-by-step

**Step 1: Define State Enumeration**
```c
typedef enum {
    STATE_IDLE,    // Baseline measurement state
    STATE_AES      // Encryption measurement state
} t_state;
```

**Step 2: Declare Global Variables**
```c
// State variable
t_state state = STATE_IDLE;

// AES message buffer (size determines test case)
static char aes_message[256] = {0};  // 256 bytes for this example
```

**Step 3: Initialize System**
```c
int main(void) {
    // Enable peripherals (GPIO, PWR, SYSCFG)
    enable_peripherals();

    // Configure unused pins as output (reduce leakage)
    init_gpio();

    // Set system clock to 168 MHz
    power_set_clock(CLOCK_168MHZ);

    // Initialize power management module
    power_init();

    // Initialize LED output module
    output_init();
    setOutputEnable(ENABLE);

    // Set initial state
    state = STATE_IDLE;
```

**Step 4: Main Loop with FSM**
```c
    while(1) {
        // Configure wakeup timer: wake after 1 second
        // 0x3E80 = 16,000 decimal
        wakeup_init(0x3E80);

        // Enter sleep mode to save energy
        // Execution halts here until wakeup timer expires
        power_enter_sleep();

        // After wakeup, execute state-specific behavior
        switch(state) {
            case STATE_IDLE:
                // Execute NOP loop for ~1 second
                // volatile prevents compiler optimization
                for (volatile uint32_t i = 0; i < 8000000; i++) {
                    __asm volatile ("nop");  // Do nothing
                }
                // Transition to next state
                state = STATE_AES;
                break;

            case STATE_AES:
                // Execute AES encryption/decryption 2000 times
                for (uint32_t i = 0; i < 2000u; i++) {
                    // Encrypt then decrypt message
                    runAES(aes_message, sizeof(aes_message));
                }
                // Transition back to IDLE
                state = STATE_IDLE;
                break;

            default:
                // Should never reach here
                state = STATE_IDLE;  // Safety fallback
                break;
        }
    }

    return 0;  // Never reached
}
```

**Execution Trace (First 3 Cycles):**
```
Cycle 1:
1. state = STATE_IDLE
2. wakeup_init(0x3E80) → Configure 1s timer
3. power_enter_sleep() → Sleep for 1s [LOW POWER]
4. Wakeup interrupt → Resume execution
5. switch(STATE_IDLE) → Execute NOP loop for ~1s [MEDIUM POWER]
6. state = STATE_AES
7. Loop back to step 2

Cycle 2:
8. state = STATE_AES
9. wakeup_init(0x3E80) → Configure 1s timer
10. power_enter_sleep() → Sleep for 1s [LOW POWER]
11. Wakeup interrupt → Resume execution
12. switch(STATE_AES) → Execute 2000× AES [HIGH POWER, 4.4s]
13. state = STATE_IDLE
14. Loop back to step 2

Cycle 3:
15. Repeat from Cycle 1...
```

**Oscilloscope View of Execution:**
```
V (mV)
35 │                          ┌──────────┐
   │                          │   AES    │
30 │                ┌───┐     │          │     ┌───┐
   │                │ I │     │          │     │ I │
   │                │ D │     │          │     │ D │
   │                │ L │     │          │     │ L │
   │                │ E │     │          │     │ E │
20 │   ┌───┐       └───┘     └──────────┘     └───┘
   │   │ S │                                          ┌───┐
   │   │ L │                                          │ S │
   │   │ E │                                          │ L │
   │   │ E │                                          │ E │
   │   │ P │                                          │ E │
10 │   └───┘                                          │ P │
   └────────────────────────────────────────────────────────> Time
      Cyc1.1  Cyc1.2      Cyc2.1    Cyc2.2           Cyc3.1
```

### Example 4: Oscilloscope Configuration

**Scenario:** Setting up oscilloscope for optimal measurement

**Hardware Connections:**
```
Probe 1 (CH1):
- Connect to point before shunt resistor (closer to power supply)
- Ground clip to board ground

Probe 2 (CH2):
- Connect to point after shunt resistor (closer to MCU)
- Ground clip to board ground

Math Channel:
- Configure as CH1 - CH2 (differential)
```

**Oscilloscope Settings:**

**1. Time Base:**
```
- Time/div: 1 second per division
- Total window: 10 seconds (10 divisions)
- Allows viewing 1-2 complete cycles
```

**2. Voltage Scales:**
```
CH1:
- 50 mV/div (for 0.1Ω shunt)
- AC coupling (to remove DC offset)

CH2:
- 50 mV/div (match CH1)
- AC coupling

Math (CH1-CH2):
- 10 mV/div
- Centered at 25 mV (mid-range of expected signals)
```

**3. Trigger:**
```
Source: Math channel (CH1-CH2)
Type: Edge trigger
Slope: Rising
Level: 25 mV (between sleep and idle levels)
Mode: Normal (wait for trigger)
```

**4. Acquisition:**
```
Mode: Sample (not peak detect)
Sample rate: 1 MS/s (overkill but ensures capture)
Memory depth: 10 Mpts (10 second window)
```

**5. Cursors:**
```
Enable time cursors (vertical):
- Cursor 1: Start of measurement region
- Cursor 2: End of measurement region
- Display: ΔX (time difference)

Enable voltage cursors (horizontal):
- Cursor 1: Baseline level
- Cursor 2: Peak level
- Display: ΔY (voltage difference)
```

**6. Measurement Functions:**
```
Enable automatic measurements on Math channel:
- Mean (average voltage over selected region)
- Maximum (peak voltage)
- Minimum (lowest voltage)
- Period (for verification of timing)
```

**Expected Display:**
```
┌──────────────────────────────────────────┐
│ Tektronix MSO5000 Series                 │
│                                          │
│  ┌────────────────────────────────────┐ │
│  │ Math: CH1-CH2                      │ │
│  │                                    │ │
│  │ 33mV─┐              ┌────────┐    │ │
│  │      │              │  AES   │    │ │
│  │      │    ┌──┐      │        │    │ │
│  │ 30mV─┤    │  │      │        │    │ │
│  │      │    └──┘      └────────┘    │ │
│  │      │                            │ │
│  │ 18mV─┴─────                       │ │
│  │    │                              │ │
│  │    Cursor 1   Cursor 2            │ │
│  │    ΔX: 4.4s   ΔY: 33mV            │ │
│  └────────────────────────────────────┘ │
│                                          │
│  Time: 1s/div                            │
│  Math: 5mV/div                           │
│  Measurements:                           │
│  - Peak: 33.2 mV                         │
│  - Mean: 28.5 mV                         │
│  - Period: 7.4 s                         │
└──────────────────────────────────────────┘
```

### Example 5: Linear Regression Analysis

**Scenario:** Analyzing energy vs. message size relationship

**Data Collected:**

| Message Size (x) | Energy (y) | x² | x·y |
|-----------------|------------|---------|---------|
| 32 bytes | 1.31 J | 1,024 | 41.92 |
| 64 bytes | 2.50 J | 4,096 | 160.00 |
| 128 bytes | 4.79 J | 16,384 | 613.12 |
| 256 bytes | 9.69 J | 65,536 | 2,480.64 |
| 512 bytes | 19.38 J | 262,144 | 9,922.56 |
| **Σ** | **992** | **37.67** | **349,184** | **13,218.24** |

**Linear Regression Calculation:**

**Step 1: Calculate Means**
```
n = 5 (number of data points)
mean(x) = Σx / n = 992 / 5 = 198.4 bytes
mean(y) = Σy / n = 37.67 / 5 = 7.534 J
```

**Step 2: Calculate Slope (m)**
```
m = [Σ(xy) - n·mean(x)·mean(y)] / [Σ(x²) - n·mean(x)²]

Numerator:
Σ(xy) - n·mean(x)·mean(y)
= 13,218.24 - 5 × 198.4 × 7.534
= 13,218.24 - 7,475.73
= 5,742.51

Denominator:
Σ(x²) - n·mean(x)²
= 349,184 - 5 × (198.4)²
= 349,184 - 196,883.2
= 152,300.8

Slope:
m = 5,742.51 / 152,300.8 = 0.0377 J/byte
```

**Step 3: Calculate Y-Intercept (b)**
```
b = mean(y) - m × mean(x)
b = 7.534 - 0.0377 × 198.4
b = 7.534 - 7.479
b = 0.055 J
```

**Step 4: Linear Equation**
```
E = 0.0377 × size + 0.055
E(bytes) = 0.0377 × bytes + 0.055 J
```

**Step 5: Validate Fit (R²)**
```
Calculate predicted values:
E_pred(32) = 0.0377 × 32 + 0.055 = 1.26 J
E_pred(64) = 0.0377 × 64 + 0.055 = 2.47 J
E_pred(128) = 0.0377 × 128 + 0.055 = 4.88 J
E_pred(256) = 0.0377 × 256 + 0.055 = 9.71 J
E_pred(512) = 0.0377 × 512 + 0.055 = 19.35 J

Residual sum of squares (SS_res):
SS_res = Σ(E_measured - E_pred)²
= (1.31-1.26)² + (2.50-2.47)² + (4.79-4.88)² + (9.69-9.71)² + (19.38-19.35)²
= 0.0025 + 0.0009 + 0.0081 + 0.0004 + 0.0009
= 0.0128

Total sum of squares (SS_tot):
SS_tot = Σ(E_measured - mean(E))²
= (1.31-7.534)² + (2.50-7.534)² + (4.79-7.534)² + (9.69-7.534)² + (19.38-7.534)²
= 38.75 + 25.34 + 7.53 + 4.65 + 140.25
= 216.52

R² = 1 - (SS_res / SS_tot)
R² = 1 - (0.0128 / 216.52)
R² = 1 - 0.000059
R² = 0.999941

Interpretation: 99.99% of variance explained by linear model (excellent fit!)
```

**Step 6: Prediction**
```
Use equation to predict energy for 1024-byte message:
E(1024) = 0.0377 × 1024 + 0.055
E(1024) = 38.60 + 0.055
E(1024) = 38.66 J

For 2000 iterations at 168 MHz
```

### Example 6: Battery Life Calculation for IoT Device

**Scenario:** Wireless sensor that encrypts data every hour

**System Parameters:**
```
Device: STM32F429-based sensor
Battery: 3× AAA (3600 mAh total at 3.3V effective)
Message size: 128 bytes per transmission
Encryption: AES-256 (using measured values)
Frequency: Once per hour
Operating lifetime target: 1 year
```

**Energy Budget Analysis:**

**Per Transmission (from measurements):**
```
Sleep before IDLE: 594 mJ × 1s = 594 mJ
IDLE state: 990 mJ × 1s = 990 mJ
Sleep before AES: 594 mJ × 1s = 594 mJ
AES encryption: 1089 mW × 4.4s = 4,792 mJ
Total per transmission: 6,970 mJ = 6.97 J
```

**Daily Energy for Encryption:**
```
Transmissions per day: 24
Energy per day (encryption): 24 × 6.97 J = 167 J/day
```

**Daily Sleep Energy (remaining 23 hours):**
```
Total seconds per day: 86,400 s
Encryption time per day: 24 × (1 + 1 + 1 + 4.4) = 24 × 7.4 = 178 s
Sleep time: 86,400 - 178 = 86,222 s

Sleep energy: 86,222 s × 0.594 W = 51,216 J/day
```

**Total Daily Energy:**
```
E_day = E_encryption + E_sleep
E_day = 167 + 51,216 = 51,383 J/day
```

**Battery Capacity:**
```
3× AAA: 3,600 mAh at 3.3V (effective after regulation)
E_battery = 3.6 Ah × 3.3V × 3600 s/h
E_battery = 42,768 J
```

**Battery Lifetime:**
```
Lifetime = E_battery / E_day
Lifetime = 42,768 J / 51,383 J/day
Lifetime = 0.83 days = 19.9 hours

FAILED! Target was 1 year (365 days)
```

**Redesign Options:**

**Option 1: Use Stop Mode Instead of Sleep**
```
Stop mode: 10 mW (vs 594 mW sleep)
Sleep energy: 86,222 s × 0.01 W = 862 J/day

Total daily: 167 + 862 = 1,029 J/day
Lifetime: 42,768 / 1,029 = 41.6 days (still insufficient!)
```

**Option 2: Reduce Transmission Frequency**
```
Every 4 hours instead of every hour:
Transmissions per day: 6
Encryption energy: 6 × 6.97 = 42 J/day
Sleep energy (stop mode): 86,400 s × 0.01 W = 864 J/day
Total: 906 J/day
Lifetime: 42,768 / 906 = 47.2 days (better but still short)
```

**Option 3: Hardware AES Accelerator (if available)**
```
Hardware AES: 5 μJ per block vs 149 μJ (30× improvement)
128 bytes = 8 blocks
Energy per transmission: 8 × 5 μJ × 2000 = 80 mJ (vs 4,792 mJ)

Total per transmission: 594 + 990 + 594 + 80 = 2,258 mJ

Daily (24 transmissions): 24 × 2.258 = 54 J/day
Sleep (stop mode): 864 J/day
Total: 918 J/day
Lifetime: 42,768 / 918 = 46.6 days (marginal improvement)
```

**Option 4: Combined Optimization**
```
- Hardware AES: 80 mJ per transmission
- Stop mode sleep: 10 mW
- Transmit every 4 hours: 6 transmissions/day

Encryption: 6 × 2.258 = 13.5 J/day
Sleep: 86,400 × 0.01 = 864 J/day
Total: 877.5 J/day

Lifetime: 42,768 / 877.5 = 48.7 days

Still only ~50 days! Need larger battery or less frequent transmissions.
```

**Final Solution:**
```
- Hardware AES: 80 mJ per transmission
- Stop mode: 10 mW
- Transmit every 12 hours: 2 transmissions/day
- Larger battery: 3× D cells (15,000 mAh at 3.3V = 178,200 J)

Daily energy: (2 × 2.258) + 864 = 869 J/day
Lifetime: 178,200 / 869 = 205 days ≈ 6.8 months

OR:

- Transmit once per day: 1 transmission/day
- Daily energy: 2.258 + 864 = 866 J/day
- Lifetime: 178,200 / 866 = 206 days ≈ 6.9 months

To reach 1 year:
Required battery: 365 × 866 = 316,090 J
= 26.6 Ah at 3.3V (approximately 6× D cells)
```

**Lesson:** Real-world battery life calculation shows why sleep mode optimization is critical. Even with efficient hardware crypto, continuous operation dominates energy budget.

## Connections to Other Topics

### Connection to Lecture 08: Energy
- **Direct Application**: This lab implements energy measurement techniques taught in Lecture 08
- **Low Power Modes**: Practical use of sleep modes discussed in lecture (slide 29-30)
- **Energy Measurement**: Applies shunt resistor method covered in lecture (slide 39-40)
- **Dynamic Power**: AES encryption demonstrates P = α×C×V²×F formula in practice
- **Energy Profiling**: Creating power profile graphs (lecture slide 38) for AES operations
- **Break-Even Analysis**: Lab data can be used to calculate DPM break-even times (lecture slide 32)
- **Component-Level Analysis**: Measuring MCU energy at module level (lecture slide 10-12)
- **V-Model Integration**: Lab measurements validate energy models (lecture slide 34, 37)

### Connection to Lecture 02: Software Paradigms
- **FSM Implementation**: Demonstrates event-driven state machine pattern from Lecture 02
- **State Pattern**: Two states (IDLE, AES) with state-specific behaviors
- **Interrupt-Driven**: Wakeup timer triggers state transitions via interrupts (no polling)
- **Structured Programming**: Main loop with clear switch-case structure
- **Modular Design**: Separation of concerns (power management, AES, timer modules)
- **Hardware Abstraction**: Using library functions (`runAES`, `power_enter_sleep`) abstracts hardware details

### Connection to Labs U2 & U3: Implementation & Integration
- **FSM Pattern Reuse**: Similar state machine structure to washing machine labs
- **Timer Usage**: Wakeup timer analogous to washing machine cycle timer
- **Hardware Integration**: Real hardware measurement similar to U3 integration testing
- **State Transitions**: Deterministic transitions based on operation completion
- **Testing Approach**: Oscilloscope measurement similar to debugging approach in U3
- **Event Handling**: Timer interrupt as event trigger (similar to button/sensor events in U2/U3)

### Connection to Lecture 04: Modelling
- **State Diagram**: FSM represented as UML state diagram
- **Sequence Diagram**: Could model timing of sleep→wake→execute→sleep sequence
- **Timing Diagrams**: Oscilloscope waveform is real-world timing diagram
- **Energy Modeling**: Measured data validates theoretical energy models
- **Block Diagram**: System components (MCU, shunt, oscilloscope) in measurement architecture

### Connection to Lecture 06: Non-Functional Requirements
- **Energy as NFR**: Lab quantifies energy consumption NFR
- **Measurable NFR**: Energy is quantifiable (Joules), not qualifiable
- **Verification**: Measurements verify if energy NFR is met
- **Trade-offs**: Energy vs. Performance (encryption speed vs. power consumption)
- **ISO/IEC 25010**: Energy efficiency is Performance Efficiency quality attribute
- **Requirements Testing**: Lab is verification activity on right side of V-model

### Connection to Lecture 05: Development Processes
- **V-Model Right Side**: Lab is system integration and testing phase
- **Verification Activity**: Measurements verify implementation meets energy specifications
- **Iterative Refinement**: Multiple message sizes show iterative testing approach
- **Documentation**: Lab report documents energy characteristics for design decisions
- **Test-Driven**: Measurement-driven validation of energy requirements

### Connection to Lecture 01: Embedded Systems Characteristics
- **Resource Constraints**: Energy measurement quantifies fundamental resource constraint
- **Real-Time**: Timing requirements impact energy (faster execution → return to sleep)
- **Efficiency**: Lab demonstrates importance of efficiency in embedded systems
- **Hardware-Software Co-Design**: Software FSM with hardware sleep modes
- **Cost Sensitivity**: Energy impacts battery size, thus product cost

### Connection to Future Lecture 09: Performance
- **Performance-Energy Trade-off**: Faster AES (higher clock) uses more power but less energy
- **Execution Time Measurement**: Oscilloscope shows AES execution time
- **Throughput**: Blocks per second vs. energy per block trade-off
- **Optimization**: Compiler optimization affects both performance and energy
- **Profiling**: Energy profiling analogous to performance profiling

### Connection to Future Lectures 10-12: FPGAs & Design Space Exploration
- **Hardware Acceleration**: AES could be implemented in FPGA for better energy efficiency
- **Design Alternatives**: Software AES (this lab) vs. hardware AES (future labs)
- **DSE Dimensions**: This lab measures energy dimension of design space
- **Pareto Front**: Trade-off between cost (software free, FPGA expensive) and energy
- **Multi-Objective Optimization**: Minimize energy while meeting performance requirements

### Connection to Future Lecture 13: RTOS Scheduling
- **Task Scheduling**: FSM states could be RTOS tasks with different priorities
- **Idle Task**: OS idle task should enter low-power mode (demonstrated here)
- **Tickless Idle**: RTOS feature avoiding periodic timer wakeups (saves energy)
- **Energy-Aware Scheduling**: Schedule tasks to maximize sleep time
- **Real-Time Constraints**: Wakeup latency must meet task deadlines

### Connection to Future Lab P4: AES in Hardware (FPGA & CSU)
- **Baseline Comparison**: This lab provides software AES baseline for comparison
- **Energy Improvement**: Hardware AES expected to be 10-100× more energy efficient
- **Same Algorithm**: Can directly compare SW vs. HW implementation energy
- **Performance Comparison**: Hardware AES will be faster (time measurement)
- **Design Decision**: Lab P2 + P4 data enables informed SW vs. HW choice

### Connection to Cryptography Concepts (General Knowledge)
- **AES-256**: Industry-standard symmetric encryption
- **Block Cipher Mode**: Lab uses ECB (each block independent)
- **Key Size Impact**: 256-bit key requires 14 rounds (more energy than AES-128's 10 rounds)
- **Encrypt+Decrypt**: Measuring full cycle (realistic for verification/authentication)
- **Side-Channel Attacks**: Power measurement could potentially reveal information about operations (not explored in this lab but relevant security topic)

## Summary

Lab P2 provides hands-on experience with energy measurement and analysis in embedded systems. Students implement a finite state machine to alternate between sleep, idle, and AES encryption states, enabling clear comparison of energy consumption. Using oscilloscope and shunt resistor measurement, students quantify energy for different message sizes and observe linear scaling relationship.

**Key Takeaways:**
1. **Sleep modes are critical**: 90%+ power savings vs. active modes
2. **Measurement methodology**: Shunt resistor + oscilloscope enables per-state energy calculation
3. **FSM for structure**: State machine provides clear separation of operational modes
4. **Linear scaling**: AES energy consumption directly proportional to message size
5. **Battery life**: Real measurements enable accurate battery sizing decisions
6. **Optimization priorities**: Maximizing sleep time more important than optimizing active code
7. **Practical skills**: Oscilloscope configuration, energy calculation, data analysis

The lab bridges theory (Lecture 08) with practice, demonstrating that energy optimization is essential for battery-operated embedded systems.

## Possible Exam Questions

### Basic Understanding (Definitions & Concepts)

1. What is a Finite State Machine (FSM)? How many states does the FSM in Lab P2 have, and what are they called?

2. Define the difference between "sleep mode" and "stop mode" on the STM32F429. Which saves more energy?

3. What is a shunt resistor and how is it used to measure current in this lab?

4. Explain what a NOP (No Operation) instruction does and why it's used in the IDLE state.

5. What does the `volatile` keyword do in C, and why is it essential for the NOP loop?

6. Describe the purpose of jumper JP3 on the MC1-Board and what replaces it during energy measurement.

7. What is the wakeup timer and how does it enable periodic operation without continuous CPU activity?

8. Define "power profile" and describe the three distinct power levels expected in this lab.

9. What is the block size for AES encryption? How many blocks are in a 256-byte message?

10. Explain why two oscilloscope probes are required for the measurement in this lab.

### Conceptual Understanding (Explain & Describe)

11. Explain the complete execution cycle of the FSM: starting from STATE_IDLE, describe all steps until returning to STATE_IDLE again.

12. Describe the relationship between message size and energy consumption for AES encryption. Why is it linear?

13. Explain why the lab implements both IDLE and AES states. What is the purpose of comparing these two states?

14. Describe the energy calculation procedure: from oscilloscope voltage measurement to final energy in Joules.

15. Explain why "hurry up and get back to sleep" is an effective energy optimization strategy.

16. Describe what happens during sleep mode: which components are active, which are stopped, and how does the MCU wake up?

17. Explain why the runAES() function performs both encryption AND decryption. What does this represent in real-world applications?

18. Describe the trade-off between clock frequency and energy consumption. Is it better to run fast or slow?

19. Explain why unused GPIO pins are configured as outputs in the initialization code.

20. Describe the difference between active power consumption during IDLE vs. AES states. Why is the difference relatively small?

### Application & Analysis

21. Given an oscilloscope measurement of 0.033V across a 0.1Ω shunt resistor for 4.4 seconds with 3.3V supply, calculate the total energy consumed.

22. A system with the measured power profile runs on a 2000 mAh @ 3.3V battery. Calculate the battery lifetime if it transmits once per hour.

23. You measure 30 mV for IDLE and 33 mV for AES across a 0.1Ω shunt. Calculate the power increase (in %) between IDLE and AES states.

24. Compare energy consumption for 64-byte vs. 512-byte messages. If 64-byte takes 2.3s and consumes 2.50J, predict energy for 512-byte message.

25. The lab uses 2000 AES iterations. Explain why this number was chosen. What would happen if only 10 iterations were used?

26. Given linear relationship E = 0.038×size + 0.1, calculate energy for a 1024-byte message. How long would this take at 1089 mW?

27. You observe a waveform with sleep=18mV, idle=30mV, aes=33mV. Calculate the current for each state with R=0.1Ω.

28. If stop mode (10mW) replaces sleep mode (594mW), calculate the new battery lifetime for the hourly transmission scenario.

29. Compare total energy per cycle for 128-byte vs. 256-byte messages. What percentage of energy is spent in sleep vs. AES states?

30. Calculate energy per byte for 128-byte (4.79J) and 512-byte (19.38J) messages. Why is per-byte energy slightly lower for larger messages?

### Design & Decision Making

31. You need to design a sensor that encrypts 64 bytes every 10 minutes and must last 6 months on 2× AA batteries. Based on lab measurements, is this feasible? Show calculations.

32. Design an oscilloscope configuration for measuring this lab: specify channels, voltage scales, time scales, trigger settings, and cursor usage.

33. Choose between sleep and stop mode for a system that wakes every 100ms to read a sensor. Justify your choice considering wake-up overhead.

34. You can choose between AES-128 (10 rounds) and AES-256 (14 rounds). Estimate the energy difference and recommend which to use for battery-powered IoT device.

35. Design an FSM with 3 states: IDLE, AES_ENCRYPT, AES_DECRYPT to measure encryption and decryption separately. Draw state diagram and explain transitions.

36. The lab uses 168 MHz clock. Would changing to 84 MHz save energy? Explain your reasoning considering power vs. execution time trade-off.

37. You need to reduce AES energy consumption by 50%. List three approaches (hardware/software) and estimate their effectiveness based on lab concepts.

38. Design a measurement procedure to determine the exact break-even time for entering sleep mode. What equipment and steps would you use?

39. Compare two battery options: 1× CR2032 (220mAh @ 3V) vs. 2× AAA (1200mAh @ 3V). Which provides longer lifetime? Consider cost and size trade-offs.

40. You observe that AES power varies between 30mW and 35mW during execution. Propose three possible causes and how to investigate each.

### Critical Thinking & Trade-offs

41. Discuss the trade-off between measurement accuracy and system perturbation when choosing shunt resistor value. What value would you recommend?

42. The lab uses software AES from a library. Compare energy trade-offs between: (1) software AES, (2) hardware AES accelerator, (3) external crypto IC.

43. Explain why optimizing the AES algorithm itself might provide less energy savings than simply using sleep mode more effectively.

44. Discuss the limitations of using oscilloscope cursors for energy measurement. What systematic errors might occur?

45. The lab achieves only ~10% power difference between IDLE and AES. Does this mean AES encryption is "free"? Explain the nuance.

46. Compare the energy measurement approach in this lab (oscilloscope + shunt) vs. using a power analyzer. What are the pros and cons of each?

47. Discuss the impact of temperature on energy measurements. How might results differ between 20°C and 70°C?

48. The lab uses fixed 2000 iterations. Propose an adaptive approach that adjusts iterations based on message size for consistent measurement duration.

49. Explain why battery lifetime calculations in this lab are optimistic. What real-world factors reduce actual battery life?

50. Discuss the security vs. energy trade-off: AES-128 vs. AES-256 vs. no encryption. How would you make this decision for a medical sensor?

### Integration & Synthesis

51. Describe how you would use the FSM pattern from this lab combined with the hardware abstraction approach from U2/U3 labs to create a portable energy measurement framework.

52. Design a complete energy budget for an IoT device that includes: sensor reading (50mJ), AES encryption (from lab), wireless transmission (100mJ), and sleep (0.5mW continuous).

53. Explain how the linear relationship E = m×size + b can be used in design space exploration when choosing between different encryption algorithms.

54. Create a test plan that uses energy measurements from this lab to verify an NFR: "Device shall operate 180 days on 2× AA batteries with hourly transmissions."

55. Describe how you would extend this lab to measure energy consumption of different AES modes (ECB, CBC, CTR) and predict which would be most efficient.

### Advanced Topics & Real-World Scenarios

56. You port this code to a Cortex-M0+ running at 48 MHz. How would you adjust the NOP loop iteration count? What energy differences would you expect?

57. Design an energy-aware task scheduler that uses sleep modes between tasks. How would you determine optimal task ordering?

58. Propose a side-channel attack based on the power measurements from this lab. What information could an attacker learn from the power profile?

59. Implement dynamic voltage and frequency scaling (DVFS): measure AES energy at 60 MHz, 84 MHz, 120 MHz, 168 MHz. Predict which is most energy-efficient.

60. Design a system that adaptively chooses between software and hardware AES based on message size, using lab measurements to determine the crossover point.

