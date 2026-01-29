# Lab P5: Design Space Exploration - AES Implementation Comparison

## Overview

This lab exercise applies **Design Space Exploration (DSE)** concepts to evaluate and compare the four different AES encryption implementations developed in previous labs. The goal is to systematically analyze trade-offs between **cost**, **performance (runtime/delay)**, and **code size (development effort)** to make an informed design decision rather than relying on intuition.

**Four AES Implementation Alternatives:**
1. **Software on Cortex-A53** (APU) - P3
2. **Software on Cortex-R5** (RPU) with remote procedure - P3
3. **Hardware acceleration on FPGA** - P4.1
4. **Hardware security unit (CSU)** - P4.2

The lab demonstrates the complete DSE process: data collection, normalization, visualization, Pareto analysis, and decision-making based on multi-objective optimization.

---

## Technical Terms & Definitions

**Design Space Exploration (DSE)**
- Systematic evaluation of multiple implementation alternatives for a system
- Analyzes trade-offs between conflicting objectives (cost, performance, power, size)
- Identifies Pareto-optimal solutions from the design space
- Essential for making informed architectural decisions

**Design Point**
- Single implementation alternative characterized by specific values for all objectives
- Each point in design space represents complete system configuration
- Example: "Cortex-A53 AES implementation" is one design point

**Design Space**
- Set of all possible implementation alternatives
- Can be discrete (finite options) or continuous (parameterized)
- In this lab: 4 discrete design points (A53, R5, FPGA, CSU)

**Objective / Optimization Criterion**
- Measurable characteristic to optimize (minimize or maximize)
- Examples: cost (minimize), performance (maximize), code size (minimize)
- Conflicting objectives create trade-offs

**Multi-Objective Optimization**
- Optimization problem with multiple, often conflicting objectives
- No single "best" solution - trade-offs exist
- Result: set of Pareto-optimal solutions
- Example: minimize cost AND maximize performance (conflict: faster hardware costs more)

**Pareto Dominance**
- Solution A dominates solution B if:
  - A is better or equal to B in all objectives, AND
  - A is strictly better than B in at least one objective
- Dominated solutions are never optimal choices

**Pareto Front / Pareto Optimal Set**
- Set of all non-dominated solutions
- No solution in the set dominates any other
- Represents the best possible trade-offs
- Decision depends on requirements/priorities

**Dominated Solution**
- Design point that is strictly worse than another in at least one objective while being no better in any other
- Should be eliminated from consideration
- Example: Solution with higher cost AND slower performance

**Normalization**
- Scaling values to common range (typically 0 to 1)
- Necessary when objectives have different units and scales
- Formula: normalized_value = value / max_value (where max = 1)
- Enables fair comparison and visualization

**Trade-off Analysis**
- Examining how improving one objective affects others
- Quantifies conflicts between objectives
- Example: FPGA faster than A53, but costs more

**Cost (Hardware Cost)**
- Unit price of hardware components required for implementation
- Measured in currency (e.g., Swiss Francs)
- Lower is better (minimize)
- Includes: processors, FPGA chips, secure elements

**Runtime / Delay / Latency**
- Time required to complete encryption/decryption operation
- Measured in milliseconds (ms) or microseconds (μs)
- Lower is better (minimize)
- Critical for real-time requirements

**Code Size / Binary Size / Development Effort**
- Size of compiled binary code
- Measured in kilobytes (KB)
- Proxy for development effort and complexity
- Larger code = more development time, testing, maintenance
- Lower is better (minimize)

**Throughput**
- Amount of data processed per unit time
- Inverse of runtime for fixed data size
- Measured in MB/s or operations/second
- Higher is better (maximize)
- Related to runtime: throughput = data_size / runtime

**3D Scatter Plot**
- Visualization showing design points in 3-dimensional space
- Axes represent three objectives (cost, runtime, code size)
- Enables visual identification of trade-offs and dominance
- Each point labeled with implementation name

**Design Decision**
- Selection of one implementation from Pareto front
- Based on requirements, priorities, constraints
- No universally "best" choice - depends on application context
- Examples: cost-sensitive → cheapest, real-time → fastest

**Benchmark**
- Standardized test for measuring system performance
- In this lab: AES encryption/decryption of 64-byte packets
- Ensures fair comparison (same workload for all alternatives)

**Cortex-A53 (APU)**
- ARM Application Processing Unit on Zynq UltraScale+ MPSoC
- High-performance processor running Linux
- Faster clock speed, suitable for complex software
- Higher cost than simpler processors

**Cortex-R5 (RPU)**
- ARM Real-Time Processing Unit on Zynq UltraScale+ MPSoC
- Real-time processor running FreeRTOS
- Lower clock speed, deterministic timing
- Lower cost than A53

**FPGA (Field-Programmable Gate Array)**
- Reconfigurable hardware for custom logic implementation
- Parallel execution enables very high performance
- Hardware development more complex than software
- Example: Xilinx Spartan-6 XC6SLX4

**CSU (Configuration Security Unit)**
- Dedicated hardware security module in Zynq UltraScale+
- Optimized for cryptographic operations
- Secure key storage (eFUSE)
- Socket-based interface (AF_ALG)
- Proxy: SE050 secure element for cost estimation

**Optimization Objective**
- Goal: minimize or maximize an objective function
- In this lab: minimize all three (cost, runtime, code size)
- Multi-objective: cannot minimize all simultaneously

**Requirements-Driven Design**
- Design decisions based on application requirements
- Hard constraints (must meet) vs preferences (nice to have)
- Example: if latency < 1ms required → eliminates R5

**Visualization**
- Graphical representation of design space
- Helps understand trade-offs and identify patterns
- 3D scatter plot shows three objectives simultaneously
- Alternative: 2D plots for pairs of objectives

**Data Collection**
- Gathering measurements for all objectives and alternatives
- Must be comparable (same test conditions, workload)
- Sources: timing measurements, price databases, binary file sizes

---

## Core Concepts

### 1. Purpose of Design Space Exploration

**Why DSE is Necessary:**
- Multiple implementation alternatives exist for same functionality
- Alternatives have different characteristics (cost, performance, size, power)
- No single "best" solution - trade-offs between objectives
- Intuition insufficient for complex decisions
- Systematic approach ensures informed decisions

**DSE Process:**
1. Define objectives (what to optimize)
2. Identify alternatives (design points)
3. Collect data (measure objectives for each alternative)
4. Normalize data (common scale)
5. Visualize design space
6. Identify Pareto-optimal solutions
7. Make decision based on requirements

**Benefits:**
- Objective comparison (data-driven, not gut feeling)
- Identifies dominated solutions (clearly worse)
- Reveals trade-offs explicitly
- Documents decision rationale
- Enables "what-if" analysis

### 2. The Four AES Implementation Alternatives

**1. Cortex-A53 Software Implementation:**
- Pure software AES running on Linux
- Advantages: no additional hardware cost (already present), easy development, flexible
- Disadvantages: moderate performance, consumes CPU resources
- From P3 lab (APU version)

**2. Cortex-R5 Software with Remote Procedure:**
- Software AES offloaded to R5 via RPMsg
- Advantages: dedicated processor (doesn't affect A53 workload), real-time behavior
- Disadvantages: communication overhead, slowest runtime, largest code size
- From P3 lab (RPU version)

**3. FPGA Hardware Acceleration:**
- Custom AES logic implemented in FPGA fabric
- Advantages: very high performance (parallel hardware), low power per operation
- Disadvantages: hardware development complexity, FPGA cost, fixed functionality
- From P4.1 lab (memory-mapped I/O interface)

**4. CSU Hardware Security Module:**
- Dedicated cryptographic accelerator
- Advantages: secure key storage, optimized for crypto, socket interface, low cost
- Disadvantages: limited to supported algorithms, moderate performance
- From P4.2 lab (AF_ALG socket interface)

### 3. Three Optimization Objectives

**Objective 1: Hardware Cost (Minimize)**
- What it represents: Bill of materials (BOM) cost for hardware components
- Why it matters: Directly affects product profitability, especially high-volume products
- Measurement: Unit prices from distributors (e.g., Digikey)
- Components:
  - A53: Part of Zynq UltraScale+ (30 Fr. allocated)
  - R5: Part of Zynq UltraScale+ (19 Fr. allocated)
  - FPGA: Separate Spartan-6 chip (22.10 Fr.)
  - CSU: SE050 secure element (3.40 Fr.)

**Objective 2: Runtime / Latency (Minimize)**
- What it represents: Time to encrypt/decrypt data
- Why it matters: Affects system responsiveness, real-time capability, throughput
- Measurement: Timing measurements for 64-byte packet AES operation
- Results:
  - A53: 0.159 ms (fast)
  - R5: 11.24 ms (slowest - communication overhead)
  - FPGA: 0.118 ms (fastest - parallel hardware)
  - CSU: 0.275 ms (good - optimized hardware)

**Objective 3: Code Size / Development Effort (Minimize)**
- What it represents: Compiled binary size as proxy for development complexity
- Why it matters: Development time, testing effort, maintenance cost, code complexity
- Measurement: Size of compiled binaries in KB
- Results:
  - A53: 75 KB (small - simple software)
  - R5: 688 KB (largest - includes FreeRTOS, RPMsg, firmware)
  - FPGA: 74 KB (small - minimal software, hardware in bitstream)
  - CSU: 375 KB (medium - socket interface code)

**Trade-offs:**
- Cost vs Performance: Faster implementations (FPGA) cost more
- Simplicity vs Performance: Simple software (A53) slower than hardware
- Security vs Cost: Secure solutions (CSU with eFUSE) add cost

### 4. Data Normalization Process

**Why Normalize:**
- Objectives have different units: Fr., ms, KB
- Different scales: 3.40 Fr. vs 0.118 ms (incomparable)
- Visualization requires common scale
- Equal weighting of objectives

**Normalization Method (Max-Norm):**
1. For each objective, find maximum value across all alternatives
2. Divide each value by the maximum: normalized = value / max_value
3. Result: normalized values in range [0, 1], where 1 = worst (highest) for that objective

**Example Calculation:**

**Cost Normalization:**
- Max cost = 30 Fr. (A53)
- A53: 30 / 30 = 1.0
- R5: 19 / 30 = 0.63 → rounded to 0.59 in data
- FPGA: 22.10 / 30 = 0.737 → 0.7
- CSU: 3.40 / 30 = 0.113 → 0 (best, lowest cost)

**Runtime Normalization:**
- Max runtime = 11.24 ms (R5)
- A53: 0.159 / 11.24 = 0.014 → 0.004 in data
- R5: 11.24 / 11.24 = 1.0 (worst)
- FPGA: 0.118 / 11.24 = 0.0105 → 0 (best, fastest)
- CSU: 0.275 / 11.24 = 0.024 → 0.014

**Code Size Normalization:**
- Max code size = 688 KB (R5)
- A53: 75 / 688 = 0.109 → 0.002
- R5: 688 / 688 = 1.0 (worst, largest)
- FPGA: 74 / 688 = 0.108 → 0 (best, smallest)
- CSU: 375 / 688 = 0.545 → 0.49

**Interpretation:**
- Lower normalized value = better (closer to optimal)
- Value of 1.0 = worst performing in that objective
- Value of 0.0 = best performing in that objective

### 5. 3D Visualization and Interpretation

**Visualization Setup:**
- X-axis: Cost (normalized)
- Y-axis: Development Effort / Code Size (normalized)
- Z-axis: Delay / Runtime (normalized)
- Each design point plotted as labeled sphere

**Reading the Plot:**
- Points closer to origin (0,0,0) are better (all objectives minimized)
- Points far from origin are worse
- Spatial relationships show trade-offs

**Design Point Positions:**
- **FPGA**: Near origin (x≈0.7, y≈0, z≈0) - excellent performance and code size, moderate cost
- **CSU**: Low cost and runtime (x≈0, y≈0.49, z≈0.014) - balanced solution
- **A53**: High cost (x≈1, y≈0.002, z≈0.004) - expensive but good performance
- **R5**: Worst runtime and code size (x≈0.59, y≈1, z≈1) - poor trade-offs

**Visual Insights:**
- R5 isolated in "bad" corner (high delay and code size)
- FPGA and CSU close to each other (similar overall quality)
- A53 expensive but fast
- Clear separation between good (FPGA, CSU, A53) and bad (R5) solutions

### 6. Pareto Dominance Analysis

**Dominance Rules:**
Solution A dominates solution B if:
- A ≤ B in all objectives (cost, runtime, code size), AND
- A < B in at least one objective

**Checking Each Pair:**

**A53 vs R5:**
- Cost: A53 (1.0) > R5 (0.59) → R5 better
- Runtime: A53 (0.004) < R5 (1.0) → A53 better
- Code: A53 (0.002) < R5 (1.0) → A53 better
- **Conclusion**: A53 dominates R5 (better in 2/3, worse in 1/3) → NO, neither dominates
- Wait, for dominance A must be better or equal in ALL. A53 is worse in cost, so doesn't dominate.
- R5 is worse in runtime and code, so doesn't dominate.
- **Neither dominates**

Actually, let me reconsider. For A to dominate B:
- A must be ≤ B in ALL objectives (equal or better)
- A must be < B in AT LEAST ONE objective (strictly better)

**A53 vs R5:**
- Cost: 1.0 vs 0.59 → A53 worse (NOT ≤)
- **A53 does NOT dominate R5** (fails first condition)

**R5 vs A53:**
- Cost: 0.59 vs 1.0 → R5 better
- Runtime: 1.0 vs 0.004 → R5 worse (NOT ≤)
- **R5 does NOT dominate A53**

**FPGA vs R5:**
- Cost: 0.7 vs 0.59 → FPGA worse (NOT ≤)
- **FPGA does NOT dominate R5**

**R5 vs FPGA:**
- Cost: 0.59 vs 0.7 → R5 better
- Runtime: 1.0 vs 0.0 → R5 worse (NOT ≤)
- **R5 does NOT dominate FPGA**

**CSU vs R5:**
- Cost: 0 vs 0.59 → CSU better (≤)
- Runtime: 0.014 vs 1.0 → CSU better (≤)
- Code: 0.49 vs 1.0 → CSU better (≤)
- **CSU dominates R5!** (better in all three objectives)

**R5 vs CSU:**
- Cost: 0.59 vs 0 → R5 worse
- Runtime: 1.0 vs 0.014 → R5 worse
- Code: 1.0 vs 0.49 → R5 worse
- **R5 does NOT dominate CSU** (confirmed)

**A53 vs FPGA:**
- Cost: 1.0 vs 0.7 → A53 worse
- **A53 does NOT dominate FPGA**

**FPGA vs A53:**
- Cost: 0.7 vs 1.0 → FPGA better (≤)
- Runtime: 0 vs 0.004 → FPGA better (≤)
- Code: 0 vs 0.002 → FPGA better (≤)
- **FPGA dominates A53!** (better in all three objectives)

**A53 vs CSU:**
- Cost: 1.0 vs 0 → A53 worse
- **A53 does NOT dominate CSU**

**CSU vs A53:**
- Cost: 0 vs 1.0 → CSU better (≤)
- Runtime: 0.014 vs 0.004 → CSU worse (NOT ≤)
- **CSU does NOT dominate A53**

**FPGA vs CSU:**
- Cost: 0.7 vs 0 → FPGA worse
- **FPGA does NOT dominate CSU**

**CSU vs FPGA:**
- Cost: 0 vs 0.7 → CSU better (≤)
- Runtime: 0.014 vs 0 → CSU worse (NOT ≤)
- **CSU does NOT dominate FPGA**

**Dominance Summary:**
- **CSU dominates R5** (CSU better in all three objectives)
- **FPGA dominates A53** (FPGA better in all three objectives)
- **Pareto Front: FPGA and CSU** (neither dominates the other)
- **Dominated: R5 and A53** (should be eliminated)

### 7. Decision-Making Based on Requirements

**Pareto Front: FPGA vs CSU Trade-off**
- Both are non-dominated solutions
- Choice depends on application requirements and priorities

**When to Choose FPGA:**
- **Performance-critical**: Absolute minimum latency required (0.118 ms vs 0.275 ms)
- **High-throughput**: Processing large volumes of data continuously
- **Real-time systems**: Hard deadlines with minimal jitter
- **Predictable timing**: Hardware execution time constant
- **Example**: High-speed network encryption, video processing

**When to Choose CSU:**
- **Cost-sensitive**: Budget constraints (3.40 Fr. vs 22.10 Fr. - 6.5x cheaper!)
- **Security-critical**: Need secure key storage in eFUSE
- **Balanced performance**: 0.275 ms acceptable for application
- **Easier development**: Socket interface simpler than FPGA hardware design
- **Flexibility**: Can use other crypto algorithms supported by CSU
- **Example**: IoT devices, cost-sensitive products, moderate performance needs

**Eliminated Solutions:**
- **R5**: Dominated by CSU (worse in all aspects) - never choose
- **A53**: Dominated by FPGA (worse in all aspects) - never choose

**Requirements-Driven Examples:**

**Example 1: Battery-Powered IoT Sensor**
- Requirements: Low cost (<5 Fr.), moderate latency (<5 ms), low power
- Decision: **CSU** (lowest cost, acceptable latency, secure key storage)

**Example 2: High-Speed Network Gateway**
- Requirements: Ultra-low latency (<0.2 ms), high throughput, cost less important
- Decision: **FPGA** (fastest, meets strict timing, cost justified by performance)

**Example 3: Safety-Critical Automotive**
- Requirements: Deterministic timing, secure storage, moderate cost
- Decision: **CSU** or **FPGA** depending on latency requirement
- If <0.15 ms needed: FPGA
- If <1 ms acceptable: CSU (cheaper, secure storage)

### 8. Practical DSE Process

**Step-by-Step Procedure:**

**1. Data Collection:**
- Measure runtime: Use timing instrumentation (clock_gettime) for each implementation
- Measure code size: Check compiled binary sizes (ls -lh)
- Research cost: Look up component prices on distributor websites (Digikey, Mouser)
- Ensure consistent test: Same workload (64-byte packet) for all alternatives

**2. Data Organization:**
- Create table with rows = alternatives, columns = objectives
- Raw values with units
- Document measurement methodology

**3. Normalization:**
- For each column, identify maximum value
- Divide all values by maximum
- Store normalized data (CSV or similar format)
- Verify: all values in [0, 1], max = 1 for each objective

**4. Visualization:**
- Use Python (matplotlib mplot3d) or similar tool
- Create 3D scatter plot with normalized axes
- Label each design point clearly
- Save high-resolution image

**5. Analysis:**
- Apply Pareto dominance rules systematically
- Identify dominated solutions
- Mark dominated points in visualization (different color/shape)
- Document Pareto front

**6. Decision:**
- Consider application requirements
- Select from Pareto front based on priorities
- Document rationale
- Communicate to stakeholders

**Tools Used:**
- Timing measurements: C code with clock_gettime()
- Price research: Digikey.ch or similar distributor
- Binary size: ls command or file properties
- Data processing: ChatGPT, Python, Excel
- Visualization: Python matplotlib (mplot3d), ChatGPT assistance
- Documentation: Text file, spreadsheet, written report

### 9. Connection to DSE Theory (Lectures 11 & 12)

**From Lecture 11 (DSE Intro):**
- This lab demonstrates complete DSE flow
- Design space: 4 discrete alternatives (finite, enumerated)
- Objectives: 3 conflicting criteria
- Multi-objective optimization applied
- Pareto analysis identifies optimal set

**From Lecture 12 (DSE Search):**
- Exhaustive search: All 4 alternatives evaluated (small design space)
- No heuristics needed (not searching, directly comparing known solutions)
- Pruning by dominance: Eliminated dominated solutions
- If design space larger: Would need search strategies (genetic algorithms, simulated annealing)

**Simplifications in This Lab:**
- Small design space (only 4 points) - exhaustive evaluation feasible
- Discrete alternatives - no parameter tuning
- Equal objective weights - no preference encoding
- Real DSE often has: thousands of design points, continuous parameters, constrained optimization

---

## Important Diagrams & Graphics

### Diagram 1: 3D Scatter Plot of Design Space

```
3D coordinate system showing 4 design points:

Z-axis (delay/runtime, normalized)
^
│ 1.0 - R5 (worst performance)
│
│ 0.5 -
│
│      CSU
│ 0.0 - FPGA, A53 (best performance)
│    /
│   /
│  / Y-axis (code size/dev effort, normalized)
│ /
└────────────────────> X-axis (cost, normalized)
0.0  0.5  1.0

Positions:
- A53:  (1.0, 0.002, 0.004) - top-right-front corner
- R5:   (0.59, 1.0, 1.0)    - mid-back-top corner (worst)
- FPGA: (0.7, 0.0, 0.0)     - mid-front-bottom (near origin)
- CSU:  (0.0, 0.49, 0.014)  - front-mid-bottom (near origin)
```

**Purpose**: Visualize all three objectives simultaneously
**Key Points**:
- Origin (0,0,0) = ideal solution (minimum cost, runtime, code size)
- R5 farthest from origin (worst overall)
- FPGA and CSU nearest to origin (best overall)
- Spatial relationships reveal trade-offs

### Diagram 2: Raw Data Table (Before Normalization)

```
┌─────────────┬──────────┬─────────────┬───────────────┐
│  Solution   │   Cost   │ Delay (ms)  │ Code Size (KB)│
├─────────────┼──────────┼─────────────┼───────────────┤
│ Cortex-A53  │ 30 Fr.   │   0.159     │      75       │
│ Cortex-R5   │ 19 Fr.   │  11.24      │     688       │
│ FPGA        │ 22.10 Fr.│   0.118     │      74       │
│ CSU         │ 3.40 Fr. │   0.275     │     375       │
└─────────────┴──────────┴─────────────┴───────────────┘

Maximum values (for normalization):
Cost:      30 Fr. (A53)
Runtime:   11.24 ms (R5)
Code Size: 688 KB (R5)
```

**Purpose**: Shows original measurements before normalization
**Key Points**:
- Different units (Fr., ms, KB) - incomparable without normalization
- R5 has maximum runtime and code size
- CSU significantly cheaper than others
- FPGA fastest in absolute terms

### Diagram 3: Normalized Data Table

```
┌─────────────┬────────────┬────────────┬────────────┐
│  Solution   │ Cost (norm)│Runtime(norm│Code Size   │
│             │            │)           │(norm)      │
├─────────────┼────────────┼────────────┼────────────┤
│ Cortex-A53  │   1.000    │   0.004    │   0.002    │
│ Cortex-R5   │   0.590    │   1.000    │   1.000    │
│ FPGA        │   0.700    │   0.000    │   0.000    │
│ CSU         │   0.000    │   0.014    │   0.490    │
└─────────────┴────────────┴────────────┴────────────┘

Interpretation:
1.0 = worst in that objective
0.0 = best in that objective
Lower values are better
```

**Purpose**: Data ready for visualization and comparison
**Key Points**:
- All values in [0, 1] range
- Common scale enables direct comparison
- Each objective has one value at 1.0 (worst) and one near 0.0 (best)

### Diagram 4: Pareto Dominance Relationships

```
Dominance Structure:

        FPGA ──────dominates──────> A53
         │                          (worse in all 3)
         │
    [Pareto Front]
         │
        CSU ────────dominates──────> R5
                                    (worse in all 3)

Pareto Front: {FPGA, CSU}
Dominated: {A53, R5}

Trade-off between FPGA and CSU:
  FPGA: Better performance, higher cost
  CSU:  Better cost, slightly worse performance
```

**Purpose**: Shows dominance relationships between design points
**Key Points**:
- Arrows point from dominating to dominated solution
- Pareto front contains only non-dominated solutions
- Choice between FPGA and CSU depends on requirements

### Diagram 5: DSE Process Flow

```
┌─────────────────────┐
│ Define Objectives   │
│ (Cost, Runtime,     │
│  Code Size)         │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│ Identify            │
│ Alternatives        │
│ (A53, R5, FPGA, CSU)│
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│ Collect Data        │
│ - Timing tests      │
│ - Price research    │
│ - Binary sizes      │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│ Normalize Data      │
│ (value / max)       │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│ Visualize           │
│ (3D scatter plot)   │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│ Pareto Analysis     │
│ (Find dominated)    │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│ Make Decision       │
│ (Based on           │
│  requirements)      │
└─────────────────────┘
```

**Purpose**: Shows complete DSE methodology
**Key Points**:
- Systematic, step-by-step process
- Data-driven decision making
- Objective analysis before subjective decision

### Diagram 6: Trade-off Visualization (2D Projections)

```
Cost vs Runtime:                Cost vs Code Size:

Runtime                         Code Size
  1.0 │ R5                        1.0 │ R5
      │                               │
  0.5 │                           0.5 │         CSU
      │     CSU                       │
  0.0 │ FPGA  A53                 0.0 │ FPGA  A53
      └─────────────> Cost            └─────────────> Cost
        0    0.5   1.0                   0    0.5   1.0

Runtime vs Code Size:

Code Size
  1.0 │ R5
      │
  0.5 │ CSU
      │
  0.0 │ FPGA  A53
      └─────────────> Runtime
        0    0.5   1.0
```

**Purpose**: 2D projections of 3D space for detailed trade-off analysis
**Key Points**:
- Each projection shows relationship between two objectives
- Helps identify specific trade-offs
- Easier to read than 3D for some analyses

### Diagram 7: Component Cost Breakdown

```
Hardware Cost Comparison:

30 Fr. ┤████████████████████████████ Cortex-A53
       │
22 Fr. ┤█████████████████████ FPGA (Spartan-6)
       │
19 Fr. ┤██████████████████ Cortex-R5
       │
 3 Fr. ┤███ CSU (SE050)
       └────────────────────────────────>
         0    10    20    30 Fr.

Cost ratio:
A53 : CSU = 8.8:1
FPGA : CSU = 6.5:1
```

**Purpose**: Visualizes absolute cost differences
**Key Points**:
- CSU dramatically cheaper than alternatives
- A53/R5 part of same chip (allocated cost)
- FPGA requires separate chip

### Diagram 8: Performance Comparison (Runtime)

```
Encryption Time for 64-byte packet:

11.24 ms ┤████████████████████████ Cortex-R5 (SLOWEST)
         │
 0.275 ms┤█ CSU
         │
 0.159 ms┤█ Cortex-A53
         │
 0.118 ms┤█ FPGA (FASTEST)
         └─────────────────────────────────>
           0       5      10   ms

Speedup vs R5:
FPGA: 95x faster
A53:  71x faster
CSU:  41x faster
```

**Purpose**: Shows absolute performance differences
**Key Points**:
- R5 much slower due to RPMsg communication overhead
- FPGA and A53 similar performance (both fast)
- Hardware solutions (FPGA, CSU) optimized for AES

### Diagram 9: Code Size Comparison

```
Binary Size:

688 KB ┤████████████████████████████ Cortex-R5
       │
375 KB ┤██████████████ CSU
       │
 75 KB ┤██ Cortex-A53
       │
 74 KB ┤██ FPGA
       └────────────────────────────────>
         0    200   400   600   KB

Size ratio:
R5 : FPGA = 9.3:1
R5 : A53  = 9.2:1
```

**Purpose**: Visualizes development complexity proxy
**Key Points**:
- R5 includes FreeRTOS, RPMsg framework, firmware (large)
- FPGA hardware logic in bitstream (not counted in binary)
- A53 simple software (minimal dependencies)

---

## Formulas & Procedures

### Formula 1: Normalization (Max-Norm)

**normalized_value = raw_value / max_value**

Where:
- raw_value = measured value for design point
- max_value = maximum value across all design points for that objective
- Result: normalized_value ∈ [0, 1], where 1 = worst

**Example:**
- FPGA cost = 22.10 Fr., max cost = 30 Fr.
- Normalized = 22.10 / 30 = 0.737 ≈ 0.7

**Alternative: Min-Max Normalization (not used in this lab):**
**normalized = (value - min) / (max - min)**
- Result: 0 = worst (minimum), 1 = best (maximum)
- Reverses interpretation

### Formula 2: Throughput from Runtime

**Throughput = Data_Size / Runtime**

Where:
- Throughput in bytes/second (or MB/s)
- Data_Size = amount of data processed (64 bytes in this lab)
- Runtime in seconds

**Example (FPGA):**
- Data = 64 bytes, Runtime = 0.118 ms = 0.000118 s
- Throughput = 64 / 0.000118 = 542,372 bytes/s ≈ 530 KB/s

### Formula 3: Speedup Factor

**Speedup = Runtime_baseline / Runtime_alternative**

Where:
- Runtime_baseline = runtime of reference implementation
- Runtime_alternative = runtime of faster implementation
- Result > 1: alternative is faster

**Example (FPGA vs R5):**
- Speedup = 11.24 ms / 0.118 ms = 95.3x
- FPGA is 95 times faster than R5

### Formula 4: Cost-Performance Ratio

**Cost_per_performance = Hardware_Cost / Throughput**

Where:
- Units: Fr. per (bytes/second) or similar
- Lower is better (more performance per unit cost)

**Example (CSU):**
- Cost = 3.40 Fr., Throughput = 64 bytes / 0.000275 s = 232,727 bytes/s
- Ratio = 3.40 / 232,727 = 0.0000146 Fr. per byte/s

### Procedure 1: Complete DSE Process

**Step 1: Define Objectives**
- Identify what to optimize (minimize or maximize)
- This lab: minimize cost, runtime, code size
- Ensure measurable and comparable

**Step 2: Identify Design Alternatives**
- List all implementation options
- This lab: 4 AES implementations (A53, R5, FPGA, CSU)
- Could be parameterized (clock speeds, buffer sizes) or discrete

**Step 3: Collect Data**
- Measure each objective for each alternative
- Timing: Instrument code, run benchmarks
- Cost: Research component prices
- Code size: Check binary file sizes
- Ensure fair comparison (same workload)

**Step 4: Create Data Table**
- Rows = design alternatives
- Columns = objectives
- Fill with raw measurements and units

**Step 5: Normalize Data**
- For each objective column:
  - Find maximum value
  - Divide all values by maximum
  - Store normalized values
- Verify all values in [0, 1]

**Step 6: Export Normalized Data**
- Save to CSV or text file
- Format: comma-separated, three columns
- Example line: "Cortex-A53,1.0,0.004,0.002"

**Step 7: Visualize**
- Load normalized data into visualization tool
- Create 3D scatter plot
- Label each point with alternative name
- Save high-resolution image

**Step 8: Pareto Analysis**
- For each pair of design points, check dominance
- Mark dominated points differently in visualization
- List Pareto-optimal solutions

**Step 9: Make Decision**
- Consider application requirements
- Select from Pareto front based on priorities
- Document rationale
- Communicate decision to stakeholders

### Procedure 2: Checking Pareto Dominance

**To check if design A dominates design B:**

```
dominated = True
strictly_better = False

For each objective i:
    If A[i] > B[i]:  # A worse than B in objective i
        dominated = False
        break
    If A[i] < B[i]:  # A better than B in objective i
        strictly_better = True

If dominated AND strictly_better:
    return "A dominates B"
Else:
    return "A does not dominate B"
```

**Repeat for all pairs to find Pareto front**

**Example (CSU vs R5):**
- Cost: 0 < 0.59 ✓ (CSU better)
- Runtime: 0.014 < 1.0 ✓ (CSU better)
- Code: 0.49 < 1.0 ✓ (CSU better)
- dominated = True, strictly_better = True
- **Result: CSU dominates R5**

### Procedure 3: Creating 3D Scatter Plot in Python

**Python code structure (with ChatGPT assistance):**

```python
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd

# Load normalized data
data = pd.read_csv('comparison.csv')

# Create 3D plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Scatter plot
ax.scatter(data['Cost'], data['Code Size'], data['Runtime'])

# Labels
ax.set_xlabel('cost (normalized)')
ax.set_ylabel('development effort / code size (normalized)')
ax.set_zlabel('delay')

# Add point labels manually
# ... (add text annotations for each point)

plt.show()
```

**Tips:**
- Use ChatGPT to generate initial code
- Manually add point labels for clarity
- Adjust viewing angle for best perspective
- Export as PNG for documentation

### Procedure 4: Requirements-Based Selection

**Decision tree for choosing from Pareto front:**

```
1. Identify hard constraints (must-have requirements):
   - Maximum cost budget
   - Maximum latency requirement
   - Security requirements

2. Filter Pareto front by hard constraints:
   - Eliminate solutions violating any hard constraint

3. Among remaining solutions, evaluate soft preferences:
   - Performance priority → choose fastest
   - Cost priority → choose cheapest
   - Balance → use weighted scoring

4. Document decision:
   - Selected solution
   - Rationale (which requirements drove decision)
   - Trade-offs accepted
```

**Example:**
- Hard constraint: Cost < 10 Fr.
- Filters to: CSU (3.40 Fr.)
- If also: Latency < 0.2 ms
- Conflict! No solution satisfies both
- Must relax one constraint or reconsider design space

---

## Comparisons & Contrasts

### Comparison 1: Hardware Cost Trade-offs

| Solution | Cost (Fr.) | Cost Rank | Key Cost Factors |
|----------|-----------|-----------|------------------|
| CSU      | 3.40      | 1 (Best)  | Simple secure element chip |
| Cortex-R5| 19        | 2         | Allocated cost from Zynq SoC |
| FPGA     | 22.10     | 3         | Separate Spartan-6 chip |
| Cortex-A53| 30       | 4 (Worst) | Allocated cost from Zynq SoC |

**Key Insights**:
- CSU dramatically cheaper (8.8x vs A53)
- FPGA requires additional chip (not in base system)
- A53/R5 costs are allocations from same SoC
- For high-volume products, CSU cost advantage significant

### Comparison 2: Performance (Runtime) Trade-offs

| Solution | Runtime (ms) | Speedup vs R5 | Performance Rank |
|----------|-------------|---------------|------------------|
| FPGA     | 0.118       | 95.3x         | 1 (Best)         |
| A53      | 0.159       | 70.7x         | 2                |
| CSU      | 0.275       | 40.9x         | 3                |
| R5       | 11.24       | 1.0x          | 4 (Worst)        |

**Key Insights**:
- FPGA fastest (parallel hardware execution)
- A53 close second (high clock speed, optimized software)
- CSU good for hardware (optimized crypto accelerator)
- R5 slow due to RPMsg communication overhead, not CPU speed
- All except R5 acceptable for most real-time requirements (<1 ms)

### Comparison 3: Development Effort (Code Size)

| Solution | Code Size (KB) | Complexity | Development Rank |
|----------|---------------|------------|------------------|
| FPGA     | 74            | Hardware + minimal SW | 1 (Best) |
| A53      | 75            | Simple software | 1 (tied) |
| CSU      | 375           | Socket interface code | 3 |
| R5       | 688           | FreeRTOS + RPMsg + firmware | 4 (Worst) |

**Key Insights**:
- FPGA smallest (hardware logic in bitstream, not binary)
- A53 simple standalone software
- CSU moderate (socket interface more complex)
- R5 largest (includes entire RTOS and communication framework)
- Code size correlates with development and maintenance effort

### Comparison 4: Pareto Front Solutions (FPGA vs CSU)

| Aspect | FPGA | CSU | Winner |
|--------|------|-----|---------|
| **Cost** | 22.10 Fr. (0.7 norm) | 3.40 Fr. (0.0 norm) | CSU (6.5x cheaper) |
| **Runtime** | 0.118 ms (0.0 norm) | 0.275 ms (0.014 norm) | FPGA (2.3x faster) |
| **Code Size** | 74 KB (0.0 norm) | 375 KB (0.49 norm) | FPGA (5x smaller) |
| **Security** | No key protection | eFUSE secure storage | CSU |
| **Flexibility** | Fixed AES only | Multiple algorithms | CSU |
| **Determinism** | Fixed timing | Variable (kernel scheduling) | FPGA |

**Trade-off Summary**:
- FPGA: Best performance and simplicity, higher cost
- CSU: Best cost and security features, slightly slower
- Neither dominates - choice depends on priorities

### Comparison 5: Dominated Solutions (Why Eliminated)

**A53 vs FPGA (A53 dominated):**
- Cost: A53 worse (30 vs 22.10 Fr.)
- Runtime: A53 worse (0.159 vs 0.118 ms)
- Code size: A53 worse (75 vs 74 KB)
- **A53 worse in all three → dominated**
- Conclusion: No reason to choose A53 over FPGA

**R5 vs CSU (R5 dominated):**
- Cost: R5 worse (19 vs 3.40 Fr.)
- Runtime: R5 much worse (11.24 vs 0.275 ms)
- Code size: R5 worse (688 vs 375 KB)
- **R5 worse in all three → dominated**
- Conclusion: No reason to choose R5 over CSU

### Comparison 6: Application Suitability

| Application Type | Best Choice | Rationale |
|------------------|-------------|-----------|
| **High-volume IoT device** | CSU | Lowest cost (critical for volume), secure storage, acceptable performance |
| **Real-time control** | FPGA | Fastest, deterministic timing, meets hard deadlines |
| **Network gateway** | FPGA | Highest throughput for continuous encryption |
| **Secure storage** | CSU | eFUSE key protection, lowest cost |
| **Development prototype** | A53 or FPGA | A53 simplest software, FPGA if performance matters |
| **Battery-powered** | FPGA or CSU | Hardware acceleration = lower power per operation |
| **Safety-critical** | FPGA | Deterministic, no OS jitter |

**Key Insight**: No universal "best" - depends on application requirements

### Comparison 7: Multi-Objective Optimization Approaches

| Approach | Description | Used in Lab? | Pros | Cons |
|----------|-------------|--------------|------|------|
| **Weighted Sum** | Combine objectives: score = w1×cost + w2×runtime + w3×size | No | Simple, single score | Weights subjective, can miss Pareto points |
| **Pareto Analysis** | Find non-dominated solutions | Yes | Objective, no weights needed | Multiple solutions, requires secondary decision |
| **Constrained Opt** | Optimize one objective, constrain others | No | Clear primary goal | Must set constraint values |
| **Goal Programming** | Minimize distance from ideal point | No | Considers all objectives | Ideal point may not exist |

**Lab Approach (Pareto)**:
- Finds all objectively good solutions
- No subjective weights during analysis
- Decision based on requirements (after analysis)
- Best for exploratory design

---

## Examples & Applications

### Example 1: Complete DSE Workflow

**Scenario:** Compare 4 AES implementations

**Step 1: Data Collection**
- Measured timing: Ran benchmarks on all 4 implementations for 64-byte packet
- Researched cost: Digikey prices for components
- Checked size: `ls -lh` on compiled binaries

**Raw Data:**
- A53: 30 Fr., 0.159 ms, 75 KB
- R5: 19 Fr., 11.24 ms, 688 KB
- FPGA: 22.10 Fr., 0.118 ms, 74 KB
- CSU: 3.40 Fr., 0.275 ms, 375 KB

**Step 2: Normalization**
- Max values: Cost=30, Runtime=11.24, Size=688
- Normalized data:
  - A53: 1.0, 0.014, 0.109
  - R5: 0.63, 1.0, 1.0
  - FPGA: 0.737, 0.0105, 0.108
  - CSU: 0.113, 0.024, 0.545

**Step 3: Visualization**
- Created 3D scatter plot with Python
- Plotted 4 points in (cost, code, runtime) space

**Step 4: Analysis**
- Found: FPGA dominates A53, CSU dominates R5
- Pareto front: {FPGA, CSU}

**Step 5: Decision**
- Requirement: Cost < 5 Fr., Latency < 1 ms
- CSU satisfies both (3.40 Fr., 0.275 ms)
- **Selected: CSU**

### Example 2: Pareto Dominance Check

**Comparing CSU vs R5:**

**Check if CSU dominates R5:**
- Cost: CSU (0.0) ≤ R5 (0.59) ✓
- Runtime: CSU (0.014) ≤ R5 (1.0) ✓
- Code: CSU (0.49) ≤ R5 (1.0) ✓
- All objectives: CSU ≤ R5 ✓
- At least one strict: CSU < R5 in all three ✓
- **Conclusion: CSU dominates R5**

**Check if R5 dominates CSU:**
- Cost: R5 (0.59) > CSU (0.0) ✗
- Fails first condition
- **Conclusion: R5 does NOT dominate CSU**

**Result**: R5 is dominated, should be eliminated from consideration

### Example 3: Requirements-Driven Selection (High-Volume Product)

**Scenario:** IoT temperature sensor for smart home
- Production volume: 1 million units
- Requirements:
  - Cost: <5 Fr. per unit (hard constraint)
  - Latency: <10 ms (hard constraint)
  - Security: Key storage required (hard constraint)
  - Size: Small form factor preferred

**Analysis:**
1. **Filter by cost:**
   - CSU: 3.40 Fr. ✓
   - R5: 19 Fr. ✗ (too expensive)
   - FPGA: 22.10 Fr. ✗ (too expensive)
   - A53: 30 Fr. ✗ (too expensive)
   - Remaining: CSU only

2. **Check latency:**
   - CSU: 0.275 ms ✓ (well within 10 ms)

3. **Check security:**
   - CSU: eFUSE key storage ✓

**Decision: CSU**
- Only solution meeting hard constraints
- Cost savings: (22.10 - 3.40) × 1M = 18.7M Fr. saved vs FPGA
- Acceptable performance for application
- Secure key storage included

### Example 4: Requirements-Driven Selection (Real-Time Control)

**Scenario:** Industrial motor controller
- Safety-critical application (SIL 3)
- Requirements:
  - Latency: <0.15 ms (hard deadline)
  - Determinism: Fixed execution time required
  - Cost: <50 Fr. acceptable
  - Security: Nice to have, not critical

**Analysis:**
1. **Filter by latency:**
   - FPGA: 0.118 ms ✓
   - A53: 0.159 ms ✗ (exceeds 0.15 ms deadline)
   - CSU: 0.275 ms ✗ (too slow)
   - R5: 11.24 ms ✗ (way too slow)
   - Remaining: FPGA only

2. **Check determinism:**
   - FPGA: Hardware execution, fixed timing ✓

3. **Check cost:**
   - FPGA: 22.10 Fr. ✓ (within budget)

**Decision: FPGA**
- Only solution meeting hard deadline
- Deterministic timing required for safety certification
- Cost acceptable for safety-critical application
- Hardware reliability (no software bugs)

### Example 5: Trade-off Analysis (CSU vs FPGA)

**Scenario:** Network encryption device

**Option A: FPGA**
- Pros:
  - Fastest (0.118 ms) → 530 KB/s throughput
  - Smallest code (74 KB) → easy maintenance
  - Deterministic timing
- Cons:
  - Higher cost (22.10 Fr.)
  - Hardware development expertise needed
  - Fixed to AES (not flexible)

**Option B: CSU**
- Pros:
  - Lowest cost (3.40 Fr.) → 85% cost reduction
  - Secure key storage (eFUSE)
  - Flexible (supports multiple algorithms)
  - Socket interface (standard API)
- Cons:
  - Slower (0.275 ms) → 233 KB/s throughput
  - Larger code (375 KB)
  - Depends on kernel (less deterministic)

**Trade-off Quantification:**
- Performance difference: FPGA 2.3x faster than CSU
- Cost difference: FPGA 6.5x more expensive than CSU
- Cost per performance: FPGA 2.8x worse than CSU

**Decision Factors:**
- If throughput > 300 KB/s required → FPGA
- If cost critical and 233 KB/s sufficient → CSU
- If need algorithm flexibility → CSU
- If need maximum determinism → FPGA

### Example 6: Visualization Creation with ChatGPT

**User prompt to ChatGPT:**
```
"I have a CSV file with 4 rows and 3 columns: cost, runtime, code_size.
All values are normalized between 0 and 1.
Create a Python script using matplotlib to make a 3D scatter plot
with cost on x-axis, code_size on y-axis, and runtime on z-axis.
Label each point with the solution name from the first column."
```

**ChatGPT Response:**
- Provides Python code with imports, data loading, 3D plotting
- May need iteration: "make points larger", "change axis labels", "adjust viewing angle"
- Final step: manually add point labels in code or graphically

**Result:**
- Professional 3D visualization in minutes
- No need to learn matplotlib details
- Iterative refinement through conversation

### Example 7: Normalization Calculation

**Given Raw Data:**
- A53 cost: 30 Fr.
- R5 cost: 19 Fr.
- FPGA cost: 22.10 Fr.
- CSU cost: 3.40 Fr.

**Normalization Steps:**

1. **Identify maximum:**
   - Max cost = 30 Fr. (A53)

2. **Apply formula for each:**
   - A53: 30 / 30 = 1.000
   - R5: 19 / 30 = 0.633 → rounded to 0.59 in data
   - FPGA: 22.10 / 30 = 0.737 → rounded to 0.7
   - CSU: 3.40 / 30 = 0.113 → rounded to 0 (best)

3. **Verify range:**
   - All values in [0, 1] ✓
   - Maximum = 1.0 ✓
   - Minimum near 0 ✓

**Interpretation:**
- A53 normalized cost = 1.0 → worst (most expensive)
- CSU normalized cost ≈ 0 → best (cheapest)
- Relative differences preserved: FPGA 2.1x cheaper than A53 in normalized and raw

---

## Connections to Other Topics

### Connection 1: Lecture 11 - DSE Introduction

**Direct Application:**
- Lab P5 applies DSE theory from Lecture 11
- Design space: 4 discrete alternatives (A53, R5, FPGA, CSU)
- Objectives: 3 conflicting criteria (cost, runtime, code size)
- Multi-objective optimization demonstrated
- Pareto analysis applied to find optimal solutions

**Key Concepts Used:**
- Design point definition
- Objective functions
- Pareto dominance
- Trade-off analysis

### Connection 2: Lecture 12 - DSE Search Strategies

**Relevance:**
- Lab uses exhaustive evaluation (all 4 alternatives tested)
- Small design space → no search needed
- Pruning by dominance (eliminating R5, A53)
- If larger space: would need heuristic search (genetic algorithms, simulated annealing)

**Comparison:**
- Exhaustive (Lab): 4 points, directly compare all
- Search (Lecture 12): thousands of points, need efficient exploration

### Connection 3: Lab P3 - AES Multi-Processor

**Measurements from P3:**
- A53 implementation: timing data used in P5
- R5 implementation: timing data used in P5
- P3 developed two of four alternatives being compared
- RPMsg communication overhead explains R5 slowness

**Key Link:**
- P3 implementations are design points in P5 design space

### Connection 4: Lab P4 - AES in Hardware

**Measurements from P4:**
- FPGA implementation (P4.1): timing and cost data
- CSU implementation (P4.2): timing and cost data
- P4 developed other two alternatives being compared

**Hardware Trade-offs:**
- FPGA fastest (parallel hardware) but requires separate chip
- CSU balanced (optimized hardware, integrated in SoC)

### Connection 5: Lecture 08 - Energy

**Energy as Additional Objective:**
- Could add energy/power as 4th objective
- Hardware accelerators (FPGA, CSU) typically lower energy per operation
- Software on A53/R5 higher energy due to longer runtime
- Trade-off: cost vs performance vs energy

**Connection:**
- Lab P2 measured energy for AES
- Could integrate P2 energy data into P5 DSE

### Connection 6: Lecture 09 - Performance

**Performance Metrics:**
- Runtime/latency: key objective in P5
- Throughput: derived from runtime (data_size / runtime)
- Speedup: quantifies performance improvement (FPGA 95x vs R5)
- Performance vs cost trade-off central to P5 decision

**Key Link:**
- Performance analysis techniques from Lecture 9 applied in P5

### Connection 7: Lecture 10 - FPGA Introduction

**FPGA Knowledge Applied:**
- FPGA as AES implementation platform (P4.1)
- Hardware acceleration principles
- Parallel execution enables high performance
- Development complexity trade-off

**Key Link:**
- Understanding FPGA from Lecture 10 essential for evaluating FPGA design point

### Connection 8: Lecture 06 - Non-Functional Requirements

**NFRs as Objectives:**
- Performance (runtime) = NFR
- Cost (hardware budget) = NFR
- Code size (maintainability) = NFR proxy
- DSE process evaluates NFR trade-offs
- Multi-objective optimization addresses conflicting NFRs

**Key Link:**
- DSE is method for analyzing NFR trade-offs

### Connection 9: Lecture 13 - RTOS Scheduling

**R5 Implementation Context:**
- R5 runs FreeRTOS (from P3)
- Scheduling overhead contributes to R5 code size
- Real-time behavior of R5 vs non-real-time A53
- Determinism consideration in design choice

**Key Link:**
- RTOS complexity explains R5 large binary size (688 KB)

### Connection 10: Lab P1 - Hardware Bring-up

**Foundation:**
- P1 introduced Ultra96 hardware platform
- Understanding of processors (A53, R5) necessary for P5
- Hardware constraints affect design alternatives

**Key Link:**
- Platform knowledge from P1 informs P5 feasibility

### Connection 11: Lab P2 - Energy Measurement

**Energy Data Integration:**
- P2 measured AES energy consumption
- Could add as 4th objective in P5 DSE
- Energy-performance-cost trade-offs
- Important for battery-powered applications

**Potential Extension:**
- 4D DSE with energy included

### Connection 12: Lecture 02 - Software Paradigms

**Implementation Paradigms:**
- A53/R5: Software implementations (different paradigms)
- FPGA: Hardware description paradigm
- CSU: Hardware/software co-design
- Design choice affects development paradigm

**Key Link:**
- Software vs hardware implementation paradigms create trade-offs

### Connection 13: Lecture 05 - Development Processes

**DSE in Development:**
- DSE occurs in architectural design phase
- Informs platform selection decisions
- Trade-off analysis before detailed design
- Helps validate requirements feasibility

**Key Link:**
- DSE is part of systematic development process

### Connection 14: Lecture 01 - Embedded Systems

**Embedded Constraints:**
- Cost sensitivity (high volume production)
- Resource constraints (memory, power)
- Real-time requirements
- All create trade-offs explored in DSE

**Key Link:**
- Embedded system characteristics drive multi-objective optimization

---

## Summary

**Lab P5 (Design Space Exploration)** brings together all previous AES implementations (from Labs P3 and P4) to demonstrate **systematic evaluation and comparison of design alternatives**. Rather than relying on intuition, the lab applies formal **multi-objective optimization** techniques to analyze trade-offs between **cost**, **performance (runtime)**, and **development effort (code size)**.

The four alternatives evaluated are:
1. **Cortex-A53 software** (fast, expensive, simple code)
2. **Cortex-R5 with remote procedure** (slow due to communication overhead, moderate cost, complex code)
3. **FPGA hardware acceleration** (fastest, moderate-high cost, minimal code)
4. **CSU hardware security** (fast, cheapest, moderate code complexity)

The **complete DSE process** includes:
1. **Data collection**: Measuring runtime (benchmarks), researching cost (component prices), checking code size (binary files)
2. **Normalization**: Scaling all values to [0, 1] range for fair comparison
3. **Visualization**: Creating 3D scatter plot showing all design points in multi-dimensional objective space
4. **Pareto analysis**: Identifying dominated solutions (R5 and A53) and Pareto-optimal solutions (FPGA and CSU)
5. **Decision-making**: Selecting from Pareto front based on application requirements

**Key findings:**
- **FPGA dominates A53** (better in all three objectives) → A53 eliminated
- **CSU dominates R5** (better in all three objectives) → R5 eliminated
- **Pareto front: {FPGA, CSU}** → neither dominates the other, choice depends on priorities
  - FPGA: Best performance (0.118 ms), but higher cost (22.10 Fr.)
  - CSU: Best cost (3.40 Fr.), but slightly slower (0.275 ms)

**Design recommendations:**
- **Cost-sensitive applications** (e.g., high-volume IoT): Choose **CSU** (6.5x cheaper than FPGA, acceptable performance)
- **Performance-critical applications** (e.g., real-time control): Choose **FPGA** (2.3x faster than CSU, meets tight deadlines)
- **Security-critical applications**: Consider **CSU** (secure key storage in eFUSE)
- **Never choose R5 or A53**: Both dominated by better alternatives

The lab demonstrates that **data-driven design space exploration** enables informed, objective decisions about system architecture, explicitly revealing trade-offs and eliminating suboptimal alternatives. This systematic approach is essential for embedded systems where multiple conflicting requirements (cost, performance, power, security) must be balanced.

---

## Possible Exam Questions

### Definitions & Technical Terms (Questions 1-20)

1. **What is Design Space Exploration (DSE) and why is it important?**
   - Systematic evaluation of multiple implementation alternatives
   - Analyzes trade-offs between conflicting objectives
   - Identifies optimal solutions from design space
   - Important: informed decisions vs gut feeling, reveals trade-offs, documents rationale

2. **Define a design point in the context of DSE.**
   - Single implementation alternative with specific values for all objectives
   - Example: "Cortex-A53 AES" is one design point characterized by cost, runtime, code size

3. **What is Pareto dominance? Provide the formal definition.**
   - Solution A dominates B if: A ≤ B in all objectives AND A < B in at least one
   - Dominated solutions never optimal choices
   - Example: CSU dominates R5 (better in cost, runtime, AND code size)

4. **Define the Pareto front and explain its significance.**
   - Set of all non-dominated solutions
   - No solution in set dominates any other
   - Represents best possible trade-offs
   - Decision depends on requirements/priorities

5. **What is multi-objective optimization and why does it create trade-offs?**
   - Optimization with multiple, often conflicting objectives
   - Trade-offs: improving one objective may worsen another
   - Example: faster hardware costs more (performance vs cost conflict)
   - No single "best" solution - multiple Pareto-optimal alternatives

6. **Explain what normalization is and why it's necessary in DSE.**
   - Scaling values to common range (typically 0 to 1)
   - Necessary: objectives have different units (Fr., ms, KB) and scales
   - Enables fair comparison and visualization
   - Formula: normalized = value / max_value

7. **What does it mean for a solution to be "dominated"? Should dominated solutions ever be chosen?**
   - Strictly worse than another in at least one objective while no better in any other
   - Should NEVER be chosen (always a better alternative exists)
   - Example: R5 dominated by CSU (worse in all three objectives)

8. **Define the three objectives optimized in Lab P5.**
   - Cost: hardware component price (minimize)
   - Runtime/Delay: time to complete encryption (minimize)
   - Code Size: binary size as development effort proxy (minimize)

9. **What is throughput and how does it relate to runtime?**
   - Amount of data processed per unit time
   - Throughput = data_size / runtime (inverse relationship)
   - Higher throughput = better performance
   - Example: FPGA throughput = 64 bytes / 0.118 ms = 542 KB/s

10. **What is a trade-off in the context of multi-objective optimization?**
    - Situation where improving one objective worsens another
    - Inherent in conflicting objectives
    - Example: FPGA faster than CSU (better performance) but costs more (worse cost)
    - Quantified by comparing Pareto-optimal solutions

11. **Define speedup factor and provide the formula.**
    - Ratio measuring performance improvement: Speedup = Runtime_baseline / Runtime_alternative
    - Result > 1 means alternative is faster
    - Example: FPGA vs R5 = 11.24 / 0.118 = 95x speedup

12. **What is the role of visualization in DSE?**
    - Graphical representation of design space
    - Enables intuitive understanding of trade-offs
    - 3D scatter plot shows three objectives simultaneously
    - Helps identify dominated solutions visually

13. **Explain the difference between hard constraints and soft preferences in design decisions.**
    - Hard constraints: must be satisfied (requirements that cannot be violated)
    - Soft preferences: nice to have (guide choice among feasible solutions)
    - Example hard: cost < 10 Fr., Example soft: prefer faster solution

14. **What is the CSU (Configuration Security Unit) and what are its advantages?**
    - Dedicated hardware security module in Zynq UltraScale+
    - Advantages: secure key storage (eFUSE), optimized for crypto, low cost, socket interface
    - Used for AES implementation in Lab P4.2

15. **Define FPGA in context of AES implementation alternatives.**
    - Field-Programmable Gate Array: reconfigurable hardware
    - Custom logic for AES enables parallel execution
    - Advantages: very high performance, deterministic timing
    - Disadvantages: hardware development complexity, cost

16. **What is a 3D scatter plot and how is it used in Lab P5?**
    - Visualization showing points in 3-dimensional space
    - Axes: cost (x), code size (y), runtime (z)
    - Each point = one design alternative
    - Enables visual identification of trade-offs and dominance

17. **Explain cost-performance ratio as a metric.**
    - Cost per unit of performance: Cost / Throughput
    - Lower is better (more performance per dollar)
    - Helps quantify value proposition
    - Example: CSU has better cost-performance ratio than FPGA

18. **What does "objective" mean in multi-objective optimization?**
    - Measurable characteristic to optimize (minimize or maximize)
    - Must be quantifiable and comparable
    - Examples: cost (minimize), throughput (maximize), energy (minimize)

19. **Define benchmark in context of performance evaluation.**
    - Standardized test for measuring system performance
    - Ensures fair comparison (same workload for all alternatives)
    - Lab P5: AES encryption/decryption of 64-byte packets

20. **What is meant by "requirements-driven design"?**
    - Design decisions based on application requirements
    - Hard constraints filter feasible solutions
    - Soft preferences guide final selection
    - Example: if latency < 1ms required → eliminates R5

### Core Concepts (Questions 21-45)

21. **Why is systematic DSE better than intuitive decision-making?**
    - Objective, data-driven vs subjective gut feeling
    - Reveals trade-offs explicitly
    - Identifies dominated solutions (objectively worse)
    - Documents decision rationale for stakeholders
    - Enables "what-if" analysis

22. **Describe the complete DSE process used in Lab P5.**
    - Define objectives (cost, runtime, code size)
    - Identify alternatives (A53, R5, FPGA, CSU)
    - Collect data (measurements, research)
    - Normalize data (common scale)
    - Visualize (3D scatter plot)
    - Pareto analysis (find dominated)
    - Make decision (based on requirements)

23. **Explain why all three objectives in Lab P5 are "minimize" objectives.**
    - Cost: lower is better (reduces product cost)
    - Runtime: lower is better (faster response, higher throughput)
    - Code size: lower is better (less development effort, simpler maintenance)
    - All represent costs or overheads to minimize

24. **How does normalization enable fair comparison of objectives with different units?**
    - Raw data: 30 Fr. vs 0.159 ms (incomparable - different units and scales)
    - Normalized: 1.0 vs 0.004 (comparable - same [0,1] scale)
    - Each objective gets equal weight
    - Enables visualization and objective comparison

25. **Why does R5 have the slowest runtime despite similar CPU architecture to A53?**
    - RPMsg communication overhead dominates
    - Data must be transferred to R5 via shared memory
    - Context switches between A53 and R5
    - Remote procedure call latency
    - Not CPU speed issue - communication bottleneck

26. **Explain the FPGA vs CSU trade-off on the Pareto front.**
    - FPGA: Better performance (0.118 ms vs 0.275 ms), smaller code, but 6.5x more expensive
    - CSU: Better cost (3.40 vs 22.10 Fr.), secure storage, but 2.3x slower
    - Neither dominates - both on Pareto front
    - Choice depends on: cost priority → CSU, performance priority → FPGA

27. **Why is code size used as a proxy for development effort?**
    - Larger binaries indicate more complex code
    - More code = more development time, testing, maintenance
    - R5: 688 KB (includes FreeRTOS, RPMsg) = complex development
    - A53: 75 KB (simple software) = easier development
    - Imperfect but reasonable proxy

28. **How does the 3D scatter plot help identify dominated solutions?**
    - Dominated solutions farther from origin (0,0,0) in "bad" direction
    - R5 in far corner (high y and z values) = clearly worse
    - Visual clustering: good solutions near origin, bad solutions far away
    - Lines can be drawn to show dominance relationships

29. **What role do application requirements play in selecting from the Pareto front?**
    - Requirements filter Pareto solutions by hard constraints
    - Priorities determine choice among remaining solutions
    - Example: cost < 5 Fr. constraint → CSU only option
    - Example: latency < 0.15 ms → FPGA only option
    - No universal "best" - depends on application context

30. **Why can't we minimize all three objectives simultaneously?**
    - Objectives are conflicting (trade-offs exist)
    - Faster hardware (minimize runtime) costs more (increases cost)
    - Simpler code (minimize code size) may sacrifice performance
    - Must accept trade-offs - no free lunch
    - Pareto front represents best compromises

31. **Explain how ChatGPT was used in Lab P5 workflow.**
    - Generate Python code for visualization from description
    - Iterative refinement: "make points larger", "change labels"
    - Rapid prototyping without deep matplotlib knowledge
    - Manual adjustments for final polish (labels, viewing angle)
    - Demonstrates AI-assisted engineering workflow

32. **Why are both A53 and R5 part of the design space despite being on the same SoC?**
    - Different use cases: A53 for general computing, R5 for real-time
    - Different characteristics: A53 faster clock, R5 deterministic
    - Offloading to R5 frees A53 for other tasks
    - Cost allocated differently (A53 higher share)
    - Represents architectural choice even on same chip

33. **What does the hyperperiod refer to in the "worst" direction in the 3D plot mean?**
    - Point (1, 1, 1) represents worst possible solution in all objectives
    - R5 closest to this (0.59, 1.0, 1.0) - worst or tied-worst in all metrics
    - Clearly dominated
    - Origin (0, 0, 0) represents ideal (unattainable) solution

34. **How does secure key storage in CSU affect design decision?**
    - eFUSE provides tamper-resistant key storage
    - Critical for security-sensitive applications
    - Differentiates CSU from other alternatives
    - May override cost/performance considerations if security required
    - Example: payment terminal requires secure storage → CSU preferred

35. **Why is FPGA performance so much better than software implementations?**
    - Parallel hardware execution (all operations simultaneous)
    - No instruction fetch/decode overhead
    - Optimized for specific AES operations
    - No OS overhead, interrupts, context switches
    - Custom datapath for AES rounds

36. **Explain the concept of "dominated solution should never be chosen".**
    - Dominated solution strictly worse than another in at least one aspect, no better in any
    - Choosing dominated = objectively bad decision (better alternative exists)
    - Example: Choosing R5 over CSU = paying more (19 vs 3.40 Fr.) for worse performance
    - Rational decision-making eliminates dominated solutions

37. **How does the data collection phase ensure fair comparison?**
    - Same workload: All implementations tested with 64-byte packet
    - Same measurement method: Timing with clock_gettime()
    - Same conditions: Same hardware platform (Ultra96)
    - Same test: Encryption + decryption for all
    - Ensures apples-to-apples comparison

38. **Why might code size not perfectly reflect development effort?**
    - Hardware development (FPGA) effort not captured in software binary size
    - Generated code vs hand-written code (different effort)
    - Code reuse: large binaries may include libraries (less development)
    - Complexity vs size: 100 lines of complex algorithm > 1000 lines boilerplate
    - Despite limitations, still useful proxy

39. **What is the significance of throughput for different applications?**
    - High-throughput apps (network gateway): need fastest solution (FPGA)
    - Low-throughput apps (occasional encryption): CSU sufficient
    - Throughput = data_size / runtime
    - FPGA throughput: 542 KB/s, CSU: 233 KB/s
    - Requirements drive minimum acceptable throughput

40. **How does normalization affect the interpretation of differences?**
    - Relative differences preserved: FPGA 2.3x faster than CSU (raw and normalized)
    - Absolute values lost: normalized doesn't show actual cost/time
    - Focus on relationships between alternatives
    - Need raw data table alongside normalized for complete picture

41. **Explain the role of the CSV file in the DSE workflow.**
    - Stores normalized data in machine-readable format
    - Input for visualization tools (Python scripts)
    - Format: comma-separated, rows=alternatives, columns=objectives
    - Enables reproducibility and automation
    - Can be imported to Excel, Python, other tools

42. **Why is visualization important even with quantitative dominance analysis?**
    - Humans better at visual pattern recognition
    - 3D plot shows all three objectives at once (holistic view)
    - Intuitive understanding of trade-offs
    - Easier to communicate to non-technical stakeholders
    - Complements quantitative analysis

43. **How does hardware acceleration affect both performance and cost?**
    - Performance: Dedicated hardware much faster (parallel, optimized)
    - Cost: Additional hardware increases BOM cost
    - Creates fundamental trade-off: speed vs cost
    - FPGA: highest performance, high cost
    - CSU: good performance, low cost (optimized for crypto)

44. **What factors beyond the three objectives might influence real design decisions?**
    - Security features (eFUSE storage)
    - Power consumption (battery life)
    - Development time (time-to-market)
    - Flexibility (support multiple algorithms)
    - Certifiability (safety standards)
    - Supply chain (component availability)
    - These could be additional objectives or constraints

45. **Explain how Lab P5 integrates knowledge from previous labs (P2, P3, P4).**
    - P2: Energy measurement (potential 4th objective)
    - P3: Developed A53 and R5 implementations being compared
    - P4: Developed FPGA and CSU implementations being compared
    - P5: Compares all implementations systematically
    - Demonstrates complete design flow: implementation → measurement → evaluation

### Comparisons & Analysis (Questions 46-70)

46. **Compare FPGA and CSU in terms of cost, performance, and security features.**
    - Cost: CSU 3.40 Fr., FPGA 22.10 Fr. (FPGA 6.5x more expensive)
    - Performance: FPGA 0.118 ms, CSU 0.275 ms (FPGA 2.3x faster)
    - Security: CSU has eFUSE secure storage, FPGA does not
    - Trade-off: FPGA better performance, CSU better cost and security

47. **Why is A53 dominated by FPGA? Show the dominance calculation.**
    - Cost: FPGA (0.7) < A53 (1.0) → FPGA better
    - Runtime: FPGA (0.0) < A53 (0.004) → FPGA better
    - Code: FPGA (0.0) < A53 (0.002) → FPGA better
    - FPGA better in ALL three objectives → A53 dominated
    - No reason to choose A53 over FPGA

48. **Compare the code sizes: why is R5 binary so much larger (688 KB vs 75 KB for A53)?**
    - R5 includes: FreeRTOS kernel, RPMsg framework, firmware for R5, communication protocol
    - A53: standalone application, minimal dependencies
    - RTOS overhead: task management, scheduling, IPC
    - R5 9x larger than A53
    - Reflects development complexity

49. **What is the cost-performance trade-off between CSU and FPGA?**
    - FPGA: High performance (0.118 ms), high cost (22.10 Fr.) → 187 Fr./ms
    - CSU: Moderate performance (0.275 ms), low cost (3.40 Fr.) → 12.4 Fr./ms
    - CSU 15x better cost-performance ratio
    - But FPGA achieves absolute best performance
    - Choice depends on: budget constraints vs performance requirements

50. **Compare throughput: which alternative best for high-data-rate applications?**
    - FPGA: 64 / 0.000118 = 542 KB/s (best)
    - A53: 64 / 0.000159 = 402 KB/s
    - CSU: 64 / 0.000275 = 233 KB/s
    - R5: 64 / 0.01124 = 5.7 KB/s (worst)
    - High-rate applications (>300 KB/s): FPGA required

51. **Why are there only two solutions on the Pareto front despite four alternatives?**
    - A53 dominated by FPGA (worse in all objectives)
    - R5 dominated by CSU (worse in all objectives)
    - Only FPGA and CSU non-dominated
    - Demonstrates: half the alternatives objectively suboptimal

52. **Compare the development effort: software (A53) vs hardware (FPGA).**
    - A53: Pure software, familiar paradigm, easy debugging, 75 KB code
    - FPGA: Hardware description language, synthesis flow, simulation/verification, 74 KB software interface
    - Similar code size doesn't capture FPGA hardware development
    - A53 easier for software developers
    - FPGA requires hardware expertise

53. **How does the speedup of FPGA vs R5 (95x) compare to cost ratio (1.2x)?**
    - Speedup: FPGA 95x faster than R5
    - Cost ratio: FPGA 1.16x more expensive than R5
    - Extreme performance gain for minimal cost increase
    - But comparison misleading: R5 artificially slow (communication overhead)
    - Better comparison: FPGA vs A53 (1.3x speedup, 0.74x cost)

54. **Compare CSU to each of the other three solutions on each objective.**
    - CSU vs A53: CSU better cost (0.0 vs 1.0), better code (0.49 vs 0.002 - wait, A53 better), worse runtime (0.014 vs 0.004) - neither dominates
    - CSU vs R5: CSU better in ALL (cost, runtime, code) - CSU dominates
    - CSU vs FPGA: CSU better cost, worse runtime, worse code - neither dominates
    - Result: CSU on Pareto front (only dominated by nothing)

Actually let me recalculate:
- CSU vs A53:
  - Cost: CSU (0.0) < A53 (1.0) ✓ CSU better
  - Runtime: CSU (0.014) > A53 (0.004) ✗ A53 better
  - CSU does NOT dominate A53 (worse in runtime)

55. **What are the advantages and disadvantages of each design point?**
    - **A53**: Fast (0.159 ms), small code (75 KB), but expensive (30 Fr.) - dominated
    - **R5**: Moderate cost (19 Fr.), but slow (11.24 ms), large code (688 KB) - dominated
    - **FPGA**: Fastest (0.118 ms), small code (74 KB), but costly (22.10 Fr.) - Pareto optimal
    - **CSU**: Cheapest (3.40 Fr.), secure storage, but slower (0.275 ms) - Pareto optimal

56. **How would adding energy consumption as a 4th objective change the analysis?**
    - Hardware accelerators (FPGA, CSU) likely lower energy per operation
    - Software (A53, R5) higher energy due to longer runtime
    - Would strengthen FPGA/CSU position (better in 4 objectives)
    - Might reveal new trade-offs (e.g., FPGA fast but high power)
    - 4D visualization challenging (multiple 3D plots or color coding)

57. **Compare the suitability of each solution for battery-powered devices.**
    - Lowest energy: FPGA or CSU (hardware acceleration, short runtime)
    - Highest energy: R5 (11.24 ms runtime)
    - Cost important for volume: CSU best (3.40 Fr.)
    - **Recommendation: CSU** (low cost, low energy, secure)

58. **What is the trade-off between flexibility and performance?**
    - FPGA: Fixed to AES (not flexible), highest performance
    - CSU: Supports multiple crypto algorithms (flexible), good performance
    - Software (A53/R5): Can implement any algorithm (most flexible), varying performance
    - Specialization improves performance but reduces flexibility

59. **Compare the determinism of each implementation.**
    - FPGA: Hardware execution, fixed timing (most deterministic)
    - R5: RTOS on R5, predictable scheduling (quite deterministic)
    - A53: Linux, non-real-time OS (least deterministic)
    - CSU: Kernel driver, variable scheduling (moderate determinism)
    - Safety-critical applications: FPGA or R5

60. **How do the Pareto front solutions compare to the dominated solutions?**
    - Pareto (FPGA, CSU): Near origin in 3D plot (good in all objectives)
    - Dominated (A53, R5): Far from origin (bad in one or more objectives)
    - Visual separation clear in scatter plot
    - Pareto solutions 2.3-95x better performance, up to 8.8x better cost

61. **Compare the security implications of each implementation.**
    - CSU: eFUSE secure key storage (best security)
    - FPGA: No key protection (keys in memory/config)
    - A53/R5: Keys in software memory (vulnerable)
    - Security-critical: CSU required
    - Others: Need external secure element

62. **What is the trade-off between code size and performance?**
    - Generally: More complex code (larger) → better performance
    - But in this lab: FPGA smallest code (0.0) AND fastest (0.0)
    - Hardware acceleration breaks this rule
    - R5: Large code (1.0) AND slow (1.0) - worst of both worlds

63. **Compare the impact of each objective on final design decision.**
    - Cost: Differentiates CSU (best) from FPGA (worse) on Pareto front
    - Performance: Differentiates FPGA (best) from CSU (worse) on Pareto front
    - Code size: Less important (both Pareto solutions good)
    - Decision usually driven by cost OR performance, code size secondary

64. **How do absolute values compare to normalized values in decision-making?**
    - Absolute: Shows real-world impact (3.40 Fr. vs 22.10 Fr.)
    - Normalized: Enables fair comparison across objectives
    - Both needed: normalized for analysis, absolute for decision
    - Example: 2.3x faster may not matter if both < 1ms requirement

65. **Compare the development complexity of each alternative.**
    - Easiest: A53 (simple software, standard tools)
    - Moderate: CSU (socket API, kernel interaction)
    - Complex: R5 (RTOS, RPMsg, multi-processor coordination)
    - Most complex: FPGA (hardware design, synthesis, verification)

66. **What are the implications of the large variation in runtimes (0.118 to 11.24 ms)?**
    - 95x difference between fastest and slowest
    - Some applications: any solution fast enough (<1 ms)
    - Other applications: only FPGA/A53 meet deadlines
    - Demonstrates importance of measuring and comparing

67. **How does the choice between Pareto solutions depend on production volume?**
    - Low volume: Cost difference less important (few units)
    - High volume: Cost difference amplified (millions of units)
    - Example: 1M units → (22.10-3.40) × 1M = 18.7M Fr. savings with CSU
    - High volume strongly favors CSU (cost-optimized)

68. **Compare the time-to-market implications of each alternative.**
    - Fastest: A53 (simple software, no new infrastructure)
    - Moderate: CSU (socket API well-documented)
    - Slower: FPGA (hardware development, verification)
    - Slowest: R5 (RTOS integration, RPMsg setup, testing)

69. **What is the trade-off between using the existing SoC (A53/R5) vs adding external components (FPGA)?**
    - Existing SoC: No additional BOM cost (amortized), simple procurement
    - External components: Additional cost per unit, supply chain complexity
    - But external may offer better performance or features
    - Design decision: leverage existing vs optimize for requirements

70. **How does the dominance analysis simplify the decision problem?**
    - Reduces 4 alternatives to 2 (eliminates A53, R5)
    - Simplifies: 4-way comparison → 2-way comparison
    - Focuses attention on real trade-offs (cost vs performance)
    - Makes decision easier for stakeholders

### Application & Problem-Solving (Questions 71-100)

71. **Given a requirement: cost < 10 Fr., latency < 1 ms. Which solution would you choose?**
    - Filter by cost: CSU (3.40 ✓), R5 (19 ✗), FPGA (22.10 ✗), A53 (30 ✗)
    - Check latency: CSU 0.275 ms ✓ (< 1 ms)
    - **Answer: CSU** (only solution meeting both constraints)

72. **Calculate the throughput of each implementation for 64-byte packets.**
    - FPGA: 64 / 0.000118 = 542,372 bytes/s ≈ 530 KB/s
    - A53: 64 / 0.000159 = 402,516 bytes/s ≈ 393 KB/s
    - CSU: 64 / 0.000275 = 232,727 bytes/s ≈ 227 KB/s
    - R5: 64 / 0.01124 = 5,694 bytes/s ≈ 5.6 KB/s

73. **If you need to encrypt 1 MB/s continuous stream, which implementations can meet this requirement?**
    - 1 MB/s = 1,024 KB/s
    - FPGA: 530 KB/s ✗ (too slow!)
    - Others: All slower than FPGA ✗
    - **Answer: NONE** (need faster implementation or parallel instances)

74. **Perform Pareto dominance check: Does A53 dominate R5?**
    - Cost: A53 (1.0) > R5 (0.59) → R5 better (A53 NOT ≤)
    - A53 does not dominate R5 (fails cost condition)
    - Runtime: R5 (1.0) > A53 (0.004) → A53 better (R5 NOT ≤)
    - R5 does not dominate A53 (fails runtime condition)
    - **Answer: Neither dominates the other**

75. **For a safety-critical automotive application requiring deterministic timing, which solution would you recommend and why?**
    - Requirements: Deterministic timing (hard real-time), safety certification
    - FPGA: Hardware execution, fixed timing, certifiable ✓✓✓
    - R5: RTOS real-time, but still software complexity ✓
    - A53: Linux non-deterministic ✗
    - CSU: Kernel driver, variable ✗
    - **Answer: FPGA** (most deterministic, hardware reliability)

76. **Calculate the normalized runtime value for A53 given: A53 runtime = 0.159 ms, max runtime = 11.24 ms.**
    - Normalized = value / max = 0.159 / 11.24 = 0.01414 ≈ 0.014
    - (Note: data shows 0.004, possible rounding or different measurement)

77. **Design a multi-criteria scoring function with weights: cost=0.5, performance=0.3, code size=0.2. Which solution scores best?**
    - A53: 0.5×1.0 + 0.3×0.004 + 0.2×0.002 = 0.5012
    - R5: 0.5×0.59 + 0.3×1.0 + 0.2×1.0 = 0.795
    - FPGA: 0.5×0.7 + 0.3×0.0 + 0.2×0.0 = 0.35
    - CSU: 0.5×0.0 + 0.3×0.014 + 0.2×0.49 = 0.1022
    - **Answer: CSU** (lowest score = best, cost-weighted favors cheap CSU)

78. **If FPGA cost dropped to 5 Fr., how would this change the Pareto front?**
    - New normalized cost: 5 / 30 = 0.167 (better than before: 0.7)
    - FPGA still dominates A53 (better in all three)
    - FPGA vs CSU: FPGA worse cost (0.167 vs 0), same runtime, same code
    - CSU still not dominated by FPGA
    - **Answer: Pareto front unchanged (FPGA, CSU)** but trade-off shifts toward FPGA

79. **For a development prototype where speed-to-market is critical, which implementation would you choose?**
    - A53: Simplest development, standard software tools ✓✓✓
    - CSU: Moderate complexity (socket API)
    - R5: Complex (RTOS, RPMsg)
    - FPGA: Most complex (hardware design)
    - **Answer: A53** despite being dominated (time-to-market trumps optimality for prototype)

80. **Calculate the cost savings per unit if choosing CSU over FPGA for 100,000 units.**
    - Cost difference: 22.10 - 3.40 = 18.70 Fr. per unit
    - Total savings: 18.70 × 100,000 = 1,870,000 Fr.
    - **Answer: 1.87 million Fr. saved**

81. **How would you visualize 4 objectives (cost, runtime, code size, energy) in DSE?**
    - Cannot directly visualize 4D
    - Options:
      1. Multiple 3D plots (each showing 3 of 4 objectives)
      2. 3D plot with 4th dimension as color/size of points
      3. Parallel coordinates plot
      4. Table with Pareto dominance analysis
    - Best: Combination of 3D plot + color coding + table

82. **Given raw data: FPGA cost=22.10, A53 cost=30, R5 cost=19, CSU cost=3.40. Calculate all normalized costs.**
    - Max = 30 (A53)
    - A53: 30/30 = 1.0
    - R5: 19/30 = 0.633
    - FPGA: 22.10/30 = 0.737
    - CSU: 3.40/30 = 0.113

83. **If energy consumption is: FPGA=10mJ, A53=25mJ, CSU=15mJ, R5=100mJ per operation, add energy as 4th objective. Which solutions are Pareto-optimal?**
    - Normalize energy: FPGA=0.1, A53=0.25, CSU=0.15, R5=1.0
    - Check dominance with 4 objectives (cost, runtime, code, energy):
      - FPGA still dominates A53 (better/equal in all 4)
      - CSU still dominates R5 (better in all 4)
      - FPGA vs CSU: neither dominates (FPGA better in 3/4, CSU better in cost)
    - **Answer: Pareto front still {FPGA, CSU}**

84. **Create a decision matrix for IoT sensor: requirements = low cost (weight=0.6), low latency (weight=0.2), security (weight=0.2).**
    - Assumptions: Security binary (CSU=1, others=0)
    - Scoring (lower is better for cost/latency):
      - A53: 0.6×1.0 + 0.2×0.004 + 0.2×0 = 0.6008
      - R5: 0.6×0.59 + 0.2×1.0 + 0.2×0 = 0.554
      - FPGA: 0.6×0.7 + 0.2×0 + 0.2×0 = 0.42
      - CSU: 0.6×0 + 0.2×0.014 + 0.2×1 = 0.2028
    - **Answer: CSU** (best score, low cost + security)

85. **How would you verify that your DSE analysis is correct?**
    - Check measurements: Re-run timing tests, verify prices
    - Verify normalization: Max value = 1.0 for each objective
    - Check dominance logic: Manually verify each pair
    - Cross-check with colleagues: Compare results
    - Sensitivity analysis: Small changes in data shouldn't drastically change Pareto front
    - Documentation: Ensure methodology transparent and reproducible

86. **Design a DSE for adding a 5th alternative: "Hardware AES on A53" (using ARM crypto extensions). How would you approach this?**
    - Measure cost: Same as A53 (no additional hardware) = 30 Fr.
    - Measure runtime: Faster than software (crypto instructions) ≈ 0.05 ms (estimate)
    - Measure code size: Similar to A53 (minimal change) ≈ 80 KB
    - Normalize with new max values (if any change)
    - Add to 3D plot
    - Check dominance against all others
    - Likely: dominates software A53 (same cost, better performance)

87. **If latency requirement changes from <1ms to <0.2ms, which solutions remain feasible?**
    - FPGA: 0.118 ms ✓ (meets requirement)
    - A53: 0.159 ms ✓ (meets requirement)
    - CSU: 0.275 ms ✗ (exceeds 0.2 ms)
    - R5: 11.24 ms ✗ (way over)
    - **Feasible: FPGA, A53**
    - Between these: FPGA dominates A53 → **Answer: FPGA**

88. **Calculate speedup: How much faster is FPGA than CSU?**
    - Speedup = Runtime_CSU / Runtime_FPGA = 0.275 / 0.118 = 2.33
    - **Answer: FPGA is 2.33x faster than CSU**

89. **For a system requiring both AES and RSA encryption, which alternative offers best flexibility?**
    - CSU: Supports multiple algorithms (AES, RSA, etc.) via AF_ALG ✓✓✓
    - FPGA: Fixed to AES (would need separate RSA module) ✗
    - A53/R5: Software can implement any algorithm ✓
    - **Answer: CSU** (hardware performance + algorithm flexibility)

90. **How would you present the DSE results to a non-technical manager?**
    - Show 3D scatter plot (visual)
    - Explain: "Points near origin are better"
    - Highlight dominated solutions: "These two are objectively worse"
    - Focus on Pareto front: "Choice between these two"
    - Present trade-off: "FPGA faster but costs 6x more"
    - Recommend based on priorities: "For cost-sensitive product, choose CSU"

91. **Calculate cost-performance ratio (Fr. per KB/s) for FPGA and CSU.**
    - FPGA: 22.10 Fr. / 530 KB/s = 0.0417 Fr. per KB/s
    - CSU: 3.40 Fr. / 227 KB/s = 0.0150 Fr. per KB/s
    - CSU 2.8x better cost-performance ratio
    - **Answer: CSU more cost-effective**

92. **If you could only choose one metric to optimize, which would you choose and why?**
    - Depends on application:
    - High-volume consumer: Cost (dominates unit economics)
    - Real-time control: Runtime (safety/deadlines)
    - Rapid prototype: Code size (development time)
    - Generally: **Cost** for commercial products (directly affects profitability)

93. **Design a test to validate that timing measurements are accurate and repeatable.**
    - Run multiple iterations (100+) of each implementation
    - Calculate mean and standard deviation
    - Check for outliers (remove or investigate)
    - Ensure consistent workload (same 64-byte packet)
    - Verify system idle during measurement (no background load)
    - Use high-resolution timer (clock_gettime with CLOCK_MONOTONIC)
    - Report: mean ± std dev

94. **How would adding "development time" as a separate objective change the analysis?**
    - Different from code size (code size is proxy, but not perfect)
    - Development time considerations:
      - A53: Fastest (simple software)
      - CSU: Moderate (API learning curve)
      - R5: Slow (RTOS + RPMsg complexity)
      - FPGA: Slowest (hardware design + verification)
    - Might strengthen A53 position (despite dominated by FPGA in other metrics)
    - Time-to-market often trump's optimality in practice

95. **Create a sensitivity analysis: How would a 10% increase in FPGA cost affect the decision?**
    - New FPGA cost: 22.10 × 1.1 = 24.31 Fr.
    - New max cost: 30 Fr. (unchanged, A53 still most expensive)
    - New normalized: 24.31 / 30 = 0.81 (vs 0.7 before)
    - Dominance: FPGA still dominates A53 (better in all)
    - Pareto front: Still {FPGA, CSU}
    - Trade-off: Shifts slightly toward CSU (cost difference larger)
    - **Answer: Decision unchanged, but CSU slightly more attractive**

96. **For a medical device requiring FDA/CE certification, which implementation would you choose?**
    - Certification considerations:
      - FPGA: Hardware simplicity, deterministic ✓✓
      - A53: Linux too complex for certification ✗
      - R5: RTOS certifiable (if using certified RTOS) ✓
      - CSU: Kernel driver certification complex ✗
    - **Answer: FPGA** (simplest to certify) or **R5** (if using certified RTOS like VxWorks)

97. **Design a weighted scoring function where all three Pareto-optimal solutions (including tie) score equally.**
    - Pareto front: {FPGA, CSU} (no tie in actual data)
    - To make them score equally with weights w1, w2, w3:
      - w1×0.7 + w2×0 + w3×0 = w1×0 + w2×0.014 + w3×0.49
      - 0.7w1 = 0.014w2 + 0.49w3
      - Many solutions exist (infinite)
    - Example: w1=0.7, w2=35, w3=1 (weights need not sum to 1)
    - Demonstrates: weighted sum can match any Pareto solution with appropriate weights

98. **How would you explain to a colleague why R5 should never be chosen?**
    - "R5 is dominated by CSU - CSU is better in every single metric"
    - "CSU costs 5.6x less (3.40 vs 19 Fr.)"
    - "CSU is 40x faster (0.275 vs 11.24 ms)"
    - "CSU has smaller code (375 vs 688 KB)"
    - "There is no scenario where R5 is the right choice"
    - "Choosing R5 = paying more for worse performance - objectively bad decision"

99. **If you had to recommend one solution without knowing the application requirements, which would it be?**
    - CSU: Best all-around
      - Lowest cost (3.40 Fr.) - good for volume production
      - Good performance (0.275 ms) - acceptable for most applications
      - Secure key storage - valuable feature
      - Moderate code complexity
    - Safe choice for unknown requirements
    - **Answer: CSU** (balanced solution on Pareto front)

100. **Summarize the key lessons learned from Lab P5 about design space exploration.**
     - Systematic analysis beats intuition
     - Half the alternatives can be objectively eliminated (dominated)
     - Trade-offs are inevitable (can't optimize all objectives simultaneously)
     - No universal "best" - depends on requirements
     - Visualization helps communicate complex trade-offs
     - Data-driven decisions require good measurements
     - Pareto analysis identifies objectively optimal solutions
     - Final decision requires requirements knowledge
     - DSE essential for multi-objective embedded systems design

---

**End of Lab P5 Notes**
