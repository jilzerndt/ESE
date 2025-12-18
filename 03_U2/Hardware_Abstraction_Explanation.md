# Hardware Abstraction Layer - CT Board vs Washing Machine

## Overview

Both versions implement **exactly the same API** (same function names and behavior), but the **implementation differs** based on the hardware available. This is a great example of modular software design and hardware abstraction!

## Concept: Hardware Abstraction Layer

The state machine logic (`state_machine.c`) remains **identical** in both versions because it only calls abstract API functions like:
- `ah_lock_door()` / `ah_unlock_door()`
- `ah_open_valve()` / `ah_close_valve()`
- `ah_heater_on()` / `ah_heater_off()`
- `ah_motor_on()` / `ah_motor_off()`
- `ah_pump_on()` / `ah_pump_off()`

The actual hardware implementation is hidden behind these functions in `action_handler.c` and `event_handler.c`.

---

## CT Board Version (Mockup/Emulation)

**Purpose**: Testing and debugging using CT Board hardware to **emulate** the washing machine

### Outputs (`action_handler.c`)

**Hardware Access**:
```c
#define PORT_OUTPUT (CT_LED->WORD)
```
- Uses **LEDs** to visualize washing machine states
- Each LED represents a different actuator (door lock, valve, heater, pump, motor)

**Logic**: **Active-HIGH** (set bit = LED on)
```c
// Turn ON
PORT_OUTPUT |= MASK_DOOR_LOCK;   // Set bit

// Turn OFF
PORT_OUTPUT &= ~MASK_DOOR_LOCK;  // Clear bit
```

**Bit Mapping** (lines 34-42):
```c
#define MASK_DOOR_LOCK          (0x00000001);  // Bit 0
#define MASK_VALVE              (0x00000010);  // Bit 4
#define MASK_HEATER             (0x00000100);  // Bit 8
#define MASK_PUMP               (0x00001000);  // Bit 12
#define MASK_MOTOR_RIGHT_SLOW   (0x00010000);  // Bit 16
#define MASK_MOTOR_RIGHT_FAST   (0x00100000);  // Bit 20
#define MASK_MOTOR_LEFT_SLOW    (0x01000000);  // Bit 24
#define MASK_MOTOR_LEFT_FAST    (0x10000000);  // Bit 28
```

**Initialization**:
```c
PORT_OUTPUT = 0;  // All LEDs off
```

### Inputs (`event_handler.c`)

**Hardware Access**:
```c
#define PORT_INPUT (CT_DIPSW->BYTE.S7_0)
```
- Reads from **DIP switches**
- You manually toggle switches to simulate sensors

**Bit Mapping** (lines 35-41):
```c
#define MASK_BUTTON_STOP        (0x01)  // Switch 0
#define MASK_BUTTON_SPIN        (0x02)  // Switch 1
#define MASK_BUTTON_WASH        (0x04)  // Switch 2
#define MASK_DOOR               (0x08)  // Switch 3
#define MASK_FLOATER            (0x10)  // Switch 4
#define MASK_THERMOSTAT         (0x20)  // Switch 5
```

**Edge Detection**:
- Buttons: **Positive edges** (switch closed)
- Door/Floater: Both positive and negative edges tracked

---

## Washing Machine Version (Actual Hardware)

**Purpose**: Controlling real washing machine actuators and reading real sensors

### Outputs (`action_handler.c`)

**Hardware Access**:
```c
#define PORT_OUTPUT (CT_GPIO->OUT.BYTE.P1)
```
- Uses **GPIO output pins** to control real actuators
- Controls real relays, motors, valves, etc.

**Logic**: **Active-LOW** (clear bit = device on)
```c
// Turn ON
PORT_OUTPUT &= MASK_DOOR_LOCK;   // Clear bit (active-low)

// Turn OFF
PORT_OUTPUT |= ~MASK_DOOR_LOCK;  // Set bit (inactive)
```

**Bit Mapping** (lines 34-42):
```c
#define MASK_DOOR_LOCK   (0x7f)  // ~Bit 7
#define MASK_VALVE_OPEN  (0xbf)  // ~Bit 6
#define MASK_HEATER_ON   (0xdf)  // ~Bit 5
#define MASK_MOTOR_SLOW  (0xf7)  // ~Bit 3
#define MASK_MOTOR_FAST  (0xfb)  // ~Bit 2
#define MASK_MOTOR_RIGHT (0xfd)  // ~Bit 1
#define MASK_PUMP_ON     (0xef)  // ~Bit 4
#define MASK_MOTOR_OFF   (0x0e)  // Clear motor bits
```

**Initialization**:
```c
PORT_OUTPUT = 0xff;  // All devices off (active-low)
```

**Motor Control**:
Separate control for direction and speed (lines 116-135):
- Direction: RIGHT or LEFT
- Speed: SLOW or FAST
- Both parameters controlled independently

### Inputs (`event_handler.c`)

**Hardware Access**:
```c
#define PORT_INPUT (CT_GPIO->IN.BYTE.P1)
```
- Reads from **GPIO input pins**
- Connected to real sensors (buttons, door switch, water floater, temperature sensor)

**Bit Mapping** (lines 35-42):
```c
#define MASK_FLOATER_HIGH     (0x01)  // Bit 0
#define MASK_FLOATER_LOW      (0x04)  // Bit 2
#define MASK_BUTTON_STOP      (0x08)  // Bit 3
#define MASK_BUTTON_SPIN      (0x10)  // Bit 4
#define MASK_BUTTON_WASH      (0x20)  // Bit 5
#define MASK_THERMOSTAT       (0x40)  // Bit 6
#define MASK_DOOR_CLOSED      (0x80)  // Bit 7
```

**Edge Detection**:
- Buttons: **Negative edges** (button release, active-low hardware)
- Sensors: Mixed positive/negative based on physical sensor behavior

---

## Comparison Summary

| Feature | CT Board (Emulation) | Washing Machine (Real HW) |
|---------|---------------------|---------------------------|
| **Output Port** | `CT_LED->WORD` | `CT_GPIO->OUT.BYTE.P1` |
| **Output Logic** | Active-HIGH | Active-LOW |
| **Output Hardware** | 32 LEDs | 8 GPIO pins |
| **Input Port** | `CT_DIPSW->BYTE.S7_0` | `CT_GPIO->IN.BYTE.P1` |
| **Input Hardware** | DIP switches | GPIO pins with sensors |
| **Purpose** | Testing/debugging | Real hardware control |
| **Init Value** | `0x00` (all off) | `0xFF` (all off, active-low) |
| **Button Edge** | Positive (switch ON) | Negative (button release) |
| **Motor Control** | 4 combined states | Direction + Speed separate |

---

## Key Advantages of This Design

1. **Testability**: Develop and test logic on CT board without needing real washing machine
2. **Safety**: No risk of damaging expensive washing machine hardware during development
3. **Debugging**: Visual feedback via LEDs makes debugging easier
4. **Portability**: Same state machine code works on both platforms
5. **Maintainability**: Hardware changes don't require state machine modifications

---

## Development Workflow

```
1. Design state machine on paper
   ↓
2. Implement state_machine.c with abstract API calls
   ↓
3. Implement action_handler.c for CT Board (LED outputs)
   ↓
4. Implement event_handler.c for CT Board (DIP switch inputs)
   ↓
5. Test and debug on CT Board
   ↓
6. Implement action_handler.c for washing machine (GPIO + active-low)
   ↓
7. Implement event_handler.c for washing machine (real sensors)
   ↓
8. Deploy to washing machine without changing state_machine.c!
```

This is the beauty of hardware abstraction - the same business logic runs on different hardware platforms!
