# Lab U2: Modelling & Implementation

## Technical Terms & Definitions

### Software Architecture Concepts

**Modular Structure**
- Organization of software into separate, independent modules
- Each module has a specific responsibility
- Modules communicate through well-defined interfaces (APIs)
- Enables separation of concerns and easier testing

**API (Application Programming Interface)**
- Specification of how software components interact
- Defines function signatures, data types, and behaviors
- Acts as a contract between modules
- Separates interface from implementation

**Mockup Functions (Mock Objects)**
- Simulated implementations of hardware or external interfaces
- Used for testing without actual hardware
- Implement the same API as real functions
- Enable development and testing in parallel with hardware availability

**Hardware Abstraction Layer (HAL)**
- Software layer between application and hardware
- Provides consistent API regardless of hardware details
- Makes code portable across different hardware platforms
- Separates application logic from hardware-specific code

**Event-Driven Architecture**
- Software responds to events (button presses, sensor signals, timeouts)
- Event handler detects and generates events
- State machine processes events and triggers actions
- Asynchronous, reactive programming model

### State Machine Implementation

**State Machine (Finite State Machine, FSM)**
- Model consisting of states, events, and transitions
- Only one state active at any time
- Events trigger transitions between states
- Actions executed during transitions or in states

**State (state_t)**
- Represents a distinct mode or condition of the system
- Defined as enumeration in C
- Examples: DOOR_OPEN, IDLE, FILL_WATER, HEAT_WATER, etc.

**Event (event_t)**
- Something that happens (button press, sensor trigger, timeout)
- Causes state machine to evaluate transitions
- Defined as enumeration in C
- Examples: DOOR_OPENED, BUTTON_WASH, TIME_OUT, FLOATER_HIGH, etc.

**Transition**
- Change from one state to another
- Triggered by specific event in specific state
- May include guard conditions
- May execute actions

**Action**
- Activity performed during transition or in state
- Examples: motor_on(), heater_on(), pump_on()
- Implemented by action_handler module

### Hardware Interfaces

**Active-Low Signal**
- Logic convention where logical "0" (low voltage) represents active/true
- Logical "1" (high voltage) represents inactive/false
- Example: Input "Door Closed" = 0 means door is closed
- Common in embedded systems for noise immunity

**Edge Detection**
- Detecting change in signal level
- **Falling Edge**: Transition from high (1) to low (0)
- **Rising Edge**: Transition from low (0) to high (1)
- Used to detect events (button presses, sensor changes)

**Memory-Mapped I/O**
- Hardware registers accessed like memory locations
- Read from memory address to get input state
- Write to memory address to control outputs
- CT Board: Input at 0x60000410, Output at 0x60000400

**GPIO (General Purpose Input/Output)**
- Pins that can be configured as inputs or outputs
- Used to interface with sensors, buttons, actuators
- Controlled through memory-mapped registers
- CT Board uses CPLD (Complex Programmable Logic Device) for GPIO

### Washing Machine Components

**Inputs (Sensors/Buttons):**
- **Floater High**: Water level sensor indicating high level
- **Floater Low**: Water level sensor indicating low level
- **Button Stop**: Emergency stop or cancel button
- **Button Spin**: Start spin cycle button
- **Button Wash**: Start wash cycle button
- **Thermostat Low**: Temperature sensor (water not hot yet)
- **Door Closed**: Door position sensor

**Outputs (Actuators):**
- **Heater On**: Electric water heater control
- **Valve On**: Water inlet valve control
- **Door Locked**: Door lock mechanism control
- **Pump**: Water drain pump control (external motor)
- **Motor**: Drum motor with multiple control signals:
  - Motor Left/Right: Direction control
  - Motor Slow/Fast: Speed control (1200 Hz = fast, 100 Hz = slow)

### Software Modules

**timer Module**
- Implements timing functionality
- **timer_start(duration)**: Start timer with duration in 100Hz cycles
  - Example: timer_start(100) = 1 second timer
- **timer_stop()**: Stop currently running timer
- Generates TIME_OUT event when timer reaches zero

**event_handler Module**
- Detects and generates events
- **eh_get_event()**: Polls inputs and timer, returns detected event
- Edge detection logic for all inputs
- Timer event generation (previous != 0, current == 0 → TIME_OUT)
- Returns NO_EVENT if nothing detected

**action_handler Module**
- Implements actions triggered by state machine
- Functions for controlling all outputs
- Examples: heater_on(), heater_off(), valve_open(), valve_close()
- Implementation: Manipulate memory-mapped output register

**state_machine Module**
- Core application logic
- **fsm_init()**: Initialize state machine to starting state
- **fsm_handle_event(event_t event)**: Process event based on current state
- Implements state transition logic
- Triggers appropriate actions

### CT Board Hardware

**CT Board (Control Technology Board)**
- Microcontroller development board
- ARM Cortex-M0 processor
- Memory-mapped peripherals
- Buttons, DIP switches, LEDs for I/O
- LCD display
- External connector for washing machine hardware

**CPLD (Complex Programmable Logic Device)**
- Programmable logic device on CT Board
- Interfaces between MCU and external hardware
- Handles I/O signals
- Accessed via memory addresses

**Keil µVision**
- Integrated Development Environment (IDE)
- For ARM microcontrollers
- Compiler, debugger, simulator
- Project management

## Core Concepts

### From Model to Code

**Development Flow:**
1. **Requirements**: Defined in previous lab (U01)
2. **Model**: UML state machine diagram (created in U01)
3. **Architecture**: Define modular structure and APIs
4. **Implementation**: Write code implementing the model
5. **Testing**: Verify implementation against requirements

**Key Principle**: Model-driven development
- Design behavior using models (UML state machines)
- Implement model in code systematically
- Maintain traceability from requirements through model to code

### Modular Software Architecture

**Separation of Concerns:**
- **timer**: Time management (independent of application)
- **event_handler**: Input monitoring and event generation
- **action_handler**: Output control and hardware interface
- **state_machine**: Application logic (washing machine behavior)
- **main**: System initialization and main loop

**Benefits:**
- **Testability**: Each module can be tested independently
- **Maintainability**: Changes localized to specific modules
- **Reusability**: Modules can be reused in other projects
- **Clarity**: Clear responsibility for each module

### Hardware Abstraction

**Abstraction Layers:**

```
┌─────────────────────────────────┐
│     State Machine (Application) │  ← High-level logic
├─────────────────────────────────┤
│        Action Handler (API)     │  ← Hardware abstraction
├─────────────────────────────────┤
│    Memory-Mapped I/O Registers  │  ← Low-level hardware access
├─────────────────────────────────┤
│          Physical Hardware      │  ← Actual washing machine
└─────────────────────────────────┘
```

**Purpose:**
- Application code doesn't directly manipulate hardware
- API functions provide semantic interface (e.g., "valve_open()" not "write 0 to bit 6")
- Easy to swap implementations (mockup vs. real hardware)
- Portable code

### Mockup Strategy

**Development Without Hardware:**
- Use CT Board buttons to simulate washing machine buttons
- Use CT Board DIP switches to simulate sensors
- Use CT Board LEDs to indicate actuator states
- Same API, different implementation

**Mockup vs. Real Implementation:**

| Aspect | Mockup | Real Hardware |
|--------|---------|---------------|
| **Input Source** | CT Board buttons/switches | Washing machine sensors |
| **Output Target** | CT Board LEDs | Washing machine actuators |
| **API** | Same function signatures | Same function signatures |
| **Implementation** | Read button registers | Read external connector |
| **Purpose** | Development & testing | Production |

**Example:**
```c
// API (same for both)
void valve_open(void);

// Mockup implementation
void valve_open(void) {
    // Turn on LED on CT Board
    set_led(LED_VALVE, ON);
}

// Real implementation
void valve_open(void) {
    // Set bit in output register to control actual valve
    *OUTPUT_REG &= ~(1 << VALVE_BIT);  // Active-low
}
```

### Event-Driven State Machine

**Main Loop Pattern:**
```c
int main(void) {
    // Initialize all modules
    fsm_init();
    timer_init();

    // Main event loop
    while(1) {
        event_t event = eh_get_event();  // Poll for events
        if(event != NO_EVENT) {
            fsm_handle_event(event);      // Process event
        }
    }
}
```

**Event Processing Flow:**
1. **Poll**: Check all inputs and timer for changes
2. **Detect**: Identify which event occurred
3. **Process**: State machine evaluates current state + event
4. **Transition**: Change to new state if appropriate
5. **Act**: Execute actions associated with transition
6. **Repeat**: Continue polling

### State Machine Implementation Pattern

**Typical Structure:**
```c
void fsm_handle_event(event_t event) {
    switch(current_state) {
        case IDLE:
            switch(event) {
                case DOOR_OPENED:
                    // Actions
                    door_unlock();
                    // Transition
                    current_state = DOOR_OPEN;
                    break;
                case BUTTON_WASH:
                    // Actions
                    door_lock();
                    valve_open();
                    // Transition
                    current_state = FILL_WATER;
                    break;
                // Other events...
            }
            break;
        case FILL_WATER:
            // Handle events for FILL_WATER state...
            break;
        // Other states...
    }
}
```

**Nested Switch Pattern:**
- Outer switch: Current state
- Inner switch: Event
- Clear mapping from UML diagram to code
- Each case corresponds to a transition in the diagram

### Timer Implementation

**Timer Mechanism:**
- 100 Hz periodic interrupt (every 10ms)
- Decrement counter each interrupt
- When counter reaches 0, timer has expired
- Event handler detects transition from non-zero to zero

**Usage Pattern:**
```c
// Start 5-second timer
timer_start(500);  // 500 * 10ms = 5 seconds

// In state machine, wait for timeout
case ROTATE_RIGHT:
    if(event == TIME_OUT) {
        motor_off();
        current_state = ROTATE_LEFT;
    }
    break;
```

**Timer Event Detection:**
```c
// In event_handler.c
static uint32_t previous_timer_value = 0;

event_t eh_get_event(void) {
    uint32_t current_timer = timer_get_value();

    // Check for timer timeout
    if(previous_timer_value != 0 && current_timer == 0) {
        previous_timer_value = current_timer;
        return TIME_OUT;
    }

    previous_timer_value = current_timer;

    // Check other events...
}
```

### Memory-Mapped I/O

**Concept:**
- Hardware registers appear at specific memory addresses
- Read/write operations access hardware
- No special instructions needed (unlike Port I/O)

**CT Board I/O Registers:**
```c
#define INPUT_REG  ((volatile uint8_t*) 0x60000410)
#define OUTPUT_REG ((volatile uint8_t*) 0x60000400)

// Read inputs
uint8_t inputs = *INPUT_REG;

// Write outputs
*OUTPUT_REG = 0xFF;  // All outputs high
```

**Volatile Keyword:**
- Tells compiler value can change unexpectedly
- Prevents optimization that would break hardware access
- Essential for memory-mapped I/O

### Active-Low Logic

**Convention:**
- 0 (low voltage) = active/true/on
- 1 (high voltage) = inactive/false/off

**Reasoning:**
- Better noise immunity (pull-up resistors)
- Safer default state (if line disconnected, pulled high = inactive)
- Common in industrial/embedded systems

**Implementation:**
```c
// Check if door is closed (active-low)
if((inputs & DOOR_CLOSED_BIT) == 0) {
    // Door is closed
}

// Turn on heater (active-low output)
*OUTPUT_REG &= ~HEATER_BIT;  // Clear bit (set to 0)

// Turn off heater
*OUTPUT_REG |= HEATER_BIT;   // Set bit (set to 1)
```

**Bit Manipulation:**
- Set bit to 0: `value &= ~(1 << bit)`
- Set bit to 1: `value |= (1 << bit)`
- Toggle bit: `value ^= (1 << bit)`
- Test bit: `(value & (1 << bit)) != 0`

## Important Diagrams & Graphics

### 1. Washing Machine I/O Schematic

**Description:**
Two horizontal bars labeled "INPUT" and "OUTPUT" with 8 pins each (0-7), connected to various washing machine components.

**Inputs (top bar, pins 0-7):**
- Pin 0: Floater High (water level sensor)
- Pin 1: (unused)
- Pin 2: Floater Low (water level sensor)
- Pin 3: Button Stop
- Pin 4: Button Spin
- Pin 5: Button Wash
- Pin 6: Thermostat Low (temperature sensor)
- Pin 7: Door Closed

Each input connected to GND through pull-down configuration (active-low).

**Outputs (bottom bar, pins 0-7):**
- Pin 0-1: (unused)
- Pin 2-3: Motor control (Fast/Slow)
  - External clock signals (1200 Hz / 100 Hz)
  - Motor direction: Left/Right
- Pin 4: Pump control (M = motor symbol)
- Pin 5: Heater On
- Pin 6: Valve On
- Pin 7: Door Locked

All outputs active-low (0 = on, 1 = off).

**How to draw:**
1. Draw two horizontal rectangles (input/output bars)
2. Label 8 pins on each (0-7)
3. Above input bar: Draw switches/buttons for each input with labels
4. Below output bar: Draw actuator symbols (motor, heater, valve) with labels
5. Show GND connections for inputs
6. Add clock signal indicators for motor (1200Hz, 100Hz)
7. Show AND gate logic for motor slow/fast control

**Meaning:**
- Complete interface specification for washing machine controller
- Shows mapping between logical pins and physical components
- Active-low convention indicated by GND connections

### 2. CT Board Pin-Out Diagram

**Description:**
Rectangular connector diagram showing physical pin layout on CT Board.

**Layout:**
```
Input Pins (top two rows):
Row 1: [ 2 ][ 4 ][ 6 ][ 8 ][10][12][14][16]
Row 2: [ 1 ][ 3 ][ 5 ][ 7 ][ 9][11][13][15]

Logical mapping: Pin 0→2, Pin 1→4, Pin 2→6, Pin 3→8, Pin 4→10, Pin 5→12, Pin 6→14, Pin 7→16

Output Pins (bottom two rows):
Row 1: [ 1 ][ 3 ][ 5 ][ 7 ][ 9][11][13][15]
Row 2: [ 2 ][ 4 ][ 6 ][ 8 ][10][12][14][16]

Logical mapping: Pin 0→1, Pin 1→3, Pin 2→5, Pin 3→7, Pin 4→9, Pin 5→11, Pin 6→13, Pin 7→15
```

**How to draw:**
1. Draw rectangular connector outline
2. Divide into 4 rows of 8 positions each
3. Label physical pin numbers (1-16)
4. Add "Input" label above top two rows
5. Add "Output" label below bottom two rows
6. Show mapping to logical bit positions (0-7)

**Meaning:**
- Maps logical pin numbers (0-7 used in software) to physical connector pins
- Shows oddball 16-pin connector layout
- Essential reference for wiring hardware to CT Board

### 3. Modular Software Architecture Diagram

**Description:**
Block diagram showing software modules and their relationships.

**Modules:**
```
┌──────────────┐
│     Main     │ (initialization, main loop)
└──────┬───────┘
       │
   ┌───┴───┬──────────┬──────────┐
   │       │          │          │
┌──▼──┐ ┌──▼──────┐ ┌▼────────┐ ┌▼─────────┐
│Timer│ │Event    │ │State    │ │Action    │
│     │ │Handler  │ │Machine  │ │Handler   │
└─────┘ └─────────┘ └─────────┘ └──────────┘
                                       │
                               ┌───────▼────────┐
                               │Hardware I/O    │
                               │(Memory-Mapped) │
                               └────────────────┘
```

**How to draw:**
1. Draw "Main" box at top
2. Draw four module boxes below: Timer, Event Handler, State Machine, Action Handler
3. Draw Hardware I/O box at bottom
4. Connect with arrows showing dependencies/calls
5. Label each connection (e.g., "polls events", "triggers actions", "controls hardware")

**Meaning:**
- Shows separation of concerns
- Clear module responsibilities
- Data/control flow through system
- Hardware abstraction at bottom layer

### 4. Event-Driven Main Loop Flowchart

**Description:**
Flowchart showing the main program execution flow.

**Flow:**
```
     [START]
        ↓
  [Initialize Modules]
        ↓
    ┌─────────┐
    │Get Event│←──────────┐
    └────┬────┘           │
         ↓                │
    ╱Event?╲              │
   ╱detected?╲            │
  ╱           ╲           │
 <   NO       YES         │
  ╲           ╱           │
   ╲         ╱            │
    ╲       ╱             │
     ┴─────┤              │
           ↓              │
   [Handle Event]         │
           ↓              │
   [Execute Actions]      │
           ↓              │
   [Change State]         │
           ↓              │
           └──────────────┘
```

**How to draw:**
1. Start with oval "START"
2. Rectangle "Initialize Modules"
3. Rectangle "Get Event" (top of loop)
4. Diamond "Event detected?"
5. If NO: Loop back to "Get Event"
6. If YES: Series of rectangles (Handle Event, Execute Actions, Change State)
7. Arrow back to "Get Event"

**Meaning:**
- Continuous polling loop
- Event-driven architecture
- Actions only occur when events detected
- Infinite loop (typical for embedded systems)

### 5. State Machine Implementation Pattern

**Description:**
Code structure diagram showing nested switch statement pattern.

**Structure:**
```
fsm_handle_event(event)
    │
    └─ switch(current_state)
           │
           ├─ case STATE_1:
           │      └─ switch(event)
           │             ├─ case EVENT_A:
           │             │      ├─ [Actions]
           │             │      └─ current_state = STATE_2
           │             ├─ case EVENT_B:
           │             │      ├─ [Actions]
           │             │      └─ current_state = STATE_3
           │             └─ ...
           │
           ├─ case STATE_2:
           │      └─ switch(event)
           │             └─ ...
           │
           └─ ...
```

**How to draw:**
1. Function box at top: "fsm_handle_event(event)"
2. Outer switch diamond: "current_state?"
3. Branches for each state (case STATE_1, case STATE_2, etc.)
4. For each state, inner switch diamond: "event?"
5. Branches for each event in that state
6. Action boxes and state assignment for each transition

**Meaning:**
- Direct mapping from UML state diagram to code
- Outer switch = current state
- Inner switch = event that occurred
- Each case = one transition in the state diagram
- Clear, systematic structure

### 6. Memory-Mapped I/O Diagram

**Description:**
Diagram showing relationship between memory addresses and hardware registers.

**Layout:**
```
Memory Space:

0x00000000 ┌──────────────┐
           │              │
           │  Program     │
           │  Memory      │
           │              │
           ├──────────────┤
           │              │
           │  Data        │
           │  Memory      │
           │              │
0x60000400 ├──────────────┤ ← OUTPUT_REG
           │ Output Reg   │    (Write to control hardware)
           │ [7:0 bits]   │
0x60000410 ├──────────────┤ ← INPUT_REG
           │ Input Reg    │    (Read to get sensor states)
           │ [7:0 bits]   │
           ├──────────────┤
           │              │
           │  Other       │
           │  Peripherals │
           │              │
0xFFFFFFFF └──────────────┘

Hardware Connection:
INPUT_REG  → [CPLD] → External Connector → Washing Machine Sensors
OUTPUT_REG → [CPLD] → External Connector → Washing Machine Actuators
```

**How to draw:**
1. Draw vertical bar representing memory address space
2. Label key addresses (0x00000000, 0x60000400, 0x60000410, etc.)
3. Divide into regions (Program, Data, Peripherals)
4. Highlight OUTPUT_REG and INPUT_REG locations
5. Show bit layout [7:0] for each register
6. Draw arrows to hardware blocks (CPLD, Connector, Devices)

**Meaning:**
- Shows where in memory space hardware registers are located
- Read/write operations at these addresses access hardware
- CPLD translates between MCU and external connector
- Memory-mapped I/O = hardware looks like memory

### 7. Mockup vs. Real Hardware Mapping

**Description:**
Side-by-side comparison showing how mockup and real implementations map to hardware.

**Layout:**
```
Development Phase (Mockup):

┌─────────────────┐         ┌─────────────────┐
│  State Machine  │────────>│Action Handler   │
│                 │<────────│(Mockup Impl.)   │
└─────────────────┘         └────────┬────────┘
                                     │
                            ┌────────▼────────┐
                            │CT Board LEDs    │
                            │CT Board Buttons │
                            │CT Board Switches│
                            └─────────────────┘

Production Phase (Real Hardware):

┌─────────────────┐         ┌─────────────────┐
│  State Machine  │────────>│Action Handler   │
│  (Same Code!)   │<────────│(Real Impl.)     │
└─────────────────┘         └────────┬────────┘
                                     │
                            ┌────────▼────────┐
                            │External Connector│
                            └────────┬────────┘
                                     │
                            ┌────────▼────────┐
                            │Washing Machine  │
                            │(Real Hardware)  │
                            └─────────────────┘
```

**How to draw:**
1. Two columns: "Development (Mockup)" and "Production (Real)"
2. In each column:
   - State Machine box at top (identical in both)
   - Action Handler box in middle (same API, different implementation)
   - Hardware boxes at bottom (different: CT Board vs. Washing Machine)
3. Arrows showing data flow
4. Emphasize that State Machine code doesn't change

**Meaning:**
- Same application logic (state machine) for both phases
- Only Action Handler implementation changes
- Hardware abstraction enables development without real hardware
- Smooth transition from development to production

## Formulas & Procedures

### Timer Duration Calculation

**Formula:**
```
Duration (seconds) = Count / Frequency

Where:
- Count = number of timer ticks
- Frequency = 100 Hz (timer interrupt rate)
```

**Examples:**
```
1 second:   timer_start(100)     // 100 / 100 = 1 s
5 seconds:  timer_start(500)     // 500 / 100 = 5 s
10 seconds: timer_start(1000)    // 1000 / 100 = 10 s
100 ms:     timer_start(10)      // 10 / 100 = 0.1 s
```

**Usage Pattern:**
```c
// In state machine, starting timer
case FILL_WATER:
    if(event == FLOATER_HIGH) {
        valve_close();
        heater_on();
        timer_start(500);  // 5 second heating time
        current_state = HEAT_WATER;
    }
    break;

// Later, waiting for timer
case HEAT_WATER:
    if(event == TIME_OUT) {
        heater_off();
        motor_on_slow_right();
        timer_start(400);  // 4 second rotation
        current_state = ROTATE_RIGHT;
    }
    break;
```

### Memory-Mapped I/O Access

**Reading Inputs:**
```c
// Define register pointer
#define INPUT_REG  ((volatile uint8_t*) 0x60000410)

// Read entire register
uint8_t inputs = *INPUT_REG;

// Check specific bit (active-low)
#define DOOR_CLOSED_BIT 7

if((inputs & (1 << DOOR_CLOSED_BIT)) == 0) {
    // Door is closed (bit is 0)
}

// Alternative: Check if bit is SET (door open)
if(inputs & (1 << DOOR_CLOSED_BIT)) {
    // Door is open (bit is 1)
}
```

**Writing Outputs:**
```c
// Define register pointer
#define OUTPUT_REG ((volatile uint8_t*) 0x60000400)

// Define bit positions
#define HEATER_BIT 5
#define VALVE_BIT  6
#define DOOR_LOCK_BIT 7

// Turn on heater (set bit to 0, active-low)
*OUTPUT_REG &= ~(1 << HEATER_BIT);

// Turn off heater (set bit to 1)
*OUTPUT_REG |= (1 << HEATER_BIT);

// Set multiple outputs at once
*OUTPUT_REG = 0xFF;  // All outputs off (all bits 1)
*OUTPUT_REG = 0x00;  // All outputs on (all bits 0)

// Control specific bits while preserving others
// Turn on valve, turn off heater:
*OUTPUT_REG = (*OUTPUT_REG & ~(1 << VALVE_BIT)) | (1 << HEATER_BIT);
```

**Bit Manipulation Formulas:**
```c
// Set bit n to 0
value &= ~(1 << n)

// Set bit n to 1
value |= (1 << n)

// Toggle bit n
value ^= (1 << n)

// Test if bit n is set
(value & (1 << n)) != 0

// Test if bit n is clear
(value & (1 << n)) == 0

// Clear specific bits (mask)
value &= ~mask

// Set specific bits (mask)
value |= mask
```

### Edge Detection Algorithm

**Detecting Falling Edge (Active-Low Input Event):**
```c
// Static variable to remember previous state
static uint8_t previous_inputs = 0xFF;  // Initialize all high

event_t eh_get_event(void) {
    uint8_t current_inputs = *INPUT_REG;
    event_t event = NO_EVENT;

    // Check for falling edge on Door Closed (bit 7)
    #define DOOR_CLOSED_BIT 7
    uint8_t door_now = current_inputs & (1 << DOOR_CLOSED_BIT);
    uint8_t door_prev = previous_inputs & (1 << DOOR_CLOSED_BIT);

    if(door_prev && !door_now) {
        // Falling edge detected: door was open (1), now closed (0)
        event = DOOR_CLOSED;
    }
    else if(!door_prev && door_now) {
        // Rising edge detected: door was closed (0), now open (1)
        event = DOOR_OPENED;
    }

    // Update previous state
    previous_inputs = current_inputs;

    return event;
}
```

**General Edge Detection Pattern:**
```c
// For falling edge (high→low): previous=1 AND current=0
if((previous & bit_mask) && !(current & bit_mask)) {
    // Falling edge on this bit
}

// For rising edge (low→high): previous=0 AND current=1
if(!(previous & bit_mask) && (current & bit_mask)) {
    // Rising edge on this bit
}

// For any edge (change): previous XOR current
if((previous & bit_mask) != (current & bit_mask)) {
    // Edge (change) detected on this bit
}
```

### State Machine Implementation Procedure

**Step-by-Step Process:**

**1. Define States and Events (in header files):**
```c
// state_machine.h
typedef enum {
    DOOR_OPEN,
    IDLE,
    FILL_WATER,
    HEAT_WATER,
    ROTATE_RIGHT,
    ROTATE_LEFT,
    EMPTY_WATER,
    SPIN_DRY,
    SHUT_DOWN
} state_t;

// event_handler.h
typedef enum {
    NO_EVENT,
    TIME_OUT,
    DOOR_OPENED,
    DOOR_CLOSED,
    BUTTON_WASH,
    BUTTON_SPIN,
    BUTTON_STOP,
    FLOATER_HIGH,
    FLOATER_LOW,
    TEMPERATURE_HOT
} event_t;
```

**2. Initialize State Machine:**
```c
// state_machine.c
static state_t current_state;

void fsm_init(void) {
    current_state = DOOR_OPEN;
    // Initialize hardware to safe state
    heater_off();
    valve_close();
    motor_off();
    pump_off();
    door_unlock();
}
```

**3. Implement Event Handler:**
```c
void fsm_handle_event(event_t event) {
    switch(current_state) {
        case DOOR_OPEN:
            if(event == DOOR_CLOSED) {
                current_state = IDLE;
            }
            break;

        case IDLE:
            if(event == DOOR_OPENED) {
                door_unlock();
                current_state = DOOR_OPEN;
            }
            else if(event == BUTTON_WASH) {
                door_lock();
                valve_open();
                current_state = FILL_WATER;
            }
            else if(event == BUTTON_SPIN) {
                pump_on();
                current_state = EMPTY_WATER;
            }
            break;

        // Continue for all states...
    }
}
```

**4. Implement Each Transition from UML Diagram:**

For each transition in state diagram:
1. Find source state in outer switch
2. Find event that triggers transition in inner switch
3. Add action function calls
4. Add state assignment
5. Add break statement

**Example Mapping:**
```
UML: IDLE --[BUTTON_WASH / door_lock(); valve_open()]--> FILL_WATER

Code:
case IDLE:
    if(event == BUTTON_WASH) {
        door_lock();        // Actions
        valve_open();
        current_state = FILL_WATER;  // Transition
    }
    break;
```

### Testing Procedure

**Systematic Testing Approach:**

**1. Module Testing (Unit Tests):**
```
Test timer module:
- Start timer with various durations
- Verify countdown
- Verify timeout detection

Test event_handler:
- Simulate input changes
- Verify correct events generated
- Verify edge detection (not level)

Test action_handler:
- Call each action function
- Verify correct output bits set/cleared
- Verify no side effects

Test state_machine (with mocks):
- Initialize to each state
- Send each possible event
- Verify correct next state
- Verify correct actions called
```

**2. Integration Testing:**
```
Test sequences of events:
1. Power on → expect DOOR_OPEN state
2. Close door → expect IDLE state
3. Press BUTTON_WASH → expect FILL_WATER state
4. Trigger FLOATER_HIGH → expect HEAT_WATER state
5. Wait for timeout → expect ROTATE_RIGHT state
... (continue through full wash cycle)

Test error conditions:
- BUTTON_STOP during operation → expect SHUT_DOWN
- Door opened during wash → expect appropriate response
- Multiple simultaneous events → verify priority
```

**3. Hardware Testing (with mockup):**
```
Using CT Board:
- Map buttons to events (SW0=Door, SW1=Wash, etc.)
- Map LEDs to outputs (LED0=Heater, LED1=Valve, etc.)
- Run through test scenarios
- Observe LED patterns
- Verify timing using stopwatch
```

**4. System Testing (with real hardware):**
```
Full wash cycle test:
1. Load washing machine
2. Close door
3. Press wash button
4. Observe complete cycle
5. Verify all steps occur in correct order
6. Measure timing of each phase
7. Verify water fills and drains correctly
8. Verify heating works
9. Verify motor rotations
```

**5. Requirements Traceability:**
```
For each requirement from U01:
- Identify corresponding behavior in state machine
- Create test case that verifies requirement
- Execute test
- Document: Requirement ID → Test ID → Result
```

## Comparisons & Contrasts

### Mockup vs. Real Hardware Implementation

| Aspect | Mockup (Development) | Real Hardware (Production) |
|--------|---------------------|---------------------------|
| **Input Source** | CT Board buttons/switches | Washing machine sensors |
| **Output Target** | CT Board LEDs | Washing machine actuators |
| **I/O Addresses** | CT Board GPIO registers | External connector registers |
| **API Functions** | Same signatures | Same signatures |
| **State Machine** | Identical code | Identical code |
| **Testing Environment** | Laboratory/desk | Actual washing machine |
| **Safety Risks** | None (just LEDs) | Water, electricity, moving parts |
| **Development Speed** | Fast (immediate feedback) | Slower (setup required) |
| **Portability** | Easy to transport | Fixed installation |
| **Cost** | Low (existing board) | Higher (actual hardware) |
| **Purpose** | Development, debugging | Production, verification |

**Key Insight:** Only action_handler implementation changes; all other code identical.

### Event-Driven vs. Time-Driven Architecture

| Aspect | Event-Driven (This Lab) | Time-Driven (Alternative) |
|--------|------------------------|--------------------------|
| **Execution Trigger** | Events (button press, sensor, timeout) | Fixed time intervals |
| **Main Loop** | Poll for events, react | Execute at regular intervals |
| **CPU Utilization** | Can sleep between events | Always running at interval rate |
| **Response Time** | Immediate (limited by polling) | Up to one interval delay |
| **Code Structure** | State machine with event handling | Periodic sampling and control |
| **Typical Use** | Interactive systems, reactive | Control loops, real-time systems |
| **This Application** | Ideal (user interactions, sensors) | Possible but less natural |

**Washing Machine Fit:** Event-driven better because:
- User interactions (button presses) are sporadic
- Sensor changes (floater, thermostat) are infrequent
- State changes happen at unpredictable times
- No need for precise periodic control (not a control loop)

### State Machine Implementation Approaches

| Aspect | Nested Switch | State Table | Function Pointers |
|--------|--------------|-------------|------------------|
| **Readability** | Good (clear structure) | Moderate (table format) | Lower (indirection) |
| **Maintainability** | Good (localized changes) | Good (data-driven) | Moderate (scattered functions) |
| **Code Size** | Moderate | Small (data) | Small (code + data) |
| **Execution Speed** | Fast (direct) | Slower (table lookup) | Fast (direct call) |
| **Flexibility** | Limited (compiled) | High (table can be modified) | High (dynamic) |
| **Complexity** | Low | Moderate | Higher |
| **This Lab Choice** | Nested switch | - | - |

**Why Nested Switch for This Lab:**
- Clear mapping from UML diagram
- Easy to understand and debug
- Good performance
- No dynamic behavior needed
- Typical embedded systems approach

### Active-Low vs. Active-High Signaling

| Aspect | Active-Low (This Lab) | Active-High (Alternative) |
|--------|--------------------|------------------------|
| **Logic Convention** | 0 = active/on, 1 = inactive/off | 1 = active/on, 0 = inactive/off |
| **Pull Resistors** | Pull-up (to Vcc) | Pull-down (to GND) |
| **Disconnected State** | High (1) = inactive (safe) | Low (0) = inactive (safe) |
| **Noise Immunity** | Better (positive rail stable) | Moderate |
| **Power Consumption** | Lower (current only when active) | Higher (current when inactive) |
| **Inverter Needed** | Yes (in software/hardware) | No |
| **Code Example** | `if((input & bit) == 0)` active | `if(input & bit)` active |
| **Output Control** | `output &= ~bit` to turn on | `output |= bit` to turn on |
| **Common Usage** | Industrial, automotive, embedded | Consumer electronics, logic circuits |

**Why Active-Low for Washing Machine:**
- Safer default (disconnected = inactive)
- Better noise immunity (important in motor/electrical environment)
- Standard in industrial control systems
- Pull-up resistors provide defined state

### Modular vs. Monolithic Architecture

| Aspect | Modular (This Lab) | Monolithic (Alternative) |
|--------|-------------------|------------------------|
| **Code Organization** | Multiple modules, clear APIs | Single large file/module |
| **Responsibilities** | Separated (timer, event, action, FSM) | Everything together |
| **Testability** | Each module tested independently | Must test entire system |
| **Maintainability** | Changes localized | Changes affect entire codebase |
| **Reusability** | Modules reusable in other projects | Must extract and modify |
| **Complexity** | Distributed (managed per module) | Concentrated (all in one place) |
| **Learning Curve** | Higher (understand module interactions) | Lower (everything visible) |
| **Best Practice** | Industry standard | Avoid for non-trivial systems |

**Why Modular for This Lab:**
- Separation of concerns (clarity)
- Easier testing (unit tests)
- Hardware abstraction (portability)
- Industry best practice
- Scalability (easy to add features)

## Examples & Applications

### Example 1: Complete State Transition Implementation

**Scenario:** Implementing the transition from IDLE to FILL_WATER when user presses WASH button.

**UML Diagram Notation:**
```
IDLE --[BUTTON_WASH / door_lock(); valve_open()]--> FILL_WATER
```

**Code Implementation:**

**1. Event Definition (event_handler.h):**
```c
typedef enum {
    NO_EVENT,
    BUTTON_WASH,
    // other events...
} event_t;
```

**2. State Definition (state_machine.h):**
```c
typedef enum {
    IDLE,
    FILL_WATER,
    // other states...
} state_t;
```

**3. Action Functions (action_handler.h/c):**
```c
// action_handler.h
void door_lock(void);
void valve_open(void);

// action_handler.c
#define OUTPUT_REG ((volatile uint8_t*) 0x60000400)
#define DOOR_LOCK_BIT 7
#define VALVE_BIT 6

void door_lock(void) {
    // Active-low: set bit to 0 to activate
    *OUTPUT_REG &= ~(1 << DOOR_LOCK_BIT);
}

void valve_open(void) {
    *OUTPUT_REG &= ~(1 << VALVE_BIT);
}
```

**4. State Machine Handler (state_machine.c):**
```c
static state_t current_state = IDLE;

void fsm_handle_event(event_t event) {
    switch(current_state) {
        case IDLE:
            if(event == BUTTON_WASH) {
                // Execute actions
                door_lock();
                valve_open();
                // Transition to new state
                current_state = FILL_WATER;
            }
            break;
        // other states...
    }
}
```

**Result:**
- When user presses WASH button, event_handler detects it and returns BUTTON_WASH
- Main loop passes event to fsm_handle_event()
- State machine executes actions (lock door, open valve)
- State machine transitions to FILL_WATER state
- Next events will be processed in context of FILL_WATER state

### Example 2: Timer-Based State Transition

**Scenario:** After filling water (FILL_WATER), heat water for 5 seconds (HEAT_WATER), then start rotating (ROTATE_RIGHT).

**UML Transitions:**
```
FILL_WATER --[FLOATER_HIGH / valve_close(); heater_on(); start_timer(500)]--> HEAT_WATER
HEAT_WATER --[TIME_OUT / heater_off(); motor_on_slow_right(); start_timer(400)]--> ROTATE_RIGHT
```

**Implementation:**

**1. Event Handler - Timer Detection (event_handler.c):**
```c
#include "timer.h"

static uint32_t previous_timer = 0;

event_t eh_get_event(void) {
    uint32_t current_timer = timer_get_value();

    // Check for timer timeout
    if(previous_timer != 0 && current_timer == 0) {
        previous_timer = current_timer;
        return TIME_OUT;
    }

    previous_timer = current_timer;

    // Check other events (sensors, buttons)...

    return NO_EVENT;
}
```

**2. State Machine Implementation:**
```c
void fsm_handle_event(event_t event) {
    switch(current_state) {
        case FILL_WATER:
            if(event == FLOATER_HIGH) {
                // Water level reached
                valve_close();
                heater_on();
                timer_start(500);  // 5 seconds = 500 * 10ms
                current_state = HEAT_WATER;
            }
            break;

        case HEAT_WATER:
            if(event == TIME_OUT) {
                // Heating complete
                heater_off();
                motor_on_slow_right();
                timer_start(400);  // 4 seconds
                current_state = ROTATE_RIGHT;
            }
            break;

        case ROTATE_RIGHT:
            if(event == TIME_OUT) {
                // Rotation complete
                motor_off();
                motor_on_slow_left();
                timer_start(400);
                current_state = ROTATE_LEFT;
            }
            break;
    }
}
```

**Execution Flow:**
1. In FILL_WATER, water rises
2. Floater High sensor triggers → FLOATER_HIGH event
3. Valve closes, heater turns on, timer starts (5s)
4. System stays in HEAT_WATER, timer counts down
5. Timer reaches 0 → TIME_OUT event
6. Heater turns off, motor starts rotating right, new timer starts (4s)
7. System in ROTATE_RIGHT, timer counts down again
8. Process continues...

### Example 3: Emergency Stop Implementation

**Scenario:** User can press STOP button at any time to abort operation and drain water.

**UML:** From any state (except DOOR_OPEN, IDLE) → SHUT_DOWN on BUTTON_STOP

**Implementation Pattern:**
```c
void fsm_handle_event(event_t event) {
    // Global event handler (checked in all states)
    if(event == BUTTON_STOP) {
        // Emergency stop: drain water and go to shutdown
        timer_stop();          // Cancel any running timer
        heater_off();          // Turn off heater immediately
        valve_close();         // Stop filling
        motor_off();           // Stop motor
        pump_on();             // Start draining
        current_state = SHUT_DOWN;
        return;  // Exit immediately
    }

    // Normal state-specific event handling
    switch(current_state) {
        case FILL_WATER:
            if(event == FLOATER_HIGH) {
                // ...
            }
            break;
        case HEAT_WATER:
            // ...
            break;
        // other states...
    }
}
```

**Result:**
- BUTTON_STOP checked first, before state-specific logic
- Works from any state
- Immediately turns off all dangerous actuators (heater, motor)
- Starts pump to drain water
- Transitions to SHUT_DOWN state

### Example 4: Mockup Implementation with CT Board

**Scenario:** Test washing machine logic using CT Board LEDs and buttons.

**Hardware Mapping:**

**Inputs (Buttons/Switches):**
```c
// CT Board Button/Switch → Washing Machine Input
Button 0 (T0) → Door Closed
Button 1 (T1) → Button Wash
Button 2 (T2) → Button Spin
Button 3 (T3) → Button Stop
DIP Switch 0 → Floater High
DIP Switch 1 → Floater Low
DIP Switch 2 → Thermostat Low
```

**Outputs (LEDs):**
```c
// CT Board LED → Washing Machine Output
LED 0 → Door Locked
LED 1 → Valve On
LED 2 → Heater On
LED 3 → Pump On
LED 4 → Motor (any)
LED 5 → Motor Direction (left/right)
LED 6 → Motor Speed (slow/fast)
```

**Mockup Action Handler Implementation:**
```c
// action_handler.c (mockup version)
#include "reg_ctboard.h"

// LED control (active-low like real washing machine)
#define LED_DOOR_LOCK  0
#define LED_VALVE      1
#define LED_HEATER     2
#define LED_PUMP       3
#define LED_MOTOR      4

void door_lock(void) {
    // Turn on LED to indicate door locked
    LED->DATA &= ~(1 << LED_DOOR_LOCK);
}

void door_unlock(void) {
    LED->DATA |= (1 << LED_DOOR_LOCK);
}

void valve_open(void) {
    LED->DATA &= ~(1 << LED_VALVE);
}

void valve_close(void) {
    LED->DATA |= (1 << LED_VALVE);
}

void heater_on(void) {
    LED->DATA &= ~(1 << LED_HEATER);
}

void heater_off(void) {
    LED->DATA |= (1 << LED_HEATER);
}

// Similar for other outputs...
```

**Mockup Event Handler:**
```c
// event_handler.c (mockup version)
#include "reg_ctboard.h"

event_t eh_get_event(void) {
    static uint8_t prev_buttons = 0;
    static uint8_t prev_switches = 0;

    uint8_t curr_buttons = BUTTON->PRESSED;
    uint8_t curr_switches = DIP_SWITCH->DATA;

    // Check button presses (falling edge on buttons)
    if((prev_buttons & BUTTON_0) && !(curr_buttons & BUTTON_0)) {
        prev_buttons = curr_buttons;
        return DOOR_CLOSED;
    }
    if((prev_buttons & BUTTON_1) && !(curr_buttons & BUTTON_1)) {
        prev_buttons = curr_buttons;
        return BUTTON_WASH;
    }

    // Check DIP switch changes (level-sensitive for sensors)
    if((curr_switches & DIP_0) && !(prev_switches & DIP_0)) {
        prev_switches = curr_switches;
        return FLOATER_HIGH;
    }

    prev_buttons = curr_buttons;
    prev_switches = curr_switches;

    // Check timer...

    return NO_EVENT;
}
```

**Testing Procedure:**
```
1. Power on CT Board → LED0 (Door) should be OFF (unlocked)
2. Press Button 0 (Door Close) → No visible change (now in IDLE)
3. Press Button 1 (Wash) → LED0 ON (locked), LED1 ON (valve)
4. Flip DIP 0 (Floater High) → LED1 OFF (valve), LED2 ON (heater)
5. Wait 5 seconds → LED2 OFF (heater), LED4 ON (motor)
6. Wait 4 seconds → LED4 blinks (direction change)
7. Press Button 3 (Stop) → LED3 ON (pump), all others OFF
8. Flip DIP 1 (Floater Low) → LED3 OFF (pump), LED0 OFF (unlock)
```

### Example 5: Bit Manipulation for Active-Low Control

**Scenario:** Control washing machine motor with multiple signals (direction, speed).

**Hardware Configuration:**
```
Motor Control (4 bits):
- Bit 2: Fast (1200 Hz clock)
- Bit 3: Slow (100 Hz clock)
- Direction: Left/Right (H-bridge control)
```

**Motor States:**
- **Off**: Both Fast and Slow = 1 (inactive)
- **Slow Right**: Slow = 0, Fast = 1, Direction = Right
- **Slow Left**: Slow = 0, Fast = 1, Direction = Left
- **Fast Right**: Slow = 1, Fast = 0, Direction = Right
- **Fast Left**: Slow = 1, Fast = 0, Direction = Left

**Implementation:**
```c
#define OUTPUT_REG ((volatile uint8_t*) 0x60000400)

// Bit definitions
#define MOTOR_FAST_BIT   2
#define MOTOR_SLOW_BIT   3
#define MOTOR_DIR_BIT    // Direction control bits...

// Helper macros
#define MOTOR_MASK  ((1 << MOTOR_FAST_BIT) | (1 << MOTOR_SLOW_BIT))

void motor_off(void) {
    // Set both speed bits to 1 (inactive)
    *OUTPUT_REG |= MOTOR_MASK;
}

void motor_on_slow_right(void) {
    uint8_t output = *OUTPUT_REG;

    // Clear slow bit (activate slow), set fast bit (deactivate fast)
    output &= ~(1 << MOTOR_SLOW_BIT);  // Slow = 0 (active)
    output |= (1 << MOTOR_FAST_BIT);   // Fast = 1 (inactive)

    // Set direction (implementation depends on H-bridge)
    // ... direction control bits ...

    *OUTPUT_REG = output;
}

void motor_on_fast_left(void) {
    uint8_t output = *OUTPUT_REG;

    // Clear fast bit, set slow bit
    output &= ~(1 << MOTOR_FAST_BIT);  // Fast = 0 (active)
    output |= (1 << MOTOR_SLOW_BIT);   // Slow = 1 (inactive)

    // Set direction for left
    // ...

    *OUTPUT_REG = output;
}
```

**Key Points:**
- Can't activate both Fast and Slow simultaneously
- Must explicitly deactivate one when activating the other
- Active-low: Clear bit (set to 0) to activate
- Preserve other bits in register when changing motor control

### Example 6: Requirement to Implementation Traceability

**Scenario:** Tracing from requirement through model to code.

**Requirement (from U01):**
```
REQ-WM-005: The washing machine shall heat water until temperature sensor
indicates hot water, then start wash cycle.

Priority: MUST
Testable: Yes - monitor temperature sensor and verify motor starts after heating
```

**UML Model (from U01):**
```
HEAT_WATER --[TEMPERATURE_HOT / heater_off(); motor_on_slow_right(); start_timer(400)]--> ROTATE_RIGHT
```

**Code Implementation (U2):**
```c
// event_handler.h
typedef enum {
    // ...
    TEMPERATURE_HOT,  // Generated when thermostat changes from LOW to HIGH
    // ...
} event_t;

// state_machine.c
void fsm_handle_event(event_t event) {
    switch(current_state) {
        // ...
        case HEAT_WATER:
            if(event == TEMPERATURE_HOT) {
                // REQ-WM-005: Stop heating when hot
                heater_off();
                // REQ-WM-005: Start wash cycle (rotation)
                motor_on_slow_right();
                timer_start(400);  // 4 seconds rotation
                current_state = ROTATE_RIGHT;
            }
            else if(event == BUTTON_STOP) {
                // Emergency stop...
            }
            break;
        // ...
    }
}

// action_handler.c
void heater_off(void) {
    // Turn off heater (set bit to 1 for active-low)
    *OUTPUT_REG |= (1 << HEATER_BIT);
}

void motor_on_slow_right(void) {
    // Activate slow speed (bit to 0) and set right direction
    *OUTPUT_REG &= ~(1 << MOTOR_SLOW_BIT);
    *OUTPUT_REG |= (1 << MOTOR_FAST_BIT);
    // Direction control...
}
```

**Test Case:**
```
TEST-WM-005: Heating to Rotation Transition
Preconditions:
- System in HEAT_WATER state
- Heater is ON
- Water temperature is LOW

Steps:
1. Simulate TEMPERATURE_HOT event (flip thermostat sensor)
2. Observe heater turns OFF (LED off in mockup, actual heater off in production)
3. Observe motor turns ON at slow speed (LED on in mockup, actual motor in production)
4. Observe motor direction is RIGHT
5. Verify timer started (4 seconds)
6. After 4 seconds, verify transition to next state

Expected Results:
- Heater OFF within 10ms of temperature sensor change
- Motor starts within 10ms
- State transitions to ROTATE_RIGHT
- Timer active for 4 seconds

Result: PASS / FAIL
Tested by: [Name]
Date: [Date]
```

**Traceability Matrix:**
```
REQ-WM-005 →
  UML State: HEAT_WATER →
    Event: TEMPERATURE_HOT →
      Code: fsm_handle_event(), case HEAT_WATER, if(event == TEMPERATURE_HOT) →
        Actions: heater_off(), motor_on_slow_right() →
          Test: TEST-WM-005
```

## Connections to Other Topics

### Connection to Lecture 03: Requirements Engineering

**Requirements Implementation:**
- U01 defined requirements for washing machine
- U2 implements those requirements in code
- Each requirement traced to specific code sections
- Testability evaluated during implementation

**RFC 2119 Keywords in Code Comments:**
```c
// The system SHALL lock the door before filling water (REQ-WM-002)
if(event == BUTTON_WASH) {
    door_lock();  // MUST happen before valve opens
    valve_open();
    current_state = FILL_WATER;
}
```

### Connection to Lecture 04: Modelling

**UML State Machine to Code:**
- Direct translation from UML diagram to nested switch structure
- States → enum values and switch cases
- Events → enum values
- Transitions → if statements within cases
- Actions → function calls

**Model-Driven Development:**
- Model created first (U01)
- Code implements model (U2)
- Model and code must stay synchronized
- Changes to requirements → update model → update code

### Connection to Lecture 05: Development Processes

**V-Model Application:**
- **Requirements Phase (U01)**: Define what system should do
- **Design Phase (U2 planning)**: Define modular architecture, APIs
- **Implementation Phase (U2 coding)**: Write code
- **Unit Test (U2)**: Test individual modules
- **Integration Test (U3)**: Test with real hardware
- **System Test**: Verify all requirements met

**Iterative Development:**
- Start with mockup (U2)
- Test with CT Board (U2)
- Move to real hardware (U3)
- Each iteration adds complexity

### Connection to Lecture 06: Non-Functional Requirements

**NFRs in This Lab:**

**Execution NFRs:**
- **Performance**: Response time to button press < 100ms (event detection in main loop)
- **Reliability**: System must not get stuck (all states have exit paths)
- **Safety**: Emergency stop (BUTTON_STOP) works from all states

**Evolution NFRs:**
- **Maintainability**: Modular structure enables easy changes
- **Testability**: Mockup functions enable testing without hardware
- **Portability**: Hardware abstraction layer (action_handler) makes code portable

**Example NFR Implementation:**
```c
// NFR: Response time < 100ms
// Implementation: Main loop polls frequently
while(1) {
    event_t event = eh_get_event();  // Fast polling (< 1ms per iteration)
    if(event != NO_EVENT) {
        fsm_handle_event(event);
    }
}

// NFR: Safety - Emergency stop must work immediately
if(event == BUTTON_STOP) {
    // Checked first, before any other event processing
    // Ensures immediate response regardless of state
    // ...
}
```

### Connection to Lab P1: Hardware Bring-up

**CT Board Programming:**
- Same board used in P1 for basic GPIO
- P1: Learned basic hardware access (buttons, LEDs, memory-mapped I/O)
- U2: Apply that knowledge to complex state machine application

**Memory-Mapped I/O:**
- P1: Introduced concept, simple examples
- U2: More complex usage with bit manipulation for multiple outputs
- Same principles, more sophisticated application

### Connection to Future Topics

**Lecture 08: Energy (Future):**
- This lab's implementation affects energy consumption
- Frequent polling in main loop consumes power
- Could optimize with interrupts (wait for events instead of polling)
- Timer implementation affects power (constant countdown vs. sleep)

**Lecture 09: Performance (Future):**
- Event processing time matters for responsiveness
- Nested switch performance vs. alternatives
- Timer resolution affects timing accuracy (100Hz = 10ms granularity)

**Lecture 13: RTOS Scheduling (Future):**
- Current implementation: bare-metal, superloop architecture
- Alternative: RTOS with tasks/threads
- State machine could be one task, event detection another
- Trade-offs: complexity vs. responsiveness

## Summary

**Key Achievements in U2:**
1. Translated UML state machine model to working C code
2. Implemented modular software architecture (timer, event_handler, action_handler, state_machine)
3. Created hardware abstraction layer (API) for all I/O
4. Developed mockup functions for testing without real hardware
5. Verified state machine behavior using CT Board LEDs and buttons

**Core Concepts Demonstrated:**
- **Model-driven development**: From UML diagram to code
- **Modular architecture**: Separation of concerns, clear interfaces
- **Hardware abstraction**: Portable code through API layer
- **Event-driven programming**: React to asynchronous events
- **State machine implementation**: Nested switch pattern
- **Memory-mapped I/O**: Direct hardware control
- **Active-low logic**: Industry-standard signal convention

**Development Strategy:**
- Build incrementally: Start with basic structure, add features
- Test frequently: Use mockup for continuous verification
- Maintain traceability: Link requirements → model → code → tests

**Preparation for U3:**
- Mockup implementation complete and tested
- Same API will be used with real hardware
- Only action_handler implementation changes
- State machine code remains identical

## Possible Exam Questions

### Basic Understanding

1. **What is the purpose of hardware abstraction in embedded systems? Give an example from the washing machine lab.**
   - Expected: Separates application logic from hardware details
   - Example: action_handler provides API (valve_open()) instead of direct register access
   - Benefits: Portability, testability, maintainability

2. **Explain the difference between mockup and real hardware implementations. What stays the same and what changes?**
   - Expected: Same API, state machine, event handler
   - Changes: Only action_handler implementation (LEDs vs. real actuators)
   - Purpose: Enable development without physical hardware

3. **What does "active-low" mean? Why is it commonly used in embedded systems?**
   - Expected: 0 = active/on, 1 = inactive/off
   - Reasons: Safety (disconnected = inactive), noise immunity, standard in industrial systems

4. **Describe the four main software modules in the washing machine implementation and their responsibilities.**
   - Expected:
     - timer: Time management, countdown, timeout events
     - event_handler: Input monitoring, event generation, edge detection
     - action_handler: Output control, hardware interface
     - state_machine: Application logic, state transitions, action coordination

5. **What is memory-mapped I/O? How is it used to control the washing machine?**
   - Expected: Hardware registers at memory addresses
   - Read from INPUT_REG to get sensor states
   - Write to OUTPUT_REG to control actuators
   - Example addresses: 0x60000410 (input), 0x60000400 (output)

6. **Explain the nested switch pattern for state machine implementation.**
   - Expected: Outer switch on current_state, inner switch on event
   - Each case represents one transition from UML diagram
   - Clear structure, easy to understand and maintain

7. **How is edge detection implemented in the event handler?**
   - Expected: Store previous input state, compare with current
   - Falling edge: previous=1 AND current=0
   - Rising edge: previous=0 AND current=1
   - Generate appropriate event (e.g., DOOR_CLOSED for falling edge on Door Closed input)

8. **What is the purpose of the timer module and how does it generate timeout events?**
   - Expected: Implements timed behavior (delays between states)
   - Counts down from specified duration (100Hz rate)
   - Event handler detects transition from non-zero to zero
   - Generates TIME_OUT event

### Application & Analysis

9. **Given the following UML transition, write the corresponding code:**
   ```
   ROTATE_RIGHT --[TIME_OUT / motor_off(); pump_on()]--> EMPTY_WATER
   ```
   - Expected:
   ```c
   case ROTATE_RIGHT:
       if(event == TIME_OUT) {
           motor_off();
           pump_on();
           current_state = EMPTY_WATER;
       }
       break;
   ```

10. **Write the code to control the heater output (bit 5 of OUTPUT_REG at 0x60000400). Remember it's active-low. Include both heater_on() and heater_off().**
    - Expected:
    ```c
    #define OUTPUT_REG ((volatile uint8_t*) 0x60000400)
    #define HEATER_BIT 5

    void heater_on(void) {
        *OUTPUT_REG &= ~(1 << HEATER_BIT);  // Clear bit (0 = on)
    }

    void heater_off(void) {
        *OUTPUT_REG |= (1 << HEATER_BIT);  // Set bit (1 = off)
    }
    ```

11. **How would you test whether the washing machine correctly locks the door before filling water? Describe your test procedure using the mockup.**
    - Expected procedure:
      1. Start system (should be in IDLE state)
      2. Press BUTTON_WASH (simulate wash button)
      3. Observe LED for door lock turns ON
      4. Observe LED for valve turns ON after door lock
      5. Verify order: door lock first, then valve
      6. Verify state machine in FILL_WATER state

12. **The washing machine should respond to the STOP button within 100ms. Explain how your implementation achieves this and what factors affect response time.**
    - Expected:
      - Main loop polls continuously (< 1ms per iteration)
      - Event handler checks for STOP button first
      - State machine checks STOP event before state-specific events
      - Response time = polling period + event processing time
      - Factors: Polling frequency, event processing complexity, interrupt latency

13. **Calculate how many timer ticks are needed for a 7.5 second delay, given the timer runs at 100Hz.**
    - Expected calculation:
      - Duration (seconds) = Count / Frequency
      - Count = Duration × Frequency = 7.5 × 100 = 750
      - Answer: timer_start(750)

14. **Given this bit manipulation operation: `*OUTPUT_REG &= ~0x60`, what outputs are affected and what is their new state? Assume active-low.**
    - Expected:
      - 0x60 = 0b01100000 = bits 5 and 6
      - `&= ~0x60` clears bits 5 and 6 (sets them to 0)
      - Active-low: 0 = ON
      - Result: Outputs 5 and 6 are turned ON (e.g., heater and valve)

### Design & Implementation

15. **Design the API for the action_handler module. List all function prototypes needed to control the washing machine.**
    - Expected functions:
    ```c
    // Door control
    void door_lock(void);
    void door_unlock(void);

    // Valve control
    void valve_open(void);
    void valve_close(void);

    // Heater control
    void heater_on(void);
    void heater_off(void);

    // Pump control
    void pump_on(void);
    void pump_off(void);

    // Motor control
    void motor_off(void);
    void motor_on_slow_right(void);
    void motor_on_slow_left(void);
    void motor_on_fast_right(void);
    void motor_on_fast_left(void);
    ```

16. **Implement the main event loop for the washing machine controller. Include initialization and continuous event processing.**
    - Expected:
    ```c
    int main(void) {
        // Initialize hardware
        hal_init();

        // Initialize application modules
        fsm_init();
        timer_init();

        // Main event loop
        while(1) {
            event_t event = eh_get_event();
            if(event != NO_EVENT) {
                fsm_handle_event(event);
            }
        }

        return 0;  // Never reached
    }
    ```

17. **How would you modify the state machine to add a "pause" feature that allows temporarily stopping the wash cycle and resuming later?**
    - Expected approach:
      - Add new state: PAUSED
      - Add new event: BUTTON_PAUSE
      - In most states, handle BUTTON_PAUSE event → save current state, transition to PAUSED
      - In PAUSED state, handle BUTTON_PAUSE event → restore saved state, resume
      - Save any active timer values
      - Turn off actuators during pause

18. **Explain how you would implement a safety timeout that aborts the wash cycle if it takes longer than 30 minutes.**
    - Expected:
      - Add global safety timer: timer_start_safety(180000) at start of cycle  // 30 min
      - Check for SAFETY_TIMEOUT event in all states (similar to BUTTON_STOP)
      - On SAFETY_TIMEOUT: Stop all actuators, transition to SHUT_DOWN
      - Reset safety timer when cycle completes normally

### Testing & Verification

19. **Describe a systematic testing procedure to verify all state transitions in the washing machine.**
    - Expected approach:
      1. Create test plan listing all states and transitions
      2. For each transition:
         - Set system to source state
         - Generate triggering event
         - Verify actions executed (check outputs)
         - Verify transition to target state
      3. Test with mockup first (automated testing possible)
      4. Repeat tests with real hardware
      5. Document results: Test ID, Input, Expected Output, Actual Output, Pass/Fail

20. **How can you guarantee that the washing machine implementation meets all requirements from U01?**
    - Expected:
      - **Traceability**: Link each requirement to specific code sections
      - **Test cases**: Create test for each requirement
      - **Code review**: Verify implementation matches design
      - **Integration testing**: Test complete system behavior
      - **Formal verification**: Use model checking (if applicable)
      - Note: "Guarantee" is strong—can only have high confidence, not absolute certainty

21. **What are the advantages of using mockup functions during development? What are the limitations?**
    - Advantages:
      - Test without physical hardware
      - Fast iteration (immediate feedback)
      - Safe (no risk of damaging hardware)
      - Reproducible (same inputs → same outputs)
      - Can automate testing
    - Limitations:
      - Not exact replica of real hardware (timing, loading)
      - Can't test real-world issues (noise, mechanical failures)
      - May not catch hardware-specific bugs
      - Different electrical characteristics

### Critical Thinking

22. **Compare event-driven architecture (used in this lab) with time-driven architecture. Which is more suitable for the washing machine application and why?**
    - Expected:
      - Event-driven: React to sporadic events (buttons, sensors)
      - Time-driven: Execute at regular intervals (control loops)
      - Washing machine better suited to event-driven because:
        - User interactions are asynchronous
        - Sensor changes are unpredictable
        - No need for precise periodic control
        - Can reduce power consumption (not constantly polling)

23. **The current implementation uses a polling loop to detect events. Discuss the advantages and disadvantages compared to an interrupt-driven approach.**
    - Polling Advantages:
      - Simpler code (no interrupt handlers)
      - Easier to debug (sequential execution)
      - No interrupt priority issues
    - Polling Disadvantages:
      - CPU always busy (higher power consumption)
      - Response time limited by poll rate
      - Can miss very short events
    - Interrupt Advantages:
      - Lower power (CPU can sleep)
      - Faster response (immediate)
      - Better for infrequent events
    - Interrupt Disadvantages:
      - More complex code
      - Race conditions possible
      - Interrupt priority management needed

24. **Discuss the trade-offs between the nested switch pattern and other state machine implementation approaches (state table, function pointers).**
    - Expected comparison:
      - Nested switch: Clear, fast, but less flexible
      - State table: Compact, data-driven, but slower (lookup overhead)
      - Function pointers: Flexible, but harder to understand
      - For this application: Nested switch appropriate (simple, clear, fast)

25. **How does modular architecture improve the maintainability of the washing machine code? Give specific examples.**
    - Expected:
      - **Localized changes**: Change timer implementation without touching state machine
      - **Independent testing**: Test each module separately
      - **Clear responsibilities**: Know where to look for specific functionality
      - **Reusability**: Can reuse timer module in other projects
      - Example: If hardware changes, only action_handler needs modification

### Connections

26. **How does this lab connect to the V-Model development process from Lecture 05?**
    - Expected:
      - U01: Requirements (left side of V)
      - U02: Design and Implementation (bottom of V)
      - U03: Integration Testing (right side of V)
      - Each level has corresponding verification
      - Traceability throughout

27. **Relate the state machine implementation to the UML modeling from Lecture 04.**
    - Expected:
      - Direct translation from UML to code
      - States → enum, Events → enum, Transitions → if statements
      - Actions in code match actions in UML
      - Model-driven development: Model first, then code

28. **How do the mockup functions demonstrate the principle of hardware abstraction discussed in earlier lectures?**
    - Expected:
      - API separates "what" from "how"
      - State machine doesn't know about hardware details
      - Can swap implementations without changing application logic
      - Portability and testability benefits

29. **This lab uses active-low signaling. How does this relate to the hardware interfaces covered in Lab P1?**
    - Expected:
      - P1: Introduced GPIO and basic I/O
      - U2: Applies same principles to complex application
      - Active-low common in embedded systems (learned in P1, applied in U2)
      - Bit manipulation techniques from P1 used extensively

30. **How might the NFRs from Lecture 06 apply to the washing machine implementation?**
    - Expected NFRs:
      - Performance: Response time < 100ms
      - Reliability: MTBF > X years, no stuck states
      - Safety: Emergency stop must always work
      - Maintainability: Modular code, clear structure
      - Testability: Mockup enables testing
