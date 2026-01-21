# Lecture 03: Requirements for Embedded Systems

## Technical Terms & Definitions

- **Requirement**: A documented physical or functional need that a particular design, product, or process aims to satisfy. In embedded systems context: "A requirement describes a behavior or property that the Embedded System shall achieve or implement."

- **Functional Requirement**: Describes what the system should do - its behaviors, functions, and capabilities. Example: "The ES shall beep when connected to a battery."

- **Non-Functional Requirement**: Describes how the system should be - its qualities, constraints, and characteristics. Example: "The ES shall weigh less than 100g."

- **Requirements Engineering**: The systematic process of eliciting, analyzing, specifying, validating, and negotiating requirements from stakeholders.

- **RFC 2119**: Internet standard document defining key words (MUST, SHOULD, MAY, etc.) to indicate requirement levels, widely adopted for requirements specification beyond internet standards.

- **MUST / SHALL / REQUIRED**: Indicates an absolute requirement of the specification - mandatory implementation.

- **MUST NOT / SHALL NOT**: Indicates an absolute prohibition of the specification.

- **SHOULD / RECOMMENDED**: Valid reasons may exist in particular circumstances to ignore the item, but full implications must be understood and carefully weighed.

- **SHOULD NOT / NOT RECOMMENDED**: Valid reasons may exist when the behavior is acceptable or useful, but full implications should be understood and carefully weighed.

- **MAY / OPTIONAL**: The item is truly optional - one vendor may include it while another may omit it.

- **MoSCoW Principle**: Prioritization method for requirements:
  - **Must have**: Critical requirements without which the product cannot function
  - **Should have**: Important but not vital; product can function without them
  - **Could have**: Desirable but not necessary; nice-to-have features
  - **Won't have**: Explicitly out of scope for current iteration

- **Requirements Elicitation**: Process of gathering requirements from stakeholders, existing systems, domain information, standards, and regulations.

- **Requirements Analysis**: Process of examining, refining, and structuring requirements to ensure quality and completeness.

- **Requirements Specification**: Process of documenting requirements in a clear, unambiguous, and structured manner.

- **Requirements Validation**: Process of ensuring requirements are correct, complete, consistent, and testable.

- **Requirements Negotiation**: Process of resolving conflicts between stakeholders and reaching agreement on requirements.

- **UML State Machine**: Graphical notation for modeling the behavior of reactive systems using states, transitions, events, and actions. Extension of classical finite state machines (FSM).

- **Reactive System**: System that responds or reacts to signals from its environment, typical of embedded systems (e.g., smoke detector, ABS, autonomous car).

- **State**: Represents a mode of operation of a system - a condition during which some invariant holds. In UML: rounded rectangle containing the state name.

- **Simple State**: Basic state with no internal structure - atomic unit of behavior.

- **Composite State**: State containing other states (substates), used for hierarchy and concurrency (covered in next lecture).

- **Initial State**: Pseudo-state indicating where the state machine begins execution. Notation: filled black circle.

- **Final State**: Indicates termination of the state machine. Notation: filled black circle with surrounding ring (bullseye).

- **Transition**: Represents a change from one state to another, triggered by events. Notation: arrow from source state to target state.

- **Self-Transition**: Transition where source and target states are the same - causes exit and re-entry of the state.

- **Event**: Internal or external interaction that causes a transition to be triggered, changing system behavior.

- **Signal Event**: Asynchronous notification by another object.

- **Call Event**: Synchronous event representing dispatching of an operation. Notation: `event_name()`.

- **Change Event**: Event triggered when some condition becomes satisfied. Notation: `when(condition)`.

- **Time Event**: Event triggered after a time duration. Notation: `after(60s)`. Only one timeout per state; started on entry, cancelled on exit.

- **Guard**: Boolean condition on a transition that controls whether the transition can be triggered. Notation: `[guard_condition]`.

- **Action**: Atomic operation executed as part of a transition or within a state. Cannot be interrupted.

- **Entry Action**: Action performed when a state becomes active. Notation: `entry/action`.

- **Exit Action**: Action performed right before a state becomes inactive. Notation: `exit/action`.

- **Do Action**: Action performed continuously as long as the state is active. Notation: `do/action`.

- **Port**: Interface point defining messages that can be sent and received by a state machine. Enables communication between FSMs.

- **Link**: Connection for sending messages between ports of different state machines.

- **Impact-Effort Matrix**: Prioritization tool evaluating requirements based on potential impact versus implementation effort.

- **State Space Explosion**: Problem where the number of states in a system grows exponentially, making simple FSMs impractical for complex systems.

## Core Concepts

### Why Requirements Matter

The "tree swing" cartoon illustrates the classic problem in software development: different stakeholders have different understandings of what needs to be built:
- Customer explains one thing
- Project leader understands something different
- Analyst designs another interpretation
- Programmer implements yet another version
- Testers receive something unexpected
- Business consultant describes it differently
- Customer ultimately needed something completely different

Requirements engineering aims to prevent these misunderstandings by creating **clear, documented, agreed-upon specifications**.

Real-world impact: Siemens smoke detector requirements contain over 700 individual requirements covering mechanical properties, packaging, standard compliance, process compliance, product approval, performance, reliability, and design.

### Characteristics of Good Requirements

A "good requirement" must meet multiple criteria:

**1. Unambiguous**
- Only one possible interpretation
- Bad example: "The system shall allow the user to only view one menu entry"
  - Does this mean: only one menu entry can be viewed at a time? Or the user can only view (not edit) one specific menu entry?
- Fix: Be explicit about what "only" modifies and what the actual behavior is

**2. Testable (Verifiable)**
- Must be possible to verify whether requirement is met
- Avoid subjective words: secure, effective, efficient, flexible, maintainable, robust, reliable, user-friendly, good-looking, easy-to-use, simple
- Avoid vague phrases: quickly, securely, with meaningful effort, in a timely manner
- Never use: etc., and/or, TBD, "…"
- Bad example: "The system shall be easy to extend with new functionality"
- Fix: Define measurable criteria (e.g., "Adding a new menu item shall require changes to at most 3 source files")

**3. Clear (Concise, Terse, Simple, Precise)**
- Use straightforward language
- Avoid unnecessary complexity
- Be specific and direct

**4. Correct**
- Requirement accurately reflects stakeholder needs
- Technically feasible
- Aligns with domain knowledge

**5. Understandable**
- All stakeholders can comprehend the requirement
- No specialized jargon unless defined
- Appropriate level of detail for audience

**6. Feasible (Realistic, Possible)**
- Can be implemented with available technology, time, and resources
- No physical impossibilities

**7. Independent**
- Can be understood without referencing other requirements
- Bad example:
  - REQ1: "The list of available flights shall include flight numbers, departure time, and arrival time for every leg of a flight."
  - REQ2: "It should be sorted by price."
- Fix REQ2: "The list of available flights shall be sorted by price in ascending order."

**8. Atomic**
- Addresses a single concern
- Cannot be meaningfully subdivided
- Easier to track, test, and implement

**9. Necessary**
- Requirement is essential for the system
- Not redundant with other requirements

**10. Implementation-Free (Abstract)**
- Describes **what**, not **how**
- Avoids prescribing specific technical solutions
- Gives developers flexibility in implementation

### Requirements Set Characteristics

Beyond individual requirements, the entire set should be:

**Consistent**: No contradictions between requirements
- Example conflict: "System shall respond within 1 second" vs. "System shall perform complex encryption requiring 5 seconds"

**Nonredundant**: Each requirement appears only once
- Avoids maintenance problems when requirements change

**Complete**: No missing information
- All necessary behaviors specified
- All edge cases covered
- All interfaces defined

### RFC 2119 Requirement Levels

**MUST / SHALL / REQUIRED**
- Absolute requirement
- Example: "The traffic light control system MUST comply with 'Wegleitung Lichtsignalanlagen V1.2 (Dez 2020)'"
- Used when: Safety-critical, legally mandated, or essential for basic function

**MUST NOT / SHALL NOT**
- Absolute prohibition
- Example: "The system MUST NOT store passwords in plaintext"
- Used when: Security violations, safety hazards, legal prohibitions

**SHOULD / RECOMMENDED**
- Strong recommendation with possible exceptions
- Example: "The traffic light SHOULD support displaying three colors"
- Implication: Implementation may deviate if fully justified

**SHOULD NOT / NOT RECOMMENDED**
- Generally avoided but exceptions possible
- Example: "The system SHOULD NOT use deprecated APIs"
- Implication: Use only if benefits outweigh costs

**MAY / OPTIONAL**
- Truly optional feature
- Example: "The system MAY support nighttime dimming mode"
- Implication: Different vendors can make different choices

### Requirements Prioritization Strategies

**MoSCoW Method**:
```
Must have     → Product cannot be sold without this
Should have   → Important but not critical to core function
Could have    → Nice-to-have, no extra development cost
Won't have    → Explicitly out of scope
```

**Numerical Priority Scale**:
- **Priority 0**: Requirement is a must. Without it, product cannot be sold at all.
- **Priority 1**: Requirement is a must. Without it, significant feature or USP missing.
- **Priority 2**: Essential but product's primary purpose not harmed if missing.
- **Priority 3**: Nice-to-have that customer would appreciate. Should cause no extra cost.

**Impact-Effort Matrix**:
```
           High Impact
               ↑
   Quick Wins  |  Major Projects
               |
───────────────┼───────────────→ High Effort
               |
   Fill-ins    |  Time Wasters
               |
           Low Impact
```

### Requirements Engineering Process

The RE process is cyclical and iterative:

**Inputs**:
- Existing system information (if replacing/upgrading)
- Stakeholder needs (customers, users, management)
- Organization standards (company policies, best practices)
- External regulations (safety standards, legal requirements)
- Domain information (industry knowledge, technical constraints)

**Process Activities**:
1. **Elicitation**: Gather requirements from all sources
2. **Analysis**: Examine, refine, classify, and structure
3. **Specification**: Document in clear, structured format
4. **Validation**: Ensure correctness, completeness, consistency
5. **Negotiation**: Resolve conflicts, reach stakeholder agreement

**Outputs**:
- Agreed requirements (signed off by stakeholders)
- System specifications (detailed technical documents)
- System models (UML diagrams, state machines, etc.)

This is an **iterative cycle** - feedback from validation leads back to elicitation, analysis continues through specification, etc.

### Modeling for Embedded Systems

**Why model?**
- Complex systems cannot be fully understood from requirements text alone
- Visual representations aid comprehension
- Models enable early verification before implementation
- Facilitate communication between stakeholders
- Can generate code automatically (model-based development)

**Embedded systems as reactive systems**:
- Respond to signals from environment
- Must react within time constraints:
  - Smoke detector: seconds
  - ABS: microseconds
  - Autonomous car: milliseconds

**UML State Machines** are ideal for embedded systems because:
- Naturally represent modes of operation (states)
- Explicitly show event-driven behavior
- Support hierarchical decomposition (composite states)
- Enable concurrent behavior modeling
- Provide formal semantics for verification

### UML State Machine Elements

**States** represent operational modes:
- System exhibits certain behavior while in a state
- Can have entry/do/exit actions
- Types: simple states, composite states (hierarchy/concurrency)

**Transitions** represent mode changes:
- Triggered by events
- Can have guards (conditions)
- Can execute actions
- Self-transitions exit and re-enter same state

**Events** trigger transitions:
- Signal events: asynchronous messages from other objects
- Call events: synchronous operation calls
- Change events: condition becomes true
- Time events: timeout expires

**Actions** are atomic operations:
- Entry actions: executed when entering state
- Exit actions: executed when leaving state
- Do actions: executed while in state
- Transition actions: executed during transition

### State Machine Execution Semantics

**Run-to-Completion**:
- Each event is processed completely before next event
- Transition execution is atomic
- No interruption during actions

**Event Processing**:
1. Event occurs (external stimulus or internal condition)
2. System checks if current state can handle event
3. If yes: execute transition (exit action → transition action → entry action)
4. If no: event is discarded (system ignores it)

**Transition Selection**:
- Multiple transitions from same state: first matching event/guard wins
- Priority: more specific events before generic
- Determinism: at most one enabled transition per event

### State Actions Timing

```
        Entry State A
             ↓
    entry/initialize()    ← Executed once on entry
             ↓
    do/monitor_sensor()   ← Executed continuously
             ↓
    exit/cleanup()        ← Executed once on exit
             ↓
        Leave State A
```

**Entry action use cases**:
- Initialize state-specific variables
- Start timers
- Turn on hardware components

**Do action use cases**:
- Continuous monitoring
- Background processing
- Animation/updating displays

**Exit action use cases**:
- Save state
- Stop timers
- Turn off hardware

### Programming State Machines

**Approach 1: Nested Switch Statements**
```c
switch (current_state) {
    case NO_ALARM:
        switch(event) {
            case SMOKE_DETECTED:
                current_state = ALARM;
                sound_buzzer();
                break;
            case NO_SMOKE:
                // stay in NO_ALARM
                break;
        }
        break;
    case ALARM:
        // ... similar structure
        break;
}
```

**Pros**:
- Straightforward, easy to understand
- Direct mapping from diagram to code

**Cons**:
- Grows quickly with complexity
- Hard to maintain
- Poor readability for large systems
- Code duplication

**Approach 2: Table-Based Implementation**
```c
enum states {NO_ALARM, ALARM} current_state;
enum events {SMOKE_DETECTED, NO_SMOKE_DETECTED} event;
typedef void (*transition_func)();

transition_func state_table[NUM_STATES][NUM_EVENTS] = {
    {handle_smoke_in_no_alarm, handle_no_smoke_in_no_alarm},  // NO_ALARM
    {handle_smoke_in_alarm, handle_no_smoke_in_alarm}          // ALARM
};

// Execute transition:
state_table[current_state][event]();
```

**Pros**:
- Scalable to larger systems
- Clear separation of structure and behavior
- Easy to extend (add row/column)
- Data-driven design

**Cons**:
- Requires function pointers (more complex)
- Indirect execution (harder to debug)
- Overhead of function call

### Interaction Between State Machines

Complex systems partition into multiple communicating FSMs:

**Port**: Interface defining messages that FSM can send/receive
- Output messages → actions of FSM
- Input messages → events of FSM

**Link**: Connection for sending messages between ports

**Example: Button-Lamp System**
```
Button FSM                          Lamp FSM
┌─────────────┐                    ┌──────────┐
│  released   │ pressed/           │   off    │ in.pressed/
│             │ out.pressed        │          │ on
│             ├───────────────────►│          ├──────────┐
└─────────────┘                    └──────────┘          │
      ▲                                  ▲                │
      │ released/                        │                │
      │ out.released                     │ in.released/   │
      │                                  │ off            │
┌─────┴───────┐                    ┌─────┴──────────────▼┐
│   pressed   │                    │         on          │
└─────────────┘                    └─────────────────────┘
```

Button produces events on its output port; Lamp reacts to events on its input port.

### State Space Explosion Problem

Simple FSMs face limitations:
- No hierarchy: flat structure requires many states
- No concurrency: cartesian product of independent behaviors
- No communication: everything in one monolithic FSM

**Example**: Digital watch with time, stopwatch, and alarm features:
- Time mode: 2 states (display, set)
- Stopwatch: 3 states (reset, running, paused)
- Alarm: 2 states (off, set)
- Total states needed: 2 × 3 × 2 = 12 states

With 5 independent features, could need 2^5 = 32 states!

**Solution**: Extensions to FSMs (covered next week)
- Hierarchical states (composite states)
- Concurrent regions (orthogonal states)
- History states
- Communication via events

These extensions, proposed by David Harel in 1984 "Statecharts" paper, form the basis of UML State Machines.

## Important Diagrams & Graphics

### Requirements Motivation: Tree Swing Cartoon

**Description**: Classic 12-panel cartoon illustrating requirements communication breakdown:

**Panel Layout** (3 rows × 4 columns):
- **Panel 1**: How customer explained it - Simple swing hanging from single branch
- **Panel 2**: How project leader understood it - Swing with modified support
- **Panel 3**: How analyst designed it - Complex multi-rope swing arrangement
- **Panel 4**: How programmer wrote it - Structural support with platforms
- **Panel 5**: What beta testers received - Minimalist pole with noose
- **Panel 6**: How business consultant described it - Elaborate tire swing
- **Panel 7**: How project was documented - Bare tree with flagpole
- **Panel 8**: What operations installed - Elaborate mechanical contraption
- **Panel 9**: How customer was billed - Expensive amusement park ride
- **Panel 10**: How it was supported - Single log on ground
- **Panel 11**: What marketing advertised - "iSwing" branded product
- **Panel 12**: What customer really needed - Simple tire swing

**Key Takeaway**: Without clear requirements, each stakeholder has different interpretation, leading to wasted effort and wrong product. Requirements engineering prevents this communication breakdown.

### ESE Advance Organizer Diagram

**Description**: Conceptual framework showing requirements' role in embedded systems engineering:

**Left Box** (Input): Functional & Non-Functional Requirements
- Foundation of system development
- Defines WHAT system must do

**Middle Section** (Process):
- **Top**: Software & Hardware Modelling (state machines, diagrams)
- **Bottom**: Design Space triangle with Energy-Cost-Performance axes
  - Points on triangle: Low-Energy CPU, 1 CPU, 2 CPUs, 1 CPU + FPGA
  - Shows trade-offs between objectives

**Right Box** (Output): Hardware BOM & Software Implementation
- Final deliverable
- WHAT becomes HOW

**Flow**: Requirements → Modeling/Design Space Exploration → Implementation

**Key Takeaway**: Requirements are the starting point; modelling and design space exploration bridge requirements to implementation.

### Requirements Engineering Process Diagram

**Description**: Circular process showing RE activities:

**Five-Stage Cycle**:
```
        Elicitation
             ↓
         Analysis
             ↓
      Specification ←──┐
             ↓         │
       Validation      │
             ↓         │
       Negotiation ────┘
```

**Inputs** (Left side, yellow boxes):
- Existing System Information
- Stakeholder Needs
- Organization Standards
- External Regulations
- Domain Information

**Process** (Center, gray oval):
Requirements Engineering Process

**Outputs** (Right side, green boxes):
- Agreed Requirements
- System Specifications
- System Models

**Key Takeaway**: RE is iterative - validation may reveal need for more elicitation, negotiation may require re-analysis, etc. Not a linear waterfall.

### UML State Machine Elements

**State Representations**:
```
┌──────────────┐    Just name
│   No Alarm   │
└──────────────┘

┌──────────────┐    With compartment separator
│   No Alarm   │
├──────────────┤
│              │
└──────────────┘

┌──────────────┐    With actions
│   No Alarm   │
├──────────────┤
│ entry/snooze │
│ do/sense     │
│ exit/        │
└──────────────┘
```

**Initial and Final States**:
```
  ●  ← Initial State (filled black circle)

 ◉  ← Final State (bullseye: filled circle with ring)
```

**Key Takeaway**: States are rounded rectangles; actions written inside with compartment separator; special pseudo-states for start/end.

### Transition Notation Diagram

**Description**: Anatomy of a transition:

```
Source State                               Target State
┌─────────────┐                           ┌─────────────┐
│  No Alarm   │                           │    Alarm    │
└─────────────┘                           └─────────────┘
      │                                         ▲
      │   event-list [guard] / action-list     │
      └────────────────────────────────────────┘
```

**Components**:
- **Arrow**: Direction of transition
- **event-list**: Events that trigger transition (e.g., "smoke_detected")
- **[guard]**: Optional condition in square brackets (e.g., "[temperature > 100]")
- **action-list**: Actions executed during transition (e.g., "/sound_buzzer")
- **Slash** separates events from actions

**Self-Transition Example**:
```
┌─────────────┐
│    State    │ ←┐
└─────────────┘  │
      └──────────┘
    event / action
```

**Key Takeaway**: Transition syntax is `event-list [guard] / action-list`. All parts optional except transition itself. Self-transitions exit and re-enter state.

### Event Types in UML

**Visual representation of event types**:

```
Signal Event:      smoke_detected
  (asynchronous notification)

Call Event:        button_pressed()
  (synchronous operation call)

Change Event:      when(temperature > threshold)
  (condition becomes true)

Time Event:        after(60s)
  (timeout expires)
```

**Key Takeaway**: Four event types support different interaction patterns. Time events particularly important for embedded systems with time constraints.

### State Actions Execution Timeline

**Description**: Temporal diagram showing when actions execute:

```
Time ──────────────────────────►

        Transition into State A
              ↓
     [entry/initialize() executed]
              ↓
     [do/monitor() executing continuously]
     │                                   │
     │    State A is active              │
     │                                   │
     [do/monitor() still executing]
              ↓
     [exit/cleanup() executed]
              ↓
        Transition out of State A
```

**Key Takeaway**: Entry executes once on entering; do executes continuously while in state; exit executes once on leaving. Order is guaranteed.

### Smoke Detector State Machine Example

**Description**: Complete UML state machine for simplified smoke detector:

```
        ●  (Initial state)
        │
        ▼
┌───────────────┐                      ┌──────────────┐
│   No Alarm    │  Smoke detected      │    Alarm     │
│               ├─────────────────────►│              │
│               │                      │              │
│               │◄─────────────────────┤              │
└───────────────┘  No smoke detected   └──────────────┘
```

**Two states**:
- **No Alarm**: Normal monitoring mode
- **Alarm**: Alert mode when smoke detected

**Two transitions**:
- "Smoke detected" triggers alarm
- "No smoke detected" returns to normal

**Key Takeaway**: Even simple systems benefit from state machine modeling. Makes behavior explicit and testable.

### Programming Approaches Comparison

**Approach 1: Switch-Case Structure**:
```
┌─────────────────────────────┐
│ switch (state)              │
│   ┌─────────────────────┐   │
│   │ case STATE_1:       │   │
│   │   switch (event)    │   │
│   │     case EVENT_A: →│   │
│   │     case EVENT_B: →│   │
│   └─────────────────────┘   │
│   ┌─────────────────────┐   │
│   │ case STATE_2:       │   │
│   │   switch (event)    │   │
│   │     case EVENT_A: →│   │
│   └─────────────────────┘   │
└─────────────────────────────┘
```

**Approach 2: Table-Based**:
```
             Event A    Event B
State 1   │  func1()    func2()  │
State 2   │  func3()    func4()  │

state_table[current_state][event]();
```

**Key Takeaway**: Switch-case is intuitive but doesn't scale; table-based is more maintainable for complex systems.

### Button-Lamp FSM Interaction Diagram

**Description**: Two communicating state machines:

**Object Diagram**:
```
┌────────┐  out    Link    in  ┌──────┐
│ Button ├──────■─────────■────┤ Lamp │
└────────┘                     └──────┘
  (port)                        (port)
```

**Button State Machine**:
```
        ●
        │
        ▼
┌──────────┐ pressed/        ┌─────────┐
│ released │ out.pressed     │ pressed │
│          ├────────────────►│         │
│          │◄────────────────┤         │
└──────────┘ released/       └─────────┘
            out.released
```

**Lamp State Machine**:
```
        ●
        │
        ▼
┌──────┐ in.pressed/    ┌────┐
│ off  │ on             │ on │
│      ├───────────────►│    │
│      │◄───────────────┤    │
└──────┘ in.released/   └────┘
        off
```

**Message Flow**:
1. Button transitions to "pressed" → sends "out.pressed" message
2. Message travels over link
3. Lamp receives "in.pressed" event → transitions to "on"

**Key Takeaway**: FSMs communicate via messages through ports and links. Output messages from one FSM become input events for another.

### Modelling Examples Across Domains

**Description**: Four examples showing modeling applies everywhere:

**Top Left**: Electronic circuit schematic (RF amplifier with transistors, capacitors, inductors)
- Shows: Hardware modeling with components and connections

**Bottom Left**: Football play diagram (tactical formation with player movements)
- Shows: Strategic/behavioral modeling with positions and actions

**Top Right**: Architectural floor plan (apartment layout with rooms, doors, fixtures)
- Shows: Spatial/structural modeling with dimensions and relationships

**Bottom Right**: 3D CAD model (building exterior with curved features)
- Shows: Geometric modeling with surfaces and volumes

**Key Takeaway**: Modeling is universal technique across engineering disciplines. For embedded systems, we model behavior (state machines) rather than geometry or topology.

### Traffic Light Intersection Diagram

**Description**: Overhead view of crossroad with traffic lights:

```
              │    ║
              │    ║
──────────────┼────║───────────
              │░░░░║
              │░░░░║  Pedestrian
──────────────┼────║───────────  crossing
══════════════╬════╬═══════════
              ║    │
              ║    │
──────────────║────┼───────────
              ║░░░░│
              ║░░░░│  Pedestrian
──────────────║────┼───────────  crossing
              ║    │
              ║    │
```

**Traffic lights** at each arm of intersection (4 total)
**Pedestrian crossings** on each road (marked with stripes)
**Control challenge**: Coordinate lights to prevent collisions, optimize flow

**Modeling question**: Do we model:
- The entire intersection as one FSM? (many states!)
- Each individual traffic light as separate FSM? (simpler, modular)
- Some intermediate decomposition?

**Key Takeaway**: System partitioning decision affects model complexity. "Divide and conquer" - model individual traffic light, then compose.

## Formulas & Procedures

### Requirements Quality Checklist

**Formula for evaluating requirement quality**:
Check each characteristic:
```
✓ Unambiguous      - Only one interpretation possible
✓ Testable         - Can verify if met
✓ Clear            - Concise, simple, precise
✓ Correct          - Accurately reflects need
✓ Understandable   - All stakeholders comprehend
✓ Feasible         - Can be implemented
✓ Independent      - Self-contained
✓ Atomic           - Single concern
✓ Necessary        - Essential for system
✓ Implementation-free - Describes what, not how
```

**Quality Score**: Number of criteria met / 10 total

### RFC 2119 Requirement Template

**Standard format**:
```
[ID] [Priority] [Subject] [KEYWORD] [Predicate] [Object] [Condition]

Examples:
REQ-001 [MUST] The system SHALL sound alarm WHEN smoke detected
REQ-002 [SHOULD] The display SHOULD show battery level IF battery-powered
REQ-003 [MAY] The device MAY support Bluetooth connectivity
```

**Keywords hierarchy** (from strongest to weakest):
```
MUST/SHALL/REQUIRED          ← Mandatory
  ↓
SHOULD/RECOMMENDED           ← Strong recommendation
  ↓
MAY/OPTIONAL                 ← Truly optional
```

### Requirements Engineering Process Steps

**Step-by-step procedure**:

**1. Elicitation Phase**
```
FOR each stakeholder DO
  - Conduct interviews
  - Observe workflow
  - Analyze existing systems
  - Review domain documents
  - Identify needs and constraints
END FOR
```

**2. Analysis Phase**
```
FOR each elicited requirement DO
  - Classify (functional/non-functional)
  - Refine (make specific)
  - Check quality (use checklist)
  - Identify conflicts
  - Prioritize (MoSCoW or numeric)
END FOR
```

**3. Specification Phase**
```
FOR each analyzed requirement DO
  - Assign unique ID
  - Write in standard template
  - Add priority level
  - Define acceptance criteria
  - Link to stakeholders
END FOR
```

**4. Validation Phase**
```
FOR each specified requirement DO
  - Check consistency with others
  - Verify testability
  - Confirm with stakeholders
  - Identify gaps
END FOR
```

**5. Negotiation Phase**
```
WHILE conflicts exist DO
  - Present trade-offs to stakeholders
  - Facilitate discussion
  - Reach compromise or make decision
  - Update requirements
END WHILE
```

### Requirement Refinement Procedure

**From system to subsystem requirements**:

1. **Understand all system requirements**
   - Read completely
   - Clarify ambiguities
   - Identify dependencies

2. **Classify requirements**
   - Functional vs. non-functional
   - Hardware vs. software vs. mechanical

3. **Partition system into subsystems**
   - Identify logical boundaries
   - Define interfaces between subsystems
   - Example: Separate traffic light control from individual light FSM

4. **Refine & filter requirements for each subsystem**
   - Allocate system requirements to subsystems
   - Derive new subsystem-specific requirements
   - Remove requirements not applicable to subsystem

5. **Create subsystem specification**
   - Document what subsystem will do
   - Describe how it fulfills allocated requirements
   - Define interfaces to other subsystems

6. **Model subsystem functional behavior**
   - Create UML state machines
   - Define states, transitions, events, actions
   - Validate model against requirements

### State Machine Construction Procedure

**Step 1: Identify States**
```
FOR each distinct mode of operation DO
  Create state with descriptive name
END FOR
```

**Step 2: Identify Events**
```
FOR each external stimulus or condition DO
  Create event with clear name
  Classify type (signal/call/change/time)
END FOR
```

**Step 3: Define Transitions**
```
FOR each event in each state DO
  IF event causes state change THEN
    Create transition to target state
    Label with: event [guard] / actions
  ELSE IF event causes action in same state THEN
    Create self-transition
  ELSE
    Event ignored in this state
  END IF
END FOR
```

**Step 4: Add Entry/Exit/Do Actions**
```
FOR each state DO
  IF initialization needed THEN
    Add entry action
  END IF
  IF cleanup needed THEN
    Add exit action
  END IF
  IF continuous processing needed THEN
    Add do action
  END IF
END FOR
```

**Step 5: Designate Initial State**
```
Add initial state pseudo-state
Add transition from initial to first real state
```

**Step 6: Add Final State (if applicable)**
```
IF state machine terminates THEN
  Add final state pseudo-state
  Add transitions from terminal states to final
END IF
```

### State Machine Execution Algorithm

**Run-to-completion semantics**:

```
current_state = initial_state

WHILE true DO
  event = get_next_event()

  IF event == NULL THEN
    CONTINUE  // No event to process
  END IF

  // Find enabled transition
  FOR each transition FROM current_state DO
    IF transition.event matches event THEN
      IF transition.guard == true THEN
        // Execute transition
        execute(current_state.exit_action)
        execute(transition.action)
        current_state = transition.target_state
        execute(current_state.entry_action)
        BREAK  // Event consumed
      END IF
    END IF
  END FOR

  // If no transition found, event is discarded
END WHILE
```

### Switch-Case Implementation Pattern

**Template for switch-case FSM**:

```c
void process_event(enum event_t event) {
  switch (current_state) {
    case STATE_A:
      switch (event) {
        case EVENT_1:
          // Execute transition actions
          action_for_A_to_B();
          // Change state
          current_state = STATE_B;
          break;
        case EVENT_2:
          // Self-transition or internal action
          action_in_A();
          break;
        default:
          // Event ignored
          break;
      }
      break;

    case STATE_B:
      switch (event) {
        case EVENT_1:
          action_for_B_to_A();
          current_state = STATE_A;
          break;
        // ... other events
      }
      break;

    // ... other states
  }
}
```

### Table-Based Implementation Pattern

**Template for table-driven FSM**:

```c
// Define function pointer type
typedef void (*transition_func_t)(void);

// Declare transition functions
void trans_A1(void);  // From STATE_A on EVENT_1
void trans_A2(void);  // From STATE_A on EVENT_2
void trans_B1(void);  // From STATE_B on EVENT_1
void trans_B2(void);  // From STATE_B on EVENT_2

// Build state transition table
transition_func_t state_table[NUM_STATES][NUM_EVENTS] = {
  // STATE_A row
  {trans_A1, trans_A2},
  // STATE_B row
  {trans_B1, trans_B2}
};

// Process event using table
void process_event(enum event_t event) {
  if (event < NUM_EVENTS && current_state < NUM_STATES) {
    // Look up and execute transition function
    state_table[current_state][event]();
  }
}

// Each transition function handles state change
void trans_A1(void) {
  // Execute actions
  perform_action();
  // Change state
  current_state = STATE_B;
}
```

## Comparisons & Contrasts

### Functional vs. Non-Functional Requirements

| Aspect | Functional Requirements | Non-Functional Requirements |
|--------|------------------------|----------------------------|
| **Definition** | What the system shall do | How the system shall be |
| **Focus** | Behaviors, functions, capabilities | Qualities, constraints, characteristics |
| **Examples** | "Shall beep when battery low" | "Shall weigh less than 100g" |
| **Testability** | Test with specific inputs | Measure properties |
| **Implementation** | Directly maps to features | Affects architecture/design |
| **Change Impact** | Often localized | May require system-wide changes |
| **Stakeholder** | Primarily users | Users, operators, maintainers |
| **Typical Count** | 50-500 for medium system | 20-100 for medium system |

### RFC 2119 Keywords Comparison

| Keyword | Strength | Flexibility | When to Use | Example Context |
|---------|----------|-------------|-------------|-----------------|
| **MUST** | Absolute | None | Safety, legal, critical function | "MUST encrypt passwords" |
| **MUST NOT** | Absolute | None | Safety hazard, legal prohibition | "MUST NOT store credit cards" |
| **SHOULD** | Strong | Justified exceptions | Best practice, recommended | "SHOULD use HTTPS" |
| **SHOULD NOT** | Strong | Justified exceptions | Generally avoided | "SHOULD NOT use globals" |
| **MAY** | Weak | Full | Optional feature | "MAY support voice control" |

**Decision tree**:
```
Is it mandatory?
├─ Yes, no exceptions → MUST / MUST NOT
└─ No
   ├─ Strongly recommended? → SHOULD / SHOULD NOT
   └─ Truly optional? → MAY
```

### MoSCoW vs. Numeric Priority

| Aspect | MoSCoW Method | Numeric Priority (0-3) |
|--------|---------------|------------------------|
| **Simplicity** | Very simple, 4 categories | Simple, 4 levels |
| **Granularity** | Coarse (Must/Should/Could/Won't) | Slightly finer (0/1/2/3 with descriptions) |
| **Interpretation** | Clear meaning for stakeholders | Requires definition document |
| **Negotiation** | Facilitates discussion | More quantitative feel |
| **Implementation** | Easy in tools (labels/tags) | Easy in tools (numbers) |
| **Scope Changes** | Won't have = explicit scope boundary | Priority 3 = maybe later |
| **Best For** | Agile, time-boxed projects | Traditional, fixed-scope projects |

### Individual Requirement vs. Requirements Set Criteria

| Criteria Level | Individual Requirement | Requirements Set |
|----------------|----------------------|------------------|
| **Unambiguous** | Single interpretation | N/A |
| **Testable** | Can verify fulfillment | N/A |
| **Clear** | Understandable statement | N/A |
| **Consistent** | Doesn't contradict itself | No contradictions between requirements |
| **Complete** | Contains all necessary info | No missing requirements for system |
| **Independent** | Self-contained | N/A |
| **Nonredundant** | Single atomic concern | Each requirement appears once |
| **Scope** | One requirement | Entire system specification |

**Key insight**: Some quality criteria apply to individual requirements (testable, atomic), while others apply to the set as a whole (consistent, complete, nonredundant).

### Requirements Engineering vs. Requirements Management

| Aspect | Requirements Engineering | Requirements Management |
|--------|------------------------|------------------------|
| **Definition** | Creating and defining requirements | Tracking and controlling requirements |
| **Activities** | Elicitation, analysis, specification, validation | Version control, change control, traceability |
| **Timing** | Early in project lifecycle | Throughout project lifecycle |
| **Focus** | Content of requirements | Metadata about requirements |
| **Key Question** | "What should the system do?" | "How are requirements evolving?" |
| **Outputs** | Requirements document, models | Change history, traceability matrix |
| **Tools** | Interviews, workshops, prototypes | Requirements management tools (DOORS, Jira) |

### Classical FSM vs. UML State Machine

| Feature | Classical FSM | UML State Machine |
|---------|---------------|-------------------|
| **States** | Simple states only | Simple + composite states |
| **Hierarchy** | Flat structure | Hierarchical nesting |
| **Concurrency** | Single active state | Orthogonal regions (parallel states) |
| **Actions** | On transitions only | Entry/do/exit + transitions |
| **Communication** | Not specified | Ports, events, signals |
| **History** | No memory of past states | History pseudo-states |
| **Guards** | Limited support | Full boolean expressions |
| **Expressiveness** | Basic | Rich |
| **Complexity** | State explosion | Manageable via hierarchy |
| **Origins** | 1950s automata theory | 1984 Harel Statecharts → UML |

### Simple State vs. Composite State

| Aspect | Simple State | Composite State |
|--------|-------------|----------------|
| **Internal Structure** | None (atomic) | Contains substates |
| **Purpose** | Represent single mode | Group related modes |
| **Transitions** | To/from external states | Can cross hierarchy levels |
| **Entry/Exit** | Single entry/exit point | Entry/exit to/from any substate |
| **Active State** | Single state active | One state per region active |
| **Complexity** | Low | Higher (but reduces overall) |
| **Example** | "Alarm" in smoke detector | "Running" with substates "Normal", "Turbo" |

### Event Types Comparison

| Event Type | Timing | Source | Example | Use Case |
|------------|--------|--------|---------|----------|
| **Signal** | Asynchronous | External object | `button_pressed` | User input, sensor data |
| **Call** | Synchronous | External caller | `stop()` | API calls, function invocation |
| **Change** | Asynchronous | Condition | `when(temp > 100)` | Threshold monitoring |
| **Time** | Asynchronous | Clock | `after(5s)` | Timeouts, periodic tasks |

**Selection guide**:
- User/sensor input → Signal event
- Function/method call → Call event
- Monitoring thresholds → Change event
- Timeouts/delays → Time event

### Entry vs. Exit vs. Do Actions

| Action Type | Execution | Timing | Interruption | Typical Use |
|-------------|-----------|--------|--------------|-------------|
| **Entry** | Once | On entering state | No (atomic) | Initialize variables, start timer |
| **Exit** | Once | On leaving state | No (atomic) | Clean up, save state, stop timer |
| **Do** | Continuously | While in state | Yes (can be preempted) | Monitor sensor, update display |

**Execution sequence**:
```
Previous State exit action
     ↓
Transition action
     ↓
New State entry action
     ↓
New State do action (continuous)
```

### Self-Transition vs. Internal Transition

| Aspect | Self-Transition | Internal Transition |
|--------|----------------|---------------------|
| **Notation** | Arrow from state to itself | Event in state compartment |
| **Exit Action** | Executed | NOT executed |
| **Entry Action** | Executed | NOT executed |
| **State Change** | Conceptually leaves and re-enters | Stays in state |
| **Use Case** | Reset state to initial conditions | Handle event without reset |
| **Example** | Counter resets to 0 | Counter increments by 1 |

**Note**: Internal transitions not covered in this lecture (simple UML State Machines only), but important distinction for next lecture.

### Switch-Case vs. Table-Based Implementation

| Aspect | Switch-Case | Table-Based |
|--------|-------------|-------------|
| **Code Structure** | Nested switches | Function pointer array |
| **Readability** | Good for small FSMs | Better for large FSMs |
| **Maintainability** | Degrades with size | Scales well |
| **Extensibility** | Add case statements | Add row/column |
| **Performance** | Compiler optimized | Function call overhead |
| **Debugging** | Easier (direct code) | Harder (indirect) |
| **Size** | Grows linearly | Grows quadratically (states × events) |
| **Ideal For** | <10 states, <10 events | >10 states or >10 events |

### Requirements Elicitation Techniques

| Technique | Interaction | Time | Detail | Best For |
|-----------|-------------|------|--------|----------|
| **Interviews** | One-on-one | Hours | High | Individual expert knowledge |
| **Workshops** | Group | Days | Medium | Building consensus |
| **Observation** | Passive | Days-Weeks | Very High | Actual workflow |
| **Questionnaires** | Remote | Weeks | Low | Large user base |
| **Prototyping** | Interactive | Weeks | High | Unclear requirements |
| **Document Analysis** | Individual | Days | Medium | Existing system replacement |

## Examples & Applications

### Example 1: Good vs. Bad Requirements

**Bad Requirement Examples**:

1. **Ambiguous**: "The traffic light should support displaying three colors."
   - What does "should" mean? SHOULD or MUST?
   - What three colors specifically?
   - Support displaying simultaneously or sequentially?

2. **Not Testable**: "The system shall be user-friendly."
   - How do you measure user-friendliness?
   - Subjective term with no clear criteria

3. **Not Independent**: "It should be sorted by price."
   - What is "it"? Requires context from another requirement
   - Pronouns make requirements dependent

**Good Requirement Examples**:

1. **Clear and Testable**: "The traffic light control system MUST count the number of cars waiting per lane."
   - Specific: per lane granularity
   - Testable: can verify counting functionality
   - Uses RFC 2119 keyword (MUST)

2. **Specific and Complete**: "The list of available flights SHALL include flight numbers, departure time, and arrival time for every leg of a flight."
   - Lists all required fields
   - Specifies granularity (every leg)
   - Uses SHALL for mandatory requirement

3. **Compliance Requirement**: "The traffic light control system MUST comply with 'Wegleitung Lichtsignalanlagen V1.2 (Dez 2020) Dokumentennummer: 601.06.40'"
   - References specific standard
   - Includes version and document number
   - Unambiguous (standard is testable)

### Example 2: Siemens Smoke Detector Requirements

**Scope**: Over 700 individual requirements covering:

**Mechanical Properties** (examples):
- "Device SHALL have dimensions not exceeding 120mm diameter × 50mm height"
- "Mounting SHALL support ceiling or wall installation"
- "Housing MUST withstand drop test from 2m height"

**Standard Compliance** (examples):
- "MUST comply with EN 14604:2005"
- "MUST meet UL 217 certification requirements"
- "SHALL pass VdS 3131 smoke detector standard"

**Performance** (examples):
- "SHALL detect smoke within 60 seconds at 0.12 dB/m obscuration"
- "Alarm MUST sound at 85 dB at 3m distance"
- "Battery life SHALL exceed 10 years in standby"

**Process Compliance** (examples):
- "Manufacturing SHALL follow ISO 9001:2015"
- "Testing SHALL include 100% functional test before shipment"

**Reliability** (examples):
- "MTBF SHALL exceed 10 years"
- "False alarm rate SHOULD be less than 1 per year"

**Key insight**: Real industrial products have hundreds of requirements spanning multiple domains. Requirements engineering is a major undertaking.

### Example 3: Kitchen Requirements Prioritization

**Must Have (Priority 0)**:
- REQ-001: "There MUST be an oven in the kitchen"
- REQ-002: "The kitchen SHALL have running water"
- REQ-003: "The kitchen MUST comply with local building codes"

**Should Have (Priority 1)**:
- REQ-004: "A dishwasher SHOULD be installed"
- REQ-005: "The kitchen SHOULD have a garbage disposal"

**Could Have (Priority 2)**:
- REQ-006: "A hook MAY be available to hang towels close to the sink"
- REQ-007: "The kitchen COULD have under-cabinet lighting"

**Won't Have**:
- REQ-008: "The kitchen WON'T have a walk-in refrigerator" (residential context)

**Rationale**:
- Without oven or water, not a functional kitchen (Priority 0)
- Dishwasher highly desirable but kitchen works without it (Priority 1)
- Towel hook nice-to-have with minimal cost (Priority 2)
- Walk-in refrigerator inappropriate for context (Won't have)

### Example 4: Smoke Detector State Machine

**Requirements** (simplified):
1. System SHALL monitor for smoke continuously
2. System MUST sound alarm when smoke detected
3. System SHALL return to monitoring when smoke clears
4. Alarm MUST sound for minimum 30 seconds

**State Machine Implementation**:
```
        ● Initial State
        │
        ▼
┌────────────────────┐
│     No Alarm       │ Smoke detected /
│                    │ sound_alarm()
│ entry/snooze_alarm │─────────────────────────┐
│ do/sense_smoke()   │                         │
│ exit/              │                         ▼
└────────────────────┘                  ┌──────────────┐
        ▲                               │    Alarm     │
        │                               │              │
        │ No smoke detected             │ entry/       │
        │                               │ do/sound()   │
        │                               │ exit/mute()  │
        └───────────────────────────────┤              │
          after(30s) AND                └──────────────┘
          no smoke
```

**State descriptions**:
- **No Alarm**:
  - Entry: Snooze any previous alarm
  - Do: Continuously monitor smoke sensor
  - Exit: (none needed)

- **Alarm**:
  - Entry: (handled by transition action)
  - Do: Sound alarm continuously
  - Exit: Mute alarm

**Transitions**:
- "Smoke detected" with action "sound_alarm()"
- "No smoke detected" with guard "[after(30s)]" to enforce minimum alarm duration

**Key insight**: State machine directly maps to requirements. Each requirement traces to states, transitions, or actions.

### Example 5: Traffic Light State Machine (Single Light)

**Simplified Single Traffic Light**:

**Requirements**:
1. Light SHALL cycle through Red → Red-Yellow → Green → Yellow → Red
2. Red MUST last minimum 30 seconds
3. Green MUST last minimum 20 seconds
4. Yellow SHALL last 3 seconds
5. Red-Yellow SHALL last 2 seconds

**State Machine**:
```
        ● Initial
        │
        ▼
┌──────────┐ after(30s)    ┌─────────────┐
│   Red    │──────────────►│ Red-Yellow  │
│          │                │             │
└──────────┘                └─────────────┘
      ▲                            │
      │                            │ after(2s)
      │                            │
      │                            ▼
┌──────────┐ after(3s)     ┌─────────────┐
│  Yellow  │◄───────────────│   Green     │
│          │                │             │
└──────────┘                └─────────────┘
                            after(20s)
```

**State actions**:
- Red: `entry/set_red_LED(ON)`
- Red-Yellow: `entry/set_red_LED(ON); set_yellow_LED(ON)`
- Green: `entry/set_green_LED(ON)`
- Yellow: `entry/set_yellow_LED(ON)`
- All: `exit/set_all_LEDs(OFF)`

**Key insight**: Time events enforce timing requirements. State machine provides visual verification that sequence is correct.

### Example 6: Button-Lamp System

**System Partitioning**:
Instead of one monolithic FSM with combined button-lamp states, partition into two communicating FSMs:

**Button FSM** (Producer):
```
        ●
        │
        ▼
┌──────────┐ pressed/        ┌─────────┐
│ Released │ out.pressed     │ Pressed │
│          ├────────────────►│         │
│          │◄────────────────┤         │
└──────────┘ released/       └─────────┘
            out.released
```
- Handles physical button state
- Produces events on output port

**Lamp FSM** (Consumer):
```
        ●
        │
        ▼
┌──────┐ in.pressed/    ┌────┐
│ Off  │ on()           │ On │
│      ├───────────────►│    │
│      │◄───────────────┤    │
└──────┘ in.released/   └────┘
        off()
```
- Controls lamp hardware
- Reacts to events on input port

**Communication**:
- Button "pressed" → sends `out.pressed` → Lamp receives `in.pressed`
- Button "released" → sends `out.released` → Lamp receives `in.released`

**Benefits of partitioning**:
- Modularity: Button and Lamp are independent
- Reusability: Button FSM can control multiple lamps
- Scalability: Easy to add more consumers
- Testability: Can test each FSM independently

### Example 7: Impact-Effort Matrix Application

**Smart Thermostat Feature Requests**:

**High Impact, Low Effort** (Quick Wins - Do First):
- REQ-10: Add Celsius/Fahrenheit toggle (Priority: High)
- REQ-15: Show current humidity (Priority: High)

**High Impact, High Effort** (Major Projects - Plan Carefully):
- REQ-01: Add voice control integration (Priority: High)
- REQ-05: Implement learning algorithm for preferences (Priority: High)

**Low Impact, Low Effort** (Fill-ins - Do If Time):
- REQ-20: Change button color scheme (Priority: Low)
- REQ-22: Add more ringtones for alerts (Priority: Low)

**Low Impact, High Effort** (Time Wasters - Avoid):
- REQ-18: Implement blockchain-based temperature logging (Priority: Very Low)
- REQ-25: Add support for 50+ international languages (Priority: Very Low)

**Decision**:
1. Implement REQ-10 and REQ-15 immediately (quick wins)
2. Plan for REQ-01 and REQ-05 in next major release
3. Add REQ-20 and REQ-22 if sprint has extra capacity
4. Reject REQ-18 and REQ-25 (not worth effort)

### Example 8: Requirement Ambiguity Resolution

**Ambiguous Requirement**:
"The system shall allow the user to only view one menu entry."

**Possible Interpretations**:
1. The user can only view (not edit) exactly one menu entry
2. Only one menu entry can be displayed at a time
3. The user has permission to view only one specific menu entry
4. Menu entries can only be viewed, no other operations allowed

**Clarification Process**:
1. **Identify ambiguity**: "only" modifier position unclear
2. **Generate interpretations**: List possible meanings
3. **Consult stakeholders**: Ask which meaning intended
4. **Rewrite clearly**:

**Disambiguated Requirements**:
- If meaning 1: "The system SHALL allow the user to view, but not edit, one menu entry at a time."
- If meaning 2: "The system SHALL display exactly one menu entry at a time."
- If meaning 3: "The user SHALL have permission to view menu entry X and no other menu entries."
- If meaning 4: "The system SHALL allow only the view operation on menu entries. Edit, delete, and create operations SHALL NOT be permitted."

**Key lesson**: Identify ambiguity early through multiple interpretations test. If you can think of two meanings, it's ambiguous.

### Example 9: Verifying Testability

**Untestable Requirement**:
"The system shall respond quickly to user input."

**Problem**: "Quickly" is subjective and unmeasurable

**Making It Testable**:
1. **Define metric**: Response time in milliseconds
2. **Set threshold**: 90% of operations under 100ms
3. **Specify conditions**: Under normal load
4. **Define test method**: Automated timing tests

**Testable Version**:
"The system SHALL respond to user button presses within 100 milliseconds for 90% of presses under normal operating load (up to 10 presses per second)."

**Test Procedure**:
```
TEST: Response Time
1. Setup: Configure system with normal load
2. Execute: Press button 100 times at varying rates up to 10 Hz
3. Measure: Record response time for each press
4. Evaluate: Calculate percentage under 100ms threshold
5. Pass Criteria: ≥90% of responses under 100ms
```

**Key insight**: Testable requirements include:
- Measurable metric (milliseconds)
- Quantitative threshold (100ms, 90%)
- Defined conditions (normal load, ≤10 Hz)
- Clear pass/fail criteria

### Example 10: Table-Based FSM Implementation

**Complete Example: Simplified Alarm System**

**States**: `DISARMED`, `ARMED`, `ALARM`
**Events**: `ARM_PRESSED`, `DISARM_PRESSED`, `SENSOR_TRIGGERED`

```c
// State and event enums
typedef enum {DISARMED, ARMED, ALARM, NUM_STATES} state_t;
typedef enum {ARM_PRESSED, DISARM_PRESSED, SENSOR_TRIGGERED, NUM_EVENTS} event_t;

// Global state variable
state_t current_state = DISARMED;

// Transition function declarations
void disarmed_arm_pressed(void);
void disarmed_disarm_pressed(void);
void disarmed_sensor_triggered(void);
void armed_arm_pressed(void);
void armed_disarm_pressed(void);
void armed_sensor_triggered(void);
void alarm_arm_pressed(void);
void alarm_disarm_pressed(void);
void alarm_sensor_triggered(void);

// Function pointer type
typedef void (*transition_t)(void);

// State transition table
transition_t state_table[NUM_STATES][NUM_EVENTS] = {
    // DISARMED row
    {disarmed_arm_pressed, disarmed_disarm_pressed, disarmed_sensor_triggered},
    // ARMED row
    {armed_arm_pressed, armed_disarm_pressed, armed_sensor_triggered},
    // ALARM row
    {alarm_arm_pressed, alarm_disarm_pressed, alarm_sensor_triggered}
};

// Event processing
void process_event(event_t event) {
    if (event < NUM_EVENTS && current_state < NUM_STATES) {
        state_table[current_state][event]();
    }
}

// Transition function implementations
void disarmed_arm_pressed(void) {
    printf("Arming system...\n");
    start_entry_timer();
    current_state = ARMED;
}

void armed_sensor_triggered(void) {
    printf("ALARM! Sensor triggered!\n");
    sound_siren();
    current_state = ALARM;
}

void alarm_disarm_pressed(void) {
    printf("Alarm disarmed\n");
    stop_siren();
    current_state = DISARMED;
}

// ... implement remaining transition functions
```

**Key advantages demonstrated**:
- Easy to add new state: add row to table
- Easy to add new event: add column to table
- Clear separation of FSM structure (table) and behavior (functions)
- Extensible without modifying dispatch logic

## Connections to Other Topics

### Forward Connections (Topics Covered Later)

**→ Lecture 04 (Modelling)**:
- Requirements provide input for modeling process
- State machines from this lecture extend to hierarchical and concurrent state machines
- Modeling techniques validate that requirements are complete and consistent

**→ Lab 02_U01 (Requirements Modeling)**:
- Hands-on practice writing requirements for traffic light system
- Creating UML state machines from requirements
- Experience iterative refinement process

**→ Lab 03_U2_U3 (Implementation & Test)**:
- Requirements guide implementation decisions
- State machines implemented in code
- Test cases derived from requirements (requirement-based testing)

**→ Lecture 06 (Non-Functional Requirements)**:
- Deep dive into non-functional requirements (performance, energy, reliability)
- Quantifying "quality" requirements
- Trade-offs between competing NFRs

**→ Lecture 08 (Energy)**:
- Energy consumption is a non-functional requirement
- State machines help identify power states
- Transitions between states have energy costs

**→ Lecture 09 (Performance)**:
- Timing requirements from this lecture (time events) become performance requirements
- State machine execution time affects real-time guarantees
- WCET (Worst-Case Execution Time) analysis of state machines

**→ Lecture 13 (RTOS Scheduling)**:
- State machines become tasks in RTOS
- Timing requirements become deadlines
- Schedulability analysis verifies requirements can be met

### Backward Connections (Prerequisites from Earlier Topics)

**← Lecture 01 (Embedded Systems)**:
- Embedded systems are reactive systems → need reactive modeling (state machines)
- Resource constraints affect requirements (memory, power, cost)
- Design space exploration starts with requirements as constraints
- Cyber-physical nature means requirements span hardware and software

**← Lecture 02 (Software Paradigms)**:
- Event-driven paradigm directly maps to state machine events
- Time-driven paradigm implemented via time events in state machines
- State machines can be implemented in any paradigm (procedural, OOP)
- Procedural: switch-case or table-based
- OOP: State pattern with state objects

**← Lab 01_P1 (Hardware Bring-up)**:
- Requirements for labs guide what hardware features to use
- State machines will control LEDs, buttons on Ultra96
- Cross-compilation requirements apply to state machine code

### Related Concepts from Broader CS/Engineering Knowledge

**Software Engineering**:
- **Requirements Engineering** is a major SE discipline with its own methodologies, tools, and best practices
- **V-Model**: Requirements at top left, validation at top right - traceability crucial
- **Agile**: User stories are lightweight requirements; MoSCoW fits agile prioritization
- **DevOps**: Requirements drive acceptance criteria in CI/CD pipelines

**Formal Methods**:
- **Model Checking**: State machines can be formally verified for properties (safety, liveness)
- **Temporal Logic**: Requirements like "always" or "eventually" expressible formally
- **Theorem Proving**: Can prove state machine satisfies requirements

**Control Systems**:
- **State Space Models**: Different from state machines but both model system behavior
- **Finite Automata**: Classical FSMs from automata theory underlie state machines
- **Petri Nets**: Alternative modeling notation for concurrent systems

**Human-Computer Interaction**:
- **User Requirements** vs. **System Requirements**: User-level requirements refined into system-level
- **Usability Requirements**: Often non-functional ("user-friendly" requires operationalization)
- **Scenario-Based Design**: Use cases and scenarios help elicit requirements

**Project Management**:
- **Scope Management**: Requirements define project scope; "won't have" prevents scope creep
- **Change Management**: Requirements changes trigger impact analysis
- **Traceability Matrix**: Links requirements to design, code, tests for verification

**Standards and Compliance**:
- **IEC 61508**: Functional safety standard requiring detailed requirements
- **ISO 26262**: Automotive safety standard with requirements management process
- **DO-178C**: Aviation software standard emphasizing requirements traceability
- **Medical Device Standards**: FDA requires comprehensive requirements documentation

### Integration with Course Themes

**Requirements → Modeling → Implementation → Design Space Exploration**:
1. **Requirements** define what system must do (this lecture)
2. **Modeling** describes how to achieve requirements (state machines)
3. **Implementation** realizes the model in code
4. **Design Space Exploration** finds best hardware/software partition to meet requirements

**Non-Functional Requirements Drive Design Space**:
- Performance requirements → may need faster CPU or hardware acceleration
- Energy requirements → may need low-power CPU or sleep states
- Cost requirements → may limit hardware options
- All three compete in design space triangle

**State Machines Throughout Course**:
- Lecture 03: Basic UML state machines
- Lecture 04: Extended state machines (hierarchy, concurrency)
- Labs: Implement state machines for traffic light, washing machine, etc.
- Lecture 13: State machines as tasks in RTOS

## Summary

### Key Takeaways

1. **Requirements are foundation of embedded systems development** - Without clear requirements, projects fail due to misunderstandings between stakeholders (tree swing problem).

2. **Good requirements have 10 characteristics**: Unambiguous, testable, clear, correct, understandable, feasible, independent, atomic, necessary, and implementation-free.

3. **RFC 2119 standardizes requirement keywords**: MUST/SHALL (mandatory), SHOULD (recommended), MAY (optional) - provides consistent interpretation across projects.

4. **Requirements have priorities**: MoSCoW method (Must/Should/Could/Won't) or numeric scales help manage scope and trade-offs.

5. **Requirements engineering is iterative**: Cycle of elicitation → analysis → specification → validation → negotiation repeats until stakeholders agree.

6. **UML State Machines ideal for embedded systems**: Reactive nature of embedded systems maps naturally to event-driven state machines.

7. **State machine elements**: States (modes), transitions (changes), events (triggers), actions (operations), guards (conditions).

8. **Actions occur at different times**: Entry (on entering), do (while in state), exit (on leaving), transition (during change).

9. **Implementation approaches**: Switch-case (simple, doesn't scale) vs. table-based (scalable, maintainable) for programming state machines.

10. **System partitioning reduces complexity**: Instead of monolithic FSM, partition into communicating FSMs with ports and links.

### Essential Facts for Exam Discussion

- **Requirement definition**: "Describes a behavior or property that the Embedded System shall achieve or implement"
- **Functional vs. Non-Functional**: What system does vs. how system is
- **RFC 2119 keywords**: MUST (absolute), SHOULD (strong recommendation), MAY (optional)
- **MoSCoW**: Must have, Should have, Could have, Won't have
- **State machine 4 elements**: States, Transitions, Events, Actions
- **Event types**: Signal (async), Call (sync), Change (condition), Time (timeout)
- **Entry/Do/Exit order**: Entry executes once on entering, Do executes continuously, Exit executes once on leaving
- **Initial state**: Filled black circle ●
- **Final state**: Bullseye ◉
- **Transition label**: `event [guard] / action`
- **Port**: Interface for FSM communication
- **Run-to-completion semantics**: Event processed atomically before next event

## Possible Exam Questions

### Basic Recall / Definitions (8 questions)

1. What is a requirement in the context of embedded systems? Distinguish between functional and non-functional requirements with examples.

2. Define the RFC 2119 keywords MUST, SHOULD, and MAY. When would you use each?

3. What are the four main elements of a UML State Machine? Briefly describe each.

4. What is the MoSCoW principle for requirements prioritization? Explain each category.

5. What is the difference between entry, do, and exit actions in a state?

6. Name and briefly describe the four types of events in UML State Machines.

7. What is meant by "run-to-completion" semantics in state machine execution?

8. What is the purpose of a port in communicating state machines?

### Comprehension / Explanation (10 questions)

9. Explain why the "tree swing" cartoon illustrates the importance of requirements engineering. What problem does it highlight?

10. Why is the requirement "The system shall be user-friendly" considered a bad requirement? How would you improve it to make it testable?

11. Describe the five phases of the requirements engineering process (elicitation, analysis, specification, validation, negotiation) and how they relate to each other.

12. Explain the difference between a simple state and a composite state in UML. Why are composite states needed?

13. What does it mean for a requirement to be "independent"? Give an example of a dependent requirement and explain how to fix it.

14. Describe the execution sequence when a transition occurs in a state machine. Include entry actions, exit actions, and transition actions.

15. Explain the difference between a signal event and a time event. Give embedded systems examples where each is appropriate.

16. What is a self-transition in a state machine? How does it differ from an internal transition conceptually?

17. Why is system partitioning important when modeling complex embedded systems? Use the traffic light example to explain.

18. Describe the state space explosion problem in finite state machines. Why does it occur and how do UML extensions address it?

### Application (8 questions)

19. Given the requirement "The traffic light shall react to the current traffic situation," identify what makes it a poor requirement and rewrite it to be testable and unambiguous.

20. You are designing a coffee machine. Write three functional requirements and three non-functional requirements using proper RFC 2119 keywords.

21. Create a simple UML state machine for an automatic door with the following requirements:
    - Door shall open when person approaches
    - Door shall close after 5 seconds if no person detected
    - Door shall remain open if person continues to be present

22. Given a state machine with states A, B, and C, and events e1 and e2, write the skeleton code for a table-based implementation in C.

23. For a battery-powered sensor node that must operate for 1 year on a single battery, write 5 requirements (mix of functional and non-functional) that would guide the design.

24. Analyze this requirement for quality: "The alarm should sound when smoke is detected unless the user pressed the button recently." List all quality issues and rewrite it properly.

25. Design a port interface for communication between a temperature sensor FSM and a heating controller FSM. Define the messages that should be sent on each port.

26. You have a state machine with 4 states and 6 events. Compare switch-case vs. table-based implementation: which would you choose and why?

### Analysis / Comparison (9 questions)

27. Compare functional requirements with non-functional requirements. How do they differ in terms of implementation impact and testing approach?

28. Contrast the MoSCoW prioritization method with numerical priority scales (0-3). In what project contexts is each more appropriate?

29. Analyze the trade-offs between switch-case and table-based implementation of state machines. Consider maintainability, performance, and scalability.

30. Compare the requirements "The system MUST respond within 1 second" vs. "The system SHOULD respond within 1 second." What are the implications of each for implementation and testing?

31. Contrast classical finite state machines (FSM) with UML State Machines. What limitations of FSMs do UML extensions address?

32. Compare entry actions, do actions, and exit actions in terms of execution timing and use cases. When would you use each?

33. Analyze the difference between having one monolithic FSM for a traffic intersection vs. separate FSMs for each traffic light. What are the pros and cons of each approach?

34. Compare signal events and call events in UML State Machines. How do they differ in timing and use cases?

35. Contrast individual requirement quality criteria (like "testable" and "atomic") with requirements set criteria (like "consistent" and "complete"). Why are both levels necessary?

### Synthesis / Design Decisions (7 questions)

36. Design a requirements prioritization strategy for a medical device project where safety is paramount. How would you adapt MoSCoW or create a custom prioritization scheme?

37. You are tasked with modeling a washing machine. Identify the states and major transitions. What events would trigger transitions? What actions would occur in each state?

38. Create a requirements engineering process for a small agile team developing an IoT device. How would you adapt the traditional RE cycle for agile iterations?

39. Design a state machine for a pedestrian crossing button that implements the following behavior:
    - Pressing button requests crossing
    - Light changes to green for pedestrians after maximum 30s
    - Green light lasts minimum 10s
    - System prevents multiple requests

40. Propose a testing strategy to verify that a state machine implementation correctly satisfies its requirements. What types of tests would you perform?

41. Design a communication protocol between two FSMs: a door sensor and a security alarm. Define the ports, messages, and how the FSMs coordinate.

42. Create a systematic procedure for converting a set of requirements into a UML state machine model. What steps would you follow?

### Critical Thinking / Trade-offs (8 questions)

43. Discuss the tension between "complete" requirements (no missing information) and "implementation-free" requirements (not prescribing solutions). How do you balance these?

44. Evaluate the claim: "If you can't test a requirement, it's not really a requirement." Do you agree? Are there valid untestable requirements?

45. Analyze the trade-off between having many detailed requirements (high overhead but clarity) vs. few high-level requirements (agility but ambiguity). When is each appropriate?

46. Critique the practice of using "TBD" (To Be Determined) in requirements documents. Under what circumstances, if any, is this acceptable?

47. Discuss why requirements prioritization is necessary. What problems arise if all requirements are treated as equally important?

48. Evaluate the benefits and drawbacks of formal requirement specification languages vs. natural language requirements. When is the additional rigor worth the cost?

49. Analyze why embedded systems particularly benefit from state machine modeling compared to other software systems (e.g., web applications, databases).

50. Discuss the chicken-and-egg problem: you need requirements to know what to model, but modeling often reveals missing requirements. How do you resolve this?

### Scenario-Based Questions (7 questions)

51. Scenario: A client says "I need a smart thermostat that works well." As a requirements engineer, what questions would you ask to elicit specific, testable requirements?

52. Scenario: You find two requirements in conflict: "System SHALL respond within 100ms" and "System SHALL perform complex encryption requiring 500ms per request." How would you resolve this in the negotiation phase?

53. Scenario: During implementation, developers discover a requirement is technically infeasible with chosen hardware. What process should be followed to handle this requirements change?

54. Scenario: You're modeling a microwave oven state machine. While modeling, you realize the requirements don't specify what happens if the door opens during cooking. How should you handle this?

55. Scenario: A state machine has grown to 20 states with 50 transitions, and switch-case code is becoming unmaintainable. Describe your refactoring plan to table-based implementation.

56. Scenario: Your requirements specify "alarm MUST sound within 5 seconds of smoke detection" but testing shows 6-second delays. The requirement came from a fire safety standard. What do you do?

57. Scenario: Two FSMs need to communicate: a button FSM and a lamp FSM. The button press should toggle the lamp. During implementation, you find race conditions. How would you resolve this in the design?

### Connections & Integration (6 questions)

58. How do the software paradigms from Lecture 02 (event-driven, time-driven) relate to UML State Machines? Give specific examples of how paradigms map to state machine elements.

59. Connect requirements engineering to the design space (energy-cost-performance triangle) from Lecture 01. How do requirements constrain the design space?

60. Explain how state machines will be used in later labs (traffic light, washing machine). What makes these systems well-suited for state machine modeling?

61. How do functional requirements from this lecture relate to the non-functional requirements that will be covered in Lecture 06? Give examples of how they interact.

62. Relate the reactive nature of embedded systems (Lecture 01) to the event-driven nature of state machines. Why is this modeling approach natural for embedded systems?

63. How will the requirements and state machines created in early design phases (this lecture) be validated in later phases (testing labs)? Describe the traceability chain.

### Code Recognition & Debugging (5 questions)

64. Analyze this state machine code and identify the bug:
```c
switch(current_state) {
    case ARMED:
        if(event == SENSOR_TRIGGERED) {
            sound_alarm();
            // Bug: forgot to change state!
        }
        break;
}
```

65. Given this requirement: "System SHALL transition from IDLE to ACTIVE when button pressed, but only if battery level > 20%", write the corresponding state machine transition with guard.

66. Review this requirement: "The system must be secure, reliable, and perform well." Identify all quality issues (at least 5) and explain each.

67. A developer implements a state machine where the do action calls `sleep(1000)`. Explain why this violates state machine semantics and how to fix it.

68. Given this state machine fragment:
```
State A: entry/start_timer()  exit/stop_timer()
State B: entry/process()

Transition: A→B on timeout / log_event()
```
Write the execution sequence when timeout occurs in State A.

### Advanced Understanding (4 questions)

69. Explain the relationship between David Harel's 1984 Statecharts paper and modern UML State Machines. What problems did Harel aim to solve?

70. Discuss how state machines enable model-based code generation. What information in a state machine is sufficient to automatically generate implementation code?

71. Analyze how state machines support verification and validation. What properties can be formally verified? What testing techniques are enabled?

72. Explain the "run-to-completion" execution semantics in detail. What are the implications for event queuing, timing, and concurrency?

### Meta-Questions About Requirements Practice (3 questions)

73. Why do requirements engineering textbooks emphasize that requirements describe "what" not "how"? What problems arise when requirements are over-specified?

74. In industrial practice, requirements documents can exceed 1000 pages (e.g., automotive ECU). How do teams manage such large requirement sets? What tools and processes are needed?

75. Reflect on the statement "Requirements are never complete until the project is finished." Is this a failure of RE process or an inherent property of complex systems?

