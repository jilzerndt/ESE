# Lab U01: Requirements & Modelling

## Technical Terms & Definitions

### Requirements Engineering Terms
- **Requirement**: A statement describing what a system must do or a constraint it must satisfy
- **Functional Requirement**: Describes what the system should do (behavior, features, operations)
- **Safety Requirement**: Ensures system operates without causing harm or dangerous situations
- **Operational Requirement**: Describes normal operating conditions and behavior
- **Timing Requirement**: Specifies time constraints for system operations
- **Failure Mode Requirement**: Defines system behavior when failures occur (fault tolerance)
- **Peer Review**: Process where colleague examines requirements for clarity, completeness, correctness
- **Requirements Iteration**: Cycle of writing, reviewing, and improving requirements based on feedback
- **Requirement Refinement**: Process of making requirements more specific, clear, and complete
- **Ambiguity**: Lack of clarity in requirement that allows multiple interpretations
- **Completeness**: Requirement covers all necessary aspects without gaps
- **Traceability**: Ability to link requirements to design, implementation, and tests

### UML State Machine Modeling Terms
- **State**: Condition or situation during the life of an object (e.g., IDLE, FILL_WATER, SPIN_DRY)
- **Event**: Occurrence that can trigger a state transition (e.g., DOOR_OPENED, WASCHEN_BTN, TIME_OUT)
- **Transition**: Change from one state to another in response to an event
- **Action**: Operation performed during a transition (e.g., door_lock(), valve_open(), motor_on())
- **Guard Condition**: Boolean condition that must be true for transition to occur (not explicitly used in this lab but implicit in some transitions)
- **Entry Action**: Action performed when entering a state
- **Exit Action**: Action performed when leaving a state
- **Do Action**: Ongoing activity performed while in a state (not used in this lab)
- **Initial State**: Starting point of state machine, marked with filled circle
- **Sensor Event**: Event triggered by hardware sensor (FLOATER_HIGH, TEMPERATURE_HOT)
- **User Event**: Event triggered by user interaction (WASCHEN_BTN, SPIN_BTN, STOP_BTN)
- **Timer Event**: Event triggered after specified time elapses (TIME_OUT)
- **Actuator Action**: Action that controls physical hardware (motor, pump, valve, heater)

### Washing Machine Domain Terms
- **Floater Sensor**: Water level sensor that triggers events when water reaches high/low thresholds
- **Temperature Sensor**: Monitors water temperature, triggers event when threshold reached
- **Valve**: Controls water inlet to drum (open = water flows in, close = water stops)
- **Pump**: Controls water outlet from drum (on = water flows out, off = water stays)
- **Heater**: Controls water temperature (on = heating, off = no heating)
- **Motor**: Controls drum rotation (direction: left/right, speed: slow/fast, state: on/off)
- **Door Lock**: Mechanical lock preventing door opening during operation
- **Wash Program**: Full cycle (fill → heat → rotate → empty → spin)
- **Spin-Dry Program**: Short cycle (empty → spin)
- **Emergency Stop**: Immediate halt of all operations via STOP_BTN

### Traffic Light Domain Terms
- **Phase**: Period when specific traffic direction has right-of-way
- **Conflict**: Situation where two traffic flows could collide
- **Clearance Interval**: All-red period ensuring intersection is clear before phase change
- **Yellow Interval**: Warning period before red light
- **Minimum Green Time**: Shortest duration green light must stay on
- **Maximum Green Time**: Longest duration green light can stay on (prevents starvation)
- **Right-of-Way**: Legal priority to proceed through intersection
- **Deadlock**: Situation where no traffic can proceed (all conflicting directions green)
- **Starvation**: One direction never gets green light (unfair scheduling)
- **Fail-Safe Mode**: Default safe state when system failure occurs (flashing yellow)

## Core Concepts

### Requirements Engineering Process (Task 1)

#### Phase 1: Initial Requirements Formulation
- **Purpose**: Capture system requirements in clear, written statements
- **Focus Areas for Traffic Light System**:
  - Safety (no conflicting green lights)
  - Timing (yellow duration, green duration, clearance intervals)
  - Fairness (all directions get service)
  - Regulations (compliance with traffic laws)
  - Failure handling (safe defaults)

#### Phase 2: Peer Review
- **Reviewer Role**: Provide honest, constructive feedback on requirements
- **Common Review Questions**:
  - Is the requirement clear and unambiguous?
  - Are values justified (why 3 seconds, 15 seconds)?
  - Are edge cases covered (failures, conflicts)?
  - Is the requirement complete (what about pedestrians)?
  - Is the requirement testable/verifiable?
  - Does it reference standards/regulations?

#### Phase 3: Requirement Improvement
- **Based on Feedback**: Address reviewer concerns systematically
- **Common Improvements**:
  - Add justification (reference regulations, standards)
  - Define ambiguous terms ("from one direction to another")
  - Add missing scenarios (pedestrians, failures)
  - Add constraints (maximum times to prevent starvation)
  - Specify failure behavior (fail-safe modes)

#### Key Insights from Lab
1. **Initial requirements are never perfect** - iteration is essential
2. **Peer review catches issues** the author missed (blind spots)
3. **Justification matters** - arbitrary values raise questions
4. **Completeness requires multiple perspectives** - reviewer added pedestrians
5. **Safety-critical systems need failure modes** - what happens when it breaks?

### UML State Machine Modeling (Task 2)

#### Modeling Methodology
1. **Identify States**: Based on system phases/modes (IDLE, FILL_WATER, HEAT_WATER, etc.)
2. **Identify Events**: User inputs, sensor triggers, timers (WASCHEN_BTN, FLOATER_HIGH, TIME_OUT)
3. **Identify Actions**: Actuator controls (door_lock(), valve_open(), motor_on())
4. **Define Transitions**: Event-triggered state changes with associated actions
5. **Handle Special Cases**: Emergency stop (STOP_BTN from any state)
6. **Validate Against Requirements**: Every requirement must be represented in model

#### State Design Principles
- **States represent waiting points** - system waits for event in each state
- **One responsibility per state** - FILL_WATER only fills, HEAT_WATER only heats
- **Clear entry/exit** - each state has well-defined entry and exit conditions
- **Safety first** - door only unlocks when safe (no water, motor off)

#### Event-Driven Control
- **Sensor Events** drive state machine:
  - FLOATER_HIGH → water level reached, proceed to next step
  - FLOATER_LOW → water drained, proceed to next step
  - TEMPERATURE_HOT → heating complete, proceed to washing
  - TIME_OUT → rotation/spin time complete, proceed to next step
- **User Events** override or initiate:
  - WASCHEN_BTN → start full wash program
  - SPIN_BTN → start spin-dry program
  - STOP_BTN → emergency stop from any state (highest priority)
  - DOOR_OPENED/CLOSED → manage door state

#### Action Coordination
- **Multiple Actions per Transition**: Common to have several actions on one transition
  - Example: STOP_BTN transition → valve_close(), heater_off(), pump_on(), state = SHUT_DOWN
- **Sequential Execution**: Actions execute in order listed on transition
- **Interlocks**: Safety actions prevent dangerous states
  - door_lock() before filling water
  - heater_off() before rotating drum
  - motor_off() before unlocking door

#### Washing Machine State Flow
1. **Initial → DOOR_OPEN**: System starts with door open, all systems off
2. **DOOR_OPEN → IDLE**: Door closed → ready for operation
3. **IDLE → FILL_WATER**: Waschen button → lock door, open valve, fill water
4. **FILL_WATER → HEAT_WATER**: Water high → close valve, turn on heater
5. **HEAT_WATER → ROTATE_RIGHT**: Temperature hot → heater off, start timer, motor right
6. **ROTATE_RIGHT → ROTATE_LEFT**: Timer expires → switch motor direction
7. **ROTATE_LEFT → EMPTY_WATER**: Timer expires → motor off, pump on, drain water
8. **EMPTY_WATER → SPIN_DRY**: Water low → pump off, start timer, motor fast
9. **SPIN_DRY → SHUT_DOWN**: Timer expires → cleanup
10. **SHUT_DOWN → IDLE**: Water drained → unlock door, ready

**Alternative Paths**:
- **IDLE → EMPTY_WATER → SPIN_DRY**: Spin button (short program)
- **Any State → SHUT_DOWN**: Stop button (emergency exit)

## Important Diagrams & Graphics

### 1. Traffic Light Crossroad Diagram (from PDF)
- **Layout**: Standard 4-way intersection with pedestrian crossings
- **Directions**: North-South (vertical) and East-West (horizontal) traffic flows
- **Elements**: Vehicle lanes, turn lanes, pedestrian crossings marked with yellow stripes
- **Conflict Points**: Center of intersection where NS and EW flows intersect
- **Concept**: Shows physical layout that requirements must control safely

### 2. Washing Machine Control System Block Diagram
- **Sensors** (inputs to state machine):
  - Door sensor (DOOR_OPENED, DOOR_CLOSED)
  - Floater sensor (FLOATER_HIGH, FLOATER_LOW)
  - Temperature sensor (TEMPERATURE_HOT)
  - User buttons (WASCHEN_BTN, SPIN_BTN, STOP_BTN)
  - Timer (TIME_OUT)

- **Actuators** (outputs from state machine):
  - Door lock mechanism (door_lock, door_unlock)
  - Water valve (valve_open, valve_close)
  - Water pump (pump_on, pump_off)
  - Water heater (heater_on, heater_off)
  - Drum motor (motor_left/right_slow/fast_on, motor_off)
  - Timer (start_timer(X))

- **Control System**: State machine processes sensor events and commands actuators

### 3. Washing Machine State Machine Diagram
**Structure**: Hierarchical state flow with emergency exit path

**States** (rectangles):
- Initial → DOOR_OPEN (startup configuration)
- IDLE (ready, waiting for user command)
- FILL_WATER (filling water into drum)
- HEAT_WATER (heating water to temperature)
- ROTATE_RIGHT (washing - drum rotating clockwise)
- ROTATE_LEFT (washing - drum rotating counterclockwise)
- EMPTY_WATER (draining water from drum)
- SPIN_DRY (high-speed spinning to remove water)
- SHUT_DOWN (cleanup and safe stop)

**Transitions** (arrows with labels):
- Format: `EVENT / actions; state = NEW_STATE`
- Example: `FLOATER_HIGH / valve_close(); heater_on(); state = HEAT_WATER`

**Main Flow Path** (Wash Program):
```
IDLE → FILL_WATER → HEAT_WATER → ROTATE_RIGHT → ROTATE_LEFT → EMPTY_WATER → SPIN_DRY → SHUT_DOWN → IDLE
```

**Short Flow Path** (Spin Program):
```
IDLE → EMPTY_WATER → SPIN_DRY → SHUT_DOWN → IDLE
```

**Emergency Path** (from any operational state):
```
Any State → SHUT_DOWN → IDLE
```

**Key Visual Features**:
- Multiple transitions from IDLE (user choice)
- STOP_BTN transitions from every state (global escape)
- Cyclic flow for wash program
- Actions listed on each transition
- State assignments explicit

### 4. Traffic Light Phase Timing Diagram
**Would show** (not provided in lab, but implied by requirements):
- **Time axis** (horizontal)
- **NS Light States**: Green (15s min) → Yellow (3s) → Red (2s all-red clearance)
- **EW Light States**: Red → Red (during NS green+yellow+clearance) → Red+Yellow (2s) → Green (15s min) → ...
- **Cycle**: Repeating pattern ensuring mutual exclusion
- **Concept**: Temporal relationships between light states, showing no overlap of green lights

### 5. Requirements Iteration Flowchart
1. **Write Initial Requirements** →
2. **Peer Review** →
3. **Collect Feedback** →
4. **Refine Requirements** →
5. **Validate** → Repeat if needed
- **Concept**: Iterative process, not one-shot

## Formulas & Procedures

### Requirements Writing Best Practices
1. **Use RFC 2119 Keywords** (from Lecture 03):
   - SHALL/MUST: Mandatory requirements
   - SHOULD: Recommended requirements
   - MAY: Optional requirements
   - Example: "The system SHALL ensure north-south and east-west never both have green lights"

2. **Structure of Good Requirement**:
   - **Subject**: System or component
   - **Modal Verb**: SHALL/SHOULD/MAY
   - **Action**: What must be done
   - **Object**: What is acted upon
   - **Constraint**: Conditions, limits, timing
   - **Rationale**: Why (safety, regulation, usability)

   Example: "The traffic light system SHALL show yellow light for 3 seconds (constraint) when changing from green to red (condition) to provide drivers time to slow down safely (rationale)."

3. **Requirement Quality Checklist**:
   - ✓ Clear: No ambiguous terms
   - ✓ Complete: Covers all aspects
   - ✓ Consistent: No contradictions with other requirements
   - ✓ Testable: Can verify compliance
   - ✓ Traceable: Can link to design and tests
   - ✓ Feasible: Technically achievable
   - ✓ Necessary: Addresses real need
   - ✓ Justified: Explains why (references standards/regulations)

### UML State Machine Construction Procedure

#### Step 1: Requirements Analysis
- Read all requirements carefully
- Highlight key system modes/phases → candidate states
- Identify all events (user inputs, sensors, timers)
- Identify all actions (actuator commands)
- List safety constraints and interlocks

#### Step 2: State Identification
- Create state for each distinct system mode
- Name states clearly (verb+noun: FILL_WATER not FILLING)
- Ensure states are mutually exclusive (can't be in two states simultaneously)
- Add IDLE state for waiting between operations
- Add SHUT_DOWN state for cleanup

#### Step 3: Transition Definition
For each state:
- What events can occur in this state?
- What state should system transition to for each event?
- What actions must be performed during transition?
- Are there safety checks or interlocks?

#### Step 4: Action Assignment
For each transition:
- **Turn off** what was active in source state (e.g., valve_close, motor_off)
- **Turn on** what needs to be active in target state (e.g., heater_on, pump_on)
- **Start timers** if target state has time limit
- **Update status** indicators (LCD display, state variable)

#### Step 5: Special Case Handling
- **Emergency Stop**: Add STOP_BTN transition from every state to safe shutdown
- **Initial State**: Define system startup configuration
- **Failure Recovery**: Consider sensor failures, stuck states
- **Interlocks**: Prevent unsafe transitions (e.g., door only unlocks when safe)

#### Step 6: Validation Against Requirements
- For each requirement, trace to state machine elements:
  - "Door opens only when no water" → DOOR_OPEN only reachable from IDLE/SHUT_DOWN after FLOATER_LOW
  - "Stop button works at all times" → STOP_BTN transition from every operational state
  - "Heater off before rotating" → HEAT_WATER → ROTATE_RIGHT transition includes heater_off()
- Check completeness: All requirements covered?
- Check correctness: No violations of requirements?

### Washing Machine State Machine Construction (Detailed)

**Given Requirements** → **State Machine Elements**:

1. "Opening door only when no water, drum not rotating, door unlocked"
   - **States**: DOOR_OPEN only reachable from IDLE/SHUT_DOWN
   - **Preconditions**: FLOATER_LOW must occur (no water), motor_off (not rotating), door_unlock action performed
   - **Interlock**: door_lock() before any water/motor operations

2. "Door status written to LCD"
   - **Actions**: state = DOOR_OPEN, state = IDLE (would write to LCD in implementation)

3. "Waschen button starts 5-step program"
   - **Transition**: IDLE →[WASCHEN_BTN] FILL_WATER
   - **State Sequence**: FILL_WATER → HEAT_WATER → ROTATE_RIGHT → ROTATE_LEFT → EMPTY_WATER → SPIN_DRY
   - **Events**: FLOATER_HIGH, TEMPERATURE_HOT, TIME_OUT (twice), FLOATER_LOW, TIME_OUT
   - **Timers**: start_timer(4) for rotations, start_timer(5) for spin

4. "Schleudern button starts spin-dry"
   - **Transition**: IDLE →[SPIN_BTN] EMPTY_WATER (skip washing steps)
   - **State Sequence**: EMPTY_WATER → SPIN_DRY
   - **Assumption**: Water already in drum, or pump just runs briefly if already empty

5. "Stop button at all times"
   - **Transitions**: From FILL_WATER, HEAT_WATER, ROTATE_RIGHT, ROTATE_LEFT, EMPTY_WATER, SPIN_DRY → SHUT_DOWN
   - **Actions**: Turn off everything (valve_close, heater_off, motor_off), pump_on to drain

6. "Temperature increased to HOT before rotating"
   - **State**: HEAT_WATER (dedicated state for heating)
   - **Transition**: HEAT_WATER →[TEMPERATURE_HOT] ROTATE_RIGHT
   - **Ensures**: Sensor confirms HOT before proceeding

7. "Heater off before rotating"
   - **Action**: HEAT_WATER →[TEMPERATURE_HOT] ROTATE_RIGHT transition includes heater_off()

## Comparisons & Contrasts

### Initial vs. Improved Traffic Light Requirements

| Aspect | Initial Requirements | Improved Requirements |
|--------|---------------------|----------------------|
| **Justification** | No explanation for timing values (3s, 15s) | Added reference to traffic regulations |
| **Failure Handling** | Not addressed | Added fail-safe mode (flashing yellow) |
| **Completeness** | 5 requirements, vehicle-only | 6 requirements, added pedestrians |
| **Ambiguity** | "From one direction to another" unclear | Clarified as conflicting traffic flows |
| **Maximum Time** | Only minimum green time | Added maximum green time to prevent starvation |
| **Safety** | Focused on preventing collisions | Also addresses driver expectations and behavior |
| **Reviewer Feedback** | Many questions raised | Addressed all major concerns |

### Full Wash vs. Spin-Dry Programs

| Aspect | Wash Program (WASCHEN_BTN) | Spin-Dry Program (SPIN_BTN) |
|--------|---------------------------|----------------------------|
| **Entry Point** | IDLE → FILL_WATER | IDLE → EMPTY_WATER |
| **Steps** | 5 steps (fill, heat, rotate×2, empty, spin) | 2 steps (empty, spin) |
| **Duration** | ~20+ seconds (4+4+5+fill/heat/drain time) | ~5-10 seconds (5s spin + drain time) |
| **Water Usage** | Fills water, heats, drains | Uses existing water or drains empty |
| **Use Case** | Full washing cycle for dirty laundry | Quick drying for already-washed items |
| **Energy** | High (heater + motor + pump) | Low (motor + pump only) |
| **State Path** | FILL → HEAT → ROT_R → ROT_L → EMPTY → SPIN | EMPTY → SPIN |

### Event Types in Washing Machine

| Event Type | Examples | Characteristics | Response Time |
|------------|----------|----------------|---------------|
| **User Events** | WASCHEN_BTN, SPIN_BTN, STOP_BTN, DOOR_OPENED, DOOR_CLOSED | Unpredictable timing, initiated by user | Immediate |
| **Sensor Events** | FLOATER_HIGH, FLOATER_LOW, TEMPERATURE_HOT | Predictable within program, triggered by physical state | Asynchronous, may vary |
| **Timer Events** | TIME_OUT | Predictable, deterministic timing | Exact (after X seconds) |

### Normal vs. Emergency Shutdown

| Aspect | Normal Shutdown (SPIN_DRY → SHUT_DOWN) | Emergency Shutdown (STOP_BTN → SHUT_DOWN) |
|--------|----------------------------------------|-------------------------------------------|
| **Trigger** | TIME_OUT (program completes) | STOP_BTN (user abort) |
| **State** | Expected, planned completion | Unexpected, interruption |
| **Actions** | Minimal (already cleaned up during program) | Extensive (valve_close, heater_off, pump_on) |
| **Water** | Already drained | May need draining (pump_on in SHUT_DOWN) |
| **Safety** | System already safe | Must make safe (turn off heater, stop motor) |
| **User Intent** | Retrieve laundry after completion | Stop operation immediately |

### State Machine Design Approaches

| Approach | Description | Pros | Cons | Lab Example |
|----------|-------------|------|------|-------------|
| **Linear Sequential** | States in fixed sequence | Simple, predictable | Inflexible, can't skip steps | Wash program main flow |
| **Choice Points** | Multiple transitions from one state | Flexible, user choice | More complex | IDLE (3 outgoing transitions) |
| **Emergency Exit** | Global transition to safe state | Safety, user control | Adds complexity to every state | STOP_BTN from all states |
| **Cyclic** | Returns to initial state | Reusable, continuous operation | Must handle cleanup | SHUT_DOWN → IDLE (ready for next cycle) |

## Examples & Applications

### Example 1: Traffic Light Requirements - Initial and Improved

**Initial Requirement 1** (Incomplete):
> "No Green Lights at Same Time: The traffic light system must make sure that the north-south and east-west directions never both have green lights at the same time, otherwise cars might crash or there could be a situation where the cars basically 'deadlock' themselves."

**Reviewer Feedback**:
> "Good start, but what happens if there's a system failure? Should specify a safe default."

**Improved Requirement 1** (Complete):
> "No Green Lights at Same Time: The traffic light system must make sure that the north-south and east-west directions never both have green lights at the same time, otherwise cars might crash or there could be a situation where the cars basically 'deadlock' themselves. In case of system failure: all lights flash yellow to indicate that drivers must proceed with caution and follow standard right-of-way rules."

**Analysis**:
- Initial version covered normal operation but missed failure mode
- Improved version adds fail-safe behavior (flashing yellow)
- Demonstrates importance of considering edge cases
- Safety-critical systems must define failure behavior

### Example 2: Requirement with Justification

**Initial Requirement 2** (No Justification):
> "Yellow Light Before Red: When a traffic light changes from green to red, it must show yellow for 3 seconds first so drivers have time to slow down."

**Reviewer Feedback**:
> "Where do the 3 seconds come from (traffic regulations)?"

**Improved Requirement 2** (With Justification):
> "Yellow Light Before Red: When a traffic light changes from green to red, it must show yellow for 3 seconds first so drivers have time to slow down. Duration is based on the traffic regulations I found."

**Analysis**:
- Arbitrary values raise questions about validity
- Justification (regulations) provides authority and traceability
- Shows requirement is not random but based on standards
- Important for safety-critical systems to reference regulations

### Example 3: Washing Machine - Fill Water to Heat Water Transition

**Requirement**: "The water temperature shall be increased to HOT before rotating the washing drum."

**State Machine Implementation**:
```
State: FILL_WATER
  - Entry: valve_open() [water flows into drum]
  - Event: FLOATER_HIGH [water level sensor triggers]
  - Transition Actions: valve_close() [stop filling], heater_on() [start heating]
  - Target State: HEAT_WATER

State: HEAT_WATER
  - Do: Wait for water to heat (heater_on from previous transition)
  - Event: TEMPERATURE_HOT [temperature sensor triggers]
  - Transition Actions: heater_off() [stop heating], start_timer(4) [prepare for rotation], motor_right_slow_on() [start rotation]
  - Target State: ROTATE_RIGHT
```

**Analysis**:
- Requirement translated to two states (FILL_WATER, HEAT_WATER) and transitions
- Events (FLOATER_HIGH, TEMPERATURE_HOT) drive progression
- Actions coordinate actuators (valve, heater, motor)
- Sequential flow ensures water is filled before heated, heated before rotating
- Safety requirement (heater_off before motor_on) enforced by transition action order

### Example 4: Emergency Stop from Any State

**Requirement**: "Pressing the stop button marked 'Stop' shall be possible at all times and result in the washing machine possibly taking out the water, turning off the water heater, and stopping all operations."

**State Machine Implementation**:
- **From FILL_WATER**: `STOP_BTN / valve_close(), pump_on(), state = SHUT_DOWN`
- **From HEAT_WATER**: `STOP_BTN / heater_off(), pump_on(), state = SHUT_DOWN`
- **From ROTATE_RIGHT**: `STOP_BTN / motor_off(), valve_close(), pump_on(), state = SHUT_DOWN`
- **From ROTATE_LEFT**: `STOP_BTN / motor_off(), valve_close(), pump_on(), state = SHUT_DOWN`
- **From EMPTY_WATER**: `STOP_BTN / valve_close(), pump_on(), state = SHUT_DOWN`
- **From SPIN_DRY**: `STOP_BTN / motor_off(), valve_close(), pump_on(), state = SHUT_DOWN`

**SHUT_DOWN State**:
- Waits for `FLOATER_LOW` event (water drained)
- Transition: `FLOATER_LOW / pump_off(), door_unlock(), state = IDLE`

**Analysis**:
- Global emergency exit pattern: STOP_BTN from all operational states
- Each transition performs cleanup specific to source state (turn off active systems)
- Common safety action: pump_on() to drain water (may already be running)
- Funnels all emergency stops through single SHUT_DOWN state
- Ensures system reaches safe state (IDLE) before allowing door open

### Example 5: Door Safety Interlock

**Requirement**: "Opening the door shall only be possible when there is no water in the machine, the drum is not rotating, and the door is unlocked."

**State Machine Implementation**:
- **Initial State**: Sets door_unlock() → door can physically open
- **DOOR_OPEN → IDLE**: User closes door, ready for operation
- **IDLE → FILL_WATER**: Transition includes door_lock() [prevents opening during operation]
- **All Operational States**: Door remains locked (door_unlock never called)
- **SHUT_DOWN → IDLE**: Only after FLOATER_LOW (no water), transition includes pump_off() [drum not rotating], door_unlock() [allow opening]
- **IDLE → DOOR_OPEN**: User can now open door (unlocked, no water, not rotating)

**Safety Guarantees**:
1. Door lock engaged before water added (IDLE → FILL_WATER: door_lock())
2. Door only unlocked after water drained (SHUT_DOWN: FLOATER_LOW required before door_unlock())
3. Motor always off before reaching IDLE (all paths through SHUT_DOWN or direct EMPTY_WATER have motor_off())
4. Physically impossible to open locked door (mechanical interlock)

**Analysis**:
- Multi-condition safety requirement enforced by state machine structure
- Door lock/unlock actions strategically placed on transitions
- State DOOR_OPEN only reachable when all safety conditions met
- Demonstrates how state machines implement safety interlocks

### Example 6: Comparing Two Program Paths

**Scenario**: User wants to spin-dry clothes that are already washed and damp.

**Wrong Approach**: Press WASCHEN_BTN
- Path: IDLE → FILL_WATER → HEAT_WATER → ROTATE_RIGHT → ROTATE_LEFT → EMPTY_WATER → SPIN_DRY
- Problem: Fills water (unnecessary), heats water (waste energy/time), washes again (not needed)
- Time: ~25+ seconds
- Energy: High (heater, extended motor, pump)

**Correct Approach**: Press SPIN_BTN
- Path: IDLE → EMPTY_WATER → SPIN_DRY
- Benefit: Skips washing steps, directly to spin-dry
- Time: ~8-10 seconds (drain if any water, then spin 5s)
- Energy: Low (motor + pump only)

**Analysis**:
- State machine provides multiple entry points for different use cases
- User can optimize by choosing appropriate program
- Demonstrates flexibility of choice point at IDLE state
- Both paths converge at SHUT_DOWN → proper cleanup regardless of program

### Example 7: Timer Usage in Rotation States

**Requirement**: "Rotating the washing drum to the right for 4s, then rotating the washing drum to the left for 4s"

**Implementation**:
```
HEAT_WATER →[TEMPERATURE_HOT] ROTATE_RIGHT
  Actions: heater_off(), start_timer(4), motor_right_slow_on(), state = ROTATE_RIGHT

ROTATE_RIGHT →[TIME_OUT] ROTATE_LEFT
  Actions: motor_off(), motor_left_slow_on(), start_timer(4), state = ROTATE_LEFT
  (Note: Original solution shows pump_on here, likely error in student solution)

ROTATE_LEFT →[TIME_OUT] EMPTY_WATER
  Actions: motor_off(), pump_on(), state = EMPTY_WATER
```

**Timer Behavior**:
1. Enter ROTATE_RIGHT: start_timer(4) begins 4-second countdown
2. During ROTATE_RIGHT: Motor runs right, timer counts down
3. TIME_OUT event: 4 seconds elapsed, transition to ROTATE_LEFT
4. Enter ROTATE_LEFT: start_timer(4) begins new 4-second countdown
5. During ROTATE_LEFT: Motor runs left, timer counts down
6. TIME_OUT event: 4 seconds elapsed, transition to EMPTY_WATER

**Analysis**:
- Timer event (TIME_OUT) drives progression after fixed duration
- start_timer(X) action sets up next TIME_OUT event
- Alternating motor directions agitates laundry
- Deterministic timing ensures consistent wash quality
- Same pattern used for SPIN_DRY (start_timer(5), then TIME_OUT → SHUT_DOWN)

## Connections to Other Topics

### Connection to Lecture 03: Requirements
- **Requirements Engineering**: Lab Task 1 directly applies requirements writing from Lecture 03
- **RFC 2119 Keywords**: Traffic light requirements should use SHALL/MUST for mandatory constraints
- **Requirements Characteristics**: Lab demonstrates need for clarity, completeness, testability
- **Peer Review**: Lab shows importance of iteration and feedback (Dominic's review improved requirements)
- **MoSCoW**: Implicit prioritization (safety requirements are MUST, pedestrians initially missed but SHOULD)
- **UML State Machines**: Lab Task 2 builds on state machine basics from Lecture 03 (states, transitions, events, actions)

### Connection to Lecture 04: Modelling
- **State Machine Construction**: Lab applies state machine modeling from Lecture 04
- **Events and Actions**: Lab uses event-driven transitions with actions (entry/exit actions implicit in transition actions)
- **Requirements to Model**: Demonstrates translation from textual requirements to formal state machine
- **Validation**: Lab shows how to validate model against requirements (each requirement maps to state machine elements)
- **Safety Interlocks**: Door lock example demonstrates guard conditions (implicit in state structure)
- **Emergency Handling**: STOP_BTN pattern shows terminate-like behavior (stop from any state)

### Connection to Lecture 02: Software Paradigms
- **Event-Driven Paradigm**: Washing machine is event-driven system (waits for events, responds with actions)
- **State Pattern**: State machine is implementation of state pattern (different behavior in different states)
- **Reactive Systems**: Both washing machine and traffic light are reactive (respond to external stimuli)
- **Real-Time Constraints**: Timing requirements (3s yellow, 4s rotation) relate to time-driven paradigm

### Connection to Lab P1: Hardware Bring-up
- **Embedded Control**: Washing machine control system would run on embedded processor
- **Sensor Integration**: FLOATER, TEMPERATURE sensors interface with hardware (GPIO, ADC)
- **Actuator Control**: Motor, pump, valve, heater controlled via GPIO, PWM, relays
- **Real Implementation**: State machine would be implemented in C code on embedded Linux (like Ultra96)

### Connection to Future Lecture: Development Processes
- **Requirements Phase**: Lab Task 1 represents requirements phase in V-model
- **Design Phase**: Lab Task 2 represents design phase (UML model)
- **Iteration**: Peer review and improvement demonstrate iterative development
- **Verification**: Validating state machine against requirements is verification activity
- **Documentation**: Both requirements document and state diagram serve as project documentation

### Connection to Future Lecture: Non-Functional Requirements
- **Timing NFRs**: Yellow light (3s), rotation (4s), spin (5s) are timing requirements
- **Safety NFRs**: Door interlock, no conflicting green lights are safety requirements
- **Reliability NFRs**: Fail-safe mode (flashing yellow) addresses reliability
- **Usability NFRs**: Door status on LCD, button responsiveness affect usability

### Connection to Future Lecture: RTOS Scheduling
- **Event Handling**: State machine events would be implemented as RTOS messages/interrupts
- **Task Structure**: Each state machine could be an RTOS task, events trigger task execution
- **Priorities**: STOP_BTN would have highest priority (emergency), other events lower priority
- **Timing**: Timer events (TIME_OUT) would use RTOS timer services

### Connection to Future Lab: Implementation & Integration
- **State Machine Implementation**: This lab's models would be implemented in code in future labs
- **Sensor Reading**: Reading FLOATER, TEMPERATURE sensors in embedded code
- **Actuator Control**: Writing to GPIO pins to control motor, pump, valve, heater
- **Testing**: Verifying state machine behavior matches model (integration testing)

## Summary

Lab U01 provides hands-on practice with two fundamental embedded systems engineering activities: **requirements engineering** and **UML state machine modeling**.

**Task 1 (Traffic Light Requirements)** demonstrates the iterative nature of requirements engineering. Initial requirements are rarely perfect—peer review reveals issues like missing justifications (why 3 seconds?), missing scenarios (pedestrians), ambiguous terms ("from one direction to another"), and missing failure modes. The improvement phase addresses these gaps, adding regulatory justifications, clarifying terminology, adding pedestrian requirements, and defining fail-safe behavior. This mirrors real-world engineering where collaboration and iteration produce better requirements.

**Task 2 (Washing Machine Modeling)** translates requirements into formal UML state machine. The model identifies 9 states representing system modes (DOOR_OPEN, IDLE, FILL_WATER, HEAT_WATER, ROTATE_RIGHT, ROTATE_LEFT, EMPTY_WATER, SPIN_DRY, SHUT_DOWN). Events from sensors (FLOATER_HIGH/LOW, TEMPERATURE_HOT), timers (TIME_OUT), and users (buttons, door) drive state transitions. Each transition specifies actions that control actuators (motor, pump, valve, heater, lock). The model handles two programs (full wash, spin-only) and emergency stop (STOP_BTN from any state).

**Key Learning Outcomes**:
1. Requirements must be clear, complete, justified, and cover failure modes
2. Peer review catches blind spots and improves requirement quality
3. State machines effectively model event-driven control systems
4. Safety interlocks (door lock) prevent dangerous states
5. Multiple program paths provide flexibility (wash vs. spin-only)
6. Emergency exits (STOP_BTN) ensure user control and safety
7. Validation ensures model implements all requirements

**Practical Skills Demonstrated**:
- Writing safety-critical requirements
- Conducting and responding to peer review
- Modeling reactive systems with UML state machines
- Coordinating multiple actuators through state transitions
- Handling normal and exceptional scenarios in one model
- Translating textual requirements to formal graphical models

This lab bridges theory (Lectures 03-04) and practice, preparing students for implementing control systems in embedded code.

## Possible Exam Questions

### Basic Definitions and Concepts (10 questions)

1. **Define "peer review" in the context of requirements engineering. Why is it important?**
   - Answer: Process where colleague examines requirements for clarity, completeness, correctness. Important because author has blind spots; reviewer provides fresh perspective and catches issues.

2. **What is the difference between a sensor event and a user event in a state machine?**
   - Answer: Sensor event triggered by hardware detecting physical condition (FLOATER_HIGH, TEMPERATURE_HOT). User event triggered by human interaction (button press: WASCHEN_BTN, SPIN_BTN).

3. **Define "state" and "transition" in UML state machines.**
   - Answer: State = condition/mode system is in (IDLE, FILL_WATER). Transition = change from one state to another triggered by event, with associated actions.

4. **What is a safety interlock? Give an example from the washing machine.**
   - Answer: Mechanism preventing dangerous operations. Example: Door lock engaged before filling water, only unlocked when no water and motor stopped (prevents opening during operation).

5. **Define "fail-safe mode" in safety-critical systems.**
   - Answer: Default safe state system enters when failure detected. Example: Traffic light flashing yellow when system fails, indicating drivers should proceed with caution.

6. **What is the purpose of a timer event (TIME_OUT) in a state machine?**
   - Answer: Triggers transition after specified duration, enabling time-based operations (rotate drum for 4s, spin for 5s).

7. **Define "requirement justification." Why is it necessary?**
   - Answer: Explanation of why requirement exists, often referencing standards/regulations. Necessary to show requirement is not arbitrary but based on authority/rationale.

8. **What is the difference between entry action and transition action?**
   - Answer: Entry action executes whenever state is entered (regardless of source). Transition action executes only on specific transition. Lab uses transition actions (actions on arrows).

9. **Define "emergency stop" in the context of control systems.**
   - Answer: Immediate halt of operations triggered by user (STOP_BTN). Must be available from any state, brings system to safe state with cleanup actions.

10. **What is a "clearance interval" in traffic light control?**
    - Answer: All-red period between phase changes ensuring intersection is clear before next direction gets green (prevents collision with cars still in intersection).

### Conceptual Understanding (12 questions)

11. **Explain why the initial traffic light requirements needed improvement. What were the main issues?**
    - Answer: Missing justifications (arbitrary timing), missing scenarios (pedestrians), ambiguous terms (direction changes), no failure handling, no maximum time (starvation risk).

12. **How does the washing machine state machine ensure the door only opens when safe?**
    - Answer: Door locked before operations (IDLE→FILL_WATER: door_lock()), only unlocked in SHUT_DOWN→IDLE after FLOATER_LOW (no water) and motor_off (not rotating). State structure enforces safety.

13. **Why are multiple actions often needed on a single transition?**
    - Answer: Transition must clean up source state (turn off active systems) and set up target state (turn on needed systems). Example: valve_close() (cleanup), heater_on() (setup) on FILL_WATER→HEAT_WATER.

14. **Explain the difference between the wash program and spin-dry program paths. When would you use each?**
    - Answer: Wash (FILL→HEAT→ROT→EMPTY→SPIN): full cycle for dirty laundry. Spin (EMPTY→SPIN): quick dry for already-washed items. Different entry points from IDLE provide flexibility.

15. **How does the state machine handle the STOP_BTN from different states? Why is this design pattern important?**
    - Answer: STOP_BTN transitions from all operational states to SHUT_DOWN, with state-specific cleanup actions. Important for safety: user control at all times, ensures safe shutdown regardless of current state.

16. **Why must the heater be turned off before rotating the drum? How does the state machine enforce this?**
    - Answer: Safety/energy requirement (heating while rotating wastes energy, may be unsafe). Enforced by transition HEAT_WATER→ROTATE_RIGHT including heater_off() action before motor_on().

17. **What role does the SHUT_DOWN state play in the washing machine design?**
    - Answer: Cleanup/safe-stop state. Drains remaining water (pump_on), waits for FLOATER_LOW, then unlocks door and returns to IDLE. Funnels all program completions and emergency stops through single path.

18. **Explain why traffic light requirements need both minimum and maximum green time.**
    - Answer: Minimum ensures enough cars pass (efficiency, fairness). Maximum prevents starvation (one direction stuck on red indefinitely). Balance between throughput and fairness.

19. **How does peer review improve requirement quality? Use the lab example.**
    - Answer: Reviewer Dominic caught missing failure handling, questioned arbitrary timing values, identified missing scenarios (pedestrians), noted ambiguities. Fresh perspective reveals author's blind spots.

20. **Why is the washing machine modeled as event-driven rather than time-driven?**
    - Answer: Operations depend on sensor conditions (water level, temperature) not just time. Can't predict exact fill time (depends on pressure), heat time (depends on start temp). Events provide flexible, accurate control.

21. **What is the purpose of the IDLE state in the washing machine?**
    - Answer: Ready state waiting for user input. Door closed and locked (safe but ready), all systems off, waiting for WASCHEN_BTN, SPIN_BTN, or DOOR_OPENED. Central hub for program selection.

22. **How would you verify that the washing machine state machine correctly implements all requirements?**
    - Answer: Trace each requirement to state machine elements. Check: states cover modes, transitions implement flows, actions control actuators, events match sensors, safety interlocks present. Walk through scenarios.

### Application and Analysis (15 questions)

23. **Improve this traffic light requirement: "The light should be green for a while." Use lab lessons.**
    - Answer: "The traffic light SHALL remain green for a minimum of 15 seconds and a maximum of 60 seconds per phase, as specified in traffic regulation XYZ, to balance traffic throughput and fairness across all directions."

24. **Draw the state transitions for the full wash program (WASCHEN_BTN). List all states and events.**
    - Answer: IDLE→[WASCHEN_BTN]→FILL_WATER→[FLOATER_HIGH]→HEAT_WATER→[TEMPERATURE_HOT]→ROTATE_RIGHT→[TIME_OUT]→ROTATE_LEFT→[TIME_OUT]→EMPTY_WATER→[FLOATER_LOW]→SPIN_DRY→[TIME_OUT]→SHUT_DOWN→[FLOATER_LOW]→IDLE.

25. **A colleague writes a requirement: "The machine should do good washing." Provide constructive feedback.**
    - Answer: Too vague, not testable. Ask: What defines "good"? Suggest: "The machine SHALL rotate the drum left and right for 4 seconds each at 60 RPM, with water temperature of 60°C (HOT), to ensure thorough cleaning according to IEC standard XYZ."

26. **What actions should occur on the transition from ROTATE_LEFT to EMPTY_WATER? Justify each.**
    - Answer: motor_off() (stop rotating), pump_on() (start draining), state=EMPTY_WATER. Motor must stop (rotation complete), pump starts removal of dirty water.

27. **Design a state machine fragment for pedestrian crossing integrated with traffic lights. Define states, events, actions.**
    - Answer: States: VEHICLE_GREEN/PEDESTRIAN_RED (vehicles go), VEHICLE_RED/PEDESTRIAN_GREEN (pedestrians cross). Events: pedestrian_button, timer. Actions: vehicle_light(green/red), ped_light(green/red), start_timer. Interlock: pedestrian green only when vehicle red.

28. **The FLOATER_HIGH sensor fails and never triggers. Trace the system behavior. What state does it get stuck in?**
    - Answer: System stuck in FILL_WATER. Valve stays open (water keeps filling), heater never turns on, never transitions to HEAT_WATER. Dangerous: overflow. Need timeout or overflow sensor for safety.

29. **Why does the washing machine have separate ROTATE_RIGHT and ROTATE_LEFT states instead of one ROTATE state with direction parameter?**
    - Answer: Simpler state machine design (each state = one behavior). Clear sequencing (right then left). Separate timer events for each direction. Could use one ROTATE state with internal transitions, but separate states clearer.

30. **A user presses WASCHEN_BTN, system reaches HEAT_WATER, then user presses STOP_BTN. Trace all actions and final state.**
    - Answer: HEAT_WATER→[STOP_BTN]→SHUT_DOWN: Actions: heater_off(), pump_on(), state=SHUT_DOWN. SHUT_DOWN waits for FLOATER_LOW, then: pump_off(), door_unlock(), state=IDLE. Final state: IDLE (ready, safe, door unlocked).

31. **Compare two design approaches: (1) STOP_BTN transitions from every state, (2) STOP_BTN sets global flag checked in every state. Which is better for state machines?**
    - Answer: Approach 1 (explicit transitions) better. Clearly visible in diagram, explicit cleanup actions per state, no implicit behavior. Global flag harder to analyze, actions not clear, violates state machine formalism.

32. **Write a requirement for the washing machine heater that includes: mandatory keyword, constraint, and rationale.**
    - Answer: "The water heater SHALL be turned off before the drum motor starts rotating (constraint) to prevent energy waste and ensure safe operation (rationale), as specified in appliance safety standard IEC 60335."

33. **The reviewer suggests adding pedestrian requirements to traffic lights. You have limited time. Is this a MUST, SHOULD, or MAY using MoSCoW? Justify.**
    - Answer: MUST. Safety-critical: pedestrians are vulnerable road users, crossroads typically have crosswalks. Omitting pedestrians creates unsafe system. Not optional for complete traffic control system.

34. **Design SHUT_DOWN state transitions and actions to safely stop from any prior state. Consider what might be on.**
    - Answer: Entry to SHUT_DOWN: valve_close() (may be filling), heater_off() (may be heating), motor_off() (may be rotating), pump_on() (drain any water). Wait for FLOATER_LOW event. Exit: pump_off(), door_unlock(), state=IDLE.

35. **A washing machine is in SPIN_DRY. Timer has 2 seconds left. User presses STOP_BTN. Should the system: (a) wait 2s then stop, or (b) stop immediately? Justify based on requirements.**
    - Answer: (b) Stop immediately. Requirement: "Stop button shall be possible at all times and result in...stopping all operations." "At all times" and "result in" implies immediate action, not waiting. Safety/responsiveness priority.

36. **Analyze this state machine design error: IDLE has transitions for WASCHEN_BTN and SPIN_BTN both going to FILL_WATER. What problems does this create?**
    - Answer: Wrong: SPIN_BTN should go to EMPTY_WATER (skip washing). Current design forces full wash regardless of button. Violates requirement: "Spin button shall start spin-dry program" (not wash+spin). Buttons not distinguishable.

37. **Propose a state machine extension to handle washing at different temperatures (COLD, WARM, HOT). Where and how would you add this?**
    - Answer: Add choice pseudo-state or button events at IDLE: WASCHEN_COLD_BTN, WASCHEN_WARM_BTN, WASCHEN_HOT_BTN. Or: FILL_WATER→choice based on user selection→[cold]→skip HEAT_WATER→ROTATE, [warm]→HEAT_WATER(medium), [hot]→HEAT_WATER(full). Requires temperature control.

### Critical Thinking and Synthesis (10 questions)

38. **Critique: "Peer review wastes time. I can write perfect requirements alone." Use lab evidence.**
    - Answer: False. Lab shows initial requirements had gaps reviewer caught: missing pedestrians, no failure handling, unclear terms, no justification. Author blind to own assumptions. Reviewer's fresh perspective essential for quality.

39. **Why is it important to reference regulations/standards in safety-critical requirements (traffic lights, washing machines)?**
    - Answer: Provides authority (not arbitrary), ensures compliance (legal requirement), enables traceability (audit trail), shares knowledge (engineers refer to standards), liability protection (followed recognized practices).

40. **Design a verification test plan for the traffic light requirement: "Yellow light SHALL show for 3 seconds before red."**
    - Answer: Test: (1) Set light to green. (2) Trigger transition to red. (3) Measure yellow duration with timer. (4) Verify: 2.9s ≤ duration ≤ 3.1s. (5) Repeat 100 times. (6) Check: all within tolerance. Pass/Fail: All measurements in range.

41. **How would you handle conflicting requirements: "Wash program SHALL complete in under 20 seconds" vs. "Water SHALL be heated to HOT (60°C)"? Assume heating takes 30s.**
    - Answer: Requirements conflict (physically impossible). Must negotiate with stakeholders: (1) Relax timing (extend to 40s), (2) Reduce temperature (warm not hot), (3) Add fast-wash mode (cold only), or (4) Use pre-heated water. Document decision and rationale.

42. **Compare state machine modeling (UML) vs. implementing directly in code. What advantages does modeling first provide?**
    - Answer: Modeling: visual (easier to understand/review), formal (can verify correctness), documentation (design artifact), abstraction (hide implementation details). Can catch design errors before coding. Code generation possible. Team communication easier.

43. **The washing machine lab solution shows ROTATE_RIGHT→ROTATE_LEFT transition with pump_on(). Is this correct? Analyze.**
    - Answer: Likely error. Pump should not turn on between rotations (washing step, need water). Pump should only activate during EMPTY_WATER. May be copy-paste error in student solution. Shows importance of validation.

44. **Propose a state machine extension for washing machine: add "PAUSE" feature. User can pause and resume. What states, events, transitions?**
    - Answer: Add PAUSE_BTN event (global from operational states→PAUSED state). PAUSED state: save context (current state), turn off actuators, unlock door. RESUME_BTN→return to previous state with history. Requires history pseudo-state (deep H*) or state stack.

45. **Why might a state machine have multiple paths to the same state (e.g., EMPTY_WATER reachable from HEAT_WATER, ROTATE_LEFT, and IDLE)?**
    - Answer: Different use cases: IDLE→EMPTY_WATER (spin-only program), ROTATE_LEFT→EMPTY_WATER (wash program flow), HEAT_WATER→EMPTY_WATER (SPIN_BTN during heating). Provides flexibility. Each path may have different preconditions/actions.

46. **Analyze the trade-off: simple sequential state machine vs. complex hierarchical/orthogonal state machine for washing machine control.**
    - Answer: Simple: easier to understand/implement/debug, sufficient for washing machine. Complex: more powerful (concurrent operations), harder to validate, may be over-engineering for this application. Choose simplest design meeting requirements.

47. **How would you implement the state machine if the embedded system has no RTOS, just a polling main loop?**
    - Answer: State variable (enum), switch statement in loop, check events (poll sensors/buttons), execute transitions (update state, call action functions), repeat. Simple but effective. Event queue if needed for buffering.

### Scenario-Based Questions (8 questions)

48. **You're reviewing a colleague's traffic light requirements. They wrote: "Green light lasts 15-60s depending on traffic." What feedback would you provide?**
    - Answer: Too vague—how is "depending on traffic" determined? Need specific algorithm or sensor criteria. If adaptive control, define: sensor type, decision logic, response time. If fixed: specify exact duration or range selection method. Not testable as written.

49. **During washing machine testing, you find: FILL_WATER→HEAT_WATER occurs even when water level is only half-full. What's wrong, and how do you fix it?**
    - Answer: Problem: FLOATER_HIGH sensor malfunction or threshold too low. Fix: (1) Check sensor placement/calibration, (2) Adjust sensor threshold, (3) Add guard condition to transition [water_level>threshold], (4) Test with various water levels to verify correct trigger.

50. **Design a state machine for a microwave oven: door open/close, start button, timer, rotating plate. Include safety interlock (no heating if door open).**
    - Answer: States: DOOR_OPEN, IDLE, HEATING. Initial→DOOR_OPEN. DOOR_OPEN→[door_close]→IDLE. IDLE→[start_btn]→HEATING (magnetron_on, rotate_on, start_timer). HEATING→[time_out]→IDLE (magnetron_off, rotate_off). HEATING→[door_open]→DOOR_OPEN (magnetron_off, rotate_off—safety interlock).

51. **The washing machine reaches HEAT_WATER but temperature never reaches HOT (sensor broken). System hangs. How would you improve the design to handle this?**
    - Answer: Add timeout: start_timer(max_heat_time) in HEAT_WATER. Add transition: HEAT_WATER→[TIME_OUT]→ERROR_STATE or HEAT_WATER→[TIME_OUT]→ROTATE_RIGHT (proceed anyway with warning). Prevents infinite wait, improves robustness.

52. **Traffic lights at intersection: NS green, EW red. Power failure. Both lights go dark. Cars proceed, collision occurs. How should requirements address this?**
    - Answer: Improved requirement 1 already addresses: "In case of system failure: all lights flash yellow." Implementation: backup power (battery) keeps lights flashing, or passive fail-safe (relay defaults to yellow when power lost). Safety-critical.

53. **You're implementing the washing machine on embedded hardware. STOP_BTN transitions exist from 6 states. How do you structure code to avoid duplication?**
    - Answer: Create stop_button_handler() function containing common cleanup code. In state switch, check STOP_BTN before state-specific logic. Call handler, set state=SHUT_DOWN. Or: event queue with priority (STOP_BTN highest priority, preempts other events).

54. **A washing machine user reports: pressed SPIN_BTN, but machine filled water first, then drained it, then spun. Machine wasted time/water. Analyze using state machine.**
    - Answer: Incorrect: if drum has water, IDLE→EMPTY_WATER should drain first, then spin. If no water, EMPTY_WATER→SPIN_DRY proceeds immediately (pump runs briefly). User observation suggests water was added—state machine bug or user misunderstanding? Verify actual behavior vs. model.

55. **Design a test scenario to verify the door safety interlock. What states, events, and expected behaviors?**
    - Answer: Test: (1) System in FILL_WATER (water flowing, door locked). (2) Try to open door (pull handle). (3) Expected: Door doesn't open (mechanical lock holds). (4) Press STOP_BTN. (5) System→SHUT_DOWN→IDLE (after drain). (6) Try to open door. (7) Expected: Door opens (unlocked). Pass/Fail: Door only opens in step 6.

### Connections and Integration (7 questions)

56. **How does Lab U01 Task 1 (requirements) relate to Lecture 03 concepts? Name at least 3 connections.**
    - Answer: (1) RFC 2119 keywords (SHALL/MUST for mandatory requirements). (2) Requirements characteristics (clear, complete, testable). (3) Peer review as validation method. (4) Iterative refinement process. (5) Safety requirements importance.

57. **How would you implement the washing machine state machine in C code for an embedded system (like Ultra96 from Lab P1)?**
    - Answer: enum for states, struct for state machine context, switch statement in main loop, sensor reading via GPIO, actuator control via GPIO/PWM, timer using RTOS or hardware timer, event queue for buffering, action functions for valve/motor/pump control.

58. **Connect the washing machine emergency stop (STOP_BTN) to the terminate pseudo-state concept from Lecture 04.**
    - Answer: Similar function: immediate exit from any state to termination/shutdown. STOP_BTN = explicit event trigger, terminate = implicit (design element). Both provide global escape, both lead to shutdown/cleanup state, both prioritize safety.

59. **How do the timing requirements in this lab (3s yellow, 4s rotation) relate to real-time NFRs that will be covered later?**
    - Answer: Timing requirements = real-time constraints (deadlines). 3s yellow = hard deadline (critical for safety). 4s rotation = soft deadline (affects quality but not safety). RTOS scheduling must guarantee timing requirements met.

60. **Relate the state machine's event-driven nature to the software paradigms from Lecture 02.**
    - Answer: Lab washing machine = event-driven paradigm. System waits for events (WASCHEN_BTN, FLOATER_HIGH), responds with actions, returns to waiting. Contrasts with time-driven (executes on fixed schedule) and procedural (sequential execution).

61. **How would model checking (Lecture 04) help verify the washing machine state machine? What properties would you check?**
    - Answer: Properties: (1) AG ¬(door_unlocked ∧ water_present) — door never unlocked with water. (2) AG (in_IDLE → AF in_IDLE) — always eventually returns to IDLE. (3) AG (stop_btn → AF in_IDLE) — stop always reaches IDLE. (4) AG ¬deadlock — no stuck states.

62. **Connect the lab's requirements iteration process to the development processes that will be covered later.**
    - Answer: Requirements→review→improve = V-model requirements phase with validation. Iterative refinement = agile sprints. Peer review = verification activity. Lab demonstrates that development is iterative, not waterfall one-pass.

### Advanced Understanding (8 questions)

63. **Analyze: State machines enforce safety through structure (which states exist, which transitions allowed). Give 3 examples from washing machine.**
    - Answer: (1) DOOR_OPEN only reachable from IDLE (ensures safe conditions). (2) No transition from operational states to DOOR_OPEN (prevents opening during operation). (3) HEAT_WATER→ROTATE_RIGHT requires TEMPERATURE_HOT (ensures heating complete).

64. **Compare explicit state machine (9 states) vs. flat code with if-statements and flags. Why is state machine better for this application?**
    - Answer: State machine: clear structure, formal semantics, visual representation, easier to verify, scales better, less bug-prone. Flat code with flags: harder to understand (flag interactions complex), error-prone (missing flag checks), hard to verify, maintenance nightmare as complexity grows.

65. **The lab uses Mermaid syntax for state machines. How does this compare to formal UML? What's lost/gained?**
    - Answer: Mermaid: simpler syntax, text-based (version control friendly), fast to write, good for documentation. Lost: some UML features (entry/exit/do actions separate, guards, pseudo-states). Gained: ease of use, tool-free editing. Trade-off: simplicity vs. expressiveness.

66. **Propose a formal verification approach for the traffic light requirement: "North-south and east-west never both green." What formalism and property?**
    - Answer: Model as state machine with states: NS_GREEN_EW_RED, NS_RED_EW_GREEN, ALL_RED, etc. Property (CTL): AG ¬(ns_green ∧ ew_green). Model checker explores all reachable states, verifies property holds. If violated, provides counterexample showing how both become green.

67. **Why might a washing machine implementation use a timeout in HEAT_WATER even though TEMPERATURE_HOT event exists?**
    - Answer: Defense against sensor failure (if TEMPERATURE_HOT never triggers, timeout prevents infinite wait). Degraded operation (proceed with lukewarm water after timeout, better than hanging). Robustness/fault tolerance. Lab shows ideal case; production needs fault handling.

68. **Analyze the dependency graph of washing machine states. Which states must execute in strict sequence? Which are parallel/independent?**
    - Answer: Strict sequence: FILL→HEAT→ROTATE_R→ROTATE_L→EMPTY→SPIN (dependencies: can't heat without water, can't rotate without hot water, can't spin without draining). Independent: IDLE can go to multiple states (choice), SHUT_DOWN can be reached from multiple states (join-like).

69. **How would you extend the state machine to support multiple wash programs (Delicate, Normal, Heavy) with different rotation times/speeds?**
    - Answer: Add program selection at IDLE: DELICATE_BTN, NORMAL_BTN, HEAVY_BTN→set program variable. Use choice pseudo-states or parameterized transitions: start_timer(rotation_time[program]), motor_speed[program]. Or: separate state sets per program (more states, clearer).

70. **Compare the lab's sequential state machine to an orthogonal regions approach. Could washing machine use concurrent regions? What would they be?**
    - Answer: Possible orthogonal regions: (1) Water control (fill, heat, drain) + (2) Motor control (idle, rotate, spin) + (3) Door status (open, locked). But: these aren't truly independent (motor can't run if no water, door can't open if motor running). Sequential better captures dependencies.

These 70 questions cover all aspects of Lab U01, from basic definitions to advanced analysis, scenario-based problem-solving, and connections to other course topics. They prepare you for expert-level oral exam discussions.
