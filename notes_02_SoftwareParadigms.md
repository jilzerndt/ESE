# Lecture 02: Software Paradigms

## Technical Terms & Definitions

### Software Paradigm
- **Definition**: A set of rules, models, and guidelines that define how software should be structured and developed
- Helps improve software quality by providing systematic approaches

### Imperative Programming
- **Definition**: Programming style that defines **HOW** to do something through a sequence of commands
- Step-by-step instructions that change program state
- Examples: C, C++, Java, Python

### Declarative Programming
- **Definition**: Programming style that defines **WHAT** to do, leaving the "how" to the language/compiler
- Specifies rules instead of steps
- Aims to avoid side effects
- Examples: SQL, Prolog

### Cyber-Physical System
- **Definition**: System representing interaction between the physical world and computation
- Embedded systems are cyber-physical systems
- Common components: Sensors, Computer, Actuators

### Structured Programming
- **Definition**: Programming approach that gives structure to programs through control flow constructs
- Allows higher level of abstraction
- Well-known paradigm using functions, loops, conditionals

### Procedural Programming
- **Definition**: Programming paradigm using procedures (subroutines/functions) to break programs into small tasks
- Enables unit testing of individual functions
- Data is separated from procedures

### Object-Oriented Programming (OOP)
- **Definition**: Programming paradigm where data and matching procedures are combined in objects
- Objects encapsulate properties (data) and behaviors (methods)
- **Class**: Abstract template representing a set of objects with similar properties
- **Object**: Instance of a class with specific property values
- **Inheritance**: Classes can inherit properties and behaviors from superclasses
- **Encapsulation**: Objects contain their own data

### Event-Driven Programming
- **Definition**: Programming paradigm where program flow is determined by events (external or internal)
- Events can be: periodic, sporadic, or one-shot
- **Event Queue**: Data structure to collect all pending events
- **Event Handler**: Function that processes a specific event type
- **Non-preemptive Scheduling**: Handlers do not interrupt each other
- **Preemptive Scheduling**: Handlers can interrupt each other; requires context saving

### Time-Driven Programming
- **Definition**: Programming paradigm where a timer defines fixed ticks for timing purposes
- All tasks scheduled based on time, not events
- Sensors must be polled (not interrupt-driven)
- Enables deterministic timing

### Task Descriptor List (TDL)
- **Definition**: Table containing cyclic schedule over one period T
- Contains all activities for the period with desired priorities
- Used in generic time-triggered schedulers
- Created offline to avoid deadlocks

### Worst-Case Execution Time (WCET)
- **Definition**: Maximum time a task can take to execute
- Critical for time-driven systems
- Sum of all WCETs must fit within scheduling period

### Period (T)
- **Definition**: Time interval after which a cyclic schedule repeats
- In simple periodic model, all tasks run with period T
- In generic model, different tasks can have different periods

### Frame (f)
- **Definition**: Smallest time tick in a scheduler
- Provides finer granularity for scheduling
- Multiple frames make up one period

### Dispatcher
- **Definition**: Component triggered by clock tick that executes handlers as planned in TDL
- Implements the schedule defined in the Task Descriptor List

### Side Effect
- **Definition**: When a function modifies state outside its local scope
- Memory contents changed multiple times during execution
- Common in imperative programming (e.g., loop counters)
- Declarative programming aims to avoid side effects

### Static Variable
- **Definition**: Variable that retains its value between function calls
- Exception to the rule that procedures don't contain persistent data

### Unit Testing
- **Definition**: Testing individual functions/procedures in isolation
- Easier in procedural programming due to data/function separation

### Polling
- **Definition**: Repeatedly checking sensor/input status at regular intervals
- Used in time-driven systems instead of interrupts
- Disadvantage: May waste CPU cycles

### Stack Frame
- **Definition**: Memory structure storing context of interrupted handler
- Necessary in preemptive event-driven systems
- Contains local variables, return addresses, register values

### Semaphore
- **Definition**: Synchronization mechanism to protect shared resources
- Used in preemptive systems to prevent race conditions

## Core Concepts

### Software's Role in Embedded Systems

**Developer's Task**: Translate a design into software

**Software Execution**:
- Processor executes machine code instructions compiled from high-level languages
- Execution changes the state of the embedded system

**What Software Describes**:
1. **What** must change in an embedded system
2. **How** the change is to be made (usually)

**Real-Time Complexity**:
- **When** the change needs to happen
- **How often** a change may be required
- **How quickly** or by what deadline

### Two Main Paradigm Categories

#### Imperative Paradigm
- **Focus**: Define HOW to do something
- **Characteristics**:
  - Sequence of commands executed to achieve required change
  - Step-by-step instructions
  - Performance closely tied to machine architecture
  - Compiler needs to know processor (instruction set architecture)
  - Developer needs to know system architecture

**Properties**:
- Easy to understand (small, specific steps)
- Very detailed (many lines of code needed)
- Defined order (commands executed in sequence)
- Can have side effects (memory changed multiple times)

**Example** (sum of array):
```c
int sum = 0, nums = [1..10];
for (i = 0; i < 10; i++){
    sum = sum + nums[i]; // side effect!
}
// Execution: sum = sum + 1; i = i + 1; sum = sum + 2; i = i + 1; ...
```

#### Declarative Paradigm
- **Focus**: Define WHAT to do
- **Characteristics**:
  - Specifies rules instead of steps
  - "How" is left to language or compiler
  - Tasks described as rules or logic
  - Aims to have no side effects

**Example** (sum of list):
```
sum[1..10]
// Definition of patterns
sum::[Int] --> Int
sum[n] = n
sum(n:ns) = n + sum(ns)

// Execution:
sum[1,2,3,4,5,6,7,8,9,10]
  = 1 + sum[2,...,10]
  = 1 + (2 + sum[3,...,10]) ...
```

### Structured Programming Paradigm

**Purpose**: Give structure to a program

**Approach**: Allow higher level of abstraction through control structures

**Familiarity**: Well-known to any programming student

**Foundation**: Forms the basis for procedural, object-oriented, and other paradigms

### Procedural Programming Paradigm

**Core Concept**: Break programs into small tasks using procedures (subroutines/functions)

**Advantages**:
- Allows full testing in small parts (unit testing)
- Higher level of abstraction
- Code reuse through functions
- Simplicity and easy testing

**Data Storage**:
- Procedures **don't contain** the data they process
  - Exception: static variables
  - Local variables lost after execution
- **Data is separated** from procedures
  - Stored in variables
  - Organized in structs

**Example Structure**:
```c
// Data:
struct sample_holder {
    int num_rows;
    int num_cols;
    char serial_nr[20];
};

// Function (separate from data):
void print_sample_holder(struct sample_holder holder) {
    printf("Number of rows: %d\n", holder.num_rows);
    printf("Number of columns: %d\n", holder.num_cols);
    printf("Serial Number: %s\n", holder.serial_nr);
}
```

**Use Cases in Embedded Systems**:
- Device drivers
- Signal filtering
- Simple control algorithms

### Object-Oriented Programming Paradigm

**Core Concept**: Data and matching procedures are combined in objects

**Objects**:
- Abstract representations storing defined properties (data)
- Have procedures operating on the properties
- May have: role, attributes, life cycle, identity, states, connections to other objects

**Similar Objects**:
- Recognizable as group through same/similar function and properties
- Distinguishable by different properties and names

**Classes**:
- Represent a set of objects with similar properties
- Combine properties and behaviors
- Can be refined through **inheritance**
- Inherit properties and behaviors from superclasses
- Organized hierarchically

**Example - Rocket Engine**:
- Each Rocket_Engine has: max thrust, current thrust, thrust vector, fuel level
- All Rocket_Engines can: ignite(), shutdown(), rud()
- Subclasses: Raptor_Engine, Merlin_Engine (inherit from Rocket_Engine)

**Advantages**:
- Natural modeling of real-world entities
- Extensibility through inheritance and polymorphism
- Encapsulation of data

**Disadvantages**:
- More overhead compared to procedural
- Complex for small systems
- May be overkill for resource-limited embedded systems

**Use Cases in Embedded Systems**:
- Systems with multiple device types
- Systems with many similar objects (e.g., IoT sensor nodes)
- Complex systems benefiting from abstraction

**Summary Points**:
- Useful for ES with many similar objects
- Classes define properties and behaviors
- Classes organized hierarchically
- Objects belong to one or more classes
- Objects encapsulate their data

### Event-Driven Programming Paradigm

**Core Concept**: Embedded systems react to events (external or internal)

**Event Types**:
- **Periodic**: Occur at regular intervals
- **Sporadic**: Occur irregularly
- **One-shot**: Occur once

**Event Processing**:
- Every event has a corresponding response (handler)
- Events issued by interrupts or internal tasks
- Event queue collects all pending events
- Event handler checks queue and processes events

**Non-Preemptive Event-Triggered Scheduling**:
- Event handler processes one event after another
- Handlers **do not interrupt** each other
- Interrupts may still preempt a handler
- Dynamic and adaptive reaction

**Problems**:
- Long runtime of one handler may prevent others from meeting deadlines
- Solution: Partition handler into immediate part and less critical part
- Conflicts with shared resources and buffers

**Preemptive Event-Triggered Scheduling**:
- Handlers **will interrupt** each other
- Context of interrupted handler must be stored (stack frame)
- Handlers must not wait on external events/resources (keep execution short)
  - No disk access in handler!
- Must protect shared resources (disable interrupts, use semaphores)

**Advantages**:
- Responsive to external events
- Efficient use of CPU (no polling)
- Natural for interrupt-driven systems

**Disadvantages**:
- Risk of missed deadlines
- Can be unpredictable
- Contention for resources
- Complex to verify timing

**Use Cases**:
- Keypad/button interfaces
- User interface (UI) systems
- Interrupt-driven controls
- Systems with sporadic events

### Time-Driven Programming Paradigm

**Core Concept**: Timer defines fixed tick for timing purposes

**Purely Time-Triggered Model**:
- No interrupts except timer tick
- Fixed and complex schedule defined when timing requirements known
- Advantages:
  - Deterministic timing feasible within WCET
  - Predictable behavior
- Disadvantages:
  - Sensors must be polled (not interrupt-driven)
  - Inflexible for sporadic tasks

**Simple Periodic Time-Triggered Model**:
- Regular timer interrupt with period T
- All tasks run with period T
- Execution times not constant → later tasks may have irregular start times
- No deadlocks expected (handlers always run in same order)
- **Requirement**: Sum of WCET of all handlers must be < T

**Example Code**:
```c
counter = 0;
main {
    define task_table;
    set_timer(start_of_first_period);
    while(1) { sleep(); }
}

timer_interrupt_handler {
    counter++;
    set_timer(start_of_next_period);
    for(k = 0; ...){
        run_handler(k);
    }
}
```

**Generic Time-Triggered Scheduler**:
- Handlers may have different periods (e.g., h1: 4ms, h2: 5ms, h3: 20ms, h4: 20ms)
- Define smaller frame f to provide finer granularity
- Use Task Descriptor List (TDL) with cyclic schedule over period T
  - Created **offline**
  - Contains all activities with desired priorities
  - Avoids deadlocks
- Dispatcher triggered by clock tick executes handlers per TDL

**Example TDL**:
| t (ms) | h(k) |
|--------|------|
| 0 | h1, h3 |
| 2 | h2 |
| 4 | h1 |
| 6 | h4 |
| 8 | h2 |
| 10 | h1 |
| ... | ... |

**Use Cases**:
- Motor control
- Closed-loop periodic tasks
- Safety-critical systems requiring deterministic timing
- Systems with predictable, periodic behavior

## Important Diagrams & Graphics

### 1. Cyber-Physical System Interaction Diagram
**Description**: Block diagram showing interaction between embedded system and physical world:
- Top: "Embedded System" box (blue)
- Middle level:
  - Left: "Sensor" box (green)
  - Right: "Actuator" box (orange/yellow)
- Bottom: "Physical World" box (white/bordered)
- Arrows showing bidirectional flow:
  - Physical World → Sensor → Embedded System
  - Embedded System → Actuator → Physical World

**Key takeaway**: ES continuously interact with physical world through sensors and actuators

### 2. Programming Paradigm Hierarchy Tree
**Description**: Tree diagram showing classification of programming paradigms:
- Root node at top (unlabeled)
- First level splits into two branches:
  - Left: "Imperative"
  - Right: "Declarative"
- Under Imperative → "Structured" → "Procedural"
- Under Procedural, three branches:
  - Left: "Object-oriented"
  - Middle: "Event-driven"
  - Right: "Time-driven"
- Under Declarative → "Functional"

**Key takeaway**: Procedural paradigm has three main sub-paradigms relevant to embedded systems

### 3. Imperative vs Declarative Code Comparison
**Description**: Side-by-side code examples with tree diagram:
- Left side: Imperative code (for loop with side effects)
- Right side: Declarative code (recursive pattern matching)
- Tree diagram shows branching from root to Imperative and Declarative nodes

**Key takeaway**: Same problem solved differently - imperative shows HOW, declarative shows WHAT

### 4. Procedural Data-Function Separation (Vial Holder Example)
**Description**: Photos and code showing lab automation equipment:
- Image 1: Automated pipetting robot (Visienco)
- Image 2: Red sample holder with multiple vial slots (Colourbox)
- Left: C struct definition for sample_holder (data)
- Right: void print_sample_holder() function (separate from data)

**Key takeaway**: In procedural programming, data structures and functions that operate on them are separate

### 5. Event Queue and Handler Diagram
**Description**: Flow diagram of event-driven system:
- Top: Three boxes labeled "sensor A", "sensor B", "..."
- Arrows pointing down to event queue (vertical stack)
- Event queue shows: event3, event2, event1, event1, event3
- Bottom of queue labeled "event1" being processed
- Arrow pointing to "handle_event()" box (purple)
- Arrow pointing to "action" output

**Key takeaway**: Events collected in queue, processed sequentially by handler

### 6. Preemptive Event Scheduling Timeline
**Description**: Timing diagram showing task preemption:
- Horizontal timeline labeled "time"
- Four colored horizontal bars showing execution of different contexts:
  - White bar: Context main (baseline)
  - Blue bars: Context h1
  - Green bars: Context h2
  - Pink bar: Context h3
- Bars stacked showing interruption patterns
- Right side: Stack diagram showing "Context h3" on top of "Context h2" on top of "Context h1" on top of "Context main"

**Key takeaway**: Higher priority handlers interrupt lower priority ones; contexts must be saved on stack

### 7. Simple Periodic Time-Triggered Schedule
**Description**: Timing diagram for periodic tasks:
- Horizontal timeline with marks at regular intervals
- Three handlers (h1, h2, h3) shown as rectangles
- Pattern repeats every period T
- Dashed box below showing "h1, h2, h3" sequence
- Handlers may have variable execution times (shown by different widths)

**Key takeaway**: All tasks run in same sequence every period T

### 8. Generic Time-Triggered Schedule
**Description**: Complex timing diagram with multiple periods:
- Timeline from 0 to 20 ms
- Major period T = 20 ms
- Frame f = 2 ms (minor ticks)
- Different handlers (h1, h2, h3, h4) scheduled at different times
- h1 appears multiple times (every 4 ms)
- h2 appears at 2, 8, 12, 18 ms (every ~5 ms)
- h3, h4 appear once in the period (20 ms period)

**Key takeaway**: Generic scheduler allows different tasks with different periods

### 9. Hard vs Soft Real-Time Diagram
**Description**: Graph showing value vs time with deadline:
- Y-axis: "Value"
- X-axis: "Time"
- Vertical line marked "Deadline"
- Two lines after deadline:
  - Solid horizontal line labeled "Hard" (value drops to zero immediately)
  - Dashed declining line labeled "Soft" (value gradually decreases)

**Key takeaway**: Hard real-time systems lose all value if deadline missed; soft real-time systems degrade gracefully

### 10. Paradigm Selection Decision Tree (Implicit)
**Description**: Not a single diagram, but concept shown through comparison tables and use case examples throughout lecture

## Formulas & Procedures

### Schedulability Condition for Simple Periodic Time-Driven Systems

**Formula**:
```
Σ WCET(hᵢ) ≤ T
```

Where:
- WCET(hᵢ) = Worst-Case Execution Time of handler i
- T = Period of the schedule
- Σ = Sum over all handlers

**Explanation**: The sum of worst-case execution times of all handlers must fit within the scheduling period T

**Example**:
If T = 20 ms and we have three handlers:
- h1: WCET = 3 ms
- h2: WCET = 5 ms
- h3: WCET = 4 ms
- Total: 3 + 5 + 4 = 12 ms < 20 ms ✓ Schedulable

### Frame Size Calculation for Generic Time-Driven Scheduler

**Procedure**:
1. List all task periods: P₁, P₂, ..., Pₙ
2. Calculate frame size f as a divisor of all periods
3. Frame f should satisfy:
   - f divides all periods Pᵢ
   - f ≥ WCET of largest task
   - f ≤ shortest period

**Example**:
Given tasks with periods: 4 ms, 5 ms, 20 ms, 20 ms
- Greatest Common Divisor approach won't work (GCD of 4, 5, 20 = 1)
- Choose f = 2 ms (divides 4 and 20, but not 5)
- Or choose f = 1 ms (divides all periods)
- Major period T = LCM(4, 5, 20) = 20 ms

### Task Descriptor List (TDL) Creation Procedure

**Steps**:
1. Determine major period T = LCM of all task periods
2. Select frame size f
3. For each frame k from 0 to T/f:
   - Calculate time t = k × f
   - For each task i:
     - If t mod Pᵢ = 0, schedule task i in frame k
4. Verify no frame exceeds frame time f with WCET
5. Order tasks within frame by priority/dependency

**Example TDL Creation**:
Tasks: h1 (P=4ms), h2 (P=5ms), h3 (P=20ms), h4 (P=20ms)
Frame f = 2 ms, Period T = 20 ms

| Frame | Time (ms) | Tasks Scheduled |
|-------|-----------|-----------------|
| 0 | 0 | h1 (0 mod 4=0), h3 (0 mod 20=0) |
| 1 | 2 | h2 (closest to 0+5=5) |
| 2 | 4 | h1 (4 mod 4=0) |
| 3 | 6 | h4 (offset start) |
| 4 | 8 | h2 (closest to 5+5=10) |
| 5 | 10 | h1 (10 mod 4 ≠ 0, but 12 mod 4=0, delay) |

## Comparisons & Contrasts

### 1. Imperative vs Declarative Programming

| Feature | Imperative | Declarative |
|---------|-----------|-------------|
| **Focus** | How to perform a task | What the outcome should be |
| **Control** | Step-by-step, explicit instructions | Rules and logic |
| **Side Effects** | Commonly present | Usually avoided |
| **Timing Support** | Good, timing and multitasking possible | Rare, poor control over execution timing |
| **Code Length** | Many lines (very detailed) | Fewer lines (more concise) |
| **Understanding** | Easy (small specific steps) | Can be abstract |
| **Execution Order** | Defined, sequential | Determined by compiler/interpreter |
| **ES Example** | C for traffic light control | SQL for configuration, logic programming |
| **Languages** | C, C++, Java, Python | SQL, Prolog, Haskell |

**When to Use**:
- **Imperative**: When you need precise control over execution, timing-critical systems, embedded systems with resource constraints
- **Declarative**: Configuration, database queries, high-level application logic (but rarely in ES)

### 2. Procedural vs Object-Oriented Programming

| Feature | Procedural | Object-Oriented |
|---------|-----------|-----------------|
| **Structure** | Functions, modules | Objects, classes |
| **Data** | Kept separate from functions | Encapsulated with objects |
| **Reuse** | Through functions | Through inheritance, polymorphism |
| **Complexity** | Simpler | More complex |
| **ES Example** | Device drivers, filtering | Systems with multiple device types |
| **Main Advantage** | Simplicity, easy testing | Natural modeling, extensibility |
| **Main Limitation** | Hard for complex systems | More overhead, complex for small systems |
| **Memory Overhead** | Lower | Higher (vtables, object metadata) |
| **Learning Curve** | Gentler | Steeper |

**When to Use**:
- **Procedural**: Simple embedded systems, resource-constrained devices, straightforward control flow
- **Object-Oriented**: Complex ES with many similar objects, systems benefiting from abstraction and hierarchy

### 3. Event-Driven vs Time-Driven Programming

| Feature | Event-Driven | Time-Driven |
|---------|-------------|-------------|
| **Trigger Source** | External or internal events | Timer interrupts, regular scheduling |
| **Use Case** | Keypad, UI, interrupt-driven controls | Motor control, closed-loop periodic tasks |
| **Predictability** | Can be unpredictable | Highly deterministic timing |
| **Concurrency** | Handlers run as triggered | All handlers scheduled with fixed periods |
| **Drawbacks** | Risk of missed deadlines, contention | Sensor polling, inflexible for sporadic tasks |
| **Response Time** | Immediate (on event) | Delayed until next time slot |
| **CPU Utilization** | Efficient (sleep when idle) | May waste cycles polling |
| **Timing Verification** | Difficult | Easier (known schedule) |

**When to Use**:
- **Event-Driven**: User interfaces, button presses, sporadic external events, interrupt-rich systems
- **Time-Driven**: Safety-critical systems, control loops, predictable periodic tasks, hard real-time requirements

### 4. Non-Preemptive vs Preemptive Event Scheduling

| Feature | Non-Preemptive | Preemptive |
|---------|---------------|------------|
| **Interruption** | Handlers don't interrupt each other | Handlers can interrupt each other |
| **Context Saving** | Not needed (except for interrupts) | Required (stack frames) |
| **Complexity** | Simpler | More complex |
| **Resource Protection** | Easier (no concurrent access) | Requires semaphores/mutexes |
| **Response Time** | Slower (wait for current handler) | Faster (high priority can preempt) |
| **Deadlock Risk** | Lower | Higher |
| **Timing Analysis** | Easier | More complex |

**When to Use**:
- **Non-Preemptive**: Simpler systems, when all events have similar priority, resource-constrained systems
- **Preemptive**: When events have different urgencies, need fast response to critical events

### 5. Simple Periodic vs Generic Time-Triggered Scheduling

| Feature | Simple Periodic | Generic Time-Triggered |
|---------|----------------|----------------------|
| **Task Periods** | All tasks have same period T | Tasks can have different periods |
| **Schedule** | Run all tasks every period | Complex offline schedule (TDL) |
| **Flexibility** | Low | Higher |
| **Start Times** | May be irregular | Precisely controlled |
| **Design Effort** | Minimal | Significant (offline analysis) |
| **Efficiency** | May waste CPU | Better CPU utilization |

**When to Use**:
- **Simple Periodic**: All tasks naturally have same period, simpler systems, prototyping
- **Generic**: Tasks with different natural periods, optimized CPU usage, complex control systems

## Examples & Applications

### 1. Vial Holder System (Procedural Example)

**Application**: Laboratory automation system for handling sample vials

**Procedural Approach**:
```c
// Data structure (separated)
struct sample_holder {
    int num_rows;
    int num_cols;
    char serial_nr[20];
};

// Function (separated from data)
void print_sample_holder(struct sample_holder holder) {
    printf("Vial Holder Information:\n");
    printf("Number of rows: %d\n", holder.num_rows);
    printf("Number of columns: %d\n", holder.num_cols);
    printf("Serial Number: %s\n", holder.serial_nr);
    printf("Total capacity: %d vials\n",
           holder.num_rows * holder.num_cols);
}

// Usage
int main() {
    struct sample_holder holder1;
    holder1.num_rows = 8;
    holder1.num_cols = 12;
    sprintf(holder1.serial_nr, "VH2024-001");
    print_sample_holder(holder1);
}
```

**Key Characteristics**:
- Data and functions separate
- Functions operate on data passed as parameters
- Easy to unit test print_sample_holder()
- No encapsulation

### 2. Rocket Engine System (Object-Oriented Example)

**Application**: SpaceX Starship engine control

**Object-Oriented Approach**:
```cpp
class Rocket_Engine {
private:
    float max_thrust;
    float current_thrust;
    Vector3 thrust_vector;
    float fuel_level;

public:
    void ignite();
    void shutdown();
    void rud();  // Rapid Unscheduled Disassembly
    void setThrust(float percent);
};

class Raptor_Engine : public Rocket_Engine {
    // Inherits all properties and methods
    // Can add specific features
};
```

**Key Characteristics**:
- Data encapsulated with methods
- Inheritance allows specialized engine types
- Natural modeling of real-world objects
- Methods operate on object's own data

### 3. Washing Machine Control (Event-Driven Example)

**Application**: Home appliance control system

**Event-Driven Approach**:
- Events: Button press, door close, water level reached, timer expiration, temperature threshold
- Each event has corresponding handler
- Event queue collects events
- Non-preemptive handler processes events sequentially

**Advantages for This Application**:
- Responsive to user button presses
- Efficient (CPU sleeps between events)
- Natural fit for interrupt-driven system

**Challenges**:
- Must ensure timer events don't miss deadlines
- Long-running handlers could delay critical events

### 4. Airbag Deployment System (Time-Driven Example)

**Application**: Automotive safety system

**Requirements**:
- Hard real-time deadline: ~10-20ms
- Must be deterministic
- Safety-critical

**Time-Driven Approach**:
- Periodic sampling of crash sensors at 1 kHz (1ms period)
- Fixed schedule ensures sensor reading → decision → deployment chain completes in time
- No risk of missed deadline due to unpredictable event timing

**Task Schedule (simplified)**:
| Time (ms) | Task |
|-----------|------|
| 0 | Read accelerometers |
| 0.2 | Process crash detection algorithm |
| 0.5 | Make deployment decision |
| 0.6 | Trigger airbag if needed |
| 1.0 | Repeat (next period) |

**Why Time-Driven**:
- Deterministic timing critical for safety
- Can prove WCET < deadline
- Predictable behavior for certification

### 5. High-Speed Train Control (Hybrid Example)

**Application**: Railway control system (e.g., AGV - French high-speed train)

**Requirements**:
- Hundreds of sensors and actuators
- Many rectifiers, motors, brakes
- Hard and short timing deadlines
- Safety-critical

**Hybrid Approach**:
- **Time-driven core**: Motor control, brake control (periodic, deterministic)
- **Event-driven peripherals**: Door sensors, emergency buttons (sporadic)
- **Object-oriented structure**: Each car/motor/brake as object

**Why Hybrid**:
- Critical control loops need deterministic timing (time-driven)
- User interactions are sporadic (event-driven)
- System complexity benefits from OOP structure

### 6. Traffic Light Controller (Comparison Example)

**Time-Driven Approach**:
```c
// Periodic schedule, e.g., T = 60 seconds
// t=0-30s: North-South green
// t=30-60s: East-West green
```
**Advantages**:
- Predictable light changes
- Deterministic timing
- Easy to verify safety properties

**Disadvantages**:
- Inefficient (lights change even with no traffic)
- Cannot adapt to traffic patterns

**Event-Driven Approach**:
```c
// Events: Car detected, pedestrian button, timer expiration
// Handlers adjust light timing based on traffic
```
**Advantages**:
- Adaptive to traffic
- More efficient

**Disadvantages**:
- Complex timing analysis
- Risk of starvation (one direction never gets green)
- Harder to verify safety

**Best Approach**: Hybrid - time-driven base schedule with event-driven adaptations within bounds

## Connections to Other Topics

### Forward References (Topics to Come)

1. **Requirements (Lecture 03)**
   - Software paradigm choice influenced by functional and non-functional requirements
   - Real-time requirements determine event-driven vs time-driven choice
   - Timing requirements affect schedulability

2. **Modelling (Lecture 04)**
   - State machines commonly used with event-driven paradigm
   - UML class diagrams for object-oriented design
   - Timing diagrams for time-driven systems

3. **Development Processes (Lecture 05)**
   - Paradigm selection occurs during architectural design phase
   - Different paradigms suit different development methodologies

4. **Non-Functional Requirements (Lecture 06)**
   - Performance requirements → imperative programming preferred
   - Maintainability requirements → OOP might be preferred
   - Safety requirements → time-driven for determinism

5. **Energy (Lecture 08)**
   - Event-driven can be more energy-efficient (sleep between events)
   - Time-driven polling wastes energy
   - Paradigm choice affects power consumption

6. **Performance (Lecture 09)**
   - Imperative typically faster than declarative
   - Procedural vs OOP overhead considerations
   - Scheduling paradigm affects response time and throughput

7. **RTOS and Scheduling (Lecture 13)**
   - RTOS enables sophisticated event-driven and time-driven scheduling
   - Task scheduling algorithms implement time-driven paradigm
   - Priority-based preemptive scheduling for event-driven systems

8. **Multi-Processor Systems (Lecture 14)**
   - Paradigms scale differently to multiple processors
   - Event-driven benefits from parallel event handling
   - Time-driven requires synchronized clocks across processors

### Backward References (Prerequisites)

1. **Computer Technology (CT) Course**
   - Structured programming basics (functions, loops, conditionals)
   - Procedural C programming
   - State machines (connects to event-driven)

2. **Lecture 01: Embedded Systems**
   - Cyber-physical systems concept
   - Design space (paradigm choice is part of design space)
   - Resource constraints (influences paradigm selection)
   - Real-time requirements mentioned (now addressed with paradigms)

## Additional Important Notes

### Paradigm Selection Guidelines

**Choose Imperative When**:
- Need precise control over execution
- Performance is critical
- Resource-constrained embedded system
- Close-to-hardware programming needed

**Choose Declarative When**:
- Configuration and rule-based systems
- Database queries
- High-level application logic
- (Rare in deeply embedded systems)

**Choose Procedural When**:
- Straightforward control flow
- Simple embedded systems
- Easy testing is priority
- Resource constraints are tight

**Choose Object-Oriented When**:
- System has many similar objects
- Complexity benefits from abstraction
- Code reuse through inheritance is valuable
- Sufficient resources for overhead

**Choose Event-Driven When**:
- Reacting to external events
- User interface heavy
- Sporadic events common
- Interrupt-rich system
- Soft real-time acceptable

**Choose Time-Driven When**:
- Hard real-time requirements
- Periodic tasks dominate
- Deterministic timing essential
- Safety-critical system
- Predictable behavior required

### Mixing Paradigms

**It's Common to Combine**:
- OOP structure with event-driven event handling
- Time-driven core with event-driven peripherals
- Procedural low-level drivers with OOP application layer

**Example Hybrid Architecture**:
```
Application Layer: Object-Oriented (sensors, actuators as objects)
     ↓
Control Layer: Time-Driven (periodic control loops)
     ↓
Driver Layer: Procedural (hardware drivers)
     ↓
Interrupts: Event-Driven (hardware events)
```

### Real-Time Considerations

**Hard Real-Time** (value = 0 if deadline missed):
- Airbag deployment
- Anti-lock braking
- Nuclear reactor control
- **Requires**: Time-driven or carefully analyzed event-driven with preemption

**Soft Real-Time** (value degrades gracefully):
- Video streaming
- User interface responsiveness
- Network packet handling
- **Can Use**: Event-driven, more flexible scheduling

### Common Pitfalls

**Event-Driven Pitfalls**:
- Long-running handlers blocking critical events
- Event queue overflow
- Priority inversion
- Missed deadlines due to event bursts

**Time-Driven Pitfalls**:
- Inflexible to changing requirements
- Wasted CPU polling sensors
- Difficult to handle sporadic events
- Over-scheduling (too many tasks per period)

**OOP Pitfalls in Embedded**:
- Excessive memory overhead
- Non-deterministic timing (virtual functions)
- Stack overflow from deep inheritance
- Complex for small systems

### Testing Implications

**Procedural Programming**:
- Unit testing straightforward (test each function)
- Easy to mock inputs

**Object-Oriented Programming**:
- Unit testing objects with dependencies more complex
- Need mocking frameworks
- Integration testing important

**Event-Driven Programming**:
- Must test all event sequences
- Difficult to reproduce timing bugs
- Need event injection for testing

**Time-Driven Programming**:
- Timing deterministic → easier verification
- Can prove schedulability mathematically
- Test each frame of schedule

## Possible Exam Questions

### Basic Recall / Definitions

1. **What are the two main software paradigm categories?** Define each and give the fundamental difference.

2. **Define Imperative Programming.** What are its main characteristics?

3. **What is the difference between procedural and object-oriented programming?** Focus on how data and functions are organized.

4. **Define a cyber-physical system.** What are its common components?

5. **What is a Task Descriptor List (TDL)?** When and how is it used?

6. **Define the following terms:**
   - Event Queue
   - Event Handler
   - WCET (Worst-Case Execution Time)
   - Frame (in scheduling context)
   - Side Effect

7. **What is the main characteristic of time-driven programming?** How does it differ from event-driven?

8. **What is inheritance in object-oriented programming?** Give an example.

### Comprehension / Explanation

9. **Explain why procedural programming makes unit testing easier.** What property enables this?

10. **Explain the concept of side effects in imperative programming.** Give a concrete example with code.

11. **Describe how an event queue works in an event-driven system.** What happens when events arrive faster than they can be processed?

12. **Explain the difference between preemptive and non-preemptive event-driven scheduling.** What additional mechanism is needed for preemptive?

13. **Why must sensors be polled in a purely time-triggered system?** What is the trade-off?

14. **Explain why declarative programming is rarely used in embedded systems.** What specific challenge does it face?

15. **Describe the concept of object encapsulation.** How does it differ from procedural data organization?

16. **Explain how a generic time-triggered scheduler differs from a simple periodic scheduler.**

### Application

17. **Given an embedded system for a washing machine with buttons, sensors, and a display:** Which programming paradigm(s) would you choose and why?

18. **You need to implement a motor control system with hard real-time requirements.** Should you use event-driven or time-driven? Justify your choice.

19. **Design a simple event-driven system** for handling button presses and LED control. Draw the event queue and handlers.

20. **You have three tasks with periods 4ms, 6ms, and 12ms.** Create a Task Descriptor List for one major period using a frame size of 2ms.

21. **Convert this imperative code to a more functional/declarative style:**
```c
int sum = 0;
for (int i = 1; i <= 10; i++) {
    sum = sum + i;
}
```

### Analysis / Comparison

22. **Compare imperative and declarative programming paradigms.** When would you use each in embedded systems?

23. **"Imperative programming mainly focuses on: (a) What needs to be done, (b) How tasks are accomplished through steps, (c) Only real-time tasks, (d) Hardware configuration"** - Answer and explain why.

24. **Analyze the trade-offs between procedural and object-oriented programming for a simple embedded system** with 10 similar sensor nodes.

25. **Compare event-driven and time-driven paradigms** for: (a) CPU utilization, (b) Timing predictability, (c) Response to sporadic events.

26. **"What would be a likely drawback of declarative programming in embedded systems?"** Analyze timing and control issues.

27. **Why is unit testing often easier in procedural programming?** Compare with object-oriented approach.

28. **Evaluate preemptive vs non-preemptive event scheduling** for a system with both high-priority emergency events and low-priority logging.

### Synthesis / Design Decisions

29. **Design a scheduler for an embedded system with these tasks:**
   - Read temperature sensor every 5ms (WCET: 1ms)
   - Update display every 20ms (WCET: 3ms)
   - Process user input every 10ms (WCET: 2ms)

   Choose time-driven or event-driven and justify. If time-driven, create a schedule.

30. **You're designing a traffic light controller.** Propose a paradigm (or hybrid) and explain:
   - How would periodic timing be handled?
   - How would pedestrian button presses be handled?
   - How would you ensure safety?

31. **An existing event-driven embedded system is missing deadlines.** What strategies could you use to fix this while keeping the event-driven paradigm?

32. **Design a hybrid system** combining time-driven core control with event-driven user interface. Draw the architecture.

### Critical Thinking / Trade-offs

33. **"In a simple periodic scheduler, what is the main requirement for meeting deadlines?"**
    - A) All tasks are event-triggered only
    - B) Interrupts are never allowed
    - C) Only one handler runs per period
    - D) The sum of the worst-case execution times fits within the scheduling period

    Answer and explain the mathematical relationship.

34. **A student suggests using object-oriented programming for a tiny 8-bit microcontroller with 2KB RAM.** Evaluate this choice and discuss potential issues.

35. **Why might time-driven scheduling be preferred over event-driven for safety-critical systems,** even if event-driven could be more responsive?

36. **Discuss the statement: "Event-driven systems are more energy-efficient than time-driven systems."** When is this true? When might it be false?

37. **In preemptive event scheduling, why must handlers not wait on external events or resources?** What could go wrong?

38. **Explain why mixing multiple paradigms in a single system might be beneficial.** Give a concrete example.

### Scenario-Based Questions

39. **A railway train control system has hundreds of sensors/actuators and hard timing deadlines.** Design the software architecture:
   - What paradigm(s) would you use?
   - How would you structure the system?
   - How would you ensure timing requirements are met?

40. **An airbag deployment system must respond within 15ms of crash detection.**
   - Would you use event-driven or time-driven?
   - Design a simple schedule or event handling strategy
   - How would you verify timing requirements?

41. **You're implementing a laboratory automation system** that controls multiple pipetting robots, each with similar functionality but different positions. Should you use procedural or object-oriented programming? Design the class hierarchy or function structure.

42. **A traffic light controller currently uses a fixed time-driven schedule** (60s cycle). The city wants to add car detection sensors for adaptive timing. How would you modify the system? Would you keep time-driven, switch to event-driven, or use a hybrid?

43. **Compare two approaches for a washing machine controller:**
   - Approach A: Event-driven (button presses trigger state changes)
   - Approach B: Time-driven (periodic polling of buttons and sensors)

   Analyze power consumption, responsiveness, and code complexity.

### Connections & Integration

44. **How does paradigm selection relate to the design space** (energy-cost-performance) introduced in Lecture 01?

45. **A system has both soft real-time (UI) and hard real-time (motor control) requirements.** How would you structure the software using multiple paradigms?

46. **Explain how object-oriented programming's inheritance concept** relates to the idea of reusing device drivers for similar hardware.

47. **How do functional requirements influence paradigm selection?** Give examples of requirements that would favor each paradigm.

### Code Recognition

48. **Given this code snippet, identify the programming paradigm:**
```c
void process_queue() {
    while (!queue_empty()) {
        event_t evt = dequeue();
        switch(evt.type) {
            case BTN_PRESS: handle_button(evt); break;
            case TIMER: handle_timer(evt); break;
        }
    }
}
```

49. **Identify the paradigm and explain key characteristics:**
```c
class Sensor {
    float value;
public:
    virtual float read() = 0;
};
class TempSensor : public Sensor {
public:
    float read() override { return read_hw(); }
};
```

50. **This code demonstrates which programming concept?**
```c
struct motor_t { int speed; int direction; };
void set_motor_speed(struct motor_t* m, int speed) {
    m->speed = speed;
}
```
Explain why this is procedural and not object-oriented.

### Advanced Understanding

51. **Discuss how the choice of programming paradigm affects the testability** of embedded systems. Which paradigm is easiest to test and why?

52. **In a generic time-triggered scheduler, what happens if a task overruns its allocated time slot?** How can this be prevented or detected?

53. **Explain why dynamic memory allocation (malloc/free) is often avoided in embedded systems.** How does this affect the object-oriented paradigm?

54. **Analyze the implications of using virtual functions in C++ for embedded systems.** What timing and memory concerns arise?

55. **How would you implement a time-driven system on a processor with only one timer?** Describe the dispatcher implementation.

56. **Discuss the challenges of debugging event-driven systems** compared to time-driven systems. What tools or strategies would help?

### Meta-Questions About the Course

57. **How does this lecture on software paradigms fit into the overall ESE course structure?** How will paradigm selection influence later topics like energy optimization and performance analysis?

58. **The lecture mentions that paradigms can be mixed in different parts of an ES.** Give an example from one of the course projects (traffic light, washing machine, or AES) where mixing paradigms would be beneficial.