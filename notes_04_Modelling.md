# Lecture 04: Modelling (Advanced UML State Machines)

## Technical Terms & Definitions

### UML State Machine Extensions
- **Composite State**: A state that contains substates (nested states), allowing hierarchical structuring of state machines
- **Hierarchical State Machine**: State machine using composite states to organize substates in a hierarchy (parent-child relationship)
- **Orthogonal State/Region**: Multiple independent state machines running concurrently within a composite state (separated by dashed lines)
- **Concurrent State Machine**: State machine with multiple orthogonal regions executing simultaneously
- **Substate**: A state nested within a composite state
- **Superstate**: Another term for composite state (contains substates)

### Pseudo-states
- **Pseudo-state**: Special nodes in state machines that are not true states but control flow elements
- **Initial Pseudo-state**: Entry point marked with filled circle, indicates where state machine begins
- **Choice Pseudo-state**: Diamond symbol for conditional branching based on guard conditions (no events)
- **History Pseudo-state**: Remembers the last active substate when exiting a composite state
  - **Shallow History (H)**: Remembers only the last active state at the same level
  - **Deep History (H*)**: Remembers the last active state at all nested levels
- **Entry Point**: Named entry into a composite state (circle on the border)
- **Exit Point**: Named exit from a composite state (circle with X on the border)
- **Fork Pseudo-state**: Horizontal bar that splits one transition into multiple concurrent transitions (entering orthogonal regions)
- **Join Pseudo-state**: Horizontal bar that merges multiple transitions from orthogonal regions into one
- **Terminate Pseudo-state**: Circle with X inside, terminates the entire state machine execution

### Model Verification
- **Model Checking**: Formal verification technique that exhaustively checks if a system model satisfies specified properties
- **Temporal Logic**: Logic for reasoning about sequences of states over time
- **CTL (Computation Tree Logic)**: Branching-time temporal logic for specifying properties
- **Formula/Property**: Logical expression describing desired system behavior (e.g., "always eventually ready")
- **Verification**: Process of proving that a system satisfies its specification
- **Validation**: Process of checking if the system meets user needs (right system)
- **State Space**: All possible states and transitions a system can have
- **Deadlock**: State where no transitions are possible, system is stuck
- **Livelock**: System keeps changing states but makes no progress toward goal

### CTL Operators
- **EF φ** (Exists Finally): There exists a path where φ eventually becomes true
- **AF φ** (Always Finally): On all paths, φ eventually becomes true
- **EG φ** (Exists Globally): There exists a path where φ is always true
- **AG φ** (Always Globally): On all paths, φ is always true
- **AX φ** (Always neXt): On all paths, φ is true in the next state
- **EX φ** (Exists neXt): There exists a path where φ is true in the next state

### Model-Based Code Generation
- **Model-Based Code Generation**: Automatic generation of source code from high-level models (e.g., UML)
- **Round-Trip Engineering**: Synchronization between model and code (changes in one reflected in the other)
- **Forward Engineering**: Generating code from models
- **Reverse Engineering**: Generating models from code
- **Code Generator**: Tool that transforms models into executable code
- **IBM Rhapsody**: Commercial tool for model-based development and code generation
- **CIRO**: Extension for code generation from UML models

### Task and Hardware Concepts
- **Task**: Unit of work that needs to be scheduled for execution on a processor
- **Task Graph**: Directed acyclic graph showing tasks and their dependencies
- **Dependency**: Constraint where one task must complete before another can start
- **Independent Tasks**: Tasks that can execute in any order or concurrently
- **Scheduler**: Component that decides which task runs when on which processor
- **Hardware-Software Binding**: Assignment of software tasks to specific hardware processors
- **Multi-Processor System**: System with multiple CPUs/cores available for task execution
- **WCET (Worst-Case Execution Time)**: Maximum time a task can take to execute under any circumstances
- **BCET (Best-Case Execution Time)**: Minimum time a task takes to execute
- **Static Analysis**: Analyzing code without executing it to determine properties like WCET
- **Dynamic Analysis**: Analyzing code by executing it (measurement, simulation)

## Core Concepts

### Composite States - Hierarchical
- **Purpose**: Organize complex state machines by grouping related states together
- **Structure**: Outer composite state contains inner substates with their own transitions
- **Inheritance of Transitions**: Transitions from the composite state apply to all substates (substates inherit them)
- **Entry/Exit**: When entering composite state, initial pseudo-state determines which substate becomes active
- **Default Entry**: Transition to composite state border → follow initial pseudo-state to default substate
- **Direct Entry**: Transition directly to specific substate
- **Exiting**: Exit action of substate executes first, then exit action of composite state

### Composite States - Orthogonal (Concurrent)
- **Purpose**: Model concurrent/parallel behavior within one state
- **Structure**: Composite state divided into multiple regions by dashed lines
- **Concurrent Execution**: Each region has its own substates and operates independently
- **Fork/Join**:
  - Fork: One transition splits into multiple, activating substates in different regions simultaneously
  - Join: Multiple transitions from different regions merge into one (synchronization point)
- **Combined State**: System is in combination of states from all regions (e.g., "state A and state B")
- **Exit**: Transition from composite state exits all orthogonal regions simultaneously

### Pseudo-state Details

#### Choice Pseudo-state
- **Function**: Branching based on conditions (guard conditions)
- **No Event**: Different from state transitions - no triggering event needed
- **Guards**: Outgoing transitions have guard conditions [condition]
- **Evaluation**: Guards evaluated when choice is reached
- **Determinism**: Guards must be mutually exclusive (only one can be true)
- **Else**: Often has [else] guard for default case

#### History Pseudo-states
- **Shallow History (H)**:
  - Remembers last active direct substate
  - Only one level of memory
  - Default transition: Used if no history (first entry)
  - Example: Resume video game level where you left off

- **Deep History (H*)**:
  - Remembers last active state at all nested levels
  - Complete state configuration preserved
  - Restores entire nested state hierarchy
  - Example: Resume game exactly where you were (level + sub-location)

#### Entry/Exit Points
- **Purpose**: Provide named, controlled entry/exit to composite states
- **Encapsulation**: Hide internal structure of composite state
- **Multiple Entry Points**: Different entry points can lead to different substates
- **Multiple Exit Points**: Different exits for different completion scenarios
- **Transition Chain**: Outside transition → entry point → internal transition → substate

#### Fork and Join
- **Fork**:
  - Splits one transition into multiple
  - All target states in orthogonal regions become active simultaneously
  - Represented by thick horizontal bar with one incoming, multiple outgoing transitions

- **Join**:
  - Synchronization point waiting for multiple transitions
  - Only fires when all incoming transitions have occurred
  - Merges multiple transitions into one outgoing transition
  - Ensures coordinated exit from orthogonal regions

#### Terminate
- **Function**: Immediately terminates entire state machine execution
- **Symbol**: Circle with X inside
- **Effect**: All active states exit, state machine stops
- **Use Case**: Error conditions, completion, shutdown scenarios

### UML State Machine Rules

#### 1. Deterministic Behavior
- **Rule**: For any state and event, there must be at most one enabled transition
- **Purpose**: Prevent ambiguity about which transition to take
- **Implementation**: Guards must be mutually exclusive, or events must be different
- **Violation**: Having two transitions with same event and overlapping guards is forbidden

#### 2. Initial State Required
- **Rule**: Every composite state and the top-level state machine must have an initial pseudo-state
- **Purpose**: Define clear starting point for execution
- **Default Substate**: Initial transition points to the default substate that becomes active first

#### 3. No Infinite Completion Cycles
- **Rule**: Should avoid completion transitions (no event, guard only) that loop indefinitely
- **Completion Transition**: Transition without event, triggered automatically when state's activities complete
- **Danger**: Can cause infinite loop if guards always evaluate to true
- **Example**: State A →[true] State B →[true] State A (infinite loop)

#### 4. No Unreachable States
- **Rule**: Every state should be reachable from the initial state through some sequence of transitions
- **Purpose**: Dead states indicate design errors or missing transitions
- **Detection**: Graph reachability analysis from initial state

### Model Checking Process
1. **Model Creation**: Developer creates formal model of system (e.g., state machine)
2. **Property Specification**: Define properties to verify using temporal logic formulas
3. **Model Checker Input**: Feed model and formulas to model checker tool
4. **Exhaustive Search**: Tool explores entire state space systematically
5. **Verification Result**:
   - **Yes**: Property holds in all reachable states (with proof)
   - **No**: Property violated (with counterexample showing violation path)
6. **Counterexample Analysis**: If violated, examine counterexample to understand failure
7. **Model Refinement**: Fix model based on findings, repeat verification

### Model-Based Code Generation Workflow
1. **Modeling Phase**: Developer creates UML models (state machines, classes, etc.)
2. **Model Validation**: Check model for consistency, completeness, correctness
3. **Code Generation**: Tool automatically generates source code (C, C++, Java) from model
4. **Compilation**: Generated code is compiled with compiler toolchain
5. **Integration**: Generated code integrated with manually written code (if any)
6. **Testing**: Test generated code for functionality and performance
7. **Deployment**: Deploy executable to target system

**Advantages**:
- Higher abstraction level (easier to understand)
- Automatic code generation (reduced manual coding errors)
- Consistency between model and code
- Documentation automatically maintained
- Platform independence (generate for different targets)

**Challenges**:
- No interaction with manually written code (separation of concerns)
- Missing guidelines for complex systems
- Keeping models simple and maintainable
- Avoiding "one solution fits all" trap
- Tool dependency and learning curve
- Generated code may not be optimally efficient

### Tasks and Scheduling

#### Task Concept
- **Definition**: Unit of work that needs CPU time to execute
- **Examples**: Function call, process, thread, interrupt handler
- **Properties**:
  - Execution time (WCET, BCET)
  - Priority
  - Deadline
  - Dependencies on other tasks
  - Resource requirements

#### Task Graph
- **Structure**: Directed Acyclic Graph (DAG)
- **Nodes**: Represent tasks
- **Edges**: Represent dependencies (A → B means B waits for A)
- **Critical Path**: Longest path through graph, determines minimum execution time
- **Parallelism**: Independent tasks can execute concurrently
- **Scheduling**: Determines order and processor assignment for tasks

#### Hardware-Software Binding
- **Problem**: Assign tasks to processors in multi-processor system
- **Constraints**:
  - Processor capabilities (some tasks may require specific processor)
  - Dependencies must be respected
  - Load balancing across processors
  - Communication overhead between processors
- **Goal**: Minimize total execution time or meet deadlines
- **Example**: Task1 → CPU1, Task2 → CPU2, Task3 → CPU1

### WCET Analysis

#### Why WCET Matters
- **Real-Time Systems**: Must guarantee tasks complete within deadlines
- **Scheduling**: Need WCET to allocate time slots and check schedulability
- **Reliability**: Underestimating WCET can cause deadline misses and system failures

#### Challenges
- **Path Explosion**: Exponentially many execution paths through code
- **Hardware Complexity**: Caches, pipelines, branch prediction make timing unpredictable
- **Input Dependency**: Execution time depends on input values
- **Coin Stacking Analogy**: Just because you've never seen coins stack to 20cm doesn't mean it's impossible

#### WCET Determination Methods
1. **Real System Measurement**:
   - Run on actual target hardware
   - Measure execution times
   - Problem: May miss worst case, limited test coverage

2. **Simulation**:
   - Simulate system behavior
   - More control over scenarios
   - Problem: Simulation accuracy, still may miss worst case

3. **Measurement-Based**:
   - Execute with many test cases
   - Record maximum observed time
   - Add safety margin
   - Problem: No guarantee of true worst case

4. **Static WCET Analysis**:
   - Analyze code without executing
   - Model hardware behavior
   - Calculate upper bound mathematically
   - Advantage: Guaranteed safe upper bound
   - Problem: May be overly pessimistic (over-approximation)

## Important Diagrams & Graphics

### 1. Hierarchical Composite State Diagram
- **Outer Rectangle**: Composite state with name at top
- **Inner Rectangles**: Substates nested inside
- **Initial Pseudo-state**: Filled circle inside composite state
- **Default Transition**: Arrow from initial to default substate
- **Inherited Transition**: Arrow from composite state border affects all substates
- **Concept**: Shows how substates are organized hierarchically within parent state

### 2. Orthogonal Regions Diagram
- **Composite State**: Outer rectangle
- **Dashed Lines**: Divide composite state into separate regions (horizontal or vertical)
- **Each Region**: Has own initial pseudo-state and substates
- **Fork Symbol**: Thick horizontal bar, one incoming arrow splits into multiple outgoing arrows
- **Join Symbol**: Thick horizontal bar, multiple incoming arrows merge into one outgoing arrow
- **Concept**: Multiple state machines executing simultaneously in parallel

### 3. History Pseudo-state Diagram
- **H Symbol**: Circle with "H" inside (shallow history)
- **H* Symbol**: Circle with "H*" inside (deep history)
- **Default Transition**: Dashed arrow from H to default substate (used on first entry)
- **Resume Behavior**: When returning to composite state via H, last active substate is reactivated
- **Nested Example**: Show difference between H (only remembers top level) and H* (remembers all levels)

### 4. Choice Pseudo-state Diagram
- **Diamond Symbol**: Choice pseudo-state
- **Multiple Outgoing Transitions**: Each with guard condition [condition]
- **No Event**: Transitions from choice have guards but no triggering events
- **[else]**: Default transition if no other guard is true
- **Concept**: Decision point based on conditions evaluated at runtime

### 5. Entry/Exit Point Diagram
- **Entry Point**: Small circle on composite state border (labeled)
- **Exit Point**: Small circle with X on composite state border (labeled)
- **External Transition**: From outside state to entry point
- **Internal Connection**: Entry point connects to specific substate inside
- **Exit Connection**: Internal substate connects to exit point, then external transition continues
- **Concept**: Controlled access points for encapsulation

### 6. Task Graph Diagram
- **Nodes**: Boxes or circles representing tasks (labeled T1, T2, etc.)
- **Edges**: Arrows showing dependencies (T1 → T2 means T2 depends on T1)
- **Parallel Branches**: Multiple tasks at same level can execute concurrently
- **Critical Path**: Highlighted longest path through graph
- **Concept**: Visualizes task dependencies and parallelism opportunities

### 7. Hardware-Software Binding Diagram
- **Hardware Layer**: Shows CPU1, CPU2, etc. (processors)
- **Software Layer**: Shows tasks T1, T2, T3, etc.
- **Binding Arrows**: Connect tasks to assigned processors
- **Communication**: Dashed arrows between tasks on different processors
- **Concept**: Mapping of software tasks to hardware resources

### 8. CTL Formula Tree Diagram
- **Root**: Initial state
- **Branches**: All possible execution paths
- **Nodes**: System states along paths
- **Labels**: State properties (e.g., "ready", "error")
- **Path Quantifiers**: E (exists path) or A (all paths) annotations
- **Temporal Operators**: F (finally), G (globally), X (next) on edges
- **Concept**: Shows state space and how CTL formulas reason about paths

### 9. Model-Based Code Generation Workflow
- **Developer Icon**: Person creating UML model
- **UML Model**: State machine diagrams
- **Code Generator Tool**: Box with arrows
- **Generated Code**: Source files (C/C++/Java)
- **Compiler**: Generates executable
- **Target System**: Embedded device
- **Concept**: Flow from model to executable code

### 10. Video Game State Machine Example
- **Movement Region**: States for walking, running, idle (orthogonal region 1)
- **Action Region**: States for not attacking, attacking (orthogonal region 2)
- **Enchanted State**: Overlay state that modifies behavior
- **Fork/Join**: Show concurrent entry to movement and action
- **Concept**: Real-world example of orthogonal states in game design

### 11. Bank ATM State Machine Example
- **Idle State**: Waiting for customer
- **Card Inserted**: Customer authentication
- **PIN Entry**: With history (remembers if returning from transaction)
- **Transaction Types**: Withdrawal, deposit, balance inquiry (choice pseudo-state)
- **Exit Points**: Different exit paths for successful/failed transactions
- **Concept**: Practical application with history and choice pseudo-states

### 12. Washing Machine State Machine Example
- **Power Off/On**: Top level states
- **Wash Cycle Composite**: Contains substates (fill, wash, rinse, spin)
- **Pause**: Transitions to pause state with history
- **Resume**: Returns to exact point in cycle using deep history
- **Terminate**: Emergency stop terminates machine
- **Concept**: Household appliance with composite states and history

## Formulas & Procedures

### CTL Formula Examples

**Basic Formulas**:
- `EF ready` - There exists a path where system eventually reaches "ready" state
- `AF ready` - System always eventually reaches "ready" state (inevitable)
- `AG safe` - System always remains in "safe" states (safety property)
- `EG working` - There exists infinite path where system keeps "working" (liveness)
- `AG (request → AF response)` - Every request is always eventually answered
- `AG ¬deadlock` - System never reaches deadlock state

**Complex Formulas**:
- `AG (EF restart)` - From any state, there's always a path back to restart (reversibility)
- `AG (error → AX ¬error)` - Error state never repeats immediately next (no infinite error)
- `EF (state1 ∧ EF state2)` - Possible to reach state1 and then later state2
- `AG EF idle` - System always eventually returns to idle (no deadlock)

### Model Checking Procedure
1. **Define System Model** (M):
   - Create state machine or other formal model
   - Identify all states, transitions, events
   - Define initial state

2. **Define Property** (φ):
   - Write temporal logic formula describing desired behavior
   - Example: AG (request → AF response)

3. **Run Model Checker**:
   - Input: M, φ
   - Algorithm: Exhaustive state space exploration
   - Check if M ⊨ φ (model satisfies formula)

4. **Analyze Result**:
   - **If YES**: Property holds, system is correct for this property
   - **If NO**: Property violated, examine counterexample

5. **Iterate**:
   - Fix model based on counterexample
   - Re-verify
   - Continue until all properties satisfied

### WCET Analysis Static Method
1. **Control Flow Graph (CFG)**:
   - Represent program as graph of basic blocks
   - Nodes = basic blocks (sequential instructions)
   - Edges = control flow (branches, loops)

2. **Loop Bound Analysis**:
   - Determine maximum iteration count for each loop
   - Use annotations, static analysis, or profiling

3. **Basic Block Timing**:
   - Calculate execution time for each basic block
   - Consider processor pipeline, cache effects

4. **Path Analysis**:
   - Use Integer Linear Programming (ILP) or other techniques
   - Find longest path through CFG considering loop bounds
   - Account for infeasible paths

5. **WCET Calculation**:
   - WCET = sum of execution times along longest path
   - Add safety margins for uncertainty

### Task Scheduling Procedure
1. **Task Graph Construction**:
   - Identify all tasks
   - Determine dependencies (which tasks must precede others)
   - Create DAG representing tasks and dependencies

2. **Task Characterization**:
   - Measure or calculate WCET for each task
   - Identify priorities, deadlines, resource requirements

3. **Processor Allocation**:
   - Assign tasks to available processors
   - Respect dependencies and constraints
   - Balance load across processors

4. **Schedule Construction**:
   - Determine execution order for tasks on each processor
   - Ensure dependencies are satisfied
   - Minimize total execution time or meet deadlines

5. **Schedulability Analysis**:
   - Verify all deadlines can be met
   - Check for overload conditions
   - Validate schedule correctness

## Comparisons & Contrasts

### Hierarchical vs. Orthogonal Composite States

| Aspect | Hierarchical | Orthogonal |
|--------|-------------|-----------|
| **Purpose** | Organize related states in hierarchy | Model concurrent/parallel behavior |
| **Structure** | Nested substates, one active at a time | Multiple regions, one substate active per region |
| **Active States** | Exactly one substate active | Multiple substates active simultaneously (one per region) |
| **Inheritance** | Substates inherit transitions from parent | Each region independent, no inheritance between regions |
| **Entry/Exit** | Initial pseudo-state determines first substate | Fork enters multiple regions, join exits synchronously |
| **Use Case** | Refinement, decomposition, abstraction | Parallelism, independent concurrent behaviors |
| **Example** | Menu system (main menu → submenu → item) | Video game (movement + actions simultaneously) |

### Shallow History (H) vs. Deep History (H*)

| Aspect | Shallow History (H) | Deep History (H*) |
|--------|-------------------|------------------|
| **Memory Depth** | Only direct substates (one level) | All nested levels |
| **Restoration** | Restores only the last active direct substate | Restores entire nested state configuration |
| **Use Case** | Simple state resumption | Complex nested state resumption |
| **Default Transition** | Required (used on first entry) | Required (used on first entry) |
| **Example** | Resume to last level in game | Resume to exact location within level |
| **Complexity** | Simpler to implement | More complex, requires full state stack |

### Choice vs. Decision (Event with Guards)

| Aspect | Choice Pseudo-state | Event-Triggered Transition with Guards |
|--------|-------------------|--------------------------------------|
| **Symbol** | Diamond | Arrow from state |
| **Trigger** | No event (automatic on completion) | Event required |
| **Evaluation** | Guards evaluated when choice reached | Guards evaluated when event occurs |
| **Timing** | Immediate decision | Decision only when event happens |
| **Use Case** | Branching after completion | Conditional response to events |
| **Example** | After task completion, check result | If alarm event and temperature>100, transition |

### Fork vs. Join

| Aspect | Fork | Join |
|--------|------|------|
| **Symbol** | Horizontal bar | Horizontal bar |
| **Direction** | One incoming, multiple outgoing | Multiple incoming, one outgoing |
| **Function** | Split transition, activate multiple regions | Synchronize transitions, merge to one |
| **Timing** | Instantaneous split | Waits for all incoming transitions |
| **Use Case** | Enter orthogonal regions concurrently | Exit orthogonal regions synchronously |
| **Example** | Start washing and heating simultaneously | Wait for both wash and rinse to complete |

### Entry/Exit Points vs. Direct Transitions

| Aspect | Entry/Exit Points | Direct Transitions |
|--------|------------------|-------------------|
| **Encapsulation** | Hides internal structure | Exposes internal substates |
| **Flexibility** | Can change internal implementation without affecting external transitions | External transitions tightly coupled to internal structure |
| **Naming** | Named, semantic interface | Directly references substates |
| **Control** | Composite state controls where entry goes | External transition controls target substate |
| **Use Case** | Public interface to composite state | Quick, simple access to specific substate |
| **Maintainability** | Better (decoupling) | Worse (tight coupling) |

### Model Checking vs. Testing

| Aspect | Model Checking | Testing |
|--------|---------------|---------|
| **Approach** | Formal verification, exhaustive | Sampling, execution-based |
| **Coverage** | All possible states and paths | Limited by test cases |
| **Guarantee** | Proves property holds or finds counterexample | Shows presence of bugs, not absence |
| **Automation** | Fully automatic once model and properties defined | Requires test case design and execution |
| **Scalability** | Limited by state space explosion | Scales better to large systems |
| **Timing** | Before implementation (model-level) | After implementation (code-level) |
| **Cost** | Expensive (state space exploration) | Varies (depends on test suite size) |
| **Output** | Yes/No + counterexample | Pass/Fail for each test |

### WCET Analysis Methods Comparison

| Method | Accuracy | Guarantee | Effort | When to Use |
|--------|----------|-----------|--------|-------------|
| **Real System** | High (if worst case found) | No | Medium | Prototype available, non-critical |
| **Simulation** | Medium | No | High | Early design phase, no hardware yet |
| **Measurement** | High (observed) | No | Low | Quick estimate, development phase |
| **Static Analysis** | Medium (over-approximation) | Yes (safe upper bound) | High | Safety-critical, certification required |

### Forward vs. Reverse Engineering

| Aspect | Forward Engineering | Reverse Engineering |
|--------|-------------------|-------------------|
| **Direction** | Model → Code | Code → Model |
| **Purpose** | Generate implementation from design | Understand or document existing code |
| **Automation** | High (code generators) | Medium (limited tool support) |
| **Accuracy** | Perfect (generated code matches model exactly) | Imperfect (model may lose code details) |
| **Use Case** | New development, model-driven design | Legacy system documentation, analysis |
| **Challenge** | Keeping model and code synchronized | Extracting high-level design from low-level code |

## Examples & Applications

### Example 1: Video Game Character State Machine

**Scenario**: Character with movement and action capabilities

**Orthogonal Regions**:
1. **Movement Region**:
   - Idle (standing still)
   - Walking (normal speed)
   - Running (fast speed)
   - Transitions based on joystick input

2. **Action Region**:
   - NotAttacking (safe)
   - Attacking (weapon drawn)
   - Transitions based on attack button

**Combined States**: Character can be in "Walking + Attacking" or "Running + NotAttacking" simultaneously

**Enchanted State**: Overlay composite state that modifies all behaviors (e.g., character moves slower when poisoned)

**Fork**: When game starts, fork into Movement.Idle and Action.NotAttacking

**Join**: When character dies, join from both regions to Dead state

### Example 2: Bank ATM State Machine

**States**:
- Idle (waiting for customer)
- CardInserted (reading card)
- PINEntry (authentication)
- Authenticated (choice pseudo-state)
- Transaction (composite with substates)

**Choice Pseudo-state** after PIN verification:
- [PIN correct] → Authenticated
- [PIN incorrect AND attempts < 3] → PINEntry (retry)
- [PIN incorrect AND attempts ≥ 3] → CardRetained (security)

**History**: PINEntry has shallow history so if customer goes back from transaction selection, they don't re-enter PIN

**Exit Points**:
- Transaction.success → exit point "complete" → Idle
- Transaction.cancel → exit point "abort" → Idle
- Transaction.error → exit point "error" → ErrorHandling

### Example 3: Washing Machine State Machine

**Top-Level States**:
- Off
- On (composite state)

**On Composite State Substates**:
- Idle (waiting)
- ProgramSelection (user choosing cycle)
- Washing (composite with substates)

**Washing Composite Substates** (with deep history):
- Fill (filling water)
- Wash (agitating)
- Rinse (rinsing)
- Spin (spinning to dry)

**Pause Behavior**:
- From any state in Washing, "pause" event → Paused state
- Paused → On[H*] (deep history) → returns to exact substate where it was paused

**Terminate**:
- Emergency stop button → Terminate pseudo-state → machine shuts down immediately

### Example 4: Traffic Light System with Model Checking

**Model**:
- States: Red, Yellow, Green
- Transitions: Red → Green → Yellow → Red (cycle)
- Timing: Red (30s), Green (25s), Yellow (5s)

**Properties to Verify** (CTL):
1. `AG ¬(red ∧ green)` - Never red and green simultaneously (safety)
2. `AG (red → AF green)` - After red, always eventually green (liveness)
3. `AG (yellow → AX red)` - Yellow always followed by red (correctness)
4. `AG EF red` - Can always return to red (no deadlock)

**Model Checker Result**: All properties satisfied → system is correct

**Counterexample** (if property 3 violated): Yellow → Green → ... (shows violation path)

### Example 5: Multi-Processor Task Scheduling

**System**: Dual-core processor (CPU1, CPU2)

**Tasks**:
- T1: Read sensor (5ms) - no dependencies
- T2: Process data (10ms) - depends on T1
- T3: Control actuator (3ms) - depends on T2
- T4: Log data (2ms) - depends on T1
- T5: Display update (8ms) - depends on T2

**Task Graph**:
```
T1 → T2 → T3
 ↓     ↓
T4    T5
```

**Binding Option 1** (balanced):
- CPU1: T1(0-5), T2(5-15), T3(15-18) → Total 18ms
- CPU2: T4(5-7), T5(15-23) → Total 8ms (idle until 5ms)
- **Total execution time**: 23ms

**Binding Option 2** (single processor):
- CPU1: T1(0-5), T2(5-15), T3(15-18), T4(18-20), T5(20-28) → Total 28ms
- CPU2: idle
- **Total execution time**: 28ms

**Best**: Option 1 achieves 5ms speedup through parallelism

### Example 6: WCET Analysis for Function

**Code**:
```c
int process(int n, int data[]) {
    int sum = 0;
    for (int i = 0; i < n; i++) {  // Loop bound: n ≤ 100
        if (data[i] > 0) {
            sum += data[i] * 2;  // 3 cycles
        } else {
            sum += data[i];      // 2 cycles
        }
    }
    return sum;  // 1 cycle
}
```

**Static WCET Analysis**:
- Loop executes maximum 100 times (n ≤ 100)
- Worst case: All data[i] > 0 (if branch, 3 cycles)
- WCET = 100 iterations × 3 cycles + 1 cycle (return) = **301 cycles**
- At 1GHz clock: 301ns

**Measurement-Based**:
- Run with various inputs, measure: max observed = 285ns
- Add 20% safety margin: 285 × 1.2 = 342ns
- **WCET estimate: 342ns** (more pessimistic but less guaranteed)

## Connections to Other Topics

### Connection to Lecture 02: Software Paradigms
- **Event-Driven Paradigm**: UML state machines are event-driven (events trigger transitions)
- **State Pattern**: Object-oriented implementation of state machines
- **Hierarchical Decomposition**: Similar to object hierarchy in OOP (inheritance)
- **Concurrency**: Orthogonal regions relate to multi-threaded programming and concurrent paradigms

### Connection to Lecture 03: Requirements
- **Requirements Modeling**: State machines model behavioral requirements from requirements specification
- **Non-Functional Requirements**: WCET analysis addresses timing requirements (real-time NFR)
- **Use Cases**: State machines implement behavior described in use cases
- **Validation**: Model checking verifies system meets specified requirements (requirements validation)

### Connection to Lab P1: Hardware Bring-up
- **Target Platform**: Tasks in task graphs execute on embedded processors like Ultra96
- **Cross-Compilation**: Generated code from models must be compiled for target architecture
- **Multi-Core**: Hardware-software binding maps tasks to multiple ARM cores (Cortex-A53)
- **Real-Time**: WCET critical for embedded systems with timing constraints

### Connection to Future Lecture: Development Processes
- **Model-Based Development**: Model-driven engineering integrates into development process
- **V-Model**: Model checking fits verification phase; code generation fits implementation phase
- **Iterative Refinement**: Model, verify, refine, repeat (agile-like for formal methods)
- **Testing**: Model checking complements traditional testing strategies

### Connection to Future Lecture: Energy
- **Task Scheduling**: Scheduling decisions affect energy consumption (idle vs. active cores)
- **WCET vs. Energy**: Trade-off between guaranteed timing (fast execution) and energy efficiency (slower, low-power modes)
- **Multi-Processor**: Core binding decisions impact energy (more cores = more energy, but faster = shorter active time)

### Connection to Future Lecture: Performance
- **WCET**: Directly relates to performance (worst-case performance)
- **Parallelism**: Orthogonal states and task graphs exploit parallelism for better performance
- **Critical Path**: Longest path in task graph determines minimum achievable execution time
- **Optimization**: Code generation can include performance optimizations

### Connection to Future Lecture: RTOS Scheduling
- **Tasks**: Task concept central to RTOS scheduling algorithms
- **Dependencies**: Task graphs represent precedence constraints for scheduler
- **WCET**: Essential input for schedulability analysis in RTOS
- **Multi-Processor Scheduling**: Hardware-software binding is multi-processor scheduling problem

### Connection to Future Lecture: Multi-Processor Systems
- **Hardware-Software Binding**: Core concept in multi-processor system design
- **Task Graphs**: Used to analyze parallelism potential in multi-processor systems
- **Load Balancing**: Distributing tasks across processors to balance workload
- **Communication**: Tasks on different processors need inter-processor communication

### Connection to Future Labs
- **Code Generation**: Labs may use generated code from models
- **Performance Analysis**: WCET analysis techniques apply to lab implementations
- **Multi-Core Implementation**: Hardware-software binding concepts apply to lab tasks on multi-core platforms
- **Verification**: Model checking techniques can verify lab implementations

## Summary

Lecture 04 extends UML State Machines with advanced features for modeling complex embedded systems. **Composite states** enable hierarchical organization (nesting substates) and concurrent behavior (orthogonal regions executing in parallel). **Pseudo-states** provide control flow: choice for conditional branching, shallow/deep history for state resumption, entry/exit points for encapsulation, fork/join for synchronization, and terminate for shutdown.

**UML rules** ensure state machines are well-formed: deterministic (no ambiguous transitions), have initial states, avoid infinite completion loops, and contain no unreachable states.

**Model checking** formally verifies system properties using temporal logic (CTL) formulas. A model checker exhaustively explores the state space, proving properties hold or providing counterexamples when violated. This complements testing by guaranteeing correctness for specified properties.

**Model-based code generation** automates transformation from UML models to executable code using tools like Rhapsody. This raises abstraction level and ensures model-code consistency, but requires disciplined modeling and tool mastery.

**Tasks** represent units of work for scheduling. **Task graphs** show dependencies between tasks, enabling analysis of parallelism and scheduling strategies. **Hardware-software binding** assigns tasks to processors in multi-processor systems, balancing load and respecting dependencies.

**WCET (Worst-Case Execution Time)** analysis determines maximum execution time, critical for real-time scheduling and schedulability analysis. Methods include measurement (no guarantee), simulation (medium accuracy), and static analysis (guaranteed safe upper bound). Understanding WCET is essential for embedded systems with timing constraints.

These concepts form the foundation for model-driven development, formal verification, and real-time system design in embedded systems engineering.

## Possible Exam Questions

### Basic Definitions and Concepts (10 questions)

1. **What is a composite state in UML, and what are the two main types?**
   - Answer: A composite state contains substates. Two types: hierarchical (nested substates, one active) and orthogonal (multiple regions, concurrent execution).

2. **Define shallow history (H) and deep history (H*) pseudo-states. How do they differ?**
   - Answer: Both remember last active state. Shallow history remembers only direct substates (one level); deep history remembers entire nested state hierarchy (all levels).

3. **What is the purpose of a choice pseudo-state?**
   - Answer: Provides conditional branching based on guard conditions without requiring events; diamond symbol with outgoing transitions with guards.

4. **What does CTL stand for, and what is it used for?**
   - Answer: Computation Tree Logic, a temporal logic for specifying properties in model checking to verify system behavior over time.

5. **Define WCET and explain why it's important.**
   - Answer: Worst-Case Execution Time, the maximum time a task can take. Critical for real-time systems to guarantee deadlines and perform schedulability analysis.

6. **What is a task graph, and what do nodes and edges represent?**
   - Answer: Directed acyclic graph for task scheduling. Nodes = tasks, edges = dependencies (source task must complete before target task starts).

7. **Explain the difference between a fork and a join pseudo-state.**
   - Answer: Fork splits one transition into multiple (enter concurrent regions); join synchronizes multiple transitions into one (exit concurrent regions).

8. **What are entry and exit points in composite states?**
   - Answer: Named connection points on composite state border that provide controlled, encapsulated access to internal substates.

9. **What is model checking?**
   - Answer: Formal verification technique that exhaustively checks if a system model satisfies specified properties expressed in temporal logic.

10. **Define hardware-software binding in multi-processor systems.**
    - Answer: The assignment of software tasks to specific hardware processors, determining which CPU executes which task.

### Conceptual Understanding (12 questions)

11. **Why must UML state machines be deterministic, and how is this achieved?**
    - Answer: Prevents ambiguity about which transition to take. Achieved by ensuring at most one transition can be triggered for any state-event combination (mutually exclusive guards or different events).

12. **Explain how substates inherit transitions from their parent composite state.**
    - Answer: Transitions from composite state border apply to all substates. If no substate handles an event, the parent's transition is taken, exiting all substates.

13. **What problem do orthogonal regions solve in state machine design?**
    - Answer: Model concurrent/parallel behaviors within one state, allowing multiple independent state machines to execute simultaneously (e.g., movement + actions in game).

14. **Why is model checking considered complementary to testing rather than a replacement?**
    - Answer: Model checking verifies models (design level), proves properties exhaustively but suffers from state explosion; testing validates implementations (code level), scales better but can't guarantee absence of bugs.

15. **Explain the "coin stacking analogy" for WCET determination challenges.**
    - Answer: Just because you've never observed coins stacking to 20cm doesn't mean it's impossible. Similarly, not observing the worst-case execution doesn't mean it won't occur—timing analysis must consider all possibilities.

16. **What advantages does model-based code generation provide over manual coding?**
    - Answer: Higher abstraction, reduced manual errors, automatic model-code consistency, better documentation, platform independence, easier maintenance of complex systems.

17. **How does deep history (H*) preserve nested state configuration?**
    - Answer: Records entire hierarchy of active states at all levels. When returning via H*, restores complete nested state structure exactly as it was when exited.

18. **What role do initial pseudo-states play in composite states?**
    - Answer: Define default substate that becomes active when composite state is entered; ensures state machine always knows where to start execution.

19. **Explain why static WCET analysis provides a "safe upper bound."**
    - Answer: Mathematically calculates maximum possible execution time considering all paths and hardware behavior, guaranteeing actual execution never exceeds this bound (may over-approximate).

20. **How do entry/exit points improve encapsulation in state machine design?**
    - Answer: Hide internal structure by providing named interface; external transitions don't directly reference substates, allowing internal changes without affecting external transitions.

21. **What is the critical path in a task graph, and why does it matter?**
    - Answer: Longest path through task graph considering dependencies. Determines minimum possible total execution time even with unlimited processors (sequential dependencies).

22. **Describe how fork/join enables synchronization in orthogonal regions.**
    - Answer: Fork simultaneously activates substates in all regions (concurrent entry); join waits for all regions to complete before proceeding (synchronized exit).

### Application and Analysis (15 questions)

23. **Given a state machine with states {A, B, C} and transitions: A→B[guard1], A→C[guard2]. What condition must guard1 and guard2 satisfy to maintain determinism?**
    - Answer: Must be mutually exclusive (not both true simultaneously). Typically includes [else] on one transition or ensure conditions don't overlap.

24. **In a multi-processor system with 2 CPUs, you have 5 tasks: T1(10ms), T2(5ms, depends on T1), T3(8ms, depends on T1), T4(3ms, depends on T2), T5(2ms, depends on T3). What's the minimum execution time?**
    - Answer: Critical path: T1→T2→T4 = 10+5+3=18ms or T1→T3→T5=10+8+2=20ms. Minimum = 20ms (even with 2 CPUs, sequential dependencies limit parallelism).

25. **A washing machine needs to pause and resume at the exact substate. Should you use shallow or deep history? Why?**
    - Answer: Deep history (H*). Washing cycle has nested substates (e.g., Wash→Agitate, Rinse→Cold/Hot); must restore exact configuration to resume correctly.

26. **Write CTL formulas to express: (a) "System never deadlocks" (b) "Every request is eventually answered".**
    - Answer: (a) AG ¬deadlock (always globally not deadlocked); (b) AG (request → AF answer) (always: if request, then always finally answered).

27. **You have a state machine where choice pseudo-state has guards [x>10], [x<5], [5≤x≤10]. Is this well-formed? Why or why not?**
    - Answer: Yes, well-formed. Guards are mutually exclusive (cover all possibilities with no overlap) and exhaustive (no gap). Deterministic branching guaranteed.

28. **Design a state machine fragment using entry/exit points for a "Menu" composite state that has three entry scenarios: from Settings, from Home, from Search.**
    - Answer: Menu composite state with three entry points (entrySettings, entryHome, entrySearch) on border. Each connects to different initial substate inside Menu based on context.

29. **Explain when you would use a choice pseudo-state vs. a regular transition with a guard condition.**
    - Answer: Choice for branching after automatic completion (no event), evaluated immediately. Guarded transition for event-driven conditional behavior (event + guard).

30. **A video game has character states: Walking, Running, Jumping, Attacking, Defending. Which states should be in orthogonal regions, and why?**
    - Answer: Movement (Walking, Running, Jumping) + Combat (Attacking, Defending) in separate orthogonal regions. Character can run while attacking simultaneously (concurrent behaviors).

31. **Given WCET measurements: 10ms, 12ms, 15ms, 14ms, 11ms. Estimate WCET with 20% safety margin. Why is this approach not guaranteed safe?**
    - Answer: Max observed = 15ms, with margin = 15×1.2 = 18ms. Not guaranteed safe because may not have tested actual worst-case scenario (limited test coverage).

32. **A model checker reports "No" for property AG (ready). The counterexample shows: Init→Process→Error→Halt. What does this mean?**
    - Answer: Property "always ready" is violated. System can reach Halt state where it's not ready. Counterexample shows execution path leading to violation.

33. **You have 3 orthogonal regions in a composite state. How many fork/join bars do you need for proper entry/exit?**
    - Answer: One fork with 3 outgoing transitions (entering all regions) and one join with 3 incoming transitions (exiting all regions synchronously).

34. **Design a state machine for a coffee machine with states {Idle, CoinInserted, Selection, Brewing, Dispensing}. Add history so if user cancels during Selection, when they insert coin again, they return to last selected option.**
    - Answer: Selection as composite with substates (Coffee, Tea, Espresso). Cancel→Idle. CoinInserted→Selection[H] (shallow history remembers last selection).

35. **In static WCET analysis, what happens if you underestimate loop bounds?**
    - Answer: Calculated WCET will be too optimistic (unsafe). May miss actual worst case, causing deadline violations in real-time system—potentially catastrophic failure.

36. **Compare the benefits of using a terminate pseudo-state vs. transitioning to a "Final" state.**
    - Answer: Terminate immediately stops entire state machine (emergency shutdown). Final state is normal completion, may have exit actions or cleanup. Terminate more abrupt, Final more controlled.

37. **You have tasks T1→T2→T3 (sequential) taking 5ms, 10ms, 3ms respectively. How does adding a second processor help?**
    - Answer: Doesn't help—tasks are sequentially dependent. Total time remains 18ms regardless of processor count. Parallelism only benefits independent tasks.

### Critical Thinking and Synthesis (12 questions)

38. **Critique: "Model checking eliminates the need for testing in embedded systems development." Discuss.**
    - Answer: False. Model checking verifies models (design), not implementations (code). Code generation errors, hardware issues, integration problems still need testing. Both methods complementary.

39. **What trade-offs exist between using hierarchical vs. orthogonal composite states for a complex system?**
    - Answer: Hierarchical: simpler, sequential, easier to understand, less state explosion. Orthogonal: models concurrency naturally, but exponentially more combined states, harder to verify, more complex synchronization.

40. **Why might generated code from models be less efficient than hand-written code? When is this acceptable?**
    - Answer: Generators prioritize correctness/generality over optimization, may include unnecessary code. Acceptable when: development time critical, safety critical (verified model), performance not bottleneck, maintainability prioritized.

41. **Design a state machine strategy for a medical device that requires history, emergency shutdown, and concurrent monitoring. Explain your choices.**
    - Answer: Top level: Normal (composite) + Emergency(terminate). Normal has orthogonal regions: Treatment + Monitoring. Treatment uses deep history (H*) to resume exactly. Emergency uses terminate for immediate stop.

42. **Explain the relationship between task graph critical path, processor count, and total execution time.**
    - Answer: Critical path = minimum time (serial dependencies). Adding processors helps parallel tasks but can't reduce critical path. Speedup limited by serial fraction (Amdahl's law analog). Optimal processor count ≈ parallelism degree.

43. **A state machine violates "no infinite completion cycles." How would model checking detect this vs. testing?**
    - Answer: Model checking: analyzes state space, detects cycle of completion transitions (no events), proves infinite loop possible. Testing: may run forever or timeout, but can't prove absence of infinite loops in all scenarios.

44. **Compare the challenges of WCET analysis for simple embedded microcontroller vs. modern multi-core processor.**
    - Answer: Microcontroller: predictable, no cache/pipeline complexity, simpler analysis. Multi-core: cache interference, pipeline speculation, inter-core communication, shared resources make timing nearly impossible to predict accurately.

45. **Why do UML state machine rules prohibit unreachable states? How does this relate to code quality?**
    - Answer: Unreachable states indicate design errors (missing transitions, logic flaws). In code: dead code wastes memory, confuses maintenance, may indicate bugs. Model checking finds unreachable states automatically.

46. **Propose a model-checking strategy for verifying a state machine with 10^20 states (state space explosion problem).**
    - Answer: Abstraction (reduce model detail), compositional verification (verify components separately), symmetry reduction (eliminate equivalent states), bounded model checking (limit search depth), statistical model checking (sampling).

47. **How does the choice between shallow vs. deep history impact system resumption behavior in nested hierarchies? Provide a scenario where the choice matters.**
    - Answer: Scenario: Game with World→Level→Room states. Shallow H: returns to last level but default room. Deep H*: returns to exact room. Matters for: save games (need exact position), pause (want exact resumption).

48. **Discuss how WCET analysis relates to energy consumption in battery-powered embedded systems.**
    - Answer: Longer execution = more energy. But aggressive optimization for speed (high frequency, no sleep) uses more power. Trade-off: fast execution (shorter time, high power) vs. slow execution (longer time, low power). WCET bounds worst-case energy.

49. **Critique the statement: "Automatically generated code should never be manually modified." When might this rule be violated, and what are the consequences?**
    - Answer: Rule prevents model-code divergence, enables regeneration. Violations needed for: platform-specific optimizations, working around generator bugs, emergency patches. Consequences: maintenance nightmare, can't regenerate, defeats purpose of model-based development.

### Scenario-Based Questions (10 questions)

50. **Design a state machine for an elevator system serving 5 floors with emergency stop capability. Include composite states and history.**
    - Answer: Top: Operating (composite with H) + Emergency(terminate). Operating regions: Movement(Idle, Moving, DoorsOpen), RequestQueue(pending floors). Emergency button→terminate. Power restore→Operating[H] resumes.

51. **A robot has states for navigation, manipulation, and battery monitoring. These operate concurrently. When battery is critical, robot must stop all activities. Model this with UML state machines.**
    - Answer: Composite state Active with 3 orthogonal regions: Navigation, Manipulation, Monitoring. Monitoring tracks battery. When critical: join from all regions→LowPower state→Charging. Fork back when charged.

52. **You're verifying a railway signaling system. Write CTL properties to check safety (no train collisions) and liveness (trains eventually proceed).**
    - Answer: Safety: AG ¬(train1_at_junction ∧ train2_at_junction). Liveness: AG (train_waiting → AF train_proceeding). Additional: AG (signal_red → ¬train_enters_section).

53. **A software update is available during device operation. Design states to safely download, install, and rollback if installation fails, using history to resume previous state.**
    - Answer: Main state Operating[H*]. Update available→Updating(composite: Downloading→Installing→Verifying). Verify success→Operating[H*] (resumes). Verify fail→Rollback→Operating[H*] (restores previous).

54. **Optimize task scheduling: T1(15ms), T2(10ms depends on T1), T3(8ms depends on T1), T4(5ms depends on T2), T5(12ms depends on T3). You have 3 CPUs. Propose binding and calculate execution time.**
    - Answer: CPU1: T1(0-15), T2(15-25), T4(25-30) = 30ms. CPU2: T3(15-23), T5(23-35) = 35ms. CPU3: idle. Critical path T1→T3→T5=35ms. Can't improve with more CPUs.

55. **A medical pump has states: Priming, Infusing, Paused, Alarming. Nurse pauses infusion to attend to patient, then resumes. Alarm can trigger from any state. Model with history and priority.**
    - Answer: Top: Normal (Priming, Infusing, Paused with H to Infusing) + Alarming. Any state + alarm_event→Alarming (highest priority). Alarm cleared→Normal[H] (returns to state before alarm).

56. **Your model checker reports state space explosion after 10^8 states. Propose 3 strategies to make verification feasible.**
    - Answer: 1) Abstract data types (use ranges instead of exact values). 2) Remove orthogonal regions (verify sequentially). 3) Compositional verification (verify subsystems separately, assume interface contracts).

57. **A drone has flight modes: Takeoff, Cruising, Landing. During cruising, it performs orthogonal tasks: Navigation, Photography, Obstacle Avoidance. Emergency requires immediate landing. Design state machine.**
    - Answer: Top: Flying→Emergency Landing. Flying has substates: Takeoff→Cruising(composite with 3 orthogonal regions: Nav, Photo, Avoid)→Landing. Emergency event from any state→Emergency Landing(terminate or direct transition).

58. **Estimate WCET for task with nested loops: outer(100 iterations), inner(50 iterations), body(10 cycles). How do cache misses (add 5 cycles) affect WCET?**
    - Answer: Without cache: 100×50×10 = 50,000 cycles. With cache miss every inner loop: 100×50×(10+5) = 75,000 cycles. Or miss per body execution: 100×50×15 = 75,000 cycles. WCET increases 50%.

59. **Design a choice pseudo-state scenario for a thermostat: if temp<18°C→Heating, if temp>25°C→Cooling, otherwise→Idle. Show guards.**
    - Answer: Sense temperature (completion)→Choice. Outgoing: [temp<18]→Heating, [temp>25]→Cooling, [18≤temp≤25]→Idle (or use [else]→Idle). Guards mutually exclusive.

60. **A system has 10 tasks with complex dependencies. Explain how you'd construct the task graph, identify critical path, and decide on processor count.**
    - Answer: 1) List tasks with dependencies (precedence constraints). 2) Draw DAG (tasks=nodes, dependencies=edges). 3) Calculate path lengths (sum task durations), identify longest=critical path. 4) Processor count: # of independent branches, max parallelism in graph. More than that shows diminishing returns.

### Connections and Integration (8 questions)

61. **How do UML state machines implement behavioral requirements specified in Lecture 03? Give an example.**
    - Answer: Requirements define what system must do (e.g., "System SHALL authenticate user within 3 attempts"). State machine models this: States(Idle, PINEntry, Authenticated), events(insertCard, enterPIN), guards[attempts<3].

62. **Relate orthogonal regions in state machines to multi-threading in software paradigms (Lecture 02). What are similarities and differences?**
    - Answer: Similar: concurrent execution, independent flows, synchronization (fork/join like thread spawn/join). Different: orthogonal regions=design abstraction, threads=implementation; state machines=event-driven, threads=time/event-driven.

63. **How does WCET analysis connect to real-time NFRs (Non-Functional Requirements) that will be covered later?**
    - Answer: Real-time NFRs specify timing constraints (deadlines, response times). WCET is input to schedulability analysis to verify system meets timing NFRs. Without accurate WCET, can't guarantee real-time requirements.

64. **Explain how model-based code generation workflow fits into an embedded systems development process (preview of Lecture 05).**
    - Answer: Design phase: create models. Implementation: generate code. Verification: model checking (model level) + testing (code level). Integration: combine generated + manual code. Deployment: target system. Iterative refinement.

65. **Connect task scheduling concepts (task graphs, dependencies) to the Lab P1 makefile dependencies you saw earlier.**
    - Answer: Makefile: targets=tasks, prerequisites=dependencies. main.o depends on main.c (T2 depends on T1). Make scheduler decides which to build, parallel builds exploit independence (like multi-processor scheduling).

66. **How might energy consumption (future Lecture 08) influence hardware-software binding decisions in multi-processor systems?**
    - Answer: More active processors=more energy. Binding influences: 1) Core count (fewer cores=less power if meets timing). 2) Core type (big.LITTLE: performance vs. efficiency cores). 3) Idle time (power down unused cores).

67. **Relate the concept of model checking's "counterexample" to debugging practices in software development.**
    - Answer: Both identify failure scenarios. Counterexample=specific execution trace violating property (like bug reproduction steps). Helps locate error source. Difference: counterexample guaranteed to exist if property violated; bugs may be hard to reproduce.

68. **How do composite states with history relate to state persistence and context saving in embedded systems?**
    - Answer: History=logical state persistence. Implementation requires saving state context (variables, active substate IDs) to memory. On power loss/resume: restore context from persistent storage, transition via history to resume exact state.

### Advanced Understanding (7 questions)

69. **Prove or disprove: "Any state machine with orthogonal regions can be flattened into an equivalent non-orthogonal state machine." Discuss implications.**
    - Answer: True but impractical. N regions with M states each→M^N combined states (state explosion). Example: 3 regions, 5 states each=125 combined states. Equivalent but unmanageable. Orthogonal regions are abstraction for tractability.

70. **Analyze the trade-offs between over-approximation (static WCET) and under-approximation (measurement-based WCET) for safety-critical systems.**
    - Answer: Over-approximation: Safe (no missed deadlines), but wastes resources (over-provisioning), may make system unschedulable unnecessarily. Under-approximation: Efficient use, but unsafe—one missed worst case=potential catastrophe. Safety-critical MUST use over-approximation (certifiable).

71. **Explain how the state space explosion problem in model checking relates to the halting problem in computability theory.**
    - Answer: Both undecidable for general case. Can't guarantee model checking terminates (infinite state space, unbounded loops). Halting problem: can't algorithmically determine if program halts. Practical: use bounded verification, abstractions, or statistical methods.

72. **Design a verification strategy combining model checking (formal) and testing (empirical) for an autonomous vehicle control system.**
    - Answer: 1) Model checking: verify critical safety properties (AG ¬collision), liveness (AF destination), mode transitions. 2) Testing: real-world scenarios, sensor noise, hardware integration, performance. Model checking=design correctness, Testing=implementation correctness.

73. **Discuss how determinism in UML state machines relates to determinism in formal language automata theory (DFA vs. NFA).**
    - Answer: UML determinism=exactly one transition per (state,event). DFA=exactly one transition per (state,symbol). UML allows guards for disambiguation. NFA=multiple possible transitions (non-deterministic). UML rules enforce DFA-like determinism for predictability.

74. **Analyze the impact of cache behavior on WCET analysis. Why does cache make timing analysis significantly harder?**
    - Answer: Cache introduces history-dependent timing: same instruction, different execution time (hit=fast, miss=slow). Timing depends on: previous executions, data access patterns, cache size/associativity. Preemption complicates (task switch may flush cache). Static analysis must consider all cache states=exponential complexity.

75. **Propose a research direction for improving scalability of model checking for large embedded systems with millions of states.**
    - Answer: 1) AI/ML-guided state space exploration (learn likely violation paths). 2) Incremental verification (verify changes only). 3) Cloud-distributed model checking (parallel exploration). 4) Property-directed reachability (work backward from property violation). 5) Hybrid formal-simulation approaches.
