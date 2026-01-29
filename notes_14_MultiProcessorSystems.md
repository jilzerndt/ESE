# Lecture 14: Multi-Processor Systems - Memory Access Architectures

## Overview

This lecture explores the evolution of multi-processor system architectures and their memory access strategies. As embedded systems demand increasing computational power, moving from single-core to multi-core processors introduces fundamental challenges in **memory bandwidth**, **cache coherence**, and **scalability**. The lecture covers three major architectural approaches: **Traditional Memory Access** (shared bus), **Uniform Memory Access (UMA)**, and **Non-Uniform Memory Access (NUMA)**, concluding with ARM's **big.LITTLE** heterogeneous multi-core architecture that optimizes both performance and energy efficiency.

**Key Topics:**
- Single CPU and multi-CPU traditional architectures with shared system bus
- Uniform Memory Access (UMA) with centralized memory controller (Northbridge)
- Non-Uniform Memory Access (NUMA) with distributed local memory per CPU
- Cache coherence challenges (snooping, write invalidation)
- big.LITTLE architecture combining high-performance and energy-efficient cores
- Execution models: CPU Migration vs Global Task Scheduling

---

## Technical Terms & Definitions

**System Bus**
- Shared communication channel connecting CPU, memory, and I/O devices
- Carries both data and instructions
- In single-CPU systems: only one master (CPU)
- In multi-CPU systems: requires arbitration for bus access
- Bottleneck for multi-processor systems due to shared bandwidth

**Bus Arbitration**
- Mechanism for multiple CPUs to coordinate access to shared system bus
- Required when multiple masters (CPUs) need to use the same bus
- Introduces latency (delay while waiting for bus access)
- CPU may stall while waiting for bus access
- Limits scalability of multi-CPU systems

**Memory Bandwidth**
- Rate at which data can be read from or written to memory
- Measured in bytes/second or GB/s
- Critical bottleneck in multi-processor systems
- Shared among all CPUs in traditional and UMA architectures
- Per-CPU bandwidth decreases as more CPUs are added

**Cache**
- Fast memory buffer between CPU and main memory
- Bridges the performance gap between CPU speed and memory speed
- Multiple levels: L1 (fastest, smallest), L2, L3 (slowest, largest)
- L1/L2 typically per-core (private)
- L3 typically shared among cores

**Cache Coherence**
- Problem of keeping multiple cache copies of same memory location consistent
- Arises when multiple CPUs cache the same memory block
- Each CPU may have different copy of data
- Critical challenge in multi-processor systems
- Requires hardware and software support

**Snooping**
- Cache coherence protocol where caches monitor (snoop) the bus
- Each cache listens to what other CPUs are loading from memory
- Detects when other CPUs access data also in local cache
- Enables coordinated cache updates
- Used in UMA systems with shared bus

**Write Invalidation**
- Cache coherence technique where writing to memory invalidates cached copies
- When CPU writes to memory block, that block marked invalid in other caches
- Forces other CPUs to reload from memory (getting updated value)
- Alternative: immediately update all cache copies
- Standard approach in modern multi-core processors

**Uniform Memory Access (UMA)**
- Architecture where all CPUs have equal (uniform) access time to any memory location
- Memory centralized with shared controller (Northbridge)
- All CPUs connected to memory controller via dedicated links
- Access time independent of which CPU accesses which memory
- Simplifies programming (no need to consider memory locality)

**Non-Uniform Memory Access (NUMA)**
- Architecture where memory access time depends on memory location relative to CPU
- Each CPU has local memory (fast access)
- Can access remote memory on other CPUs (slower access via interconnects)
- Local access faster than remote access → non-uniform
- Scales better than UMA but more complex to program

**Northbridge (Memory Controller)**
- Central hub in UMA architecture connecting CPUs to memory
- Replaced the shared system bus with higher-bandwidth connections
- Handles memory access requests from all CPUs
- Must have higher bandwidth than system bus to prevent bottleneck
- Connects to multiple memory channels for increased bandwidth
- Bottleneck limits UMA scalability

**Memory Controller**
- Hardware component managing access to memory
- In UMA: centralized in Northbridge
- In NUMA: integrated into each CPU (local memory controller)
- Handles address decoding, timing, refresh, etc.
- In NUMA: also handles remote memory access requests

**Interconnect**
- Communication fabric connecting CPUs in multi-processor system
- In NUMA: enables remote memory access between CPUs
- Can be bus-based or point-to-point
- Trade-off: bandwidth vs complexity vs latency
- Examples: Intel QuickPath Interconnect (QPI), AMD HyperTransport, ARM CoreLink

**Point-to-Point Interconnect**
- Direct connections between every pair of CPUs
- Eliminates multi-hop latency (compared to shared interconnect)
- Higher bandwidth (dedicated links)
- But: complexity increases with N² connections for N CPUs
- Requires sophisticated routing logic

**Local Memory**
- Memory directly attached to a CPU in NUMA architecture
- Fast access (no interconnect traversal)
- CPU's own memory address space
- Physically close to CPU (lower latency, higher bandwidth)

**Remote Memory**
- Memory attached to a different CPU in NUMA architecture
- Accessed via interconnect
- Slower access than local memory
- Contributes to non-uniform access times
- May require multiple hops through interconnects

**L1 Cache (Level 1)**
- Smallest, fastest cache level
- Directly integrated into CPU core
- Typically split: instruction cache (L1I) and data cache (L1D)
- Size: typically 16-64 KB per core
- Lowest latency: 3-5 clock cycles

**L2 Cache (Level 2)**
- Intermediate cache level
- Larger than L1 (typically 256 KB - 1 MB per core)
- Private to each core in most designs
- Acts as buffer reducing L3 and memory access
- Latency: ~12-20 clock cycles

**L3 Cache (Level 3)**
- Largest, slowest cache level
- Shared among all cores in a cluster or die
- Size: several MB to tens of MB
- Helps with cache coherence (snooping in L3 instead of memory)
- Reduces bandwidth pressure on memory controller
- Latency: ~40-70 clock cycles

**big.LITTLE Architecture**
- ARM heterogeneous multi-core architecture
- Combines high-performance "big" cores with energy-efficient "LITTLE" cores
- Same instruction set (ISA) but different implementations
- Enables dynamic performance/energy optimization
- Used extensively in mobile devices (smartphones, tablets)

**big Cores**
- High-performance cores in big.LITTLE architecture
- Optimized for compute-intensive tasks
- Higher clock speed, deeper pipelines, more complex execution units
- More power consumption
- Examples: ARM Cortex-A7x (A72, A73, A76, A78)

**LITTLE Cores**
- Energy-efficient cores in big.LITTLE architecture
- Optimized for background tasks and low-intensity workloads
- Lower clock speed, simpler design, in-order execution
- Significantly lower power consumption
- Examples: ARM Cortex-A5x (A53, A55)

**Cluster**
- Group of cores of the same type (all big or all LITTLE)
- Cores in cluster typically share L2 or L3 cache
- OS can manage cores within cluster collectively
- Simplifies scheduling and cache coherence

**Coherent Interconnect**
- Interconnect supporting cache coherence protocols
- Ensures data consistency across all caches and memory
- Examples: ARM CoreLink CCI (Cache Coherent Interconnect)
- Critical for big.LITTLE global task scheduling

**CPU Migration (big.LITTLE Execution Model)**
- Execution model where big and LITTLE cores are paired
- Each pair appears as single logical core to OS
- Only one core of pair active at any time
- System switches between big and LITTLE based on load
- Simpler for OS, but less flexible

**Global Task Scheduling (big.LITTLE Execution Model)**
- Execution model where OS sees all cores individually
- Scheduler can use any subset of cores simultaneously
- High-priority/compute-intensive tasks → big cores
- Low-priority/light tasks → LITTLE cores
- More complex for OS, but higher flexibility and parallelism

**Dynamic Voltage and Frequency Scaling (DVFS)**
- Technique adjusting CPU voltage and clock frequency based on load
- Lower frequency/voltage for light loads → energy savings
- Higher frequency/voltage for heavy loads → performance
- Used in CPU Migration model to transition between big and LITTLE
- Common power management technique

**Task Migration**
- Moving a running task from one core to another
- In big.LITTLE: moving task between big and LITTLE cores based on load
- Requires saving/restoring task context
- Introduces overhead but enables dynamic optimization
- Types: fork migration, wake migration, forced migration, idle-pull, offload

**Tracked Load (big.LITTLE)**
- Moving average of task's computational demand
- Tracked by scheduler over time
- Used to determine whether task should run on big or LITTLE core
- Adjusted based on CPU frequency (DVFS)
- Enables automatic per-task optimization

**Up Migration**
- Moving task from LITTLE to big core
- Triggered when tracked load exceeds threshold
- Happens when task becomes compute-intensive
- Optimizes performance for demanding task

**Down Migration**
- Moving task from big to LITTLE core
- Triggered when tracked load falls below threshold
- Happens when task becomes less demanding
- Optimizes energy efficiency

**Fork Migration**
- Task migration at creation (fork system call)
- New tasks default to big core initially
- Assumption: demanding tasks benefit from starting on big core
- Light tasks quickly migrate down to LITTLE core

**Wake Migration**
- Task migration when task wakes from blocked state
- Uses tracked load history to choose big or LITTLE core
- Assumes task will resume with similar load characteristics
- Key mechanism for load-based core assignment

**Access Time / Latency**
- Time required to complete memory read or write operation
- In UMA: uniform for all CPUs
- In NUMA: varies (local fast, remote slow)
- Affected by: cache hits/misses, bus contention, interconnect hops
- Critical performance metric

**Scalability**
- Ability of architecture to support increasing number of processors
- Traditional bus: poor scalability (~4 CPUs max)
- UMA: limited by Northbridge bandwidth (~4-8 CPUs)
- NUMA: better scalability (10s-100s of CPUs possible)
- big.LITTLE: scalability within big and LITTLE clusters

**Bus Latency**
- Delay in bus communication due to signal propagation
- Increases with physical bus length (more CPUs = longer bus)
- Adds to overall memory access latency
- Contributes to non-scalability of bus-based systems

**Memory Address Space**
- Range of addressable memory locations
- In UMA: single unified address space
- In NUMA: each CPU has own address space, remote addresses mapped
- Memory mapping requires OS/software support in NUMA

**Memory Mapping**
- OS mechanism mapping virtual addresses to physical memory
- In NUMA: critical for performance (map to local memory when possible)
- OS must be NUMA-aware to optimize mappings
- Poor mapping degrades performance (excessive remote accesses)

**Cache Line**
- Smallest unit of data transferred between cache and memory
- Typical size: 64 bytes
- Cache coherence operates at cache line granularity
- False sharing: multiple CPUs accessing different data in same cache line

**Cache Hit**
- Requested data found in cache
- Fast access (few clock cycles)
- Avoids slow memory access
- High hit rate essential for performance

**Cache Miss**
- Requested data not in cache
- Must fetch from next level cache or memory
- Slow access (tens to hundreds of cycles)
- Triggers cache line fill

**Word Width**
- Number of bits transferred in single bus transaction
- Traditional systems: 32-bit or 64-bit
- Affects memory bandwidth (wider = more data per cycle)
- Bottleneck in traditional bus-based systems

**I/O (Input/Output)**
- Communication with peripheral devices
- In traditional/UMA: via system bus or Northbridge
- In NUMA: can be attached to specific CPU nodes
- I/O traffic competes with memory traffic for bandwidth

**Embedded System**
- Special-purpose computing system with specific application
- Constrained resources: power, size, cost
- Traditional architectures: single CPU, maybe GPU
- Modern embedded: multi-core (e.g., smartphone SoCs)
- NUMA less common in embedded (too complex, power-hungry)

**Ultra96 Zynq UltraScale+**
- Example multi-processor SoC used in labs
- Quad-core ARM Cortex-A53 (APU) cluster
- Dual-core ARM Cortex-R5 (RPU)
- Shared DDR controller (UMA for A53 cluster)
- On-chip memory (OCM) for R5 (NUMA-like local memory)
- Hybrid UMA + NUMA characteristics

---

## Core Concepts

### 1. Traditional Memory Access - Single CPU System

**Architecture:**
- One CPU connected to memory and I/O via system bus
- CPU is sole master (controls bus)
- Simple and straightforward design
- Foundation for understanding multi-CPU challenges

**Bottlenecks:**
1. **Single Bus**: Data and instructions share same bus → contention
2. **Memory Speed**: DRAM much slower than CPU → CPU often stalled
3. **Word Width**: Limited data transfer per cycle
4. **Computational Power**: Eventually, single CPU insufficient for workload

**Why Multiple CPUs?**
- Single CPU performance plateaued (power wall, instruction-level parallelism limits)
- Adding cores easier than making single core much faster
- Enables true parallel execution for multi-threaded workloads
- Essential for modern performance demands

### 2. Traditional Memory Access - Multiple CPU System

**Architecture:**
- Multiple CPUs sharing single system bus
- All CPUs connected to shared memory and I/O
- More computing performance through parallelism
- Entire memory space accessible to all CPUs

**Bus Arbitration Problem:**
- Multiple masters (CPUs) competing for bus access
- Need coordination mechanism (arbiter)
- Only one CPU can use bus at a time
- Other CPUs must wait → stall

**New Bottlenecks:**
1. **Shared Bus**: Bandwidth divided among CPUs
   - 4 CPUs → each gets ~25% of bus bandwidth
   - Per-CPU bandwidth decreases with more CPUs
2. **Arbitration Delay**: Time waiting for bus access
3. **Bus Length**: Physical distance increases with more CPUs
   - Signal propagation delay (speed of light limit!)
   - Longer bus = higher latency
4. **Contention**: More CPUs = more conflicts

**Scalability Limit:**
- Practical limit: approximately **4 CPU cores**
- Beyond this: arbitration overhead and bandwidth sharing make additional CPUs ineffective
- Fundamental architectural limit of shared bus

### 3. Cache Introduction and Cache Coherence Problem

**Why Caches?**
- Gap between CPU speed and memory speed growing
- Memory bandwidth limited
- Caches bridge the gap: small, fast memory close to CPU
- High hit rate makes average access time much faster

**Cache in Multi-CPU:**
- Each CPU has own cache (L1, L2)
- Reduces memory traffic (cache hits don't use bus/memory)
- Isolates CPUs somewhat (less bus contention)
- But introduces **cache coherence problem**

**Cache Coherence Problem:**
```
Initial: Memory X = 100

CPU1 reads X → X=100 cached in CPU1
CPU2 reads X → X=100 cached in CPU2

CPU1 writes X = 200 → Updates CPU1 cache and memory

Problem: CPU2's cache still has X=100! (stale data)
```

**Consequences:**
- Incorrect program behavior (reads old value)
- Race conditions, synchronization failures
- Critical for correctness of multi-threaded programs
- Must be solved in hardware (transparent to software)

**Snooping Protocol:**
- All caches monitor (snoop) bus transactions
- When CPU1 reads from memory, other CPUs see the read
- Each CPU checks if it has that address cached
- Enables cache-to-cache transfers, coherence detection

**Write Invalidation:**
- When CPU writes to memory, hardware broadcasts "invalidate" signal
- All other caches check if they have that address
- If yes: mark cache line as invalid
- Next read by other CPU will miss, reload from memory (getting new value)
- Most common coherence protocol

**Alternative: Write Update:**
- Instead of invalidating, immediately update all caches with new value
- Higher bus traffic but lower latency for next read
- Less common in practice

**Cache Coherence Overhead:**
- Snooping requires extra logic in each cache
- Broadcasts on bus increase traffic
- Invalidations reduce effective cache size (more misses)
- Becomes more complex with more CPUs
- Still active research area

### 4. Uniform Memory Access (UMA) Architecture

**Motivation:**
- Shared bus doesn't scale beyond ~4 CPUs
- Need higher bandwidth to support more processors
- Goal: uniform access time for all CPUs

**Architecture Changes:**
- Replace shared bus with **Northbridge (Memory Controller)**
- Direct connections from each CPU to Northbridge
- Multiple memory channels for higher total bandwidth
- All memories in single address space (software sees one memory)
- L2 caches in each CPU buffer accesses

**Key Features:**
- **Uniform Access Time**: Any CPU accessing any memory has same latency
- **No Arbitration**: Direct CPU-Northbridge links (not shared)
- **Higher Bandwidth**: Multiple paths, multiple memory channels
- **Scalable... to a point**: Better than shared bus

**L3 Cache Addition:**
- Shared L3 cache between Northbridge and CPUs
- Acts as additional buffer layer
- Snooping can happen in L3 instead of Northbridge
  - Faster (don't need to check memory controller)
  - Reduces Northbridge load
- Helps cache coherence scaling

**Limitations:**
- **Northbridge Bandwidth**: Still a central bottleneck
  - All memory requests funnel through Northbridge
  - Can't scale indefinitely
- **I/O Through Northbridge**: I/O traffic competes with memory traffic
- **Practical Limit**: ~4-8 CPUs depending on workload

**When UMA Works Well:**
- Moderate number of CPUs (4-8)
- Software doesn't need memory locality awareness
- Simpler programming model (no NUMA considerations)

### 5. Beyond UMA - Linking UMA Blocks

**Concept:**
- Connect multiple UMA blocks via interconnects
- Each block: small number of CPUs with local Northbridge and memory
- Interconnects enable access to remote blocks

**Properties:**
- Local memory access: fast (within UMA block)
- Remote memory access: slower (through interconnect, remote Northbridge)
- No longer uniform! Transition to NUMA

**Challenges:**
- Adds complexity (multiple memory controllers)
- Remote access latency increases with number of hops
- Cache coherence across interconnects more difficult
- Software must be aware of locality for good performance

### 6. Non-Uniform Memory Access (NUMA) Architecture

**Fundamental Change:**
- Each CPU has its own **local memory** directly attached
- No central Northbridge bottleneck
- Each CPU has integrated memory controller
- Interconnects link CPUs for remote memory access

**Memory Access:**
- **Local Access**: CPU accessing its own memory
  - Fast: direct connection to local memory controller
  - Low latency, high bandwidth
- **Remote Access**: CPU accessing another CPU's memory
  - Slower: must go through interconnect, remote memory controller
  - Higher latency, lower bandwidth
- **Non-Uniform**: Access time depends on locality

**Scalability:**
- Eliminates central bandwidth bottleneck
- Each CPU has full bandwidth to local memory
- Adding CPUs doesn't reduce per-CPU bandwidth for local accesses
- Can scale to 10s or 100s of CPUs
- "Embedded has left the building" - typically server/HPC systems

**Memory Address Space:**
- Each CPU manages own memory address space
- OS must map remote addresses to appropriate CPU/memory
- Requires **NUMA-aware software**
- Applications should allocate/access local memory when possible

**Interconnect Topologies:**
- **Bus-based**: Simple but limited bandwidth
- **Ring**: Better bandwidth, but latency grows with distance
- **Mesh/Torus**: 2D or 3D grid connections
- **Point-to-Point**: Direct connection between every pair
  - Highest bandwidth, lowest latency
  - But N² connections for N CPUs (complexity!)

**Cache Coherence in NUMA:**
- Much more challenging than UMA
- Can't snoop on a bus (no central bus!)
- Directory-based protocols:
  - Track which caches have which lines
  - Forward coherence messages over interconnect
- Or point-to-point coherence messages
- Significant research area

**NUMA Software Challenges:**
- OS must be NUMA-aware:
  - Allocate memory local to thread when possible
  - Pin threads to CPUs to maintain locality
  - Balance load across NUMA nodes
- Applications benefit from NUMA awareness:
  - Data structures partitioned by locality
  - Minimize remote memory access
- Poor software can hurt NUMA performance badly

**When NUMA Makes Sense:**
- Large number of CPUs (>8)
- Workloads with natural partitioning (e.g., separate tasks on separate data)
- Server, HPC, data center applications
- Less common in embedded (complexity, power, software challenges)

### 7. big.LITTLE Architecture Overview

**Heterogeneous Multi-Core:**
- Different types of cores in same system
- **big cores**: High performance, high power (Cortex-A7x)
- **LITTLE cores**: Low performance, low power (Cortex-A5x)
- Same instruction set (ISA) - binary compatible
- Different microarchitecture (implementation)

**Motivation:**
- Most tasks don't need maximum performance constantly
- Background tasks: email sync, messaging, sensors
  - Wasting power to run on big core
- Demanding tasks: gaming, video, heavy computation
  - Need big core performance
- Dynamic switching optimizes energy vs performance trade-off

**Organization:**
- Cores grouped into **clusters** by type
  - big cluster: multiple big cores (typically 2-4)
  - LITTLE cluster: multiple LITTLE cores (typically 2-4)
- Example configurations: 4×big + 4×LITTLE, 2×big + 4×LITTLE
- All cores connected via **coherent interconnect**
- Shared memory controller and memory

**Characteristics:**
- **Performance Difference**: big cores ~2-5x faster than LITTLE
- **Power Difference**: big cores consume ~5-10x more power at same frequency
- **Energy Efficiency**: LITTLE cores much better performance per watt for light loads
- **Flexibility**: Can run any subset of cores simultaneously (in global task scheduling)

### 8. big.LITTLE Execution Model 1: CPU Migration

**Concept:**
- Pair each big core with a LITTLE core
- OS sees each pair as single **logical core**
- Only one core of pair active at any time
- Switch dynamically based on load

**Operation:**
1. Task starts on one core (say, big)
2. System monitors load (CPU utilization, frequency)
3. If load decreases: migrate to LITTLE core (down migration)
4. If load increases: migrate to big core (up migration)
5. Uses DVFS (Dynamic Voltage and Frequency Scaling) for smooth transitions

**Migration Process:**
- Save task context from current core
- Power down current core
- Power up target core
- Restore context on target core
- Resume execution - transparent to task

**Advantages:**
- **Simple for OS**: Only sees logical cores (not aware of big/LITTLE)
- **No OS Changes**: Works with existing schedulers
- **Lower Complexity**: Pairing reduces configuration space

**Disadvantages:**
- **Less Flexible**: Can't use big and LITTLE simultaneously for one logical core
- **Coarse-Grained**: Entire core switches, not individual tasks
- **Limited Parallelism**: Only N logical cores, not 2N physical cores

**Use Cases:**
- Systems with symmetric workloads (all tasks similar priority)
- Simpler devices where software complexity must be minimal
- Backward compatibility with non-big.LITTLE-aware OSes

### 9. big.LITTLE Execution Model 2: Global Task Scheduling

**Concept:**
- OS is aware of all individual cores (big and LITTLE)
- All cores visible as separate CPUs to scheduler
- OS can use any subset of cores simultaneously
- Scheduler intelligently assigns tasks based on characteristics

**Task-to-Core Assignment:**
- **High-Priority/Compute-Intensive → big cores**
  - Heavy computation (gaming, video encoding)
  - Performance-critical tasks
  - Interactive foreground apps
- **Low-Priority/Light Tasks → LITTLE cores**
  - Background services
  - Idle loops
  - Sensor monitoring, UI responsiveness

**Tracked Load Mechanism:**
- Scheduler tracks **moving average** of each task's load
- Load = CPU time used over recent time window
- Adjusts for CPU frequency (DVFS)
- High tracked load → task needs big core
- Low tracked load → task can use LITTLE core

**Dynamic Migration:**
- Task starts on big core (fork migration)
- If load stays low: migrate down to LITTLE (wake migration, forced migration)
- If load increases: migrate up to big
- Real-time tracking and migration

**Migration Thresholds:**
```
         ^ Tracked Load
  Up     |  ─ ─ ─ ─ ─ ─ ─ ─ ─
Migration|         ▲
Threshold|       /   (migrate to big)
         | ─ ─ ─ ─ ─ ─ ─ ─ ─
  Down   |     ▼
Migration|   (migrate to LITTLE)
Threshold|
         └────────────────────> Time
```
- Hysteresis prevents ping-ponging

**Benefits:**
- **Higher Parallelism**: Can use all cores simultaneously
  - Example: 4 big + 4 LITTLE = 8 cores available
- **Flexible Configurations**: Different numbers of big and LITTLE
- **Optimized Task Placement**: Right task on right core type
- **Better Performance**: Heavy tasks don't wait for light tasks
- **Lower Latency**: Light tasks not blocked by heavy tasks
- **Interrupt Affinity**: Can direct interrupts to LITTLE for low latency

**Disadvantages:**
- **OS Complexity**: Scheduler must be big.LITTLE-aware
  - Significant kernel changes
  - More sophisticated scheduling logic
- **Software Support**: Requires Linux kernel extensions (or equivalent)
- **More Context Switches**: More migration events

**Performance Example:**
- Compute-heavy tasks on big cores (5-8)
- UI responsiveness, sensor interrupts on LITTLE cores (1-4)
- Heavy tasks: better completion time (no interference from light tasks)
- Light tasks: shorter latency (don't wait for heavy tasks)

**Use Cases:**
- Smartphones, tablets (heterogeneous workload)
- Always-on devices (background + foreground tasks)
- Complex systems where fine-grained control beneficial

### 10. Fork, Wake, and Forced Migration (Global Task Scheduling)

**Fork Migration:**
- Occurs at task creation (fork() system call)
- **Default: new task starts on big core**
- Assumption: new tasks often demanding initially
- If truly light: will quickly migrate down via wake migration
- Benefits demanding tasks without penalty

**Wake Migration:**
- Task transitions from blocked (sleeping) to ready
- Scheduler chooses core based on **tracked load history**
- If tracked load high: wake on big core
- If tracked load low: wake on LITTLE core
- Most important mechanism for task-to-core assignment

**Forced Migration:**
- Periodic re-evaluation of running tasks
- If task on wrong core type based on current load:
  - big task on LITTLE → force up migration
  - LITTLE task on big → force down migration
- Handles gradual load changes not caught by wake migration

**Idle-Pull Migration:**
- When core becomes idle, looks for task to pull from another core
- big core idle: pull high-load task from LITTLE core
- LITTLE core idle: pull low-load task from big core (less common)

**Offload Migration:**
- Proactively offload task from overloaded core
- If big cores all busy, don't let LITTLE cores sit idle
- Push lower-priority task from big to LITTLE
- Free big core for higher-priority task

**Tracked Load Calculation:**
- Moving average with time decay
- Adjusted for CPU frequency:
  ```
  If CPU running at 50% max frequency,
  task accrues tracked load at 50% rate
  ```
- Prevents DVFS from affecting migration decisions
- Normalized load metric across different core frequencies

### 11. Energy and Performance Optimization

**Energy-Performance Trade-off:**
- Maximum performance: all big cores active
  - High power consumption
  - Drains battery quickly
- Minimum energy: all LITTLE cores active
  - Lower power consumption
  - May not meet performance requirements
- big.LITTLE: dynamic middle ground

**Scenarios:**

**Scenario 1: Light Background Tasks**
- Only LITTLE cores active
- big cores powered off (zero static power)
- Excellent energy efficiency
- Example: music playback, instant messaging background sync

**Scenario 2: Burst Performance**
- Heavy task arrives (e.g., user launches game)
- Migrate to big core
- High performance for short duration
- Then migrate back to LITTLE
- Energy spent only when needed

**Scenario 3: Sustained Heavy Load**
- Continuous demanding workload (e.g., video recording)
- Keep on big cores
- Accept higher power consumption for required performance
- Still better than always-big approach

**Scenario 4: Mixed Workload**
- Some heavy tasks on big cores
- Many light tasks on LITTLE cores
- Optimal: right task on right core
- Global task scheduling shines here

**Power Savings:**
- Studies show 50-70% energy savings for typical mobile workloads
- Compared to using only big cores for everything
- Key: most tasks are light most of the time

### 12. UMA vs NUMA in Embedded: Zynq UltraScale+ Example

**Zynq UltraScale+ Architecture:**
- **Quad-core Cortex-A53** (APU cluster)
  - Shared L2 cache within cluster
  - Connected to shared DDR controller via CCI
  - **UMA within A53 cluster**: uniform access to DDR
- **Dual-core Cortex-R5** (RPU)
  - 256 KB On-Chip Memory (OCM) - local, fast
  - Also can access DDR via CCI, but slower
  - **NUMA-like**: OCM is local fast memory, DDR is remote

**Classification:**
- **Hybrid UMA + NUMA**
- UMA: A53 cluster accessing shared DDR
- NUMA: R5 local OCM vs shared DDR

**A53 Cluster: UMA Characteristics:**
- All 4 A53 cores have uniform access to DDR
- CCI (Cache Coherent Interconnect) provides coherence
- Single memory controller (DDR controller)
- Software sees one address space
- Typical UMA architecture for small core count

**R5: NUMA-like Characteristics:**
- OCM (256 KB) is local memory for R5
  - Very fast access
  - Used for real-time critical data
- DDR access via CCI and interconnect
  - Slower than OCM
  - Shared with A53 cores
- Non-uniform: OCM fast, DDR slow

**Why This Design?**
- A53 runs Linux, needs lots of memory (DDR)
  - UMA simplifies OS memory management
- R5 runs FreeRTOS, needs deterministic real-time access
  - Local OCM provides guaranteed access time
  - No contention from A53
- Hybrid design fits different processor roles

**Programming Implications:**
- A53: standard Linux programming (UMA assumption)
- R5: must explicitly place real-time data in OCM for performance
  - Linker scripts place code/data in OCM
  - Shared data with A53 goes in DDR

---

## Important Diagrams & Graphics

### Diagram 1: Traditional Memory Access - Single CPU

```
           ┌─────────┐
           │   CPU   │
           └────┬────┘
                │
    ┌───────────┼───────────┐
    │           │           │
    │         Bus           │
    │           │           │
    └───────────┼───────────┘
                │
       ┌────────┴────────┐
       │                 │
  ┌────▼────┐      ┌────▼────┐
  │ Memory  │      │   I/O   │
  └─────────┘      └─────────┘
```

**Purpose**: Shows simplest architecture with single bottleneck
**Key Points**:
- Single bus for everything (data, instructions, I/O)
- CPU is only master
- Memory speed and bus bandwidth limit performance

### Diagram 2: Traditional Memory Access - Multiple CPU

```
  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐
  │ CPU │  │ CPU │  │ CPU │  │ CPU │
  └──┬──┘  └──┬──┘  └──┬──┘  └──┬──┘
     │        │        │        │
     └────────┴────────┴────────┘
              │
    ┌─────────┼─────────┐
    │       Bus         │ (shared, arbitrated)
    └─────────┼─────────┘
              │
       ┌──────┴──────┐
       │             │
  ┌────▼────┐   ┌───▼────┐
  │ Memory  │   │  I/O   │
  └─────────┘   └────────┘
```

**Purpose**: Shows shared bus bottleneck with multiple CPUs
**Key Points**:
- All CPUs share single bus (arbitration needed)
- Bandwidth divided among CPUs
- Bus length increases with more CPUs (latency)
- Limit: ~4 CPUs

### Diagram 3: Traditional Memory Access with Caches

```
  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐
  │ CPU │  │ CPU │  │ CPU │  │ CPU │
  ├─────┤  ├─────┤  ├─────┤  ├─────┤
  │L1|L1D│  │L1|L1D│  │L1|L1D│  │L1|L1D│
  └──┬──┘  └──┬──┘  └──┬──┘  └──┬──┘
     │        │        │        │
     └────────┴────────┴────────┘
              │
    ┌─────────┼─────────┐
    │       Bus         │ (snooping for coherence)
    └─────────┼─────────┘
              │
       ┌──────┴──────┐
       │             │
  ┌────▼────┐   ┌───▼────┐
  │ Memory  │   │  I/O   │
  └─────────┘   └────────┘
```

**Purpose**: Shows cache addition and coherence challenge
**Key Points**:
- Each CPU has L1I (instruction) and L1D (data) cache
- Caches reduce bus traffic (good!)
- But multiple copies of data create coherence problem
- Snooping on bus maintains coherence

### Diagram 4: UMA Architecture (without L3)

```
  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐
  │ CPU │  │ CPU │  │ CPU │  │ CPU │
  ├─────┤  ├─────┤  ├─────┤  ├─────┤
  │L1|L1D│  │L1|L1D│  │L1|L1D│  │L1|L1D│
  ├─────┤  ├─────┤  ├─────┤  ├─────┤
  │  L2 │  │  L2 │  │  L2 │  │  L2 │
  └──┬──┘  └──┬──┘  └──┬──┘  └──┬──┘
     │        │        │        │
     │  (dedicated links, not shared)
     │        │        │        │
    ┌▼────────▼────────▼────────▼┐
    │   Memory Controller        │
    │     (Northbridge)           │
    └┬────────┬────────┬─────────┘
     │        │        │
  ┌──▼───┐ ┌─▼────┐ ┌─▼───┐
  │Memory│ │Memory│ │ I/O │
  └──────┘ └──────┘ └─────┘
```

**Purpose**: Shows UMA improvement over shared bus
**Key Points**:
- Northbridge replaces shared bus
- Dedicated CPU-to-Northbridge links
- Multiple memory channels increase bandwidth
- Still centralized (Northbridge bottleneck)

### Diagram 5: UMA Architecture with L3 Cache

```
  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐
  │ CPU │  │ CPU │  │ CPU │  │ CPU │
  ├─────┤  ├─────┤  ├─────┤  ├─────┤
  │L1|L1D│  │L1|L1D│  │L1|L1D│  │L1|L1D│
  ├─────┤  ├─────┤  ├─────┤  ├─────┤
  │  L2 │  │  L2 │  │  L2 │  │  L2 │
  └──┬──┘  └──┬──┘  └──┬──┘  └──┬──┘
     │        │        │        │
    ┌▼────────▼────────▼────────▼┐
    │      L3 Cache               │ (shared)
    └─────────┬────────────────────┘
              │
    ┌─────────▼────────────┐
    │   Memory Controller  │
    │     (Northbridge)    │
    └┬────────┬────────────┘
     │        │
  ┌──▼───┐ ┌─▼────┐
  │Memory│ │ I/O  │
  └──────┘ └──────┘
```

**Purpose**: Shows L3 cache improving UMA
**Key Points**:
- L3 shared among all CPUs
- Acts as buffer between L2 and Northbridge
- Snooping in L3 reduces Northbridge load
- But bandwidth bottleneck remains

### Diagram 6: NUMA Architecture

```
 ┌────────────┐          ┌────────────┐
 │    CPU     │          │    CPU     │
 │  + Memory  │◄────────►│  + Memory  │
 │ Controller │          │ Controller │
 └──────┬─────┘          └─────┬──────┘
        │                      │
   ┌────▼────┐            ┌───▼─────┐
   │ Memory  │            │ Memory  │
   │ (local) │            │ (local) │
   └─────────┘            └─────────┘
     (fast)                 (slow for
                            other CPU)

  Local access: fast (direct)
  Remote access: slow (via interconnect)
```

**Purpose**: Shows NUMA distributed memory
**Key Points**:
- Each CPU has own local memory
- Integrated memory controller per CPU
- Interconnect for remote access
- Non-uniform: local fast, remote slow

### Diagram 7: NUMA with Point-to-Point Interconnect

```
  ┌───────┐     ┌───────┐
  │CPU+Mem│◄───►│CPU+Mem│
  │   1   │  ╲ ╱│   2   │
  └───┬───┘   ╳ └───┬───┘
      │      ╱ ╲    │
      │     ╱   ╲   │
      │    ╱     ╲  │
  ┌───▼───┐     ┌──▼────┐
  │CPU+Mem│◄───►│CPU+Mem│
  │   3   │     │   4   │
  └───────┘     └───────┘

  Point-to-point: direct connection
  between every pair of CPUs
  (4 CPUs → 6 links)
```

**Purpose**: Shows full-mesh interconnect
**Key Points**:
- Direct links between all CPU pairs
- No multi-hop latency
- High bandwidth per link
- Complexity: N² links for N CPUs

### Diagram 8: big.LITTLE Architecture

```
     ┌──────────────────┐
     │Interrupt Controller│
     └─────┬──────┬─────┘
           │      │
      ┌────▼──┐ ┌▼─────┐     ┌────────────┐
      │ big   │ │LITTLE│     │Rest of     │
      │cluster│ │cluster│     │System      │
      │(2-4cores│(2-4cores)    │(GPU,Video, │
      └───┬───┘ └──┬────┘     │Display,etc)│
          │        │          └──────┬─────┘
          └────┬───┴──────────┬──────┘
               │              │
        ┌──────▼──────────────▼──────┐
        │  Coherent Interconnect     │
        └──────────┬──────────────────┘
                   │
        ┌──────────▼──────────┐
        │   Memory Controller │
        └──────────┬──────────┘
                   │
             ┌─────▼─────┐
             │    DDR    │
             └───────────┘
```

**Purpose**: Shows big.LITTLE heterogeneous architecture
**Key Points**:
- Separate big and LITTLE clusters
- Both connected to coherent interconnect
- Shared memory controller and memory
- Cache coherence maintained across clusters

### Diagram 9: CPU Migration Execution Model

```
  Logical    Physical Cores
   Core       (paired)

   LC1    ┌──────────┐
          │  1 (big) │
          ├──────────┤ Switch
          │1'(LITTLE)│ based on
          └──────────┘  load

   LC2    ┌──────────┐
          │  2 (big) │
          ├──────────┤
          │2'(LITTLE)│
          └──────────┘

   LC3    ┌──────────┐
          │  3 (big) │
          ├──────────┤
          │3'(LITTLE)│
          └──────────┘

   LC4    ┌──────────┐
          │  4 (big) │
          ├──────────┤
          │4'(LITTLE)│
          └──────────┘

  OS sees: 4 logical cores
  Only one per pair active
```

**Purpose**: Shows CPU migration pairing model
**Key Points**:
- big and LITTLE cores paired
- OS sees pairs as single logical cores
- Dynamic switching between big and LITTLE
- Only one active per pair at any time

### Diagram 10: Global Task Scheduling Model

```
       Scheduler
          │
    ┌─────┴──────────────┐
    │                    │
    ▼                    ▼
 ┌───────┐          ┌────────┐
 │  1 (L)│          │ 5 (B) │
 │  2 (L)│  Light   │ 6 (B) │ Heavy
 │  3 (L)│  Tasks   │ 7 (B) │ Tasks
 │  4 (L)│          │ 8 (B) │
 └───────┘          └────────┘
  LITTLE              big
  Cluster           Cluster

 All 8 cores visible and usable
```

**Purpose**: Shows global task scheduling flexibility
**Key Points**:
- OS sees all cores individually
- Any subset can be active simultaneously
- Light tasks on LITTLE, heavy on big
- Maximum parallelism and flexibility

### Diagram 11: Tracked Load and Migration Thresholds

```
Tracked
 Load
  ▲
  │    ┌──── Up Migration Threshold
  │    │
  │  ┌─┴─┐
  │  │   └──┐       Migration
  │  │      └───┐      Up ↑
  │  │          └─┐
  │ ─┼─────────────┼──────
  │  │              ╲
  │  │               ╲
  │  │                ╲─┐  Migration
  │  └─── Down          └───┐ Down ↓
  │       Migration          │
  │       Threshold        ┌─┘
  │                        │
  └────────────────────────┴────► Time

    B = big core
    L = LITTLE core
```

**Purpose**: Visualizes dynamic migration based on load
**Key Points**:
- Moving average of task load tracked
- Exceeds up threshold → migrate to big
- Falls below down threshold → migrate to LITTLE
- Hysteresis prevents thrashing

### Diagram 12: Zynq UltraScale+ UMA+NUMA Hybrid

```
  ┌────────┐ ┌────────┐
  │R5 │R5 │ │R5 │R5 │
  │L1I│L1D│ │L1I│L1D│
  └┬───┴┬──┘ └┬───┴┬──┘
   └────┴─────┴────┘ (RPU)
        │
    ┌───▼──────┐
    │   OCM    │ (256 KB, local, fast)
    │(On-Chip  │
    │ Memory)  │
    └──────────┘

  ┌──┬──┐ ┌──┬──┐ ┌──┬──┐ ┌──┬──┐
  │A53│  │ │A53│  │ │A53│  │ │A53│  │
  │L1I│L1D│ │L1I│L1D│ │L1I│L1D│ │L1I│L1D│
  └──┴──┘ └──┴──┘ └──┴──┘ └──┴──┘
     └──────┬──────┬──────┘
            │      (APU)
        ┌───▼─────┐
        │   L2    │ (shared in cluster)
        └────┬────┘
             │
        ┌────▼────────┐
        │    CCI      │ (Cache Coherent
        │(Interconnect)│  Interconnect)
        └──────┬──────┘
               │
        ┌──────▼───────────┐
        │  DDR Controller  │
        └──────┬───────────┘
               │
        ┌──────▼──────┐
        │DDR3, DDR4,  │ (shared, slower
        │  LPDDR4     │  for R5)
        └─────────────┘

UMA: A53 cluster → DDR (uniform)
NUMA-like: R5 OCM (local) vs DDR (remote)
```

**Purpose**: Shows real-world hybrid UMA+NUMA in embedded SoC
**Key Points**:
- A53 cluster: UMA to DDR via CCI
- R5: local OCM (fast) + shared DDR (slower)
- Hybrid design for different processor roles

---

## Formulas & Procedures

### Formula 1: Per-CPU Bandwidth in Shared Bus System

**BW_per_CPU = BW_total / N**

Where:
- BW_per_CPU = bandwidth available to each CPU
- BW_total = total bus bandwidth
- N = number of CPUs

**Interpretation:**
- Adding CPUs decreases per-CPU bandwidth linearly
- Example: 100 GB/s bus, 4 CPUs → 25 GB/s per CPU
- Example: 100 GB/s bus, 8 CPUs → 12.5 GB/s per CPU
- Fundamental scalability problem

### Formula 2: Bus Latency Due to Signal Propagation

**Latency_prop = Distance / (0.5 × c)**

Where:
- Distance = physical length of bus (m)
- c = speed of light = 3×10⁸ m/s
- 0.5 factor accounts for PCB traces (slower than vacuum)

**Example:**
- Bus length = 0.3 m (30 cm)
- Latency = 0.3 / (0.5 × 3×10⁸) = 2 ns
- Seems small, but at 3 GHz: 6 clock cycles!
- Adds to every memory access

### Formula 3: Cache Hit Rate and Effective Access Time

**T_effective = T_cache × HitRate + T_memory × (1 - HitRate)**

Where:
- T_effective = average memory access time
- T_cache = cache access time (e.g., 1 ns)
- T_memory = main memory access time (e.g., 100 ns)
- HitRate = fraction of accesses hitting cache (e.g., 0.95)

**Example:**
- Cache: 1 ns, Memory: 100 ns, Hit Rate: 95%
- T_eff = 1×0.95 + 100×0.05 = 0.95 + 5 = 5.95 ns
- ~6x faster than always accessing memory!
- High hit rate critical for performance

### Formula 4: NUMA Access Time

**T_NUMA = T_local × P_local + T_remote × P_remote**

Where:
- T_NUMA = average access time in NUMA system
- T_local = local memory access time
- T_remote = remote memory access time (usually 2-3× T_local)
- P_local = probability of local access
- P_remote = probability of remote access (= 1 - P_local)

**Example:**
- Local: 50 ns, Remote: 150 ns
- Good locality (90% local): T = 50×0.9 + 150×0.1 = 45 + 15 = 60 ns
- Poor locality (50% local): T = 50×0.5 + 150×0.5 = 25 + 75 = 100 ns
- Demonstrates importance of memory locality in NUMA

### Procedure 1: Determining if System is UMA or NUMA

**Step 1: Identify Memory Topology**
- How many memory controllers?
  - One central controller → likely UMA
  - Multiple controllers (one per CPU/node) → likely NUMA
- Where is memory attached?
  - All memory attached to central controller → UMA
  - Each CPU has local memory → NUMA

**Step 2: Check Access Time Uniformity**
- Do all CPUs have same latency to all memory?
  - Yes → UMA
  - No (local fast, remote slow) → NUMA

**Step 3: Software Hints**
- Does OS expose NUMA topology (e.g., `numactl` on Linux)?
  - Yes → NUMA
  - No → likely UMA or system too small for NUMA

**Example: Zynq UltraScale+**
- A53 cluster: one DDR controller, all A53s uniform access → UMA
- R5: has OCM (local) + DDR (shared) → NUMA characteristics
- Answer: **Hybrid UMA + NUMA**

### Procedure 2: Cache Coherence via Write Invalidation

**On Write to Address X by CPU1:**
1. CPU1 writes new value to cache
2. CPU1 broadcasts write on bus/interconnect
3. All other CPUs snoop the write
4. Each CPU checks if X is in its cache
5. If yes: mark cache line containing X as **invalid**
6. CPU1 writes through to memory (or marks modified)

**On Read of Address X by CPU2 (after invalidation):**
1. CPU2 attempts to read X from cache
2. Cache line marked invalid → **cache miss**
3. CPU2 fetches X from memory (or from CPU1's cache if modified)
4. CPU2 gets updated value
5. Coherence maintained!

**Result**: Coherence maintained automatically in hardware, transparent to software

### Procedure 3: big.LITTLE Global Task Scheduling Decision

**At Task Creation (Fork Migration):**
1. New task created via fork()
2. Default: assign to **big core**
3. Assumption: new tasks often initially demanding
4. If actually light: will migrate down quickly

**At Task Wake (Wake Migration):**
1. Task transitions from blocked to ready
2. Scheduler examines task's **tracked load**
3. Compare tracked load to thresholds:
   - If tracked_load > up_threshold: assign to **big core**
   - If tracked_load < down_threshold: assign to **LITTLE core**
4. Place task in appropriate core's run queue

**During Execution (Forced Migration):**
1. Periodically (e.g., every 100 ms), scheduler evaluates running tasks
2. For each task:
   - Check current core type (big or LITTLE)
   - Check current tracked load
3. If mismatch (heavy task on LITTLE, or light task on big):
   - Migrate task to appropriate core type
4. Update tracked load continuously

**Tracked Load Update:**
```
tracked_load_new = α × actual_load + (1-α) × tracked_load_old

Where:
- α = decay factor (e.g., 0.2)
- actual_load = recent CPU usage
- Adjusted for CPU frequency (DVFS)
```

### Procedure 4: Cache Coherence with Snooping

**Setup:**
- All caches connected to shared bus or interconnect
- Each cache has snooping logic monitoring bus

**On Every Bus Transaction:**
1. CPU initiates memory read/write
2. Transaction broadcast on bus
3. **All other caches snoop** the transaction
4. Each cache checks if address is cached locally
5. If yes:
   - **On read**: no action (read doesn't change data)
   - **On write**: invalidate local copy (or update with new value)
6. Memory controller also sees transaction

**Snoop Responses:**
- **Cache hit in another cache**: May supply data directly (cache-to-cache transfer)
- **Modified data in another cache**: That cache must supply data (not memory)
- **No hit**: Memory supplies data

**Result:**
- Maintains coherence across all caches
- Automatic, transparent to software
- Overhead: bus traffic, snoop logic complexity

---

## Comparisons & Contrasts

### Comparison 1: Single CPU vs Multi-CPU Traditional Architecture

| Aspect | Single CPU | Multi-CPU (Shared Bus) |
|--------|------------|----------------------|
| **Computing Power** | Limited to one CPU | N× CPUs = N× potential performance |
| **Bus Contention** | None (single master) | High (multiple masters) |
| **Arbitration** | Not needed | Required (adds latency) |
| **Memory Bandwidth** | Full bandwidth for CPU | Divided among N CPUs |
| **Complexity** | Simple | Moderate |
| **Scalability** | N/A | Poor (~4 CPUs max) |
| **Cache Coherence** | Not an issue | Major challenge |

**Key Insight**: Multi-CPU adds parallelism but introduces bandwidth and coherence challenges.

### Comparison 2: Shared Bus vs UMA (Northbridge)

| Aspect | Shared Bus | UMA (Northbridge) |
|--------|------------|-------------------|
| **Topology** | Single shared bus | Star (CPUs to Northbridge) |
| **Arbitration** | Required | Not needed (dedicated links) |
| **Bandwidth per CPU** | Total / N | Higher (dedicated link) |
| **Access Time** | Variable (arbitration delay) | Uniform for all CPUs |
| **Scalability** | ~4 CPUs | ~4-8 CPUs |
| **Bus Length** | Increases with CPUs | Fixed per-CPU link |
| **Bottleneck** | Shared bus | Northbridge bandwidth |
| **Complexity** | Simple | Moderate |

**Key Insight**: UMA improves over shared bus but Northbridge becomes new bottleneck.

### Comparison 3: UMA vs NUMA

| Aspect | UMA | NUMA |
|--------|-----|------|
| **Memory Access Time** | Uniform for all CPUs | Non-uniform (local vs remote) |
| **Memory Location** | Centralized | Distributed (local per CPU) |
| **Memory Controller** | Single (Northbridge) | Multiple (one per CPU) |
| **Scalability** | Limited (~4-8 CPUs) | Good (10s-100s CPUs) |
| **Bandwidth Bottleneck** | Central controller | None for local access |
| **Programming Model** | Simple (one memory) | Complex (locality awareness) |
| **Cache Coherence** | Snooping on bus | Directory-based/complex |
| **Software Support** | Standard | NUMA-aware OS required |
| **Use Case** | Small multi-core | Large server, HPC |
| **Typical in Embedded?** | Yes (small core counts) | Rare (too complex) |

**Key Insight**: NUMA scales better but at cost of programming complexity.

### Comparison 4: Cache Coherence - Snooping vs Directory-Based

| Aspect | Snooping (UMA) | Directory-Based (NUMA) |
|--------|---------------|----------------------|
| **Where Used** | Shared bus, UMA systems | NUMA, large-scale systems |
| **How It Works** | Broadcast on bus, all caches check | Central directory tracks cache states |
| **Scalability** | Poor (broadcast to all) | Better (point-to-point messages) |
| **Bus Traffic** | High (all snoop every transaction) | Lower (only relevant nodes contacted) |
| **Complexity** | Moderate | High |
| **Latency** | Low (broadcast fast on bus) | Higher (directory lookup + messages) |
| **Typical System Size** | <8 CPUs | >8 CPUs |

**Key Insight**: Snooping simpler but doesn't scale; directory-based required for large systems.

### Comparison 5: big vs LITTLE Cores

| Aspect | big Cores | LITTLE Cores |
|--------|-----------|--------------|
| **Performance** | High | Low |
| **Clock Speed** | Higher (e.g., 2.0-3.0 GHz) | Lower (e.g., 1.0-1.8 GHz) |
| **Power Consumption** | High (5-10× LITTLE) | Low |
| **Energy Efficiency** | Lower (but fast) | Higher (but slow) |
| **Pipeline** | Deep, out-of-order | Shallow, in-order |
| **Area (Die Size)** | Larger | Smaller |
| **Use Case** | High-intensity tasks | Background, light tasks |
| **Examples** | Cortex-A7x (A72, A76, A78) | Cortex-A5x (A53, A55) |
| **Typical Count** | 2-4 in smartphone SoC | 2-6 in smartphone SoC |

**Key Insight**: big cores for performance, LITTLE for efficiency - complementary roles.

### Comparison 6: CPU Migration vs Global Task Scheduling

| Aspect | CPU Migration | Global Task Scheduling |
|--------|--------------|----------------------|
| **Visibility to OS** | Pairs as logical cores | All cores individually visible |
| **Active Cores** | One per pair max | Any subset simultaneously |
| **Parallelism** | Limited (N logical cores) | Maximum (2N physical cores) |
| **Core Usage** | Exclusive (big OR LITTLE) | Simultaneous (big AND LITTLE) |
| **Flexibility** | Lower | Higher |
| **OS Complexity** | Simpler (no changes needed) | Higher (big.LITTLE-aware scheduler) |
| **Software Support** | Works with existing OS | Requires modified OS |
| **Migration** | Entire logical core switches | Individual tasks migrate |
| **Use Case** | Simpler devices, backward compat | Smartphones, complex workloads |
| **Performance** | Good | Better (more parallelism) |

**Key Insight**: CPU Migration simpler but Global Task Scheduling more powerful.

### Comparison 7: big.LITTLE vs Homogeneous Multi-Core

| Aspect | big.LITTLE (Heterogeneous) | Homogeneous Multi-Core |
|--------|---------------------------|----------------------|
| **Core Types** | 2 (big + LITTLE) | 1 (all same) |
| **Performance Range** | Wide (energy-efficient to high-perf) | Fixed |
| **Energy Efficiency** | Excellent (choose right core for task) | Good (DVFS only) |
| **Scheduling Complexity** | High (task-to-core assignment) | Low (any core is same) |
| **Peak Performance** | Limited by big core count | All cores same performance |
| **Peak Efficiency** | Excellent (LITTLE cores) | Moderate |
| **Dynamic Range** | Very wide (big vs LITTLE) | Narrow (DVFS range) |
| **Use Case** | Mobile (battery-constrained) | Server, desktop |
| **Example** | ARM SoCs in phones | Intel Core i7, AMD Ryzen |

**Key Insight**: big.LITTLE optimizes energy-performance trade-off better than homogeneous.

### Comparison 8: L1, L2, and L3 Caches

| Aspect | L1 Cache | L2 Cache | L3 Cache |
|--------|----------|----------|----------|
| **Size** | Smallest (16-64 KB) | Medium (256 KB - 1 MB) | Largest (2-64 MB) |
| **Speed** | Fastest (3-5 cycles) | Medium (~12-20 cycles) | Slowest (~40-70 cycles) |
| **Location** | In CPU core | In CPU core | Shared among cores |
| **Sharing** | Private per core | Usually private | Shared (within cluster) |
| **Split** | Often (I-cache, D-cache) | Unified | Unified |
| **Purpose** | Minimize CPU stalls | Buffer to L3/memory | Reduce memory traffic |
| **Coherence** | Must be maintained | Must be maintained | Snooping happens here (UMA) |

**Key Insight**: Hierarchy trades size for speed; L3 helps coherence and memory bandwidth.

### Comparison 9: Local vs Remote Memory Access (NUMA)

| Aspect | Local Memory | Remote Memory |
|--------|-------------|--------------|
| **Location** | Same CPU/node | Different CPU/node |
| **Access Path** | Direct to local controller | Via interconnect + remote controller |
| **Latency** | Low (e.g., 50 ns) | High (e.g., 150 ns, 2-3× local) |
| **Bandwidth** | High (dedicated) | Lower (shared interconnect) |
| **Hops** | 0 (direct) | 1+ (depends on topology) |
| **OS Consideration** | Prefer allocating here | Avoid if possible |
| **Performance** | Best | Degraded |

**Key Insight**: NUMA performance critically depends on memory locality - software must be aware.

### Comparison 10: Embedded Multi-Core vs Server/HPC Multi-Core

| Aspect | Embedded | Server/HPC |
|--------|----------|-----------|
| **Core Count** | 2-8 typically | 10s to 100s |
| **Architecture** | UMA (small), big.LITTLE | NUMA (large scale) |
| **Memory Model** | Usually UMA | Usually NUMA |
| **Power Budget** | Tight (battery, thermal) | Generous (plugged in, cooling) |
| **Complexity** | Simpler (cost, development) | Complex (performance matters most) |
| **Heterogeneity** | Common (big.LITTLE, accelerators) | Less common (mostly homogeneous) |
| **OS** | May be RTOS or lightweight Linux | Full-featured Linux, specialized OS |
| **Use Case** | Smartphones, IoT, automotive | Data centers, supercomputers |

**Key Insight**: Embedded optimizes for power and simplicity; server for raw performance and scale.

---

## Examples & Applications

### Example 1: Cache Coherence Violation (Without Coherence Protocol)

**Scenario: Simple Counter Without Coherence**

```
Initial state: counter = 0 in memory

CPU1:                    CPU2:
1. Read counter (0)
   → CPU1 cache: 0
2. Increment: 0+1=1
3. Write 1 to cache
   → CPU1 cache: 1
   → Memory: 1
                         4. Read counter
                            → CPU2 cache: 0 (STALE!)
                         5. Increment: 0+1=1
                         6. Write 1 to cache
                            → CPU2 cache: 1
                            → Memory: 1

Expected: counter = 2
Actual: counter = 1 (WRONG!)
```

**With Write Invalidation:**
```
Initial state: counter = 0

CPU1:                    CPU2:
1. Read counter (0)      2. Read counter (0)
   → CPU1 cache: 0          → CPU2 cache: 0
3. Write 1 to cache
   → Broadcast invalidate
                         → CPU2 cache: INVALID
   → Memory: 1
                         4. Read counter
                            → Cache miss (invalid)
                            → Fetch from memory: 1
                            → CPU2 cache: 1
                         5. Increment: 1+1=2
                         6. Write 2
                            → Memory: 2

Result: counter = 2 (CORRECT!)
```

### Example 2: Calculating Northbridge Bandwidth Requirement (UMA)

**System Specification:**
- 4 CPUs
- Each CPU: 3 GHz, 64-bit data path
- Each CPU can request memory every 5 cycles (average)
- Memory access: 64 bytes per request

**CPU Memory Bandwidth:**
- Frequency: 3 GHz = 3 × 10⁹ cycles/s
- Requests per CPU: 3×10⁹ / 5 = 6×10⁸ requests/s
- Bandwidth per CPU: 6×10⁸ × 64 bytes = 38.4 GB/s

**Total System Bandwidth Needed:**
- 4 CPUs × 38.4 GB/s = 153.6 GB/s

**Northbridge Requirements:**
- Must provide **at least 153.6 GB/s** to memory
- Typical: 2-4× this for burst capacity
- Modern: Dual-channel DDR4-3200 = ~50 GB/s (insufficient for 4 CPUs!)
- Need: Quad-channel DDR4 or higher bandwidth memory

**Conclusion**: Northbridge bandwidth is real bottleneck, limits scalability.

### Example 3: NUMA Locality Impact on Performance

**Application: Matrix Multiplication (A × B = C)**

**Scenario 1: Poor Locality (All Remote Access)**
- Task on CPU1
- Matrices A, B, C allocated on CPU2's memory
- Every memory access: remote (150 ns)
- 1 billion accesses × 150 ns = 150 seconds

**Scenario 2: Good Locality (Local Access)**
- Task on CPU1
- Matrices A, B, C allocated on CPU1's local memory
- Every memory access: local (50 ns)
- 1 billion accesses × 50 ns = 50 seconds

**Speedup: 3× just from memory locality!**

**Best Practice:**
- Use `numactl` to bind task to CPU and allocate memory locally
- Example: `numactl --cpunodebind=0 --membind=0 ./matrix_multiply`

### Example 4: big.LITTLE Task Assignment

**Smartphone Workload:**

**Scenario: User Playing Game**

**Tasks:**
1. **Game Rendering** (high priority, compute-intensive)
   - Tracked load: high (consistently high CPU usage)
   - Assignment: **big core** (need maximum performance)

2. **Physics Engine** (high priority, episodic)
   - Tracked load: varies (high during explosions, low otherwise)
   - Assignment: **big core** when high, migrates to LITTLE when low

3. **Background Music** (low priority, steady light load)
   - Tracked load: low (minimal CPU, mostly waiting on audio hardware)
   - Assignment: **LITTLE core** (efficient, low power)

4. **Email Sync** (low priority, intermittent)
   - Tracked load: low (network I/O bound)
   - Assignment: **LITTLE core**

5. **Touch Input Handler** (high priority, low compute)
   - Tracked load: low (interrupt-driven, little processing)
   - Assignment: **LITTLE core** (low latency, efficient)

**Core Allocation:**
- big cores: Game Rendering, Physics (when active)
- LITTLE cores: Music, Email, Touch Input, Physics (when idle)

**Result:**
- Maximum game performance (big cores dedicated)
- Low power for background tasks (LITTLE cores efficient)
- Optimal battery life for given performance level

### Example 5: CPU Migration in Action

**Scenario: Web Browsing**

**Initial State:**
- User idle, browsing static page
- Browser task: low load (just rendering static content)
- Current: **LITTLE core** (low power)

**Event: User Clicks Link to Video**
1. Video loading starts
2. Tracked load increases (parsing, decoding preparation)
3. Exceeds **up migration threshold**
4. System migrates task to **big core**
5. Video decoding and rendering: high load (stays on big core)
6. Smooth playback

**After Video Ends:**
1. User returns to static page
2. Tracked load decreases
3. Falls below **down migration threshold**
4. System migrates task to **LITTLE core**
5. Low power while idle

**Benefits:**
- High performance when needed (video playback)
- Low power when idle (browsing static pages)
- Automatic, transparent to user

### Example 6: Ultra96 Memory Access Analysis

**Analyzing Zynq UltraScale+ Architecture:**

**A53 Cluster:**
- 4× Cortex-A53 cores
- All connected to DDR via CCI (Cache Coherent Interconnect)
- Shared L2 cache within cluster
- **UMA**: All A53 cores have uniform access time to DDR
- Access time: ~100-200 ns (DDR latency)

**R5 Processors:**
- 2× Cortex-R5 cores
- 256 KB On-Chip Memory (OCM) - local, tightly coupled
- Also connected to DDR via interconnect (can access but slower)
- **NUMA-like**:
  - Local OCM access: ~10-20 ns (very fast)
  - Remote DDR access: ~100-200 ns (same as A53)
  - Ratio: DDR is 5-10× slower than OCM

**Performance Implications:**
- R5 real-time code: must use OCM for critical data
- R5 communication with A53: use shared DDR
- A53 doesn't need special consideration (UMA)

**Answer to Exercise:**
- **A53 cluster: UMA**
- **R5: NUMA-like**
- **Overall: Hybrid UMA + NUMA**

### Example 7: Tracked Load Calculation

**Task Execution:**

```
Time    CPU Usage   Frequency   Adjusted Usage   Tracked Load
0       80%         100% (max)  80%              -
1       80%         100%        80%              80% (initial)
2       60%         100%        60%              0.2×60 + 0.8×80 = 76%
3       60%         50% (DVFS)  30% (adjusted)   0.2×30 + 0.8×76 = 66.8%
4       90%         100% (DVFS↑) 90%            0.2×90 + 0.8×66.8 = 71.4%
5       90%         100%        90%              0.2×90 + 0.8×71.4 = 75.1%
```

**Explanation:**
- Time 0-2: High usage at full frequency → tracked load high
- Time 3: DVFS lowers frequency to save power
  - Actual CPU usage still 60%, but at 50% frequency
  - Adjusted: 60% × 50% = 30% (equivalent full-speed usage)
  - Prevents DVFS from triggering migration
- Time 4-5: Load increases, DVFS raises frequency
  - Tracked load rises appropriately

**Migration Decision:**
- Thresholds: Up = 85%, Down = 40%
- Tracked load 71-76%: stays on current core (between thresholds)
- If tracked load exceeds 85%: migrate to big core
- If tracked load falls below 40%: migrate to LITTLE core

---

## Connections to Other Topics

### Connection 1: Lecture 13 - RTOS Scheduling

**Multi-Core Scheduling:**
- Lecture 13: scheduling on single-core or small multi-core
- Lecture 14: architectural foundations enabling multi-core scheduling
- Global Task Scheduling (big.LITTLE) = advanced scheduling leveraging heterogeneous cores
- RTOS must be aware of core types for optimal task placement

**Task Affinity:**
- RTOS can pin tasks to specific CPUs
- Important in NUMA: pin task to CPU with local memory
- Important in big.LITTLE: keep demanding task on big core

**Real-Time Implications:**
- Shared bus arbitration introduces non-determinism (bad for RT)
- NUMA remote access has variable latency (bad for hard RT)
- Local memory (OCM in R5) provides deterministic access (good for RT)

### Connection 2: Lecture 08 - Energy

**Power Consumption:**
- Single CPU: power linear with frequency and voltage
- Multi-core: can power off unused cores (more energy efficient)
- big.LITTLE: major energy savings (light tasks on LITTLE cores)

**Dynamic Power Management:**
- DVFS from Lecture 8 used in big.LITTLE CPU Migration
- Combining heterogeneous cores + DVFS = maximum flexibility

**Energy-Performance Trade-offs:**
- More cores: higher peak performance, higher peak power
- big.LITTLE: better trade-off than homogeneous multi-core

### Connection 3: Lecture 09 - Performance

**Parallelism:**
- Multi-core enables thread-level parallelism
- Speedup limited by Amdahl's Law (sequential portions)
- Cache coherence overhead reduces parallel efficiency

**Memory Bottleneck:**
- Memory bandwidth from Lecture 9 is fundamental bottleneck in Lecture 14
- Traditional bus: severe bottleneck
- UMA: Northbridge bottleneck
- NUMA: eliminates central bottleneck

**Performance Scaling:**
- Ideal: N cores = N× performance
- Reality: shared resources (memory, bus, caches) limit scaling
- NUMA scales better for memory-intensive workloads

### Connection 4: Lab P3 - AES Multi-Processor

**Zynq UltraScale+ Architecture:**
- P3 uses A53 (APU) and R5 (RPU) from same chip
- Lecture 14 explains architectural details of that chip
- A53 cluster: UMA architecture
- R5: NUMA-like (OCM local, DDR remote)

**Communication Overhead:**
- RPMsg overhead in P3 partly due to memory access patterns
- R5 using shared DDR (remote) adds latency
- Understanding from Lecture 14 explains P3 performance

### Connection 5: Lab P2 - Energy Measurement

**Multi-Core Energy:**
- P2 measured single core energy
- Multi-core: energy = per-core energy × number of active cores
- big.LITTLE: LITTLE cores consume much less energy
- Practical: 4× LITTLE cores may consume less than 1× big core!

### Connection 6: Lecture 11/12 - Design Space Exploration

**Architectural Design Space:**
- Single CPU vs Multi-CPU vs Heterogeneous: design choices
- Trade-offs: performance, energy, cost, complexity
- big.LITTLE: point in design space optimizing energy-performance

**DSE Application:**
- Could use DSE to choose:
  - Number of cores (2, 4, 8?)
  - Homogeneous vs heterogeneous
  - UMA vs NUMA
- Objectives: performance, energy, cost, scalability

### Connection 7: Lecture 10 - FPGA

**Heterogeneous Systems:**
- big.LITTLE: CPU heterogeneity (big vs LITTLE cores)
- FPGA: another form of heterogeneity (CPU + FPGA fabric)
- Zynq SoCs: combine multi-core CPU + FPGA + dedicated blocks
- All connected via coherent interconnect

**Memory Access:**
- FPGA can access DDR via AXI interconnect
- Similar to NUMA: FPGA has local memory (BRAM), can access remote (DDR)

### Connection 8: Lab P5 - Design Space Exploration

**Multi-Processor Option:**
- P5 compared 4 implementations including A53 and R5 (two different cores)
- Could add: "4× A53" vs "2× A53 + 2× R5" as design points
- Trade-off: homogeneous (simpler) vs heterogeneous (flexibility)

**Multi-Core DSE:**
- Lecture 14 architectural concepts inform DSE decisions
- More cores: higher performance potential, higher complexity/cost

### Connection 9: Lecture 01 - Embedded Systems

**Embedded Multi-Core Trends:**
- Modern embedded systems increasingly multi-core
- Smartphones: 8-core big.LITTLE standard
- Automotive: multi-core for safety and performance
- IoT: often single-core (simplicity, cost)

**Embedded Constraints:**
- Power: big.LITTLE addresses power constraint
- Cost: UMA simpler/cheaper than NUMA for small systems
- Complexity: embedded avoids NUMA (too complex)

### Connection 10: Lecture 06 - Non-Functional Requirements

**Performance NFR:**
- Multi-core can meet higher performance NFRs
- Scalability: NUMA meets very high performance NFRs

**Energy NFR:**
- big.LITTLE meets energy efficiency NFRs better than homogeneous

**Real-Time NFR:**
- Cache coherence and bus arbitration threaten RT guarantees
- Local memory (NUMA, OCM) improves RT predictability

### Connection 11: Lecture 02 - Software Paradigms

**Parallel Programming:**
- Multi-core requires parallel programming paradigms
- Thread-based, message-passing, data-parallel
- Cache coherence enables shared-memory threading

**Challenges:**
- Race conditions amplified in multi-core
- Synchronization (locks, semaphores) necessary
- NUMA awareness complicates programming

### Connection 12: Lecture 05 - Development Processes

**Multi-Core Development:**
- Architecture selection (UMA vs NUMA, homogeneous vs heterogeneous) in design phase
- Parallel software development more complex (concurrency bugs)
- Testing more challenging (race conditions, timing-dependent bugs)

**Verification:**
- Must verify cache coherence correctness
- Must verify schedulability on multi-core (more complex than single-core)

### Connection 13: Computer Architecture (General Knowledge)

**Cache Hierarchy:**
- L1, L2, L3 caches from computer architecture
- Multi-core: private L1/L2, shared L3 common design
- Coherence protocols from architecture

**Interconnects:**
- Bus, crossbar, mesh, point-to-point from architecture
- Trade-offs: latency, bandwidth, complexity, cost

**Memory Hierarchy:**
- Registers → Caches → Main Memory → Disk
- Multi-core adds complexity (coherence, bandwidth sharing)

---

## Summary

**Multi-processor systems** evolved from simple single-CPU architectures to complex multi-core designs to meet increasing performance demands. This lecture explored three major architectural paradigms and their trade-offs.

**Traditional Memory Access** with shared bus connects multiple CPUs to single memory, but suffers from fundamental **bandwidth bottleneck** and **scalability limitations** (~4 CPUs max). The bus must be arbitrated among CPUs, introducing latency and reducing per-CPU bandwidth. Adding caches helps performance but creates the **cache coherence problem** requiring solutions like snooping and write invalidation.

**Uniform Memory Access (UMA)** replaces the shared bus with a central **Northbridge (memory controller)** connected to CPUs via dedicated links, providing uniform access time for all CPUs. Additional L3 caching improves coherence and bandwidth. However, the Northbridge itself becomes a bottleneck, limiting scalability to ~4-8 CPUs.

**Non-Uniform Memory Access (NUMA)** distributes memory with each CPU having local memory and integrated memory controller. Local access is fast; remote access (via interconnect) is slower—hence non-uniform. NUMA eliminates the central bandwidth bottleneck and scales to large systems (10s-100s of CPUs), but requires **NUMA-aware software** to optimize memory locality. Point-to-point interconnects provide best performance. NUMA complexity makes it rare in embedded systems.

**big.LITTLE Architecture** (ARM) combines heterogeneous cores: high-performance "big" cores for compute-intensive tasks and energy-efficient "LITTLE" cores for background tasks. Two execution models exist:
- **CPU Migration**: Paired big/LITTLE cores appear as logical cores to OS; system dynamically switches between them based on load (simpler, less flexible)
- **Global Task Scheduling**: All cores visible individually; scheduler assigns tasks to appropriate core type based on tracked load (complex, more flexible, better performance)

The **tracked load** mechanism uses moving averages to determine task intensity and trigger up-migration (to big core) or down-migration (to LITTLE core). This enables automatic per-task optimization, achieving 50-70% energy savings for typical mobile workloads compared to always using big cores.

**Key Architectural Insights:**
- Memory bandwidth is the fundamental bottleneck in multi-processor scaling
- Cache coherence complexity grows with number of cores
- UMA simplicity suits small embedded systems (4-8 cores)
- NUMA scalability suits large servers but rarely embedded
- Heterogeneous multi-core (big.LITTLE) optimizes energy-performance trade-off better than homogeneous
- Modern embedded SoCs (like Zynq UltraScale+) use hybrid UMA+NUMA approaches

These architectural concepts directly impact embedded system design, explaining why certain design choices (like R5 local OCM, A53 cluster sharing DDR) are made and how they affect performance, energy, and programmability.

---

## Possible Exam Questions

### Definitions & Technical Terms (Questions 1-30)

1. **What is the system bus and why is it a bottleneck in multi-CPU systems?**
   - Shared communication channel for CPU, memory, I/O
   - Bottleneck: bandwidth shared among all CPUs, arbitration delay, increases latency
   - Limits scalability to ~4 CPUs

2. **Define cache coherence and explain why it's a problem in multi-processor systems.**
   - Problem of keeping multiple cache copies of same memory location consistent
   - Arises when multiple CPUs cache same data
   - Without solution: CPUs may read stale data (incorrect program behavior)

3. **What is snooping in the context of cache coherence?**
   - Protocol where caches monitor (snoop) bus transactions
   - Each cache listens to other CPUs' memory accesses
   - Detects when others access data also in local cache
   - Enables coordinated cache updates

4. **Explain write invalidation as a cache coherence mechanism.**
   - When CPU writes to memory, hardware broadcasts "invalidate" signal
   - Other caches with that address mark it invalid
   - Next read by other CPU misses, reloads from memory (getting new value)
   - Most common coherence protocol

5. **Define Uniform Memory Access (UMA) and its key characteristic.**
   - Architecture where all CPUs have equal access time to any memory
   - Memory centralized with shared controller (Northbridge)
   - Key: access time independent of which CPU accesses which memory

6. **What is Non-Uniform Memory Access (NUMA)?**
   - Architecture where access time depends on memory location relative to CPU
   - Each CPU has local memory (fast) and can access remote memory (slow)
   - Local access faster than remote → non-uniform
   - Scales better than UMA but more complex

7. **Define Northbridge and its role in UMA architecture.**
   - Central memory controller hub in UMA
   - Connects all CPUs to memory via dedicated links
   - Handles memory access requests from all CPUs
   - Bottleneck limiting UMA scalability

8. **What is bus arbitration and why is it needed?**
   - Mechanism for multiple CPUs to coordinate shared bus access
   - Needed when multiple masters compete for bus
   - Introduces latency (waiting for bus access)
   - Limits multi-CPU scalability

9. **Define big.LITTLE architecture.**
   - ARM heterogeneous multi-core architecture
   - Combines high-performance "big" cores with energy-efficient "LITTLE" cores
   - Same ISA, different implementations
   - Enables dynamic performance/energy optimization

10. **What are big cores and LITTLE cores in ARM architecture?**
    - big: High-performance, high-power (Cortex-A7x)
    - LITTLE: Low-performance, low-power (Cortex-A5x)
    - 2-5× performance difference, 5-10× power difference
    - Right core for right task

11. **Define CPU Migration execution model in big.LITTLE.**
    - big and LITTLE cores paired, appear as single logical core to OS
    - Only one of pair active at any time
    - Dynamically switch based on load (using DVFS)
    - Simpler for OS but less flexible

12. **Define Global Task Scheduling execution model in big.LITTLE.**
    - OS sees all cores individually (not paired)
    - Can use any subset simultaneously
    - Scheduler assigns tasks to appropriate core type based on load
    - More complex but higher parallelism

13. **What is tracked load in big.LITTLE context?**
    - Moving average of task's computational demand
    - Tracked by scheduler over time
    - Used to determine whether task should run on big or LITTLE
    - Adjusted for CPU frequency (DVFS)

14. **Define up migration and down migration.**
    - Up migration: moving task from LITTLE to big core (load increased)
    - Down migration: moving task from big to LITTLE core (load decreased)
    - Based on tracked load vs thresholds

15. **What is a coherent interconnect?**
    - Interconnect supporting cache coherence protocols
    - Ensures data consistency across all caches and memory
    - Example: ARM CoreLink CCI
    - Critical for big.LITTLE global task scheduling

16. **Define local memory and remote memory in NUMA.**
    - Local: memory directly attached to a CPU (fast access)
    - Remote: memory attached to different CPU (slower, via interconnect)
    - Non-uniform access time: local << remote

17. **What is a memory controller and where is it located in UMA vs NUMA?**
    - Hardware managing memory access (addressing, timing, refresh)
    - UMA: centralized in Northbridge
    - NUMA: integrated into each CPU (local controller)

18. **Define point-to-point interconnect and its advantage.**
    - Direct connections between every pair of CPUs
    - Eliminates multi-hop latency
    - Higher bandwidth (dedicated links)
    - But complexity O(N²) for N CPUs

19. **What is L3 cache and how does it help in multi-core systems?**
    - Largest, slowest cache level, shared among cores
    - Acts as buffer between cores and memory controller
    - Snooping happens in L3 (faster than checking memory)
    - Reduces memory bandwidth pressure

20. **Define fork migration, wake migration, and forced migration.**
    - Fork: new task starts on big core (default assumption: demanding)
    - Wake: task wakes, assigned based on tracked load history
    - Forced: periodic re-evaluation, migrate if on wrong core type

21. **What is memory bandwidth and why is it critical?**
    - Rate data can be read/written to memory (bytes/second)
    - Critical: CPU much faster than memory → memory becomes bottleneck
    - Multi-core: shared bandwidth divided among CPUs

22. **Define scalability in context of multi-processor architectures.**
    - Ability to support increasing number of processors effectively
    - Traditional bus: poor (~4 CPUs)
    - UMA: limited (~4-8 CPUs)
    - NUMA: good (10s-100s CPUs)

23. **What is DVFS and how is it used in CPU Migration?**
    - Dynamic Voltage and Frequency Scaling
    - Adjusts CPU voltage/frequency based on load
    - Used in CPU Migration for smooth transitions between big/LITTLE
    - Combines with core switching for wide energy range

24. **Define cache hit and cache miss.**
    - Hit: requested data found in cache (fast, few cycles)
    - Miss: data not in cache, must fetch from memory (slow, 100s cycles)
    - High hit rate essential for performance

25. **What is a cluster in big.LITTLE architecture?**
    - Group of cores of same type (all big or all LITTLE)
    - Cores in cluster typically share L2/L3 cache
    - Simplifies management and coherence

26. **Define memory address space in UMA vs NUMA.**
    - UMA: single unified address space (one memory)
    - NUMA: each CPU has own space, remote addresses mapped by OS
    - NUMA requires software support for mapping

27. **What is cache line and why is it relevant to coherence?**
    - Smallest unit transferred between cache and memory (typically 64 bytes)
    - Coherence operates at cache line granularity
    - One CPU writing byte in line invalidates entire line in other caches

28. **Define signal propagation delay in context of bus latency.**
    - Time for electrical signal to traverse physical bus
    - Limited by speed of light (~0.5c in PCB traces)
    - Longer bus (more CPUs) = higher latency
    - Contributes to non-scalability

29. **What is the difference between embedded multi-core and server multi-core?**
    - Embedded: 2-8 cores, UMA, power-constrained, big.LITTLE common
    - Server: 10s-100s cores, NUMA, performance-focused, homogeneous
    - Embedded prioritizes simplicity and energy

30. **Define heterogeneous vs homogeneous multi-core.**
    - Heterogeneous: different core types (e.g., big.LITTLE, CPU+FPGA)
    - Homogeneous: all cores identical
    - Heterogeneous better energy-performance trade-off

### Core Concepts (Questions 31-60)

31. **Why does shared bus limit scalability to approximately 4 CPUs?**
    - Bandwidth divided: 4 CPUs = 25% bandwidth each
    - Arbitration overhead increases with more masters
    - Bus length increases (signal propagation delay)
    - At ~4 CPUs, overhead outweighs parallelism benefit

32. **Explain the memory bandwidth bottleneck in traditional multi-CPU systems.**
    - Multiple CPUs share single bus to memory
    - Bus bandwidth fixed (e.g., 100 GB/s)
    - Each CPU gets total/N bandwidth
    - CPUs stall waiting for bus → underutilized
    - Fundamental limit on parallelism

33. **How does UMA improve upon traditional shared bus architecture?**
    - Replaces shared bus with Northbridge
    - Dedicated CPU-to-Northbridge links (no arbitration)
    - Multiple memory channels increase total bandwidth
    - Uniform access time (no arbitration variation)
    - But Northbridge becomes new bottleneck

34. **Why does UMA still have scalability limits despite eliminating shared bus?**
    - Northbridge bandwidth still centralized and limited
    - All memory traffic funnels through Northbridge
    - I/O also goes through Northbridge (competes with memory)
    - Practical limit: ~4-8 CPUs before bottleneck saturates

35. **How does NUMA eliminate the central bandwidth bottleneck?**
    - Each CPU has own local memory and controller
    - Local access doesn't use central resource
    - Adding CPUs doesn't reduce bandwidth for existing CPUs (for local access)
    - Interconnect only used for remote access

36. **What are the software challenges introduced by NUMA?**
    - Must allocate memory local to CPU for performance
    - OS must be NUMA-aware (topology, allocation, scheduling)
    - Applications benefit from locality awareness
    - Poor software can severely hurt performance (excessive remote access)
    - More complex than UMA programming

37. **Explain how cache coherence is achieved with snooping protocol.**
    - All caches connected to bus/interconnect
    - Every memory transaction broadcast
    - All caches snoop (monitor) transactions
    - If address matches cached data:
      - Reads: no action
      - Writes: invalidate local copy
    - Next read misses, reloads updated value

38. **Why is L3 cache beneficial for cache coherence?**
    - Snooping can happen in L3 instead of memory/Northbridge
    - Faster (L3 closer than memory)
    - Reduces memory controller traffic
    - L3 acts as "snoop filter"
    - Improves scalability slightly

39. **What is the fundamental motivation for big.LITTLE architecture?**
    - Most tasks don't need maximum performance constantly
    - Background tasks: waste power on big cores
    - Demanding tasks: need big core performance
    - Heterogeneous cores enable dynamic optimization
    - Much better energy efficiency than homogeneous

40. **How does big.LITTLE achieve energy savings?**
    - Light tasks run on LITTLE cores (much lower power)
    - Heavy tasks run on big cores (when needed)
    - Only use power needed for current workload
    - Studies show 50-70% energy savings for typical mobile workloads
    - Better than homogeneous with DVFS alone

41. **Compare CPU Migration vs Global Task Scheduling trade-offs.**
    - CPU Migration: simpler OS, less flexible, N logical cores
    - Global Task Scheduling: complex OS, more flexible, 2N physical cores
    - Migration: works with existing OS
    - Global: requires big.LITTLE-aware scheduler
    - Global better performance (more parallelism)

42. **How does tracked load mechanism work in global task scheduling?**
    - Scheduler maintains moving average of task's CPU usage
    - Updated continuously during execution
    - Adjusted for CPU frequency (DVFS)
    - High tracked load → task assigned to big core
    - Low tracked load → task assigned to LITTLE core
    - Enables automatic per-task optimization

43. **Explain the role of thresholds in task migration decisions.**
    - Two thresholds: up_migration_threshold, down_migration_threshold
    - Tracked load exceeds up_threshold → migrate to big
    - Tracked load falls below down_threshold → migrate to LITTLE
    - Hysteresis (gap between thresholds) prevents ping-ponging
    - Typical: up=85%, down=40% (example)

44. **Why is Zynq UltraScale+ considered hybrid UMA+NUMA?**
    - A53 cluster: UMA (all A53s uniform access to DDR via CCI)
    - R5: NUMA-like (local OCM fast, shared DDR slow)
    - Two different architectural paradigms in one chip
    - Fits different processor roles (A53 general-purpose, R5 real-time)

45. **How does local memory (OCM) benefit R5 real-time performance?**
    - OCM: 256 KB, tightly coupled, deterministic access (~10-20 ns)
    - DDR: shared, slower (~100-200 ns), non-deterministic (contention)
    - Real-time code/data in OCM ensures predictable timing
    - Critical for meeting hard real-time deadlines

46. **What problems arise from multiple hops in NUMA interconnects?**
    - Each hop adds latency (router delays)
    - Bandwidth may be shared at each hop
    - Stronger non-uniformity (local vs 1-hop vs 2-hop vs 3-hop)
    - More complex cache coherence
    - Point-to-point interconnect avoids this but increases complexity

47. **How do cache hierarchies (L1, L2, L3) support multi-core systems?**
    - L1: fastest, per-core, minimizes CPU stalls
    - L2: buffer to L3/memory, per-core or small shared
    - L3: shared, large, reduces memory traffic
    - Hierarchy trades size for speed
    - Shared L3 helps coherence and bandwidth

48. **Why is memory locality critical for NUMA performance?**
    - Local access: fast (direct to local controller)
    - Remote access: slow (interconnect + remote controller)
    - Ratio: remote can be 2-3× slower
    - Poor locality: excessive remote access → severe performance degradation
    - Software must optimize for locality

49. **Explain false sharing and its impact on multi-core performance.**
    - Multiple CPUs accessing different data in same cache line
    - One CPU writes → invalidates entire line in other caches
    - Other CPUs' data also invalidated (even though not modified)
    - Causes unnecessary cache misses
    - Reduces parallel efficiency

50. **How does big.LITTLE handle interrupts for low latency?**
    - Interrupts can be targeted to specific core type
    - Low-latency interrupts (e.g., touch input) → LITTLE cores
    - LITTLE cores have capacity (not busy with heavy tasks)
    - Faster response than waiting for big core to be available
    - Compute-heavy interrupts → big cores

51. **What is the relationship between cache coherence and correctness?**
    - Coherence ensures all CPUs see same memory value
    - Critical for shared-memory parallel programs
    - Without coherence: race conditions, incorrect results
    - Example: counter increments lost
    - Must be solved in hardware (transparent to software)

52. **Why is directory-based coherence needed for NUMA vs snooping for UMA?**
    - UMA: broadcast on bus feasible (small number of CPUs)
    - NUMA: no central bus, broadcasting expensive over interconnect
    - Directory tracks which caches have which lines
    - Send coherence messages only to relevant nodes
    - Scalable to large systems

53. **How does adding more cores affect cache coherence complexity?**
    - More caches to keep coherent
    - More snoop traffic (UMA) or directory messages (NUMA)
    - Larger L3 or more complex directory
    - More possible states to track
    - Active research area

54. **Explain the energy-performance trade-off space that big.LITTLE addresses.**
    - Homogeneous cores: fixed performance/energy point (DVFS range limited)
    - big.LITTLE: wide range (LITTLE efficient, big fast)
    - Dynamic: match core to task requirement
    - Better than always-big (energy) or always-LITTLE (performance)

55. **Why is global task scheduling more suitable for complex workloads?**
    - Mixed workload: some heavy, many light tasks simultaneously
    - Global: heavy on big, light on LITTLE (at same time)
    - CPU Migration: must choose one per pair (less parallelism)
    - Smartphone workload: inherently heterogeneous → global better

56. **How does cache impact memory bandwidth bottleneck?**
    - Cache hits don't use memory bandwidth
    - High hit rate → less memory traffic
    - Multi-core: caches reduce shared bandwidth pressure
    - But cache misses still bottleneck
    - Coherence traffic adds to bus usage

57. **What role does software play in NUMA vs UMA performance?**
    - UMA: software mostly agnostic (hardware provides uniform access)
    - NUMA: software critical (must optimize locality)
    - NUMA-aware allocation: allocate local memory
    - NUMA-aware scheduling: pin tasks to CPUs
    - Poor software: NUMA can be slower than UMA!

58. **Explain how point-to-point interconnect improves over shared interconnect.**
    - Shared: must traverse to central point, potential contention, multi-hop
    - Point-to-point: direct link, no contention, single hop
    - Higher aggregate bandwidth (N² links)
    - Lower latency (no multi-hop)
    - But higher complexity and cost

59. **Why do embedded systems typically use UMA instead of NUMA?**
    - Small core count (2-8) → UMA sufficient
    - Simplicity: NUMA complex for software and hardware
    - Power: NUMA overhead not justified
    - Cost: UMA cheaper (fewer controllers, simpler interconnect)
    - NUMA benefits appear at larger scale (server/HPC)

60. **How does big.LITTLE relate to design space exploration concepts?**
    - big.LITTLE: point in architectural design space
    - Axes: energy, performance, cost
    - Pareto-optimal: better energy-performance than homogeneous
    - Could use DSE to determine: how many big? how many LITTLE?
    - Real-world application of multi-objective optimization

### Comparisons & Problem-Solving (Questions 61-100)

61. **Compare memory bandwidth per CPU in traditional multi-CPU (4 CPUs, 100 GB/s bus) vs UMA (same CPUs, same total bandwidth).**
    - Traditional: 100 GB/s / 4 = 25 GB/s per CPU (shared, divided)
    - UMA: 100 GB/s total, but dedicated links reduce contention
    - UMA better: no arbitration delay, but Northbridge still bottleneck
    - If Northbridge has 100 GB/s: similar limitation
    - UMA advantage: uniform access time, not bandwidth per se

62. **Given: CPU freq=2GHz, 4 CPUs, 64-byte accesses every 10 cycles. Calculate total memory bandwidth needed.**
    - Accesses per CPU per second: 2×10⁹ / 10 = 2×10⁸
    - Bandwidth per CPU: 2×10⁸ × 64 bytes = 12.8 GB/s
    - Total: 4 × 12.8 = 51.2 GB/s
    - Memory system must provide >51.2 GB/s

63. **Why would NUMA with poor memory locality perform worse than UMA?**
    - NUMA with poor locality: mostly remote accesses (2-3× slower)
    - UMA: consistent (moderate) access time
    - Example: NUMA remote 150ns, UMA 100ns
    - If 80% remote in NUMA: 0.8×150 + 0.2×50 = 130 ns average (worse than UMA 100 ns!)
    - NUMA needs good locality to win

64. **Compare cache coherence overhead in 4-CPU UMA vs 16-CPU NUMA.**
    - 4-CPU UMA: snooping on bus, broadcast to 4 caches (manageable)
    - 16-CPU NUMA: directory-based, point-to-point messages (complex)
    - UMA: every write broadcasts to all → O(N) traffic
    - NUMA: directory + targeted messages → better scaling
    - But NUMA inherently more complex

65. **For a smartphone (8-core big.LITTLE, 4 big + 4 LITTLE), which execution model would you recommend?**
    - Recommend: Global Task Scheduling
    - Reason: smartphones have mixed workload (heavy apps + background)
    - Global allows simultaneous: heavy on big, light on LITTLE
    - Modern Android/iOS support global task scheduling
    - Worth OS complexity for energy savings

66. **Calculate energy savings: big core 5W, LITTLE 0.5W, task on LITTLE 90% of time.**
    - Always big: 5W
    - big.LITTLE: 0.9×0.5 + 0.1×5 = 0.45 + 0.5 = 0.95W
    - Savings: (5 - 0.95) / 5 = 80.1% energy reduction!
    - Demonstrates power of running light tasks on LITTLE

67. **Why is tracked load adjusted for CPU frequency (DVFS)?**
    - Task may have same CPU usage % at different frequencies
    - 50% usage at 1 GHz = less absolute load than 50% at 2 GHz
    - Without adjustment: DVFS would trigger incorrect migrations
    - Adjustment: normalize to equivalent full-speed usage
    - Prevents frequency changes from causing migration thrashing

68. **Compare latency: UMA L3 snooping vs NUMA directory lookup + remote fetch.**
    - UMA L3 snoop: ~40-70 cycles (L3 latency)
    - NUMA directory lookup: ~50 cycles + interconnect ~50-100 cycles + remote memory ~200 cycles = ~300-350 cycles
    - UMA faster for coherence checks
    - But NUMA scales better (doesn't bottleneck)

69. **Design a memory allocation strategy for NUMA system running parallel matrix multiplication.**
    - Partition matrix into blocks
    - Assign each block to one CPU
    - Allocate block's memory on that CPU's local memory (numactl --membind)
    - Pin worker thread to same CPU (numactl --cpunodebind)
    - Maximizes local access, minimizes remote
    - Achieves near-linear speedup

70. **For Ultra96 R5 real-time task, where should you place code and data?**
    - Code: OCM (256 KB, local, deterministic)
    - Critical data structures: OCM
    - Shared data with A53: DDR (slower but accessible)
    - Linker script: place .text and .data in OCM
    - Result: deterministic real-time execution

71. **Compare scalability: shared bus, UMA, NUMA for doubling from 4 to 8 CPUs.**
    - Shared bus: 4→8 CPUs: probably won't work (bandwidth/arbitration collapse)
    - UMA: 4→8: may work but Northbridge heavily loaded, diminishing returns
    - NUMA: 4→8: scales well (local bandwidth unchanged)
    - NUMA clear winner for scaling

72. **Explain why big.LITTLE achieves better energy-performance than DVFS alone on homogeneous cores.**
    - DVFS: limited range (e.g., 0.8-2.0 GHz, 2.5× frequency range)
    - big.LITTLE: 5-10× power difference between big and LITTLE
    - DVFS on homogeneous: still using big core at low frequency (inefficient)
    - LITTLE core at full speed more efficient than big core at low speed
    - Wider dynamic range

73. **How would you verify if a system is experiencing cache coherence problems?**
    - Symptoms: incorrect results in parallel programs (race conditions)
    - Different results on different runs (non-deterministic)
    - Failures scale with number of cores (more cores = more frequent)
    - Test: disable caching (performance tanks but results correct?) suggests coherence issue
    - Hardware counters: track coherence misses, invalidations

74. **Design thresholds for up/down migration: heavy task varies 30-90% load, light task 5-20% load.**
    - Up threshold: 60% (well above light task max)
    - Down threshold: 25% (well below heavy task min)
    - Hysteresis: 60-25 = 35% gap (prevents thrashing)
    - Heavy task: 30-90% → mostly above 60% → stays on big
    - Light task: 5-20% → always below 25% → stays on LITTLE
    - Transitioning task: migrates appropriately

75. **Calculate remote access percentage tolerable in NUMA for same performance as UMA.**
    - UMA: 100 ns average
    - NUMA: local 50 ns, remote 150 ns
    - Need: P_local × 50 + P_remote × 150 ≤ 100
    - P_remote = 1 - P_local
    - P_local × 50 + (1 - P_local) × 150 ≤ 100
    - 50P + 150 - 150P ≤ 100
    - -100P ≤ -50
    - P_local ≥ 0.5 (50% local minimum)
    - Answer: Can tolerate up to 50% remote accesses

76. **Why might a 4-big + 4-LITTLE system outperform 8-big system for battery life?**
    - 8-big: all cores consuming 5W each when active = 40W peak
    - 4+4 big.LITTLE: 4×5W + 4×0.5W = 20+2 = 22W peak
    - More importantly, average: big.LITTLE uses LITTLE for light tasks
    - Example: 2 big active (heavy) + 3 LITTLE (light) + 3 idle = 2×5 + 3×0.5 = 11.5W
    - 8-big would use 5 cores at low frequency ≈ 5×2W = 10W (competitive)
    - But LITTLE at full speed more efficient than big at low speed
    - Answer: big.LITTLE wins by using right core for task

77. **How does cache line size affect coherence overhead?**
    - Larger line: fewer lines to track (simpler directory)
    - But: more false sharing (unrelated data in same line)
    - Smaller line: less false sharing
    - But: more lines to track, more coherence traffic
    - Typical: 64 bytes as compromise

78. **Design allocation for big.LITTLE with asymmetric counts (2 big, 6 LITTLE) vs symmetric (4 big, 4 LITTLE).**
    - 2+6: optimizes for typical workload (mostly light tasks, occasional heavy)
    - Lower cost, lower power (fewer expensive big cores)
    - 4+4: better peak performance (more big cores for parallel heavy tasks)
    - Higher cost, higher power
    - Choose based on workload: mobile → 2+6, performance-focused → 4+4

79. **Why is Northbridge bandwidth a bottleneck even with multiple memory channels?**
    - Multiple channels increase total bandwidth (e.g., dual-channel 2× single)
    - But Northbridge must still aggregate all CPU requests
    - Switch fabric inside Northbridge has limits
    - All traffic funnels through Northbridge (single point)
    - Can add more channels but doesn't eliminate bottleneck

80. **Calculate speedup from adding 4th CPU in shared bus vs UMA system.**
    - Shared bus: bandwidth/4 per CPU, arbitration overhead → speedup ~2.5× (not 4×)
    - UMA: better bandwidth management, less arbitration → speedup ~3.2× (better, still not 4×)
    - Neither reaches 4× due to memory bottleneck and Amdahl's law
    - NUMA with good locality: closer to 4× (local bandwidth not shared)

81. **Explain why signal propagation delay matters more at higher CPU frequencies.**
    - Propagation delay: ~2 ns for 30cm (fixed, physics)
    - At 1 GHz: 2 ns = 2 cycles
    - At 3 GHz: 2 ns = 6 cycles
    - At 5 GHz: 2 ns = 10 cycles
    - Relative impact increases with frequency

82. **Compare memory latency impact: computation-intensive vs memory-intensive workload on multi-core.**
    - Computation-intensive: memory latency matters less (high cache hit rate)
    - Multi-core scales well (CPUs mostly computing, not waiting for memory)
    - Memory-intensive: memory latency critical (many cache misses)
    - Multi-core scales poorly (all CPUs waiting for memory, bottleneck)
    - Architecture choice matters more for memory-intensive

83. **Design a big.LITTLE scheduler policy for video encoding (heavy, but allows frame drops).**
    - Video encoding: heavy computational load
    - But soft real-time: occasional frame drops acceptable
    - Strategy: use big cores normally
    - If thermal constraint or battery low: migrate to LITTLE
    - Accept slower encoding (frame drops) to save energy/reduce heat
    - Dynamic priority: high when user interacting, lower when background

84. **How would cache coherence work if CPUs had no caches?**
    - No caches → no multiple copies → no coherence problem!
    - But: terrible performance (all accesses to slow memory)
    - Not practical for modern CPUs
    - Demonstrates: caches essential for performance, coherence essential for correctness

85. **Calculate L3 cache size needed: 4 CPUs, each with 1 MB L2, want 80% L2 misses to hit L3.**
    - If L2 miss, check L3 before memory
    - 80% of L2 misses should find data in L3
    - L3 should be >>L2 to hold evicted L2 data
    - Typical: L3 = 2-4× total L2 size
    - Total L2: 4 MB, so L3: 8-16 MB
    - Answer: ~8-16 MB L3

86. **Why might a NUMA system with 8 CPUs outperform UMA with 8 CPUs for embarrassingly parallel workload?**
    - Embarrassingly parallel: no inter-task communication
    - Each task accesses only its own data
    - NUMA: allocate each task's data locally
    - Result: all accesses local (8× local bandwidth)
    - UMA: all tasks compete for shared bandwidth (bottleneck)
    - NUMA wins decisively

87. **Design memory hierarchy for embedded device: cost-sensitive, 2 CPUs, moderate performance.**
    - UMA architecture (simplicity, sufficient for 2 CPUs)
    - Per-core: 32 KB L1I + 32 KB L1D (standard)
    - Per-core: 256 KB L2 (reasonable buffer)
    - Shared: 512 KB L3 (helps coherence)
    - Single-channel DDR (lower cost)
    - Total cost << NUMA, performance good for 2 CPUs

88. **How does fork migration default (new task on big core) avoid penalizing demanding tasks?**
    - New task: no history (don't know if demanding or light)
    - Default to big: ensures demanding task has immediate performance
    - If actually light: tracked load stays low → migrates down quickly (wake migration)
    - Cost: light task wastes power briefly on big core (milliseconds)
    - Benefit: demanding task doesn't start slow on LITTLE

89. **Compare UMA with 4 CPUs + L3 vs NUMA with 4 CPUs (no L3) for workload with 50% shared data.**
    - 50% shared data → frequent cross-CPU access
    - UMA + L3: shared data in L3 (fast access for all)
    - NUMA: shared data must be in one CPU's memory (remote for others)
    - Result: UMA likely better (shared L3 helps shared data)
    - NUMA better for partitioned data, worse for shared

90. **Calculate maximum CPUs sustainable: memory system 200 GB/s, each CPU needs 40 GB/s for full utilization.**
    - Total bandwidth / per-CPU requirement = 200 / 40 = 5 CPUs max
    - Beyond 5: CPUs can't get enough bandwidth → underutilized
    - Solution: NUMA (each CPU gets local bandwidth)

91. **Why is coherent interconnect critical for big.LITTLE global task scheduling but not CPU migration?**
    - Global: tasks can be on any core, accessing shared data
    - Need coherence across all cores (big and LITTLE) simultaneously
    - CPU Migration: only one core active per pair
    - No simultaneous access → coherence simpler
    - Global: requires hardware coherence support

92. **Design test to measure cache coherence overhead: compare cache-line-aligned vs misaligned data structures.**
    - Aligned: each thread's data in separate cache lines
    - Misaligned: threads share cache lines (false sharing)
    - Run parallel increment on each structure
    - Measure time: misaligned should be much slower (coherence traffic)
    - Demonstrates coherence overhead from false sharing

93. **How would you optimize a NUMA system for database workload (shared data, many transactions)?**
    - Challenge: database = lots of shared data (tables, indices)
    - Strategy: partition by tables/rows to different CPUs
    - Allocate each partition locally
    - Minimize cross-partition transactions
    - Use NUMA-aware database (e.g., PostgreSQL with NUMA support)
    - Alternative: replicate read-only data on all nodes (if small enough)

94. **Compare 4-core homogeneous at 2 GHz vs 2-big (2.5 GHz) + 2-LITTLE (1.5 GHz) for mixed workload.**
    - Homogeneous: all tasks at 2 GHz
    - big.LITTLE: heavy on big (2.5 GHz), light on LITTLE (1.5 GHz)
    - Heavy tasks: big.LITTLE wins (2.5 > 2.0 GHz)
    - Light tasks: big.LITTLE much lower power (LITTLE cores)
    - Overall: big.LITTLE better performance AND better energy

95. **Why is L3 cache shared rather than private in most multi-core designs?**
    - Shared: better utilization (any core can use full L3)
    - Private: some cores' L3 full, others empty (inefficient)
    - Shared: simplifies coherence (common point for snooping)
    - Shared: enables cache-to-cache transfers between cores
    - Trade-off: shared has contention, but benefits outweigh

96. **Calculate worst-case memory access time in NUMA: local 50ns, interconnect hop 30ns, 3-hop path to remote memory.**
    - Local memory access: 50 ns
    - Remote memory: local controller 50ns + 3 hops × 30ns + remote controller 50ns
    - Total: 50 + 90 + 50 = 190 ns
    - Ratio: 190 / 50 = 3.8× slower!
    - Demonstrates: strong non-uniformity in multi-hop NUMA

97. **Design scheduler policy for asymmetric real-time system: 1 hard RT task, many soft RT tasks on big.LITTLE.**
    - Hard RT: guaranteed performance → always on big core (even if "light")
    - Soft RT: can tolerate variations → use tracked load migration
    - Pin hard RT to big core (highest priority)
    - Soft RT on remaining big + all LITTLE cores
    - Ensures hard RT deadline while optimizing others

98. **How does big.LITTLE affect software development compared to homogeneous multi-core?**
    - Homogeneous: OS handles scheduling (transparent to app)
    - big.LITTLE: ideally transparent, but app can hint
    - Some OSes expose core type to apps (opt-in optimization)
    - Generally: works with existing software (OS handles)
    - Advanced: apps can use thread affinity to prefer big/LITTLE

99. **Why might NUMA with point-to-point interconnect still have non-uniformity despite no multi-hop?**
    - Even single hop has latency (interconnect traversal)
    - Remote memory controller access adds latency
    - Local: CPU → local controller → memory
    - Remote: CPU → interconnect → remote controller → memory
    - Additional steps → still slower than local
    - Non-uniformity reduced but not eliminated

100. **Design a multi-processor architecture for 128 CPUs: UMA or NUMA? Why?**
    - NUMA required (UMA can't scale to 128 CPUs)
    - UMA Northbridge would collapse (impossible bandwidth)
    - NUMA: 128 local memory controllers (128× local bandwidth)
    - Interconnect: mesh or hypercube topology
    - Directory-based coherence
    - NUMA-aware OS essential
    - Example: AMD EPYC, Intel Xeon multi-socket servers
    - Answer: NUMA only viable option at this scale

---

**End of Lecture 14 Notes**
