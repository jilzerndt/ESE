# Lecture 06: Non-Functional Requirements

## Technical Terms & Definitions

### Core Definitions

**Non-Functional Requirement (NFR)**
- A requirement for a product that specifies criteria that can be used to judge a system but does NOT describe the desired behavior (or functionality) of the system
- Describes what the system *should be* instead of what the system *should do*
- The difference between software that works and software that works *well*

**Alternative Names for NFRs**
- **Quality attributes**: Properties and characteristics the system should demonstrate
- **Constraints (limitations)**: Restrictions, standards, and environmental factors that must be taken into account
- **Technical requirements**: Requirements focused on technical properties
- **"-ilities"**: Reliability, Usability, Maintainability, etc.

### NFR Categories

**Functional Requirements**
- Describe what the system should DO
- Define the desired behavior or functionality of the system
- Example: "The traffic light control shall show a green light to only one lane at a time"

**Efficiency (NFR Category)**
- Related to resource usage and performance
- Subcategories: Energy, Cost, Development Time, Performance, Memory, Weight, Space

**Dependability (NFR Category)**
- Related to system reliability and trustworthiness
- Subcategories: Reliability, Availability, Integrity, Standard compliance, Safety, Maintainability

**Security (NFR Category)**
- Related to protection of system and data
- Subcategories: Confidentiality, Availability, Integrity
- Note: Availability and Integrity overlap with Dependability

### Classification Dimensions

**Development Classification**
- **Qualities**: Properties and characteristics that the delivered system should demonstrate
- **Constraints**: Limitations, standards, and environmental factors that must be taken into account in the solution

**Temporal Classification**
- **Execution Requirements**: Observable during operation (runtime properties)
- **Evolution Requirements**: Observable during development (development-time properties)

**Measurable Classification**
- **Quantifiable Requirements**: Can be measured with specific numeric values (preferred)
- **Qualifiable Requirements**: Can only be assessed qualitatively (pass/fail, compliant/non-compliant)

### Execution NFRs

**Energy Requirements**
- Power consumption limits
- Battery life requirements
- Energy efficiency targets

**Physical Requirements**
- Size constraints (dimensions)
- Weight limitations
- Form factor specifications

**Performance Requirements**
- Response time (how fast the system responds to inputs)
- Execution time (how long operations take)
- Transaction rates (operations per second)
- Throughput (data processed per time unit)

**Memory Requirements**
- RAM (Random Access Memory) usage limits
- ROM/Flash storage capacity
- Data storage requirements

**Product Cost Requirements**
- Bill-of-Material (BOM) costs
- Software license fees
- Manufacturing costs
- Manufacturing overhead

**Security Requirements**
- Protection of system and data
- Resistance to attacks
- Access control requirements

**Complexity Requirements**
- System complexity limits
- Code complexity constraints

### Evolution NFRs

**Manufacturability Requirements**
- Ease of manufacturing the product
- Automated manufacturing capabilities
- Example: "It shall be possible to manufacture the traffic light in a fully automated process"

**Extensibility Requirements**
- Ability to add new features
- System expandability
- Example: "The traffic light control system shall be extensible to enable the use with a 5-road intersection"

**Usability Requirements**
- How difficult it is to learn the system
- Ease of operation
- Often expressed in learning time metrics
- User interface quality

**Modifiability Requirements**
- Ease of making changes to the system
- Time needed to implement new features
- Measured in: Time to implement a new feature

**Portability Requirements**
- Ability to run on different platforms
- Cross-platform compatibility
- Hardware independence

**Testability Requirements**
- Ease of testing the system
- Test coverage achievability
- Hardware-software integration testing complexity

**Configurability Requirements**
- System configuration flexibility
- Parameter adjustment capabilities

**Scalability Requirements**
- System growth capabilities
- Handling increased load
- Adding more resources

**Development Time Requirements**
- Time-to-market constraints
- Development schedule limits

### Quantifiable NFRs

**Capacity**
- Program code size limits
- Data storage capacity
- Storage space availability
- Example: "What size can our program code occupy? How much data can we store?"

**Performance (Quantifiable)**
- Request processing speed
- Concurrent request handling capacity
- Response times under load
- Measured in: ms, operations/second, throughput

**Usability (Quantifiable)**
- Learning time
- Time to complete tasks
- Error rates
- Measured in: hours, days, errors per session

**Security (Quantifiable)**
- Effort required to break into system
- Skill level needed for attacks
- Time to compromise system
- Measured in: effort, skill level, time

**Reliability (Quantifiable)**
- **MTBF (Mean Time Between Failures)**: How often the software fails
- Recovery time after failure
- System uptime percentage
- Example: "The MTBF for a single traffic light shall be larger than 500 years"

### Qualifiable NFRs

**Regulatory Requirements**
- Local, regional, provincial, or state laws and regulations
- Legal compliance
- What is legally allowed

**Localization Requirements**
- Adaptation to measurement systems
- Time zone handling
- Locally-specific concepts
- Regional adaptations

**Legal Requirements**
- Privacy of information
- Intellectual property rights
- Export restrictions on technologies
- Data protection compliance

**Standard Compliance**
- Industry standards adherence
- Process standards compliance
- Technical standards fulfillment
- Example: "The traffic light control system shall fit a standard traffic control housing according to the SN XXXX, V1.x, 2023"

### Constraints

**Definition**: Limitations, standards, and environmental factors that must be fulfilled when developing an Embedded System.

**Types of Constraints:**
- **Standard compliance**: Must adhere to specific standards
- **Company development processes**: Must follow organizational procedures
- **Programming language constraints**: Must use specific languages
- **Platform reuse constraints**: Must use existing platforms
- **Intellectual Property constraints**: IP licensing and usage restrictions

### ISO/IEC 25010 Categories

**Functional Suitability**
- Functional Completeness
- Functional Correctness
- Functional Appropriateness

**Performance Efficiency**
- Time Behaviour
- Resource Utilization
- Capacity

**Compatibility**
- Co-existence
- Interoperability

**Interaction Capability**
- Appropriateness Recognizability
- Learnability
- Operability
- User Error Protection
- User Engagement
- Inclusivity
- User Assistance
- Self-descriptiveness

**Reliability**
- Faultlessness
- Availability
- Fault Tolerance
- Recoverability

**Security**
- Confidentiality
- Integrity
- Non-repudiation
- Accountability
- Authenticity
- Resistance

**Maintainability**
- Modularity
- Reusability
- Analysability
- Modifiability
- Testability

**Flexibility**
- Adaptability
- Scalability
- Installability
- Replaceability

**Safety**
- Operational Constraint
- Risk Identification
- Fail Safe
- Hazard Warning
- Safe Integration

### Metrics and Measurement

**MTBF (Mean Time Between Failures)**
- Formula: Average time the system operates before failing
- Unit: hours, days, years
- Higher value = more reliable system
- Can be calculated from design data

**Product Cost Formula**
```
Product Cost = Bill-of-Material (BOM)
              + Software License Fees
              + Manufacturing Cost
              + Manufacturing Overhead
```

**Measurable Criteria Derivation Process**
1. **Quality Factors**: Identify the NFR (e.g., Reliability, Modifiability, Usability)
2. **Define Metrics**: Determine how to measure (e.g., MTBF, Time to implement feature, Learning time)
3. **Realization of Metrics**: How to count/measure in practice (e.g., Run and count crashes, Count person-months, Count learning hours)

### Timing and Real-Time Constraints

**Real-Time Constraints**
- Missed deadlines can cause physical harm or failure
- Critical in embedded systems
- Hard vs. soft real-time requirements

**Safety**
- Often safety-critical environments (aerospace, medical, automotive)
- Failure can result in loss of life or significant damage

**Reliability/Robustness**
- Must operate continuously in unpredictable conditions
- Environmental challenges (temperature, vibration, etc.)

**Fault Tolerance**
- Must continue operating under partial hardware/software failure
- Graceful degradation
- Redundancy mechanisms

**Maintainability**
- Embedded code often has long lifespans
- Embedded systems have long product lifetime
- Must be maintainable over decades

**Testability**
- Hardware-software integration complicates testing
- Edge cases difficult to test
- Need for proper test strategies

## Core Concepts

### Functional vs Non-Functional Requirements

**Key Distinction:**
- **Functional**: What the system DOES (behavior, functionality)
- **Non-Functional**: What the system IS (quality, characteristics, constraints)

**Interdependence:**
- All possible implementations may fulfill functional requirements
- NFRs guide design toward the optimal solution
- NFRs act as decision criteria between implementation alternatives
- NFRs are NOT "nice to have" – they are critical

### The Three Main NFR Categories for Embedded Systems

**1. Efficiency** (Resource Usage)
- How well the system uses resources
- Includes: Energy, Cost, Time, Memory, Space, Weight
- Critical for embedded systems with limited resources
- Trade-off: Often conflicts with other NFRs

**2. Dependability** (Trustworthiness)
- Can the system be relied upon?
- Includes: Reliability, Availability, Integrity, Safety, Maintainability
- Essential for safety-critical systems
- Overlap with Security (Availability, Integrity)

**3. Security** (Protection)
- How well is the system protected?
- Includes: Confidentiality, Availability, Integrity
- CIA triad (Confidentiality, Integrity, Availability)
- Increasing importance in connected embedded systems

### Qualities vs Constraints

**Qualities:**
- Define properties and characteristics the delivered system should demonstrate
- Can often be measured or assessed
- Examples: Performance, Reliability, Usability
- Drive optimization decisions

**Constraints:**
- Limitations, standards, and environmental factors
- Must be taken into account in the solution
- Often binary (met or not met)
- Examples: Standard compliance, Platform requirements
- Act as "guardrails" to exclude non-compliant solutions

### Execution vs Evolution NFRs

**Execution NFRs** (Runtime)
- Relevant for the embedded system in *operation*
- Observable when the system is running
- Examples: Energy consumption, Performance, Memory usage, Security
- Affect end-user experience directly
- Measured during operation or testing

**Evolution NFRs** (Development)
- Relevant for the embedded system in the *development phase*
- Observable during development
- Examples: Maintainability, Testability, Scalability, Development time
- Affect development team and long-term costs
- Measured during development process

### Quantifiable vs Qualifiable NFRs

**Quantifiable Requirements** (Preferred)
- Can be measured with specific numeric values
- Testable with clear pass/fail criteria
- Examples:
  - "Energy consumption < 5W"
  - "Response time < 100ms"
  - "MTBF > 500 years"
- Advantages: Clear, unambiguous, verifiable

**Qualifiable Requirements**
- Can only be assessed qualitatively
- Pass/fail or compliance-based
- Examples:
  - "Shall comply with ISO standard X"
  - "Shall be easy to use"
  - "Shall follow company development process"
- Challenge: Harder to verify objectively

**Goal**: Convert qualifiable into quantifiable whenever possible
- Example: "Easy to use" → "Users can complete task X in < 10 minutes after 1 hour of training"

### Trade-offs Between NFRs

**Common Trade-offs:**

1. **Security vs. Usability**
   - More security = More complexity, harder to use
   - Example: Complex passwords vs. ease of login
   - Context matters: Banking app vs. Mobile game

2. **Performance vs. Maintainability**
   - Optimized code often harder to maintain
   - Readable code may be slower
   - Trade-off: Speed vs. long-term costs

3. **Performance vs. Security**
   - Security checks add overhead
   - Encryption slows down operations
   - Balance: Adequate security without unacceptable slowdown

4. **Maintainability vs. Time-to-Market**
   - Clean code takes longer to write
   - Quick solutions create technical debt
   - Short-term vs. long-term thinking

5. **Reliability vs. Cost**
   - Redundancy increases reliability but costs more
   - More testing increases reliability but extends schedule
   - Question: How much reliability is needed?

6. **Safety vs. Flexibility**
   - Safety constraints limit flexibility
   - More configurability = more potential for unsafe states
   - Medical device vs. Consumer product

7. **Portability vs. Performance**
   - Platform-specific optimizations not portable
   - Generic code slower than optimized
   - Trade-off: Portability vs. maximum performance

**Context-Dependent Decision Making:**
- Medical device: Safety and Reliability >> Time-to-Market
- Mobile game: Time-to-Market and Usability >> Reliability
- Automotive: Safety >> Cost (but cost still important)
- IoT device: Energy >> Performance

### Design Space and NFRs

**Design Space Triangle** (Cost-Energy-Performance)
- Most popular NFRs for embedded systems
- Three competing objectives
- Different implementation points on triangle
- Examples:
  - Low-Energy CPU: Low energy, higher cost, lower performance
  - 2 CPUs: Higher performance, higher energy, higher cost
  - 1 CPU + FPGA: Balanced approach

**NFRs as Design Guides:**
1. Start with functional requirements (define what to do)
2. Apply NFRs to narrow down options
3. Constraints eliminate non-compliant solutions
4. Qualities optimize among remaining options
5. Result: Optimal solution meeting all requirements

**Funnel Approach:**
- Top: All possible implementations (wide)
- Middle: Filtered by constraints (narrower)
- Bottom: Optimized by qualities (narrow)
- Result: Single optimal solution (point)

### Deriving Measurable Criteria

**Challenge**: Some NFRs are not directly measurable

**Process:**
1. **Identify Quality Factor**: Name the NFR (e.g., "Modifiability")
2. **Define Measurable Criteria**: What can we measure? (e.g., "Time to implement new feature")
3. **Determine Realization**: How to measure in practice? (e.g., "Count person-months on timesheet")

**Examples:**

| Quality Factor | Measurable Criteria | Realization |
|---------------|---------------------|-------------|
| Reliability | Mean time to failure | Run system and count crashes |
| Modifiability | Time to add feature | Count person-months on timesheet |
| Usability | Learning time | Count hours spent learning to operate ES |
| Energy | Power consumption | Measure with power analyzer |
| Memory | Code/data size | Read from .lst/.map file |

**Straight-forward Measurements:**
- Energy consumption: Power analyzer
- Memory consumption: Linker map file (.lst/.map)
- Performance: Timing measurements, profilers
- Code size: Compiler output

**Non-trivial Measurements:**
- Usability: User studies, task completion time
- Maintainability: Code metrics, change effort tracking
- Reliability: Long-term testing, failure data collection

### Requirements Characteristics (Recap)

Good requirements must be:
- **Unambiguous**: Clear, single interpretation
- **Testable (verifiable)**: Can prove compliance
- **Clear**: Concise, terse, simple, precise
- **Correct**: Accurate representation of needs
- **Understandable**: Stakeholders can comprehend
- **Feasible**: Realistic, possible to implement
- **Independent**: Not overlapping with others
- **Atomic**: Single, indivisible requirement
- **Necessary**: Required for system success
- **Implementation-free**: Abstract, not specifying how

**Priority Assignment:**
- Usually assign priorities to requirements (e.g., 0-3)
- Helps with trade-off decisions
- Guides implementation order
- Critical for resource-limited projects

## Important Diagrams & Graphics

### 1. Design Space Triangle

**Description:**
- Triangle with three corners labeled: Cost, Energy, Performance
- Interior shows different implementation options (boxes):
  - Low-Energy CPU (near Energy corner)
  - 1 CPU (near Cost corner)
  - 2 CPUs (near Performance corner)
  - 1 CPU + FPGA (center-right)
- Points on/in triangle represent different implementations
- Each point represents trade-offs between the three dimensions

**How to draw:**
1. Draw equilateral triangle
2. Label corners: Top = Energy, Bottom-left = Cost, Bottom-right = Performance
3. Place implementation options as boxes at different positions
4. Closer to a corner = better in that dimension, worse in others

**Meaning:**
- Visualizes the three-way trade-off in embedded system design
- No single solution optimal in all three dimensions
- Different applications require different points in the space

### 2. NFR Radar Chart (Spider Diagram)

**Description:**
- Octagonal (8-sided) radar chart with axes for different NFRs
- Axes (clockwise from top): Cost, Performance, Latency, Energy, Configurability, Manufacturability, Testability, Safety, Security
- Scale 0-10 on each axis (0 at center, 10 at perimeter)
- Multiple colored polygons representing different system configurations
- Shows relative strengths/weaknesses across NFR dimensions

**How to draw:**
1. Draw 8+ axes radiating from center (equally spaced)
2. Label each axis with an NFR
3. Mark scale 0-10 from center to edge
4. Plot values for a system design
5. Connect points to form polygon
6. Compare multiple designs with different colored polygons

**Meaning:**
- Visual comparison of how well different designs meet various NFRs
- Larger area = generally better (but depends on priorities)
- Shape shows relative balance between different NFRs

### 3. Efficiency, Dependability & Security Categories

**Description:**
Three columns with icons:
- **Left (Efficiency)**: Speedometer icon
  - Lists: Energy, Cost, Development Time, Performance, Memory, Weight, Space
- **Center (Dependability)**: Checkmark icon
  - Lists: Reliability, Availability, Integrity, Standard compliance, Safety, Maintainability
  - Note: Availability and Integrity appear in both Dependability and Security (shown with dashed box)
- **Right (Security)**: Lock icon
  - Lists: Confidentiality, Availability, Integrity

**Venn diagram inset** showing overlap between Dependability and Security:
- Dependability contains: Reliability, Safety, Confidentiality, Maintainability
- Overlap: Availability, Integrity
- Security distinct elements

**How to draw:**
1. Create three columns with headers and icons
2. List sub-categories under each
3. Draw dashed box around overlapping items (Availability, Integrity)
4. Add small Venn diagram showing the relationship

**Meaning:**
- Shows three main NFR categories for embedded systems
- Illustrates overlap between Dependability and Security
- Provides taxonomy for organizing NFRs

### 4. ISO/IEC 25010 Software Product Quality Table

**Description:**
Large table with main quality characteristics as column headers:
- Functional Suitability
- Performance Efficiency
- Compatibility
- Interaction Capability
- Reliability
- Security
- Maintainability
- Flexibility
- Safety

Each column lists sub-characteristics (3-9 items per category)

**How to draw:**
1. Create table with 9 columns
2. Header row: Main quality characteristics
3. Rows below: List all sub-characteristics for each category
4. Align sub-characteristics vertically within columns
5. Use different row colors for readability

**Meaning:**
- International standard for software quality requirements
- Comprehensive framework for categorizing NFRs
- Industry-accepted taxonomy
- Useful reference for identifying relevant NFRs

### 5. Classification of NFRs Diagram

**Description:**
Three classification approaches shown with boxes:

**Development Classification:**
- Qualities (define properties and characteristics)
- Constraints (limitations, standards, environmental factors)

**Temporal Classification:**
- Execution Requirements (observable during operation)
- Evolution Requirements (observable during development)

**Measurable Classification:**
- Quantifiable Requirements (preferred) ← arrow pointing to this
- Qualifiable Requirements

**How to draw:**
1. Three sections labeled "Development," "Temporal," "Measurable"
2. Each section has 2 boxes with classification types
3. Arrow pointing to "Quantifiable Requirements" indicating preference
4. Use different colors or shading for different classifications

**Meaning:**
- Shows multiple ways to categorize NFRs
- Not mutually exclusive (one NFR can fit multiple classifications)
- Quantifiable preferred because it's measurable
- Helps organize and think about NFRs systematically

### 6. Mapping NFRs to Categories Table

**Description:**
2x2 table mapping requirement types to temporal categories:

|              | Qualifiable          | Quantifiable |
|--------------|----------------------|--------------|
| **Execution**    | Standard Compliance  | Current consumption<br>Security<br>Performance<br>Usability |
| **Evolution**    | SW Licenses<br>Process Compliance | Development time<br>Project Cost<br>Testability<br>Scalability |

**How to draw:**
1. Create 2x3 table
2. Headers: "Requirement" (row), "Qualifiable", "Quantifiable" (columns)
3. Row labels: "Execution", "Evolution"
4. Fill cells with examples
5. Use different shading for row headers

**Meaning:**
- Shows how NFRs can be classified on multiple dimensions simultaneously
- Demonstrates that NFRs don't fit into single categories
- Helps identify measurement approaches

### 7. Deriving Measurable Criteria Flow Diagram

**Description:**
Three-level hierarchy showing progression from abstract to concrete:

**Level 1: Quality Factors**
- Reliability | Modifiability | Usability

**Level 2: Measurable Criteria** (arrow down from Level 1)
- Mean time to failure? | Time it needs to implement a new feature to ES? | Time taken to learn how to work with ES?

**Level 3: Counts taken from Realization** (arrow down from Level 2)
- Run it and count crashes? | Count the Person Months on the time sheet? | Count the hours spent learning operating the ES?

Two callout boxes on left:
- "Define the Metrics" (pointing to Level 2)
- "Realization of the Metrics" (pointing to Level 3)

**How to draw:**
1. Three horizontal levels with content boxes
2. Downward arrows between levels
3. Vertical separators creating columns for different NFR types
4. Callout boxes on left side explaining each level

**Meaning:**
- Shows systematic process for making NFRs measurable
- Progression from abstract quality to concrete measurement
- Practical approach to NFR verification

### 8. Design Funnel Diagram

**Description:**
Triangle/funnel shape showing design space narrowing:
- **Top (wide)**: "Idea" - represents all possible implementations
- **Middle layers**: Progressively narrower, shown in different colors (orange, blue, green)
- **Bottom (narrow)**: "Possible Implementations" - final solution space
- Green circle at bottom represents optimal solution

Text annotations on right:
- "All possible implementations fulfill the functional requirements"
- "NFRs are responsible for guiding the design towards the optimal solution"
- "Constraints NFRs represent guardrails to exclude solutions"
- "The earlier in the design process the compliance to an NFR can be confirmed, the better"
- "Comparison between two solutions concerning compliance to an NFR might be useful"

**How to draw:**
1. Draw triangle/funnel with apex at top, wide base at bottom
2. Divide into horizontal layers with different colors
3. Label top "Idea" and bottom "Possible Implementations"
4. Add green circle at bottom point (optimal solution)
5. Add text annotations on the side

**Meaning:**
- Visual representation of how NFRs narrow design space
- Shows progressive refinement from many options to optimal solution
- Illustrates role of constraints (eliminate) vs. qualities (optimize)
- Earlier filtering is better (saves effort)

### 9. Other Classification Diagrams (From Literature)

**Boehm's NFR List** (shown in slide):
- Circular diagram with "As-is utility" in center
- Branches to "General utility" and "Maintainability"
- Further subdivisions showing relationships
- Shows classic academic taxonomy

**Key Types of Non-Functional Requirements** (shown in slide):
- Boxes for: Performance, Scalability, Portability
- Center: "Types of non-functional requirements"
- Additional boxes: Compatibility, Reliability, Maintainability, Sustainability, Availability

**Non-functional Requirements Map** (shown in slide):
- Tree structure
- "Execution requirement" and "Evolution requirement" as main branches
- Each branch subdividing into specific NFRs
- Shows hierarchical relationships

## Formulas & Procedures

### Product Cost Calculation

**Formula:**
```
Product Cost = Bill-of-Material (BOM)
              + Software License Fees
              + Manufacturing Cost
              + Manufacturing Overhead
```

**Components:**
- **BOM (Bill of Materials)**: Cost of all hardware components
- **Software License Fees**: Cost of licensed software/tools
- **Manufacturing Cost**: Direct costs of production
- **Manufacturing Overhead**: Indirect production costs

**Example Requirement:**
"The product cost of a single traffic light shall not be higher than 3000 CHF @1000 pieces."

### MTBF (Mean Time Between Failures)

**Concept:**
- Measure of system reliability
- Average time between system failures
- Higher MTBF = more reliable system

**Units**: Hours, days, months, years

**Calculation**: Based on design data, component reliability, operating conditions

**Example Requirement:**
"The MTBF for a single traffic light shall be larger than 500 years."

**Practical Note:**
- Cannot actually test 500-year MTBF
- Calculated from component specifications
- Based on statistical models and historical data

### Deriving Measurable Criteria - Procedure

**Step 1: Identify Quality Factor**
- Name the NFR you need to measure
- Example: Reliability, Modifiability, Usability

**Step 2: Define Measurable Criteria**
- What can be measured that indicates the quality?
- Must be quantifiable
- Examples:
  - Reliability → Mean time to failure
  - Modifiability → Time to implement new feature
  - Usability → Time to learn operation

**Step 3: Determine Realization Method**
- How to actually measure/count in practice?
- Examples:
  - Mean time to failure → Run system and count crashes
  - Time to implement feature → Count person-months on timesheet
  - Learning time → Count hours spent in training/learning

**Example Application:**

```
NFR: "The system shall be easy to use"

Step 1 - Quality Factor: Usability

Step 2 - Measurable Criteria:
"Time taken for a new user to learn how to complete basic tasks"

Step 3 - Realization:
"Conduct user study with 10 representative users.
Measure time from first interaction to successful
completion of task list X.
Requirement: Average time < 2 hours."
```

### Energy Measurement Procedure

**Tools Needed:**
- Power analyzer
- Oscilloscope with current measurement capability
- Current sense resistor

**Procedure:**
1. Insert current measurement in power supply path
2. Configure power analyzer or oscilloscope
3. Run system through typical operation cycle
4. Measure power over time: P(t)
5. Calculate energy: E = ∫ P(t) dt
6. Report: Average power, peak power, total energy

**Example:**
- Measure washing machine energy consumption
- Record power during full wash cycle
- Calculate total energy in Wh or kWh
- Compare against requirement

### Memory Measurement Procedure

**For Embedded Systems:**

**1. Code Size (ROM/Flash):**
- Compile project
- Check linker map file (.map) or list file (.lst)
- Look for memory usage summary
- Example output:
  ```
  Code:  45678 bytes
  Data:  12345 bytes
  Total: 58023 bytes
  ```

**2. RAM Usage:**
- Static allocation: From linker map
- Dynamic allocation: Requires runtime analysis
- Stack usage: Requires worst-case analysis or measurement

**3. Verification:**
- Compare against requirement
- Ensure margins for future expansion
- Example requirement: "Code size < 64 KB"

### Requirements Verification Procedure

**General Approach:**

**1. For Quantifiable NFRs:**
```
1. Identify metric and threshold
2. Implement measurement method
3. Execute test/measurement
4. Compare result to threshold
5. Document: Pass/Fail with evidence
```

**2. For Qualifiable NFRs:**
```
1. Identify standard/regulation
2. Obtain compliance checklist
3. Review system against checklist
4. Document compliance for each item
5. Result: Compliant/Non-compliant
```

**Example - Performance Requirement:**
```
Requirement: "System response time < 100ms for 95% of requests"

Verification:
1. Instrument system to log response times
2. Generate representative load (1000 requests)
3. Collect timing data for all requests
4. Sort response times
5. Calculate 95th percentile
6. Compare: 95th percentile = 87ms < 100ms ✓ PASS
```

### Trade-off Analysis Procedure

**When NFRs Conflict:**

**Step 1: Identify Conflicting NFRs**
- Example: Security vs. Usability

**Step 2: Quantify Impact**
- Measure how much each option affects each NFR
- Create trade-off matrix

**Step 3: Prioritize Based on Context**
- Which NFR is more critical for this application?
- Consider: Safety, regulations, market needs

**Step 4: Make Informed Decision**
- Choose option that best meets priorities
- Document rationale
- May need stakeholder approval

**Example:**
```
Trade-off: Performance vs. Energy

Option A: High-speed CPU
- Performance: 100 MIPS (good)
- Energy: 5W (poor)

Option B: Low-power CPU
- Performance: 50 MIPS (acceptable)
- Energy: 1W (excellent)

Decision: Choose Option B if energy is priority
(e.g., battery-powered device)
```

## Comparisons & Contrasts

### Functional vs Non-Functional Requirements

| Aspect | Functional Requirements | Non-Functional Requirements |
|--------|------------------------|---------------------------|
| **Definition** | What the system DOES | What the system IS |
| **Focus** | Behavior, functionality | Quality, characteristics, constraints |
| **Example** | "Show green light to one lane at a time" | "Product cost < 3000 CHF" |
| **Verification** | Test functionality | Measure/assess quality |
| **When specified** | Early in requirements phase | Throughout requirements, some emerge during design |
| **Impact** | Defines if system works | Defines if system works *well* |
| **User visibility** | Directly visible to users | May be invisible but affects user experience |
| **Failure impact** | Feature doesn't work | System works but poorly |

### Execution vs Evolution NFRs

| Aspect | Execution NFRs | Evolution NFRs |
|--------|---------------|----------------|
| **When relevant** | During system operation (runtime) | During development phase |
| **Observable** | When system is running | During development process |
| **Who cares most** | End users, operators | Developers, maintainers |
| **Time horizon** | Entire operational lifetime | Development + maintenance period |
| **Examples** | Energy, Performance, Memory, Security | Testability, Maintainability, Scalability, Development time |
| **Measurement** | Runtime measurements, operational monitoring | Development metrics, code analysis |
| **Impact of failure** | Poor user experience, system failure | High development cost, technical debt |
| **Visibility** | Visible in deployed system | Visible in development process |

### Quantifiable vs Qualifiable NFRs

| Aspect | Quantifiable NFRs | Qualifiable NFRs |
|--------|------------------|------------------|
| **Measurability** | Numeric value | Pass/fail, compliant/non-compliant |
| **Clarity** | Clear threshold | May be subjective |
| **Testability** | Easy to verify objectively | May require expert judgment |
| **Examples** | "Response time < 100ms", "MTBF > 10 years" | "Comply with ISO standard X", "Follow company process" |
| **Preference** | Preferred (more objective) | Less preferred (more subjective) |
| **Verification cost** | Can be automated | May require manual review/audit |
| **Dispute likelihood** | Low (objective) | Higher (subjective) |
| **Example conversion** | "Easy to use" → "Task completion time < 10 min" | "Easy to use" (remains vague) |

### Qualities vs Constraints

| Aspect | Qualities | Constraints |
|--------|-----------|-------------|
| **Nature** | Properties to demonstrate | Limitations to satisfy |
| **Decision type** | Optimization | Go/No-go |
| **Examples** | Performance, Usability, Reliability | Standard compliance, Platform requirements |
| **Flexibility** | Degrees of satisfaction | Must be met (binary) |
| **Role in design** | Guide toward optimal solution | Exclude non-compliant solutions |
| **Trade-offs** | Can trade off against each other | Cannot be violated |
| **Verification** | "How good is it?" | "Does it comply?" |
| **Design impact** | Influence design choices | Eliminate design options |

### Different NFR Categorizations Compared

| Categorization | Categories | Purpose | Use Case |
|----------------|-----------|---------|----------|
| **Function/Non-function** | Functional, Efficiency, Dependability, Security | Separate behavior from quality | General requirements classification |
| **ISO/IEC 25010** | 9 main categories (Functional Suitability, Performance Efficiency, etc.) | Comprehensive software quality model | Systematic NFR identification |
| **Development** | Qualities, Constraints | Distinguish optimization from limitations | Design decision-making |
| **Temporal** | Execution, Evolution | Separate runtime from development-time concerns | Lifecycle management |
| **Measurable** | Quantifiable, Qualifiable | Focus on measurability | Verification planning |
| **Avizienis taxonomy** | Dependability attributes + Security | Academic/research framework | Dependable systems design |

### Common NFR Trade-offs

| Trade-off | Description | Example Context |
|-----------|-------------|-----------------|
| **Security vs. Usability** | More security = more complex = harder to use | Banking app (high security) vs. Game (easy login) |
| **Performance vs. Maintainability** | Optimized code harder to understand and modify | Real-time system vs. Long-term project |
| **Performance vs. Security** | Security checks add overhead and slow system | High-speed trading vs. Secure messaging |
| **Maintainability vs. Time-to-Market** | Clean code takes longer to develop | Startup (speed) vs. Enterprise (quality) |
| **Reliability vs. Cost** | Redundancy and testing increase reliability and cost | Safety-critical system vs. Consumer device |
| **Safety vs. Flexibility** | Safety constraints limit what users can do | Medical device vs. General-purpose computer |
| **Portability vs. Performance** | Platform-specific optimizations not portable | Cross-platform app vs. Native app |
| **Energy vs. Performance** | Higher performance requires more energy | Battery-powered device vs. Plugged-in system |

### Embedded Systems vs General Software - NFR Importance

| NFR | Embedded Systems | General Software |
|-----|-----------------|------------------|
| **Energy** | Critical (battery-powered) | Less critical (plugged in) |
| **Real-time** | Often required (hard deadlines) | Less common (soft deadlines) |
| **Safety** | Often critical (physical consequences) | Less critical (data/usability) |
| **Cost** | Very critical (high volume, BOM) | Less critical (copies free) |
| **Memory** | Highly constrained | Less constrained |
| **Reliability** | Critical (inaccessible, long lifetime) | Important but recoverable |
| **Performance** | Often fixed (can't upgrade) | Can be upgraded |
| **Maintainability** | Long lifetime (10+ years) | Shorter lifecycle |

## Examples & Applications

### Example 1: Traffic Light Control System NFRs

**Context**: Urban traffic light system with pedestrian crossing

**Requirements:**

**R1: Product Cost (Execution NFR, Quantifiable)**
"The product cost of a single traffic light shall not be higher than 3000 CHF @1000 pieces."
- Category: Efficiency (Cost)
- Type: Execution, Quantifiable
- Verification: Calculate BOM + licenses + manufacturing cost

**R2: Physical Constraint (Constraint, Qualifiable)**
"The traffic light control system shall fit a standard traffic control housing according to the SN XXXX, V1.x, 2023"
- Category: Constraint
- Type: Execution, Qualifiable
- Verification: Physical measurement and standard compliance check

**R3: Safety (Functional Requirement - for comparison)**
"The traffic light control shall show a green light to only one lane at a time on the crossroad."
- This is actually a FUNCTIONAL requirement (behavior)
- Included to show contrast with NFRs

**R4: Extensibility (Evolution NFR, Qualifiable)**
"The traffic light control system shall be extensible to enable the use with a 5-road intersection."
- Category: Evolution requirement
- Type: Evolution, Qualifiable (can it support 5 roads? yes/no)
- Verification: Architecture review, test with 5-road configuration

**R5: Manufacturability (Evolution NFR, Qualifiable)**
"It shall be possible to manufacture the traffic light in a fully automated process."
- Category: Evolution requirement
- Type: Evolution, Qualifiable
- Verification: Manufacturing process review

**R6: Reliability (Execution NFR, Quantifiable)**
"The MTBF for a single traffic light shall be larger than 500 years."
- Category: Dependability (Reliability)
- Type: Execution, Quantifiable
- Verification: Calculation from component specifications
- Note: Cannot test directly (500 years!), must calculate

**Analysis:**
- Mix of functional and non-functional requirements
- Shows quantifiable (R1, R6) vs. qualifiable (R2, R4, R5)
- Shows execution (R1, R2, R6) vs. evolution (R4, R5)
- Demonstrates real-world NFR specification

### Example 2: Healthcare.gov Failure Case Study

**Context**:
- U.S. healthcare marketplace website launched October 2013
- Immediate and catastrophic failure
- Only ~6 people successfully registered on launch day out of millions of visitors

**User-Visible Issues:**
- Long page load times (sometimes over 8 minutes)
- Site crashes during registration or checkout
- Error messages without explanations
- Frozen pages and data loss
- System completely unusable for weeks

**Violated NFRs Analysis:**

| NFR Category | Issue | Evidence of Violation |
|--------------|-------|----------------------|
| **Performance** | Not stress-tested for expected load | Could handle ~1,000 concurrent users, but 250,000+ tried to access |
| **Scalability** | No proper load balancing or horizontal scaling | Infrastructure couldn't dynamically scale under load |
| **Reliability** | System crashed repeatedly, data lost | Frequent "500" errors, incomplete transactions |
| **Usability** | Complex registration, unclear errors | Users couldn't understand what failed or how to retry |
| **Maintainability** | Fragmented codebase across contractors | Small changes broke other modules |
| **Security** | Rushed security reviews | Incomplete vulnerability testing before launch |

**Root Causes:**
- NFRs not properly specified or tested
- Focus on functional requirements only
- Inadequate load testing
- Poor system integration
- Rushed timeline

**Lessons:**
- **NFRs are NOT optional**
- **Performance and scalability must be tested early**
- **System integration is critical**
- **Usability affects adoption**
- **Security cannot be an afterthought**

**Cost of Failure:**
- Reputational damage
- Political fallout
- Lost enrollment (people couldn't sign up)
- Expensive recovery effort (months of work)
- Public trust damaged

**Resolution:**
- Major technical overhaul
- Brought in new technical leadership
- Extensive redesign and testing
- Eventually worked, but huge initial cost

### Example 3: Energy Measurement - Washing Machine

**Context**: Measuring energy consumption of embedded washing machine controller

**Setup:**
- Ultra96 board or similar embedded system
- Power analyzer connected to power supply
- Full wash cycle simulation

**Measurement Procedure:**
1. Connect power analyzer
2. Start washing machine cycle
3. Record power consumption over time
4. Generate Power vs. Time graph

**Typical Results:**

```
Phase 1 (0-2 min): Filling water
- Power: 2-3W (valve open, pump off, motor off)

Phase 2 (2-6 min): Heating water
- Power: 8-10W (heater on, high power consumption)

Phase 3 (6-8 min): Washing
- Power: 4-5W (motor rotating, heater off)

Phase 4 (8-10 min): Emptying
- Power: 4-5W (pump on, motor slow)

Phase 5 (10-12 min): Spin dry
- Power: 6W (motor high speed)
```

**Total Energy Calculation:**
- Average power: ~5W
- Cycle time: 12 minutes = 0.2 hours
- Energy: 5W × 0.2h = 1 Wh

**NFR Verification:**
- Requirement: "Energy consumption per cycle < 2 Wh"
- Measured: 1 Wh
- Result: ✓ PASS

**Notes:**
- Shows clear methodology for energy NFR
- Demonstrates straight-forward quantifiable measurement
- Real data from embedded system

### Example 4: Memory Measurement - Ultra96 Firmware

**Context**: Checking code and data size for embedded firmware

**Memory Map File Extract:**
```
Memory Map of the Image

Image Entry point : 0x00000000

Load Region LR_IROM1 (Base: 0x00000000, Size: 0x0000358B, Max: 0x00010000, ABSOLUTE)

Execution Region ER_IROM1 (Exec base: 0x00000000, Load base: 0x00000000,
                            Size: 0x0000358B, Max: 0x00010000, ABSOLUTE)

Exec Addr    Load Addr    Size       Type   Attr    Object
0x00000000   0x00000000   0x00000204  Data   RO      startup_rtboard.o
0x00000204   0x00000204   0x00000204  Code   RO      system_init.o
0x00000408   0x00000408   0x00001234  Code   RO      main.o
...
```

**Analysis:**
- Code size: 13,899 bytes (0x358B)
- Max available: 65,536 bytes (0x10000) = 64KB
- Usage: 21.2%
- Margin: 78.8% (51,637 bytes free)

**NFR Verification:**
- Requirement: "Code size shall not exceed 64 KB"
- Measured: 13.9 KB
- Result: ✓ PASS with good margin

**RAM Analysis:**
```
Load Region LR_IRAM1 (Base: 0x20000000, Size: 0x00003500, Max: 0x00008000)

Execution Region RW_IRAM1 (Exec base: 0x20000000, Size: 0x00003500)

0x20000000   0x00000000   0x00000200  Data   RW      globals.o
0x20000200   0x00000000   0x00001000  Data   RW      buffers.o
0x20001200   0x00000000   0x00000300  Zero   ZI      bss.o
```

- Data size: 13,568 bytes (0x3500)
- Max available: 32,768 bytes (0x8000) = 32KB
- Usage: 41.4%

**Notes:**
- Demonstrates straight-forward memory measurement
- Linker map file provides all needed information
- Important to leave margin for stack and future growth

### Example 5: MTBF Calculation - Reliability Requirement

**Context**: Traffic light system reliability requirement

**Requirement**: "The MTBF for a single traffic light shall be larger than 500 years."

**Approach**: Cannot test 500 years! Must calculate from component data.

**Calculation Method:**

**1. System Components:**
- MCU: MTBF = 1,000,000 hours
- LEDs: MTBF = 100,000 hours (each)
- Power supply: MTBF = 200,000 hours
- Sensors: MTBF = 150,000 hours

**2. System Failure Rate:**
For series system (any component failure causes system failure):
```
System Failure Rate = Sum of component failure rates
λ_system = λ_MCU + λ_LEDs + λ_power + λ_sensors

λ = 1/MTBF

λ_system = 1/1,000,000 + 3/100,000 + 1/200,000 + 1/150,000
         = 0.000001 + 0.00003 + 0.000005 + 0.0000067
         = 0.0000427 failures/hour
```

**3. System MTBF:**
```
MTBF_system = 1 / λ_system
            = 1 / 0.0000427
            = 23,419 hours
            = 2.67 years
```

**Result**:
- Calculated MTBF: 2.67 years
- Required MTBF: 500 years
- **✗ FAIL** - System does not meet requirement

**Solution Options:**
1. Use more reliable components (higher MTBF)
2. Add redundancy (parallel components)
3. Improve environmental conditions (temperature control)
4. Negotiate requirement (is 500 years realistic?)

**With Redundancy:**
If we add redundant LEDs and power supply:
- MTBF can improve significantly
- Calculation becomes more complex (parallel systems)
- May be able to meet 500-year target

**Notes:**
- Shows quantifiable NFR that cannot be tested directly
- Demonstrates calculation-based verification
- Highlights importance of early analysis
- May reveal unrealistic requirements

### Example 6: Usability NFR - Making It Measurable

**Initial (Vague) Requirement:**
"The system shall be easy to use."

**Problem**:
- Not testable
- Subjective
- No clear success criterion

**Making It Measurable - Iteration 1:**
"New users shall be able to operate the system after training."

**Better, but still issues**:
- How much training?
- What does "operate" mean?
- Still subjective

**Making It Measurable - Iteration 2:**
"After 1 hour of training, new users shall be able to complete basic task list X in under 10 minutes with less than 2 errors."

**Analysis:**
- ✓ Specific training duration (1 hour)
- ✓ Specific tasks (list X)
- ✓ Measurable completion time (< 10 min)
- ✓ Measurable error rate (< 2 errors)
- ✓ Testable (user study)

**Verification Procedure:**
1. Define "basic task list X" (e.g., start system, configure parameters, run operation, view results)
2. Create standard 1-hour training module
3. Recruit 10 representative users
4. Conduct user study:
   - Give training
   - Ask users to complete task list
   - Measure: completion time, errors
5. Analyze results:
   - Average time
   - Average errors
   - Pass/fail determination

**Example Results:**
```
User 1: 8 min, 1 error ✓
User 2: 12 min, 0 errors ✗ (too slow)
User 3: 7 min, 2 errors ✓
...
User 10: 9 min, 1 error ✓

Average: 9.2 min, 1.3 errors
Result: ✓ PASS (8 out of 10 users passed)
```

**Lessons:**
- Convert qualitative to quantitative
- Specify: who, what, how much, when
- Make success criteria objective
- Plan verification method

### Example 7: Security vs. Usability Trade-off

**Scenario 1: Mobile Banking App**

**Security Priority HIGH**

Option A: High Security
- Multi-factor authentication (password + SMS + fingerprint)
- Session timeout after 2 minutes
- Complex password requirements (12+ chars, special chars, etc.)
- Regular password changes (every 30 days)
- **Usability: LOW** (annoying, time-consuming)
- **Security: HIGH** (very difficult to compromise)

**Decision**: Choose Option A despite low usability
- **Rationale**: Financial data, regulatory requirements, high risk
- **Context**: Banking requires maximum security

**Scenario 2: Mobile Game**

**Usability Priority HIGH**

Option B: High Usability
- Social login (Google, Facebook)
- Remember me (stay logged in)
- Simple password (6+ characters)
- No password expiry
- **Usability: HIGH** (quick, convenient)
- **Security: MEDIUM** (some risk)

**Decision**: Choose Option B despite lower security
- **Rationale**: Low-value data, user retention important, competitive market
- **Context**: Games need easy onboarding

**Key Insight**: Same trade-off, different decisions based on context

### Example 8: Performance vs. Energy Trade-off

**Context**: Wearable fitness tracker

**Option A: High-Performance CPU**
- Processor: ARM Cortex-A53 @ 1.4 GHz
- Performance: 2000 DMIPS (Dhrystone MIPS)
- Power consumption: 300 mW
- Battery life: 1 day
- User experience: Fast, responsive
- Cost: $15 per unit

**Option B: Low-Power CPU**
- Processor: ARM Cortex-M4 @ 80 MHz
- Performance: 100 DMIPS
- Power consumption: 30 mW
- Battery life: 10 days
- User experience: Adequate for task
- Cost: $3 per unit

**Analysis:**

| Factor | Option A | Option B | Winner |
|--------|----------|----------|--------|
| Performance | 2000 DMIPS | 100 DMIPS | A |
| Energy | 300 mW | 30 mW | B |
| Battery life | 1 day | 10 days | B |
| Cost | $15 | $3 | B |
| User experience | Excellent | Good | A |

**Decision: Choose Option B**

**Rationale:**
1. **Primary user pain point**: Frequent charging (1 day battery)
2. **Sufficient performance**: 100 DMIPS adequate for fitness tracking
3. **Cost sensitive**: High-volume consumer product
4. **Market differentiation**: 10-day battery life is compelling feature

**Implementation:**
- Use efficient algorithms optimized for M4
- Offload complex processing to smartphone (BLE communication)
- Sacrifice some features that require high performance
- Result: Successful product with excellent battery life

**Lesson**: Not always about maximum performance—match performance to actual needs

## Connections to Other Topics

### Connection to Lecture 01: Embedded Systems

**Cyber-Physical Systems:**
- NFRs especially critical for cyber-physical systems
- Physical consequences of violations (safety, timing)
- Energy constraints common in embedded systems
- Real-time requirements link to performance NFRs

**Design Space Exploration:**
- NFRs define the design space dimensions (Cost, Energy, Performance)
- Trade-offs between NFRs central to design space exploration
- Optimal design point balances multiple NFRs

### Connection to Lecture 02: Software Paradigms

**Software Architecture and NFRs:**
- Architecture choices impact NFR achievement
- Event-driven vs. time-driven affects real-time performance
- Object-oriented affects maintainability
- Procedural may be more efficient but less maintainable

**Paradigm Selection:**
- NFRs guide selection of appropriate paradigm
- Safety-critical: More structured approaches
- Performance-critical: May need lower-level control

### Connection to Lecture 03: Requirements Engineering

**Requirements Characteristics:**
- NFRs must meet same quality criteria as functional requirements
- Testable, unambiguous, clear, correct, etc.
- Priority assignment applies to NFRs
- RFC 2119 keywords applicable (SHALL, SHOULD, MAY, etc.)

**Requirements Specification:**
- NFRs part of complete requirements document
- Must be specified alongside functional requirements
- Often cross-cutting (affect multiple functional areas)

### Connection to Lecture 04: Modelling

**UML and NFRs:**
- State machines can model timing constraints
- Performance can be analyzed from state machine timing
- WCET (Worst-Case Execution Time) relates to performance NFRs

**Model-Based Analysis:**
- Models can be analyzed for NFR compliance
- UPPAAL: Model checking for timing properties
- Early verification of timing, safety NFRs

**Code Generation:**
- Generated code must meet NFRs
- Code generator efficiency affects performance
- Safety-critical: Need certified code generation

### Connection to Lecture 05: Development Processes

**Process Selection and NFRs:**
- Safety-critical systems: V-Model (emphasis on verification)
- Time-to-market critical: Agile (rapid delivery)
- Cost-critical: May influence process choice

**Verification at Each Level:**
- V-Model: Verification strategy for each abstraction level
- NFRs verified at appropriate levels
- Integration testing for system-level NFRs

**CI/CD and NFRs:**
- Automated testing includes NFR verification
- Performance regression testing
- Energy consumption monitoring
- Code size tracking

**Abstraction Levels:**
- NFRs specified and verified at different levels
- System level: Overall energy, cost, performance
- Module level: Component-specific NFRs
- Block level: Algorithm efficiency

### Connection to Lecture 08: Energy (Future Topic)

**Energy as Primary NFR:**
- Detailed study of energy consumption
- Measurement techniques (power analyzer)
- Optimization strategies
- Trade-offs with performance

**Preview:**
- Energy budgets for embedded systems
- Dynamic vs. static power
- Energy-aware design techniques

### Connection to Lecture 09: Performance (Future Topic)

**Performance NFRs Deep Dive:**
- Response time, throughput, latency
- Performance measurement and profiling
- Optimization techniques
- Real-time constraints

**Preview:**
- Timing analysis
- Worst-case execution time (WCET)
- Performance modeling

### Connection to Lecture 10-12: FPGA and DSE (Future Topics)

**Hardware/Software Trade-offs:**
- FPGA for performance or energy optimization
- Hardware acceleration vs. software flexibility
- Design space exploration considers multiple NFRs simultaneously

**DSE Process:**
- Systematic exploration of NFR trade-offs
- Pareto-optimal solutions
- Multi-objective optimization

### Connection to Lecture 13: RTOS Scheduling (Future Topic)

**Real-Time NFRs:**
- Scheduling algorithms affect timing guarantees
- Priority assignment affects responsiveness
- RTOS overhead affects performance

**Timing Constraints:**
- Hard vs. soft real-time requirements
- Schedulability analysis verifies timing NFRs

### Connection to Lab Exercises

**P1: Hardware Bring-up:**
- Physical constraints (board dimensions, connectors)
- Cost constraints (BOM)
- Development time

**U01: Requirements Modeling:**
- Specifying both functional and non-functional requirements
- Washing machine timing requirements
- Safety requirements (interlocks)

**U2/U3: Implementation & Testing:**
- Testability NFRs
- Code quality, maintainability

**P2: Energy Analysis:**
- Direct application of energy NFRs
- Measurement and verification

**P3-P5: Performance Optimization:**
- Performance NFRs in practice
- Trade-offs between implementations

### Connection to Industry Practice

**ISO 26262 (Automotive Safety):**
- Safety NFRs in automotive systems
- ASIL levels define safety requirements
- Relationship between safety and other NFRs

**DO-178C (Aviation Software):**
- Certification requirements for avionics
- Safety and reliability NFRs
- Verification and validation requirements

**IEC 61508 (Functional Safety):**
- Generic safety standard
- SIL levels (Safety Integrity Levels)
- Applies to embedded systems across industries

**GDPR (Data Protection):**
- Security and privacy NFRs
- Data protection by design
- Affects embedded systems handling personal data

## Summary

**Core Messages:**
1. Non-functional requirements describe what the system should BE, not what it should DO
2. NFRs are critical for embedded systems—they often determine success or failure
3. NFRs encompass efficiency, dependability, and security
4. Multiple classification schemes exist; no single "correct" taxonomy
5. Prefer quantifiable NFRs for objective verification
6. NFRs often conflict, requiring trade-off decisions based on context
7. Early consideration of NFRs is essential to avoid costly late-stage problems
8. Good requirements (including NFRs) must be testable, clear, and unambiguous
9. The earlier NFR compliance can be confirmed, the better
10. Context determines which trade-offs are appropriate

**Key Takeaways:**
- **NFRs ≠ "nice to have"** – they are essential requirements
- **"Works" ≠ "Works well"** – NFRs distinguish the two
- **Measure whenever possible** – quantifiable preferred over qualifiable
- **Consider context** – medical device vs. mobile game have different priorities
- **Trade-offs are inevitable** – must make informed decisions
- **Plan verification early** – know how you'll test before you build
- **Document rationale** – explain trade-off decisions for future reference

## Possible Exam Questions

### Basic Definitions (Understanding Terminology)

1. **What is a non-functional requirement? How does it differ from a functional requirement?**
   - Expected: Definition emphasizing "what the system should BE vs. DO"
   - Should mention: Criteria for judging system quality, not behavior

2. **List and explain the three main categories of non-functional requirements according to the Avizienis taxonomy.**
   - Expected: Efficiency, Dependability, Security with subcategories
   - Note overlap between Dependability and Security

3. **What does MTBF stand for and what does it measure?**
   - Expected: Mean Time Between Failures
   - Measure of reliability
   - Higher MTBF = more reliable system

4. **Explain the difference between quantifiable and qualifiable NFRs. Give one example of each.**
   - Expected: Quantifiable = numeric values; Qualifiable = pass/fail
   - Examples: "Response time < 100ms" vs. "Comply with ISO standard X"

5. **What are the alternative names used for non-functional requirements in literature?**
   - Expected: Quality attributes, Constraints, Technical requirements, "-ilities"

6. **Define "constraints" in the context of non-functional requirements.**
   - Expected: Limitations, standards, and environmental factors that must be fulfilled
   - Examples: Standards, processes, platform requirements

7. **What is the difference between "qualities" and "constraints" in NFR classification?**
   - Expected: Qualities = properties to demonstrate; Constraints = limitations to satisfy
   - Qualities optimize; Constraints eliminate options

8. **List five examples of quantifiable NFRs.**
   - Expected answers might include: Energy consumption, Response time, Memory usage, Cost, MTBF, Code size, etc.

9. **What does the Product Cost formula include?**
   - Expected: BOM + Software License Fees + Manufacturing Cost + Manufacturing Overhead

10. **Explain the concept of "Design Space" in the context of embedded systems.**
    - Expected: Multi-dimensional space with axes like Cost, Energy, Performance
    - Different implementation points represent different trade-offs

### Conceptual Understanding (Explaining Concepts)

11. **Explain why NFRs are particularly important in embedded systems compared to general software.**
    - Expected points:
      - Resource constraints (memory, energy)
      - Safety-critical applications
      - Long product lifetime
      - Physical consequences of failure
      - Cannot easily upgrade hardware

12. **Describe the difference between Execution NFRs and Evolution NFRs. Give three examples of each.**
    - Expected:
      - Execution: relevant during operation (Energy, Performance, Memory)
      - Evolution: relevant during development (Maintainability, Testability, Scalability)

13. **Explain the three-step process for deriving measurable criteria from qualitative NFRs.**
    - Expected:
      1. Identify Quality Factor
      2. Define Measurable Criteria
      3. Determine Realization Method
    - Example: Usability → Learning time → Count hours in user study

14. **Why is it preferred to have quantifiable rather than qualifiable NFRs?**
    - Expected:
      - Objective verification
      - Clear pass/fail criteria
      - Less ambiguity
      - Easier to test
      - Reduced disputes

15. **Explain how the ISO/IEC 25010 standard categorizes software quality requirements.**
    - Expected:
      - 9 main quality characteristics
      - Each with sub-characteristics
      - Comprehensive framework
      - Examples: Functional Suitability, Performance Efficiency, Reliability, Security, etc.

16. **Describe the role of NFRs in guiding embedded system design.**
    - Expected:
      - Guide design toward optimal solution
      - Constraints eliminate non-compliant options
      - Qualities optimize among remaining options
      - Funnel/pyramid effect: many options → optimal solution

17. **Explain why it's important to verify NFR compliance as early as possible in the design process.**
    - Expected:
      - Cheaper to fix issues early
      - Avoid late-stage redesign
      - May affect fundamental architecture
      - Reduces project risk

18. **What are the main challenges associated with non-functional requirements?**
    - Expected:
      - Difficult to write testably
      - Difficult to implement
      - Time-consuming and expensive to test
      - Can conflict with each other
      - Can impact functionality if not properly handled

### Application/Analysis (Applying Knowledge)

19. **Classify the following requirements as functional or non-functional. For non-functional requirements, further classify them:**
    a) "The system shall respond to user input within 200ms"
    b) "The system shall calculate the square root of a number"
    c) "The product cost shall not exceed $50 per unit"
    d) "The system shall comply with ISO 9001"

    - Expected:
      - a) NFR - Performance (Execution, Quantifiable)
      - b) Functional
      - c) NFR - Cost (Execution, Quantifiable)
      - d) NFR - Constraint (Qualifiable)

20. **For each of the following vague NFRs, provide a quantifiable version:**
    a) "The system shall be fast"
    b) "The system shall be reliable"
    c) "The system shall use little energy"

    - Expected improvements:
      - a) "System response time shall be < 100ms for 95% of requests"
      - b) "System MTBF shall be > 10 years"
      - c) "Average power consumption shall be < 5W during normal operation"

21. **A traffic light system has the following components with MTBF values:**
    - MCU: 500,000 hours
    - LEDs: 50,000 hours (3 units)
    - Power supply: 100,000 hours

    **Calculate the system MTBF. Does it meet a requirement of 5 years?**

    - Expected calculation:
      - System failure rate = 1/500000 + 3/50000 + 1/100000
      - Calculate result
      - Convert to years
      - Compare to requirement

22. **Identify three execution NFRs and three evolution NFRs that would be relevant for a battery-powered wearable fitness tracker.**
    - Expected:
      - Execution: Energy consumption, Size/weight, Response time
      - Evolution: Maintainability, Testability, Portability (to different platforms)

23. **The following requirement needs improvement: "The embedded system shall not use too much memory." Rewrite it as a good NFR.**
    - Expected:
      - Specific threshold: "< 32 KB RAM"
      - Clear scope: "during normal operation"
      - Testable: "verified via linker map file"
      - Example: "The system RAM usage shall not exceed 32 KB during normal operation, verified via linker map file analysis"

24. **Explain how you would verify the following NFR: "The system shall be easy for new users to learn."**
    - Expected:
      1. Make it quantifiable (e.g., "Complete task X in < 10 min after 1 hour training")
      2. Design user study with representative users
      3. Measure task completion time and errors
      4. Statistical analysis of results
      5. Pass/fail determination based on threshold

25. **For a washing machine control system, suggest appropriate measurable criteria for the NFR "The system shall be maintainable."**
    - Expected metrics:
      - Time to implement typical feature addition (e.g., < 2 person-days)
      - Code complexity metrics (cyclomatic complexity < 10)
      - Test coverage (> 80%)
      - Documentation completeness

### Comparisons (Analyzing Differences)

26. **Compare and contrast quantifiable and qualifiable NFRs in terms of verification approach and preference.**
    - Expected table or structured comparison
    - Key differences: measurability, objectivity, testability
    - Why quantifiable preferred

27. **Discuss the relationship between Dependability and Security. What are the overlapping concepts?**
    - Expected:
      - Overlap: Availability, Integrity
      - Dependability unique: Reliability, Safety, Maintainability
      - Security unique: Confidentiality
      - Diagram showing Venn overlap

28. **Compare how the following trade-off would be handled differently for:**
    - a) A medical device
    - b) A mobile game

    **Trade-off: Security vs. Usability**

    - Expected:
      - Medical: Prioritize security (patient safety, regulations)
      - Game: Prioritize usability (user retention, competitive market)
      - Different contexts = different priorities

29. **Explain the difference between "software that works" and "software that works well." How do NFRs relate to this distinction?**
    - Expected:
      - "Works" = functional requirements met
      - "Works well" = NFRs met
      - Example: Healthcare.gov had functionality but poor NFRs

30. **Compare Execution and Evolution NFRs in terms of:**
    - Who cares about them
    - When they are relevant
    - How they are measured

    - Expected structured comparison addressing all three aspects

### Critical Thinking (Analysis and Synthesis)

31. **Analyze the healthcare.gov case study. Identify which specific NFRs were violated and explain the consequences.**
    - Expected:
      - Performance: couldn't handle load
      - Scalability: no proper scaling
      - Reliability: frequent crashes
      - Usability: unclear errors
      - Consequences: Only 6 successful registrations on launch day

32. **You are designing a battery-powered IoT sensor that will be deployed in remote locations. It has the following constraints:**
    - Battery life: minimum 5 years
    - Cost: maximum $10 per unit
    - Data transmission: every hour

    **Discuss the key NFRs and the trade-offs you would need to consider.**

    - Expected:
      - Primary NFRs: Energy (critical), Cost, Reliability
      - Trade-offs: Performance vs. Energy (low-power CPU)
      - Communication protocol choice (energy efficiency)
      - Redundancy vs. Cost (for reliability)

33. **Explain why the requirement "The MTBF shall be > 500 years" cannot be directly tested. How would you verify this requirement?**
    - Expected:
      - Cannot run 500-year test (impractical)
      - Must calculate from component specifications
      - Use reliability theory (failure rates)
      - Consider operating conditions
      - Possibly accelerated life testing

34. **Discuss why NFRs often conflict with each other. Give three examples of conflicting NFR pairs and explain the nature of the conflict.**
    - Expected examples:
      - Security vs. Usability (more security = more complex)
      - Performance vs. Maintainability (optimized code = less readable)
      - Reliability vs. Cost (redundancy expensive)
      - Explanation of why they conflict

35. **A project manager says: "We'll focus on functional requirements first and worry about NFRs later." Explain why this is a problematic approach.**
    - Expected:
      - NFRs affect fundamental architecture
      - Late-stage changes expensive
      - May require complete redesign
      - Example: Adding scalability late is difficult
      - NFRs guide design from the start

36. **You have two CPU options for an embedded system:**
    - Option A: High-performance, high-energy, expensive
    - Option B: Low-performance, low-energy, cheap

    **For each of the following applications, choose an option and justify:**
    a) Safety-critical medical device
    b) High-volume consumer IoT sensor
    c) Autonomous vehicle

    - Expected: Different choices based on context
    - Justification considering relevant NFRs for each application

37. **Explain the "funnel" or "pyramid" concept in NFR-driven design. How do constraints and qualities play different roles?**
    - Expected:
      - Start with many possible implementations (wide top)
      - Constraints eliminate non-compliant options (narrowing)
      - Qualities optimize among remaining options (narrowing further)
      - Result: Optimal solution (narrow bottom)

38. **Why is it important to have standardized NFR taxonomies like ISO/IEC 25010? What are the benefits for a development team?**
    - Expected:
      - Common language for communication
      - Comprehensive checklist (don't forget important NFRs)
      - Industry best practices
      - Easier to compare systems
      - Facilitates requirements elicitation

### Scenario-Based (Real-World Application)

39. **You are developing a traffic light control system. Write three specific NFRs (one execution/quantifiable, one evolution/quantifiable, one constraint/qualifiable) that would be appropriate.**
    - Expected format similar to lecture examples
    - Proper structure and specificity
    - Testability considerations

40. **A client says: "I need a system that is very reliable." Your task is to turn this into a well-specified NFR. Show your process step by step.**
    - Expected:
      1. Clarify what "reliable" means (availability? fault tolerance? MTBF?)
      2. Determine measurable metric (e.g., MTBF)
      3. Establish threshold (e.g., > 10 years)
      4. Specify verification method
      5. Final requirement: "System MTBF shall be > 10 years, calculated from component specifications"

41. **Your embedded system design currently fails to meet an energy consumption NFR. List five potential strategies to reduce energy consumption and the trade-offs each might involve.**
    - Expected strategies:
      1. Use lower-power CPU (trade-off: performance)
      2. Reduce clock frequency (trade-off: slower execution)
      3. Implement sleep modes (trade-off: wake-up latency)
      4. Optimize algorithms (trade-off: development time)
      5. Use hardware acceleration (trade-off: cost, complexity)

42. **You need to measure the usability of an embedded system interface. Design a usability test procedure including:**
    - What you will measure
    - How you will measure it
    - What threshold constitutes "pass"

    - Expected:
      - Define tasks (e.g., "Configure device", "Start operation")
      - Metrics: Task completion time, errors, user satisfaction
      - Procedure: User study with N representative users
      - Threshold: e.g., "80% of users complete tasks in < 10 min with < 2 errors"

43. **Analyze this requirement: "The embedded system shall be developed using the V-Model process." Is this an NFR? If so, what type? How would you verify it?**
    - Expected:
      - Yes, it's an NFR
      - Type: Constraint (evolution, qualifiable)
      - Verification: Process audit, review of development artifacts
      - Check for V-Model documentation at each phase

44. **A wearable fitness tracker project has these NFRs:**
    - NFR1: Battery life > 7 days
    - NFR2: Response time < 100ms
    - NFR3: Cost < $30 per unit

    **Your current design achieves:**
    - Battery life: 3 days
    - Response time: 50ms
    - Cost: $25

    **Which NFR should you prioritize improving? Justify your answer.**

    - Expected reasoning:
      - NFR1 (battery) is failing (3 < 7 days)
      - NFR2 and NFR3 are passing
      - For wearable, battery life critical to user experience
      - Prioritize battery even if it means sacrificing some performance/cost margin

### Connections (Relating to Other Topics)

45. **How do the abstraction levels from Lecture 05 (System, Module, Block, Logic, Device) relate to NFR specification and verification?**
    - Expected:
      - Different NFRs relevant at different levels
      - System level: Overall energy, cost, performance
      - Module level: Component-specific NFRs
      - Verification strategy matches abstraction level

46. **Explain how the development process choice (Waterfall, V-Model, Agile) from Lecture 05 might be influenced by different NFRs.**
    - Expected:
      - Safety-critical (high safety NFR): V-Model (emphasis on verification)
      - Time-to-market critical: Agile (rapid delivery)
      - Well-defined stable requirements: Waterfall
      - NFRs influence process selection

47. **How does the choice of software paradigm (from Lecture 02) affect the achievement of different NFRs?**
    - Expected:
      - Event-driven: Better responsiveness, may affect timing determinism
      - Object-oriented: Better maintainability, potential performance overhead
      - Procedural: Better efficiency, may sacrifice maintainability

48. **In Lecture 04, we learned about WCET (Worst-Case Execution Time). How does WCET relate to NFRs?**
    - Expected:
      - WCET is a performance NFR metric
      - Critical for real-time systems
      - Can be analyzed from models before implementation
      - Relates to timing constraints and deadlines

49. **Explain how NFRs connect to the Design Space Exploration topics that will be covered in future lectures.**
    - Expected:
      - DSE explores trade-offs between multiple NFRs
      - Design space dimensions are NFRs (energy, cost, performance)
      - Goal: Find Pareto-optimal solutions
      - Systematic exploration of NFR trade-offs

50. **The healthcare.gov case study showed massive failures. Relate this to the V-Model from Lecture 05. What went wrong from a process perspective?**
    - Expected:
      - Inadequate verification at each level
      - NFRs not tested until late
      - No proper integration testing
      - V-Model emphasizes verification—wasn't followed properly
      - Lesson: Process compliance is itself an NFR

### Advanced/Synthesis Questions

51. **Design a complete set of NFRs for a battery-powered remote medical monitoring device. Include at least 8 NFRs covering different categories (Efficiency, Dependability, Security). Make them all quantifiable where possible.**
    - Expected comprehensive list with proper specification

52. **Critique this set of requirements. Identify which are functional vs. non-functional, and improve any that are poorly written:**
    a) "The system shall be fast and efficient"
    b) "The system shall monitor patient heart rate every second"
    c) "The system shall be built according to industry standards"
    d) "The system shall not use more than 50mW of power"

    - Expected analysis and improvements for each

53. **You are reviewing a requirements document for an embedded system. You notice it contains 50 functional requirements but only 3 NFRs (all qualifiable). What questions would you ask? What additional NFRs would you recommend adding?**
    - Expected:
      - Concern about missing NFRs
      - Questions about energy, performance, cost, reliability
      - Recommend quantifiable NFRs in each major category

54. **Explain how the concept of "technical debt" relates to NFRs, particularly evolution NFRs like maintainability and testability.**
    - Expected:
      - Sacrificing maintainability for speed creates technical debt
      - Poor testability makes future changes risky
      - Evolution NFRs affect long-term costs
      - Trade-off: Time-to-market vs. long-term maintainability

55. **A system has been deployed and users complain it's "unreliable." However, testing showed it met all functional requirements. What NFRs might have been missed or inadequately specified? How would you identify and address this?**
    - Expected:
      - Likely missing reliability NFRs (MTBF, availability, fault tolerance)
      - Possibly usability issues (users perceive as "unreliable")
      - Process: Gather failure data, conduct user studies
      - Specify proper reliability NFRs, implement verification
