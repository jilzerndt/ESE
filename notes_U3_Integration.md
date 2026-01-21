# Lab U3: Test & Integration

## Technical Terms & Definitions

### Integration Concepts

**System Integration**
- Process of combining individual modules into a complete system
- Verifying that modules work together correctly
- Transitioning from mockup/simulation to real hardware
- Testing complete system functionality end-to-end

**Hardware-in-the-Loop (HIL)**
- Testing approach where software runs with actual hardware
- More realistic than simulation/mockup
- Detects hardware-specific issues
- Essential step before deployment

**Integration Testing**
- Testing phase that verifies module interactions
- Follows unit testing (individual modules)
- Precedes system testing (complete functionality)
- Part of V-Model right side (verification)

**API Stability**
- API (interface) remains unchanged during integration
- Only implementation changes (from mockup to real hardware)
- Demonstrates value of abstraction
- Enables smooth transition

### Hardware Integration

**External Connector**
- Physical interface between CT Board and washing machine hardware
- 16-pin connector with defined pinout
- Accessed via memory-mapped registers
- CT Board: 0x60000400 (output), 0x60000410 (input)

**Real Hardware Interface**
- Physical washing machine components (valves, motors, sensors, etc.)
- Electrical signals match specification (voltage levels, timing)
- Active-low convention maintained
- More complex than mockup (electrical loading, noise, mechanical delays)

**Signal Conditioning**
- Preparing electrical signals for proper interface
- May include: Level shifters, filters, debouncing
- Protection circuits (overvoltage, overcurrent)
- In this lab: Handled by washing machine supplier hardware

**Electrical Loading**
- Real actuators draw current (LEDs in mockup don't)
- May affect signal levels and timing
- Need proper drive capability
- Protection circuits essential

### Testing Concepts

**Regression Testing**
- Re-running tests after changes to ensure nothing broke
- Important after switching from mockup to real hardware
- Verifies that previously working functionality still works
- Should be automated where possible

**End-to-End Testing**
- Testing complete workflow from start to finish
- Example: Full wash cycle from button press to completion
- Verifies system-level behavior
- Most realistic testing approach

**Boundary Conditions**
- Testing at limits of system capability
- Examples: Empty/full water levels, temperature extremes
- Often where bugs are found
- Critical for safety

**Error Injection**
- Deliberately introducing faults to test error handling
- Examples: Disconnect sensor, simulate timeout, force STOP
- Verifies robustness
- Important for safety-critical systems

### Hardware-Specific Issues

**Bounce (Contact Bounce)**
- Mechanical switches don't transition cleanly
- Multiple rapid on/off transitions when pressed/released
- Can be detected as multiple events
- Solution: Debouncing (hardware or software)

**Noise Immunity**
- Electrical noise can cause false triggers
- Active-low with pull-ups helps
- May need filtering
- More problematic with real hardware than mockup

**Timing Variations**
- Real actuators have mechanical delays
- Sensor response not instantaneous
- Need to account for in design
- Example: Door lock takes time to engage

**Power Supply Issues**
- Motors and actuators draw significant current
- Can cause voltage drops
- May affect microcontroller operation
- Need adequate power supply capacity

## Core Concepts

### Integration Process

**Development Progression:**
1. **Modeling (U01)**: Design behavior with UML state machines
2. **Implementation (U02)**: Write code with mockup
3. **Verification (U02)**: Test with CT Board LEDs/buttons
4. **Integration (U03)**: Connect to real hardware
5. **System Testing (U03)**: Verify complete functionality
6. **Deployment**: Production use

**Key Principle:** Each step builds on previous, catches issues early

### API as Integration Enabler

**Abstraction Benefit:**

```
Application Layer (State Machine)
         ↓
    API Boundary  ← INTERFACE STAYS SAME
         ↓
Implementation Layer
    ├─ U2: Mockup (CT Board LEDs)
    └─ U3: Real Hardware (Washing Machine)
```

**What Changes:**
- Internal implementation of action_handler functions
- Memory addresses or hardware access methods
- Timing considerations (real hardware slower)

**What Doesn't Change:**
- Function signatures (API)
- State machine logic
- Event handler (mostly - may need debouncing)
- Timer module
- Main program structure

### From Mockup to Real Hardware

**Transition Steps:**

**1. Verify Mockup Working:**
- All tests pass with CT Board
- State machine behaves correctly
- Timing is appropriate
- No stuck states

**2. Update Implementation:**
- Modify action_handler functions
- Change memory addresses if needed
- Add hardware-specific initialization
- May add debouncing logic

**3. Incremental Testing:**
- Test one output at a time
- Verify each input separately
- Combine gradually
- Fix issues as they arise

**4. Integration Testing:**
- Run complete sequences
- Verify all transitions
- Test edge cases
- Measure timing

**5. System Validation:**
- Full wash cycle test
- Verify all requirements met
- Safety testing
- Long-duration testing

### Debugging with Real Hardware

**Challenges:**
- Can't easily observe internal state (no LEDs showing everything)
- Real-time constraints (can't pause)
- Safety concerns (water, electricity, moving parts)
- Harder to reproduce issues

**Strategies:**
- **Logging**: Print debug information to UART/display
- **State Indication**: Use spare LEDs to show current state
- **Slow Motion**: Run at reduced speed for observation
- **Breakpoints**: Use debugger carefully (may affect timing)
- **Safety First**: Have emergency stop readily accessible

### Hardware-Specific Considerations

**Timing Differences:**

**Mockup (U2):**
- LED turns on/off instantly (< 1µs)
- Button press detected immediately
- No mechanical delays
- Perfectly consistent timing

**Real Hardware (U3):**
- Valve takes ~100ms to fully open
- Motor takes ~200ms to reach speed
- Door lock mechanism needs ~150ms
- Sensor readings may be noisy
- Need to account for delays in state machine

**Example Impact:**
```c
// May work in mockup but fail with real hardware:
valve_open();
// Immediately check water level - PROBLEM!
// In mockup, everything is instant
// In real hardware, water takes time to flow

// Better approach:
valve_open();
timer_start(100);  // Wait 1 second for water to start flowing
// Then check floater in next state
```

### Safety Considerations

**Safety-Critical Aspects:**
- **Door Lock**: Must be engaged before water/heat
- **Heater**: Must not operate without water (burnout)
- **Motor**: Must stop if door opens
- **Emergency Stop**: Must work from any state
- **Timeout**: Long cycles should have safety timeout

**Implementation:**
```c
// Safety interlock: Check door before heater
if(door_is_locked() && water_level_ok()) {
    heater_on();
} else {
    // Safety violation - shut down
    heater_off();
    valve_close();
    current_state = SHUT_DOWN;
}

// Emergency stop check in all states
if(event == BUTTON_STOP || door_opened_during_cycle()) {
    emergency_shutdown();
    current_state = SHUT_DOWN;
}
```

### Testing Strategy

**Incremental Approach:**

**Phase 1: Individual Output Testing**
```
Test: door_lock()
  1. Call function
  2. Measure output signal (oscilloscope)
  3. Verify correct bit goes low (active-low)
  4. Observe physical door lock engages
  5. Measure lock time (~150ms)

Test: valve_open()
  1. Call function
  2. Verify correct bit goes low
  3. Hear/observe valve solenoid click
  4. Observe water begins to flow (if plumbed)
  5. Measure response time

... (repeat for all outputs)
```

**Phase 2: Individual Input Testing**
```
Test: DOOR_CLOSED detection
  1. Close door manually
  2. Verify event generated
  3. Check debouncing works
  4. Verify falling edge only (not level)
  5. Test rapid open/close (bounce)

Test: FLOATER_HIGH detection
  1. Simulate high water level
  2. Verify event generated
  3. Check timing
  4. Verify no false triggers

... (repeat for all inputs)
```

**Phase 3: State Transition Testing**
```
Test: IDLE → FILL_WATER transition
  1. Set state to IDLE
  2. Trigger BUTTON_WASH event
  3. Verify door_lock() called
  4. Verify valve_open() called
  5. Verify state changes to FILL_WATER
  6. Observe physical hardware responds
  7. Measure response time

... (repeat for all transitions)
```

**Phase 4: Sequence Testing**
```
Test: Complete wash cycle
  1. Start from power-on
  2. Close door
  3. Press WASH button
  4. Let cycle run to completion
  5. Verify each state transition
  6. Measure timing of each phase
  7. Verify door unlocks at end

Test: Error conditions
  - Door opens during wash
  - STOP button during operation
  - Sensor failures
  - Timeout scenarios
```

### Common Integration Issues

**Issue 1: Bounce on Button Inputs**
- **Symptom**: Multiple events from single button press
- **Cause**: Mechanical switch bounce (multiple transitions)
- **Solution**: Software debouncing (wait for stable signal)

```c
// Debouncing algorithm
#define DEBOUNCE_TIME 5  // 50ms at 100Hz

static uint8_t stable_state = 0xFF;
static uint8_t debounce_counter = 0;

uint8_t get_debounced_input(void) {
    uint8_t raw_input = *INPUT_REG;

    if(raw_input == stable_state) {
        debounce_counter = 0;  // Reset counter
    } else {
        debounce_counter++;
        if(debounce_counter >= DEBOUNCE_TIME) {
            // Input has been stable for debounce period
            stable_state = raw_input;
            debounce_counter = 0;
            return raw_input;  // New stable state
        }
    }

    return stable_state;  // Return last stable state
}
```

**Issue 2: Timing Mismatches**
- **Symptom**: State transitions happen too fast for hardware
- **Cause**: Mockup has zero delays, real hardware has mechanical delays
- **Solution**: Add appropriate delays, use sensor feedback

```c
// Original (works in mockup):
case FILL_WATER:
    if(event == FLOATER_HIGH) {
        valve_close();
        heater_on();  // Start immediately
        current_state = HEAT_WATER;
    }
    break;

// Fixed (works with real hardware):
case FILL_WATER:
    if(event == FLOATER_HIGH) {
        valve_close();
        timer_start(20);  // Wait 200ms for valve to fully close
        current_state = FILL_WATER_WAIT;
    }
    break;

case FILL_WATER_WAIT:
    if(event == TIME_OUT) {
        heater_on();  // Now safe to turn on heater
        current_state = HEAT_WATER;
    }
    break;
```

**Issue 3: Electrical Noise**
- **Symptom**: Spurious events, false triggers
- **Cause**: Motor switching, relay chatter
- **Solution**: Filter inputs, require multiple consecutive readings

```c
// Noise filtering: Require N consecutive identical readings
#define FILTER_COUNT 3

static uint8_t filter_buffer[FILTER_COUNT];
static uint8_t filter_index = 0;

uint8_t get_filtered_input(void) {
    filter_buffer[filter_index] = *INPUT_REG;
    filter_index = (filter_index + 1) % FILTER_COUNT;

    // Check if all readings are identical
    uint8_t filtered = filter_buffer[0];
    for(int i = 1; i < FILTER_COUNT; i++) {
        if(filter_buffer[i] != filtered) {
            return previous_value;  // Not stable yet
        }
    }

    return filtered;  // All agree
}
```

**Issue 4: Power Supply Droops**
- **Symptom**: Microcontroller resets when motor starts
- **Cause**: Motor inrush current causes voltage drop
- **Solution**: Adequate power supply, decoupling capacitors, soft-start

**Issue 5: Sensor Hysteresis**
- **Symptom**: Floater bounces around threshold
- **Cause**: Water movement, sensor sensitivity
- **Solution**: Implement hysteresis, ignore rapid changes

## Important Diagrams & Graphics

### 1. Integration Process Flow

**Description:**
Flow diagram showing progression from mockup to real hardware.

```
┌─────────────────────┐
│  Requirements (U01) │
└──────────┬──────────┘
           │
           ↓
┌─────────────────────┐
│  UML Model (U01)    │
└──────────┬──────────┘
           │
           ↓
┌─────────────────────┐
│ Implementation (U02)│
│  - State Machine    │
│  - Event Handler    │
│  - Action Handler   │
│    (Mockup)         │
└──────────┬──────────┘
           │
           ↓
┌─────────────────────┐
│  Testing (U02)      │
│  - CT Board LEDs    │
│  - Buttons/Switches │
│  - Functional Verify│
└──────────┬──────────┘
           │
           ↓
┌─────────────────────┐
│ Integration (U03)   │
│  - Update Action    │
│    Handler          │
│  - Connect Real HW  │
│  - Incremental Test │
└──────────┬──────────┘
           │
           ↓
┌─────────────────────┐
│ System Test (U03)   │
│  - Full Cycle       │
│  - Error Conditions │
│  - Safety Tests     │
└──────────┬──────────┘
           │
           ↓
┌─────────────────────┐
│    Deployment       │
└─────────────────────┘
```

**How to draw:**
1. Draw vertical sequence of boxes
2. Connect with downward arrows
3. Label each phase
4. Note what happens in each phase
5. Highlight U02 vs. U03 phases

**Meaning:**
- Shows complete development lifecycle
- U02 and U03 are part of larger process
- Each phase builds on previous
- Testing increases in realism

### 2. API Boundary During Integration

**Description:**
Diagram showing what changes and what stays the same during integration.

```
┌─────────────────────────────────────────┐
│         UNCHANGED IN U3                 │
│  ┌───────────────────────────────────┐  │
│  │      State Machine Logic          │  │
│  │   (fsm_handle_event, states)      │  │
│  └──────────────┬────────────────────┘  │
│                 │                        │
│  ┌──────────────▼────────────────────┐  │
│  │       Event Handler               │  │
│  │  (eh_get_event, edge detection)   │  │
│  └──────────────┬────────────────────┘  │
│                 │                        │
│  ┌──────────────▼────────────────────┐  │
│  │          Timer Module             │  │
│  │   (timer_start, timer_stop)       │  │
│  └───────────────────────────────────┘  │
└─────────────────────────────────────────┘

═══════════════════════════════════════════ ← API BOUNDARY

┌─────────────────────────────────────────┐
│         CHANGES IN U3                   │
│  ┌───────────────────────────────────┐  │
│  │      Action Handler Impl.         │  │
│  │  ┌─────────────────────────────┐  │  │
│  │  │ U2: CT Board LEDs/Buttons   │  │  │
│  │  │   - LED control             │  │  │
│  │  │   - Button reading          │  │  │
│  │  │   - Mockup implementation   │  │  │
│  │  └─────────────────────────────┘  │  │
│  │                                   │  │
│  │  ┌─────────────────────────────┐  │  │
│  │  │ U3: Real Washing Machine    │  │  │
│  │  │   - External connector      │  │  │
│  │  │   - Real actuators          │  │  │
│  │  │   - Real sensors            │  │  │
│  │  │   - Debouncing, delays      │  │  │
│  │  └─────────────────────────────┘  │  │
│  └───────────────────────────────────┘  │
└─────────────────────────────────────────┘
```

**How to draw:**
1. Two large boxes: "UNCHANGED" and "CHANGES"
2. Horizontal line between them labeled "API BOUNDARY"
3. In UNCHANGED: List modules that don't change
4. In CHANGES: Show two alternative implementations
5. Use arrows to show calls across API

**Meaning:**
- Clear visualization of abstraction benefit
- Majority of code doesn't change
- Only hardware interface layer changes
- API enables smooth transition

### 3. Testing Pyramid for Integration

**Description:**
Pyramid showing testing levels from unit to system tests.

```
              ┌─────────────┐
              │   System    │  ← U3: Complete wash cycle
              │   Tests     │     Error conditions
              │             │     Safety verification
              └─────┬───────┘
                    │
          ┌─────────▼──────────┐
          │  Integration Tests │  ← U3: Module interactions
          │                    │     Real hardware combined
          │                    │     Sequence testing
          └─────────┬──────────┘
                    │
      ┌─────────────▼─────────────┐
      │      Component Tests      │  ← U2/U3: Individual I/O
      │                           │     State transitions
      │                           │     Event generation
      └────────────┬──────────────┘
                   │
     ┌─────────────▼──────────────┐
     │        Unit Tests           │  ← U2: Module testing
     │                             │     Function-level tests
     │                             │     Mockup verification
     │                             │
     └─────────────────────────────┘

     More Tests                    More Complex/Realistic
     Faster                        Slower
     Cheaper                       More Expensive
```

**How to draw:**
1. Draw pyramid divided into 4 levels
2. Label each level (bottom to top): Unit, Component, Integration, System
3. Annotate which lab phase (U2/U3)
4. Add characteristics at bottom (More tests/faster vs. Complex/slow)
5. Use different colors for each level

**Meaning:**
- Foundation is many unit tests (fast, cheap)
- Fewer integration tests (slower, more setup)
- Fewest system tests (most complex, most realistic)
- U2 focuses on lower levels, U3 on upper levels

### 4. Hardware Connection Diagram

**Description:**
Physical connection between CT Board and washing machine.

```
┌──────────────────────┐
│     CT Board         │
│   (Microcontroller)  │
│                      │
│  ┌────────────────┐  │
│  │ MCU Core       │  │
│  └────────┬───────┘  │
│           │          │
│  ┌────────▼───────┐  │
│  │ GPIO/CPLD      │  │
│  │ 0x60000400     │  │
│  │ 0x60000410     │  │
│  └────────┬───────┘  │
│           │          │
└───────────┼──────────┘
            │
    ┌───────▼────────┐
    │ 16-pin         │
    │ External       │
    │ Connector      │
    └───────┬────────┘
            │
┌───────────▼──────────────┐
│  Washing Machine         │
│  Hardware Interface      │
│                          │
│  Inputs:                 │
│  - Door sensor           │
│  - Floaters              │
│  - Buttons               │
│  - Thermostat            │
│                          │
│  Outputs:                │
│  - Door lock             │
│  - Valve solenoid        │
│  - Heater relay          │
│  - Motor control         │
│  - Pump control          │
└──────────────────────────┘
```

**How to draw:**
1. Top box: CT Board with MCU and GPIO
2. Middle: Connector with arrows
3. Bottom box: Washing machine hardware
4. Label signal flow direction
5. List input/output components

**Meaning:**
- Shows physical signal path
- CT Board is controller
- Connector is interface point
- Real hardware at bottom

### 5. Debouncing Timeline

**Description:**
Timing diagram showing button bounce and debouncing effect.

```
Physical Button:
Press ↓                            Release ↑
     │                                  │
─────┘  ┐┌┐┌──────────────────────┐┌┐┌─┘────
        └┘└┘    (stable low)       └┘└┘
         ↑                               ↑
       Bounce                         Bounce

Raw Signal Read:
  1  ┐ ┐┌┐┌──────────────────────┐┌┐┌─┐
  0  └─┘└┘                        └┘└┘ └─
       ↑ Multiple transitions        ↑

After Debouncing (50ms filter):
  1  ┐                              ┌─────
  0  └──────────────────────────────┘
     ↑                              ↑
   Stable Low                    Stable High
   (BUTTON_PRESSED event)        (BUTTON_RELEASED event)

Time →
```

**How to draw:**
1. Three horizontal timelines stacked vertically
2. Top: Physical button state with bounce indication
3. Middle: Raw signal with multiple transitions
4. Bottom: Debounced signal (clean transitions)
5. Vertical markers showing event generation
6. Time axis at bottom

**Meaning:**
- Shows why debouncing is necessary
- Physical contacts don't transition cleanly
- Raw signal has multiple edges
- Debounced signal has clean transitions
- Events generated only on stable transitions

### 6. State Transition with Hardware Delays

**Description:**
Timing diagram showing difference between mockup and real hardware.

```
Event: BUTTON_WASH pressed

Mockup (U2) - Instant Response:
        ↓ Event
State:  IDLE ────────→ FILL_WATER
                ↓
Actions: door_lock() ─┘─ valve_open()

Outputs: ──┬─ Door Lock ON
           └─ Valve ON

Time: 0ms      <1ms    <1ms

Real Hardware (U3) - With Delays:
        ↓ Event
State:  IDLE ────────→ FILL_WATER
                ↓
Actions: door_lock() ─┘─ valve_open()

Door Lock:     ──┐     Physical lock engages
Actuator          └────┐     (150ms delay)
                       └─── LOCKED

Valve:        ──┐      Solenoid energizes
Solenoid         └────┐     (100ms delay)
                      └─── OPEN

Water Flow:            Start flowing
                           └────── (200ms+ delay)

Time: 0ms      <1ms    100ms   150ms   200ms+
```

**How to draw:**
1. Two sections: Mockup and Real Hardware
2. For each: Timeline showing event, state change, actions, outputs
3. Use horizontal lines with vertical transitions
4. Annotate delays for real hardware
5. Show cumulative effects of delays

**Meaning:**
- Mockup has instant response (no physical delays)
- Real hardware has mechanical delays
- Must account for delays in state machine design
- Some delays are cumulative (valve must open before water flows)

## Formulas & Procedures

### Debouncing Algorithm

**Software Debouncing with Counter:**

```c
#define DEBOUNCE_SAMPLES 5  // Number of consecutive identical readings
#define INPUT_REG ((volatile uint8_t*) 0x60000410)

static uint8_t stable_input = 0xFF;
static uint8_t debounce_count[8];  // One counter per input bit

uint8_t get_debounced_input(void) {
    uint8_t raw_input = *INPUT_REG;
    uint8_t debounced = stable_input;

    for(int bit = 0; bit < 8; bit++) {
        uint8_t mask = 1 << bit;
        uint8_t raw_bit = raw_input & mask;
        uint8_t stable_bit = stable_input & mask;

        if(raw_bit == stable_bit) {
            // Input matches stable state, reset counter
            debounce_count[bit] = 0;
        } else {
            // Input differs from stable state, increment counter
            debounce_count[bit]++;
            if(debounce_count[bit] >= DEBOUNCE_SAMPLES) {
                // Input has been different for enough samples
                // Update stable state
                if(raw_bit) {
                    debounced |= mask;  // Set bit
                } else {
                    debounced &= ~mask;  // Clear bit
                }
                debounce_count[bit] = 0;
            }
        }
    }

    stable_input = debounced;
    return debounced;
}
```

**Timing Calculation:**
```
Debounce Time = DEBOUNCE_SAMPLES × Sample Period
Sample Period = 10ms (for 100Hz main loop)
Example: 5 samples × 10ms = 50ms debounce time
```

### Integration Testing Procedure

**Step-by-Step Checklist:**

**Phase 1: Pre-Integration Verification**
```
□ Mockup version fully tested
□ All unit tests pass
□ State machine behavior verified
□ Timing measurements recorded
□ Documentation complete
□ Safety procedures reviewed
□ Emergency stop accessible
```

**Phase 2: Hardware Preparation**
```
□ Washing machine hardware connected to external connector
□ Power supply adequate for all actuators
□ All connections secure
□ Safety interlocks functional
□ Measurement equipment ready (oscilloscope, multimeter)
□ Backup power source available
```

**Phase 3: Individual Output Testing**
```
For each output (door_lock, valve, heater, pump, motor):
  □ Call function from debugger or test code
  □ Measure output signal with oscilloscope
  □ Verify correct voltage levels (0V = on, 5V = off for active-low)
  □ Verify correct pin (check with schematic)
  □ Observe physical actuator response
  □ Measure response time
  □ Check for electrical issues (current draw, noise)
  □ Verify output turns off correctly
  □ Document results
```

**Phase 4: Individual Input Testing**
```
For each input (door, buttons, floaters, thermostat):
  □ Manually trigger sensor/button
  □ Observe input register value changes
  □ Verify correct bit changes
  □ Verify active-low logic (0 when active)
  □ Test debouncing (rapid changes filtered)
  □ Verify edge detection (event generated once)
  □ Test with noise (motor running)
  □ Document voltage levels and timing
```

**Phase 5: State Transition Testing**
```
For each state transition:
  □ Set initial state
  □ Trigger event (manually or via sensor)
  □ Verify actions executed (observe physical hardware)
  □ Verify state change
  □ Measure transition time
  □ Check for race conditions
  □ Verify no unintended side effects
  □ Document results
```

**Phase 6: Sequence Testing**
```
□ Power-on sequence (initialization)
□ Door close → idle
□ Wash button → start cycle
□ Fill water (valve opens, wait for floater)
□ Heat water (heater on, wait for temperature)
□ Rotate (motor alternates directions)
□ Empty water (pump on, wait for low floater)
□ Spin dry (motor fast)
□ Shutdown (pump drains remaining water)
□ Door unlock
□ Return to idle
```

**Phase 7: Error Condition Testing**
```
□ STOP button during filling
□ STOP button during heating
□ STOP button during rotating
□ STOP button during draining
□ Door opened during wash (should abort)
□ Floater stuck high (timeout handling)
□ Floater stuck low (timeout handling)
□ Temperature sensor failure
□ Power interruption (brownout recovery)
□ Long cycle timeout (safety abort)
```

**Phase 8: Safety Verification**
```
□ Door cannot open when locked
□ Heater turns off if door opens
□ Motor stops if door opens
□ Valve closes if error occurs
□ Emergency stop accessible and functional
□ No water leaks
□ No electrical shorts
□ Proper grounding
□ Overcurrent protection functional
```

**Phase 9: Performance Measurement**
```
□ Response time: Button press to action < 100ms
□ Fill time: Empty to full (record for calibration)
□ Heat time: Cold to hot (record for calibration)
□ Rotate time: Per requirement
□ Drain time: Full to empty (record)
□ Spin time: Per requirement
□ Total cycle time: Should match design
□ Power consumption: Measure for each phase
```

**Phase 10: Regression Testing**
```
□ Re-run all mockup tests with real hardware
□ Verify no functionality lost
□ Check all requirements still met
□ Performance within specifications
□ No new bugs introduced
```

**Phase 11: Documentation**
```
□ Test results recorded
□ Issues documented with resolution
□ Code changes documented
□ Timing measurements tabulated
□ Photos/videos of hardware operation
□ Lessons learned noted
□ Integration complete checklist signed off
```

### Timing Measurement Procedure

**Measuring Response Time:**

```c
// Add timestamp recording to action functions (for testing)
#include "hal_timer.h"

uint32_t event_timestamp = 0;
uint32_t action_timestamp = 0;

// In event handler (when event detected)
event_t eh_get_event(void) {
    // ... event detection code ...
    if(event != NO_EVENT) {
        event_timestamp = get_microseconds();  // Record when event detected
        return event;
    }
}

// In action handler (when action executed)
void door_lock(void) {
    action_timestamp = get_microseconds();  // Record when action starts
    *OUTPUT_REG &= ~(1 << DOOR_LOCK_BIT);

    // Calculate and log response time
    uint32_t response_time = action_timestamp - event_timestamp;
    log_response_time("door_lock", response_time);  // Send to UART or store
}
```

**Analysis:**
```
Response Time = Action Timestamp - Event Timestamp

Components:
- Event detection delay: ~1ms (polling rate)
- Event processing: <1ms (switch statements)
- Action execution: <1µs (register write)
- Total: Typically 1-2ms (well under 100ms requirement)
```

### Memory Address Update Procedure

**If External Connector Uses Different Addresses:**

```c
// Old (mockup) - U2
#define INPUT_REG  ((volatile uint8_t*) 0x60000210)  // CT Board buttons
#define OUTPUT_REG ((volatile uint8_t*) 0x60000200)  // CT Board LEDs

// New (real hardware) - U3
#define INPUT_REG  ((volatile uint8_t*) 0x60000410)  // External connector
#define OUTPUT_REG ((volatile uint8_t*) 0x60000400)  // External connector

// All code using these macros automatically updated
// This is why #define is used instead of hardcoded addresses
```

**Verification:**
```
1. Read datasheet for external connector addresses
2. Update #define statements in header file
3. Recompile entire project
4. Test each I/O individually
5. Verify no hardcoded addresses remain (search codebase)
```

## Comparisons & Contrasts

### Mockup (U2) vs. Real Hardware (U3)

| Aspect | Mockup (U2) | Real Hardware (U3) |
|--------|------------|-------------------|
| **Input Source** | CT Board buttons/switches | Washing machine sensors |
| **Output Target** | CT Board LEDs | Washing machine actuators |
| **Response Time** | Instant (< 1µs) | Mechanical delays (100-200ms) |
| **Timing** | Perfectly consistent | Variable (mechanical, electrical) |
| **Noise** | None | Electrical noise from motors/relays |
| **Bounce** | None (clean signals) | Mechanical bounce on switches |
| **Safety Risk** | None | Water, electricity, moving parts |
| **Debugging** | Easy (LEDs visible) | Harder (internal state not visible) |
| **Reproducibility** | Perfect | Variable (environmental factors) |
| **Testing Speed** | Fast | Slower (physical processes) |
| **Cost** | Low (existing board) | Higher (actuators, sensors) |
| **Power Consumption** | Minimal (LEDs) | Significant (motors, heaters) |
| **Failure Modes** | Software bugs only | Software + hardware failures |

**Code Changes Required:** Minimal (only action_handler implementation)

### Unit Testing vs. Integration Testing vs. System Testing

| Aspect | Unit Testing (U2) | Integration Testing (U3) | System Testing (U3) |
|--------|-------------------|------------------------|-------------------|
| **Scope** | Individual modules | Module interactions | Complete system |
| **Hardware** | Mockup | Real hardware, incremental | Real hardware, complete |
| **Isolation** | High (one module at a time) | Medium (combinations) | Low (everything together) |
| **Speed** | Fast | Medium | Slow |
| **Cost** | Low | Medium | High |
| **Complexity** | Low | Medium | High |
| **Debugging** | Easy | Moderate | Difficult |
| **Realism** | Low | Medium | High |
| **Automation** | Easy | Possible | Difficult |
| **Example** | Test timer_start() | Test state machine with real I/O | Full wash cycle |

**Testing Strategy:** Many unit tests, fewer integration tests, fewest system tests (testing pyramid)

### Software Debouncing vs. Hardware Debouncing

| Aspect | Software Debouncing | Hardware Debouncing |
|--------|-------------------|-------------------|
| **Implementation** | Code (filtering algorithms) | Capacitor + resistor circuit |
| **Cost** | Free (code) | Small cost (components) |
| **Flexibility** | Easy to adjust parameters | Fixed (need component change) |
| **CPU Usage** | Some (polling, filtering) | None (hardware handles) |
| **Response Time** | Configurable (trade-off) | Fixed by RC time constant |
| **Reliability** | Depends on code correctness | Very reliable (physics) |
| **Typical Usage** | Microcontroller inputs | External button circuits |
| **This Lab** | Software (in event_handler) | N/A (hardware provided) |

**Best Practice:** Use both for critical inputs (redundancy)

### Incremental Integration vs. Big Bang Integration

| Aspect | Incremental (This Lab) | Big Bang (Not Recommended) |
|--------|----------------------|---------------------------|
| **Approach** | Connect one I/O at a time, test | Connect everything at once |
| **Risk** | Low (issues isolated) | High (many variables) |
| **Debugging** | Easy (know what changed) | Hard (everything suspect) |
| **Time** | Longer initial | Shorter initial, longer debugging |
| **Success Rate** | High | Low |
| **Control** | High (systematic) | Low (chaotic) |
| **Example** | Test door lock, then valve, then heater... | Connect all, hope it works |

**Recommended:** Always use incremental integration for complex systems

## Examples & Applications

### Example 1: Updating Action Handler for Real Hardware

**Scenario:** Transitioning door_lock() from mockup to real hardware.

**Mockup Implementation (U2):**
```c
// action_handler.c (mockup version)
#include "reg_ctboard.h"

#define LED_DOOR_LOCK 0

void door_lock(void) {
    // Turn on LED to indicate door locked
    LED->DATA &= ~(1 << LED_DOOR_LOCK);  // Active-low
}

void door_unlock(void) {
    // Turn off LED
    LED->DATA |= (1 << LED_DOOR_LOCK);
}
```

**Real Hardware Implementation (U3):**
```c
// action_handler.c (real hardware version)
#define OUTPUT_REG ((volatile uint8_t*) 0x60000400)
#define DOOR_LOCK_BIT 7

void door_lock(void) {
    // Energize door lock solenoid
    *OUTPUT_REG &= ~(1 << DOOR_LOCK_BIT);  // Active-low, set to 0

    // Note: Physical lock takes ~150ms to engage
    // State machine should wait before assuming door is locked
}

void door_unlock(void) {
    // De-energize door lock solenoid
    *OUTPUT_REG |= (1 << DOOR_LOCK_BIT);  // Set to 1 (inactive)

    // Lock releases immediately when de-energized
}
```

**Key Differences:**
1. Memory address changed (LED register → external connector register)
2. Bit position changed (bit 0 → bit 7)
3. Added comment about mechanical delay
4. Same function signatures (API unchanged)
5. Same active-low logic

**State Machine (UNCHANGED):**
```c
// This code doesn't need to change!
case IDLE:
    if(event == BUTTON_WASH) {
        door_lock();  // Same API call works with both implementations
        valve_open();
        current_state = FILL_WATER;
    }
    break;
```

### Example 2: Adding Debouncing for Button Inputs

**Problem:** In U2 with mockup, button presses were clean. In U3 with real hardware, button bounces cause multiple BUTTON_WASH events.

**Symptom:**
```
User presses WASH button once
→ Event handler detects 5-10 falling edges (bounce)
→ State machine receives BUTTON_WASH event multiple times
→ Unexpected behavior (tries to start wash cycle multiple times)
```

**Solution: Add Debouncing to Event Handler**

```c
// event_handler.c (enhanced for U3)

#define DEBOUNCE_TIME 5  // 50ms at 100Hz polling rate

// Debounce state for each button
static uint8_t button_debounce_counter[3] = {0, 0, 0};  // WASH, SPIN, STOP
static uint8_t button_stable_state[3] = {1, 1, 1};  // Start with buttons not pressed (1 = inactive for active-low)

event_t eh_get_event(void) {
    uint8_t inputs = *INPUT_REG;

    // Debounce BUTTON_WASH (bit 5)
    uint8_t wash_current = (inputs >> 5) & 1;
    if(wash_current == button_stable_state[0]) {
        button_debounce_counter[0] = 0;  // Reset counter
    } else {
        button_debounce_counter[0]++;
        if(button_debounce_counter[0] >= DEBOUNCE_TIME) {
            // Button state has been stable for debounce period
            button_stable_state[0] = wash_current;
            button_debounce_counter[0] = 0;

            // Detect falling edge (button press)
            if(wash_current == 0) {  // Active-low: 0 = pressed
                return BUTTON_WASH;
            }
        }
    }

    // Similar debouncing for BUTTON_SPIN and BUTTON_STOP...

    // Check timer timeout...

    // Check sensors (may not need debouncing)...

    return NO_EVENT;
}
```

**Result:**
- Button must be stable for 50ms before event generated
- Eliminates bounce (typically < 20ms)
- Clean single event per button press
- No changes needed to state machine

### Example 3: Handling Mechanical Delays in State Machine

**Problem:** Heater turns on before valve fully closes, risks operating heater without water.

**Unsafe Sequence (works in mockup, fails in real hardware):**
```
FILL_WATER --[FLOATER_HIGH]--> HEAT_WATER
Actions: valve_close(); heater_on();

Timeline:
t=0ms:     FLOATER_HIGH event detected
t=1ms:     valve_close() called (bit set)
t=1ms:     heater_on() called (bit set)
t=100ms:   Valve physically closes
t=101ms:   Heater operating without water flowing!  // UNSAFE!
```

**Solution: Add Intermediate Wait State**

```c
// Add new state
typedef enum {
    // ... existing states ...
    FILL_WATER,
    VALVE_CLOSING_WAIT,  // NEW: Wait for valve to fully close
    HEAT_WATER,
    // ...
} state_t;

// Updated state machine
void fsm_handle_event(event_t event) {
    switch(current_state) {
        case FILL_WATER:
            if(event == FLOATER_HIGH) {
                valve_close();
                timer_start(20);  // Wait 200ms for valve to fully close
                current_state = VALVE_CLOSING_WAIT;  // Go to wait state
            }
            break;

        case VALVE_CLOSING_WAIT:
            if(event == TIME_OUT) {
                // Now safe to turn on heater
                heater_on();
                current_state = HEAT_WATER;
            }
            else if(event == BUTTON_STOP) {
                // Can still abort during wait
                timer_stop();
                pump_on();
                current_state = SHUT_DOWN;
            }
            break;

        case HEAT_WATER:
            // ... continue with normal heating logic ...
            break;
    }
}
```

**Result:**
- Valve has time to fully close before heater activates
- Safe operation
- Demonstrates need for hardware-aware state machine design

### Example 4: Complete Integration Test Sequence

**Scenario:** First integration test of full wash cycle with real hardware.

**Test Plan:**
```
Test ID: INT-001
Description: Complete wash cycle with real hardware
Preconditions:
- Code updated with real hardware action handlers
- Hardware connected and powered
- Water supply available
- Emergency stop accessible
```

**Test Execution:**

```
Step 1: Power On
  Action: Apply power to CT Board
  Expected: System initializes to DOOR_OPEN state
  Observed: ✓ Door unlocked (verified with multimeter on output bit 7)
  Result: PASS

Step 2: Close Door
  Action: Close washing machine door (trigger door sensor)
  Expected: System transitions to IDLE state
  Observed: ✓ Door sensor reading changed (bit 7 of input = 0)
           ✓ Event DOOR_CLOSED generated
           ✓ State changed to IDLE (verified via debugger)
  Measurement: Response time = 15ms (event detection to state change)
  Result: PASS

Step 3: Press WASH Button
  Action: Press WASH button
  Expected: Door locks, valve opens, transition to FILL_WATER
  Observed: ✓ BUTTON_WASH event generated
           ✓ door_lock() called (output bit 7 = 0)
           ✓ Physical door lock engaged (heard click, ~150ms delay)
           ✓ valve_open() called (output bit 6 = 0)
           ✓ Valve solenoid clicked (~100ms delay)
           ✓ Water started flowing (~500ms delay from valve opening)
           ✓ State = FILL_WATER
  Measurement: Button press to door locked = 165ms
               Button press to water flowing = 615ms
  Result: PASS (within acceptable range)

Step 4: Water Fills to High Level
  Action: Wait for water to reach high floater
  Expected: FLOATER_HIGH event, valve closes, heater turns on, wait state
  Observed: ✓ Floater sensor triggered (input bit 0 = 0)
           ✓ FLOATER_HIGH event generated
           ✓ valve_close() called
           ✓ Valve closed (~100ms delay)
           ✓ 200ms timer started
           ✓ After timeout, heater_on() called
           ✓ Heater relay clicked
           ✓ State = HEAT_WATER
  Measurement: Fill time = 42.3 seconds
               Valve close delay = 97ms
  Result: PASS

Step 5: Water Heats
  Action: Wait for water to reach temperature
  Expected: TEMPERATURE_HOT event, heater off, motor starts
  Observed: ✓ Temperature sensor changed (input bit 6 changed)
           ✓ TEMPERATURE_HOT event generated
           ✓ heater_off() called
           ✓ Heater relay de-energized
           ✓ motor_on_slow_right() called
           ✓ Motor started rotating (observed drum movement)
           ✓ Timer started (4 seconds)
           ✓ State = ROTATE_RIGHT
  Measurement: Heat time = 127 seconds (cold water to hot)
  Result: PASS

Step 6: Rotation Cycle
  Action: Wait for rotation timeout and direction changes
  Expected: Motor alternates directions multiple times
  Observed: ✓ After 4s, TIME_OUT event
           ✓ Motor direction changed to left
           ✓ State = ROTATE_LEFT
           ✓ After another 4s, TIME_OUT event
           ✓ Motor stopped
           ✓ Pump started
           ✓ State = EMPTY_WATER
  Measurement: Rotation phase = 8 seconds (4s each direction)
  Result: PASS

Step 7: Drain Water
  Action: Wait for water to drain to low floater
  Expected: Pump runs until FLOATER_LOW, then spin cycle
  Observed: ✓ Pump running (audible)
           ✓ Water draining (visible)
           ✓ Low floater triggered (input bit 2 = 0)
           ✓ FLOATER_LOW event generated
           ✓ pump_off() called
           ✓ motor_on_fast() called (spin cycle)
           ✓ Motor running at high speed
           ✓ Timer started
           ✓ State = SPIN_DRY
  Measurement: Drain time = 38 seconds
  Result: PASS

Step 8: Spin Dry
  Action: Wait for spin timer to expire
  Expected: Motor stops, pump drains remaining water, shut down
  Observed: ✓ After timer expired, TIME_OUT event
           ✓ Motor stopped
           ✓ Pump restarted (drain any remaining water)
           ✓ State = SHUT_DOWN
  Measurement: Spin time = 10 seconds (as configured)
  Result: PASS

Step 9: Final Drain and Unlock
  Action: Wait for final drain complete
  Expected: FLOATER_LOW again, pump stops, door unlocks, return to IDLE
  Observed: ✓ FLOATER_LOW event
           ✓ pump_off() called
           ✓ door_unlock() called
           ✓ Door lock released (heard click)
           ✓ State = IDLE
  Result: PASS

Total Cycle Time: 239 seconds (~4 minutes)
Overall Result: ✓ PASS - Full cycle completed successfully

Issues Found: None
Notes:
- All timing within expected ranges
- All actuators functioned correctly
- All sensors provided clean signals (debouncing effective)
- No electrical issues
- No safety violations
```

### Example 5: Debugging a Hardware Issue

**Scenario:** After integration, motor doesn't start during ROTATE_RIGHT state.

**Symptoms:**
- State machine transitions to ROTATE_RIGHT correctly
- motor_on_slow_right() function is called (verified with debugger)
- Output register bit is set correctly (verified with oscilloscope)
- But motor doesn't physically rotate

**Debug Process:**

**Step 1: Verify Software**
```c
// Add debug output
void motor_on_slow_right(void) {
    printf("motor_on_slow_right() called\n");  // Add logging

    *OUTPUT_REG &= ~(1 << MOTOR_SLOW_BIT);  // Slow = active (0)
    *OUTPUT_REG |= (1 << MOTOR_FAST_BIT);   // Fast = inactive (1)

    printf("Output register = 0x%02X\n", *OUTPUT_REG);  // Check value
}
```

**Output:**
```
motor_on_slow_right() called
Output register = 0xFB  // Binary: 11111011
                        //          ^^^^ bit 2 = 0 (slow active)
                        //          ^^^ bit 3 = 1 (fast inactive)
```
→ Software correct, register value correct

**Step 2: Verify Electrical Signal**
```
Use oscilloscope to measure output pins:
- Motor Slow (pin 5): Measured 0.1V (correct for logic 0)
- Motor Fast (pin 7): Measured 5.0V (correct for logic 1)
```
→ Electrical signals correct at connector

**Step 3: Check Hardware Wiring**
```
Trace from connector to motor driver:
- Pin 5 (Slow) → Wire connected to motor driver input A
- Pin 7 (Fast) → Wire connected to motor driver input B
```
→ Wiring looks correct

**Step 4: Check Motor Driver**
```
Measure motor driver outputs:
- Motor driver input A: 0.1V (correct)
- Motor driver input B: 5.0V (correct)
- Motor driver enable: 0V (PROBLEM!)
```
→ **Found issue: Motor driver enable signal not connected**

**Solution:**
```
Check hardware schematic: Motor driver requires enable signal (active-high)
Our output register doesn't have a bit assigned for motor enable!

Options:
1. Hardware fix: Tie motor enable to Vcc (always enabled)
2. Software fix: Use unused output bit for enable control

Choose option 2 (more control):
```

**Code Fix:**
```c
// action_handler.c
#define MOTOR_ENABLE_BIT 1  // Use output bit 1 for motor enable

void motor_on_slow_right(void) {
    // Enable motor first
    *OUTPUT_REG |= (1 << MOTOR_ENABLE_BIT);  // Enable = 1 (active-high)

    // Set direction and speed
    *OUTPUT_REG &= ~(1 << MOTOR_SLOW_BIT);   // Slow = 0 (active)
    *OUTPUT_REG |= (1 << MOTOR_FAST_BIT);    // Fast = 1 (inactive)
}

void motor_off(void) {
    // Disable motor
    *OUTPUT_REG &= ~(1 << MOTOR_ENABLE_BIT);  // Enable = 0 (disabled)

    // Also deactivate speed controls
    *OUTPUT_REG |= (1 << MOTOR_SLOW_BIT);     // Slow = 1 (inactive)
    *OUTPUT_REG |= (1 << MOTOR_FAST_BIT);     // Fast = 1 (inactive)
}
```

**Retest:**
```
motor_on_slow_right() called
→ Motor enables (verified: enable signal = 5V)
→ Motor starts rotating at slow speed
→ ✓ FIXED!
```

**Lesson:**
- Hardware integration reveals issues not present in mockup
- Systematic debugging: software → electrical → hardware
- Always consult hardware schematics
- Test incrementally (one function at a time)

## Connections to Other Topics

### Connection to Lab U02: Implementation

**Direct Continuation:**
- U02: Developed code with mockup
- U03: Same code, real hardware
- Demonstrates value of abstraction and modular design

**Skills Built Upon:**
- U02: State machine implementation
- U03: Hardware integration and debugging
- Progressive complexity: simple → realistic

### Connection to Lab U01: Requirements Modeling

**Requirements Verification:**
- U01: Defined requirements and model
- U03: Final verification that requirements met
- Complete traceability: Requirements → Model → Code → Tests

**Example:**
```
REQ-WM-001: Door must lock before filling water
→ UML: IDLE --[BUTTON_WASH / door_lock(); valve_open()]--> FILL_WATER
→ Code: door_lock() called before valve_open()
→ U03 Test: Verify door physically locks before water flows
   - Measure door lock time: ~150ms
   - Measure water flow start: ~600ms after button press
   - Confirm: door locks BEFORE water flows ✓
```

### Connection to Lecture 05: Development Processes

**V-Model Right Side:**
- Left side (descent): Requirements → Design → Implementation
- Bottom: Implementation complete (U02)
- Right side (ascent): Unit Test → Integration Test → System Test
- U03 represents Integration and System Testing phases

**Verification Strategy:**
- Each development phase has corresponding test phase
- Integration testing verifies module interactions (U03)
- System testing verifies complete requirements (U03)

### Connection to Lecture 06: Non-Functional Requirements

**NFR Verification:**

**Performance NFR:**
```
Requirement: "Response time < 100ms for button press"
U03 Test: Measured 15ms from button press to state change ✓ PASS
```

**Reliability NFR:**
```
Requirement: "System must not get stuck in any state"
U03 Test: Emergency stop works from all states ✓ PASS
        All timeouts functional ✓ PASS
```

**Safety NFR:**
```
Requirement: "Door must not open during wash cycle"
U03 Test: Attempted to open door while washing - lock held ✓ PASS
        Lock released only in IDLE state ✓ PASS
```

### Connection to Future Labs

**P2: Energy Analysis (Future):**
- U03 real hardware enables actual energy measurements
- Can measure power consumption of each actuator
- Optimize state machine for energy efficiency

**P3: Multi-Processor (Future):**
- Could distribute washing machine control across processors
- One processor for state machine, another for motor control
- Demonstrates scalability of modular architecture

### Connection to Industry Practice

**Embedded Systems Development:**
- Standard practice: Develop with simulation/mockup
- Then integrate with real hardware incrementally
- Matches industry V-Model process

**Hardware-Software Co-Development:**
- Software developed in parallel with hardware
- Mockup enables software progress before hardware ready
- Integration phase critical for finding interface issues

**Safety-Critical Systems:**
- Washing machine has safety aspects (water, heat, motors)
- Integration testing crucial for safety verification
- Cannot rely on simulation alone for safety-critical systems

## Summary

**Lab U3 Achievements:**
1. Successfully transitioned from mockup (U2) to real hardware
2. Updated action_handler implementation for washing machine interface
3. Conducted incremental integration testing (outputs, inputs, transitions)
4. Verified complete system functionality with full wash cycle
5. Demonstrated that modular architecture enables smooth hardware transition

**Key Skills Demonstrated:**
- **Hardware Integration**: Connecting software to physical devices
- **Incremental Testing**: Systematic approach to integration
- **Debugging**: Hardware-software interface issues
- **Safety Verification**: Testing safety-critical behaviors
- **Requirements Validation**: End-to-end verification against original requirements

**Critical Insights:**
- **Abstraction Works**: API boundary enabled code reuse (95%+ of code unchanged)
- **Hardware is Different**: Real hardware has delays, noise, bounce (not in mockup)
- **Testing is Essential**: Each integration step must be verified
- **Incremental Approach**: Test one thing at a time, fix issues immediately

**Journey Complete:**
- U01: Requirements and Model
- U02: Implementation with Mockup
- U03: Integration with Real Hardware
- Result: Working washing machine controller!

## Possible Exam Questions

### Basic Understanding

1. **What is the main difference between Lab U02 and Lab U03?**
   - Expected: U02 uses mockup (CT Board LEDs/buttons), U03 uses real washing machine hardware
   - Same state machine and event handler, only action_handler changes
   - U03 adds integration testing and hardware-specific issues (debouncing, delays)

2. **Why is the API boundary important during integration?**
   - Expected: Separates interface from implementation
   - Allows changing implementation without changing application code
   - Enables smooth transition from mockup to real hardware
   - State machine code doesn't need to change

3. **What is contact bounce and why does it need to be handled?**
   - Expected: Mechanical switches don't transition cleanly
   - Multiple rapid on/off transitions when pressed (bounce period typically < 20ms)
   - Can be detected as multiple events
   - Solution: Debouncing (wait for stable signal before generating event)

4. **List three hardware-specific issues that don't appear in mockup but must be handled with real hardware.**
   - Expected any 3 of:
     - Contact bounce
     - Mechanical delays (actuators take time to respond)
     - Electrical noise
     - Power supply issues
     - Signal loading
     - Timing variations

5. **What is the testing pyramid and how does it apply to labs U02 and U03?**
   - Expected: Many unit tests (bottom), fewer integration tests (middle), fewest system tests (top)
   - U02 focuses on unit and component testing
   - U03 adds integration and system testing
   - More tests at lower levels = faster, cheaper; fewer at higher = slower but more realistic

6. **Explain the incremental integration approach used in U03.**
   - Expected: Connect and test one I/O at a time
   - Test each output individually before combining
   - Test each input individually
   - Then test state transitions
   - Finally test complete sequences
   - Advantage: Easy to isolate problems

### Application & Analysis

7. **Given this debouncing algorithm, calculate how long a button must be pressed before an event is generated, assuming the main loop runs at 100Hz:**
   ```c
   #define DEBOUNCE_SAMPLES 5
   ```
   - Expected calculation:
     - Loop period = 1/100Hz = 10ms
     - Debounce time = 5 × 10ms = 50ms
     - Button must be stable for 50ms

8. **The following code worked perfectly in U02 mockup but fails in U03 with real hardware. Identify the problem and propose a fix:**
   ```c
   case FILL_WATER:
       if(event == FLOATER_HIGH) {
           valve_close();
           heater_on();  // PROBLEM: Heater on while valve still closing
           current_state = HEAT_WATER;
       }
   ```
   - Expected problem: Valve takes ~100ms to close, heater turns on immediately
   - Risk: Operating heater without water (if valve not fully closed)
   - Fix: Add intermediate wait state
   ```c
   case FILL_WATER:
       if(event == FLOATER_HIGH) {
           valve_close();
           timer_start(20);  // Wait 200ms
           current_state = VALVE_CLOSING_WAIT;
       }
   case VALVE_CLOSING_WAIT:
       if(event == TIME_OUT) {
           heater_on();  // Now safe
           current_state = HEAT_WATER;
       }
   ```

9. **Describe a systematic procedure to test whether the door lock works correctly with real hardware.**
   - Expected procedure:
     1. Call door_lock() from test code
     2. Measure output bit with oscilloscope (should be 0 for active-low)
     3. Observe/hear physical lock engage (~150ms delay)
     4. Try to open door (should be locked)
     5. Call door_unlock()
     6. Verify output bit = 1
     7. Verify physical lock releases
     8. Try to open door (should open)
     9. Measure unlock time
     10. Repeat multiple times for reliability

10. **During integration testing, you observe that pressing the WASH button sometimes starts the cycle, sometimes doesn't respond, and occasionally starts two cycles. What is the likely cause and how would you fix it?**
    - Expected cause: Contact bounce (mechanical switch bouncing)
    - Effect: Multiple falling edges detected, inconsistent event generation
    - Fix: Implement software debouncing
      ```c
      // Require stable signal for 50ms before generating event
      if(button_stable_for_debounce_time && !button_previously_stable) {
          return BUTTON_WASH;  // Clean single event
      }
      ```

11. **What code module from U02 typically needs the MOST changes when transitioning to U03? What module needs the LEAST changes?**
    - MOST changes: action_handler (hardware interface implementation)
      - All function bodies change (LEDs → real actuators)
      - Memory addresses may change
      - May add debouncing, delays
    - LEAST changes: state_machine (application logic)
      - Typically zero changes if API well-designed
      - Only changes if hardware delays require state machine modifications

12. **Calculate the expected response time from button press to door lock actuation, given:**
    - Main loop polling rate: 100Hz (10ms per iteration)
    - Event detection time: 1ms
    - State machine processing: < 1ms
    - Register write time: < 1µs
    - Door lock mechanical delay: 150ms

    - Expected calculation:
      - Polling delay (worst case): 10ms
      - Event processing: ~2ms
      - Mechanical delay: 150ms
      - Total: ~162ms from press to physical lock engaged
      - Software response: ~12ms (electrical signal)
      - Mechanical response: +150ms (physical movement)

### Design & Critical Thinking

13. **Design a test plan to verify the safety requirement: "Door must not open during wash cycle." Include specific test cases and expected results.**
    - Expected test plan:
      - Test 1: During FILL_WATER, try to open door
        - Expected: Door locked (cannot open)
      - Test 2: During HEAT_WATER, try to open door
        - Expected: Door locked
      - Test 3: During ROTATE, try to open door
        - Expected: Door locked
      - Test 4: In IDLE, try to open door
        - Expected: Door unlocked (can open)
      - Test 5: Simulate door sensor failure (force open signal)
        - Expected: System aborts, goes to SHUT_DOWN
      - Test 6: Cut power to lock during wash
        - Expected: Lock releases (fail-safe?)

14. **Why is it important to test with real hardware even if the mockup version passes all tests?**
    - Expected reasons:
      - Mechanical delays not in mockup
      - Electrical noise not simulated
      - Contact bounce not present in mockup
      - Loading effects different
      - Power supply issues only with real hardware
      - Safety validation requires physical verification
      - Timing assumptions may be wrong
      - Example: Valve may take longer to close than assumed

15. **Compare software debouncing with hardware debouncing. Which would you choose for the washing machine buttons and why?**
    - Comparison:
      - Software: Free, flexible, consumes CPU, configurable
      - Hardware: Costs money, fixed, zero CPU, very reliable
    - For washing machine: **Prefer both (defense in depth)**
      - External hardware debouncing (RC filter on button input)
      - Software debouncing as backup (catch any remaining noise)
      - Buttons are safety-critical (wash cycle start)
      - Redundancy justified for reliability

16. **Explain why incremental integration is preferred over "big bang" integration (connecting everything at once).**
    - Expected:
      - Incremental: Low risk, easy debugging, controlled
      - Big bang: High risk, hard debugging, chaotic
      - Incremental advantages:
        - Know exactly what changed (one I/O at a time)
        - Can isolate problems immediately
        - Catch issues early before they compound
        - Systematic, methodical approach
      - Example: If motor doesn't work, with incremental you tested it individually so you know the problem is in motor code/hardware. With big bang, could be motor, power supply, interference from other actuators, etc.

17. **The mockup version runs perfectly for hours. The real hardware version fails after 15 minutes. What types of issues could cause this?**
    - Expected issues:
      - **Thermal**: Components heating up (motors, heaters, power supply)
      - **Memory leak**: Software bug not caught in short mockup tests
      - **Power supply**: Voltage droop under sustained load
      - **Mechanical wear**: Moving parts binding or jamming
      - **Water accumulation**: Leaks causing short circuits
      - **Timeout bugs**: Long-running cycles hit unexpected edge cases
      - Mockup doesn't have these physical failure modes

18. **How would you modify the event handler to handle electrical noise from the motor interfering with sensor readings?**
    - Expected techniques:
      - **Filtering**: Require multiple consecutive identical readings
        ```c
        // Ignore single glitches
        if(current == previous && previous == prev_prev) {
            // Stable for 3 readings, accept it
        }
        ```
      - **Timing**: Ignore sensor changes immediately after motor events
        ```c
        if(time_since_motor_on < 100ms) {
            // Ignore sensor changes during motor startup
        }
        ```
      - **Hysteresis**: Require large change before accepting (for analog sensors)
      - **Hardware filtering**: Add capacitors to sensor inputs

### Testing & Verification

19. **Design a test case to verify the complete wash cycle from power-on to completion. Include preconditions, steps, measurements, and pass/fail criteria.**
    - Expected test case structure:
      ```
      Test ID: SYS-001
      Description: Complete wash cycle
      Preconditions:
      - System powered off
      - Water supply connected
      - Door open
      - No errors from previous tests

      Steps:
      1. Apply power → Expect: DOOR_OPEN state
      2. Close door → Expect: IDLE state
      3. Press WASH → Expect: door locks, valve opens, FILL_WATER
      4. Wait for fill → Expect: FLOATER_HIGH, heating starts
      5. Wait for heat → Expect: motor starts, rotations
      6. Wait for rotations → Expect: pump on, draining
      7. Wait for drain → Expect: spin cycle
      8. Wait for spin → Expect: final drain, door unlock, IDLE

      Measurements:
      - Total cycle time (expect ~4-5 minutes)
      - Response times at each transition (< 100ms)
      - All outputs activate correctly (verify with multimeter)
      - All sensors detect correctly

      Pass Criteria:
      - Complete cycle without errors
      - All states visited in correct order
      - All timing within specifications
      - Door locked during wash, unlocked at end
      - No water leaks
      - No electrical faults
      ```

20. **You need to verify that the emergency STOP button works from all states. Outline your test procedure.**
    - Expected procedure:
      ```
      For each state (IDLE, FILL_WATER, HEAT_WATER, ROTATE_RIGHT, etc.):
        1. Start system and navigate to that state
        2. Press STOP button
        3. Verify BUTTON_STOP event generated
        4. Verify state changes to SHUT_DOWN
        5. Verify all dangerous actuators off (heater, motor)
        6. Verify pump turns on (drain water)
        7. Verify system waits for FLOATER_LOW
        8. Verify door unlocks when safe (water drained)
        9. Verify system returns to IDLE
        10. Measure response time (should be < 100ms)
        11. Document: State tested, response time, result

      Special cases:
      - Test with door open (should still work)
      - Test during mechanical delays (valve closing, motor starting)
      - Test rapid repeated presses (debouncing handles it)

      Pass criteria: STOP works from ALL states, < 100ms response
      ```

21. **What measurements would you take during integration testing to verify the system meets non-functional requirements?**
    - Expected measurements:
      - **Performance**: Response times (button → action)
      - **Timing**: Duration of each phase (fill, heat, rotate, drain, spin)
      - **Energy**: Power consumption of each actuator, total cycle energy
      - **Reliability**: Run multiple cycles, count failures
      - **Safety**: Verify door lock hold force, measure timeout responses
      - **Accuracy**: Water level detection accuracy, temperature accuracy
      - Methods: Oscilloscope, multimeter, stopwatch, power analyzer

### Connections

22. **How does Lab U03 demonstrate the verification side of the V-Model from Lecture 05?**
    - Expected:
      - V-Model left side: Requirements → Design → Implementation (U01, U02)
      - V-Model right side: Unit Test → Integration Test → System Test (U02, U03)
      - U03 represents Integration and System Testing
      - Each development level has corresponding test level
      - Verification ensures implementation meets requirements

23. **Relate the integration testing in U03 to the requirements specified in U01. Give a specific example.**
    - Expected example:
      ```
      U01 Requirement: "The washing machine shall lock the door before filling water"
      → U01 Model: IDLE --[BUTTON_WASH / door_lock(); valve_open()]--> FILL_WATER
      → U02 Implementation: door_lock() called before valve_open()
      → U03 Integration Test:
        - Measure door lock signal: Activates at t=1ms
        - Physical lock engages: t=150ms
        - Valve signal: Activates at t=2ms
        - Water flows: t=600ms
        - Conclusion: Door physically locked BEFORE water flows ✓
      ```

24. **How does the modular architecture developed in U02 facilitate the integration testing in U03?**
    - Expected:
      - Each module tested independently (unit tests in U02)
      - Integration testing focuses on interfaces between modules
      - Can replace action_handler without affecting other modules
      - Clear API boundaries = clear test boundaries
      - Example: Test state_machine → action_handler interface separately from action_handler → hardware interface

25. **U03 reveals hardware issues not present in U02 mockup. How does this relate to the concept of abstraction and its limitations?**
    - Expected:
      - Abstraction is useful (enables development without hardware)
      - But abstraction is not perfect (mockup != reality)
      - Must eventually test with real hardware
      - Abstraction limitations:
        - Timing differences (instant vs. delays)
        - Electrical effects (noise, loading)
        - Mechanical issues (bounce, jamming)
      - Lesson: Abstraction is a tool, not a replacement for reality
      - Integration testing is essential to validate assumptions

26. **How might the issues discovered during U03 integration influence the NFRs for future washing machine designs?**
    - Expected:
      - Add NFR for debouncing requirements (e.g., "Buttons shall be debounced for minimum 50ms")
      - Add NFR for timing margins (e.g., "State machine shall account for actuator delays of up to 200ms")
      - Add NFR for noise immunity (e.g., "System shall operate correctly with X dB of electrical noise")
      - Add NFR for safety timeouts (e.g., "System shall abort if any phase exceeds 2× expected duration")
      - Lessons learned → improved requirements for next design

27. **Compare the testing approach in U03 with how you would test a safety-critical system (e.g., medical device, automotive system).**
    - Similarities:
      - Incremental integration testing
      - Hardware-in-the-loop testing required
      - Safety requirements verification
      - Systematic test procedures
    - Differences:
      - Safety-critical: Formal verification methods, more rigorous
      - Safety-critical: Regulatory compliance testing (FDA, ISO 26262)
      - Safety-critical: Fault injection testing (what if sensor fails?)
      - Safety-critical: Long-term reliability testing (1000s of hours)
      - Safety-critical: Independent verification & validation
      - Washing machine: Less stringent (not life-safety), but still has safety aspects

28. **How does the integration process in U03 prepare you for real-world embedded systems development?**
    - Expected:
      - Industry standard approach: Mockup → integration → verification
      - Experience with hardware-software interface issues
      - Systematic debugging methodology
      - Importance of incremental testing
      - API design for hardware abstraction
      - Documentation and traceability
      - Safety considerations
      - Real-world: Same challenges but larger scale (more modules, more hardware, more safety requirements)

29. **If this washing machine were to be certified for sale (e.g., CE marking in Europe), what additional testing beyond U03 would be required?**
    - Expected additional testing:
      - **Electrical Safety**: Insulation, grounding, fault protection (IEC 60335)
      - **EMC**: Electromagnetic compatibility (emissions and immunity)
      - **Mechanical Safety**: No pinch points, stable under load
      - **Water Safety**: IPX4 rating (splash-proof)
      - **Energy Efficiency**: Power consumption labeling
      - **Environmental**: Temperature, humidity ranges
      - **Reliability**: MTBF testing (100s-1000s of cycles)
      - **Documentation**: User manual, safety warnings, disposal instructions
      - U03 is functional verification; certification is much more comprehensive

30. **Discuss the trade-offs between thorough integration testing (like U03) and time-to-market pressure in commercial embedded systems development.**
    - Expected discussion:
      - **Thorough testing** (U03 approach):
        - Pros: Catches bugs early, higher quality, fewer field failures
        - Cons: Takes time, costs money, delays product launch
      - **Rushed testing** (skip integration, go straight to system test):
        - Pros: Faster to market, lower development cost
        - Cons: More bugs in field, expensive recalls, reputation damage
      - **Trade-off factors**:
        - Safety criticality (medical device: thorough; mobile app: can patch later)
        - Competitive pressure (first to market advantage)
        - Cost of field failures (recall cost vs. testing cost)
        - Regulatory requirements (some industries require thorough testing)
      - **Best practice**: Risk-based approach
        - Critical features: Thorough testing (like U03)
        - Non-critical features: Basic testing
        - Balance speed and quality
      - Real world: Often compressed schedule, must be efficient with testing (automate where possible, prioritize high-risk areas)
