# Lecture 11: Design Space Exploration (DSE) - Introduction

## Technical Terms & Definitions

### Core DSE Terms
- **Design Space Exploration (DSE)**: The systematic investigation of alternative design solutions to find optimal implementations that balance multiple conflicting objectives (e.g., performance, energy, area, cost)
- **Multiobjective Optimization Problem**: A problem with multiple, often conflicting objectives (e.g., minimize energy AND maximize performance) where no single solution optimizes all objectives simultaneously
- **Decision Space**: The space of all possible design configurations/alternatives (input space); each point represents a specific design decision
- **Objective Space**: The space of all possible performance/quality metrics (output space); each point represents a specific combination of objective values
- **Design Point**: A specific configuration in the decision space that represents one possible implementation alternative
- **Objective Vector**: The set of objective values (e.g., [energy, performance, area]) corresponding to a specific design point

### Pareto Optimality Terms
- **Pareto-Dominance**: Design A dominates design B if A is better or equal in all objectives and strictly better in at least one objective
- **Pareto-Optimal Design**: A design that is not dominated by any other design in the decision space; no improvement in one objective is possible without worsening another
- **Pareto-Optimal Set (Pareto Front)**: The set of all Pareto-optimal designs; represents the best trade-offs between conflicting objectives
- **Weakly Pareto-Optimal**: A design where no other design is better in all objectives (but may be equal in all)
- **Strictly Pareto-Optimal**: A design where any improvement in one objective necessarily worsens at least one other objective

### Architecture Terms
- **Architecture Template**: A parameterized system structure that defines the space of possible implementations (e.g., number of processors, cache sizes, communication topology)
- **Allocation**: The assignment of computational resources (processors, memories, buses) to a system design
- **Binding**: The mapping of application tasks/functions to allocated hardware resources
- **Scheduling**: The temporal ordering of task executions on bound resources
- **Resource Type**: A category of hardware components (e.g., RISC processor, DSP, FPGA, ASIC)
- **Resource Instance**: A specific instantiation of a resource type in the architecture

### Design Evaluation Terms
- **Analysis**: Mathematical modeling and calculation to predict design properties (fast, early availability, low accuracy for complex systems)
- **Simulation**: Execution of a model that mimics system behavior (medium speed and accuracy, widely used)
- **Measurement**: Observation of a real prototype or final system (slow, late availability, high accuracy)
- **Performance Model**: A mathematical or computational model that predicts execution time, throughput, or latency
- **Energy Model**: A model that estimates energy consumption based on component activity and characteristics
- **Validation**: Verification that the design model accurately represents the real system behavior

### System Design Terms
- **Hardware/Software Partitioning**: The decision of which functions to implement in hardware (FPGA, ASIC) vs software (processor)
- **Co-Design**: Simultaneous design and optimization of hardware and software components
- **System-on-Chip (SoC)**: Integration of multiple hardware components (processors, accelerators, memories) on a single chip
- **IP Core**: Pre-designed hardware module (Intellectual Property) that can be integrated into larger designs
- **Processing Element (PE)**: A computational unit in a multi-processor system (CPU, DSP, accelerator, etc.)

### Network Processor Terms
- **Network Processor**: Specialized processor designed for packet processing in networking equipment
- **Packet Processing**: The operations performed on network packets (classification, routing, filtering, modification)
- **Throughput**: The rate at which packets can be processed (packets/second or bits/second)
- **Latency**: The time delay from packet arrival to completion of processing
- **Pipeline Stage**: A functional unit in a pipelined network processor that performs a specific processing step
- **Packet Header**: The metadata portion of a network packet containing addressing and control information

### Cache Organization Terms
- **Cache**: Fast memory buffer that stores frequently accessed data to reduce access latency
- **Cache Size**: Total storage capacity of the cache (e.g., 32 KB)
- **Cache Line Size**: The granularity of data transfer between cache and main memory (e.g., 32 bytes)
- **Associativity**: Number of cache locations where a memory block can be placed (direct-mapped, 2-way, 4-way, fully associative)
- **Hit Rate**: Percentage of memory accesses that find data in the cache
- **Miss Penalty**: Additional time required when requested data is not in cache
- **Cache Replacement Policy**: Algorithm for deciding which cache line to evict (LRU, random, FIFO)

### Exploration Process Terms
- **Specification**: High-level description of system requirements and constraints
- **Exploration**: Systematic investigation of the design space to identify promising alternatives
- **Refinement**: Detailed design and implementation of a selected architecture alternative
- **Iteration**: Repeated cycles of exploration and refinement to converge on optimal design
- **Design Constraint**: A requirement that limits the design space (e.g., energy < 100 mJ, cost < $50)
- **Feasible Design**: A design point that satisfies all specified constraints

### Optimization Terms
- **Objective Function**: A mathematical function that quantifies a design quality metric to be optimized
- **Constraint Function**: A mathematical inequality or equality that must be satisfied by feasible designs
- **Global Optimum**: The best solution across the entire design space
- **Local Optimum**: A solution that is best within a neighborhood but may not be globally optimal
- **Design Variable**: A parameter that can be adjusted during exploration (e.g., number of processors, voltage level)
- **Search Space**: The set of all possible combinations of design variable values

## Core Concepts

### 1. Design Space Exploration as Multiobjective Optimization

**Fundamental Principle:**
Embedded system design involves balancing multiple conflicting objectives:
- **Performance** (maximize throughput, minimize latency)
- **Energy consumption** (minimize for battery life)
- **Area/Cost** (minimize chip size and manufacturing cost)
- **Reliability** (maximize correctness and fault tolerance)
- **Real-time guarantees** (meet deadlines)

**Why Multiobjective?**
- Optimizing one objective typically degrades others
- Example: Faster execution (better performance) usually requires more energy
- No single "best" design exists; instead, there are trade-offs

**Key Insight:**
The goal of DSE is not to find ONE optimal design, but to identify the **Pareto-optimal set** of designs that represent the best possible trade-offs.

### 2. Decision Space vs Objective Space

**Decision Space:**
- **Input**: Design parameters and configuration choices
- **Examples**:
  - Number of processors (1, 2, 4, 8)
  - Memory size (64 KB, 128 KB, 256 KB)
  - Hardware/software partitioning decisions
  - Clock frequency (100 MHz, 200 MHz, 400 MHz)
- **Dimensionality**: Number of independent design parameters
- **Size**: Product of all parameter options (can be exponentially large)

**Objective Space:**
- **Output**: Quality metrics evaluated for each design
- **Examples**:
  - Execution time (seconds)
  - Energy consumption (Joules)
  - Area (mm²)
  - Cost ($)
- **Dimensionality**: Number of objectives to optimize

**Mapping:**
Each point in decision space maps to ONE point in objective space through design evaluation (analysis/simulation/measurement).

**Critical Understanding:**
- Multiple decision space points may map to the same objective space point (different implementations with same performance)
- Decision space exploration seeks to find objective space Pareto front

### 3. Pareto-Dominance and Optimality

**Dominance Definition (for minimization):**
Design A dominates design B if:
1. A is **better or equal** in ALL objectives
2. A is **strictly better** in AT LEAST ONE objective

Mathematically: A ≻ B if f_i(A) ≤ f_i(B) for all i, and f_j(A) < f_j(B) for at least one j

**Pareto-Optimal Design:**
A design is Pareto-optimal if **no other design dominates it**.

**Key Implication:**
- For a Pareto-optimal design, improving one objective REQUIRES worsening at least one other objective
- These designs represent fundamental trade-offs
- The set of all Pareto-optimal designs forms the **Pareto front**

**Visual Understanding:**
In a 2D objective space (e.g., energy vs performance):
- Pareto front is the "boundary" of achievable designs
- Points below/left of Pareto front are not achievable
- Points above/right of Pareto front are dominated (suboptimal)

### 4. Architecture Template Approach

**Concept:**
An architecture template is a parameterized system structure that defines a family of related designs.

**Template Components:**
1. **Resource Types**: Available hardware components (processors, memories, buses, accelerators)
2. **Topology**: Interconnection structure between components
3. **Parameters**: Configurable aspects (counts, sizes, speeds)

**Example - Network Processor Template:**
- Parameter 1: Number of pipeline stages (1-8)
- Parameter 2: Processing element type per stage (RISC, DSP, ASIC)
- Parameter 3: Memory configuration (size, organization)
- Result: Template generates 1000s of possible architectures

**Advantages:**
- Structured exploration of related designs
- Captures domain-specific knowledge
- Enables systematic parameter sweeping
- Facilitates automated design generation

**Limitations:**
- Template structure may exclude innovative alternatives
- Requires upfront definition of parameter space

### 5. Allocation, Binding, and Scheduling

**The Three-Stage Mapping Process:**

**1. Allocation:**
- **What**: Select hardware resources (processors, memories, communication links)
- **Decision**: Which resource types and how many instances?
- **Example**: Allocate 3 RISC processors, 2 DSPs, 1 shared bus

**2. Binding:**
- **What**: Assign application tasks to allocated resources
- **Decision**: Which task executes on which processor?
- **Example**: Bind encryption task to DSP, bind control logic to RISC processor

**3. Scheduling:**
- **What**: Determine execution order and timing of tasks
- **Decision**: When does each task start and finish?
- **Example**: Task A at t=0-10ms, Task B at t=10-15ms, Task C at t=5-20ms (parallel with A)

**Interdependencies:**
- Binding choices constrain scheduling options
- Scheduling analysis may reveal need for more allocation
- Iterative refinement often necessary

**Optimization Space:**
- Each combination of allocation + binding + scheduling is a design point
- DSE explores this combined space to find Pareto-optimal solutions

### 6. Design Evaluation Methods

**Three Fundamental Approaches:**

**1. Analysis (Mathematical Modeling):**
- **Method**: Use equations to calculate performance/energy
- **Advantages**: Very fast, available early in design process
- **Disadvantages**: Low accuracy for complex systems, requires simplifying assumptions
- **Example**: Calculate execution time = (instruction count) × (CPI) × (clock period)
- **Use Case**: Early exploration of large design spaces

**2. Simulation:**
- **Method**: Execute a model that mimics system behavior
- **Advantages**: Medium accuracy, reasonable speed, widely applicable
- **Disadvantages**: Requires detailed models, slower than analysis
- **Types**: Instruction-set simulation, cycle-accurate simulation, transaction-level modeling
- **Example**: SystemC model simulating processor and bus activity
- **Use Case**: Mid-stage exploration with moderate detail

**3. Measurement:**
- **Method**: Build prototype and measure actual behavior
- **Advantages**: Highest accuracy, reveals real-world issues
- **Disadvantages**: Slow, expensive, available only late in design
- **Example**: Power meter measuring FPGA prototype energy consumption
- **Use Case**: Final validation and fine-tuning

**Typical DSE Flow:**
1. Early: Use analysis for broad exploration
2. Middle: Use simulation for promising candidates
3. Late: Use measurement for final validation

### 7. V-Model Integration with DSE

**Where DSE Fits in V-Model:**

DSE is primarily applied during the **System Design** and **System Integration** phases:

**Left Side (Specification and Design):**
- **System Requirements**: Define objectives and constraints for DSE
- **System Design**: Perform DSE to select architecture
- **HW/SW Architecture**: Perform detailed DSE for partitioning decisions
- **Implementation**: May use local DSE for component optimization

**Right Side (Integration and Testing):**
- **System Integration**: Validate DSE assumptions with measurements
- **System Testing**: Verify that selected design meets objectives
- **Iteration**: If objectives not met, return to DSE with refined constraints

**Key Insight:**
DSE is NOT a one-time activity. It involves:
- Initial exploration to select baseline architecture
- Iterative refinement as more detailed information becomes available
- Validation that predictions match reality

### 8. Case Study Insights: Network Processor

**Problem Setup:**
Design a network processor for packet processing with:
- **Input**: Stream of network packets
- **Processing**: 6 operations (classification, routing, filtering, QoS, modification, forwarding)
- **Objectives**: Maximize throughput, minimize latency, minimize energy

**Architecture Template:**
- **Structure**: Pipeline with 6 stages (one per operation)
- **Decision Variables**:
  - Processing element type for each stage (RISC, DSP, or ASIC)
  - Memory organization
  - Bus configuration

**Key Trade-offs Discovered:**
1. **ASIC stages**: High throughput, low energy per packet, but high area/cost and inflexible
2. **RISC stages**: Low throughput, high energy per packet, but flexible and low cost
3. **DSP stages**: Medium performance, good for signal processing operations
4. **Hybrid solutions**: Mix of processor types offers good trade-offs

**Pareto Front Results:**
- Pure RISC: Lowest cost, lowest performance
- Pure ASIC: Highest performance, highest cost
- Mixed designs: Dominate pure solutions for many use cases
- Optimal depends on workload and priorities

**Practical Lesson:**
Heterogeneous architectures (mixing different processor types) often provide better trade-offs than homogeneous designs.

### 9. Case Study Insights: Cache Organization

**Problem Setup:**
Optimize cache memory configuration for a given application with objectives:
- Maximize hit rate (minimize miss rate)
- Minimize area (cache size)
- Consider access time

**Decision Variables:**
1. **Cache size**: 1 KB, 2 KB, 4 KB, 8 KB, 16 KB, 32 KB
2. **Line size**: 8 B, 16 B, 32 B, 64 B
3. **Associativity**: Direct-mapped (1-way), 2-way, 4-way, 8-way, fully associative

**Exploration Results:**
- **Increasing cache size**: Improves hit rate but increases area
- **Increasing line size**: May improve or worsen hit rate (depends on spatial locality)
- **Increasing associativity**: Generally improves hit rate but increases access time and area
- **Pareto front**: Reveals optimal configurations for different area budgets

**Key Findings:**
- Small line sizes (8-16 B) are often suboptimal (dominated by larger lines)
- High associativity (8-way, fully associative) provides diminishing returns
- Sweet spot often at 4 KB-16 KB size, 32 B lines, 2-way or 4-way associativity

**Practical Lesson:**
The optimal cache configuration is highly application-dependent. DSE reveals that intuition alone is insufficient; systematic exploration uncovers non-obvious optimal designs.

## Important Diagrams & Graphics

### 1. Multiobjective Optimization Concept
**Description for Redrawing:**
- Two axes: Energy (J) on x-axis, Performance (1/execution time) on y-axis
- Multiple scattered design points in the space
- Ideal point in top-left corner (low energy, high performance) - usually unachievable
- Pareto front as a curve connecting non-dominated points
- Dominated points shown in gray/faded, Pareto-optimal points highlighted
- Arrows showing trade-off direction along Pareto front
- Label regions: "Infeasible" (beyond Pareto front), "Dominated" (above/right), "Pareto-optimal" (on the front)

### 2. Decision Space to Objective Space Mapping
**Description for Redrawing:**
- **Left side**: Decision space with 2 parameters (e.g., # processors: 1,2,4; Memory: S,M,L)
- Grid showing all combinations (3×3 = 9 design points)
- **Arrow labeled "Evaluation"** pointing right
- **Right side**: Objective space (Energy vs Performance)
- Each decision point maps to one objective point
- Show that different decision points can map to similar objective points
- Highlight Pareto front in objective space

### 3. Pareto-Dominance Illustration
**Description for Redrawing:**
- 2D objective space (minimize Energy, minimize Execution Time)
- Plot 5-6 design points labeled A, B, C, D, E, F
- For point A:
  - Shade rectangular region to upper-right (dominated by A)
  - Shade rectangular region to lower-left (dominates A)
- Use different colors/patterns:
  - Point A in red
  - Points dominated by A in gray
  - Points dominating A in green
  - Points non-comparable to A in blue
- Add arrows showing dominance relationships

### 4. Pareto Front in 2D Objective Space
**Description for Redrawing:**
- Axes: Energy (x) and Execution Time (y), both to be minimized
- Plot 15-20 design points scattered across space
- Connect Pareto-optimal points with a stepped/curved line (Pareto front)
- Use different markers:
  - Pareto-optimal points: large filled circles
  - Dominated points: small hollow circles
- Add annotations:
  - "Best energy efficiency" (rightmost Pareto point)
  - "Best performance" (leftmost Pareto point)
  - "Balanced trade-off" (middle Pareto points)
- Show that moving along Pareto front trades energy for performance

### 5. Architecture Template for Network Processor
**Description for Redrawing:**
- **Top**: Packet input arrow
- **Middle**: Pipeline of 6 stages in sequence:
  - Stage 1: Classification
  - Stage 2: Routing
  - Stage 3: Filtering
  - Stage 4: QoS
  - Stage 5: Modification
  - Stage 6: Forwarding
- **Each stage**: Box with three options (RISC, DSP, or ASIC) shown as sub-boxes
- **Bottom**: Packet output arrow
- **Side**: Shared memory and interconnect
- Label parameters: PE type for each stage, memory size, bus width

### 6. Allocation, Binding, Scheduling Example
**Description for Redrawing:**
Three panels side-by-side:

**Panel 1 - Allocation:**
- Hardware resources: 2 processors (P1, P2), 1 memory (M), 1 bus (B)
- Show as boxes representing allocated components

**Panel 2 - Binding:**
- Application tasks: A, B, C, D, E (shown as circles)
- Arrows from tasks to processors: A→P1, B→P1, C→P2, D→P2, E→P2

**Panel 3 - Scheduling:**
- Timeline (horizontal axis)
- Two rows (P1 and P2)
- Gantt chart showing: P1: [A][B], P2: [C][D][E]
- Indicate communication events on bus between tasks

### 7. DSE in V-Model
**Description for Redrawing:**
- **Left descending side** (Specification):
  - System Requirements (top)
  - System Design ← **DSE Loop 1 (Architecture Selection)**
  - HW/SW Architecture ← **DSE Loop 2 (Partitioning)**
  - Detailed Design
  - Implementation (bottom)

- **Right ascending side** (Validation):
  - Unit Testing (bottom)
  - Integration Testing
  - System Integration ← **DSE Validation**
  - System Testing
  - Acceptance Testing (top)

- Add arrows showing DSE iterations between left and right sides
- Label "Specification & Exploration" on left, "Integration & Validation" on right

### 8. Network Processor Pareto Front
**Description for Redrawing:**
- Axes: Energy per packet (x-axis, mJ), Throughput (y-axis, Mpps)
- Plot different design points with labels:
  - "Pure RISC" (low energy, low throughput)
  - "Pure ASIC" (high energy, very high throughput)
  - "Pure DSP" (medium on both)
  - Multiple "Mixed" designs (1 RISC + 5 ASIC, 2 RISC + 4 ASIC, etc.)
- Draw Pareto front connecting non-dominated designs
- Show dominated designs in gray
- Add annotations for key trade-off regions

### 9. Cache Organization Design Space
**Description for Redrawing:**
- **3D representation** (or multiple 2D slices):
  - X-axis: Cache size (1, 2, 4, 8, 16, 32 KB)
  - Y-axis: Line size (8, 16, 32, 64 B)
  - Z-axis: Associativity (1, 2, 4, 8, Full)
- Show design points as dots
- Color code by miss rate (blue=low, red=high)
- Indicate Pareto-optimal designs with larger markers
- Include 2D projection showing miss rate vs area trade-off

### 10. Design Evaluation Methods Comparison
**Description for Redrawing:**
Three columns (Analysis, Simulation, Measurement):

**Row 1 - Method**:
- Analysis: Equations and formulas
- Simulation: Computer with model
- Measurement: Physical prototype with instruments

**Row 2 - Speed**:
- Analysis: Fast (seconds)
- Simulation: Medium (minutes to hours)
- Measurement: Slow (hours to days)

**Row 3 - Accuracy**:
- Analysis: Low to Medium
- Simulation: Medium to High
- Measurement: Very High

**Row 4 - Availability**:
- Analysis: Early (specification stage)
- Simulation: Middle (design stage)
- Measurement: Late (prototype stage)

**Bottom**: Arrow showing typical DSE flow from Analysis → Simulation → Measurement

### 11. Performance Model for Packet Processing
**Description for Redrawing:**
- **Top**: Packet arrival with rate λ (packets/second)
- **Middle**: Network processor with pipeline stages
  - Each stage has processing time T_i
  - Show packet flow through stages
- **Equations**:
  - Latency = Σ T_i (sum of all stage times)
  - Throughput = 1 / max(T_i) (limited by slowest stage)
- **Gantt chart** showing pipelined execution:
  - Packet 1 through all stages
  - Packet 2 starting as Packet 1 advances
  - Illustrate steady-state throughput

### 12. Energy Model Components
**Description for Redrawing:**
Flow diagram:
- **Input**: Application workload (operations, data)
- **Allocation**: Hardware components with power characteristics
- **Binding**: Task-to-resource mapping
- **Scheduling**: Timing of task executions
- **Calculation**: E_total = Σ (P_i × t_i) for all active resources i
- **Breakdown**: Show static power (idle) vs dynamic power (active)
- **Output**: Total energy consumption (Joules)

### 13. Iterative DSE Process Flow
**Description for Redrawing:**
Circular flow diagram:
1. **Specification** (top): Define objectives, constraints
   ↓
2. **Initial Exploration** (right): Broad search, low accuracy
   ↓
3. **Candidate Selection** (bottom): Choose promising designs
   ↓
4. **Detailed Evaluation** (left): Simulation/measurement
   ↓
5. **Refinement** (top-left): Adjust parameters
   ↓
6. Decision diamond: "Objectives met?"
   - No → Return to step 2 with refined search
   - Yes → Exit to Implementation

Label the outer loop "Exploration Iterations"

### 14. Design Constraint Visualization
**Description for Redrawing:**
- 2D objective space (Energy vs Latency)
- Scatter plot of design points
- Draw constraint boundaries:
  - Vertical line: Energy ≤ E_max
  - Horizontal line: Latency ≤ L_max
  - Shaded region: Feasible space (bottom-left)
- Highlight designs:
  - In feasible space (green)
  - Violating constraints (red)
  - On Pareto front AND feasible (dark green, best solutions)

## Formulas & Procedures

### 1. Pareto-Dominance (Minimization)

**Strict Dominance:**
```
Design A strictly dominates design B (A ≻ B) if:
  ∀i: f_i(A) ≤ f_i(B)  (A is better or equal in ALL objectives)
  ∃j: f_j(A) < f_j(B)  (A is strictly better in AT LEAST ONE objective)

Where f_i is the i-th objective function (smaller is better)
```

**Weak Dominance:**
```
Design A weakly dominates design B if:
  ∀i: f_i(A) ≤ f_i(B)  (A is better or equal in ALL objectives)

Note: A may equal B in all objectives
```

**Pareto-Optimal Set:**
```
P = {A ∈ X | ¬∃B ∈ X : B ≻ A}

P = set of all designs A in decision space X
    such that no other design B dominates A
```

**For Maximization Objectives:**
Flip the inequality directions:
```
A ≻ B if:
  ∀i: f_i(A) ≥ f_i(B)  (A is better or equal in ALL objectives)
  ∃j: f_j(A) > f_j(B)  (A is strictly better in AT LEAST ONE objective)
```

### 2. Performance Metrics for Network Processor

**Latency (per packet):**
```
L = Σ(i=1 to n) T_i

Where:
  L = total packet latency
  n = number of pipeline stages
  T_i = processing time of stage i
```

**Throughput (steady state):**
```
Θ = 1 / max(T_i)  for i = 1 to n

Where:
  Θ = throughput (packets/second)
  T_i = processing time of stage i

Bottleneck: Stage with maximum T_i limits throughput
```

**Pipeline Efficiency:**
```
η = (Ideal Throughput) / (Actual Throughput)
  = (n / Σ T_i) / (1 / max(T_i))
  = (n × max(T_i)) / Σ T_i

Where:
  η = 1 means perfectly balanced pipeline
  η < 1 indicates imbalanced stages
```

### 3. Energy Calculation

**Total Energy:**
```
E_total = Σ(i=1 to m) (P_i × t_i)

Where:
  m = number of hardware resources
  P_i = power consumption of resource i
  t_i = active time of resource i
```

**Power Breakdown:**
```
P_total = P_static + P_dynamic

P_static = Leakage power (always consumed when on)
P_dynamic = α × C × V² × f

Where:
  α = activity factor (0 to 1)
  C = capacitance
  V = supply voltage
  f = clock frequency
```

**Energy-Delay Product (EDP):**
```
EDP = E × D

Where:
  E = energy consumption
  D = delay (execution time)

Lower EDP indicates better energy efficiency
Used to compare designs with different energy/performance trade-offs
```

### 4. Cache Performance Metrics

**Miss Rate:**
```
Miss Rate = (Number of Misses) / (Total Accesses)

Hit Rate = 1 - Miss Rate
```

**Average Memory Access Time (AMAT):**
```
AMAT = T_cache + (Miss Rate × T_miss_penalty)

Where:
  T_cache = cache hit time
  T_miss_penalty = additional time for cache miss
```

**Cache Size Calculation:**
```
Cache Size = (Number of Sets) × (Associativity) × (Line Size)

For direct-mapped: Associativity = 1
For n-way set-associative: Associativity = n
For fully associative: Number of Sets = 1
```

### 5. Design Space Size Calculation

**Total Design Points:**
```
|X| = Π(i=1 to n) |D_i|

Where:
  X = decision space
  n = number of design parameters
  D_i = domain (set of possible values) for parameter i
  |D_i| = cardinality of domain i
```

**Example:**
```
Parameter 1: Number of processors = {1, 2, 4, 8} → |D_1| = 4
Parameter 2: Memory size = {64KB, 128KB, 256KB} → |D_2| = 3
Parameter 3: Cache type = {Direct, 2-way, 4-way} → |D_3| = 3

Total design points = 4 × 3 × 3 = 36
```

### 6. Speedup and Efficiency

**Speedup (parallel execution):**
```
S = T_sequential / T_parallel

Where:
  T_sequential = execution time on 1 processor
  T_parallel = execution time on p processors

Ideal speedup: S = p (linear scaling)
```

**Parallel Efficiency:**
```
E = S / p = T_sequential / (p × T_parallel)

Where:
  p = number of processors
  E = 1 means perfect scaling
  E < 1 indicates parallelization overhead
```

**Amdahl's Law:**
```
S_max = 1 / (f_serial + (1 - f_serial)/p)

Where:
  f_serial = fraction of code that must execute serially
  p = number of processors

Shows that serial portions limit maximum achievable speedup
```

### 7. DSE Exploration Procedure

**Step-by-Step DSE Process:**

1. **Problem Formulation:**
   - Define objectives: O = {o_1, o_2, ..., o_k}
   - Define constraints: C = {c_1, c_2, ..., c_m}
   - Define decision variables: V = {v_1, v_2, ..., v_n}

2. **Design Space Definition:**
   - For each variable v_i, define domain D_i
   - Calculate design space size: |X| = Π |D_i|

3. **Initial Exploration:**
   - Select exploration strategy (exhaustive, heuristic, random sampling)
   - Generate candidate designs
   - Evaluate using fast methods (analysis, simple models)

4. **Filtering:**
   - Eliminate infeasible designs (violate constraints)
   - Identify dominated designs
   - Retain Pareto-optimal and near-optimal designs

5. **Detailed Evaluation:**
   - For promising candidates, use accurate evaluation (simulation, measurement)
   - Validate performance predictions

6. **Selection:**
   - Choose final design based on:
     - Position on Pareto front
     - Designer preferences/priorities
     - Risk and uncertainty considerations

7. **Validation:**
   - Implement selected design
   - Measure actual performance
   - If objectives not met, iterate from step 3 with refined search

## Comparisons & Contrasts

### 1. Decision Space vs Objective Space

| Aspect | Decision Space | Objective Space |
|--------|---------------|-----------------|
| **Nature** | Input parameters and configuration choices | Output quality metrics |
| **Dimension** | Number of design parameters | Number of objectives |
| **Points represent** | Specific design configurations | Performance/quality outcomes |
| **Size** | Often exponentially large | Typically bounded by physical limits |
| **Designer control** | Direct (choose parameter values) | Indirect (result of evaluation) |
| **Exploration** | Search for good parameter combinations | Visualize trade-offs, identify Pareto front |
| **Example axes** | # processors, memory size, algorithm choice | Energy, performance, area, cost |
| **Mapping** | Each decision point → one objective point | Multiple decision points may share objective point |

**Key Insight:** DSE searches decision space to discover the Pareto front in objective space.

### 2. Analysis vs Simulation vs Measurement

| Aspect | Analysis | Simulation | Measurement |
|--------|----------|------------|-------------|
| **Method** | Mathematical equations | Execute behavioral model | Observe real system |
| **Speed** | Very fast (seconds) | Medium (minutes-hours) | Slow (hours-days) |
| **Accuracy** | Low to medium | Medium to high | Very high |
| **Availability** | Early (spec phase) | Middle (design phase) | Late (prototype available) |
| **Cost** | Minimal | Low to medium | High (requires prototype) |
| **Coverage** | Depends on model | Depends on stimuli | Limited by test cases |
| **Flexibility** | Easy to change parameters | Medium (recompile model) | Difficult (redesign hardware) |
| **Best for** | Broad exploration | Candidate evaluation | Final validation |

**DSE Strategy:** Start with analysis for wide exploration, use simulation for promising designs, validate final choice with measurement.

### 3. Strict Pareto-Optimal vs Weakly Pareto-Optimal

| Aspect | Strictly Pareto-Optimal | Weakly Pareto-Optimal |
|--------|------------------------|----------------------|
| **Definition** | No other design is better in ANY objective | No other design is better in ALL objectives |
| **Improvement** | Any objective improvement requires worsening another | Another design may equal all objectives |
| **Uniqueness** | Represents fundamental trade-off | May have equivalent designs |
| **Mathematical** | ¬∃B: B ≥ A ∧ B ≠ A | ¬∃B: B > A (in all objectives) |
| **Practical meaning** | True optimum, cannot be improved | May have redundancy |
| **Selection** | Always prefer these | Consider eliminating duplicates |

**Typical Use:** Most DSE focuses on strictly Pareto-optimal designs since they represent non-redundant trade-offs.

### 4. Single-Objective vs Multiobjective Optimization

| Aspect | Single-Objective | Multiobjective |
|--------|------------------|----------------|
| **Goals** | One objective (e.g., minimize energy) | Multiple objectives (e.g., minimize energy AND latency) |
| **Optimal solution** | One globally best solution | Set of Pareto-optimal solutions (trade-offs) |
| **Comparison** | Simple: A < B or B < A | Complex: A may dominate, be dominated, or be incomparable to B |
| **Decision making** | Automatic (choose minimum/maximum) | Requires designer preference/priority |
| **Search complexity** | Find single optimum | Find Pareto front (many solutions) |
| **Typical methods** | Gradient descent, greedy algorithms | Evolutionary algorithms, Pareto-based search |
| **Result presentation** | Single value | Pareto front curve/surface |

**ESE Context:** Almost all embedded system design problems are multiobjective (performance, energy, cost, area).

### 5. Exhaustive Search vs Heuristic Search

| Aspect | Exhaustive Search | Heuristic Search |
|--------|-------------------|------------------|
| **Coverage** | Evaluates every design point | Evaluates subset of design space |
| **Optimality guarantee** | Guaranteed to find all Pareto-optimal | May miss some optimal designs |
| **Computation time** | O(N) where N = design space size | O(k) where k << N |
| **Scalability** | Poor (exponential space growth) | Good (polynomial or manageable) |
| **When feasible** | Small design spaces (< 10^6 points) | Large design spaces |
| **Examples** | Full enumeration, grid search | Genetic algorithms, simulated annealing, particle swarm |
| **Implementation** | Simple nested loops | Complex search logic |
| **Repeatability** | Deterministic | May be stochastic (different runs differ) |

**Practical Recommendation:** Use exhaustive for < 10,000 points, use heuristics for larger spaces.

### 6. Homogeneous vs Heterogeneous Architectures

| Aspect | Homogeneous | Heterogeneous |
|--------|-------------|---------------|
| **Resource types** | One type (e.g., all RISC processors) | Multiple types (RISC, DSP, ASIC, FPGA) |
| **Programming** | Simpler (uniform interface) | Complex (different programming models) |
| **Load balancing** | Easy (tasks are interchangeable) | Difficult (tasks have affinity) |
| **Performance** | Limited by single processor type | Can match tasks to optimal resources |
| **Energy efficiency** | Moderate | Higher (use specialized, efficient units) |
| **Cost** | Lower (economy of scale) | Higher (diverse components) |
| **Flexibility** | High (any task on any processor) | Medium (constrained by resource capabilities) |
| **Examples** | Multi-core CPU, GPU | SoC with ARM + DSP + GPU + NPU |

**DSE Finding:** Heterogeneous architectures often dominate homogeneous in Pareto front for performance and energy.

### 7. Hardware Implementation vs Software Implementation

| Aspect | Hardware (ASIC/FPGA) | Software (Processor) |
|--------|---------------------|---------------------|
| **Performance** | Very high (parallel, dedicated) | Moderate (sequential, shared) |
| **Energy efficiency** | Very high (optimized circuits) | Lower (instruction overhead) |
| **Development time** | Long (months) | Short (days-weeks) |
| **Development cost** | High (NRE for ASIC) | Low |
| **Flexibility** | Low (ASIC fixed) / Medium (FPGA reconfigurable) | High (software update) |
| **Area/size** | Small (dedicated logic) | Requires processor (larger) |
| **Verification** | Difficult (hardware bugs costly) | Easier (software testing) |
| **Best for** | Fixed algorithms, high volume | Changing requirements, low volume |

**DSE Decision:** Hardware/software partitioning is a key DSE problem - frequently executed, compute-intensive functions go to hardware; control and variable functions go to software.

## Examples & Applications

### 1. Traffic Light Controller DSE

**Problem:**
Design a traffic light controller for an intersection.

**Objectives:**
- Minimize average vehicle wait time
- Minimize energy consumption
- Minimize cost

**Decision Variables:**
- Sensor type: None (fixed timing), loop detector, camera
- Controller: Simple microcontroller, advanced processor
- Light type: Incandescent, LED
- Algorithm: Fixed timing, actuated, adaptive

**DSE Results:**
- **Design A**: No sensors, simple MCU, LED, fixed timing
  - Low cost ($500), medium energy, high wait time
- **Design B**: Camera, advanced processor, LED, adaptive
  - High cost ($5000), low energy (LED), very low wait time
- **Design C**: Loop detector, medium MCU, LED, actuated
  - Medium cost ($1500), low energy, low wait time (Pareto-optimal for balanced priorities)

**Lesson:** Even simple systems benefit from DSE to identify trade-offs.

### 2. Washing Machine Controller

**Problem:**
Design embedded controller for washing machine.

**Objectives:**
- Minimize manufacturing cost
- Minimize energy per wash cycle
- Maximize features/usability

**Decision Variables:**
- MCU type: 8-bit, 16-bit, 32-bit
- Memory size: 4 KB, 8 KB, 16 KB
- Motor control: Simple on/off, PWM, servo
- Sensor suite: Temperature only, temp + weight, temp + weight + conductivity

**Exploration Findings:**
- 8-bit MCU sufficient for basic control
- PWM motor control significantly improves energy efficiency vs on/off
- Weight sensor enables adaptive water/detergent use → energy savings exceed sensor cost
- 32-bit MCU enables WiFi connectivity (high-end market) but increases cost

**Pareto Front:**
- Low-end: 8-bit MCU, 4 KB memory, basic control ($5 controller, 1.2 kWh/cycle)
- Mid-range: 16-bit MCU, 8 KB memory, PWM + weight sensor ($12 controller, 0.8 kWh/cycle) [Best value]
- High-end: 32-bit MCU, 16 KB memory, full sensors + connectivity ($25 controller, 0.7 kWh/cycle)

### 3. Network Processor for Router

**Problem:**
Design packet processing pipeline for network router (1 Gbps throughput target).

**Operations:** Classification → Routing → Filtering → QoS → Modification → Forwarding

**Architecture Template:**
6-stage pipeline, each stage can be: RISC (50 MHz), DSP (200 MHz), or ASIC (custom)

**Characteristics:**
- RISC: 100 cycles/packet, 10 mW
- DSP: 40 cycles/packet, 30 mW
- ASIC: 5 cycles/packet, 5 mW (but high NRE cost)

**Exploration Results:**
- Pure RISC: 500 Kpps throughput, 60 mW → too slow
- Pure DSP: 5 Mpps throughput, 180 mW → meets throughput, high energy
- Pure ASIC: 40 Mpps throughput, 30 mW → overkill performance, high cost
- **Optimal hybrid**: RISC for classification/forwarding (flexible), ASIC for routing/filtering (critical path), DSP for QoS/modification (medium complexity)
  - Result: 5 Mpps, 90 mW, 30% lower cost than pure ASIC

**Key Insight:** Heterogeneous design dominates homogeneous solutions.

### 4. Cache Memory Configuration

**Problem:**
Optimize L1 cache for embedded processor running image processing application.

**Workload Characteristics:**
- Sequential image scanning (good spatial locality)
- Processes 640×480 images
- Each pixel: 3 bytes (RGB)

**Decision Variables:**
- Cache size: 1 KB, 2 KB, 4 KB, 8 KB, 16 KB, 32 KB
- Line size: 8 B, 16 B, 32 B, 64 B
- Associativity: 1-way (direct), 2-way, 4-way

**Simulation Results:**
- **8 B lines**: High miss rate (poor spatial locality exploitation)
- **64 B lines**: Lower miss rate for sequential access, but occasional wasted bandwidth
- **32 B lines**: Best miss rate for this workload
- **2 KB cache**: Miss rate 15%
- **4 KB cache**: Miss rate 8%
- **8 KB cache**: Miss rate 6% (diminishing returns)

**Pareto-Optimal Designs:**
1. 4 KB, 32 B line, 2-way (balanced)
2. 8 KB, 32 B line, 2-way (performance-oriented)
3. 2 KB, 32 B line, direct (area-constrained)

**Practical Choice:** 4 KB, 32 B, 2-way selected (good performance, reasonable area).

### 5. Smartphone SoC Architecture

**Problem:**
Design System-on-Chip for mid-range smartphone.

**Objectives:**
- Maximize performance (benchmark score)
- Minimize power (battery life)
- Minimize area (cost)
- Meet thermal constraint (< 5W peak)

**Decision Variables:**
- Application processors: 2, 4, or 8 cores
- GPU: Integrated, discrete low-end, discrete high-end
- DSP: None, single, dual
- Image processor: None, low-end, high-end
- Memory: LPDDR3, LPDDR4, LPDDR5

**DSE Approach:**
1. Initial sweep: 5×3×3×3×3 = 405 configurations
2. Filter: Eliminate designs violating thermal constraint (158 remain)
3. Evaluate: Use performance models and power models
4. Pareto analysis: 23 designs on Pareto front

**Selected Design:**
- 4 cores (quad-core)
- Discrete low-end GPU
- Single DSP
- Low-end image processor
- LPDDR4 memory

**Rationale:** This design is on Pareto front, balances performance and power, and has area within cost target.

### 6. Battery-Powered Sensor Node

**Problem:**
Design wireless sensor node for 5-year deployment on coin cell battery.

**Objectives:**
- Maximize battery life (target: 5 years)
- Minimize cost (target: < $20)
- Maintain sensing quality (sample rate ≥ 1 Hz)

**Decision Variables:**
- MCU: 8-bit @ 1 MHz, 16-bit @ 8 MHz, 32-bit @ 48 MHz
- Radio: Low-power (10 mW), medium (50 mW), high-power (100 mW)
- Sensing rate: 0.1 Hz, 1 Hz, 10 Hz
- Duty cycle: 1%, 5%, 10%, always-on
- Sleep mode: Deep sleep, light sleep, no sleep

**Energy Budget (CR2032 battery, 220 mAh @ 3V):**
E_total = 220 mAh × 3V × 3600 s/h = 2376 J
For 5 years: E_per_day = 2376 J / (5×365) = 1.3 J/day

**DSE Results:**
- Always-on → battery lasts 1 week (infeasible)
- 10% duty cycle → battery lasts 1.7 months (infeasible)
- 1% duty cycle + deep sleep + 1 Hz sensing + low-power radio → 5.2 years (feasible!)
- 32-bit MCU uses 3× more energy than 8-bit → 8-bit preferred

**Selected Design:**
- 8-bit MCU @ 1 MHz
- Low-power radio (10 mW)
- 1 Hz sensing rate
- 1% duty cycle (active 14.4 minutes/day)
- Deep sleep mode (3 μA idle)

**Validation:** Prototype measured 0.95 J/day → 6.8 year lifetime (exceeds target).

### 7. Autonomous Drone Flight Controller

**Problem:**
Design flight control system for delivery drone.

**Objectives:**
- Minimize latency (control loop < 10 ms)
- Minimize weight (flight time)
- Maximize safety (redundancy)
- Minimize cost

**Decision Variables:**
- Main processor: ARM Cortex-M4, Cortex-M7, Cortex-A53
- IMU sensor rate: 100 Hz, 500 Hz, 1 kHz
- Number of IMUs: 1, 2 (redundancy)
- Estimation algorithm: Simple complementary filter, Kalman filter, Extended Kalman Filter
- Motor control: 400 Hz PWM, 2 kHz PWM
- Battery monitoring: Voltage only, voltage + current

**Constraints:**
- Control loop latency < 10 ms (hard real-time)
- Total weight < 50 g

**DSE Process:**
1. Timing analysis:
   - M4 + EKF + 1 kHz IMU: 12 ms latency (infeasible)
   - M7 + EKF + 1 kHz IMU: 7 ms latency (feasible)
   - M4 + Kalman + 500 Hz IMU: 9 ms latency (feasible)

2. Weight analysis:
   - Single IMU: 35 g total
   - Dual IMU: 42 g total (feasible)

3. Safety analysis:
   - Single IMU: Sensor failure → crash
   - Dual IMU: Sensor failure → graceful degradation

**Pareto-Optimal Designs:**
- **Cost-optimized**: M4 + Kalman + 500 Hz + single IMU + voltage monitor (latency 9 ms, $45)
- **Safety-optimized**: M7 + EKF + 1 kHz + dual IMU + voltage+current monitor (latency 7 ms, $85)
- **Balanced**: M7 + Kalman + 500 Hz + dual IMU + voltage monitor (latency 6 ms, $65) [SELECTED]

**Implementation:** The balanced design was prototyped and tested successfully.

## Connections to Other Topics

### 1. Lecture 01 (Embedded Systems)
- **DSE applies embedded system characteristics**: Real-time constraints, resource limitations, energy constraints all become objectives or constraints in DSE
- **Cyber-physical systems**: DSE considers both computational and physical aspects (e.g., sensor/actuator selection)
- **Application domains**: DSE is essential for automotive, IoT, medical devices to meet diverse requirements

### 2. Lecture 02 (Software Paradigms)
- **Object-oriented design**: System models used in DSE often use OO representations (classes, inheritance)
- **Component-based design**: Architecture templates define reusable components and their composition
- **Concurrency**: Allocation, binding, and scheduling in DSE directly address concurrent task execution

### 3. Lecture 03 (Requirements)
- **Functional requirements**: Define what tasks must be allocated, bound, and scheduled in DSE
- **Non-functional requirements**: Translate directly to DSE objectives (performance, energy, cost)
- **Requirement conflicts**: DSE reveals trade-offs between conflicting requirements through Pareto fronts

### 4. Lecture 04 (Modelling)
- **System models**: UML, SysML models serve as input to DSE (define tasks, dependencies, data flows)
- **Performance models**: Essential for evaluation within DSE (analysis, simulation)
- **Abstraction levels**: DSE operates at architecture level, between high-level requirements and detailed implementation

### 5. Lecture 05 (Development Processes)
- **V-Model integration**: DSE is key activity during system design phase (left side of V)
- **Iteration**: DSE is inherently iterative, aligning with agile and spiral processes
- **Verification**: DSE validation requires measurement on right side of V-Model

### 6. Lecture 06 (Non-Functional Requirements)
- **Direct connection**: NFRs are the objectives in DSE (performance, energy, reliability, cost)
- **Quantification**: DSE requires quantitative NFR specifications (e.g., energy < 1 J, latency < 100 ms)
- **Trade-off analysis**: DSE reveals when NFRs conflict and quantifies trade-offs

### 7. Lecture 08 (Energy)
- **Energy as objective**: Energy minimization is primary objective in most embedded DSE problems
- **Energy models**: Models developed in Lecture 08 are used for evaluation in DSE
- **DVFS**: Voltage/frequency selection is a common DSE decision variable

### 8. Lecture 09 (Performance)
- **Performance metrics**: Throughput, latency, response time are key DSE objectives
- **Performance models**: Used extensively in DSE for evaluation
- **Amdahl's Law**: Guides DSE decisions about parallelization and multi-processor allocation

### 9. Lecture 10 (FPGA)
- **Hardware/software partitioning**: DSE determines which functions to implement in FPGA vs processor
- **FPGA parameters**: LUT count, block RAM, DSP slices are DSE decision variables
- **Reconfigurability**: Enables runtime DSE and adaptive systems

### 10. Lecture 12 (DSE Search Algorithms)
- **This lecture**: Provides foundation (problem definition, Pareto optimality)
- **Next lecture**: Covers search algorithms (genetic algorithms, simulated annealing) to efficiently explore spaces defined here
- **Methodology**: This lecture defines "what to find," Lecture 12 covers "how to find it"

### 11. Lecture 13 (RTOS Scheduling)
- **Scheduling in DSE**: Task scheduling is one of the three DSE mapping steps (after allocation and binding)
- **Real-time constraints**: Become constraints in DSE formulation
- **Priority assignment**: Can be a DSE decision variable

### 12. Lecture 14 (Multi-Processor Systems)
- **Allocation problem**: How many processors? What types? → DSE decision variables
- **Communication architecture**: Bus topology, NoC configuration → DSE design space
- **Load balancing**: Binding and scheduling in DSE address multi-processor load distribution

### 13. Labs (P1-P5)
- **P1 (Hardware Bringup)**: Platform characterization provides parameters for DSE models
- **P2 (Energy Measurement)**: Measurement techniques validate DSE energy predictions
- **P3 (AES Multi-Processor)**: Example of allocation (A53 vs R5) and binding (which AES implementation) DSE problem
- **P4 (AES Hardware)**: Hardware/software partitioning DSE (software AES vs FPGA vs CSU)
- **P5 (DSE)**: Direct application of DSE methodology to practical problem

### 14. Interdisciplinary Connections
- **Optimization theory**: Multiobjective optimization is foundation of DSE
- **Operations research**: Search algorithms for large design spaces
- **Control theory**: Co-design of controller and platform
- **Economics**: Cost-benefit analysis parallels DSE trade-off analysis
- **Decision theory**: Multi-criteria decision making under uncertainty

## Summary

**Design Space Exploration (DSE)** is the systematic methodology for finding optimal embedded system architectures by exploring the trade-offs between conflicting objectives. Key takeaways:

### Core Principles
1. **Multiobjective Nature**: Embedded systems have conflicting goals (performance vs energy vs cost vs area) with no single "best" solution
2. **Pareto Optimality**: The goal is to find the Pareto-optimal set of designs representing fundamental trade-offs
3. **Decision vs Objective Space**: DSE searches the decision space (design parameters) to discover the Pareto front in the objective space (quality metrics)

### Key Concepts
- **Pareto-dominance** defines when one design is objectively better than another
- **Architecture templates** provide structured parameterization of design spaces
- **Allocation, binding, and scheduling** are the three fundamental mapping decisions in system design
- **Analysis, simulation, and measurement** provide increasing accuracy at increasing cost for design evaluation

### Practical Application
- DSE is integrated into the V-Model during system design and validated during integration
- Iterative exploration refines the search based on increasingly detailed models
- Case studies (network processor, cache organization) demonstrate DSE uncovering non-obvious optimal designs
- Heterogeneous architectures often dominate homogeneous solutions on the Pareto front

### Critical Understanding
DSE is **not** about finding the single best design, but about:
1. **Discovering** the space of achievable performance combinations (Pareto front)
2. **Quantifying** trade-offs between conflicting objectives
3. **Enabling** informed decision-making based on designer priorities
4. **Identifying** dominated (suboptimal) designs to avoid

The next lecture (DSE Search) will cover algorithms to efficiently explore large design spaces without exhaustive enumeration.

## Possible Exam Questions

### Definition & Terminology (Basic Understanding)

1. What is Design Space Exploration (DSE)?
2. Define the decision space and explain what points in it represent.
3. Define the objective space and explain what points in it represent.
4. What is a design point?
5. What does "multiobjective optimization" mean?
6. Define Pareto-dominance for minimization objectives.
7. What is a Pareto-optimal design?
8. What is the Pareto-optimal set (Pareto front)?
9. Explain the difference between strict and weak Pareto-dominance.
10. What is an architecture template in DSE?
11. Define "allocation" in the context of system design.
12. Define "binding" in the context of system design.
13. Define "scheduling" in the context of system design.
14. What are the three main design evaluation methods in DSE?
15. What is hardware/software partitioning?
16. Define "design constraint" in DSE.
17. What is a feasible design?
18. What does "dominated design" mean?
19. Explain what a "resource type" is.
20. What is a processing element (PE)?

### Conceptual Understanding (Explain & Describe)

21. Explain why embedded system design is inherently a multiobjective problem.
22. Describe the relationship between decision space and objective space.
23. Explain why improving one objective often requires degrading another in Pareto-optimal designs.
24. Describe the concept of Pareto-dominance with a concrete example.
25. Explain how an architecture template helps structure the design space.
26. Describe the three-stage mapping process (allocation, binding, scheduling) with an example.
27. Explain the trade-offs between analysis, simulation, and measurement for design evaluation.
28. Describe how DSE integrates into the V-Model development process.
29. Explain the iterative nature of DSE.
30. Describe what the Pareto front represents in practical terms for a designer.
31. Explain why heterogeneous architectures often appear on the Pareto front.
32. Describe the relationship between design space size and number of parameters.
33. Explain how constraints affect the feasible design space.
34. Describe the role of models in DSE.
35. Explain why DSE is performed at the architecture level rather than detailed design level.

### Application & Analysis (Apply Concepts)

36. Given two designs A (energy=10J, time=5s) and B (energy=12J, time=4s), does either dominate the other?
37. If design A has energy=5J and time=10s, and design B has energy=5J and time=12s, which dominates?
38. Given three designs: A(10J, 5s), B(8J, 6s), C(12J, 4s). Identify which are Pareto-optimal.
39. A design space has 3 parameters: processors ∈ {1,2,4}, memory ∈ {32KB, 64KB}, cache ∈ {none, 4KB, 8KB}. How many design points exist?
40. For a network processor with 6 stages, if stage times are [10, 15, 12, 20, 8, 10] ms, what is the throughput?
41. For the same network processor, what is the latency per packet?
42. Which stage is the bottleneck in question 40?
43. If a cache has 256 sets, 4-way associativity, and 32-byte lines, what is the total cache size?
44. Given hit rate = 95% and miss penalty = 100 cycles, what is the average number of extra cycles per access?
45. Apply Pareto-dominance to three smartphone designs: A($200, 10h battery), B($250, 12h battery), C($220, 11h battery).
46. In a washing machine DSE, if adding a weight sensor costs $5 but saves $8 worth of energy over product lifetime, is it Pareto-optimal?
47. For a sensor node that must last 3 years on a 1000mAh battery at 3V, what is the daily energy budget?
48. If design space has 5 parameters with 4, 3, 2, 6, 5 options respectively, how many total designs exist?
49. Given speedup S=3.5 on 4 processors, what is the parallel efficiency?
50. Using Amdahl's law, if 20% of code is serial and you have 8 processors, what is maximum speedup?

### Comparison & Contrast (Analyze Differences)

51. Compare decision space and objective space in terms of what they represent.
52. Compare strict vs weak Pareto-dominance.
53. Compare analysis, simulation, and measurement in terms of speed and accuracy.
54. Compare homogeneous and heterogeneous architectures for embedded systems.
55. Compare hardware implementation vs software implementation for a compute-intensive algorithm.
56. Compare single-objective and multiobjective optimization problems.
57. Compare exhaustive search and heuristic search for DSE.
58. Compare allocation and binding in the system design process.
59. Compare the role of DSE in the specification phase vs integration phase of the V-Model.
60. Compare decision variables and objective functions in DSE.

### Case Studies & Examples (Practical Understanding)

61. In the network processor case study, why do mixed (heterogeneous) designs often dominate pure designs?
62. For the network processor, explain why using all ASIC stages might not be optimal despite highest performance.
63. In the cache organization case study, why might very small line sizes (8B) be dominated?
64. Explain why fully associative caches might be suboptimal despite best hit rates.
65. For the traffic light controller example, describe one Pareto-optimal design and its trade-offs.
66. In the washing machine controller, why might a weight sensor improve Pareto optimality?
67. For the smartphone SoC example, explain why a thermal constraint reduces the feasible design space.
68. In the battery-powered sensor node, why is deep sleep mode critical for achieving 5-year lifetime?
69. For the drone flight controller, explain the trade-off between single IMU and dual IMU designs.
70. In the network processor, if ASIC development cost is very high, how might this affect the optimal design choice?

### Process & Methodology (Procedural Knowledge)

71. Describe the step-by-step DSE process from problem formulation to final selection.
72. How do you determine if a design point is feasible in DSE?
73. Explain how to identify dominated designs in an objective space plot.
74. Describe the filtering process after initial exploration in DSE.
75. How do you select a final design from the Pareto-optimal set?
76. Explain the typical progression from analysis to simulation to measurement in DSE.
77. Describe how to validate DSE predictions with real measurements.
78. How do you handle constraint violations during DSE?
79. Explain the iterative refinement process in DSE.
80. Describe how to incorporate designer preferences into design selection from Pareto front.

### Formulas & Calculations (Quantitative Skills)

81. Write the mathematical definition of strict Pareto-dominance for minimization.
82. Write the formula for throughput in a pipelined system.
83. Write the formula for latency in a pipelined system.
84. Write the equation for total energy consumption given power and time for each resource.
85. Write the formula for Average Memory Access Time (AMAT).
86. Write the equation for design space size given n parameters.
87. Write the formula for speedup in parallel systems.
88. Write the equation for parallel efficiency.
89. Write Amdahl's Law formula.
90. Write the formula for cache size given sets, associativity, and line size.

### Critical Thinking & Trade-offs (Advanced Analysis)

91. Why might a design on the Pareto front still not be the best choice for a specific application?
92. Explain why DSE is necessary even for experienced designers with good intuition.
93. Discuss the trade-off between exploration thoroughness and time-to-market.
94. Why might the Pareto front change if we add a new objective (e.g., reliability)?
95. Explain how uncertainty in models affects DSE results and design decisions.
96. Discuss why hardware/software partitioning is rarely "all hardware" or "all software" for complex systems.
97. Why is it important to consider manufacturing cost separately from NRE (non-recurring engineering) cost in DSE?
98. Explain how the relative importance of objectives might change over a product's lifecycle.
99. Discuss the tension between design space size and exploration feasibility.
100. Why might a suboptimal design be chosen in practice despite DSE identifying better alternatives?

### Integration & Synthesis (Connect Concepts)

101. How does DSE relate to non-functional requirements analysis?
102. Explain how DSE integrates with the V-Model development process.
103. Describe the connection between DSE and performance modeling.
104. How do energy models from energy analysis lectures feed into DSE?
105. Explain the relationship between DSE and RTOS scheduling.
106. How does FPGA configurability affect DSE for hardware/software partitioning?
107. Describe how DSE can incorporate real-time constraints.
108. Explain how DSE results from simulations should be validated with measurements.
109. How does component-based design interact with architecture templates in DSE?
110. Describe the relationship between DSE and system-level modeling (UML/SysML).

### Practical Scenarios (Problem Solving)

111. You're designing a smartwatch. What objectives and constraints would you define for DSE?
112. For an automotive ECU with hard real-time constraints, how would you incorporate these into DSE?
113. You discover that your DSE predictions differ significantly from prototype measurements. What might be wrong?
114. Your design space has 10^12 points. What exploration strategy would you recommend?
115. A customer wants both maximum performance AND minimum energy. How do you explain the trade-off using Pareto concepts?
116. Your selected design fails thermal testing. How do you use DSE to find an alternative?
117. You need to reduce product cost by 20%. How can DSE help identify feasible design changes?
118. Two Pareto-optimal designs have nearly identical objective values. How do you choose between them?
119. Midway through development, a new energy regulation is announced. How does this affect your DSE?
120. You're asked to explain to management why you need 3 weeks for DSE. What's your justification?
