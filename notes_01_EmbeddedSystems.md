# Lecture 01: Embedded Systems

## Technical Terms & Definitions

### Embedded System (ES)
- **Definition**: A computer system that is part of a larger system, serving to control devices, facilities, or processes
- **Key characteristics**:
  - Must include a processor engine (e.g., Microcontroller)
  - Often part of a larger system
  - Serve to control devices, facilities, processes
  - Often build Wireless Sensor Networks (WSN)

### Processor Engine
- The computational core of an embedded system, typically a microcontroller

### Wireless Sensor Networks (WSN)
- Networks formed by multiple embedded systems that communicate wirelessly

### Real-Time Operating System (RTOS)
- Operating system designed for embedded systems that must respond to events within specific time constraints

### Built-in Applications
- Software permanently integrated into embedded systems with nothing added at later stages

### Design Space
- The multi-dimensional space representing all possible design choices for an embedded system
- Main dimensions: Energy, Cost, Performance
- Additional dimensions: Security, Safety, Latency, Testability, Manufacturability, Configurability

### Bill of Materials (BOM)
- Complete list of hardware components needed for a system

### Hardware Accelerator
- Specialized hardware component designed to perform specific tasks faster than a general-purpose CPU (e.g., FPGA, CSU)

### Application Processing Unit (APU)
- High-performance processor unit, typically ARM Cortex-A series, designed for application-level tasks

### Real Time Processing Unit (RPU)
- Processor optimized for real-time operations, typically ARM Cortex-R series

### Graphics Processing Unit (GPU)
- Specialized processor for graphics rendering (e.g., ARM Mali-400 MP2)

### Configuration Security Unit (CSU)
- Hardware component providing cryptographic operations and security features

### Programmable Logic (FPGA)
- Field-Programmable Gate Array - reconfigurable hardware that can be programmed to implement custom logic

### Zynq UltraScale+ MPSoC
- Multi-Processor System-on-Chip from Xilinx combining multiple processor types with programmable logic

### Bare-metal Application
- Software that runs directly on hardware without an operating system

### Microcontroller (MCU)
- Integrated circuit containing processor, memory, and input/output peripherals

### System Bus
- Communication pathway consisting of address lines, data lines, and control signals connecting system components

### A/D Converter (Analog-to-Digital Converter)
- Component that converts continuous analog signals into discrete digital values

## Core Concepts

### Computer Engineering Hierarchy
Computer Engineering sits between Microelectronics and Computer Science, spanning multiple abstraction levels:
- **Bottom layer (Microelectronics)**: Semiconductors, Transistors
- **Middle layer (Computer Engineering)**: Gates, Flip-flops, State-machines
- **Top layer (Computer Science)**: Operating Systems, Application Programs

**Increasing level of abstraction** as you move up the hierarchy.

### Computer System Definition
A computer system is a device that:
1. Processes input
2. Takes decisions based on the outcome
3. Outputs the processed information

**Key relationship**: Hardware and software work together to create an application
- Often common hardware is used for many different applications
- Application is defined by the software (e.g., controls for washing machines, vending machines)

### Properties of Embedded Systems - Detailed

1. **Must Include Processor Engine**
   - Every ES requires at least one microcontroller or processor

2. **May Come Without User Interface**
   - Many ES have no UI or only a minor UI
   - Contrasts with general-purpose computers

3. **Designed for a Specific Purpose**
   - Not general-purpose like PCs
   - Tailored to specific application requirements

4. **Often Use RTOS**
   - Real-time constraints require specialized operating systems
   - Deterministic behavior is critical

5. **Built-in Applications**
   - Software is permanently integrated
   - No user-installable applications (unlike PCs)

6. **Resource-Limited**
   - Constrained in energy consumption
   - Limited memory capacity
   - Cost-sensitive designs

### Differences: Embedded Systems vs General-Purpose Computers

| Aspect | Embedded System | General-Purpose Computer |
|--------|----------------|-------------------------|
| **Purpose** | Specific, dedicated function | Multiple, user-defined tasks |
| **User Interface** | Minimal or none | Rich UI (keyboard, mouse, display) |
| **Software** | Fixed, built-in | User-installable applications |
| **Resources** | Highly constrained | Abundant (memory, power) |
| **Upgradability** | Usually fixed | Easily upgradable |
| **Operating System** | Often RTOS or bare-metal | General OS (Windows, Linux, macOS) |
| **Real-time Requirements** | Often critical | Usually not critical |

### Hardware Components of Embedded Systems

Standard embedded system architecture consists of:

1. **CPU (Central Processing Unit)**
   - Executes program instructions
   - Processes data

2. **Memory**
   - Stores programs and data
   - Types: RAM, ROM, Flash

3. **Input/Output Components**
   - Sensors (inputs): pressure sensors, temperature sensors, etc.
   - Actuators (outputs): valves, motors, displays
   - Communication interfaces: Serial interfaces, USB, etc.
   - A/D Converters: Transform analog signals to digital

4. **System Bus**
   - Connects all components
   - Components:
     - **Data lines**: Transfer data
     - **Address lines**: Specify memory/peripheral addresses
     - **Control signals**: Coordinate operations

**Example Architecture**: Sensing → Processing → Acting

### Design Space Concept

The design space represents all possible implementation choices for an embedded system. Key insight: **There is no single "good" solution** - only trade-offs.

**Primary Trade-off Triangle**:
- **Energy**: Power consumption, battery life
- **Cost**: Manufacturing cost, component cost
- **Performance**: Speed, throughput, responsiveness

**Design Choices Include**:
- Type of MCU (ARM-M0+, ARM-R5, RISC-V, TI MSP430, etc.)
- Number of MCUs (1, 2, or more)
- Cache system (size, type, number of levels)
- Hardware accelerators (special purpose, FPGA)
- Interconnect between MCUs (bus systems, bus widths)
- Sensor types (analog vs. digital)
- Software architecture (OS vs. bare-metal)
- Binding of SW blocks to HW components

**Extended Dimensions** (beyond Energy-Cost-Performance):
- **Security**: Protection against attacks
- **Safety**: Failure prevention, safety-critical requirements
- **Latency**: Response time requirements
- **Testability**: Ease of testing and verification
- **Manufacturability**: Production feasibility
- **Configurability**: Flexibility for different configurations

**Key Insight**: In embedded systems, HW and SW architectures influence each other → Requires systematic co-design approach

## Important Diagrams & Graphics

### 1. Computer Engineering Abstraction Layers
**Description**: A vertical stack showing layers from bottom to top:
- Bottom: Semiconductors / Microelectronics (with transistor symbol)
- Middle: Gates, Flip-flops, State-machines / Computer Engineering (with gate and flip-flop symbols)
- Top: Operating Systems, Application Programs / Computer Science (with Java/C++ logos, OS icons)
- Arrow on left labeled "increasing level of abstraction" pointing upward
- Visual representations: Transistor symbol, logic gate, flip-flop circuit, CPUs, computer peripherals

**Key takeaway**: Computer Engineering bridges the gap between hardware and software

### 2. Computer System Block Diagram
**Description**: Simple flow diagram showing:
- Left: "Information input" arrow
- Center: Box labeled "computer system" containing two layers:
  - Top layer: "Software"
  - Bottom layer: "Hardware"
- Right: "Information output" arrow
- All connected with directional arrows showing information flow

**Key takeaway**: Hardware and software work together as a unified system

### 3. Embedded System Hardware Architecture
**Description**: Block diagram showing system bus architecture:
- Left side: Two main blocks:
  - "CPU" block
  - "Memory" block
- Right side: "Input/Output" block containing:
  - Two "A/D Converter" blocks
  - "Valve Control" block
  - "Serial Interface" block
- External connections shown:
  - Pressure sensor → A/D Converter
  - Temperature sensor → A/D Converter
  - Valve Control → Valve (actuator graphic)
  - Serial Interface ↔ host (computer graphic)
- Bottom: Three-colored system bus connecting all components:
  - Blue line: data lines
  - Green line: address lines
  - Red line: control signals

**Key takeaway**: Sensing, Processing, Acting architecture with system bus interconnect

### 4. Advance Organizer ESE (Main Course Diagram)
**Description**: Comprehensive diagram showing the embedded systems engineering process:
- Top: "Embedded Systems Engineering" header
- Left box: "Functional & Non-Functional Requirements"
- Center top: "Software & Hardware Modelling" (with state machine diagram)
- Center: Large "Design Space" box containing:
  - Triangle with three vertices labeled "Energy", "Cost", "Performance"
  - Four design option boxes: "Low-Energy CPU", "1 CPU", "1 CPU + FPGA", "2 CPUs"
  - Points plotted on the triangle showing trade-offs
- Right box: "Hardware-BOM & Software-Implementation"
- Arrows showing flow: Requirements → Modelling → Design Space → Implementation

**Key takeaway**: Systematic design flow from requirements through modeling and design space exploration to implementation

### 5. Design Space Triangle Detail
**Description**: Triangle diagram showing three-way trade-off:
- Three vertices labeled: "Energy" (top), "Cost" (bottom-left), "Performance" (bottom-right)
- Design options positioned around triangle:
  - "Low-Energy CPU" near Energy vertex
  - "1 CPU" near Cost vertex
  - "2 CPUs" near Performance vertex
  - "1 CPU + FPGA" between Energy and Performance
- Points on triangle showing different design solutions

**Key takeaway**: No solution optimizes all three dimensions simultaneously - trade-offs are necessary

### 6. Extended Design Space Spider/Radar Chart
**Description**: Multi-dimensional spider chart with 10 axes radiating from center:
- Axes (clockwise from top): Cost, Performance, Latency, Energy, Configurability, Manufacturability, Testability, Safety, Security
- Scale: 0 (center) to 10 (outer edge)
- Multiple colored polygons showing different design solutions
- Each solution has different strengths and weaknesses across dimensions

**Key takeaway**: Real design space is multi-dimensional, not just energy-cost-performance

### 7. Ultra96-V2 System Architecture
**Description**: Block diagram of the Zynq UltraScale+ MPSoC system:
- Top: "2 GB LPDDR4 (DDR4 x32)" memory block
- Center left: "MPSoC XCZU3EG" main chip containing:
  - Processing System (PS) with:
    - "APU (4x Arm Cortex-A53)" - quad-core 1.2GHz
    - "RPU (2x Arm Cortex-R5)" - dual-core 500MHz
    - "GPU (Arm Mali-400 MP2)"
  - Interconnect and I2C components
- Center right: "Programmable Logic (PL)" containing FPGA fabric
- Peripheral connections shown:
  - Mini DisplayPort
  - Wi-Fi/Bluetooth module
  - USB 3.0 Host and Device ports
  - USB 2.0 Device
  - I2C Switch connecting to I2C peripherals
  - SPI connection to FPGA
  - GPIO to expansion connectors

**Key takeaway**: Heterogeneous multi-processor system combining CPUs, GPU, and FPGA

### 8. Traffic Light Intersection Diagram
**Description**: Top-down view of four-way intersection:
- Four roads meeting at center (north, south, east, west)
- Yellow striped pedestrian crossings on all four sides
- Black arrows showing:
  - Straight traffic flows
  - Left turn options
  - Right turn options
- Visual representation of real-world ES application

**Key takeaway**: Complex real-world system requiring coordination of multiple signals

## Formulas & Procedures

### No explicit formulas in this introductory lecture.

### Systematic ES Design Procedure (Conceptual)
1. **Define Requirements**
   - Functional requirements (what the system must do)
   - Non-functional requirements (performance, energy, cost constraints)

2. **Model System**
   - Software modeling (behavior, state machines)
   - Hardware modeling (components, interfaces)

3. **Explore Design Space**
   - Identify HW/SW options
   - Evaluate trade-offs (energy, cost, performance)
   - Select appropriate design point

4. **Implement**
   - Hardware BOM selection
   - Software implementation
   - HW/SW binding and integration

5. **Verify**
   - Testing
   - Validation against requirements

## Comparisons & Contrasts

### 1. Embedded Systems vs General-Purpose Computers

**Embedded Systems**:
- ✓ Optimized for specific tasks
- ✓ Resource-efficient
- ✓ Real-time capable
- ✓ Reliable and deterministic
- ✗ Limited flexibility
- ✗ Difficult to upgrade
- ✗ Application-specific

**General-Purpose Computers**:
- ✓ Highly flexible
- ✓ User-installable software
- ✓ Easy to upgrade
- ✓ Rich user interface
- ✗ Energy-hungry
- ✗ Expensive
- ✗ Non-deterministic timing

### 2. RTOS vs General-Purpose OS

**Real-Time OS (RTOS)**:
- Deterministic task scheduling
- Guaranteed response times
- Minimal resource footprint
- Specialized for embedded use cases

**General-Purpose OS**:
- Fair scheduling (not deterministic)
- Best-effort performance
- Large resource requirements
- Feature-rich for diverse applications

### 3. Different Processor Types in Ultra96

| Processor | Type | Speed | Use Case |
|-----------|------|-------|----------|
| **ARM Cortex-A53** | Application Processor | 1.2 GHz (quad-core) | General-purpose computing, Linux |
| **ARM Cortex-R5** | Real-time Processor | 500 MHz (dual-core) | Real-time tasks, deterministic control |
| **ARM Mali-400 MP2** | Graphics Processor | - | Graphics rendering |
| **FPGA Fabric** | Programmable Logic | - | Custom hardware acceleration |

### 4. Design Space Trade-off Examples

**Low-Energy CPU**:
- ✓ Excellent energy efficiency
- ✓ Low cost
- ✗ Limited performance

**1 CPU**:
- ✓ Low cost
- ✓ Simple design
- ✗ Moderate energy and performance

**2 CPUs**:
- ✓ High performance (parallel execution)
- ✗ Higher cost
- ✗ Higher energy consumption

**1 CPU + FPGA**:
- ✓ High performance for specific tasks
- ✓ Flexible (reconfigurable)
- ✗ Higher cost
- ○ Moderate energy (depends on usage)

## Examples & Applications

### 1. Traffic Light Control System
**Description**: Intersection management system controlling vehicle and pedestrian traffic

**Requirements**:
- Handle pedestrian crossings
- Control car traffic flow
- Manage unequal traffic distribution
- Coordinate multiple traffic lights

**Design Questions**:
- How many traffic lights needed?
- Do we need request buttons for pedestrians?
- Should we implement automatic car detection?
- How to handle emergency vehicles?

**ES Characteristics Demonstrated**:
- Real-time requirements (safety-critical)
- Sensor inputs (car detectors, pedestrian buttons)
- Actuator outputs (traffic light LEDs)
- Embedded control logic

### 2. Washing Machine Control System
**Description**: Automated washing machine controller

**Context in Course**:
- Used in exercises U1, U2, and U3
- Implemented on CT1 boards (STM32 Cortex-M microcontroller)
- Requires Keil μVision development environment

**ES Characteristics**:
- User interface (buttons, display)
- Multiple washing programs
- Sensor inputs (water level, temperature)
- Motor control (drum rotation)
- State machine-based control
- Real-time constraints (temperature monitoring)

### 3. AES Encryption/Decryption System
**Description**: Advanced Encryption Standard (AES) implementation on embedded hardware

**Context in Course**:
- Focus of practical exercises P1-P4
- Implemented on Ultra96 board
- Multiple implementation approaches explored

**AES Design Variants**:
- **Software on APU**: High-level implementation on Cortex-A53
- **Software on RPU**: Real-time implementation on Cortex-R5
- **Hardware in FPGA**: Custom hardware acceleration
- **Hardware in CSU**: Using dedicated crypto unit

**Key Concept**: Same functionality implemented in different ways to explore design space trade-offs in:
- Performance (speed)
- Energy consumption
- Cost (silicon area)
- Flexibility

**AES Technical Details**:
- Symmetric encryption algorithm
- Key sizes: 128, 192, or 256 bits
- Block cipher operating on 128-bit plaintext blocks
- Produces ciphertext output

## Connections to Other Topics

### Forward References (Topics to Come)

1. **Software Paradigms** (Lecture 02)
   - How to structure embedded software
   - Programming models and patterns

2. **Requirements** (Lecture 03)
   - Detailed study of functional and non-functional requirements
   - Requirements serve as starting point for design

3. **Modelling** (Lecture 04)
   - Software and hardware modeling techniques
   - State machines, UML diagrams

4. **Non-Functional Requirements** (Lecture 06)
   - Deep dive into energy, performance, cost constraints
   - Quantifying and validating requirements

5. **Energy** (Lecture 08)
   - Energy consumption analysis
   - Power modes and optimization
   - Connects to P2 project

6. **Performance** (Lecture 09)
   - Performance analysis and optimization
   - Timing analysis

7. **FPGA** (Lecture 10)
   - Programmable logic fundamentals
   - Hardware acceleration
   - Connects to P4 project

8. **Design Space Exploration** (Lectures 11-12)
   - Systematic exploration of design alternatives
   - Automated DSE methods
   - Connects to P5 project

9. **RTOS and Scheduling** (Lecture 13)
   - Real-time operating systems in detail
   - Task scheduling algorithms

10. **Multi-Processor Systems** (Lecture 14)
    - Parallel computing on embedded systems
    - Connects to P3 project (APU/RPU)

### Backward References (Prerequisites)

1. **Computer Technology (CT) Course**
   - Basic microcontroller programming
   - CT1 board familiarity
   - Digital logic fundamentals
   - C programming for embedded systems

2. **Digital Electronics**
   - Logic gates, flip-flops
   - State machines
   - Semiconductors and transistors

## Additional Important Notes

### Course Hardware Platforms

**CT1 Board**:
- STM32F407 microcontroller (ARM Cortex-M4)
- Used for exercises U1, U2, U3
- Keil μVision required for development

**Ultra96-V2 Board**:
- Xilinx Zynq UltraScale+ EG MPSoC
- Used for projects P1-P4
- Custom PetaLinux operating system
- Development via VS Code with cross-compilation
- DHCP server for easy SSH connection over Ethernet
- Specifications:
  - Wi-Fi, Bluetooth
  - SD-Card slot
  - Various I/O interfaces
  - 2GB LPDDR4 RAM

### Three Main Course Projects

1. **Traffic Light Control** (Conceptual example in lectures)
2. **Washing Machine Control** (Exercises U1-U3)
3. **AES Encryption** (Projects P1-P4)

### Key Philosophical Points

1. **No Single "Good" Solution**
   - Every design involves trade-offs
   - Optimal solution depends on requirements and constraints
   - Design space exploration is essential

2. **HW/SW Co-design**
   - Hardware and software decisions are interdependent
   - Cannot optimize separately
   - Need systematic approach considering both

3. **Resource Constraints are Fundamental**
   - Unlike general-purpose computing, resources are always limited
   - Energy, memory, cost must be carefully managed
   - Drives many design decisions

4. **Real-Time Requirements**
   - Many ES have timing constraints
   - Deterministic behavior often required
   - Influences both HW and SW choices

### Instructor Notes
- Lecturers: Dr. Simon Künzli, Tobias Welti
- Semester: HS 2025
- MS Teams code: yywj31v (for passive streaming)

## Summary - Key Takeaways

1. **Embedded Systems are specialized computers** designed for specific control tasks, often with limited resources and real-time requirements

2. **ES differ fundamentally from PCs** in purpose, flexibility, resource availability, and user interaction

3. **No single optimal solution exists** - design involves navigating multi-dimensional trade-off space

4. **Systematic approach required** - from requirements through modeling and design space exploration to implementation

5. **HW and SW must be co-designed** - they influence each other and cannot be optimized independently

6. **Course uses two main platforms**: CT1 board (STM32) for exercises and Ultra96 (Zynq MPSoC) for projects

7. **Three example systems** illustrate concepts: traffic light control, washing machine, and AES encryption

8. **Design space has many dimensions** - primary triangle of energy/cost/performance, plus security, safety, latency, etc.
