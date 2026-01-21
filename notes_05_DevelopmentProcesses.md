# Lecture 05: Development Processes

## Technical Terms & Definitions

### Abstraction Levels
- **Abstraction**: Hiding implementation details while exposing essential features at higher conceptual levels
- **System Level**: Highest abstraction - components, subsystems, tasks describing functionality
- **Module Level**: Processors, MCUs, ASICs, buses, peripherals, memory, interacting SW modules
- **Block Level**: Counters, registers, ALUs (hardware); program sequences, loops (software)
- **Logic Level**: Logic gates, flip-flops (hardware); instructions like ADDS R1, R2, R3 (software)
- **Device Level**: Electronic components, transistors (hardware); machine code like 0x21ff (software)

### Design Process Terms
- **Design Process**: Systematic approach to implement desired functionality with specific physical components
- **Specification**: Formal description of functional behavior at a given abstraction level
- **Exploration**: Investigating alternative solutions to meet requirements
- **Refinement**: Moving from higher abstraction level to next deeper level with more detail
- **Top-Down Design**: Starting from high-level idea and progressively adding detail through refinement
- **Bottom-Up Design**: Starting from low-level components and building up (less common for initial design)
- **Design Space**: Set of all possible design choices and implementations
- **Solution Space**: Range of valid implementations at current abstraction level

### Verification & Validation Terms
- **Verification**: Checking if system does things right (meets specifications correctly)
- **Validation**: Checking if system does the right things (meets user needs)
- **Model Checking**: Formal verification using temporal logic (from Lecture 04)
- **Analysis**: Mathematical or logical examination of system properties
- **Simulation**: Executing model to observe behavior without physical implementation
- **Measurement**: Testing actual physical system to observe real behavior
- **Functional Requirements**: What the system must do
- **Non-Functional Requirements**: How well the system must do it (performance, energy, cost)

### Development Process Models
- **Development Process/Model**: Structured approach for organizing development activities
- **Waterfall Model**: Sequential development process flowing through distinct phases
- **Phase**: Distinct stage in development with specific goals and deliverables
- **Requirements/Analysis**: Initial phase defining what system must do
- **Design**: Planning how to implement requirements
- **Implementation**: Actually building/coding the system
- **Test**: Verifying implementation works correctly
- **Operation**: Deploying and maintaining the system in use
- **V-Model**: Extension of waterfall with explicit verification/testing at each level
- **Iteration**: Repeating phases or activities based on feedback

### Agile Development Terms
- **Agile Development**: Iterative, incremental approach emphasizing flexibility and customer collaboration
- **Sprint**: Fixed time period (typically 2-4 weeks) for completing work in agile methods
- **Scrum**: Agile framework with defined roles, events, and artifacts
- **Product Backlog**: Prioritized list of features/requirements for the product
- **Sprint Backlog**: Subset of product backlog selected for current sprint
- **User Story**: Short description of feature from user perspective
- **Increment**: Usable product functionality delivered at end of sprint
- **Retrospective**: Team reflection on process to identify improvements
- **Stand-up/Daily Scrum**: Brief daily meeting for team synchronization
- **Demo**: Presentation of completed work to stakeholders
- **Extreme Programming (XP)**: Agile method emphasizing technical practices (pair programming, TDD)
- **Test-Driven Development (TDD)**: Writing tests before implementation code
- **Pair Programming**: Two developers working together at one workstation
- **Continuous Integration**: Frequently merging code changes to detect integration issues early

### CI/CD Terms
- **CI/CD**: Continuous Integration / Continuous Deployment - automated build, test, deploy pipeline
- **Continuous Integration (CI)**: Automatically building and testing code after each commit
- **Continuous Deployment (CD)**: Automatically deploying successful builds to production/target
- **Build Pipeline**: Automated sequence of steps from code commit to deployment
- **Build Server**: Dedicated machine running automated builds and tests
- **Version Control System (VCS)**: Software managing code versions (Git, SVN, etc.)
- **Commit**: Saving code changes to version control
- **Jenkins**: Popular open-source CI/CD automation server
- **GitLab CI Runner**: CI/CD tool integrated with GitLab
- **GitHub Actions**: CI/CD built into GitHub
- **Automated Testing**: Tests executed automatically by CI system
- **Build Status**: Indicator showing if latest build passed or failed
- **Hardware-in-the-Loop (HIL)**: Testing with actual hardware in automated pipeline

## Core Concepts

### Abstraction Layers in Embedded Systems

**Purpose of Abstraction**:
- Manage complexity by hiding details
- Enable different perspectives (system architect vs. chip designer)
- Facilitate communication between disciplines
- Support iterative refinement from concept to implementation

**Layer Hierarchy** (from high to low):
1. **System Level**: Overall functionality, subsystem interactions
2. **Module Level**: Hardware/software architecture, processor selection
3. **Block Level**: Internal structure of modules
4. **Logic Level**: Gate-level hardware, assembly-level software
5. **Device Level**: Transistors, machine code

**Abstraction Influences Development**:
- Higher levels: faster simulation, less accurate
- Lower levels: slower simulation, more accurate
- Design decisions at higher levels constrain lower levels
- Each level requires different skills and tools

### Design Process: Specification, Exploration, Refinement

**Three Core Activities at Each Abstraction Level**:

1. **Specification**:
   - Formally describe desired functionality
   - Example: UML state machine for washing machine control
   - Define interfaces between components
   - Document constraints and requirements

2. **Exploration**:
   - Investigate alternative implementations
   - Example washing machine alternatives:
     - Could we heat and fill water simultaneously?
     - Should we add more states for clarity?
     - Could we distribute functions across multiple MCUs?
   - Evaluate trade-offs (cost, performance, energy, complexity)
   - Select most promising solution

3. **Refinement**:
   - Elaborate selected solution at next lower abstraction level
   - Add implementation details
   - Decompose into subcomponents
   - Repeat specification-exploration cycle at new level

**Iterative Nature**:
- Not strictly linear - may need to go back up (abstraction)
- Verification may reveal need to revisit earlier decisions
- Learning at lower levels may influence higher-level choices

### Top-Down Design Approach

**Principle**:
- Start from high-level idea/requirements
- Progressively add detail through refinement
- Keep solution space open as long as possible
- Make commitments gradually as understanding improves

**Benefits**:
- **Efficiency**: Higher abstraction = faster exploration
- **Flexibility**: More options available early
- **Risk Management**: Catch fundamental issues before detailed implementation
- **Communication**: High-level models understandable to non-experts

**Visualized as Pyramid**:
- Top: Initial idea (many possible implementations)
- Bottom: Final implementation (specific choice)
- **Good Path**: Systematic refinement through center (optimal solution)
- **Bad Path**: Early commitment to suboptimal direction (miss better solutions)

**Opposite**: Bottom-up would start with transistors/machine code - impractical for complex systems

### Verification and Evaluation Throughout Development

**"Does Our System Do the Right Things Right?"**

**Multiple Verification Methods**:

1. **Formal Model Verification** (Model Checking):
   - Mathematical proof of properties
   - From Lecture 04: CTL formulas, exhaustive state space exploration
   - Example: Verify washing machine never opens door with water present

2. **Analysis**:
   - Static analysis of models/code without execution
   - Example: WCET analysis (from Lecture 04)
   - Calculat properties like worst-case execution time, memory usage

3. **Simulation**:
   - Execute model to observe behavior
   - Faster than real system, controllable scenarios
   - Accuracy depends on model fidelity
   - Example: Simulate task scheduling before hardware exists

4. **Measurement**:
   - Test actual implementation on real hardware
   - Most accurate but slowest, expensive
   - May not cover all scenarios
   - Example: Measure actual execution time on Ultra96

**Trade-offs**:
- **Speed**: Faster at higher abstraction levels
- **Accuracy**: More accurate at lower levels (closer to reality)
- **Coverage**: Formal methods can be exhaustive, testing is sampling

### Waterfall Model

**Structure**: Sequential phases, each must complete before next begins

**Phases**:
1. **Requirements/Analysis**: What must system do? Gather and document requirements
2. **Design**: How will we build it? Architecture, algorithms, interfaces
3. **Implementation**: Write code, build hardware
4. **Test**: Verify implementation matches design and requirements
5. **Operation**: Deploy, maintain, support in production

**Key Characteristics**:
- **Sequential**: No phase overlap
- **Documentation-Heavy**: Each phase produces documents for next
- **Phase Gates**: Formal approval needed to proceed
- **Predictable**: Clear milestones, easy to plan
- **Rigid**: Difficult to accommodate changes once phase completed

**When Waterfall Works Well**:
- Requirements well-understood and stable
- Technology mature and proven
- Hardware development (physical constraints limit iteration)
- Regulated industries requiring extensive documentation
- Large teams needing clear structure

**Limitations**:
- **Late Feedback**: Users see product only at end
- **Inflexible**: Changes expensive/difficult after requirements phase
- **Integration Risk**: Components first integrated at test phase
- **Unrealistic**: Assumes perfect up-front understanding

### V-Model

**Extension of Waterfall**: Adds explicit verification at each level

**Left Side (Design/Development)**:
1. **Requirements Specification** → Acceptance Test (right side)
2. **System Specification** → System Validation
3. **Architecture Design** → System Integration & Test
4. **Module Design** → Unit/Module Test
5. **Module Implementation** (bottom of V)

**Right Side (Test/Verification)**:
- Tests defined during corresponding design phase
- Bottom-up testing (unit → integration → system → acceptance)
- Each test level verifies corresponding design level

**Key Improvements over Waterfall**:
- **Test Planning Early**: Tests designed with requirements
- **Traceability**: Clear mapping from requirements to tests
- **Verification Focus**: Explicit verification activities
- **Feedback Loops**: Can iterate within levels

**Advantages**:
- Well-suited for embedded systems (hardware + software)
- Clear verification strategy
- Widely used in industry (automotive, aerospace, medical)
- Good for safety-critical systems

**Still Limitations**:
- Sequential nature inherited from waterfall
- Less flexible than agile methods
- Late user feedback (after implementation)

### Agile Development Methods

**Philosophy**: Value individuals, working software, customer collaboration, responding to change

**Common Characteristics Across Agile Methods**:
- **Iterative**: Development in short cycles (sprints)
- **Incremental**: Each iteration delivers working functionality
- **Adaptive**: Requirements can change based on feedback
- **Collaborative**: Close customer/stakeholder involvement
- **Self-Organizing Teams**: Teams decide how to accomplish work
- **Regular Reflection**: Continuous process improvement

**Scrum Framework**:
- **Roles**: Product Owner, Scrum Master, Development Team
- **Events**: Sprint Planning, Daily Scrum, Sprint Review, Sprint Retrospective
- **Artifacts**: Product Backlog, Sprint Backlog, Increment
- **Time-boxed Sprints**: Typically 2-4 weeks
- **Definition of Done**: Clear criteria for completion

**Extreme Programming (XP) Practices**:
- **Test-Driven Development**: Write test first, then code to pass it
- **Pair Programming**: Two developers, one keyboard
- **Continuous Integration**: Integrate and test frequently
- **Small Releases**: Deliver functional software frequently
- **Simple Design**: Do simplest thing that works
- **Refactoring**: Improve code structure without changing behavior

**Other Agile Variants**:
- **Crystal**: Family of methods tailored to project characteristics
- **Dynamic Software Development Method (DSDM)**: RAD approach with active user involvement
- **Feature-Driven Development (FDD)**: Organized around delivering features
- **Lean Software Development**: Inspired by lean manufacturing, eliminate waste

**Agile Benefits**:
- **Early Value Delivery**: Working software each sprint
- **Flexibility**: Adapt to changing requirements
- **Transparency**: Regular demos and communication
- **Quality**: Continuous testing and integration
- **Team Morale**: Empowerment and collaboration

**Agile Challenges**:
- **Scalability**: Harder with large teams/projects
- **Documentation**: May be insufficient for regulated industries
- **Predictability**: Harder to estimate total cost/time
- **Discipline Required**: Success depends on team maturity
- **Customer Availability**: Needs regular stakeholder engagement

### Waterfall/V-Model vs. Agile

**When to Use Waterfall/V-Model**:
- Requirements stable and well-understood
- Hardware development (physical constraints)
- Regulated industries (medical devices, aerospace)
- Large, distributed teams
- Fixed-price contracts
- High documentation requirements

**When to Use Agile**:
- Requirements uncertain or evolving
- Software-intensive projects
- Need for rapid time-to-market
- Close customer collaboration possible
- Small to medium co-located teams
- Innovation-driven projects

**Hybrid Approaches**:
- Many organizations combine elements
- Example: V-Model for hardware, Agile for software
- "Agile within Waterfall": Agile sprints within waterfall phases
- "Water-Scrum-Fall": Requirements (waterfall), Development (agile), Deployment (waterfall)

### Continuous Integration / Continuous Deployment (CI/CD)

**Purpose**: Automate build, test, and deployment to catch issues early

**Continuous Integration (CI)**:

**Workflow**:
1. Developer commits code to version control
2. CI server detects commit (automatically)
3. CI server checks out code
4. CI server builds code
5. CI server runs automated tests
6. CI server reports results (email, web dashboard)
7. Developer reads report and fixes issues if any

**Benefits**:
- **Fast Feedback**: Know within minutes if commit broke something
- **Integration Issues Caught Early**: No "integration hell" at end
- **Always Releasable**: Main branch should always build and pass tests
- **Shared Responsibility**: Entire team sees build status

**Continuous Deployment (CD)**:
- Extends CI to automatically deploy successful builds
- Can deploy to staging, testing, or production environments
- Enables rapid delivery of features to users
- Requires high confidence in automated testing

**Pipeline Stages** (typical):
1. **Build**: Compile code, create executables
2. **Test**: Run unit tests, integration tests
3. **Analysis**: Static code analysis, code coverage, complexity metrics
4. **Documentation**: Generate API docs, diagrams
5. **Package**: Create deployable artifacts
6. **Deploy**: Push to target environment (staging/production)

**Popular CI/CD Tools**:
- **Jenkins**: Open-source, highly extensible with plugins
- **GitLab CI**: Integrated with GitLab repository
- **GitHub Actions**: Integrated with GitHub
- **CircleCI, Travis CI**: Cloud-based services

**Advanced CI/CD**:
- **Hardware-in-the-Loop**: Automated testing with real embedded hardware
  - Example: CI server connected to multiple Ultra96 boards
  - Automatically flash firmware and run tests on hardware
  - Catch hardware-specific issues (timing, peripherals)
- **Memory Footprint Tracking**: Monitor code size over time
- **Performance Regression Testing**: Detect performance degradation
- **Parallel Builds**: Speed up by building for multiple targets simultaneously

**CI/CD in Embedded Systems**:
- More complex than pure software (need hardware, cross-compilation)
- May need custom test infrastructure (boards, fixtures, power control)
- Firmware flashing, serial console capture
- Real-time constraints testing
- Power consumption measurement

**V-Model + Agile + CI/CD**:
- V-Model and Agile both benefit from CI/CD
- V-Model: Automated testing at each verification level
- Agile: Enables continuous delivery within sprints
- CI/CD provides fast feedback loop for any process

## Important Diagrams & Graphics

### 1. Abstraction Levels Table (Hardware and Software)
**Structure**: Two-column table with 5 rows

| Level | Hardware | Software |
|-------|----------|----------|
| System | Components, subsystems | Tasks describing functionality |
| Module | Processors, MCUs, ASICs, buses, memory | Interacting SW modules |
| Block | Counters, registers, ALUs | Program sequences, loops |
| Logic | Logic gates, flip-flops | Instructions (ADDS R1, R2, R3) |
| Device | Electronic components, transistors | Machine code (0x21ff) |

**Concept**: Parallel abstraction hierarchies for hardware and software

### 2. Specification-Exploration-Refinement Diagram
**Structure**: Two abstraction layers (n and n+1) connected by refinement arrow

**Abstraction Layer n** (e.g., System-Level):
- Specification box with circular arrow (Explore alternative Solutions)
- Downward arrow labeled "Refinement"

**Abstraction Layer n+1** (e.g., Module-Level):
- Specification box with circular arrow (Explore alternative Solutions)

**Upward arrow**: "Abstraction" (feedback path)

**Concept**: Iterative process at each level, with refinement moving downward and potential abstraction moving back up

### 3. Specification-Exploration-Refinement with Evaluation
**Enhanced version** showing:
- **Evaluation/Verification box** spanning right side
- **Simulation Speed**: Increases going up (higher abstraction)
- **Simulation Accuracy**: Increases going down (lower abstraction)
- **Trade-off**: Fast but less accurate (high) vs. slow but accurate (low)

**Concept**: Design decisions must balance simulation speed for exploration with accuracy for validation

### 4. Top-Down Design Pyramid (Two Versions)
**Left Pyramid** (Careful Refinement):
- Top: "Idea"
- Triangle showing narrowing path through center (orange/blue/green)
- Bottom: "Possible implementations" (smaller green area)
- **Arrow path**: Stays centered, reaches optimal solution

**Right Pyramid** (Premature Commitment):
- Top: "Idea"
- Triangle with arrow veering to side early
- Bottom: "Possible implementations" (green area off to side, not optimal)

**Concept**: Keeping solution space open leads to better outcomes; early commitment may miss optimal solution

### 5. WCET Analysis Methods Graph
**Graph showing** (from Lecture 04 recap):
- **X-axis**: Real System, Simulation, Measurement, WCET Analysis
- **Y-axis**: Execution time range (Best-case to Worst-case)
- **Bars**: Showing uncertainty/range for each method
- **Concept**: Different methods have different accuracy and guarantees

### 6. Multi-Processor System with Task Binding
**Three sections**:
- **SW-Model**: Task graph (T1, T2, T3, T4, T5, T6, T7 with dependencies)
- **Binding**: Lines connecting tasks to processors
- **HW-Model**: 6 CPUs (4× Cortex-A53, 2× Cortex-R5)

**Shows**: Iterations of adding T7 and rebinding tasks

**Concept**: Software must be mapped to hardware (binding decisions)

### 7. Waterfall Model Diagram
**Structure**: Cascading boxes flowing downward:
1. Requirements / Analysis
2. Design
3. Implementation
4. Test
5. Operation

**Arrows**: One-way, downward only
**Concept**: Sequential, no feedback loops

### 8. V-Model Diagram
**V-Shape with two sides**:

**Left Side (Descending)**:
- Requirements Specification
- System Specification
- Architecture Design
- Module Design
- Module Implementation (bottom point)

**Right Side (Ascending)**:
- Unit/Module Test
- System Integration & Test
- System Validation
- Acceptance Test

**Horizontal Arrows**: Connect corresponding design and test levels
**Design Arrow**: Points down-left
**Test Arrow**: Points up-right
**Implementation Arrow**: Horizontal at bottom

**Concept**: Each design level has corresponding test level; testing bottom-up mirrors design top-down

### 9. Agile Sprint Cycle (Multiple Iterations)
**Structure**: Circular flow repeated multiple times:
- **Product Backlog** (stack of requirements)
- **Sprint Backlog** (selected items)
- **Sprint** (circle with inner cycle: Plan → Build → Test → Review)
- **Working increment** (deliverable)
- **Launch** (deployment)

**Timeline**: 24h daily cycle within 30-day sprint cycle
**Multiple sprints** shown in sequence
**Concept**: Iterative delivery with regular cycles

### 10. Scrum Framework Detail
**Circle divided into segments** (like clock):
- **Sprint Planning** (top)
- **Daily Scrum** (right)
- **Sprint Review** (bottom)
- **Sprint Retrospective** (left)

**Center**: Sprint (time-boxed iteration)
**Outside boxes**: Product Backlog, Sprint Backlog, Increment
**Concept**: Scrum ceremonies and artifacts organized around sprint

### 11. CI/CD Pipeline Workflow
**Components** (6 numbered steps):
1. **Developer workstation** → Check-In/Check-Out → **SW Repository**
2. **SW Repository** ← Check for Modifications ← **Build & Test Server**
3. **Build & Test Server**: Build & Test (cycle)
4. **Build & Test Server** → Notify Status → **Developer**
5. **Build & Test Server** → Publish Results → **Web Server**
6. **Web Server** → Read Reports → **Developer workstation**

**Color coding**:
- Red/Pink: Fully Automated zone (steps 2-5)
- Yellow: Human Interaction (steps 1, 6)

**Concept**: Automated feedback loop from commit to test results

### 12. CI/CD Pipeline Stages (Example Pipelines from GitLab)
**Three example pipelines shown**:

**Pipeline 1** (Simple):
- Tests: pylint, unittests (parallel)
- Build:doc: build_doc
- Build:release: build_exe
- Deploy: pages, pages:deploy

**Pipeline 2** (More Complex):
- Prebuild: doxygen
- Build: build_firmware, documentation
- Analysis: analyze, cppcheck
- Deploy: pages, pages:deploy

**Pipeline 3** (Single Stage):
- Build: application

**Concept**: Pipelines customizable to project needs, can run stages in parallel

### 13. Hardware-in-the-Loop CI/CD
**Image showing**:
- Multiple embedded boards (looks like Ultra96 or similar)
- Connected to cables
- Controlled by CI/CD server

**Concept**: Automated testing includes real hardware, not just simulation

## Formulas & Procedures

### Design Process Procedure (Iterative)
For each abstraction level:
1. **Specify** functionality at current level
2. **Explore** alternative implementations
3. **Evaluate** alternatives (simulation, analysis)
4. **Select** most promising solution
5. **Verify** selected solution meets requirements
6. **Refine** to next lower abstraction level
7. If verification fails, **abstract** back up and reconsider (iterate)
8. Repeat steps 1-7 at new lower level

**Continue** until reaching implementation level (device/machine code)

### Requirements Analysis to Design Mapping
**Requirements** → **System Specification** → **Architecture** → **Module Design** → **Implementation**

**Example** (Washing Machine):
- Requirement: "Door opens only when no water"
- System Spec: State machine with door states and water level states
- Architecture: MCU with water sensor input, door lock output
- Module Design: GPIO pin for sensor, PWM/relay for lock
- Implementation: C code reading GPIO, controlling output

### V-Model Test Planning Procedure
**For each design phase, define corresponding test**:

1. **Requirements Specification** → Define **Acceptance Test**:
   - What: Validate system meets user needs
   - Who: Customer/end-user
   - When: After system deployment
   - How: Real-world scenarios, user acceptance criteria

2. **System Specification** → Define **System Validation**:
   - What: Verify complete system behavior
   - Who: System test team
   - When: After integration
   - How: End-to-end scenarios, integration tests

3. **Architecture Design** → Define **System Integration & Test**:
   - What: Verify modules work together
   - Who: Integration test team
   - When: After module implementation
   - How: Interface tests, interaction scenarios

4. **Module Design** → Define **Unit/Module Test**:
   - What: Verify individual modules/components
   - Who: Developers
   - When: During/after implementation
   - How: Unit tests, code coverage

**Advantage**: Test planning concurrent with design ensures traceability

### Agile Sprint Procedure (Scrum)
**Sprint Duration**: Typically 2-4 weeks fixed length

**Sprint Planning** (Beginning):
1. Review product backlog with Product Owner
2. Select items for sprint (based on priority and capacity)
3. Create sprint backlog
4. Define sprint goal
5. Plan tasks for first few days

**Daily During Sprint**:
- **Daily Scrum** (15 minutes, same time/place):
  - What did I do yesterday?
  - What will I do today?
  - Any impediments?
- **Development Work**: Implement, test, integrate
- **Update Sprint Backlog**: Track progress

**Sprint Review** (End of sprint):
1. Demo completed work to stakeholders
2. Get feedback on increment
3. Update product backlog based on feedback
4. Discuss what's next

**Sprint Retrospective** (After review):
1. What went well?
2. What could improve?
3. What will we commit to improve next sprint?

**Then**: Start next sprint (immediately, no gap)

### CI/CD Setup Procedure
**Initial Setup**:
1. **Choose CI/CD Tool**: Jenkins, GitLab CI, GitHub Actions, etc.
2. **Set Up Build Server**: Dedicated machine or cloud service
3. **Connect to Version Control**: Configure repository integration
4. **Write Build Script**: Define how to compile code
5. **Write Test Scripts**: Automated tests to run
6. **Configure Triggers**: When to run (e.g., on every commit)
7. **Set Up Notifications**: Email, Slack, etc. for build status

**Pipeline Configuration** (example for embedded system):
```yaml
stages:
  - build
  - test
  - deploy

build_firmware:
  stage: build
  script:
    - make clean
    - make all
  artifacts:
    paths:
      - firmware.elf

unit_tests:
  stage: test
  script:
    - make test
    - ./run_tests

hardware_test:
  stage: test
  script:
    - flash_firmware.sh firmware.elf
    - run_hw_tests.sh
  tags:
    - hardware-runner

deploy_staging:
  stage: deploy
  script:
    - scp firmware.elf staging-server:/deploy/
  only:
    - main
```

**Maintenance**:
- Monitor build success rate
- Keep pipeline fast (< 10 minutes ideally)
- Fix broken builds immediately (don't commit on red)
- Gradually add more checks (static analysis, coverage)

## Comparisons & Contrasts

### Abstraction Levels: Higher vs. Lower

| Aspect | Higher Abstraction (System) | Lower Abstraction (Device) |
|--------|---------------------------|---------------------------|
| **Complexity Visible** | Low (details hidden) | High (all details exposed) |
| **Simulation Speed** | Fast (less to compute) | Slow (every transistor/cycle) |
| **Simulation Accuracy** | Lower (approximate) | Higher (exact behavior) |
| **Design Decisions** | Architectural choices | Implementation details |
| **Skills Required** | System architect, algorithm designer | Circuit designer, assembly programmer |
| **Tools** | UML, MATLAB, system simulators | SPICE, gate-level simulators, assemblers |
| **Changes Cost** | Low (easy to change) | High (expensive to change) |
| **Example** | "Washing machine control system" | "NAND gate with 3 transistors" |

### Waterfall vs. V-Model

| Aspect | Waterfall | V-Model |
|--------|-----------|---------|
| **Structure** | Sequential phases | V-shaped with paired design/test levels |
| **Testing** | Single test phase at end | Testing at each level |
| **Test Planning** | During test phase | During corresponding design phase |
| **Traceability** | Implicit | Explicit (design ↔ test mapping) |
| **Verification** | Late | Continuous |
| **Iteration** | Not formally supported | Possible within corresponding levels |
| **Use Case** | Simple projects | Complex, safety-critical systems |
| **Industry Adoption** | Declining | Widely used in embedded, automotive |

### Waterfall vs. Agile

| Aspect | Waterfall | Agile |
|--------|-----------|-------|
| **Approach** | Plan-driven, sequential | Adaptive, iterative |
| **Requirements** | Fixed early, changes difficult | Evolving, changes welcomed |
| **Delivery** | Single release at end | Frequent incremental releases |
| **Customer Involvement** | Primarily at start and end | Continuous throughout |
| **Documentation** | Comprehensive, formal | Lightweight, just enough |
| **Team Structure** | Hierarchical roles | Self-organizing teams |
| **Planning** | Detailed up-front | Just-in-time, adaptive |
| **Risk Management** | Front-loaded | Continuous throughout |
| **Predictability** | High (schedule, cost) | Lower (scope flexible) |
| **Quality Approach** | Testing phase at end | Continuous testing and integration |
| **Suitable For** | Stable requirements, regulated | Evolving requirements, innovation |
| **Example Domain** | Hardware, aerospace | Web apps, mobile apps |

### Scrum vs. Extreme Programming (XP)

| Aspect | Scrum | Extreme Programming |
|--------|-------|-------------------|
| **Focus** | Project management | Engineering practices |
| **Roles** | Prescribed (PO, SM, Team) | Not prescribed |
| **Sprint Length** | 2-4 weeks | 1-2 weeks (typically) |
| **Iteration Goal** | Potentially shippable increment | Working software |
| **Engineering Practices** | Not specified | Heavily specified (TDD, pair programming, etc.) |
| **Testing** | Not prescribed | TDD mandatory |
| **Design** | Emergent | Simple design, refactoring |
| **Collaboration** | Daily stand-ups | Pair programming (constant) |
| **Can Combine** | Yes - Scrum for process, XP practices for quality |

### CI vs. CD

| Aspect | Continuous Integration (CI) | Continuous Deployment (CD) |
|--------|---------------------------|---------------------------|
| **Scope** | Build and test | Build, test, and deploy |
| **Frequency** | Every commit | Every successful build |
| **Goal** | Catch integration issues early | Deliver value to users quickly |
| **Risk** | Low (code stays in repo) | Higher (code goes to production) |
| **Prerequisites** | Automated build, automated tests | CI + deployment automation + high test coverage |
| **Team Confidence Needed** | Moderate | High |
| **Typical For** | All software projects | Web services, SaaS |
| **Embedded Systems** | Very common | Less common (firmware updates risky) |

### Development Process Selection Criteria

| Criterion | Waterfall/V-Model | Agile |
|-----------|------------------|-------|
| **Requirements Stability** | Stable, well-defined | Evolving, unclear |
| **Domain Maturity** | Mature, proven | New, innovative |
| **Team Size** | Large (100+) | Small to medium (<50) |
| **Team Location** | Distributed | Co-located (ideally) |
| **Customer Availability** | Limited | High |
| **Regulatory** | Heavily regulated | Less regulated |
| **Hardware/Software** | Hardware-intensive | Software-intensive |
| **Timeline** | Long (years) | Short (months) |
| **Budget** | Fixed | Flexible |
| **Contract Type** | Fixed-price | Time & materials |

## Examples & Applications

### Example 1: Washing Machine at Different Abstraction Levels

**System Level**:
- Control system with sensors (water level, door, temperature, buttons) and actuators (motor, pump, valve, heater, lock)
- State machine defines overall behavior (Lab U01)
- Focus: What does system do?

**Module Level**:
- MCU (e.g., STM32), sensors connected to GPIO/ADC, actuators via GPIO/PWM/relays
- Software modules: state machine handler, sensor drivers, actuator drivers, timer management
- Focus: What components, how they interact?

**Block Level**:
- MCU internal: GPIOs, timers, ADC channels, PWM units
- Software: Functions (read_water_level(), control_motor(), state_machine_step())
- Program loops, interrupt handlers
- Focus: Internal structure of modules?

**Logic Level**:
- Assembly instructions: `LDR R0, [R1]` to read GPIO, `STR R2, [R3]` to write PWM
- Digital logic for GPIO pins (D flip-flops, combinational logic)
- Focus: Exact instructions, gate connections?

**Device Level**:
- Transistors in GPIO output driver
- Machine code: `0xE5910000` (binary encoding of LDR)
- Focus: Physical implementation (almost never need this for application development)

### Example 2: Top-Down Design of Traffic Light System

**Starting Point**: "Safe and efficient traffic light control"

**System Level - Exploration**:
- **Alternative 1**: Fixed timing (simple, predictable)
- **Alternative 2**: Traffic-adaptive (efficient but complex)
- **Alternative 3**: Pedestrian-activated (balances efficiency and safety)
- **Selected**: Fixed timing with pedestrian buttons (good balance)

**Module Level - Exploration**:
- **Alternative 1**: One MCU per direction (distributed, redundant)
- **Alternative 2**: Central MCU controlling all lights (simpler, cheaper)
- **Alternative 3**: PLC-based (industrial-grade, expensive)
- **Selected**: Central MCU (cost-effective for this scale)

**Block Level - Exploration**:
- **Alternative 1**: State machine in software (flexible)
- **Alternative 2**: Ladder logic (traditional for traffic control)
- **Selected**: Software state machine (easier to modify)

**Result**: Systematic exploration led to optimal solution; if we'd committed to distributed MCUs early, couldn't have selected cheaper central approach later

### Example 3: V-Model for Embedded System (Drone)

**Left Side (Design)**:

**Requirements**: "Drone SHALL maintain altitude within ±1m when hovering"
→ **Defines Acceptance Test**: Measure actual altitude variation during 5-minute hover

**System Specification**: "Altitude control loop with barometer, PID controller, motor control"
→ **Defines System Validation**: Test complete flight control system in simulation and flight

**Architecture**: "Barometer → I2C → MCU → PWM → ESCs → Motors"
→ **Defines Integration Test**: Verify sensor readings reach motor control correctly

**Module Design**: "PID controller: Kp=1.5, Ki=0.1, Kd=0.05"
→ **Defines Unit Test**: Test PID algorithm with various inputs, verify output correctness

**Implementation**: Write C code for PID, sensor drivers, etc.

**Right Side (Verification)**:

**Unit Test**: Pass test vectors through PID function, verify math correct
→ Result: PID algorithm works

**Integration Test**: Sensor connected, verify PID receives correct altitude, motors respond
→ Result: Components communicate correctly

**System Validation**: Fly drone in controlled environment, log altitude
→ Result: System behavior matches specification

**Acceptance Test**: Customer flies drone in real conditions, measure altitude stability
→ Result: Meets ±1m requirement

### Example 4: Agile Sprint for Embedded Web Server Feature

**Sprint Goal**: "Add HTTPS support to embedded web server"

**Sprint Planning**:
- Selected user stories from product backlog:
  - "As a user, I want secure login" (priority 1)
  - "As admin, I need encrypted config transfer" (priority 2)
- Broke down into tasks:
  - Integrate mbedTLS library
  - Generate/store certificates
  - Modify HTTP server for HTTPS
  - Update login page
  - Test with various browsers

**Daily Work**:
- Day 1-2: mbedTLS integration, first Daily Scrum reveals memory constraint issue
- Day 3-4: Optimize memory usage, team decides to drop TLS 1.3 for now (1.2 sufficient)
- Day 5-7: Certificate handling
- Day 8-10: HTTPS server modifications
- Day 11-12: Browser testing, found issue with Safari - fixed
- Day 13: Buffer day for unexpected issues
- Day 14: Sprint review preparation

**Sprint Review**:
- Demo: Show HTTPS connection with lock icon, encrypted login
- Feedback: Customer wants client certificate authentication (add to backlog)
- Retrospective: Memory issue caught early (good), should have researched TLS versions earlier (improve)

**Result**: Working HTTPS feature delivered in 2 weeks, adaptations made during sprint

### Example 5: CI/CD for Multi-Target Embedded System

**Setup**:
- Product: IoT sensor node with multiple MCU options (STM32, NRF52, ESP32)
- Repository: Git with main branch and feature branches
- CI Server: GitLab CI with custom runners

**Pipeline on Each Commit**:

1. **Build Stage** (parallel):
   - Build for STM32: `arm-none-eabi-gcc` cross-compile
   - Build for NRF52: Different compiler flags
   - Build for ESP32: ESP-IDF toolchain
   - Generate: Binary files for each target

2. **Test Stage** (parallel):
   - Unit tests: Host-based tests (x86 simulation)
   - Static analysis: cppcheck, clang-tidy
   - Code coverage: Measure test coverage
   - Memory analysis: Check code size vs. flash/RAM budgets

3. **Hardware Test Stage** (parallel):
   - STM32 runner: Flash to STM32 board, run integration tests
   - NRF52 runner: Flash to NRF52 board, test BLE functionality
   - ESP32 runner: Flash to ESP32 board, test WiFi
   - Tests: Serial communication, automated test scripts

4. **Documentation Stage**:
   - Generate Doxygen from code comments
   - Build user manual from Markdown
   - Publish to internal wiki

**Result**:
- Full pipeline: 15 minutes
- Developer knows within 15 minutes if commit broke any target
- Nightly builds: Extended tests (power consumption, stress tests)
- Hardware-in-the-loop catches target-specific bugs immediately

**Example Issue Caught**:
- Commit changed timer configuration
- Built successfully for all targets
- Hardware test on NRF52 failed (GPIO timing violated spec)
- Developer notified, fixed within an hour
- Without CI/CD: Would have discovered when customer tested, days/weeks later

### Example 6: Hybrid Approach (V-Model + Agile + CI/CD)

**Project**: Medical infusion pump (safety-critical)

**Overall Structure**: V-Model (regulatory requirement)
- Requirements phase: 3 months, formal specification
- Design phase: 2 months, architecture and module design
- Implementation phase: 6 months (using Agile)
- Test phase: 4 months, formal V-model validation

**Within Implementation Phase**: Agile Sprints
- 2-week sprints
- Each sprint delivers tested module increment
- Daily stand-ups for team coordination
- Sprint reviews with broader stakeholder group

**Throughout**: CI/CD
- Every commit triggers build and unit tests
- Nightly: Full regression test suite
- Weekly: Hardware-in-the-loop tests on actual pump hardware
- Documentation auto-generated

**V-Model Documentation**:
- Requirements traceability matrix maintained
- Each sprint updates design documents
- Test reports generated from CI/CD results
- Formal review gates at V-model phase boundaries

**Benefits of Hybrid**:
- V-Model: Satisfies regulatory documentation requirements (FDA, CE mark)
- Agile: Enables adaptive implementation, team collaboration
- CI/CD: Continuous quality assurance, fast feedback
- Result: Compliant product delivered efficiently

## Connections to Other Topics

### Connection to Lecture 01: Embedded Systems
- **Design Space**: Development processes explore cost-performance-energy triangle (Lecture 01 concept)
- **Cyber-Physical Systems**: Development must consider physical constraints (hardware limits iteration like agile)
- **Real-Time**: Timing requirements drive verification approach (measurement, WCET analysis)
- **Resource Constraints**: Abstraction levels help manage limited memory, processing power

### Connection to Lecture 02: Software Paradigms
- **Event-Driven Paradigm**: Agile sprints driven by events (sprint planning, review)
- **Time-Driven Development**: Waterfall is time-driven (phases have durations)
- **Object-Oriented Design**: Supports modularity needed for V-Model module testing
- **TDD Approach**: Extreme Programming's test-first aligns with paradigm of testing as design tool

### Connection to Lecture 03: Requirements
- **Requirements Engineering**: First phase of Waterfall and V-Model
- **Agile Requirements**: User stories vs. formal requirements specification
- **RFC 2119 Keywords**: Used in requirements specification phase
- **Traceability**: V-Model ensures requirements trace through design to tests
- **Verification**: Model checking (Lecture 03/04) fits into verification phase of V-Model

### Connection to Lecture 04: Modelling
- **UML State Machines**: Created during design phase (Waterfall/V-Model) or sprint (Agile)
- **Model-Based Code Generation**: Can fit into CI/CD pipeline (model→code→build→test)
- **Model Checking**: Verification method used at various abstraction levels
- **Specification**: Formalization step in design process
- **Exploration**: Model alternatives before code implementation

### Connection to Lab U01: Requirements Modeling
- **Requirements Iteration**: Peer review → improvement is mini-agile cycle
- **State Machine Development**: Would occur in design phase (Waterfall/V-Model) or during sprint (Agile)
- **Washing Machine Example**: Shows specification at system level (top-down design)
- **Testing**: State machine validated against requirements (V-Model approach)

### Connection to Future Lecture 06: Non-Functional Requirements
- **V-Model Verification**: NFRs verified at system validation level
- **Performance Testing**: Can be automated in CI/CD pipeline
- **Trade-offs**: Exploration step evaluates NFR trade-offs (energy vs. performance)
- **Agile NFRs**: Non-functional requirements may be harder to manage in agile (need continuous attention)

### Connection to Future Lecture 08: Energy
- **Design Space Exploration**: Energy is one axis (with cost, performance)
- **Module Level Decisions**: Processor selection impacts energy (happens at module abstraction level)
- **CI/CD Measurement**: Energy consumption can be measured in hardware-in-the-loop tests
- **Iterative Optimization**: Agile sprints can focus on energy optimization

### Connection to Future Lecture 09: Performance
- **WCET Analysis**: Performed during design/verification (static analysis in CI/CD)
- **Performance Testing**: Automated in CI/CD pipeline (regression detection)
- **Abstraction Levels**: Performance estimated at high levels, measured at low levels
- **Optimization Sprints**: Agile can dedicate sprints to performance improvement

### Connection to Future Lecture 10-12: FPGA and DSE
- **Hardware Development**: More waterfall-like (physical constraints)
- **DSE**: Exploration step in design process (evaluate hardware alternatives)
- **Co-Design**: Hardware and software developed in parallel (needs coordination process)
- **Verification**: FPGA simulation at multiple abstraction levels

### Connection to Future Lecture 13: RTOS Scheduling
- **Integration Testing**: RTOS behavior verified in system integration phase (V-Model)
- **Real-Time Requirements**: Drive verification approach (timing measurements critical)
- **Task Development**: Tasks implemented in implementation phase, tested in unit tests

## Summary

Lecture 05 covers **development processes** for embedded systems, bridging from initial idea to final product. The development journey involves managing **abstraction levels** (System → Module → Block → Logic → Device) for both hardware and software, with each level hiding details while enabling design at appropriate granularity.

The **design process** iterates through **specification** (defining functionality), **exploration** (evaluating alternatives), and **refinement** (moving to next lower abstraction level). **Top-down design** keeps solution space open, enabling optimal solutions by making commitments gradually.

**Verification and validation** occur throughout development using model checking, analysis, simulation, and measurement. Higher abstraction levels enable faster but less accurate simulation; lower levels are slower but more accurate.

**Waterfall model** organizes development as sequential phases (Requirements → Design → Implementation → Test → Operation). Well-suited for stable requirements and hardware development, but inflexible for changes. **V-Model** extends waterfall by adding explicit verification at each design level, creating traceability from requirements through tests.

**Agile development** (Scrum, XP, etc.) takes iterative, incremental approach with short sprints delivering working increments. Emphasizes adaptability, customer collaboration, and continuous improvement. Better suited for evolving requirements and software-intensive projects, but challenges include predictability and scalability.

**CI/CD (Continuous Integration/Continuous Deployment)** automates build, test, and deployment, providing fast feedback on every code commit. Critical for modern development regardless of overall process (works with V-Model or Agile). Can include hardware-in-the-loop testing for embedded systems.

**No single process fits all projects** - selection depends on requirements stability, hardware/software mix, team size, regulatory constraints, and organizational culture. Many organizations use **hybrid approaches** (e.g., V-Model for hardware, Agile for software, CI/CD throughout).

## Possible Exam Questions

### Basic Definitions and Concepts (10 questions)

1. **Define "abstraction" in the context of embedded systems development. Why is it important?**
   - Answer: Hiding implementation details while exposing essential features at higher conceptual level. Important for managing complexity, enabling communication across disciplines, and supporting iterative refinement.

2. **What are the three core activities in the design process at each abstraction level?**
   - Answer: Specification (define functionality), Exploration (evaluate alternatives), Refinement (move to next lower abstraction level).

3. **Define the Waterfall model. What are its five main phases?**
   - Answer: Sequential development process flowing through distinct phases. Phases: Requirements/Analysis, Design, Implementation, Test, Operation.

4. **What is the key difference between the Waterfall model and the V-Model?**
   - Answer: V-Model adds explicit verification/testing at each design level, with tests defined during corresponding design phase. Creates traceability between design and testing.

5. **Define "sprint" in agile development context.**
   - Answer: Fixed time period (typically 2-4 weeks) for completing specific work, delivering potentially shippable product increment at end.

6. **What does CI/CD stand for? Briefly explain each.**
   - Answer: Continuous Integration / Continuous Deployment. CI: Automatically building and testing code after each commit. CD: Automatically deploying successful builds to target environments.

7. **What is the "System Level" abstraction? Give examples for hardware and software.**
   - Answer: Highest abstraction level. Hardware: components and subsystems. Software: tasks describing system functionality.

8. **Define "top-down design approach."**
   - Answer: Starting from high-level idea/requirements and progressively adding detail through refinement to lower abstraction levels. Keeps solution space open as long as possible.

9. **What is "hardware-in-the-loop" (HIL) testing?**
   - Answer: Automated testing with actual hardware in CI/CD pipeline, catching hardware-specific issues (timing, peripherals) that simulation might miss.

10. **Define "product backlog" in Scrum.**
    - Answer: Prioritized list of features/requirements for the product, maintained by Product Owner, source for sprint planning.

### Conceptual Understanding (12 questions)

11. **Explain why higher abstraction levels enable faster simulation but lower accuracy.**
    - Answer: Higher levels hide details (fewer things to compute/simulate), so simulation runs faster. But hiding details means approximations, reducing accuracy. Lower levels model every transistor/instruction (accurate) but computationally expensive (slow).

12. **Why is the exploration step important in the design process? What happens if you skip it?**
    - Answer: Exploration evaluates alternative implementations to find optimal solution. Skipping it means committing to first solution thought of, likely suboptimal. May miss better cost/performance/energy trade-offs.

13. **Explain the "pyramid" concept of top-down design. Why is keeping solution space open beneficial?**
    - Answer: Pyramid wide at top (many possible implementations), narrow at bottom (specific implementation). Keeping solution space open (staying centered in pyramid) allows systematic exploration. Early commitment (veering to side) may miss optimal path discovered later.

14. **How does the V-Model improve upon the Waterfall model's weaknesses?**
    - Answer: Addresses late testing issue by defining tests during design (earlier defect detection). Creates explicit traceability (requirements to tests). Enables iteration within corresponding levels. Still sequential overall but better verification.

15. **Why is Waterfall better suited for hardware development than software development?**
    - Answer: Hardware has physical constraints (can't easily iterate once manufactured), stable requirements (hard to change circuit boards), and well-understood technology. Software more flexible, easier to change, requirements often evolve.

16. **Explain how agile methods handle changing requirements differently than Waterfall.**
    - Answer: Agile welcomes changes, adapts in each sprint based on feedback. Product backlog continuously reprioritized. Waterfall locks requirements early, changes expensive/difficult after design phase. Agile delivers increments (can course-correct), Waterfall delivers at end (too late to change).

17. **What role does CI/CD play in both Waterfall/V-Model and Agile processes?**
    - Answer: Provides fast feedback regardless of overall process. In V-Model: automates unit/integration/system tests at each level. In Agile: enables continuous delivery within sprints. Both benefit from early defect detection, automated regression testing.

18. **Why might formal documentation be important even when using Agile methods?**
    - Answer: Regulated industries (medical, automotive, aerospace) require traceability for certification. Long-term maintenance needs documentation. Knowledge transfer when team changes. Safety-critical systems need proof of verification.

19. **Explain the trade-off between simulation speed and accuracy at different abstraction levels.**
    - Answer: Fundamental trade-off: detail vs. performance. High abstraction (fast simulation) enables broad exploration, catching architectural issues. Low abstraction (accurate simulation) validates implementation, catching timing/hardware issues. Need both perspectives at appropriate stages.

20. **How does the iterative nature of agile align with the exploration step in the design process?**
    - Answer: Both involve trying alternatives and learning. Each sprint explores implementation approach, gets feedback, adapts next sprint. Similar to design exploration at abstraction level. Agile makes this explicit at implementation level.

21. **Why is test-driven development (TDD) considered valuable in Extreme Programming?**
    - Answer: Tests define expected behavior before implementation (specification). Immediate feedback when code breaks tests. High test coverage catches regressions. Forces modular design (testable code). Tests serve as documentation.

22. **What makes CI/CD more challenging for embedded systems compared to pure software?**
    - Answer: Need cross-compilation (multiple target architectures), hardware access (boards, fixtures), firmware flashing, serial console capture, timing-sensitive tests, power control. Can't just run tests on build server CPU.

### Application and Analysis (15 questions)

23. **At what abstraction level would you make the decision to use an ARM Cortex-A53 vs. Cortex-R5 processor? Justify.**
    - Answer: Module level. Processors, MCUs are module-level components. System level would decide "need processor", module level selects specific processor based on performance/cost/real-time requirements.

24. **A project uses Waterfall and discovers during testing that a fundamental requirement was misunderstood. What are the implications?**
    - Answer: Major problem: must go back to requirements phase, redo design and implementation. Extremely expensive and time-consuming. Delivery delayed significantly. Could have been caught with incremental delivery (agile) or early prototyping.

25. **Design a V-Model verification strategy for the washing machine from Lab U01. Map requirements to test levels.**
    - Answer: Requirements: "Door opens only when no water" → Acceptance Test: User tries opening door in various scenarios. System Spec: State machine transitions → System Validation: Test all state machine paths. Architecture: MCU with sensors/actuators → Integration Test: Verify sensor readings trigger correct actuator responses. Module: Door lock function → Unit Test: Call function with various inputs, verify output.

26. **Compare development approaches for two projects: (A) New innovative IoT product with uncertain market, (B) Safety-critical avionics system. Which process for each?**
    - Answer: (A) Agile: Requirements uncertain, need fast iteration, customer feedback critical, software-intensive, speed to market important. (B) Waterfall/V-Model: Requirements well-defined (aviation standards), safety-critical (need formal verification), heavily regulated (documentation required), hardware-intensive.

27. **Your CI/CD pipeline takes 2 hours to run. Developers complain it's too slow. Propose optimizations.**
    - Answer: (1) Parallelize stages (build targets simultaneously). (2) Incremental builds (only rebuild changed parts). (3) Tiered testing (fast unit tests on every commit, slow integration tests nightly). (4) Hardware test pool (multiple boards, not sequential). (5) Cache dependencies. (6) Optimize test suite (remove redundant tests).

28. **A Scrum team consistently fails to complete sprint commitments. Analyze possible causes and solutions.**
    - Answer: Causes: (1) Over-committing (too many stories), (2) Unclear definitions of done, (3) External interruptions, (4) Technical debt slowing development, (5) Poor task breakdown. Solutions: (1) Better velocity estimation, (2) Clear acceptance criteria, (3) Protect sprint time, (4) Allocate time for refactoring, (5) Improve sprint planning.

29. **Sketch a CI/CD pipeline for a multi-target embedded system (3 different MCUs). What stages would you include?**
    - Answer: Stages: (1) Build (parallel for each MCU), (2) Unit Test (host-based simulation), (3) Static Analysis (cppcheck, memory check), (4) Hardware Test (parallel, one runner per MCU with actual boards), (5) Documentation (Doxygen), (6) Package (create release binaries). Notifications on failure. Artifacts: binaries for each target.

30. **How would you integrate model-based code generation (Lecture 04) into a V-Model process?**
    - Answer: Design phase: Create UML models (state machines, etc.). Module Design: Refine models with implementation details. Implementation: Generate code from models (automated). Unit Test: Test generated code functions. System Integration: Test model behavior with other components. Validation: Verify model matches requirements. CI/CD: Automate model→code→test.

31. **A company develops embedded firmware using Waterfall but wants faster customer feedback. Propose a hybrid approach.**
    - Answer: Hybrid "Water-Scrum-Fall": (1) Requirements phase (Waterfall): Formal specification (2-3 months). (2) Architecture design (Waterfall): High-level design (1 month). (3) Implementation (Agile): 2-week sprints with incremental delivery, customer demos each sprint (6 months). (4) Validation (Waterfall): Formal testing and documentation (2 months). Provides customer feedback during implementation while maintaining formal structure.

32. **You're verifying a real-time system. Explain which verification methods (model checking, simulation, measurement) you'd use and when.**
    - Answer: Model checking: Early (design phase) to verify state machine properties (deadlock-free, always eventually reaches state). Simulation: Implementation phase to estimate timing, test scenarios before hardware. Measurement: After implementation on actual hardware to verify WCET, catch hardware-specific issues. Use all three: each catches different issues at appropriate stages.

33. **Design a sprint structure for an embedded systems team that must maintain 80/20 split between new features and technical debt.**
    - Answer: Each 2-week sprint: (1) Sprint planning: Select user stories (60-70% capacity), allocate remaining 30-40% to tech debt. (2) Tech debt backlog: Maintained separately, items pulled into sprint. (3) Definition of done: Includes "no new static analysis warnings" to prevent debt accumulation. (4) Retrospective: Review debt trends, adjust ratio if needed. Track debt metrics in CI/CD.

34. **A medical device company must use V-Model for certification but has software team experienced in Agile. How can they work together?**
    - Answer: V-Model for structure, Agile for implementation: (1) V-Model phases define formal deliverables (requirements doc, design doc, test plan). (2) Within implementation phase: Agile sprints deliver modules incrementally. (3) Documentation updated each sprint (not deferred to end). (4) Formal review gates between V-Model phases. (5) CI/CD generates traceability matrix automatically. (6) Sprint reviews include V-Model stakeholders.

35. **Analyze: A team completes all unit tests but system integration fails. What might have gone wrong? How could process prevent this?**
    - Answer: Issues: (1) Interface mismatches not caught by unit tests, (2) Integration assumptions wrong, (3) Timing issues only apparent when combined, (4) Resource conflicts (memory, interrupts). Prevention: (1) V-Model: Design integration tests during architecture phase, (2) CI/CD: Continuous integration (integrate frequently, not at end), (3) Mocking: Unit tests should include interface tests, (4) Hardware-in-loop: Test on actual platform early.

36. **Compare abstraction vs. refinement in the design process. When would you move up (abstract) vs. down (refine)?**
    - Answer: Refinement (down): Normal progression, adding detail, when current level verified and ready to implement. Abstraction (up): When verification fails at lower level (issue with chosen approach), need to reconsider higher-level decision. Example: Refine when state machine correct, implement functions. Abstract when module tests show performance issue, revisit architecture (maybe need faster processor).

37. **Your CI/CD pipeline passes all tests but customers report critical bugs. What's wrong with your process?**
    - Answer: Issues: (1) Tests don't cover real usage scenarios, (2) Missing hardware-in-loop tests (only simulation), (3) Environment differences (customer hardware vs. test setup), (4) Performance/stress testing inadequate, (5) Non-functional requirements not tested. Fixes: (1) Add scenario-based tests, (2) Customer environment replication, (3) Hardware-in-loop with various boards, (4) Beta testing program, (5) Improve test coverage metrics.

### Critical Thinking and Synthesis (10 questions)

38. **Critique: "Agile doesn't work for embedded systems because hardware can't iterate quickly." Discuss with nuance.**
    - Answer: Partially true but oversimplified. Hardware less iterative (can't change PCB easily), but software on embedded systems CAN iterate. Hybrid approaches work: hardware using V-Model (stable architecture), software using Agile (firmware evolves). Early hardware prototypes enable software iteration. FPGA/dev boards allow experimentation. Challenge: need hardware earlier, but agile principles (feedback, adaptation) still valuable for embedded software.

39. **Analyze the claim: "CI/CD is just automated testing - nothing new." What's missing in this view?**
    - Answer: Too narrow. CI/CD is broader: (1) Culture shift (fast feedback, continuous integration vs. big-bang), (2) Process change (commit small changes frequently), (3) Quality gates (blocks merging if tests fail), (4) Deployment automation (not just testing), (5) Monitoring and metrics (track trends), (6) Documentation generation, (7) Build artifacts management. Testing is component, but paradigm is continuous quality assurance.

40. **Design a development process for a startup building novel embedded hardware with unproven technology. Justify your choices.**
    - Answer: Hybrid process: (1) Initial exploration (3-4 sprints): Rapid prototyping with dev boards, spike solutions, fail fast. (2) Architecture phase (1 month): Once approach proven, formal design (partial V-Model). (3) Hardware development (Waterfall): PCB design, manufacturing (can't iterate). (4) Software development (Agile): 2-week sprints while waiting for hardware. (5) Integration (V-Model): Formal testing. (6) CI/CD throughout: Fast feedback. Rationale: Unproven tech needs exploration (agile), hardware needs planning (waterfall), continuous testing (CI/CD).

41. **How does the choice of development process affect team structure, skill requirements, and communication patterns?**
    - Answer: Waterfall: Hierarchical teams, specialists (requirements analysts, designers, coders, testers), sequential handoffs, formal documentation communication. Agile: Cross-functional teams, generalists (everyone codes/tests), constant collaboration (daily standups), informal communication. V-Model: Hybrid (specialists but paired design-test). CI/CD: Requires DevOps skills (automation, scripting). Process shapes organization; Conway's Law: "organizations design systems that mirror their communication structure."

42. **Propose a verification strategy that combines model checking, simulation, and measurement. When would you use each?**
    - Answer: Multi-level verification: (1) Model Checking (design phase): Verify state machine properties (deadlock, liveness) - mathematical proof. (2) High-level Simulation (architecture): Estimate performance, evaluate alternatives - fast exploration. (3) Detailed Simulation (module design): Timing analysis, WCET estimation - more accurate. (4) Measurement (implementation): Actual hardware tests, validation - ground truth. Each catches different issues: model checking finds logical errors, simulation catches design issues, measurement reveals implementation bugs. Use progressively as design refines.

43. **Analyze: Why do many embedded systems projects still use Waterfall/V-Model despite Agile's popularity in web/mobile development?**
    - Answer: Fundamental differences: (1) Hardware constraints (can't iterate PCBs rapidly), (2) Real-time requirements (need upfront architecture for timing), (3) Safety critical (regulations require formal documentation), (4) Certification costs (changing design expensive after certification testing), (5) Long product lifecycles (decades vs. months, need maintainability docs), (6) Physical manufacturing (tooling, inventory), (7) Team distribution (hardware/firmware/mechanical in different locations). Waterfall/V-Model better fits these constraints. Agile thrives with pure software flexibility.

44. **How might AI/ML change embedded systems development processes in the future?**
    - Answer: Potential impacts: (1) Design exploration: AI explores design space faster (DSE), (2) Code generation: AI generates embedded code from specifications, (3) Testing: AI generates test cases, predicts failure modes, (4) Verification: AI-assisted formal verification, (5) Process adaptation: AI recommends process based on project characteristics, (6) Bug prediction: AI flags risky code areas, (7) Requirements analysis: AI extracts requirements from natural language. Challenges: Safety-critical systems need explainable AI, verification of AI-generated code, regulatory acceptance.

45. **Compare hardware-software co-design challenges when using Waterfall vs. Agile. Which process handles this better?**
    - Answer: Co-design challenges: Hardware and software interdependent (SW needs HW platform, HW capabilities driven by SW needs). Waterfall: Requires accurate up-front specification (interface freeze), hardware developed first (long lead time), software waits. Risk: Late discovery of HW/SW mismatch. Agile: Difficult because hardware can't iterate rapidly. Solution: Hybrid - V-Model for hardware (stable architecture), Agile for software using emulation/dev boards until hardware ready. Early hardware prototypes critical. Neither process perfect; co-design inherently challenging.

46. **Evaluate the statement: "The best development process is no process - just let good engineers build things."**
    - Answer: Flawed for anything beyond trivial projects. Problems without process: (1) Unclear requirements (engineers guess what customers want), (2) No coordination (parts don't integrate), (3) No quality assurance (untested), (4) Knowledge silos (only original developer understands), (5) Unpredictable (no schedule/budget control), (6) Not scalable (chaos with >5 people). Good engineers need process to collaborate effectively, manage complexity, ensure quality. Process provides structure for communication, verification, documentation. "No process" IS a process - just a bad one.

47. **Design a process for maintaining legacy embedded systems (20+ years old) while adding new features. What special considerations?**
    - Answer: Challenges: Old toolchains, undocumented, fragile, team knowledge lost. Process: (1) Archaeology phase: Understand existing system (reverse engineering), document architecture. (2) Test harness creation: Build automated tests for current behavior (regression prevention). (3) CI/CD setup: Automate build (even if old tools), testing. (4) Incremental modernization: Small changes, heavy testing. (5) Risk management: Change carefully (medical devices, infrastructure). (6) Parallel development: New features on modern platform, migrate gradually. (7) Documentation as you go: Capture knowledge. Cannot use pure agile (too risky) or pure waterfall (too slow). Hybrid with emphasis on safety.

### Scenario-Based Questions (10 questions)

48. **You're leading development of an automotive ECU (safety-critical, heavily regulated). Which development process and why? Describe key activities.**
    - Answer: V-Model (automotive standard ASPICE requires V-model-like process): (1) Requirements: Functional safety requirements (ISO 26262), stakeholder requirements. (2) System Specification: Safety architecture, FMEA analysis. (3) Architecture Design: Hardware selection (MCU), software modules, AUTOSAR architecture. (4) Module Design: Detailed design, coding rules (MISRA-C). (5) Implementation: Coding, peer review, static analysis. (6) Unit Test: 100% coverage target. (7) Integration Test: Module interaction, ECU-level. (8) System Validation: HIL testing, vehicle integration. (9) Acceptance: Functional safety audit. CI/CD: Automated builds, static analysis, unit tests. Documentation for certification.

49. **A sprint review reveals customers want a completely different feature than what was demoed. How does Agile handle this? What if using Waterfall?**
    - Answer: Agile: Product Owner updates product backlog, reprioritizes, new feature added for future sprints. Current increment may be discarded (sunk cost, but learned customer needs). Team adapts next sprint. Customer engaged continuously, so this should be rare (but process handles it). Waterfall: Disaster - requirements locked, design based on wrong feature. Change requires going back to requirements phase (months of rework), massive cost overrun, schedule delay. May require contract renegotiation. Illustrates agile's "welcome changing requirements" vs. waterfall's rigidity.

50. **Design a CI/CD pipeline for firmware that must be certified (e.g., medical device). How do you balance automation with regulatory requirements?**
    - Answer: Pipeline stages: (1) Build (automated), (2) Unit Test (automated, 100% coverage required), (3) Static Analysis (automated, MISRA-C compliance), (4) Documentation Generation (automated), (5) Traceability Matrix (automated - requirements to code to tests), (6) Integration Test (automated on HIL), (7) Gate (manual review for regulatory submission), (8) Package (automated - creates submission bundle). All test results stored for audit. Manual review doesn't redo testing (trusts automation), but validates completeness for submission. CI/CD speeds development while maintaining regulatory rigor. Key: automation of evidence collection, not replacement of regulation.

51. **Your embedded team transitions from Waterfall to Agile. 6 months later, project behind schedule, quality issues. Diagnose and fix.**
    - Answer: Common transition mistakes: (1) "Agile" without discipline (no retrospectives, poor sprint planning), (2) No automated testing (technical debt accumulates), (3) Missing CI/CD (integration problems), (4) Unclear definition of done, (5) Too much WIP (starting many features, finishing few), (6) No product owner (priorities unclear), (7) Forgetting hardware constraints (can't iterate PCB). Diagnosis: Review metrics (velocity, defect rate, test coverage). Fixes: (1) Agile coaching/training, (2) Set up CI/CD, (3) Define "done" including tests, (4) Limit WIP, (5) Proper product backlog management, (6) Retrospectives to improve process. May need hybrid approach if pure agile mismatch.

52. **Compare two scenarios: (A) Fixed-price contract for embedded system, (B) Internal R&D project. Which development process for each and why?**
    - Answer: (A) Fixed-price: Waterfall/V-Model. Reasons: Scope must be locked (price based on fixed requirements), customer expects upfront schedule/cost estimate, contractual obligation to deliver specified system, change orders costly. Process provides predictability, formal milestones for payment. (B) Internal R&D: Agile. Reasons: Requirements uncertain (exploring possibilities), can pivot based on findings, budget flexible, no contract (internal stakeholders), learning-focused (fail fast acceptable), shorter feedback cycles valuable. Process enables exploration, adaptation. Fixed-price contracts and agile fundamentally clash (scope vs. time/cost trade-off).

53. **Your CI/CD pipeline detects a broken build at 3 AM (developer in different timezone committed). Design a process to handle this efficiently.**
    - Answer: Process: (1) Immediate notification (email, Slack, SMS to committer), (2) Automatic rollback (revert commit if breaks main branch), (3) Block further commits to main until fixed (branch protection), (4) Create issue ticket automatically (with build logs, failed tests), (5) Committer fixes on their time (if working) or first thing next day, (6) Culture rule: "You break it, you fix it" (responsibility), (7) Parallel work continues on feature branches (not blocked). Prevention: (1) Pre-commit hooks run tests locally, (2) Pull request requires passing CI before merge, (3) Staging branch (test before main). Goal: Protect main branch quality without waking people unnecessarily.

54. **Design a retrospective format for embedded systems team that includes both hardware and software engineers. What topics to cover?**
    - Answer: Format (1 hour, end of sprint): (1) What went well: Celebrate successes (both HW and SW achievements). (2) What didn't go well: Hardware delays? Software bugs? Interface issues? Integration problems? (3) Hardware-software interface review: Communication clear? Specifications adequate? Surprises? (4) Tool/process improvements: Need better debugging tools? Simulation environment? (5) Learning: Any new technologies/techniques tried? (6) Action items: Concrete improvements for next sprint (assign owners). Unique to embedded: (1) Hardware availability (boards for testing), (2) Toolchain issues (compilers, debuggers), (3) Lab equipment access. Foster cross-discipline understanding (SW appreciates HW constraints, HW understands SW needs).

55. **A safety-critical system finds a critical bug after deployment. Compare how Waterfall and Agile processes would have prevented this.**
    - Answer: Waterfall prevention: (1) Formal requirements review (catch specification errors), (2) Design review (catch architectural issues), (3) Code inspection (catch implementation bugs), (4) Extensive test phase (multiple test levels), (5) Independent verification & validation (separate team). Heavy process aims to prevent bugs reaching deployment. Agile prevention: (1) Continuous testing (catch bugs immediately), (2) Frequent demos (stakeholder validation), (3) Incremental delivery (smaller changes, easier to validate), (4) TDD (tests define expected behavior), (5) Pair programming (second pair of eyes). Fast feedback aims to catch bugs early. Reality: Both can fail; defense-in-depth (formal reviews + automation + continuous testing) best for safety-critical.

56. **You must train new team members on your development process. Design a training program that covers Waterfall, Agile, and CI/CD concepts.**
    - Answer: Training program (2 weeks): **Week 1 - Fundamentals**: Day 1-2: Overview (abstraction levels, design process), history (why processes exist). Day 3: Waterfall deep-dive (phases, documentation, when to use). Day 4: V-Model (verification strategy, traceability). Day 5: Waterfall hands-on exercise (small example project). **Week 2 - Modern practices**: Day 1-2: Agile principles, Scrum/XP practices. Day 3: CI/CD (pipeline concept, automation). Day 4-5: Hands-on (mini project: 2 sprints with CI/CD pipeline). **Embedded-specific**: Integrate examples (washing machine, traffic light), discuss hardware constraints, show actual CI/CD for embedded (cross-compilation, hardware-in-loop). **Assessment**: Final project applying concepts. **Mentoring**: Pair new members with experienced during real sprints.

57. **Design an exploration process for choosing between three embedded processor architectures. What criteria and process steps?**
    - Answer: Module-level exploration: **Alternatives**: ARM Cortex-M4, Cortex-R5, Cortex-A53. **Criteria**: (1) Performance (can meet timing requirements?), (2) Power consumption (battery life), (3) Cost (BOM impact), (4) Toolchain maturity (compiler, debugger), (5) Ecosystem (libraries, RTOS support), (6) Team experience. **Process**: **Step 1**: Specify requirements (computational needs, real-time constraints). **Step 2**: Initial research (datasheets, benchmarks). **Step 3**: Prototype each option (dev boards, implement critical algorithm). **Step 4**: Measurement (actual performance, power, development time). **Step 5**: Analysis (create comparison matrix). **Step 6**: Decision (select optimal, document rationale). **Step 7**: Refine to block level (peripheral selection within chosen architecture). Systematic exploration avoids arbitrary choices.

### Connections and Integration (7 questions)

58. **How do abstraction levels (Lecture 05) relate to the UML state machines from Lecture 04? At what level are state machines typically created?**
    - Answer: State machines are system or module level abstraction. System level: Overall control flow (like washing machine top-level states). Module level: Detailed state machine with implementation considerations (timing, sensors). Block level would be: functions implementing state machine logic. Logic level: assembly code for state transitions. State machines bridge requirements (what) to implementation (how) at appropriate abstraction.

59. **Connect the development processes (Lecture 05) to requirements engineering (Lecture 03). How does process choice affect requirements handling?**
    - Answer: Waterfall: Requirements frozen early, formal specification, traceability matrix, changes via change control board. Agile: Requirements as user stories, evolving product backlog, continuous refinement, informal documentation. V-Model: Requirements define acceptance tests, formal but allows iteration. Lab U01's peer review cycle is mini-agile sprint. Process determines how requirements captured, updated, verified.

60. **How does CI/CD (Lecture 05) integrate with model checking (Lecture 04) and WCET analysis (Lecture 04)?**
    - Answer: CI/CD pipeline stages: (1) Model checking: Automated verification of state machine properties using model checker tool, (2) WCET analysis: Static analysis tools run automatically, report worst-case timing, (3) Both generate reports in CI/CD dashboard. Enables continuous verification (not just one-time). Catch violations immediately when code changes. Example: Commit changes timing, CI/CD runs WCET tool, detects deadline violation, blocks merge.

61. **Relate the design space exploration (Lecture 05 exploration step) to energy-performance-cost triangle (Lecture 01).**
    - Answer: Exploration step evaluates alternatives on energy-performance-cost axes. Example: Processor selection (module level): Low-power CPU (low energy, low performance, low cost) vs. High-performance CPU (high energy, high performance, high cost) vs. Multi-core (medium energy, high performance, medium-high cost). Exploration systematically evaluates trade-offs. Refinement commits to point on triangle. DSE (future lectures) formalizes this exploration.

62. **How would the washing machine state machine (Lab U01) be developed differently using Waterfall vs. Agile?**
    - Answer: Waterfall: (1) Requirements phase: Formal specification document (all requirements listed). (2) Design phase: Create complete UML state machine, review, approve. (3) Implementation: Code entire state machine. (4) Test: Verify all states/transitions. Sequential, complete design before coding. Agile: (1) Sprint 1: Basic states (IDLE, WASH, SPIN), demo. (2) Sprint 2: Add heating, emergency stop, demo. (3) Sprint 3: Add history, refinement, demo. Incremental, working system each sprint, evolving design. Both reach same end state machine, but agile delivers functionality earlier.

63. **Connect top-down design (Lecture 05) with task-to-hardware binding (Lecture 04). At what abstraction level does binding occur?**
    - Answer: Top-down: Start with high-level tasks (SW model at system level) and hardware architecture (HW model at system level). Binding decision at module level: Assign tasks to specific processors. Refinement continues to block level: Task implementation details, processor configuration. Task graph and processor options come from system level; binding choice made at module level during exploration step (evaluate alternatives like different task distributions).

64. **How do non-functional requirements (future Lecture 06) drive the choice of development process and verification methods?**
    - Answer: Tight NFRs (real-time, safety, energy) favor Waterfall/V-Model: Need upfront analysis (can system meet timing?), formal verification (prove safety), extensive testing. Relaxed NFRs allow Agile: Can measure and optimize iteratively. Verification methods: Tight NFRs need formal methods (model checking, WCET analysis) in CI/CD. Process must support NFR validation at each level (V-Model good for this). Example: Safety-critical medical device (tight NFRs) → V-Model with formal verification. Experimental IoT product (relaxed NFRs) → Agile with measurement-based optimization.

### Advanced Understanding (8 questions)

65. **Analyze: How does Conway's Law ("organizations design systems that mirror their communication structure") affect choice between Waterfall and Agile?**
    - Answer: Waterfall encourages siloed organization: Requirements team → Designers → Coders → Testers (sequential communication, formal handoffs). Results in layered system architecture (matches org structure). Agile encourages cross-functional teams: Everyone talks to everyone (dense communication). Results in modular architecture with clear interfaces (enables parallel development). If organization can't restructure (large company with established departments), Waterfall may fit existing communication patterns better. Changing process without changing organization structure fails.

66. **Propose a formal method for deciding which development process to use for a given project. What factors and weights?**
    - Answer: Decision matrix with weighted criteria: **Factors** (weights in parentheses): (1) Requirements stability (3): Stable → Waterfall, Uncertain → Agile. (2) Hardware/Software ratio (2): Hardware-heavy → Waterfall, Software-heavy → Agile. (3) Team size (2): Large (>50) → Waterfall, Small (<20) → Agile. (4) Regulatory requirements (3): Heavy → V-Model, Light → Agile. (5) Customer availability (1): Low → Waterfall, High → Agile. (6) Time-to-market pressure (2): Long → Waterfall, Short → Agile. (7) Budget flexibility (1): Fixed → Waterfall, Flexible → Agile. Score each factor (1-5), multiply by weight, sum. High score → Waterfall, Low score → Agile, Medium → Hybrid. Not absolute but provides structured decision.

67. **How might quantum computing change embedded systems development processes in the future, particularly verification and exploration?**
    - Answer: Potential impacts: (1) Verification: Quantum algorithms for model checking (exponentially faster state space exploration), exhaustive testing feasible for larger systems. (2) Exploration: DSE with quantum optimization (explore entire design space simultaneously), find true optimal vs. heuristic. (3) Cryptography: Quantum-safe security becomes requirement, affects requirements phase. (4) Simulation: Quantum simulation of complex systems (chemistry, materials) enables better abstractions. Challenges: Quantum expertise rare, tools immature, most embedded systems don't need quantum speed. Process impact likely gradual (quantum-as-a-service for specific verification/optimization tasks) rather than revolutionary.

68. **Critique the assumption that "more testing always leads to better quality." How do development processes balance testing and other quality factors?**
    - Answer: Diminishing returns: Initial testing finds most bugs (high value), additional testing finds fewer bugs (lower value per hour). Trade-offs: Time spent testing could be spent on better design (prevents bugs), refactoring (improves maintainability), reviews (catches design flaws). Waterfall risks: Heavy testing at end may find unfixable design flaws. Agile balance: Continuous testing throughout (TDD, CI/CD) but not excessive. V-Model: Right-size testing at each level. Quality not just "absence of bugs" but also maintainability, performance, security. Processes like XP emphasize design quality (simple design, refactoring) alongside testing. Best: Prevent defects (design, reviews) AND detect defects (testing) in balanced proportion.

69. **Design a metrics dashboard for tracking development process effectiveness. What metrics for Waterfall vs. Agile vs. CI/CD?**
    - Answer: **Waterfall metrics**: (1) Phase completion (on schedule?), (2) Requirements volatility (# of changes), (3) Defect discovery rate by phase (catching bugs early?), (4) Cost/schedule variance. **Agile metrics**: (1) Velocity (story points per sprint, stable?), (2) Sprint burndown (work progressing?), (3) Cycle time (idea to production), (4) Team satisfaction (retrospectives). **CI/CD metrics**: (1) Build success rate (how often passes?), (2) Mean time to repair broken build, (3) Test coverage (%), (4) Deployment frequency. **Universal**: (1) Defect density (bugs per KLOC), (2) Customer satisfaction, (3) Time-to-market. Dashboard shows trends, not just snapshots. Red flags: Decreasing velocity (agile), increasing requirements changes (waterfall), low build success rate (CI/CD).

70. **Analyze: As embedded systems become more software-intensive (IoT, autonomous vehicles), how should development processes evolve?**
    - Answer: Shift toward software practices: (1) More agile methods (even with hardware constraints), (2) Over-the-air firmware updates enable iteration (changes post-deployment like web apps), (3) DevOps culture (CI/CD standard, not optional), (4) Cloud integration (embedded device + cloud backend requires coordination), (5) Security emphasis (connected devices vulnerable), (6) Data-driven development (telemetry informs next version). But: Safety-critical nature remains (can't be purely agile, need verification). Future: Hybrid processes standard (agile development + V-model verification), continuous delivery for non-critical updates (features), formal releases for safety-critical (firmware), DevSecOps (security integrated). Hardware still constrains but less than before.

71. **How do open-source development practices compare to traditional embedded systems processes? Could a medical device be developed open-source?**
    - Answer: Open-source characteristics: (1) Distributed development (contributors worldwide), (2) "Release early, release often" (very agile), (3) Peer review (pull requests), (4) Continuous integration (GitHub Actions common), (5) Meritocracy (best ideas win). Contrasts: (1) Less formal process (no mandated Waterfall/Agile), (2) Lightweight documentation (README, code comments), (3) Community-driven requirements (scratching own itch). Medical device open-source: Technically possible (open-source projects can follow V-Model, maintain traceability), legally complex (who liable if bug causes harm? FDA approval process?). Examples exist: Open APS (artificial pancreas), but regulatory path unclear. Future: Hybrid (open-source components, proprietary integration, company responsible for certification).

72. **Evaluate: "AI-assisted code generation will make development processes obsolete." What's valid and what's missing in this claim?**
    - Answer: Valid elements: (1) AI can accelerate coding (implementation phase faster), (2) AI can generate tests (automation of test creation), (3) AI can suggest designs (exploration assistance). Missing: (1) Requirements still need human understanding (what to build), (2) Architecture decisions require domain expertise (how to structure), (3) Verification still needed (AI-generated code can have bugs), (4) Processes about communication and coordination (not just coding), (5) Safety-critical systems need explainable, auditable code (AI black box problematic). Likely future: AI accelerates within process (faster exploration, faster implementation) but doesn't eliminate process. Process evolves to incorporate AI tools (e.g., AI-assisted code review in CI/CD), but human oversight remains. Claim is oversimplified.

These 72 exam questions cover all aspects of Lecture 05, from basic definitions through advanced synthesis, with emphasis on practical application and integration with other course topics.
