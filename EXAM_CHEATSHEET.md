# ESE Exam Cheat Sheet

## 1. Embedded Systems (Lecture 01)

**Key Terms:**
- **Embedded System (ES)**: Computer system part of larger system, controlling devices/facilities/processes. Must include processor engine, often specific purpose
- **Design Space**: Multi-dimensional space of all possible design choices. Primary dimensions: Energy, Cost, Performance. Extended: Security, Safety, Latency, Testability, Manufacturability, Configurability
- **RTOS**: Real-Time Operating System - designed for deterministic timing behavior and guaranteed deadline satisfaction
- **Hardware Accelerator**: Specialized hardware (FPGA, CSU) performing specific tasks faster than general CPU
- **APU (Application Processing Unit)**: ARM Cortex-A53, high-performance for general computing (Ultra96: quad-core 1.2GHz)
- **RPU (Real-Time Processing Unit)**: ARM Cortex-R5, optimized for real-time tasks (Ultra96: dual-core 500MHz)
- **CSU (Configuration Security Unit)**: Hardware for cryptographic operations and security
- **FPGA (Programmable Logic)**: Reconfigurable hardware for custom logic implementation
- **System Bus**: Communication pathway with data lines (transfer data), address lines (specify locations), control signals (coordinate operations)
- **Bare-metal Application**: Software running directly on hardware without OS
- **BOM (Bill of Materials)**: Complete list of hardware components needed for system
- **Zynq UltraScale+ MPSoC**: Multi-Processor System-on-Chip combining APU, RPU, GPU, CSU, and FPGA

**Six Properties of ES:**
1. Must include processor engine
2. May come without user interface
3. Designed for specific purpose (not general-purpose)
4. Often use RTOS
5. Built-in applications (no user-installable software)
6. Resource-limited (energy, memory, cost)

**Key Comparisons:**

*ES vs General-Purpose Computers:*
- ES: Specific purpose, minimal UI, fixed software, resource-constrained, often real-time critical
- PC: Multiple tasks, rich UI, user-installable apps, abundant resources, no real-time guarantees

*RTOS vs General-Purpose OS:*
- RTOS: Deterministic scheduling, guaranteed response times, minimal footprint
- General OS: Fair scheduling (non-deterministic), best-effort performance, large footprint

*Processor Types (Ultra96):*
- Cortex-A53 (APU): 1.2GHz quad-core → general computing, Linux
- Cortex-R5 (RPU): 500MHz dual-core → real-time tasks, deterministic control
- Mali-400 MP2 (GPU): graphics rendering
- FPGA: custom hardware acceleration, reconfigurable

*Design Space Trade-offs:*
- Low-Energy CPU: Excellent energy, low cost, limited performance
- 1 CPU: Low cost, simple design, moderate energy/performance
- 2 CPUs: High performance (parallel), higher cost, higher energy
- 1 CPU + FPGA: High performance for specific tasks, flexible, higher cost, moderate energy

**Important Diagrams:**
- **Design Space Triangle**: Three vertices (Energy-Cost-Performance) with design options positioned showing trade-offs. No solution optimizes all three simultaneously
- **Extended Design Space**: Spider/radar chart with 10 axes (Cost, Performance, Latency, Energy, Configurability, Manufacturability, Testability, Safety, Security)
- **ES Hardware Architecture**: CPU + Memory + I/O (sensors/actuators) connected via System Bus. Flow: Sensing → Processing → Acting
- **Ultra96 Architecture**: MPSoC containing PS (APU + RPU + GPU) and PL (FPGA fabric), 2GB LPDDR4, various I/O interfaces

**Critical Points:**
- **No single "good" solution** - every design involves trade-offs. Optimal depends on requirements/constraints
- **HW/SW Co-design required** - hardware and software decisions are interdependent, cannot optimize separately
- **Resource constraints fundamental** - unlike PCs, energy/memory/cost always limited
- **Systematic design flow**: Requirements → Modeling → Design Space Exploration → Implementation → Verification
- **Computer Engineering** bridges Microelectronics (bottom) and Computer Science (top) with increasing abstraction

## 2. Software Paradigms (Lecture 02)

**Key Terms:**
- **Software Paradigm**: Set of rules/models defining how software should be structured and developed
- **Imperative Programming**: Defines **HOW** to do something through step-by-step commands (C, C++, Java). Good control over timing
- **Declarative Programming**: Defines **WHAT** to do, leaving "how" to compiler (SQL, Prolog). Rare in ES, poor timing control
- **Procedural Programming**: Uses procedures/functions to break programs into small tasks. Data separated from procedures. Enables unit testing
- **Object-Oriented Programming (OOP)**: Data and procedures combined in objects. Class = template, Object = instance, Inheritance = classes inherit from superclasses, Encapsulation = objects contain their data
- **Event-Driven Programming**: Program flow determined by events (periodic, sporadic, one-shot). Event Queue collects events, Event Handler processes them
- **Time-Driven Programming**: Timer defines fixed ticks for timing. All tasks scheduled based on time. Deterministic but inflexible
- **Task Descriptor List (TDL)**: Table with cyclic schedule over period T. Created offline. Contains all activities with priorities
- **WCET (Worst-Case Execution Time)**: Maximum time a task can take to execute. Critical for schedulability
- **Period (T)**: Time interval after which cyclic schedule repeats
- **Frame (f)**: Smallest time tick in scheduler, provides finer granularity
- **Non-Preemptive Scheduling**: Handlers don't interrupt each other
- **Preemptive Scheduling**: Handlers can interrupt each other; requires context saving (stack frames)
- **Side Effect**: Function modifies state outside local scope (e.g., loop counter)
- **Cyber-Physical System**: Interaction between physical world and computation (Sensors → Computer → Actuators)

**Formulas:**
- **Schedulability (Simple Periodic)**: Σ WCET(hᵢ) ≤ T (sum of all worst-case execution times must fit within period)
- **Major Period for Generic Scheduler**: T = LCM(all task periods)
- **Frame Selection**: f must divide all periods, f ≥ WCET(largest task), f ≤ shortest period

**Key Comparisons:**

*Imperative vs Declarative:*
- Imperative: HOW to do (step-by-step), good timing control, common side effects, many lines, used in ES
- Declarative: WHAT to do (rules), poor timing control, avoids side effects, concise, rare in ES

*Procedural vs Object-Oriented:*
- Procedural: Data separate from functions, simpler, lower overhead, easy unit testing → device drivers, simple control
- OOP: Data encapsulated with objects, inheritance/polymorphism, higher overhead, complex → systems with many similar objects

*Event-Driven vs Time-Driven:*
- Event-Driven: Triggered by events (buttons, interrupts), responsive, efficient CPU (sleep when idle), unpredictable timing, risk of missed deadlines → UI, sporadic events
- Time-Driven: Triggered by timer, deterministic timing, must poll sensors, inflexible for sporadic tasks → motor control, safety-critical

*Non-Preemptive vs Preemptive Event Scheduling:*
- Non-Preemptive: Handlers don't interrupt, simpler, no context saving, slower response → simpler systems, similar priorities
- Preemptive: Handlers interrupt each other, complex, requires stack frames & semaphores, fast response → different urgencies

*Simple Periodic vs Generic Time-Triggered:*
- Simple: All tasks same period T, run all every period, minimal design effort → all tasks naturally same period
- Generic: Different periods, complex offline TDL, better CPU utilization → tasks with different natural periods

**Important Diagrams:**
- **Programming Paradigm Hierarchy**: Root splits into Imperative (→ Structured → Procedural → [OOP, Event-Driven, Time-Driven]) and Declarative (→ Functional)
- **Event Queue System**: Sensors → Event Queue (event stack) → Event Handler → Action
- **Preemptive Timeline**: Shows contexts stacked (main, h1, h2, h3) with higher priority interrupting lower, contexts saved on stack
- **Simple Periodic Schedule**: All handlers (h1, h2, h3) run sequentially every period T
- **Generic Time-Triggered Schedule**: Frame f provides granularity, TDL defines which handlers run at each frame. Example: T=20ms, f=2ms, different tasks at different frames

**Critical Points:**
- **Imperative = HOW, Declarative = WHAT**
- **Procedural**: Data/functions separate, enables unit testing. Exception: static variables
- **OOP**: Natural for many similar objects. More overhead, may be overkill for resource-limited ES
- **Event-Driven**: Responsive, efficient, but unpredictable. Long handlers can delay others. Must protect shared resources in preemptive mode
- **Time-Driven**: Deterministic, provable WCET, but inflexible. Sensors must be polled. TDL created offline to avoid deadlocks
- **Hybrid approaches common**: E.g., time-driven core + event-driven peripherals + OOP structure
- **Hard Real-Time** (value = 0 if deadline missed): Airbag, ABS → needs time-driven or carefully analyzed event-driven
- **Soft Real-Time** (value degrades): Video, UI → can use event-driven
- **Paradigm selection part of design space**: Affects performance, energy, testability

## 3. Hardware Bringup / P1

**Key Terms:**
- **Cross-Compiler / Cross-Toolchain**: Compiler running on host system (x86-64) generating executables for different target system (ARM AArch64). Required when dev machine != embedded platform architecture
- **Host System**: Development computer where code written/compiled (Windows/Linux PC, x86-64)
- **Target System**: Embedded platform where compiled code executes (Ultra96, ARM Cortex-A53, AArch64)
- **Sysroot**: Directory containing target system's libraries, headers, files needed for cross-compilation. Allows cross-compiler to link against correct target libraries (~4.6GB for Ultra96)
- **Ultra96 / Zynq UltraScale+ MPSoC**: Dev board with System-on-Chip combining FPGA fabric + multiple ARM processors
- **AArch64**: 64-bit ARM architecture (ARM Architecture 64-bit), used in modern ARM processors like Cortex-A53
- **Processing System (PS)**: ARM Cortex-A53 processors running embedded Linux (fixed hardware, software)
- **Programmable Logic (PL)**: FPGA fabric for custom hardware acceleration (reconfigurable hardware)
- **USB Gadget Ethernet**: USB protocol creating network interface over USB connection. Board appears as network device to host at IP 10.0.0.1
- **SSH (Secure Shell)**: Encrypted protocol for secure remote access. Used to execute commands on Ultra96
- **SCP (Secure Copy Protocol)**: Protocol for secure file transfer using SSH. Used to transfer compiled executables to Ultra96
- **Makefile**: File containing rules for `make` build automation tool. Defines how to compile/link source code

**No formulas, but important procedures:**
- **Cross-compilation command**: `aarch64-none-linux-gnu-gcc --sysroot=$(SYS_ROOT)/cortexa72-cortexa53-xilinx-linux -mcpu=cortex-a53 -O0 -o main.elf src/main.c`
- **File transfer**: `scp -O main.elf ese@10.0.0.1:/home/ese/`
- **Remote connection**: `ssh ese@10.0.0.1` (password: ese)
- **Execute on target**: `chmod +x main.elf && ./main.elf`

**Key Comparisons:**

*Native vs Cross-Compilation:*
- Native: Compiler runs on target, same architecture, simpler setup, depends on target performance
- Cross: Compiler runs on host, different architecture, needs sysroot, faster builds, less target resources needed

*Host vs Target System:*
- Host: x86-64 (Intel/AMD), Windows/Linux desktop, high resources (GB RAM, fast CPU), full IDE/tools → development/compilation
- Target: ARM AArch64 (Cortex-A53), Embedded Linux, limited resources, basic shell → execution/deployment

*PS (Processing System) vs PL (Programmable Logic):*
- PS: Fixed ARM CPU hardware, programmed with software (C/C++), sequential instructions, GHz clock, runs OS/control logic
- PL: Reconfigurable FPGA hardware, programmed with HDL (VHDL/Verilog), parallel circuits, custom interfaces, hardware acceleration

*SCP vs SSH:*
- SCP: File transfer, non-interactive, one-time operation, `scp main.elf ese@10.0.0.1:~/`
- SSH: Remote shell access, interactive persistent session, execute commands, `ssh ese@10.0.0.1`

**Critical Points:**
- **Development workflow cycle**: Edit (host) → Cross-compile (host) → Transfer (SCP) → Execute (SSH to target) → Test → Iterate
- **Why cross-compilation needed**: Embedded platform has different instruction set (ARM vs x86), lacks resources for native compilation, native would be extremely slow
- **Ultra96 components**: 4 USER LEDs (GPIO), User Button (SW1), Power Button (SW4, manual start), USB 3 Device Port (Gadget Ethernet), SD Card (boot/filesystem)
- **Boot sequence**: Press SW4 → FPGA Configuration ~5s (INIT LED red → DONE LED blue) → Linux Boot ~25s → SSH Ready. Total ~30 seconds
- **Fixed IP**: Ultra96 always at 10.0.0.1 via USB Gadget Ethernet. No separate network cable needed
- **Environment variables critical**: PATH (for cross-compiler binaries), SYS_ROOT (for target libraries). Must restart VS Code after setting
- **Compiler flags important**: `--sysroot` (target libraries), `-mcpu=cortex-a53` (optimize for processor), `-O0/-O1/-O2/-O3` (optimization levels)
- **Makefile benefits**: Automates cross-compilation, detects host OS (Windows vs Linux), finds all .c files automatically, tracks dependencies
- **Sysroot necessary**: Target uses different C library (glibc for AArch64), system calls differ between architectures, ABIs are architecture-specific
- **Login credentials**: Username `ese`, Password `ese`
- **Build targets**: `make all` (build), `make clean` (remove artifacts), `make test` (verify toolchain with `gcc -v`)

## 4. Requirements (Lecture 03)

**Key Terms:**
- **Requirement**: Documented physical/functional need that design/product aims to satisfy. Describes behavior/property ES shall achieve/implement
- **Functional Requirement**: What system should DO - behaviors, functions, capabilities. Ex: "ES shall beep when connected to battery"
- **Non-Functional Requirement**: How system should BE - qualities, constraints, characteristics. Ex: "ES shall weigh less than 100g"
- **RFC 2119 Requirement Levels**: MUST/SHALL (mandatory), MUST NOT/SHALL NOT (prohibited), SHOULD/RECOMMENDED (strong rec, exceptions possible), SHOULD NOT (avoid, exceptions possible), MAY/OPTIONAL (truly optional)
- **MoSCoW Principle**: Prioritization - Must have (critical), Should have (important not vital), Could have (nice-to-have), Won't have (out of scope)
- **UML State Machine**: Graphical notation for modeling reactive systems using states, transitions, events, actions. Extension of classical FSM
- **State**: Mode of operation where some invariant holds. Notation: rounded rectangle
- **Initial State**: Where state machine begins. Notation: filled black circle
- **Final State**: Termination of state machine. Notation: bullseye (filled circle with ring)
- **Transition**: Change from one state to another, triggered by events. Notation: arrow with label `event[guard]/action`
- **Event**: Interaction triggering transition. Types: Signal (async notification), Call (sync operation), Change (`when(condition)`), Time (`after(60s)`)
- **Guard**: Boolean condition controlling if transition can trigger. Notation: `[guard_condition]`
- **Action**: Atomic operation executed during transition/within state. Entry (`entry/action`), Exit (`exit/action`), Do (`do/action`)
- **Port**: Interface defining messages FSM can send/receive. Enables communication between FSMs
- **State Space Explosion**: Number of states grows exponentially, making simple FSMs impractical for complex systems

**No formulas, but key notation:**
- **Transition label**: `event[guard]/action`  (event triggers transition if guard true, action executes)
- **State internal**: `entry/action`, `exit/action`, `do/action`
- **Time event**: `after(duration)`  (e.g., `after(60s)`)
- **Change event**: `when(condition)`  (e.g., `when(temp > 80)`)

**10 Characteristics of Good Requirements:**
1. **Unambiguous**: Only one interpretation
2. **Testable**: Can verify if met. Avoid: secure, efficient, easy, user-friendly, quickly, etc., TBD, and/or
3. **Clear**: Concise, simple, precise
4. **Correct**: Reflects stakeholder needs, technically feasible
5. **Understandable**: All stakeholders comprehend
6. **Feasible**: Can be implemented with available resources
7. **Independent**: Can understand without referencing others
8. **Atomic**: Single concern, cannot meaningfully subdivide
9. **Necessary**: Essential, not redundant
10. **Implementation-Free**: Describes WHAT not HOW

**Requirements Set must be:** Consistent (no contradictions), Nonredundant (no duplicates), Complete (no missing info)

**Key Comparisons:**

*Functional vs Non-Functional:*
- Functional: System behavior (WHAT to do), actions/operations/functions → beep, display, calculate
- Non-Functional: System quality (HOW to be), constraints/properties → weight, speed, energy, reliability

*RFC 2119 Requirement Strengths:*
- MUST/SHALL: Absolute requirement, mandatory
- SHOULD: Strong recommendation, exceptions if justified
- MAY: Optional, vendor choice

*Event Types in UML:*
- Signal: Asynchronous notification
- Call: Synchronous operation `event_name()`
- Change: `when(condition)` becomes true
- Time: `after(duration)` elapsed. Only one timeout per state, started on entry, cancelled on exit

**Critical Points:**
- **Requirements prevent "tree swing problem"**: Each stakeholder (customer, developer, tester) has different interpretation without clear requirements
- **Requirements Engineering Process**: Elicitation → Analysis → Specification → Validation → Negotiation (iterative cycle)
- **State machines model reactive systems**: Systems that respond to signals from environment (smoke detector, ABS, autonomous car)
- **Transitions triggered by events**: State change when event occurs AND guard condition true, then action executes
- **Actions are atomic**: Cannot be interrupted. Entry/exit/do actions provide state behavior
- **Port-based communication**: Multiple FSMs communicate via ports (output messages = actions, input messages = events)
- **State space explosion problem**: Independent features multiply states (2 features with 2 states each = 4 total states, 5 features = 32 states). Solution: hierarchical states, concurrent regions (next lecture)
- **Self-transition**: Source = target state. Causes exit then re-entry of state
- **Only one time event per state**: Multiple timeouts need multiple states

## 5. Modelling - Advanced UML State Machines (Lecture 04)

**Key Terms:**
- **Composite State (Superstate)**: State containing substates, allows hierarchical organization. Substates inherit transitions from composite state
- **Orthogonal State/Region**: Multiple independent state machines running concurrently within composite state (separated by dashed lines)
- **Pseudo-states**: Special nodes controlling flow, not true states
- **Choice Pseudo-state**: Diamond for conditional branching based on guards (no event trigger). Guards must be mutually exclusive
- **History Pseudo-state**: Remembers last active substate. Shallow History (H) = same level only, Deep History (H*) = all nested levels
- **Entry/Exit Point**: Named entry/exit into/from composite state (circle on border, circle with X)
- **Fork/Join**: Horizontal bar. Fork splits one transition into multiple (entering orthogonal regions), Join merges multiple into one (synchronization)
- **Terminate**: Circle with X inside, terminates entire state machine
- **Model Checking**: Formal verification exhaustively checking if model satisfies specified properties
- **CTL (Computation Tree Logic)**: Branching-time temporal logic for specifying properties
- **Deadlock**: State where no transitions possible, system stuck
- **Livelock**: System changes states but makes no progress
- **Model-Based Code Generation**: Automatic source code generation from high-level models (UML)

**CTL Operators:**
- **EF φ** (Exists Finally): There exists path where φ eventually true
- **AF φ** (Always Finally): On all paths, φ eventually true
- **EG φ** (Exists Globally): There exists path where φ always true
- **AG φ** (Always Globally): On all paths, φ always true
- **AX/EX φ**: Next state has φ (All paths / Exists path)

**Key Comparisons:**

*Shallow vs Deep History:*
- Shallow (H): Remembers last active state at same level only. Nested substates start from initial
- Deep (H*): Remembers last active state at ALL nested levels. Full state restoration

*Fork vs Join:*
- Fork: 1 transition → multiple concurrent transitions (entering orthogonal regions simultaneously)
- Join: Multiple transitions → 1 (synchronization point, wait for all regions)

*Choice vs State Transition:*
- Choice: Conditional branch with NO event, guards evaluated immediately, deterministic (mutually exclusive guards)
- State Transition: Triggered by event, guard evaluated when event occurs

*Entry Point vs Direct Entry:*
- Entry Point: Named entry on border, encapsulates entry logic, cleaner for complex composite states
- Direct Entry: Transition directly to specific substate, simpler but exposes internal structure

**Critical Points:**
- **Hierarchical states solve state space explosion**: Grouping related states reduces total number of states/transitions
- **Concurrent regions model parallelism**: System in combination of states from all regions (e.g., "State A AND State B")
- **Inheritance**: Substates inherit transitions from composite state. Allows common transitions without duplication
- **Entry/Exit actions**: Entering composite → entry action of composite, then entry of substate. Exiting → exit of substate, then exit of composite
- **Fork creates concurrency, Join synchronizes**: Fork enters all orthogonal regions simultaneously, Join waits for all to reach join point
- **History enables "resume where left off"**: Useful for interrupted processes (e.g., phone call interrupts music playback)
- **Model checking verifies correctness**: CTL formulas express properties like "always safe" (AG safe), "eventually ready" (AF ready), "possible to reach goal" (EF goal)
- **Choice enforces determinism**: Unlike state with multiple outgoing transitions, choice requires mutually exclusive guards + [else]
- **Terminate vs Final State**: Terminate kills entire SM immediately, Final state completes normally
- **Model-based code generation**: Forward engineering (model → code), Reverse engineering (code → model), Round-trip (bidirectional sync)

## 6. Requirements & Modelling Lab (U01)

**Key Terms:**
- **Washing Machine State Machine**: Models wash program (IDLE → FILL_WATER → HEAT_WATER → WASCHEN_MAIN → WASCHEN_SLOW → EMPTY_WATER → SPIN_DRY → IDLE)
- **Events**: WASCHEN_BTN (user), SPIN_BTN (user), STOP_BTN (emergency), FLOATER_HIGH/LOW (water level), TEMPERATURE_HOT (water temp), TIME_OUT (timer), DOOR_OPENED/CLOSED
- **Actions**: door_lock(), door_unlock(), valve_open(), valve_close(), pump_on(), pump_off(), heater_on(), heater_off(), motor_on(direction, speed), motor_off()
- **Traffic Light System**: Requirements for safe intersection control - no conflicting greens, timing constraints, fairness, fail-safe mode
- **Peer Review**: Colleague examines requirements for clarity, completeness, correctness, testability

**Critical Points:**
- **Washing machine demonstrates reactive system**: Responds to sensors (floater, temperature), user buttons, timeouts
- **Safety requirements critical**: Door locked during operation, emergency stop halts immediately, fail-safe defaults
- **Timing requirements**: Minimum/maximum durations for phases, clearance intervals for traffic lights
- **Requirements iteration**: Write → review → refine based on feedback
- **Traceability**: Link requirements to states/transitions in model, then to code implementation

## 7. Development Processes (Lecture 05)

**Key Terms:**
- **Abstraction Levels**: System (components, tasks) → Module (processors, memory) → Block (registers, loops) → Logic (gates, instructions) → Device (transistors, machine code)
- **Specification**: Formal description of functional behavior at abstraction level
- **Refinement**: Moving from higher to lower abstraction level with more detail (top-down)
- **Verification**: Does things RIGHT (meets specifications correctly)
- **Validation**: Does RIGHT things (meets user needs)
- **Waterfall Model**: Sequential phases (Requirements → Design → Implementation → Test → Operation). No iteration, rigid
- **V-Model**: Extension of waterfall with explicit testing at each level. Left side: development, right side: verification/validation
- **Agile**: Iterative, incremental, flexible. Sprint = 2-4 weeks, deliver working increment
- **Scrum**: Agile framework with Product Backlog, Sprint Backlog, Daily Standup, Demo, Retrospective
- **CI/CD**: Continuous Integration (auto build/test after each commit) / Continuous Deployment (auto deploy to target)

**Key Comparisons:**
*Waterfall vs V-Model vs Agile:*
- Waterfall: Sequential, no iteration, late testing, rigid. Good for: well-defined, stable requirements
- V-Model: Sequential with verification at each level, earlier testing, still rigid. Good for: safety-critical, regulated industries
- Agile: Iterative, flexible, continuous feedback, working software each sprint. Good for: evolving requirements, innovation

**Critical Points:**
- **Top-down refinement**: Start high-level, progressively add detail
- **Verification vs Validation**: Build right (verification) vs Build right thing (validation)
- **CI/CD enables rapid iteration**: Automated build/test/deploy catches issues early
- **Hardware-in-the-Loop (HIL)**: Automated testing with actual hardware in CI pipeline
- **Abstraction manages complexity**: Hide details at each level, focus on relevant aspects

## 8. Non-Functional Requirements (Lecture 06)

**Key Terms:**
- **Non-Functional Requirement (NFR)**: Specifies WHAT system should BE, not what it should DO. Criteria for judging system (quality attributes, constraints)
- **Three NFR Categories**: Efficiency (energy, cost, performance, memory), Dependability (reliability, safety, maintainability), Security (confidentiality, integrity, availability)
- **Qualities vs Constraints**: Qualities = properties system should demonstrate, Constraints = limitations/standards to account for
- **Execution vs Evolution**: Execution = observable during operation (runtime), Evolution = observable during development
- **Quantifiable vs Qualifiable**: Quantifiable = numeric values (preferred, testable), Qualifiable = pass/fail assessment

**Key NFRs:**
- **Energy**: Power consumption, battery life, energy efficiency
- **Performance**: Response time, throughput, latency, execution time
- **Memory**: RAM usage, ROM/Flash capacity
- **Cost**: BOM cost, licenses, manufacturing
- **Reliability**: MTBF (Mean Time Between Failures), availability, failure rate
- **Safety**: Hazard prevention, fault tolerance, fail-safe defaults
- **Security**: Confidentiality (CIA triad), Integrity, Availability
- **Maintainability**: Ease of updates, bug fixes, modifications

**Critical Points:**
- **NFRs differentiate working from working WELL**: Functional = system works, NFRs = works efficiently, reliably, securely
- **Testability essential**: NFRs must be measurable/verifiable. Avoid vague terms: "fast", "secure", "user-friendly"
- **Trade-offs common**: Energy vs Performance, Cost vs Reliability, Security vs Usability
- **Design space driven by NFRs**: Energy-Cost-Performance triangle from Lecture 01
- **Document as SMART**: Specific, Measurable, Achievable, Relevant, Time-bound

## 9. Implementation Lab (U2) & Integration Lab (U3)

**Key Terms:**
- **Hardware Abstraction Layer (HAL)**: Software layer between application and hardware. Provides consistent API regardless of hardware details, enables portability
- **Mockup Functions**: Simulated implementations for testing without hardware. Same API as real functions
- **Event-Driven Architecture**: Software responds to events (buttons, sensors, timeouts). Event handler generates events, state machine processes them
- **State Machine Implementation**: FSM in C using enums for states/events, switch-case or table-driven execution
- **API (Application Programming Interface)**: Contract between modules defining function signatures, separating interface from implementation
- **Integration Testing**: Testing combined modules together after unit testing
- **System Testing**: Testing entire system as a whole, end-to-end functionality

**Critical Points:**
- **Modular structure**: Separate modules (main, SM, events, actions, HAL) with clear interfaces
- **HAL benefits**: Hardware changes don't affect application code, testable without hardware, portable
- **Table-driven SM**: 2D array mapping (current_state, event) → (next_state, action). Scales better than nested switch-case
- **U2 implements washing machine SM**: From UML model to C code with mockup hardware functions
- **U3 integrates with real hardware**: Replace mockups with actual CT1 board hardware drivers (GPIO, timers, sensors)

## 10. Energy (Lecture 08)

**Key Terms:**
- **Low Power vs Low Energy**: Power = V×I (Watts, instantaneous), Energy = Power × Time (Joules, total consumption over time)
- **Static Power**: Power when not switching, due to leakage currents. Increasingly dominant as transistors shrink
- **Dynamic Power**: Power during switching. Formula: **P = α×C×V²×F** (α=activity, C=capacitance, V=voltage, F=frequency)
- **Power Gating**: Cut-off power to inactive units. Reduces static power
- **DVS (Dynamic Voltage Scaling)**: Adjust voltage dynamically. Very effective (P ∝ V²)
- **DFS (Dynamic Frequency Scaling)**: Adjust frequency. Linear effect (P ∝ F), also called CPU throttling
- **Clock Gating**: Disable clock to unused peripherals. Reduces dynamic power
- **Low Power Modes (LPM)**: Different sleep states (LPM0-LPM4). Trade-off: power savings vs wake-up time
- **DPM (Dynamic Power Management)**: Strategy for transitioning between power states. Break-even time: when savings exceed transition cost
- **Energy Harvesting**: Collecting energy from environment (solar, vibration, RF). System runs only when sufficient energy

**Formulas:**
- **Dynamic Power**: P_dyn = α × C × V² × F  (α=switching activity, C=capacitance, V=voltage, F=frequency)
- **Static Power**: P_stat = V × I_leak  (leakage current)
- **Total Power**: P_total = P_dyn + P_stat
- **Energy**: E = P × t  (power times time)
- **Break-even time**: T_break-even = (E_shutdown + E_wakeup) / (P_active - P_sleep)

**Key Comparisons:**
*Memory Types (Energy):*
- DRAM: 1T1C, volatile, refresh needed, lower energy during idle
- SRAM: 6T, volatile, no refresh, higher active power but faster
- FeRAM: Non-volatile, low write energy, persistent
- MRAM: Non-volatile, 1T1R, magnetization-based

**Critical Points:**
- **Voltage has squared effect**: Reducing V by 50% reduces dynamic power by 75%
- **Static power now dominant**: After 2015, leakage exceeds dynamic power in modern processes
- **DPM must consider break-even**: Don't sleep if wake-up cost > savings. Depends on sleep duration
- **Low power ≠ low energy**: Can have high power but short time (low energy), or low power but long time (high energy)
- **Energy harvesting challenges**: Intermittent operation, need for clean restarts, variable availability

## 11. Energy Analysis Lab (P2)

**Key Terms:**
- **Energy Measurement**: Using tools (multimeter, oscilloscope, power profiler) to measure actual consumption
- **AES (Advanced Encryption Standard)**: Symmetric encryption algorithm. 128/192/256-bit keys, 128-bit blocks
- **Energy Profiling**: Measuring energy consumption of different implementations/algorithms
- **Optimization for Energy**: Modifying code/hardware to reduce energy consumption

**Critical Points:**
- **Measure don't assume**: Actual energy often differs from estimates. Measurement reveals hotspots
- **AES on different platforms**: Compare software (A53, R5) vs hardware (FPGA, CSU) implementations
- **Trade-offs visible**: Performance vs energy. Hardware accelerators often more energy-efficient for specific tasks
- **P2 measures AES energy**: Foundation for design space exploration in P5

## 12. Performance (Lecture 09)

**Key Terms:**
- **Performance**: How fast/efficiently system executes tasks. Measured by execution time, throughput, latency
- **Latency**: Time from input to output (response time)
- **Throughput**: Amount of work done per time unit (ops/sec, MB/s)
- **Profiling**: Measuring where program spends time. Identifies bottlenecks
- **Optimization**: Improving performance through algorithm changes, compiler flags, hardware acceleration
- **Amdahl's Law**: Speedup limited by serial portion. Speedup = 1 / ((1-P) + P/S) where P=parallelizable fraction, S=speedup of parallel part
- **Cache**: Fast memory close to CPU. Reduces memory access latency. L1 (smallest, fastest), L2, L3 (largest, slowest)
- **Cache Hit/Miss**: Hit = data in cache (fast), Miss = data not in cache (slow, fetch from RAM)
- **Compiler Optimization Levels**: -O0 (none), -O1 (basic), -O2 (moderate), -O3 (aggressive)

**Formulas:**
- **Execution Time**: T = Instructions × CPI × Clock Period  (CPI=cycles per instruction)
- **Amdahl's Law**: Speedup = 1 / ((1-P) + P/S)  (P=parallel fraction, S=speedup of parallel part)
- **Speedup**: Speedup = T_old / T_new  (how many times faster)
- **Efficiency**: Efficiency = Speedup / Number_of_Processors

**Critical Points:**
- **Measure first, optimize second**: Profiling identifies actual bottlenecks, not guessed ones
- **Amdahl's Law limits parallelism**: Even with infinite cores, speedup limited by serial portion. If 10% serial, max speedup = 10×
- **Cache crucial for performance**: Modern CPUs depend on cache. Cache-friendly code (locality) performs better
- **Compiler optimizations trade-offs**: -O3 faster but larger code, harder to debug. -O0 easier debug but slower
- **Optimization hierarchy**: Algorithm > Data structures > Compiler flags > Low-level tricks

## 13. AES Multi-Processor Lab (P3)

**Key Terms:**
- **Multi-Processor System**: System with multiple CPUs/cores for parallel execution
- **Thread**: Lightweight unit of execution sharing memory with other threads
- **Parallel Processing**: Executing multiple tasks simultaneously on different cores
- **Synchronization**: Coordinating threads to avoid race conditions (mutexes, semaphores)
- **Race Condition**: Bug where outcome depends on timing of events (threads accessing shared data)

**Critical Points:**
- **P3 implements AES on multiple cores**: APU (4× Cortex-A53) and RPU (2× Cortex-R5)
- **Parallel speedup**: Divide AES work across cores for faster execution
- **Synchronization overhead**: Threads must coordinate, adds overhead
- **Performance vs cores**: More cores ≠ linear speedup (Amdahl's Law applies)

## 14. FPGA Introduction (Lecture 10)

**Key Terms:**
- **FPGA**: Field-Programmable Gate Array. Reconfigurable hardware implementing custom digital circuits
- **LUT (Lookup Table)**: Memory-based logic element storing output for all input combinations. 6-input LUT = 64-bit truth table
- **CLB (Configurable Logic Block)**: Building block with LUTs, flip-flops, multiplexers
- **Bitstream**: Binary file loaded into FPGA to define functionality (configuration data)
- **Configuration Memory**: SRAM cells storing LUT contents, routing connections
- **Routing/Interconnect**: Programmable connections between CLBs via multiplexers
- **ASIC**: Application-Specific IC. Custom chip, faster/efficient than FPGA but not reconfigurable
- **Pipelining**: Overlapping operations, new data every clock cycle. Increases throughput
- **Parallelism**: Simultaneous execution using independent hardware resources
- **AXI Bus**: ARM bus protocol for on-chip communication between processor and FPGA
- **Memory-Mapped I/O**: Hardware registers accessed via memory addresses

**Key Comparisons:**
*FPGA vs CPU vs ASIC:*
- FPGA: Reconfigurable, parallel, spatial computation, moderate speed/power, field-programmable → flexible hardware acceleration
- CPU: Sequential, temporal computation, general-purpose, slower for specific tasks → general computing
- ASIC: Fixed design, fastest/most efficient, expensive NRE, high volume only → production after design finalized

*Hardcore vs Softcore Processor:*
- Hardcore: Fixed silicon (ARM Cortex-A53/R5), fast, efficient → Zynq PS
- Softcore: Implemented in FPGA fabric (MicroBlaze, RISC-V), flexible, uses FPGA resources → PL

**Critical Points:**
- **FPGA = custom hardware**: No instruction set, implements function directly in logic gates
- **LUT implements any function**: 6-input LUT can implement ANY 6-input boolean function (2^6 = 64 entries)
- **Configuration not programming**: Bitstream defines hardware structure, not software instructions
- **Pipelining key advantage**: FPGA processes new data every cycle. CPU waits for entire operation
- **Trade-off: flexibility vs efficiency**: FPGA slower than ASIC but reconfigurable. Faster than CPU for parallel tasks
- **AES in FPGA**: Parallel rounds, pipelined SubBytes/MixColumns → high throughput, deterministic latency

## 15. AES Hardware Lab (P4)

**Key Terms:**
- **Hardware Acceleration**: Using FPGA/CSU for faster execution than software
- **CSU (Configuration Security Unit)**: Dedicated crypto hardware on Zynq. Fixed AES implementation
- **FPGA AES Implementation**: Custom AES circuit in programmable logic
- **Deterministic Latency**: Guaranteed consistent execution time (FPGA benefit)

**Critical Points:**
- **P4 compares hardware AES**: FPGA implementation vs CSU vs software (A53/R5)
- **Hardware wins for throughput**: FPGA/CSU much faster than software for repeated operations
- **Fixed-function CSU**: Optimized but inflexible. FPGA customizable
- **Foundation for DSE (P5)**: Measurements feed into design space exploration

## 16. Design Space Exploration Intro (Lecture 11)

**Key Terms:**
- **Design Space Exploration (DSE)**: Systematic search through possible implementations to find optimal solutions
- **Design Point**: One specific implementation choice (e.g., "1 CPU + FPGA")
- **Objective**: Goal to minimize (cost, energy) or maximize (performance)
- **Multi-Objective Optimization**: Optimizing multiple conflicting objectives simultaneously
- **Pareto Optimal**: Solution where improving one objective worsens another. No dominated by any solution
- **Pareto Front**: Set of all Pareto-optimal solutions. Trade-off curve
- **Dominated Solution**: Solution worse in all objectives than another solution. Should be eliminated
- **Trade-off**: Improving one objective requires sacrificing another

**Critical Points:**
- **DSE automates design space navigation**: Instead of manual trial, systematically explore options
- **No single best solution**: Multiple Pareto-optimal solutions, choice depends on priorities
- **Multi-dimensional**: Energy-Cost-Performance (3D), or more dimensions (security, safety, etc.)
- **Pareto front shows trade-offs**: Decision makers choose from Pareto-optimal options based on requirements

## 17. Design Space Exploration Search (Lecture 12)

**Key Terms:**
- **Exhaustive Search**: Evaluate ALL possible design points. Guaranteed optimal but expensive (2^n combinations)
- **Heuristic Search**: Smart search using rules/patterns. Faster but may miss optimal
- **Genetic Algorithm**: Evolutionary approach. Population of solutions, selection, crossover, mutation. Iteratively improves
- **Simulated Annealing**: Probabilistic search inspired by metallurgy. Accepts worse solutions with decreasing probability
- **Gradient Descent**: Iterative optimization following steepest improvement direction
- **Search Space**: Set of all possible design configurations
- **Evaluation**: Measuring/simulating design point's objectives (energy, cost, performance)

**Key Comparisons:**
*Search Strategies:*
- Exhaustive: Complete, slow, guaranteed optimal → small design spaces only
- Genetic Algorithm: Fast, parallel, good for multi-objective → large spaces, multiple optima
- Simulated Annealing: Escapes local optima, probabilistic → complex landscapes
- Gradient Descent: Fast, greedy, local search → convex problems, single optimum

**Critical Points:**
- **Curse of dimensionality**: Search space grows exponentially (2^n for n binary choices)
- **Heuristics sacrifice optimality for speed**: May find good solution faster than exhaustive
- **Multi-objective needs special handling**: Can't optimize all simultaneously, find Pareto front instead
- **Evaluation bottleneck**: Measuring real hardware slow. Simulation/modeling accelerates DSE

## 18. Design Space Exploration Lab (P5)

**Key Terms:**
- **Normalization**: Scaling different metrics to comparable range (0-1). Uses max-norm: value / max_value
- **Pareto Dominance**: Solution A dominates B if A better/equal in all objectives AND strictly better in at least one
- **Pareto Analysis**: Identifying dominated solutions and Pareto front
- **Trade-off Analysis**: Comparing Pareto-optimal solutions to understand compromises

**Formulas:**
- **Max-Norm Normalization**: normalized_value = value / max_value  (scales to [0, 1], lower=better after normalization)
- **Dominance Check**: A dominates B if ∀i: A[i] ≤ B[i] AND ∃j: A[j] < B[j]  (for minimization objectives)

**Critical Points from P5 Data:**
- **Four AES implementations**: Cortex-A53 (software), Cortex-R5 (software), FPGA (hardware), CSU (hardware)
- **Three objectives**: Cost (normalized), Runtime (normalized), Code Size (normalized)
- **Pareto Front: FPGA and CSU**: Neither dominates the other. FPGA=0 runtime, CSU=0 cost
- **Dominated: A53 and R5**: Both worse than Pareto-optimal solutions in multiple objectives
- **Decision**: Choose FPGA for performance-critical, CSU for cost-sensitive applications

## 19. RTOS Scheduling (Lecture 13)

**Key Terms:**
- **RTOS**: Real-Time OS guaranteeing predictable timing and deadline satisfaction. Focuses on determinism over fairness
- **Task States**: Running (executing on CPU), Ready (waiting for CPU), Blocked (waiting for event/resource)
- **Context Switch**: Saving/restoring task state (registers, PC, SP). Overhead: hundreds of cycles
- **Preemption**: Forcibly interrupting running task for higher-priority task. Triggered by system tick
- **Priority**: Task importance. Higher priority tasks preempt lower. Can be static or dynamic
- **RMS (Rate Monotonic Scheduling)**: Static priority by period. Shorter period = higher priority
- **EDF (Earliest Deadline First)**: Dynamic priority by deadline. Closest deadline = highest priority
- **Utilization (U)**: Fraction of CPU time used. U = Σ(Ci/Ti) where Ci=execution time, Ti=period
- **Schedulability**: Whether all tasks can meet deadlines. RMS: U ≤ n(2^(1/n)-1), EDF: U ≤ 1
- **Priority Inversion**: Low-priority task holds resource needed by high-priority task. High blocked by low!
- **Priority Inheritance**: Temporary priority boost to task holding resource needed by higher-priority task

**Formulas:**
- **Utilization**: U = Σ(Ci/Ti)  (sum of execution_time/period for all tasks)
- **RMS Schedulability**: U ≤ n(2^(1/n)-1)  (n=number of tasks. n→∞: limit = ln(2) ≈ 0.69)
- **EDF Schedulability**: U ≤ 1  (optimal for single processor)
- **Response Time**: R_i = C_i + Σ(⌈R_i/T_j⌉ × C_j) for all higher-priority tasks j

**Key Comparisons:**
*RMS vs EDF:*
- RMS: Static priority (by period), simpler, pessimistic bound (U≤0.69), can fail even if U<1 → easier implementation
- EDF: Dynamic priority (by deadline), optimal (U≤1), complex, runtime overhead → theoretical best

*RTOS vs General-Purpose OS:*
- RTOS: Deterministic, predictable, minimal, priority-based, hard real-time → embedded, safety-critical
- General OS: Fair, best-effort, feature-rich, time-sharing, soft real-time → desktops, servers

**Critical Points:**
- **Task state transitions**: Running ↔ Ready (preempt/dispatch), Running → Blocked (wait), Blocked → Ready (event)
- **RMS simple but pessimistic**: Easy to implement, but wastes CPU (only 69% guaranteed schedulable)
- **EDF optimal for single CPU**: Can schedule up to 100% utilization, but complex
- **Priority inversion dangerous**: Mars Pathfinder failed due to this. Solution: priority inheritance protocol
- **Context switch overhead**: Must be included in schedulability analysis
- **WCET critical**: Schedulability analysis requires accurate worst-case execution times

## 20. Multi-Processor Systems (Lecture 14)

**Key Terms:**
- **UMA (Uniform Memory Access)**: All processors have equal access time to shared memory. Symmetric multi-processing
- **NUMA (Non-Uniform Memory Access)**: Processors have local memory (fast) and remote memory (slower). Access time varies
- **SMP (Symmetric Multi-Processing)**: Multiple identical processors sharing memory and I/O. UMA architecture
- **AMP (Asymmetric Multi-Processing)**: Different processor types with different roles (e.g., big.LITTLE, Zynq APU+RPU)
- **big.LITTLE**: ARM architecture with big (high-performance) and LITTLE (energy-efficient) cores. Task migrates based on load
- **Cache Coherence**: Ensuring all caches see consistent view of memory. Critical for shared-memory multiprocessors
- **MESI Protocol**: Cache coherence protocol. States: Modified, Exclusive, Shared, Invalid
- **CPU Migration**: Moving task from one processor to another. Useful for load balancing but has overhead (cache cold start)
- **Global Scheduling**: Single scheduler assigns tasks to any processor. Better load balancing
- **Partitioned Scheduling**: Each processor has own task set and scheduler. Simpler, no migration

**Key Comparisons:**
*UMA vs NUMA:*
- UMA: Equal access time, simpler programming, limited scalability → small systems (2-8 cores)
- NUMA: Non-uniform access, local=fast, scalable, complex programming → large systems (many cores)

*Global vs Partitioned Scheduling:*
- Global: Single queue, better load balance, complex, migration overhead → dynamic workloads
- Partitioned: Per-processor queues, simpler, no migration, potential imbalance → static workloads

*big.LITTLE vs Homogeneous:*
- big.LITTLE: Energy-efficient (LITTLE for light, big for heavy), asymmetric, migration needed → mobile/battery
- Homogeneous: All cores same, simpler scheduling, less efficient → general-purpose

**Critical Points:**
- **Cache coherence essential**: Without it, cores see stale data. MESI protocol maintains consistency
- **Amdahl's Law limits speedup**: Serial portion limits benefit of adding cores. P=90% parallel, S_max=10×
- **big.LITTLE for energy**: Migrate tasks between cores based on load. LITTLE cores save energy during light load
- **Zynq is AMP**: APU (A53 for Linux/apps) + RPU (R5 for real-time) + FPGA. Different roles, explicit communication
- **Migration overhead**: Cold cache after migration. Balance load benefit vs overhead
- **Partitioned simpler for RTOS**: Each core runs independent scheduler. Easier real-time analysis

