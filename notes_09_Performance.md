# Lecture 09: Performance and Optimization

## Technical Terms & Definitions

### Performance Metrics

**Execution Time**: Total time required for a program or function to complete its operations. Important for meeting deadlines and returning to low-power mode sooner.

**Response Time (Latency)**: Time elapsed between a request and the system's response. Critical for user experience and hard real-time systems.

**Memory Footprint**: Total amount of memory used by a program, including:
- **Code Size**: Size of the compiled program binary
- **Data Size**: Size of program data structures and variables
- Must fit into the microcontroller's available memory

**CPU Utilization**: Percentage of CPU time actively used by tasks. Important for:
- Creating feasible schedules in RTOS
- Determining interrupt service time
- Ensuring headroom for future features

**Working Set**: The subset of program data actively being used at any given time, ideally fitting within cache for optimal performance.

### Bottlenecks

**Bottleneck**: A section of code or system resource that limits overall system performance, like the narrow neck of a bottle limiting flow.

**Bad Code**: Inefficient implementation patterns such as polling instead of using interrupts, resulting in wasted CPU cycles.

**Memory Thrashing**: Excessive cache misses due to poor memory access patterns, causing frequent reloading of data from slower main memory.

**Polling**: Continuously checking a condition in a loop (wastes CPU cycles vs. interrupt-driven approach).

### Profiling

**Profiling**: Systematic measurement and analysis of program execution to identify performance bottlenecks and resource-hungry functions.

**Performance Monitor Unit (PMU)**: Built-in hardware in ARM Cortex processors for counting performance events:
- Bus cycles and accesses
- Cache hits/misses/allocations
- Memory accesses
- CPU cycles
- Load and store instructions
- Branch predictions and pipeline flushes

**Embedded Trace Macrocell (ETM)**: ARM hardware component that provides real-time instruction trace for debugging and profiling.

**Flame Graph**: Visualization showing call stack over time, where:
- Width represents execution time (wider = longer)
- Height represents call stack depth
- Helps identify which functions consume most time

**Hardware Profilers**: Tools like Segger J-Trace and Lauterbach Trace32 that use ETM for live, non-intrusive profiling.

**Software Profilers**: Tools like gprof, Oprofile, and valgrind that add instrumentation to code for profiling (introduces overhead).

**GPIO Profiling**: Simple profiling technique using GPIO pins to signal function entry/exit, measurable with oscilloscope.

### Optimization Techniques

**Amdahl's Law**: Formula calculating overall speedup when improving only a portion of a program:
- S_overall = 1 / ((1-f) + f/a)
- f = fraction of runtime improved
- a = speedup factor for that fraction
- Demonstrates diminishing returns of optimization

**Loop Unrolling**: Optimization technique that reduces loop overhead by:
- Executing multiple iterations per loop cycle
- Reducing condition checks and branches
- Updating loop counter less frequently
- Trade-off: increases code size

**Horner's Rule**: Efficient algorithm for polynomial evaluation that minimizes multiplications by factoring:
- Converts a₄x⁴ + a₃x³ + a₂x² + a₁x + a₀
- Into ((((a₄·x + a₃)·x + a₂)·x + a₁)·x + a₀)
- Reduces from 10 multiplications to 4

**Inline Functions**: Functions whose code is inserted directly at call site instead of using subroutine call:
- Eliminates call overhead (parameter passing, stack frame)
- Increases code size
- Compiler usually decides automatically

**Spatial Locality**: Principle that data near recently accessed data is likely to be accessed soon (sequential array access).

**Temporal Locality**: Principle that recently accessed data is likely to be accessed again soon.

### Vector Processing (SIMD)

**SIMD (Single Instruction, Multiple Data)**: Parallel processing where one instruction operates on multiple data elements simultaneously.

**ARM NEON**: ARM's SIMD instruction set extension that:
- Adds 32x 128-bit registers for vector operations
- Can process 16x int8, 8x int16, 4x int32, or 2x double per instruction
- Significantly accelerates data-parallel operations

**Vectorization**: Process of converting scalar operations to vector operations:
- Can be done manually using NEON intrinsics
- Or automatically by compiler (auto-vectorization)

**Intrinsics**: Function calls in C that map directly to specific assembly instructions (like NEON instructions).

**Auto-vectorization**: Compiler optimization that automatically recognizes vectorizable code patterns and generates SIMD instructions.

**Vector Register**: Wide register (128-bit in NEON) holding multiple data elements for parallel processing.

### Compiler Optimization

**Optimization Flags**:
- **-O0**: No optimization (debugging)
- **-O1**: Basic optimization
- **-O2**: Moderate optimization including auto-vectorization
- **-O3**: Aggressive optimization including loop unrolling
- **-Os**: Optimize for reduced code size
- **-Oz**: Optimize for smallest possible code size

**Optimization Levels**: Different degrees of compiler optimization trading compilation time, code size, and execution speed.

### Other Terms

**Subroutine Overhead**: Time and resources consumed by function calls:
- Parameter passing (register or stack)
- Saving/restoring registers
- Link register management
- Stack frame allocation/deallocation

**Stack Frame**: Memory structure on the stack storing local variables, saved registers, return address, and parameters for a function call.

**Link Register**: Special register storing return address for function calls (LR on ARM).

**Branch Misprediction**: When CPU incorrectly predicts branch direction, requiring pipeline flush (performance penalty).

**Pipeline Flush**: Clearing the instruction pipeline when a branch misprediction occurs, wasting cycles.

**Context Switch**: OS operation saving one task's state and loading another's (does not preserve cache contents).

**Boolean Short-circuit Evaluation**: Optimization where Boolean expressions stop evaluating once result is determined (A || B stops if A is true).

**Library Functions**: Pre-written, optimized functions provided by libraries (should be profiled/benchmarked before use).

**Feasible Schedule**: In RTOS, a task schedule where all tasks can meet their deadlines.

---

## Core Concepts

### Performance Optimization Process

**The Six Steps for Optimization:**
1. **Set Clear Performance Goals**: Define specific, measurable targets (execution time, latency, memory limits)
2. **Identify Bottlenecks**: Find code sections limiting performance (bad code, inefficient algorithms, hardware limitations)
3. **Profile the Code**: Measure actual performance to identify resource-hungry functions
4. **Optimize Algorithms and Data Structures**: Use efficient algorithms, avoid unnecessary data transfers
5. **Use the Caches**: Respect spatial/temporal locality, keep working set in cache
6. **Use Parallelization/Concurrency**: SIMD, interrupts, RTOS tasks, multiple cores

**Why Profile Instead of Speculate:**
- Human intuition about bottlenecks is often wrong
- Profiling provides objective data
- Amdahl's Law shows small improvements to minor sections yield little benefit
- Focus optimization effort where it matters most

### Amdahl's Law: Understanding Speedup Limits

**The Law:**
```
S_overall = T_orig / T_new = 1 / ((1-f) + f/a)

Where:
- S_overall = overall speedup factor
- f = fraction of runtime spent in improved section
- a = speedup factor for that section
- (1-f) = fraction of runtime unchanged
```

**Improvement Percentage:**
```
improvement = 100% × (1 - 1/S_overall)
```

**Key Insights:**
1. **Fraction f is Critical**: Only large f values (>70%) allow significant overall speedup
2. **Acceleration Factor a is Secondary**: Improving by 10x or 1000x matters little if f is small
3. **Diminishing Returns**: Even infinite speedup (a→∞) limited by: S_overall = 1/(1-f)
4. **Profile First**: Identify high-f sections before investing optimization effort

**Example 1:** Improve 10% of runtime (f=0.1) by 10,000x (a=10,000):
```
S_overall = 1 / (0.9 + 0.1/10000) = 1 / 0.90001 ≈ 1.11
improvement = 100% × (1 - 1/1.11) ≈ 9.9%
```
Despite 10,000x improvement, overall speedup only ~11%!

**Example 2:** Improve 90% of runtime (f=0.9) by 100x (a=100):
```
S_overall = 1 / (0.1 + 0.9/100) = 1 / 0.109 ≈ 9.17
improvement = 100% × (1 - 1/9.17) ≈ 89%
```
Much better! Focus on the right bottlenecks.

**Multicore Implications:**
- Better parallelization (higher f) enables better scaling
- Inter-processor communication overhead reduces effective f
- Large datasets needed to justify many cores for single task

### Cache Optimization

**Why Cache Matters:**
- Cache is 10-100x faster than main memory
- Cache misses stall the CPU waiting for data
- Good cache utilization crucial for performance

**Optimization Strategies:**

**1. Respect Spatial Locality:**
- Access data sequentially when possible
- Entire cache line (typically 64 bytes) loaded on miss
- Sequential access maximizes cache line utilization

**2. Respect Temporal Locality:**
- Once data is loaded, perform ALL operations on it
- Avoid loading, storing, then reloading same data
- Combine subsequent loops over same data range

**3. Data Structure Choice:**
```c
// Array of Structures (AoS) - Bad for selective access
struct multichannel_t {
    uint16_t ch_a, ch_b, ch_c;
} data[128];

// Accessing only ch_a loads all three channels into cache
for(int i=0; i<128; i++) {
    process(data[i].ch_a);  // Wastes 2/3 of each cache line
}

// Structure of Arrays (SoA) - Good for selective access
struct multichannel_array_t {
    uint16_t ch_a[128];
    uint16_t ch_b[128];
    uint16_t ch_c[128];
} data;

// Accessing ch_a only loads ch_a into cache
for(int i=0; i<128; i++) {
    process(data.ch_a[i]);  // Efficient cache usage
}
```

**4. Know Your Hardware:**
- Cortex-A53: 32 KB L1 data cache, 256-2048 KB L2 cache
- Keep working set within L1 cache if possible
- L2 cache can preserve data across context switches

**5. Multitasking Considerations:**
- Context switches don't preserve L1 cache
- L2 cache may retain working set between task switches
- Minimize data footprint for better cache retention

### Algorithm Optimization

**Principle:** Choose algorithmically efficient solutions before micro-optimizations.

**Example: Polynomial Evaluation**
```c
// Naive approach: y = a₄x⁴ + a₃x³ + a₂x² + a₁x + a₀
// Requires: 10 multiplications, 4 additions
y = a4*x*x*x*x + a3*x*x*x + a2*x*x + a1*x + a0;

// Horner's Rule:
// d₀ = a₄
// d₁ = x·d₀ + a₃
// d₂ = x·d₁ + a₂
// d₃ = x·d₂ + a₁
// d₄ = x·d₃ + a₀
// Requires: 4 multiplications, 4 additions (60% fewer operations!)
double d = a4;
d = x*d + a3;
d = x*d + a2;
d = x*d + a1;
d = x*d + a0;
```

**Data Transfer Minimization:**
- Loading/storing data costs energy and cycles
- Load data once, perform all operations, then store result
- Compiler usually handles this, but can be helped:
  - Combine two sequential loops over same data into one
  - Avoid unnecessary intermediate storage

**Library Functions:**
- Profile them! Someone may have optimized already
- But some libraries prioritize generality over performance
- Benchmark before committing

### Loop Optimization

**1. Simple Termination Conditions:**
```c
// Bad - function call every iteration
while(get_length() > i) { ... }

// Good - evaluate once
int length = get_length();
while(length > i) { ... }
```

**2. Boolean Condition Ordering:**
```c
// OR (||): Put most likely TRUE condition first
if(common_condition || rare_condition) { }
// If common_condition is true, rare_condition not evaluated

// AND (&&): Put most likely FALSE condition first
if(rare_condition && common_condition) { }
// If rare_condition is false, common_condition not evaluated
```

**3. Loop Unrolling:**
```c
// Original - 32 iterations, 32 condition checks
for(int i=0; i<32; i++) {
    if(n & 1) bits++;
    n >>= 1;
}

// Unrolled - 8 iterations, 8 condition checks (4x fewer)
while(n != 0) {
    if(n & 1) bits++;
    if(n & 2) bits++;
    if(n & 4) bits++;
    if(n & 8) bits++;
    n >>= 4;
}
```

**Unrolling Trade-offs:**
- ✅ Fewer branch instructions and condition checks
- ✅ Less frequent loop counter updates
- ✅ Better instruction-level parallelism
- ❌ Significantly larger code size
- 💡 Let compiler do it with `-O3`

### Subroutine Call Optimization

**Call Overhead Includes:**
1. **Parameter Passing:**
   - First 4 params in R0-R3 (Cortex-R5) or 8 params in X0-X7 (Cortex-A53)
   - Additional params pushed to stack
   - Registers are cheapest

2. **Stack Frame Management:**
   - Save caller-saved registers
   - Save link register (LR)
   - Allocate space for local variables
   - All must be undone on return

3. **Branch Instruction:**
   - Potential pipeline flush if mispredicted

**Optimization Strategies:**
1. **Limit Parameters:** Keep ≤4 (ARM32) or ≤8 (ARM64) for register passing
2. **Inline Small Functions:** Use `inline` keyword for frequently-called small functions
3. **Worth the Overhead:** Ensure subroutine does significant work relative to call cost

### Vector Processing (SIMD)

**Concept:** Process multiple data elements with single instruction.

**ARM NEON Capabilities:**
```
128-bit register can hold:
- 16× int8_t   (16 bytes)
- 8× int16_t   (8 half-words)
- 4× int32_t   (4 words)
- 2× int64_t   (2 double-words)
- 4× float     (4 single-precision)
- 2× double    (2 double-precision)
```

**Example: Vector Addition**
```c
// Scalar version - processes 1 element per instruction
for(int i=0; i<100; i++) {
    vec_C[i] = vec_A[i] + vec_B[i];
}

// NEON version - processes 4 floats per instruction
// Automatically done by compiler with -O2 or -O3
void vec_add(float *vec_A, float *vec_B, float *vec_C, int len_vec) {
    for(int i=0; i<len_vec; i++) {
        vec_C[i] = vec_A[i] + vec_B[i];
    }
}
// Compiler generates: FADD v0.4S, v0.4S, v1.4S
// Processes 4 floats simultaneously!
```

**Auto-vectorization Requirements:**
- No dependencies between loop iterations
- Simple loop conditions (no complex branches)
- Array sizes known or multiples of vector width
- Enabled with `-O2` or `-O3`

**Helping the Compiler:**
```c
// Tell compiler vector is multiple of 4
for(int i=0; i<(len_vec & ~3); i++) {
    // Eliminates code for handling remainder elements
    // Reduces code size and overhead
}
```

**Vectorization Trade-offs:**
- ✅ 2x to 16x speedup depending on data type
- ❌ Increased code size (setup overhead, remainder handling)
- ❌ Separate pipeline for NEON (switching overhead)
- ✅ Can reduce unrolled loop code size
- 💡 Best for large data sets and simple operations

### Profiling Techniques

**Hardware-Based Profiling:**
- **Segger J-Trace / Lauterbach Trace32:**
  - Non-intrusive (no overhead)
  - Real-time trace using ETM
  - Function call graphs, execution time
  - Expensive specialized hardware

- **Performance Monitor Unit (PMU):**
  - Built into ARM Cortex processors
  - 6 programmable event counters
  - Counts: cycles, cache events, branches, bus accesses
  - Low overhead, configure via registers
  - Tools: ARM Carbon Analyzer GUI

**Software-Based Profiling:**
- **gprof, Oprofile, valgrind:**
  - Function-level profiling
  - Create call graphs
  - Identify memory leaks, cache thrashing
  - Introduces overhead (instrumentation)
  - Requires OS support

**Simple GPIO Profiling:**
```c
void critical_function() {
    GPIO_SET(PIN_1);    // Signal entry
    // ... function code ...
    GPIO_CLEAR(PIN_1);  // Signal exit
}
// Measure pulse width on oscilloscope
```

**Flame Graph Interpretation:**
- Width = time spent in function
- Height = call stack depth
- Wide boxes at bottom = bottlenecks
- Look for unexpectedly wide regions

---

## Important Diagrams & Graphics

### 1. **Design Space Triangle (Slide 2)**
```
         Energy
        /      \
       /        \
      /          \
   Cost -------- Performance
```
**Description:** Three-dimensional trade-off space for embedded system design. Different hardware configurations (1 CPU, Low-Energy CPU, 2 CPUs, 1 CPU + FPGA) occupy different positions. Performance optimization must consider all three axes. Draw triangle with three axes labeled and example configurations plotted.

### 2. **Performance Optimization Steps (Slide 6)**
```
1. Set clear performance goals
   ↓
2. Identify bottlenecks
   ↓
3. Profile the code
   ↓
4. Optimize algorithms and data structure
   ↓
5. Use the caches
   ↓
6. Use parallelization, concurrency
```
**Description:** Sequential process for systematic performance optimization. Each step builds on previous ones. Draw as vertical flowchart with arrows.

### 3. **Flame Graph Example (Slide 9-10)**
```
Time →
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
█████                   ████████        ████
██████  ████  ███  ████████████████  ███████
███████████████████████████████████████████
```
**Description:** Horizontal bars representing function execution over time. Width indicates duration, vertical stacking shows call depth. Wider boxes indicate more time spent. Color (red/orange) typically indicates "hot" (time-consuming) functions. Draw with varying widths and stacking to show realistic profile.

### 4. **PMU Block Diagram (Slide 12)**
```
                        ┌──────────────┐
Events from other units─┤ Count Enable │─┐
(branch, cache, etc)    │ Set/Clear    │ │
                        │ Event        │ │
System Control          │ Selection    │ │
Processor/APB Interface─┤ Registers    │ │
                        └──────────────┘ │
                                         │
                     ┌───────────────────┴────────┐
                     │  Cycle Counter             │
                     ├────────────────────────────┤
CLKIN ─────────────→ │  Performance Counter (×6)  │─→ nPMUIRQ
                     ├────────────────────────────┤   (Interrupt/
                     │  Interrupt/Overflow        │    Overflow)
                     │  registers                 │
                     └────────────────────────────┘
```
**Description:** ARM Cortex-A53 Performance Monitor Unit architecture. Shows event sources feeding into counter array (1 cycle counter + 6 performance counters), controlled via registers, generating interrupts on overflow. Draw boxes for main components with arrows showing data flow.

### 5. **Amdahl's Law Graph - Speedup vs Fraction (Slide 23)**
```
S_overall │   a=1000
    50    │   ╱───────
          │  ╱
    40    │ ╱
          │╱
    30    │╱
          │╱
    20    │╱  a=10
          │───
    10    │───────────
          │
     0    └─────────────────────
           0   0.2  0.4  0.6  0.8  1.0
                       f
```
**Description:** Overall speedup S_overall plotted against fraction f of runtime improved, for different acceleration factors a (10, 1000). Shows that only large f values enable significant speedup. Curve is nearly flat until f>0.8, then rises steeply. Draw with two curves showing different a values.

### 6. **Amdahl's Law - Parallel Speedup vs Cores (Slide 24)**
```
Parallel Speedup │
         70      │   100% parallel ╱
                 │               ╱
         60      │             ╱
                 │           ╱
         50      │         ╱
                 │       ╱   95% ───────
         40      │     ╱
                 │   ╱   90% ────
         30      │ ╱   80% ──
                 │╱  75% ─
         20      │  25% (flat)
                 │
         10      │
                 │
          0      └─────────────────────
                  0   10   20   30   40   50   60   70
                          Number of cores
```
**Description:** Shows how parallelization efficiency limits speedup with multiple cores. Even 95% parallel code barely scales beyond 20 cores due to 5% sequential overhead. Draw multiple curves for different parallelization percentages.

### 7. **Data Structure Comparison - Cache Lines (Slides 14-15)**
```
Array of Struct (AoS):
Cache line: │ch_a[0]│ch_b[0]│ch_c[0]│ch_a[1]│ch_b[1]│...
            └───────┴───────┴───────┴───────┴───────┘
            (accessing ch_a loads b and c too - wasted!)

Struct of Arrays (SoA):
Cache line: │ch_a[0]│ch_a[1]│ch_a[2]│ch_a[3]│ch_a[4]│...
            └───────┴───────┴───────┴───────┴───────┘
Cache line: │ch_b[0]│ch_b[1]│ch_b[2]│ch_b[3]│ch_b[4]│...
            (accessing ch_a loads only ch_a - efficient!)
```
**Description:** Demonstrates how data structure layout affects cache efficiency. AoS wastes cache bandwidth when accessing single channel; SoA maximizes cache line utilization for sequential access. Draw cache lines as boxes showing memory layout.

### 8. **Subroutine Call Overhead Diagram (Slide 34)**
```
CALLER:
┌─────────────────────────────────────────────┐
│ PUSH {R0-R3}     - Save caller-saved regs   │
│ MOV R0,Rx        - Copy params to R0-R3     │
│ ...              - (extra params on stack)  │
│ BL callee        - Branch & Link            │
└─────────────────────────────────────────────┘
                    ↓
CALLEE - Prolog:
┌─────────────────────────────────────────────┐
│ PUSH {R4-R7,LR}  - Save callee-saved regs   │
│ SUB SP,SP,#...   - Allocate stack space     │
│                                              │
│        CODE OF SUBROUTINE                    │
│                                              │
│ ADD SP,SP,#...   - De-allocate stack        │
│ POP {R4-R7,PC}   - Restore regs & return    │
└─────────────────────────────────────────────┘
                    ↓
CALLER - Return:
┌─────────────────────────────────────────────┐
│ MOV Rx,R0        - Get return value         │
│ POP {R0-R3}      - Restore caller registers │
└─────────────────────────────────────────────┘
```
**Description:** Complete sequence showing function call overhead on ARM. Parameter passing via registers (R0-R3), stack frame allocation, register preservation, and cleanup on return. Draw as three connected boxes with assembly code.

### 9. **SIMD Vector Processing Diagram (Slide 36)**
```
        Dn: │  a  │  b  │  c  │  d  │ (four 32-bit values)
            └──┬──┴──┬──┴──┬──┴──┬──┘
               │     │     │     │
              ×│    ×│    ×│    ×│    (parallel multiply)
               │     │     │     │
        Dm: │  e  │  f  │  g  │  h  │ (four 32-bit values)
            └──┬──┴──┬──┴──┬──┴──┬──┘
               │     │     │     │
               ↓     ↓     ↓     ↓
        Qd: │ a*e │ b*f │ c*g │ d*h │ (four 64-bit results)
            └─────┴─────┴─────┴─────┘

Single instruction performs 4 multiplications!
```
**Description:** SIMD operation multiplying four pairs of values simultaneously with one instruction. Show two source registers, operation symbols, and result register with element-wise operations.

### 10. **Loop Unrolling Comparison (Slide 30-31)**
```
ORIGINAL LOOP (smaller code, more branches):
mov    r1, r0          │ .LBB0_1:
mov    r0, #0          │   and    r3, r1, #1
cmp    r1, #0          │   cmp    r2, r1, lsr #1
bxeq   lr              │   add    r0, r0, r3
mov    r2, #0          │   lsr    r3, r1, #1
                       │   mov    r1, r3
UNROLLED LOOP          │   bne    .LBB0_1
(larger code, fewer    │   bx     lr
branches):             │
  [Assembly showing    │ (8 instructions, 1 branch per iteration)
   4x unrolled with    │
   parallel operations]│
(20+ instr, 1 branch  │
 per 4 iterations)     │
```
**Description:** Side-by-side comparison showing assembly code for original vs unrolled loop. Original is shorter but branches more often. Unrolled is longer but processes multiple elements per branch. Draw as two columns with instruction counts.

### 11. **Vectorized vs Non-vectorized Code (Slide 39)**
```
NON-VECTORIZED (sequential):
.LBB0_1:
  LDR    s0, [x0],#4    ← load 1 float
  LDR    s1, [x1],#4    ← load 1 float
  SUBS   x8, x8, #1     ← decrement counter
  FADD   s0, s0, s1     ← add 1 pair
  STR    s0, [x2],#4    ← store 1 result
  B.NE   .LBB0_1        ← branch if not done

VECTORIZED (parallel):
.L3.108:
  LDR    q0, [x13],#0x10 ← load 4 floats
  LDR    q1, [x12],#0x10 ← load 4 floats
  SUBS   x10, x10, #4    ← decrement by 4
  FADD   v0.4S,v0.4S,v1.4S ← add 4 pairs!
  STR    q0, [x11],#0x10  ← store 4 results
  B.NE   .L3.108         ← branch if not done

Same structure, but 4× throughput per iteration!
```
**Description:** Assembly comparison showing scalar floating-point addition vs NEON vector addition. Highlight FADD instruction differences: `FADD s0,s0,s1` (scalar) vs `FADD v0.4S,v0.4S,v1.4S` (vector). Draw side-by-side with arrows showing data flow.

---

## Formulas & Procedures

### 1. **Amdahl's Law - Overall Speedup**

**Formula:**
```
S_overall = T_orig / [(1-f)·T_orig + (f/a)·T_orig]
          = 1 / [(1-f) + f/a]
```

**Variables:**
- `S_overall` = overall speedup factor (e.g., 2.0 = twice as fast)
- `T_orig` = original total runtime
- `f` = fraction of runtime spent in improved section (0 to 1)
- `a` = acceleration factor for improved section (e.g., 10 = 10× faster)
- `(1-f)` = fraction of runtime unchanged

**Procedure:**
1. Profile code to determine `f` (fraction of time in target section)
2. Determine potential speedup `a` for that section
3. Calculate `S_overall = 1 / ((1-f) + f/a)`
4. Evaluate if improvement justifies effort

**Example:**
```
Given: f = 0.1 (10% of runtime), a = 10,000 (huge improvement)
S_overall = 1 / (0.9 + 0.1/10000)
          = 1 / (0.9 + 0.00001)
          = 1 / 0.90001
          ≈ 1.11

Result: Only 11% faster overall despite 10,000× local improvement!
```

### 2. **Improvement Percentage**

**Formula:**
```
improvement = 100% × (1 - 1/S_overall)
```

**Procedure:**
1. Calculate `S_overall` using Amdahl's Law
2. Compute `1/S_overall` (inverse)
3. Subtract from 1
4. Multiply by 100 for percentage

**Example:**
```
Given: S_overall = 1.11
improvement = 100% × (1 - 1/1.11)
            = 100% × (1 - 0.901)
            = 100% × 0.099
            = 9.9%
```

### 3. **Amdahl's Law Limits**

**Limit 1: Perfect Optimization (a → ∞)**
```
lim(a→∞) S_overall = lim(a→∞) 1/((1-f) + f/a)
                    = 1/(1-f)
```
**Meaning:** Even perfect speedup of a section limited by unchanged fraction.

**Example:**
```
f = 0.9 (90% improved to perfection)
S_overall_max = 1/(1-0.9) = 1/0.1 = 10

Cannot exceed 10× speedup no matter how fast we make the 90% section!
```

**Limit 2: Complete Improvement (f → 1)**
```
lim(f→1) S_overall = lim(f→1) 1/((1-f) + f/a)
                    = 1/(0 + 1/a)
                    = a
```
**Meaning:** If entire program improved, overall speedup equals section speedup.

### 4. **Horner's Rule for Polynomial Evaluation**

**General Form:**
```
For polynomial: P(x) = aₙxⁿ + aₙ₋₁xⁿ⁻¹ + ... + a₁x + a₀

Horner's form: P(x) = ((...((aₙ·x + aₙ₋₁)·x + aₙ₋₂)·x...)·x + a₀)
```

**Procedure:**
1. Start with highest degree coefficient: `result = aₙ`
2. For each remaining coefficient (n-1 down to 0):
   - Multiply result by x: `result = result × x`
   - Add next coefficient: `result = result + aᵢ`
3. Final result is P(x)

**Example - 4th Degree Polynomial:**
```
y(x) = a₄x⁴ + a₃x³ + a₂x² + a₁x + a₀

d₀ = a₄
d₁ = x·d₀ + a₃ = a₄x + a₃
d₂ = x·d₁ + a₂ = a₄x² + a₃x + a₂
d₃ = x·d₂ + a₁ = a₄x³ + a₃x² + a₂x + a₁
d₄ = x·d₃ + a₀ = a₄x⁴ + a₃x³ + a₂x² + a₁x + a₀ = y(x)

Naive: 10 multiplications + 4 additions
Horner: 4 multiplications + 4 additions (60% fewer operations!)
```

**Code Implementation:**
```c
double horner_poly4(double x, double a4, double a3,
                    double a2, double a1, double a0) {
    double d = a4;
    d = x*d + a3;
    d = x*d + a2;
    d = x*d + a1;
    d = x*d + a0;
    return d;
}
```

### 5. **PMU Configuration Procedure**

**Steps to Use Performance Monitor Unit:**

1. **Select Events to Count:**
```c
// Choose from: CPU cycles, cache hits/misses,
// branch mispredictions, bus accesses, etc.
PMU_EventSelect(counter_0, EVENT_CACHE_MISS);
PMU_EventSelect(counter_1, EVENT_BRANCH_MISPREDICTED);
```

2. **Enable Counters:**
```c
PMU_EnableCounter(counter_0);
PMU_EnableCounter(counter_1);
```

3. **Reset Counters:**
```c
PMU_ResetCounters();  // Start from zero
```

4. **Execute Code Section:**
```c
// Your code to profile here
my_function();
```

5. **Read Counter Values:**
```c
uint32_t cache_misses = PMU_ReadCounter(counter_0);
uint32_t branch_mispred = PMU_ReadCounter(counter_1);
```

6. **Analyze Results:**
```c
printf("Cache misses: %u\n", cache_misses);
printf("Branch mispredictions: %u\n", branch_mispred);
```

### 6. **Loop Unrolling Factor Calculation**

**Determining Optimal Unroll Factor:**

**Considerations:**
- Larger unrolling → fewer branches but more code size
- Typical factors: 2, 4, 8, 16
- Must handle remainder iterations if total not multiple of factor

**Example - Unroll by 4:**
```c
// Original
for(int i=0; i<n; i++) {
    process(data[i]);
}

// Unrolled by 4
int i;
for(i=0; i<(n & ~3); i+=4) {  // Process multiples of 4
    process(data[i]);
    process(data[i+1]);
    process(data[i+2]);
    process(data[i+3]);
}
for(; i<n; i++) {  // Handle remainder (0-3 iterations)
    process(data[i]);
}
```

**Bitwise Trick for Alignment:**
```
n & ~3  means  n & 0xFFFFFFFC
Clears last 2 bits, giving largest multiple of 4 ≤ n

Examples:
100 & ~3 = 100 (already multiple of 4)
101 & ~3 = 100 (rounds down to 100)
103 & ~3 = 100 (rounds down to 100)
104 & ~3 = 104 (already multiple of 4)
```

### 7. **Cache Line Efficiency Calculation**

**Formula:**
```
Cache Efficiency = (Useful Data per Cache Line) / (Cache Line Size)
```

**Example:**
```
Cache line size: 64 bytes
Accessing uint16_t ch_a from struct with 3× uint16_t:

Array of Struct:
- Struct size: 3 × 2 = 6 bytes
- Cache line holds: 64 / 6 ≈ 10 complete structs
- Accessing ch_a: use 2 bytes, load 6 bytes
- Efficiency: 2/6 = 33%

Struct of Arrays:
- Accessing ch_a[]: contiguous uint16_t values
- Cache line holds: 64 / 2 = 32 values
- Accessing ch_a: use all 64 bytes
- Efficiency: 64/64 = 100%
```

### 8. **SIMD Throughput Calculation**

**Formula:**
```
SIMD Speedup = (Vector Width / Scalar Width) × Efficiency

Where:
- Vector Width = 128 bits (NEON)
- Scalar Width = data type size
- Efficiency = accounting for overhead and remainder handling
```

**Examples:**
```
Processing 1000× float (32-bit):
Scalar: 1000 iterations
SIMD: 128/32 = 4 elements per instruction
      ≈ 1000/4 = 250 iterations (+ overhead)
Theoretical speedup: 4× (practical: 3-3.5× due to overhead)

Processing 1000× uint8_t (8-bit):
SIMD: 128/8 = 16 elements per instruction
      ≈ 1000/16 = 62.5 iterations
Theoretical speedup: 16× (practical: 10-14×)
```

### 9. **Compiler Optimization Flag Selection**

**Decision Tree:**

```
Need debugging?
├─ Yes → Use -O0 (no optimization)
└─ No → Continue

Need maximum performance?
├─ Yes → Use -O3 (aggressive, larger code)
└─ No → Continue

Limited code memory?
├─ Yes → Use -Os or -Oz (optimized for size)
└─ No → Use -O2 (balanced optimization)

Special cases:
- Critical section timing: Use -O0 with asm volatile
- Profiling: Use -O2 with -g (debug symbols)
- Release build: Use -O3 with -DNDEBUG
```

---

## Comparisons & Contrasts

### 1. **Performance Metrics**

| Metric | What It Measures | When Important | Units |
|--------|-----------------|----------------|-------|
| **Execution Time** | Total runtime of program/function | Meeting deadlines, battery life | seconds, ms |
| **Response Time/Latency** | Time from request to response | User experience, hard real-time | ms, μs |
| **Memory Footprint** | Code + data size | Resource-constrained MCUs | bytes, KB |
| **CPU Utilization** | % of CPU time used | RTOS scheduling, headroom | percentage |
| **Throughput** | Operations per unit time | Data processing pipelines | ops/sec |

**When to Optimize Each:**
- **Execution Time**: When tasks miss deadlines or battery drains too fast
- **Latency**: When UI feels sluggish or control loops unstable
- **Memory**: When approaching flash/RAM limits
- **CPU Utilization**: When adding features or improving responsiveness

### 2. **Profiling Methods**

| Method | Intrusiveness | Accuracy | Cost | Best For |
|--------|---------------|----------|------|----------|
| **Hardware Trace (J-Trace)** | None | Exact | High ($$$$) | Production optimization |
| **PMU Counters** | Minimal | Good | Free (built-in) | Cache/branch analysis |
| **Software Profilers (gprof)** | Moderate | Good | Free | Function-level analysis |
| **GPIO Toggle** | Minimal | Good | Very Low | Simple timing |
| **printf/Logging** | High | Poor | Free | Quick checks |

**Trade-offs:**
- **Hardware trace**: Non-intrusive but requires expensive tools
- **PMU**: Low overhead but requires manual configuration
- **Software profilers**: Easy to use but add instrumentation overhead
- **GPIO**: Simple setup but manual measurement

### 3. **Data Structure Layouts**

| Aspect | Array of Structures (AoS) | Structure of Arrays (SoA) |
|--------|---------------------------|---------------------------|
| **Definition** | `struct {a,b,c} data[N];` | `struct {a[N], b[N], c[N]};` |
| **Memory Layout** | a₀b₀c₀a₁b₁c₁... | a₀a₁a₂...b₀b₁b₂...c₀c₁c₂... |
| **Cache Efficiency** | Poor (loads unused fields) | Excellent (loads only needed field) |
| **Access Pattern** | `data[i].a` | `data.a[i]` |
| **Vectorization** | Difficult | Easy |
| **Best For** | Accessing all fields together | Accessing one field at a time |

**Example:**
```c
// AoS - Good for: Processing entire records
for(int i=0; i<N; i++) {
    process_all(data[i].a, data[i].b, data[i].c);
}

// SoA - Good for: Processing single channel
for(int i=0; i<N; i++) {
    process_channel(data.a[i]);  // Only loads 'a' array
}
```

### 4. **Loop Optimization Techniques**

| Technique | Code Size | Execution Speed | Complexity |
|-----------|-----------|-----------------|------------|
| **None (-O0)** | Smallest | Slowest | Simple |
| **Simple Optimization** | Small | Moderate | Simple |
| **Unrolling (2×)** | Medium | Fast | Moderate |
| **Unrolling (4×)** | Large | Faster | Higher |
| **Vectorization** | Medium-Large | Fastest | High |
| **Unroll + Vector** | Largest | Fastest | Highest |

**Guidelines:**
- Small loops (<10 iterations): Consider full unrolling
- Medium loops (10-100): 2× or 4× unrolling
- Large loops (>100): Let compiler vectorize
- Embedded (flash limited): Avoid excessive unrolling

### 5. **Function Call Strategies**

| Strategy | Code Size | Call Overhead | Best For |
|----------|-----------|---------------|----------|
| **Normal Function** | Smallest | Full overhead | Large, reusable functions |
| **Inline Function** | Larger | None | Small, frequently-called |
| **Static Inline** | Medium | None | Small, file-local |
| **Macro** | Varies | None | Trivial operations, type-generic |
| **Function Pointer** | Small | Highest | Callbacks, polymorphism |

**Parameter Passing Methods:**

| Method | Speed | Limitation | Example |
|--------|-------|------------|---------|
| **Register (R0-R3/X0-X7)** | Fastest | 4 or 8 params | `void f(int a, int b, int c);` |
| **Stack** | Slower | Many params | `void f(int p1...p10);` |
| **Pointer** | Fast | Indirection | `void f(struct data *p);` |
| **Global** | Fast | Not reentrant | External variables |

### 6. **Compiler Optimization Levels**

| Flag | Speed | Code Size | Compilation Time | Use Case |
|------|-------|-----------|------------------|----------|
| **-O0** | Slowest | Small | Fast | Debugging |
| **-O1** | Moderate | Small | Fast | Basic optimization |
| **-O2** | Fast | Medium | Medium | Balanced (default release) |
| **-O3** | Fastest | Large | Slow | Maximum performance |
| **-Os** | Moderate | Small | Medium | Size-constrained systems |
| **-Oz** | Slow | Smallest | Medium | Extreme size constraints |

**Optimizations Enabled:**

| Optimization | -O0 | -O1 | -O2 | -O3 | -Os |
|--------------|-----|-----|-----|-----|-----|
| Dead code elimination | ❌ | ✅ | ✅ | ✅ | ✅ |
| Constant folding | ❌ | ✅ | ✅ | ✅ | ✅ |
| Inlining | ❌ | Limited | Yes | Aggressive | Limited |
| Loop unrolling | ❌ | ❌ | Limited | Yes | ❌ |
| Vectorization | ❌ | ❌ | Yes | Yes | ❌ |
| Aggressive inlining | ❌ | ❌ | ❌ | Yes | ❌ |

### 7. **SIMD vs Scalar Processing**

| Aspect | Scalar | SIMD (NEON) |
|--------|--------|-------------|
| **Registers** | 32-bit (R0-R15) | 128-bit (V0-V31) |
| **Elements per Op** | 1 | 2-16 (depends on type) |
| **Throughput** | 1× baseline | 2-16× baseline |
| **Code Size** | Smaller | Larger (setup overhead) |
| **Power** | Lower per instruction | Higher per instruction |
| **Energy Efficiency** | 1× | Better (more work per instruction) |
| **Suitable For** | Control logic | Data-parallel operations |

**When to Use Each:**
- **Scalar**: Control flow, complex logic, small data sets
- **SIMD**: Image processing, DSP, vector math, large arrays

### 8. **Cache Levels**

| Cache | Cortex-A53 Size | Access Latency | Scope | Purpose |
|-------|-----------------|----------------|-------|---------|
| **L1 Data** | 32 KB | 3-4 cycles | Per core | Hot data |
| **L1 Instruction** | 32 KB | 2-3 cycles | Per core | Hot code |
| **L2 Unified** | 256 KB - 2 MB | ~10-20 cycles | Shared | Working set |
| **Main RAM** | MB-GB | ~100-200 cycles | Global | All data |

**Optimization Strategy:**
- **L1**: Keep innermost loop code + data here (< 32 KB)
- **L2**: Keep working set here (< 256 KB ideal)
- **Avoid**: Random access patterns causing cache thrashing

### 9. **Parallelization Approaches**

| Approach | Granularity | Overhead | Scalability | Best For |
|----------|-------------|----------|-------------|----------|
| **SIMD** | Instruction | Very low | 2-16× | Data-parallel loops |
| **Multi-threading** | Task | Medium | Depends on f | Independent tasks |
| **Multi-core** | Process | High | Limited by Amdahl | Parallel workloads |
| **FPGA Coprocessor** | Algorithm | Very high (setup) | High | Specific algorithms |
| **Interrupt-driven** | Event | Low | N/A | Asynchronous I/O |

**Selection Guide:**
1. **Data parallel loop?** → Use SIMD
2. **Independent tasks?** → Use threads/cores
3. **I/O bound?** → Use interrupts/async
4. **Compute bound?** → Consider FPGA
5. **Real-time critical?** → Profile with Amdahl's Law

### 10. **Optimization vs Debug Builds**

| Aspect | Debug Build | Release Build |
|--------|-------------|---------------|
| **Flags** | `-O0 -g` | `-O2` or `-O3 -DNDEBUG` |
| **Speed** | Slow | Fast |
| **Code Size** | Small | Medium-Large |
| **Debuggability** | Excellent | Poor |
| **Assertions** | Enabled | Disabled |
| **Inlining** | None | Aggressive |
| **Variable Location** | Predictable | Optimized away |
| **Use Case** | Development | Production |

---

## Examples & Applications

### Example 1: **Optimizing Factorial Calculation**

**Problem:** Calculate factorial array: `fact[i] = fact[i-1] * i`

**Original Code:**
```c
int main(void) {
    int fact[5];
    fact[0] = 1;

    for (int i = 1; i < 5; ++i) {
        fact[i] = fact[i - 1] * i;
    }
}
```

**Issue:** Dependencies between iterations prevent vectorization.

**Analysis:**
- Each iteration depends on previous result: `fact[i-1]`
- Cannot parallelize with SIMD
- Cannot unroll effectively due to dependencies
- Already optimal algorithm (O(n))

**Possible Optimization:**
```c
// If factorial values known at compile time, precompute
static const int fact[5] = {1, 1, 2, 6, 24};
// No runtime computation needed!
```

**Lesson:** Not all loops are parallelizable. Look for algorithmic improvements or precomputation opportunities.

### Example 2: **Optimizing Array Computation**

**Problem:** Compute array with complex expression in loop

**Original Code:**
```c
int main(void) {
    int arr[1000];
    int a = 1, b = 5, c = 25, d = 7;

    for (int i = 0; i < 1000; ++i) {
        arr[i] = (((c % d) * a / b) % d) * i;
    }
    return 0;
}
```

**Issues:**
1. Expression `(((c % d) * a / b) % d)` constant for all iterations
2. Computed 1000 times unnecessarily
3. Division and modulo expensive operations

**Optimized Code:**
```c
int main(void) {
    int arr[1000];
    int a = 1, b = 5, c = 25, d = 7;

    // Pre-calculate constant expression
    int temp = (((c % d) * a / b) % d);

    // Reverse loop for better optimization
    for (int i = 1000; i != 0; --i) {
        arr[i] = temp * i;
    }
    return 0;
}
```

**Improvements:**
- **Constant folding**: Compiler can compute `temp` at compile time
- **Loop simplification**: `i != 0` simpler than `i < 1000`
- **Countdown loop**: More efficient on ARM (compare to zero)
- **Vectorizable**: No dependencies, simple multiplication

**Performance Gain:** ~5-10× faster due to eliminating repeated expensive operations.

### Example 3: **Amdahl's Law - Loop Optimization**

**Scenario:**
- `for` loop consumes 98% of execution time
- Optimized loop runs 3000× faster
- Calculate overall speedup

**Solution:**
```
Given:
f = 0.98 (98% in loop)
a = 3000 (loop 3000× faster)

S_overall = 1 / ((1-f) + f/a)
          = 1 / ((1-0.98) + 0.98/3000)
          = 1 / (0.02 + 0.000327)
          = 1 / 0.020327
          ≈ 49.2

improvement = 100% × (1 - 1/49.2)
            = 100% × (1 - 0.0203)
            = 100% × 0.9797
            ≈ 98%
```

**Result:** Despite 3000× loop speedup, overall program only ~49× faster (98% improvement).

**Lesson:** High f value (98%) enables large speedup. Confirms importance of profiling to find high-f sections.

### Example 4: **Vector Addition with NEON**

**Problem:** Add two float arrays of 1000 elements

**Scalar Version:**
```c
void vec_add_scalar(float *a, float *b, float *c, int len) {
    for (int i = 0; i < len; i++) {
        c[i] = a[i] + b[i];
    }
}
// Assembly: 1 FADD per iteration
// 1000 iterations total
```

**Vectorized Version (Auto-vectorized by compiler with -O2):**
```c
void vec_add_vector(float *a, float *b, float *c, int len) {
    // Hint to compiler that len is multiple of 4
    for (int i = 0; i < (len & ~3); i++) {
        c[i] = a[i] + b[i];
    }
}
// Assembly: FADD v0.4S, v0.4S, v1.4S
// Processes 4 floats per instruction
// 250 iterations total (+ setup overhead)
```

**Performance:**
- Scalar: 1000 additions + 1000 branches
- Vector: 250 quad-additions + 250 branches + setup
- Theoretical speedup: 4×
- Practical speedup: ~3.5× (accounting for setup overhead)

**Memory Access Pattern:**
```
Scalar: Load 1 float, load 1 float, add, store 1 float
Vector: Load 4 floats, load 4 floats, add 4, store 4 floats
        Better memory bandwidth utilization!
```

### Example 5: **Cache-Friendly Data Access**

**Problem:** Process only channel A from multichannel sensor data

**Bad Approach (Array of Structures):**
```c
struct sensor_t {
    uint16_t ch_a, ch_b, ch_c;
} data[10000];

// Process only channel A
uint32_t sum = 0;
for (int i = 0; i < 10000; i++) {
    sum += data[i].ch_a;  // Loads ch_b and ch_c too!
}
```

**Cache Behavior:**
- Each access loads 64-byte cache line
- Cache line contains ~10 structs = 30 channels
- Only using 10 ch_a values = 33% efficiency
- 20 channels loaded unnecessarily

**Good Approach (Structure of Arrays):**
```c
struct sensor_array_t {
    uint16_t ch_a[10000];
    uint16_t ch_b[10000];
    uint16_t ch_c[10000];
} data;

// Process only channel A
uint32_t sum = 0;
for (int i = 0; i < 10000; i++) {
    sum += data.ch_a[i];  // Only loads ch_a!
}
```

**Cache Behavior:**
- Each access loads 64-byte cache line
- Cache line contains 32 ch_a values
- Using all 32 values = 100% efficiency
- No wasted bandwidth

**Performance:** ~3× faster due to better cache utilization + enables vectorization.

### Example 6: **Horner's Rule Optimization**

**Problem:** Evaluate polynomial y = 2x⁴ + 3x³ + 5x² + 7x + 11 for x = 1.5

**Naive Approach:**
```c
double naive_poly(double x) {
    return 2*x*x*x*x + 3*x*x*x + 5*x*x + 7*x + 11;
}
// Operations: 10 multiplications, 4 additions
// x² computed 4 times
// x³ computed 2 times
// x⁴ computed 1 time
```

**Horner's Rule:**
```c
double horner_poly(double x) {
    double d = 2.0;
    d = x * d + 3.0;  // 2x + 3
    d = x * d + 5.0;  // 2x² + 3x + 5
    d = x * d + 7.0;  // 2x³ + 3x² + 5x + 7
    d = x * d + 11.0; // 2x⁴ + 3x³ + 5x² + 7x + 11
    return d;
}
// Operations: 4 multiplications, 4 additions
// 60% fewer multiplications!
```

**Calculation for x = 1.5:**
```
d₀ = 2
d₁ = 1.5 × 2 + 3 = 6
d₂ = 1.5 × 6 + 5 = 14
d₃ = 1.5 × 14 + 7 = 28
d₄ = 1.5 × 28 + 11 = 53

Result: 53.0
```

**Performance Gain:**
- Naive: ~30 cycles (10 FP multiplies)
- Horner: ~12 cycles (4 FP multiplies)
- Speedup: ~2.5×

### Example 7: **Loop Unrolling Trade-off**

**Problem:** Count set bits in 32-bit integer

**Original:**
```c
int count_bits(unsigned int n) {
    int bits = 0;
    while (n != 0) {
        if (n & 1) bits++;
        n >>= 1;
    }
    return bits;
}
// Assembly: ~8 instructions per iteration
// 32 iterations worst case
// 32 branches
```

**Unrolled (4×):**
```c
int count_bits_unrolled(unsigned int n) {
    int bits = 0;
    while (n != 0) {
        if (n & 1) bits++;
        if (n & 2) bits++;
        if (n & 4) bits++;
        if (n & 8) bits++;
        n >>= 4;
    }
    return bits;
}
// Assembly: ~20 instructions per iteration
// 8 iterations worst case
// 8 branches (4× fewer!)
```

**Analysis:**
- Code size: 2.5× larger
- Branch count: 4× fewer
- Execution time: ~2.5× faster (branch prediction helps)
- Best for: Frequently called, performance-critical

**Alternative - Population Count Intrinsic:**
```c
int count_bits_intrinsic(unsigned int n) {
    return __builtin_popcount(n);
}
// Single instruction on many CPUs!
// Fastest option if available
```

### Example 8: **PMU-Based Profiling**

**Scenario:** Identify cache miss rate for sorting algorithm

**Code:**
```c
void profile_sort() {
    // Configure PMU
    PMU_Reset();
    PMU_EventSelect(0, EVENT_CPU_CYCLES);
    PMU_EventSelect(1, EVENT_L1_DCACHE_REFILL);  // Cache misses
    PMU_Enable();

    // Run algorithm
    bubble_sort(data, 10000);

    // Read results
    uint32_t cycles = PMU_ReadCounter(0);
    uint32_t cache_misses = PMU_ReadCounter(1);

    printf("Cycles: %u\n", cycles);
    printf("Cache misses: %u\n", cache_misses);
    printf("Miss rate: %.2f%%\n",
           (cache_misses * 100.0) / (cycles / 10));
}
```

**Interpretation:**
- Low miss rate (< 5%): Good cache utilization
- Medium miss rate (5-15%): Consider data structure changes
- High miss rate (> 15%): Serious optimization needed

**Optimization Based on Results:**
- High miss rate → Change data layout (AoS to SoA)
- Many branch mispredictions → Simplify conditionals
- Many bus accesses → Reduce memory traffic

---

## Connections to Other Topics

### To Lecture 01: Embedded Systems
- **Performance constraints** fundamental to embedded systems
- **Resource limitations** (memory, CPU) drive need for optimization
- **Real-time requirements** demand predictable, fast execution
- **Energy considerations** link performance to battery life (Lecture 08)

### To Lecture 02: Software Paradigms
- **Event-driven programming** as parallelization strategy (interrupts)
- **Procedural programming** optimization techniques apply to C code
- **Object-oriented overhead** (vtables, virtual calls) vs performance
- **Design patterns** impact on performance (abstraction costs)

### To Lecture 04: Modelling
- **Performance modeling** before implementation
- **Profiling data** validates models
- **Amdahl's Law** as analytical model for optimization ROI
- **Architecture decisions** based on performance requirements

### To Lecture 05: Development Processes
- **Optimization timing** in development cycle (avoid premature optimization)
- **Profiling** as part of testing/validation phase
- **Performance regression testing** in CI/CD
- **Release builds** vs debug builds in deployment

### To Lecture 06: Non-Functional Requirements
- **Performance is an NFR** (response time, throughput)
- **Trade-offs** between performance, energy, cost, safety
- **Specification** of performance goals (step 1 of optimization)
- **Verification** through profiling and measurement

### To Lecture 08: Energy
- **Performance-energy trade-off**: Faster often means more power
- **DVFS** balances performance and energy
- **Sleep modes** vs performance requirements
- **Amdahl's Law applied to energy**: Optimizing high-energy sections
- **Energy per operation** metric combines both domains

### To Future Lecture 10: FPGAs
- **Hardware acceleration** as ultimate optimization
- **Parallelism** naturally expressed in hardware
- **Performance beyond CPUs** for specific algorithms
- **When software optimization insufficient** → Consider FPGA
- **Energy efficiency** of dedicated hardware vs general CPU

### To Future Lecture 11-12: Design Space Exploration (DSE)
- **Performance** as one dimension of design space
- **Pareto frontiers** trading performance vs energy vs cost
- **Amdahl's Law** guides where to invest hardware resources
- **Profiling results** inform hardware/software partitioning decisions

### To Future Lecture 13: RTOS Scheduling
- **CPU utilization** determines schedulability
- **Task priorities** based on performance requirements (deadlines)
- **Context switch overhead** impacts multitasking performance
- **Interrupt latency** as performance metric
- **Cache effects** of task switching on performance

### To Future Lecture 14: Multi-Processor Systems
- **Parallelization** at processor level
- **Amdahl's Law limitations** for multicore scaling
- **Inter-processor communication** overhead
- **Load balancing** for performance optimization
- **Shared cache** (L2) vs private cache (L1) effects

### To Lab P2: Energy Analysis
- **Performance monitoring** techniques similar to energy measurement
- **Oscilloscope profiling** concept applied to performance (GPIO method)
- **Energy-performance trade-off**: Fast AES uses more power
- **FSM performance** analysis

### To Lab P3: Multi-Processor AES (Future)
- **Parallel AES** implementation using multiple cores
- **Amdahl's Law** validation with actual multi-core measurements
- **Synchronization overhead** impact on performance
- **Performance scaling** with 2 vs 1 core

### To Lab P4: AES in Hardware (Future)
- **FPGA performance** vs CPU performance
- **Hardware acceleration** as optimization endpoint
- **Energy efficiency** of hardware implementation
- **Fixed-function accelerators** (like NEON) concept extended

### To Lab P5: Design Space Exploration (Future)
- **Performance profiling** informs DSE decisions
- **Bottleneck identification** guides hardware/software partitioning
- **Amdahl's Law** applied to decide what to accelerate in hardware
- **Optimization trade-offs** across entire design space

---

## Summary

**Performance optimization is a systematic engineering process**, not guesswork:

1. **Set quantifiable goals** (execution time, latency, throughput, memory limits)
2. **Profile to identify bottlenecks** - measure, don't speculate
3. **Apply Amdahl's Law** - optimize high-impact sections (large f)
4. **Choose appropriate optimizations**:
   - Algorithm: Most impactful (Horner's rule example)
   - Data structure: Enable cache efficiency (SoA vs AoS)
   - Compiler flags: Free performance (-O2, -O3)
   - SIMD: 2-16× speedup for data-parallel code
   - Loop techniques: Unrolling for small loops
   - Parallelization: Multiple cores when beneficial

**Key Principles:**
- **Profile before optimizing** - Intuition often wrong about bottlenecks
- **Focus on high-f sections** - Small improvements to minor code yield little
- **Balance trade-offs** - Performance vs code size, energy, development time
- **Let compiler help** - Modern compilers excellent at low-level optimization
- **Know your hardware** - Cache sizes, SIMD width, PMU capabilities
- **Measure results** - Verify optimizations actually help

**Common Pitfalls to Avoid:**
- Premature optimization before profiling
- Optimizing low-impact code
- Ignoring code size constraints
- Breaking readability/maintainability
- Assuming vectorization is always faster
- Over-unrolling loops
- Forgetting to test with optimization flags

**Modern Optimization Workflow:**
```
Requirements → Profile → Analyze (Amdahl) → Optimize → Verify → Repeat
                ↑                                           ↓
                └───────────────────────────────────────────┘
```

---

## Possible Exam Questions

### Basic Knowledge (Definitions & Concepts)

1. **Define "execution time" and explain why minimizing it is important in embedded systems.**

2. **What is response time (latency) and how does it differ from execution time?**

3. **Explain the term "bottleneck" in the context of software performance.**

4. **What is profiling and why is it important before optimizing code?**

5. **Define the Performance Monitor Unit (PMU) and list three types of events it can count.**

6. **What is a flame graph and what does the width of boxes represent?**

7. **What is loop unrolling and what is the primary trade-off?**

8. **Define SIMD (Single Instruction Multiple Data) and explain its purpose.**

9. **What is ARM NEON and what is the width of its vector registers?**

10. **Explain the difference between spatial locality and temporal locality.**

11. **What is auto-vectorization and which compiler optimization levels enable it?**

12. **Define "inline function" and explain when it should be used.**

13. **What is the working set and why should it fit in cache?**

14. **Explain what subroutine call overhead includes.**

15. **What is memory thrashing and what causes it?**

16. **Define CPU utilization and explain its relevance to RTOS scheduling.**

17. **What is Horner's rule and what problem does it solve?**

18. **Explain boolean short-circuit evaluation and how it can be optimized.**

19. **What is the difference between -O2 and -O3 compiler optimization levels?**

20. **What are intrinsics in the context of SIMD programming?**

### Conceptual Understanding

21. **Explain why "profiling before optimizing" is critical. What can go wrong if you skip profiling?**

22. **Describe the six-step process for performance optimization and explain why the order matters.**

23. **Why does Array of Structures (AoS) typically have worse cache performance than Structure of Arrays (SoA) when accessing a single field?**

24. **Explain how context switches in an RTOS can negatively impact cache performance.**

25. **Why might loop unrolling not always improve performance despite reducing branches?**

26. **Describe the overhead involved in a function call on ARM architecture and explain why inline functions avoid this.**

27. **Explain why vectorization is more difficult when loop iterations have dependencies.**

28. **How does the compiler use hints like `(len & ~3)` to optimize vectorized code?**

29. **Explain the trade-off between code size and execution speed in optimization.**

30. **Why is it important to combine two sequential loops over the same data range into one loop?**

31. **Describe how the PMU enables low-overhead profiling compared to software profilers.**

32. **Explain why polling is generally less efficient than interrupt-driven programming.**

33. **How does parameter passing via registers differ from stack-based parameter passing in terms of performance?**

34. **Explain why "respecting spatial locality" improves cache performance with examples.**

35. **Describe how branch misprediction impacts pipeline performance.**

### Amdahl's Law

36. **State Amdahl's Law formula and define each variable.**

37. **Calculate: A section consuming 95% of runtime is improved 1000×. What is the overall speedup and percentage improvement?**

38. **Explain what Amdahl's Law demonstrates about the importance of choosing the right code sections to optimize.**

39. **If you improve 20% of a program's runtime by a factor of 50, what is the maximum overall speedup?**

40. **Amdahl's Law shows that as f approaches 1, what is the maximum possible speedup as a approaches infinity?**

41. **Why does Amdahl's Law suggest that the acceleration factor (a) is less important than the fraction (f)?**

42. **Explain how Amdahl's Law applies to multicore processor scaling.**

43. **Given a program where 90% of runtime is improved 100×, calculate the overall speedup and improvement percentage.**

44. **What does Amdahl's Law teach us about inter-processor communication overhead in parallel systems?**

45. **If optimizing a function that takes 5% of total runtime by 10,000×, why might the overall improvement be disappointing?**

### Practical Application

46. **You have a function processing 1000 floats in an array. Describe how NEON vectorization can accelerate it and estimate the speedup.**

47. **Given a struct with three uint16_t fields (a, b, c) in a 10000-element array, and you only need to process field 'a', should you use AoS or SoA? Why?**

48. **Describe how you would use the PMU to identify cache miss problems in your code.**

49. **You have a loop calculating: `arr[i] = ((c % d) * a / b) * i` for 1000 iterations where a, b, c, d are constants. How would you optimize it?**

50. **Explain how Horner's rule would optimize the polynomial: y = 3x⁴ + 2x³ + 5x² + 7x + 1. Show the calculation steps.**

51. **A loop iterates 32 times doing simple operations. Should you unroll it? By what factor? Justify your answer.**

52. **You're optimizing code for a flash-constrained microcontroller. Which compiler optimization flag should you use and why?**

53. **Describe a scenario where inline functions would be counterproductive.**

54. **How would you profile a function to determine if it's a bottleneck using GPIO pins and an oscilloscope?**

55. **Given a loop with condition `while(getRandom() > i && i < 100)`, which condition should come first for optimization?**

### Analysis & Comparison

56. **Compare hardware-based profiling (J-Trace) vs software-based profiling (gprof) in terms of overhead, accuracy, and cost.**

57. **Compare the performance characteristics of -O2 vs -O3 compiler optimization. When would you choose each?**

58. **Contrast scalar and vector (SIMD) processing in terms of throughput, code size, and suitable applications.**

59. **Compare Array of Structures (AoS) vs Structure of Arrays (SoA) for cache efficiency and vectorization potential.**

60. **Explain the trade-offs between loop unrolling factors of 2×, 4×, 8×, and 16×.**

61. **Compare L1 cache vs L2 cache in terms of size, latency, and impact on performance.**

62. **Contrast the overhead of register-based parameter passing vs stack-based parameter passing.**

63. **Compare polling vs interrupt-driven I/O in terms of CPU utilization and responsiveness.**

### Design Decisions

64. **You're designing a signal processing application. How would you decide whether to use SIMD optimization?**

65. **Your profiling shows the bottleneck is memory access, not computation. What optimization strategies would you consider?**

66. **When would you choose to manually unroll a loop instead of relying on compiler optimization?**

67. **How do you decide whether to use an inline function, a normal function, or a macro?**

68. **Your application barely meets performance requirements but is near flash memory limit. What approach would you take?**

69. **Profiling shows high cache miss rate. What are three strategies you could use to improve it?**

70. **You need to optimize a function called millions of times per second. Describe your optimization process.**

### Critical Thinking & Problem Solving

71. **Why might a 1000× improvement to a function result in barely noticeable overall speedup? Explain using Amdahl's Law.**

72. **Explain why the statement "more optimization levels always means faster code" is not always true.**

73. **A colleague insists on fully unrolling all loops for maximum performance. What would you tell them?**

74. **How does the design space triangle (Energy-Cost-Performance) influence optimization decisions?**

75. **Why might auto-vectorization sometimes produce slower code than scalar code?**

76. **Explain how optimizing for performance can conflict with optimizing for energy. Give an example.**

77. **Your optimized code runs slower than the original. What could have gone wrong?**

78. **Discuss why "profile first, optimize second" is better than "optimize everything".**

### Integration & Connections

79. **How does performance optimization relate to the energy concepts from Lecture 08? Discuss trade-offs.**

80. **Explain how the Performance Monitor Unit (PMU) is similar to the energy measurement techniques from Lab P2.**

81. **How will Amdahl's Law apply when you use multiple processor cores in future labs?**

82. **Discuss how performance requirements (NFRs from Lecture 06) should guide the optimization process.**

83. **How does the choice of software paradigm (Lecture 02) affect optimization opportunities?**

84. **Explain how profiling results would inform hardware/software partitioning decisions in DSE (future lectures).**

85. **How do RTOS context switches (future Lecture 13) impact the cache optimization strategies discussed here?**

### Advanced Analysis

86. **You have a loop with 75% parallelizable code running on 4 cores. Using Amdahl's Law, predict the maximum speedup.**

87. **Explain why large datasets are necessary to justify many-core parallelization using Amdahl's Law principles.**

88. **Given assembly code showing FADD (scalar) vs FADD v0.4S (vector), explain the performance difference.**

89. **Analyze why preparation overhead and remainder handling limit SIMD speedup below theoretical maximum.**

90. **A flame graph shows a narrow but very tall call stack. What does this indicate about the performance characteristics?**

91. **Explain how spatial locality enables both cache optimization AND vectorization opportunities.**

92. **Discuss why simple loop termination conditions are important for both unrolling and vectorization.**

93. **Analyze the relationship between working set size and cache hierarchy (L1, L2, RAM) for performance.**

94. **Why might a function with 8 parameters perform worse than one with 4 parameters on ARM Cortex-A53?**

95. **Explain how boolean condition ordering exploits short-circuit evaluation for performance.**

96. **Analyze why countdown loops (`i-- > 0`) might be more efficient than count-up loops (`i++ < n`) on ARM.**

97. **Discuss how alignment and cache line boundaries affect SoA vs AoS performance.**

98. **Explain why `-Os` (optimize for size) might actually improve performance in some cases.**

99. **Analyze the complete optimization process: requirements → profile → Amdahl analysis → optimize → verify. Why is each step essential?**

100. **Synthesize the relationship between algorithmic optimization (e.g., Horner's rule), compiler optimization (e.g., -O3), and hardware features (e.g., NEON). Which should be prioritized and why?**

