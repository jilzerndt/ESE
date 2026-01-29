# Lecture 12: Design Space Exploration (DSE) - Search Algorithms

## Technical Terms & Definitions

### Search and Optimization Terms
- **Search Strategy**: A systematic method for exploring the design space to find optimal or near-optimal solutions
- **Linear Optimization**: Optimization where objectives and constraints are linear functions of decision variables; can be solved efficiently with specialized solvers
- **Non-linear Optimization**: Optimization where objectives or constraints involve non-linear relationships; generally more complex to solve
- **Exhaustive Search**: Evaluating every possible design point in the decision space; guarantees finding optimal solutions but computationally expensive
- **Heuristic Search**: Using rules of thumb or intelligent strategies to explore promising regions of the design space without exhaustive evaluation
- **Feasible Solution**: A design point that satisfies all specified constraints
- **Infeasible Solution**: A design point that violates one or more constraints

### Solution Representation Terms
- **Solution Encoding**: The representation format used to describe a design point in a computer-readable form
- **Binary Representation**: Encoding design decisions as vectors of 0s and 1s (e.g., x = (0,1,0,1,1,0))
- **Solution Vector**: A vector containing all decision variable values for a design point
- **Allocation Vector**: Binary vector indicating which hardware resources are selected (1 = selected, 0 = not selected)
- **Binding Vector**: Binary vector indicating which tasks are mapped to which resources
- **RTE Vector (Runtime Environment)**: Binary vector encoding scheduling and runtime configuration decisions
- **Solution Length**: The dimensionality of the solution vector (number of decision variables)

### Evolutionary Algorithm Terms
- **Evolutionary Algorithm (EA)**: A population-based metaheuristic optimization algorithm inspired by biological evolution
- **Population**: A set of candidate solutions maintained by an evolutionary algorithm (e.g., 100 solutions)
- **Individual**: A single solution within the population
- **Generation**: One iteration of the evolutionary loop, producing a new set of offspring solutions
- **Offspring**: New solutions created by variation operators from parent solutions
- **Parent Solutions**: Existing solutions selected to create offspring through variation
- **Child Solutions**: New solutions created from parent solutions via mutation or recombination

### Variation Operators
- **Variation**: The process of creating new solutions from existing ones using mutation and/or recombination
- **Mutation**: A variation operator that makes small random changes to a single parent solution
- **Recombination (Crossover)**: A variation operator that combines parts of two or more parent solutions to create offspring
- **Mutation Rate**: The probability of changing each element in a solution during mutation
- **Crossover Point**: The position in the solution vector where parent solutions are split and recombined

### Selection Terms
- **Selection**: The process of choosing which solutions to use for variation or which to keep in the population
- **Selection for Variation**: Choosing parent solutions to create offspring (typically favors better solutions)
- **Selection for Survival**: Choosing which solutions to keep in the population after offspring are created (removes dominated solutions)
- **Fitness Value**: Numerical score(s) representing solution quality based on objective function evaluation
- **Fitness Assignment**: The process of evaluating solutions and assigning fitness values
- **Tournament Selection**: Selection method where k random solutions compete and the best is chosen
- **Elitism**: Strategy ensuring the best solutions are always preserved in the next generation

### Convergence and Performance Terms
- **Convergence**: The tendency of the population to approach the Pareto-optimal front over successive generations
- **Premature Convergence**: When the EA stops improving before finding the true Pareto front (gets stuck in local optimum)
- **Diversity**: The spread of solutions across different regions of objective space
- **Exploration**: Searching broadly across the design space to discover diverse solutions
- **Exploitation**: Intensively searching near known good solutions to find local improvements
- **Stopping Criterion**: Condition for terminating the evolutionary search (e.g., generation limit, no improvement for N generations)

### Evaluation Terms
- **Fitness Evaluation**: Computing objective values for a solution by executing allocation, binding, scheduling, and performance analysis
- **Evaluation Speed**: How quickly a single solution can be evaluated
- **Evaluation Accuracy**: How precisely the evaluation captures real system behavior
- **Evaluation Effort**: Computational resources required for evaluation (time, memory, complexity)
- **Model-based Evaluation**: Using mathematical models or simulations to estimate performance
- **Measurement-based Evaluation**: Using real prototypes or hardware to measure performance

### DSE Quality Metrics
- **Accuracy (Evaluation)**: How well the evaluation captures relevant system properties
- **Speed (Evaluation)**: Execution time for evaluating a single design point
- **Confidence (Search)**: Reliability of result quality from the search algorithm
- **Convergence (Search)**: How well the search algorithm approaches optimum results
- **Effort (Setup)**: Work required to set up the DSE (model creation, problem formulation)
- **Coverage**: Percentage of the design space explored by the search

### Problem-Specific Terms
- **Design Variable**: A parameter that can be varied during exploration (e.g., area planted, processor type)
- **Constraint**: A requirement that limits feasible solutions (e.g., x₁ + x₂ ≤ 100 m²)
- **Objective Function**: Mathematical expression computing an objective value from decision variables
- **Linear Dependency**: Relationship where objective changes proportionally with decision variable
- **Non-linear Dependency**: Complex relationship between decision variables and objectives

## Core Concepts

### 1. Search Strategies for DSE

**Fundamental Challenge:**
Given a design space with potentially millions or billions of design points, how do we efficiently find Pareto-optimal solutions?

**Three Main Approaches:**

**A. Analytical/Mathematical Optimization:**
- **When applicable**: Linear objectives, linear constraints, continuous variables
- **Method**: Use optimization solvers (linear programming, convex optimization)
- **Example**: Online-optimizer.appspot.com for linear problems
- **Advantages**: Finds exact optimal solutions quickly
- **Limitations**: Only works for specific problem classes (linear, convex)

**B. Exhaustive Enumeration:**
- **When applicable**: Small design spaces (< 10,000 design points)
- **Method**: Evaluate every possible design, identify Pareto front
- **Advantages**: Guaranteed to find all Pareto-optimal solutions
- **Limitations**: Computationally infeasible for large spaces

**C. Heuristic/Metaheuristic Search:**
- **When applicable**: Large design spaces, non-linear relationships, complex constraints
- **Method**: Evolutionary algorithms, simulated annealing, genetic algorithms
- **Advantages**: Scales to huge design spaces, handles non-linearity
- **Limitations**: No guarantee of finding true optimum, stochastic results

**Key Insight:**
The choice of search strategy depends on:
1. Size of design space
2. Linearity of objectives/constraints
3. Evaluation cost per design point
4. Required solution quality vs available time

### 2. Solution Representation (Encoding)

**Why Representation Matters:**
The way we encode solutions affects:
- How easily we can generate new solutions
- Whether variation operators produce valid solutions
- Computational efficiency of the search

**Binary Vector Representation:**

For complex systems, a solution is encoded as a concatenated binary vector:

```
Solution_x = (Allocation_x, Binding_x, RTE_x)
          = (x₁, x₂, x₃, ..., xₙ) where xᵢ ∈ {0, 1}
```

**Example: Network Processor**

**Allocation Part:**
```
Hardware elements: [DSP1, DSP2, DSP3, ARM9₁, ARM9₂, ARM9₃, ARM9₄,
                    Checksum, Cipher1, Cipher2, PowerPC1, PowerPC2, PowerPC3]

Allocation_x = (0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1)
```
Meaning: Select DSP2, Checksum, Cipher2, PowerPC3

**Binding Part:**
```
Tasks: [Voice Encoder, RTP Tx, UDP Tx, ...]
Resources: [DSP1, DSP1, DSP1, DSP2, DSP2, DSP2, ...]

Binding_x = (0, 1, 0, 0, ..., 1, 0, 0, ...)
```
Encodes which task runs on which resource instance

**Advantages of Binary Encoding:**
- Computer-friendly (efficient storage and manipulation)
- Standard variation operators (bit-flip mutation, crossover) applicable
- Easy to generate random solutions

**Disadvantages:**
- Not human-readable
- May generate infeasible solutions (require validity checking)
- Large solution vectors for complex systems

### 3. Evolutionary Algorithms for DSE

**Core Principle:**
Maintain a **population** of good solutions and iteratively **evolve** them toward the Pareto-optimal front through:
1. **Variation** (creating offspring from parents)
2. **Evaluation** (computing fitness)
3. **Selection** (choosing which solutions survive)

**Why EAs Work for DSE:**
- Handle huge design spaces (don't need to evaluate every point)
- Work with non-linear objective functions
- Handle multiple objectives naturally
- Balance exploration (diversity) and exploitation (refinement)
- Can be parallelized (evaluate multiple solutions simultaneously)

**Population-Based Approach:**
Instead of maintaining one "current" solution like hill-climbing, EAs maintain a diverse set of solutions. This provides:
- **Robustness**: Less likely to get stuck in local optima
- **Diversity**: Multiple trade-off points on Pareto front
- **Parallel search**: Explore multiple regions simultaneously

**Typical EA Parameters:**
- Population size: 50-200 solutions (balance between diversity and efficiency)
- Offspring size: Equal to population size (e.g., 100 parents → 100 offspring)
- Generations: 100-1000+ iterations until convergence
- Mutation rate: 1-5% per gene
- Crossover rate: 60-95% of offspring created by recombination

### 4. Variation: Creating New Solutions

**Two Main Variation Operators:**

**A. Mutation (Asexual Reproduction):**

**Concept**: Make small random changes to a single parent solution

**Binary Mutation Example:**
```
Parent:  (0, 1, 1, 0, 1, 0, 0, 1)
         ↓  ↓  ↓  ↓  ↓* ↓  ↓  ↓
Child:   (0, 1, 1, 0, 0, 0, 0, 1)
```
The bit at position 5 is flipped (mutated)

**Purpose:**
- Introduces new genetic material not present in population
- Enables local refinement of solutions
- Prevents premature convergence
- Maintains diversity

**B. Recombination/Crossover (Sexual Reproduction):**

**Concept**: Combine parts of two (or more) parent solutions

**Single-Point Crossover Example:**
```
Parent 1:  (1, 1, 0, 0 | 1, 0, 1, 1)
Parent 2:  (0, 0, 1, 1 | 0, 1, 0, 0)
           ────────────┼────────────
                       ↓ crossover point
Child 1:   (1, 1, 0, 0 | 0, 1, 0, 0)
Child 2:   (0, 0, 1, 1 | 1, 0, 1, 1)
```

**Purpose:**
- Combines successful building blocks from different solutions
- Enables faster convergence by mixing good partial solutions
- Exploits structure in the problem

**Parameter Tunin:**
- **High mutation, low crossover**: More exploration, slower convergence
- **Low mutation, high crossover**: More exploitation, faster convergence (risk: premature convergence)

### 5. Selection Mechanisms

**Two Types of Selection:**

**A. Selection for Variation (Mating Selection):**

**Purpose**: Choose parent solutions to create offspring

**Common Methods:**
1. **Random Selection**: Every solution has equal chance
   - Maintains high diversity
   - Slow convergence

2. **Fitness-Proportional Selection**: Probability proportional to fitness
   - Better solutions more likely selected
   - Risk: very good solutions dominate population quickly

3. **Tournament Selection**: Randomly sample k solutions, pick best
   - Good balance of exploration and exploitation
   - Commonly used in practice

**B. Selection for Survival (Environmental Selection):**

**Purpose**: Reduce combined population (parents + offspring) back to fixed size

**Key Criterion**: Pareto-dominance

**Process:**
1. Combine parent population (100) + offspring (100) = 200 solutions
2. Identify non-dominated solutions (Pareto front of current population)
3. Keep all non-dominated solutions
4. If more than 100 non-dominated: Use diversity metrics to keep most spread-out solutions
5. If fewer than 100 non-dominated: Fill remaining slots with best-dominated solutions

**Goal**: Maintain diverse set of Pareto-optimal solutions

### 6. The Evolutionary Loop

**Complete EA Cycle:**

```
Initialize: Generate random population of N solutions
│
├─► Generation Loop:
│   │
│   1. Selection for Variation
│   │   └─ Choose parent solutions from population
│   │
│   2. Variation
│   │   ├─ Apply mutation to create offspring
│   │   └─ Apply recombination to create offspring
│   │
│   3. Feasibility Check
│   │   └─ Remove or repair infeasible offspring
│   │
│   4. Fitness Evaluation
│   │   └─ Compute objective values for all offspring
│   │
│   5. Selection for Survival
│   │   ├─ Combined population = parents + offspring (2N solutions)
│   │   └─ Select best N solutions for next generation
│   │
│   6. Termination Check
│   │   └─ If stopping criterion met, exit loop
│   │
│   └─ Return to step 1
│
Final Result: Pareto-optimal set from final population
```

**Stopping Criteria:**
- Maximum generations reached (e.g., 1000 generations)
- No improvement for k consecutive generations
- Target objective values achieved
- Computational budget exhausted (time or evaluations)

**Typical Run:**
- Early generations: Rapid improvement, high diversity
- Middle generations: Gradual refinement, convergence toward Pareto front
- Late generations: Fine-tuning, minimal improvement

### 7. Linear vs Non-Linear Optimization

**Linear Optimization Example: Potato Field (Simple)**

**Problem Setup:**
- Variables: x₁ (area without fertilizer), x₂ (area with fertilizer)
- Constraints: x₁ + x₂ ≤ 100 m²
- Objectives:
  - Harvest: f₁ = x₁ × 3.5 + x₂ × 5 (maximize)
  - Savings: f₂ = 1000 - x₁ × 6 - x₂ × 7.5 (maximize)

**Key Property**: Linear relationship between decision variables and objectives

**Solution Approach**:
- Use linear programming solver
- Finds exact Pareto front: a straight line in objective space
- Very fast computation

**Pareto Front Shape**: Linear (straight line connecting corner points)

**Non-Linear Optimization Example: Network Processor (Complex)**

**Problem Characteristics:**
- Variables: Allocation (which processors), Binding (task-to-processor mapping), Scheduling
- Constraints: Resource limitations, dependency constraints, timing requirements
- Objectives: Performance, energy, cost
- **Non-linear relationships**: Adding a processor doesn't linearly improve performance (due to communication overhead, synchronization, etc.)

**Solution Approach**:
- Cannot use simple linear solver
- Evolutionary algorithm explores space intelligently
- Finds approximation of Pareto front

**Pareto Front Shape**: Non-linear (curved, potentially disconnected)

### 8. Feasibility Checking

**Challenge:**
Random variation operators may create infeasible solutions that violate constraints.

**Example Constraint Violations:**
- Allocate more resources than available
- Bind task to non-allocated resource
- Schedule violates dependency constraints
- Total cost exceeds budget

**Two Approaches:**

**A. Repair Strategy:**
- Detect constraint violations
- Modify solution to make it feasible
- Example: If x₁ + x₂ > 100, scale both down proportionally

**B. Penalty Strategy:**
- Allow infeasible solutions
- Assign poor fitness values (heavy penalty)
- EA naturally eliminates them through selection

**Trade-off:**
- Repair: Ensures all evaluated solutions are valid, but repair logic can be complex
- Penalty: Simpler implementation, but wastes evaluations on infeasible solutions

### 9. Search Efficiency Trade-offs

**The DSE Triangle:**

**Three Competing Factors:**

1. **Evaluation Accuracy**
   - How well does evaluation capture real system behavior?
   - Higher accuracy → Better design decisions
   - Cost: More complex models, slower evaluation

2. **Evaluation Speed**
   - How fast can one solution be evaluated?
   - Faster evaluation → Explore more designs
   - Cost: May sacrifice accuracy

3. **Search Confidence**
   - How likely to find near-optimal solutions?
   - Better search → Higher quality results
   - Cost: More evaluations, longer runtime

**Practical Strategies:**

**Multi-Fidelity Approach:**
1. **Initial exploration**: Use fast, low-accuracy models, large EA population
2. **Refinement**: Use medium-accuracy simulation, moderate population
3. **Final validation**: Use high-accuracy measurement on final candidates

**Adaptive Search:**
- Start with high diversity (exploration)
- Gradually increase exploitation as population converges

## Important Diagrams & Graphics

### 1. Decision Space to Objective Space Mapping (Linear Case)
**Description for Redrawing:**
- **Left panel**: Decision space
  - X-axis: x₁ (area without fertilizer), 0-100 m²
  - Y-axis: x₂ (area with fertilizer), 0-100 m²
  - Constraint line: x₁ + x₂ = 100 (diagonal line)
  - Shaded triangular region: Feasible space
  - Points: (1, 99), (50, 50), (99, 1) marked on constraint line

- **Right panel**: Objective space
  - X-axis: Savings (CHF), 0-1000
  - Y-axis: Harvest (kg), 0-500
  - Pareto front: Straight diagonal line from (0, 500) to (1000, 0)
  - Same three points mapped to objective space
  - Show linear relationship with arrows connecting decision to objective space

### 2. Binary Solution Representation - Allocation
**Description for Redrawing:**
- **Top**: Hardware architecture diagram showing:
  - Bus connecting multiple components
  - Upper row: DSP units (DSP, DSP, DSP), ARM9 units (ARM9 × 4)
  - Lower row: Checksum, Cipher units (Cipher × 2), PowerPC units (PowerPC × 3)

- **Bottom**: Binary vector representation
  - Vector: (0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1)
  - Arrows pointing from "1" bits to corresponding selected hardware
  - Highlight selected components: DSP2, Checksum, Cipher2, PowerPC3
  - Label: "Allocation_x = (0,1,0,0,0,0,0,1,0,1,0,0,1)"

### 3. Binary Solution Representation - Binding
**Description for Redrawing:**
- **Left**: Task graph showing:
  - Voice Encoder (green task node)
  - Checksum (green task node)
  - RTP Tx (green task node)

- **Center**: Allocated resources:
  - DSP1 (blue)
  - DSP2 (blue)
  - Checksum hardware (blue)

- **Mapping arrows**: From tasks to resources
  - Voice Encoder → DSP2
  - Checksum → Checksum hardware
  - RTP Tx → DSP2

- **Bottom**: Binary binding vector
  - Long vector with 1s indicating task-resource pairs
  - Label sections for each task's possible bindings
  - Highlight active bindings with boxes

### 4. 3D Objective Space Visualization
**Description for Redrawing:**
- **3D coordinate system**:
  - X-axis: Performance (backbone network)
  - Y-axis: Performance (access network)
  - Z-axis: Savings (cost)

- **Data points**: Scattered dots representing evaluated designs
  - Points clustered in achievable region
  - Some points form visible Pareto front surface

- **Annotations**:
  - Points labeled 'a', 'b', 'c' on Pareto surface
  - Dashed lines connecting labels to corresponding design architectures (shown as insets)

- **Three inset architecture diagrams**:
  - Design (a): DSP-DSP-PowerPC-Cipher configuration
  - Design (b): DSP-Cipher configuration
  - Design (c): DSP-DSP configuration

### 5. Mutation Operator
**Description for Redrawing:**
- **Parent solution**: Horizontal bar divided into segments (gray and white)
  - Representation: 4 gray, 4 white segments
  - Binary: Gray = 1, White = 0

- **Arrow down** labeled "mutation"
  - Small box labeled "parameter mutated" pointing to one segment

- **Child solution**: Same structure but one segment flipped
  - Show which bit changed color
  - Highlight mutated position with circle or marker

- **Side text**:
  - "parent solution" above
  - "child solution" below
  - Show binary representation next to each

### 6. Recombination (Crossover) Operator
**Description for Redrawing:**
- **Two parent solutions** at top:
  - Parent #1: Bar with pattern (e.g., gray-gray-white-white)
  - Parent #2: Bar with different pattern (e.g., white-white-gray-gray)

- **Vertical crossover line** between parents
  - Labeled "crossover point"

- **Crossing arrows** showing recombination
  - Left parts stay with original parent
  - Right parts swap between parents

- **Two child solutions** at bottom:
  - Child #1: Left from Parent #1 + Right from Parent #2
  - Child #2: Left from Parent #2 + Right from Parent #1

- **Labels**: "parent solution #1", "parent solution #2", "child solution #1", "child solution #2"

### 7. Evolutionary Loop Flow Diagram
**Description for Redrawing:**
- **Circular flow with four main components**:

1. **Top left box**: "Population Size 100"
   - Arrow pointing right to "solution set"

2. **Top circle**: "selection for variation"
   - Gray oval surrounding the process

3. **Right side**: "variation: generation of new solutions"
   - Rectangle box
   - Output: "Population Size 100 + 100 new"

4. **Bottom right box**: "fitness assignment: evaluation of new solutions"

5. **Bottom circle**: "selection for survival"
   - Gray oval

6. **Bottom left box**: "Reduce Population Size to 100"
   - Arrow loops back to top

- **Flow arrows** connecting all components in clockwise direction
- **Gray shaded regions** highlighting selection phases

### 8. Population Evolution Over Generations
**Description for Redrawing:**
- **2D objective space**: f₁ (x-axis) vs f₂ (y-axis)

- **Three snapshots** (or animation frames):

  **Generation 0** (Initial):
  - Scattered points (blue and green dots)
  - Random distribution
  - Some dominated, some non-dominated

  **Generation 50** (Middle):
  - Points converging toward Pareto front
  - Better spread along trade-off curve
  - Fewer dominated solutions

  **Generation 100** (Final):
  - Points aligned along Pareto front
  - Good distribution
  - All or most non-dominated

- **Pareto front**: Curved line showing true optimum
- **Legend**: Blue = non-dominated, Green = Pareto-optimal

### 9. DSE Quality Trade-offs (Two Panels)
**Description for Redrawing:**

**Left Panel - "Evaluating a single design point":**
- **Triangle diagram** with three corners:
  - Top: "Accuracy - capturing relevant system properties"
  - Bottom-left: "Effort - model creation, flexibility and scope"
  - Bottom-right: "Speed - evaluation execution time"
- Arrows showing trade-offs between corners

**Right Panel - "Searching the design space":**
- **Triangle diagram** with three corners:
  - Top: "Confidence - reliability of result quality"
  - Bottom-left: "Effort - DSE specification and setup"
  - Bottom-right: "Convergence - towards optimum results"
- Note "inter-dependence" between left and right panels

### 10. Potato Field - Decision Space (With Fertilizer)
**Description for Redrawing:**
- **2D coordinate system**:
  - X-axis: x₂ (area with fertilizer), 0-100 m²
  - Y-axis: x₁ (area without fertilizer), 0-100 m²

- **Constraint boundary**: Diagonal line x₁ + x₂ = 100
  - Shaded blue triangular region: Feasible space

- **Label**: "Decision Space"

- **Data box** (side or top):
  - 1 m² without fertilizer ↔ 3.5 kg, 6 CHF
  - 1 m² with fertilizer ↔ 5 kg, 7.5 CHF
  - Assumption: 1000 CHF available

### 11. Potato Field - Objective Space (With Fertilizer)
**Description for Redrawing:**
- **2D coordinate system**:
  - X-axis: Harvest (kg), 0-500
  - Y-axis: Savings (CHF), 0-1000

- **Dense Pareto front**: Band of blue points forming diagonal line
  - From approximately (0, 1000) to (500, 250)

- **One point highlighted** (green): (425, 325)
  - Corresponds to solution x₁=50, x₂=50

- **Label**: "Objective Space"
  - Annotation showing this is optimal for sum of objectives with full land use

### 12. Network Processor - Complete Solution Representation
**Description for Redrawing:**
Three side-by-side panels showing:

**Panel 1 - Allocation:**
- Hardware elements selection
- Bus connecting components
- Selected: Highlighted in blue
- Binary vector below

**Panel 2 - Binding:**
- Task graph at top showing packet processing flow
- Tasks mapped to allocated resources
- Arrows showing bindings
- Binary vector below

**Panel 3 - Runtime Environment (Scheduling):**
- Timeline showing task execution
- Tasks: RTP Tx, UDP Tx, Build IP Header, Route Look Up, ARP Look Up, Schedule, Link Tx
- Two RTOS schedulers shown
- PowerPC resource executing tasks
- Binary vector below

### 13. Feasibility Check Flow
**Description for Redrawing:**
- **Flowchart**:
  1. **Start**: "Generated Solution_x"
  2. **Diamond**: "Is solution feasible?"
     - Checks: All constraints satisfied?
  3. **If Yes**: → "Evaluate fitness" → "Add to population"
  4. **If No**: → "Repair or Discard"
     - Repair: Modify to satisfy constraints
     - Discard: Generate new solution
  5. **End**: Valid solution ready for evaluation

### 14. Search Strategy Decision Tree
**Description for Redrawing:**
- **Root**: "DSE Problem"

- **Branch 1**: "Linear objectives and constraints?"
  - **Yes** → "Use Linear Programming Solver" → "Fast, Exact Solution"
  - **No** → Continue to Branch 2

- **Branch 2**: "Design space size?"
  - **Small (< 10,000)** → "Exhaustive Enumeration" → "Complete Pareto Front"
  - **Large (> 10,000)** → Continue to Branch 3

- **Branch 3**: "Evaluation cost?"
  - **Fast (< 1s)** → "Evolutionary Algorithm with large population"
  - **Slow (> 1min)** → "Evolutionary Algorithm with small population or surrogate models"

## Formulas & Procedures

### 1. Linear Optimization Problem Formulation (Potato Field with Fertilizer)

**Decision Variables:**
```
x₁ = area without fertilizer (m²)
x₂ = area with fertilizer (m²)
```

**Constraints:**
```
x₁ + x₂ ≤ 100  (total field area)
x₁ ≥ 0
x₂ ≥ 0
```

**Objective Functions:**
```
Maximize Harvest: f₁ = 3.5 × x₁ + 5.0 × x₂  (kg)
Maximize Savings: f₂ = 1000 - 6.0 × x₁ - 7.5 × x₂  (CHF)
```

**Conversion:**
For minimization (if required):
```
Minimize negative harvest: -f₁ = -3.5 × x₁ - 5.0 × x₂
Minimize negative savings: -f₂ = -1000 + 6.0 × x₁ + 7.5 × x₂
```

**Single-Objective Weighted Sum (if needed):**
```
Maximize: f_combined = w₁ × f₁ + w₂ × f₂

Where w₁ + w₂ = 1, and w₁, w₂ ≥ 0
```

**Example Solution:**
```
For w₁ = w₂ = 0.5 (equal weight) and constraint x₁ + x₂ = 100:
Optimal: x₁ = 50 m², x₂ = 50 m²
Results: f₁ = 425 kg, f₂ = 325 CHF
```

### 2. Evolutionary Algorithm Pseudocode

```
FUNCTION EvolutionaryAlgorithm(problem, params):

    // Initialization
    population ← GenerateRandomSolutions(params.populationSize)
    FOR EACH solution IN population:
        IF NOT IsFeasible(solution):
            solution ← RepairOrRegenerate(solution)
        END IF
        solution.fitness ← Evaluate(solution, problem)
    END FOR

    generation ← 0

    // Main evolutionary loop
    WHILE NOT TerminationCriterion(generation, population):

        offspring ← []

        // Generate offspring
        FOR i FROM 1 TO params.offspringSize:

            // Selection for variation
            parents ← SelectParents(population, params.selectionMethod)

            // Variation operators
            IF Random() < params.crossoverRate:
                child ← Recombination(parents)
            ELSE:
                child ← CopyOne(parents)
            END IF

            child ← Mutation(child, params.mutationRate)

            // Feasibility check
            IF NOT IsFeasible(child):
                child ← RepairOrRegenerate(child)
            END IF

            // Evaluate
            child.fitness ← Evaluate(child, problem)

            offspring.APPEND(child)
        END FOR

        // Selection for survival
        combinedPopulation ← population + offspring
        population ← SelectSurvivors(combinedPopulation, params.populationSize)

        generation ← generation + 1

    END WHILE

    // Extract Pareto front from final population
    paretoFront ← ExtractNonDominated(population)

    RETURN paretoFront

END FUNCTION
```

**Key Parameters:**
- `populationSize`: Typically 50-200
- `offspringSize`: Typically equal to populationSize
- `crossoverRate`: Typically 0.7-0.9
- `mutationRate`: Typically 0.01-0.05 per gene

### 3. Binary Mutation Operator

```
FUNCTION BinaryMutation(solution, mutationRate):

    child ← Copy(solution)

    FOR i FROM 1 TO Length(child):
        IF Random() < mutationRate:
            child[i] ← NOT child[i]  // Flip bit: 0→1 or 1→0
        END IF
    END FOR

    RETURN child

END FUNCTION
```

**Mutation Rate Calculation:**
```
Typical approach: mutationRate = 1 / n

Where:
  n = length of solution vector

Example: If solution has 100 bits, mutationRate = 1/100 = 0.01
         On average, 1 bit flipped per mutation
```

**Expected Number of Mutations:**
```
E[mutations] = n × p_mutation

Where:
  n = solution length
  p_mutation = mutation rate per bit

Example: n = 100, p_mutation = 0.01
         E[mutations] = 100 × 0.01 = 1 bit flipped on average
```

### 4. Single-Point Crossover

```
FUNCTION SinglePointCrossover(parent1, parent2):

    n ← Length(parent1)
    crossoverPoint ← RandomInteger(1, n-1)

    child1 ← []
    child2 ← []

    // Copy first part from respective parents
    FOR i FROM 1 TO crossoverPoint:
        child1.APPEND(parent1[i])
        child2.APPEND(parent2[i])
    END FOR

    // Swap second part
    FOR i FROM crossoverPoint+1 TO n:
        child1.APPEND(parent2[i])
        child2.APPEND(parent1[i])
    END FOR

    RETURN (child1, child2)

END FUNCTION
```

**Two-Point Crossover:**
```
Same as single-point, but with two crossover points:
- Copy segments: [start, point1] and [point2, end] from parent1
- Copy middle segment [point1+1, point2-1] from parent2
```

**Uniform Crossover:**
```
FOR each position i:
    IF Random() < 0.5:
        child[i] ← parent1[i]
    ELSE:
        child[i] ← parent2[i]
    END IF
```

### 5. Tournament Selection

```
FUNCTION TournamentSelection(population, tournamentSize):

    tournament ← []

    // Randomly sample tournamentSize individuals
    FOR i FROM 1 TO tournamentSize:
        randomIndex ← RandomInteger(0, Length(population)-1)
        tournament.APPEND(population[randomIndex])
    END FOR

    // Return best individual from tournament
    winner ← FindBest(tournament)  // Based on Pareto-dominance

    RETURN winner

END FUNCTION
```

**Binary Tournament** (most common):
```
tournamentSize = 2

Probability better solution selected:
P(select better) = depends on dominance relationships
```

**Selection Pressure:**
```
Larger tournamentSize → Higher selection pressure
  → Faster convergence
  → Risk of premature convergence

Smaller tournamentSize → Lower selection pressure
  → More exploration
  → Slower convergence
```

### 6. Selection for Survival (Pareto-based)

```
FUNCTION SelectSurvivors(combinedPopulation, targetSize):

    survivors ← []
    remaining ← Copy(combinedPopulation)

    // Iteratively select non-dominated fronts
    WHILE Length(survivors) < targetSize:

        // Find non-dominated solutions in remaining set
        front ← ExtractNonDominated(remaining)

        IF Length(survivors) + Length(front) <= targetSize:
            // Entire front fits
            survivors ← survivors + front
            remaining ← remaining - front
        ELSE:
            // Need to select subset of front
            needed ← targetSize - Length(survivors)
            selected ← SelectMostDiverse(front, needed)
            survivors ← survivors + selected
            BREAK
        END IF

    END WHILE

    RETURN survivors

END FUNCTION
```

**Diversity Metric** (Crowding Distance):
```
For each solution i in a front:
    distance[i] ← 0

    FOR each objective m:
        Sort front by objective m
        distance[first] ← ∞
        distance[last] ← ∞

        FOR i FROM 2 TO (Length(front) - 1):
            distance[i] += (front[i+1].objective[m] - front[i-1].objective[m])
                          / (max[m] - min[m])
        END FOR
    END FOR

Prefer solutions with larger distance[i] (more isolated = more diverse)
```

### 7. Termination Criteria

**Maximum Generations:**
```
IF generation >= maxGenerations:
    TERMINATE

Typical: maxGenerations = 100 to 1000
```

**No Improvement:**
```
IF NoImprovementFor(k generations):
    TERMINATE

Where "improvement" means:
  - New non-dominated solution found, OR
  - Existing solution dominated and removed

Typical: k = 20 to 100 generations
```

**Hypervolume Convergence:**
```
hypervolume ← CalculateHypervolume(paretoFront, referencePoint)

IF |hypervolume[generation] - hypervolume[generation - k]| < ε:
    TERMINATE  // Converged

Where:
  k = lookback window (e.g., 10 generations)
  ε = convergence threshold (e.g., 0.01)
```

### 8. Feasibility Checking and Repair

**Generic Feasibility Check:**
```
FUNCTION IsFeasible(solution, constraints):

    FOR EACH constraint IN constraints:
        IF NOT EvaluateConstraint(solution, constraint):
            RETURN FALSE
        END IF
    END FOR

    RETURN TRUE

END FUNCTION
```

**Example Repair Strategy (Allocation):**
```
FUNCTION RepairAllocation(solution, maxResources):

    allocated ← CountOnes(solution.allocation)

    WHILE allocated > maxResources:
        // Randomly remove one allocated resource
        randomIndex ← RandomBit(solution.allocation, value=1)
        solution.allocation[randomIndex] ← 0
        allocated ← allocated - 1
    END WHILE

    RETURN solution

END FUNCTION
```

**Example Repair Strategy (Potato Field):**
```
FUNCTION RepairPotato(x1, x2, maxArea):

    total ← x1 + x2

    IF total > maxArea:
        // Scale down proportionally
        scale ← maxArea / total
        x1 ← x1 × scale
        x2 ← x2 × scale
    END IF

    RETURN (x1, x2)

END FUNCTION
```

## Comparisons & Contrasts

### 1. Linear vs Non-Linear Optimization

| Aspect | Linear Optimization | Non-Linear Optimization |
|--------|---------------------|------------------------|
| **Objective functions** | Linear combinations of variables | Non-linear relationships (polynomials, exponentials, etc.) |
| **Pareto front shape** | Straight lines or planes | Curved, potentially disconnected surfaces |
| **Solution methods** | Linear programming, Simplex algorithm | Evolutionary algorithms, gradient methods |
| **Computation time** | Fast (polynomial time) | Slow (depends on heuristic) |
| **Solution quality** | Exact optimal solutions | Approximate optimal solutions |
| **Example** | Potato field (harvest = 3.5×x₁ + 5×x₂) | Network processor (communication overhead, synchronization) |
| **Scalability** | Scales well to thousands of variables | Limited by evaluation cost |
| **Guarantee** | Provably optimal | No optimality guarantee |

**Key Insight:** Real embedded system DSE problems are almost always non-linear due to complex interactions between components.

### 2. Mutation vs Recombination

| Aspect | Mutation | Recombination (Crossover) |
|--------|----------|---------------------------|
| **Number of parents** | 1 (asexual) | 2 or more (sexual) |
| **Purpose** | Introduce new genetic material | Combine existing good solutions |
| **Effect** | Small local changes | Mix building blocks from parents |
| **Exploration** | High (creates diversity) | Medium (explores combinations) |
| **Exploitation** | Low (random changes) | High (preserves good patterns) |
| **Convergence speed** | Slower | Faster |
| **Diversity** | Maintains diversity | Can reduce diversity |
| **Implementation** | Simple (flip bits) | More complex (ensure valid offspring) |
| **Typical rate** | 1-5% per gene | 60-95% of offspring |

**Best Practice:** Use both operators in combination for balance between exploration and exploitation.

### 3. Selection for Variation vs Selection for Survival

| Aspect | Selection for Variation | Selection for Survival |
|--------|------------------------|----------------------|
| **Purpose** | Choose parents to create offspring | Choose solutions to keep in population |
| **Input size** | Population (N solutions) | Combined pool (2N solutions) |
| **Output size** | N parents | N survivors |
| **Selection pressure** | Can be low (explore diversity) | High (only keep best) |
| **Criterion** | Fitness, diversity, random | Pareto-dominance |
| **Methods** | Random, tournament, fitness-proportional | Non-dominated sorting, elitism |
| **Effect on convergence** | Influences speed | Determines final quality |
| **Elitism** | Not necessarily elite | Always keeps best solutions |
| **When applied** | Before variation | After fitness evaluation |

### 4. Exhaustive Search vs Evolutionary Algorithm

| Aspect | Exhaustive Search | Evolutionary Algorithm |
|--------|-------------------|----------------------|
| **Coverage** | 100% of design space | Small fraction (< 1%) |
| **Evaluations** | All possible designs | Population × Generations |
| **Optimality** | Guaranteed to find all Pareto-optimal | No guarantee, approximation |
| **Computation time** | Exponential in problem size | Manageable (user-controlled) |
| **Feasibility** | Small spaces only (< 10,000 points) | Large spaces (millions, billions) |
| **Parallelization** | Trivial (evaluate all independently) | Possible (evaluate population in parallel) |
| **Example size** | 10 binary variables → 1024 designs | 100 binary variables → 2^100 designs |
| **Result confidence** | 100% certain | Stochastic (different runs differ) |

**Decision Rule:** Use exhaustive for < 10,000 designs; use EA for larger spaces.

### 5. Binary Encoding vs Other Encodings

| Aspect | Binary Encoding | Integer Encoding | Real-Valued Encoding | Graph Encoding |
|--------|----------------|------------------|---------------------|----------------|
| **Representation** | (0, 1, 0, 1, 1) | (2, 4, 1, 3) | (0.5, 3.2, 1.8) | Nodes and edges |
| **Variable types** | Discrete on/off | Discrete choices | Continuous | Structural |
| **Mutation** | Bit flip | ±1 or random jump | Add Gaussian noise | Add/remove edge |
| **Crossover** | Simple bit swap | Swap segments | Blend/average | Subgraph swap |
| **Repair complexity** | Medium | Medium | Low | High |
| **Human readability** | Poor | Good | Good | Medium |
| **Examples** | Allocation, selection | Processor types | Voltage, frequency | Task graphs |
| **Storage efficiency** | High | Medium | Low | Variable |

**DSE Practice:** Binary encoding most common for allocation/binding; real-valued for parameter tuning.

### 6. Evaluation Accuracy vs Speed

| Approach | Method | Accuracy | Speed | When to Use |
|----------|--------|----------|-------|------------|
| **Analysis** | Mathematical models | Low-Medium | Very fast (ms) | Initial broad exploration |
| **Fast Simulation** | High-level simulation | Medium | Fast (seconds) | Mid-stage exploration with EA |
| **Detailed Simulation** | Cycle-accurate | High | Slow (minutes) | Candidate refinement |
| **Emulation** | FPGA prototype | Very high | Medium (seconds) | Final validation |
| **Measurement** | Real hardware | Highest | Slow (hours) | Final few candidates |

**Multi-Fidelity Strategy:**
1. EA with fast evaluation (10,000 designs)
2. Re-evaluate top 100 with detailed simulation
3. Build and measure top 5 designs

### 7. Small vs Large Population Size

| Aspect | Small Population (50-100) | Large Population (200-500) |
|--------|--------------------------|---------------------------|
| **Diversity** | Lower | Higher |
| **Convergence speed** | Faster | Slower |
| **Exploration** | Limited | Extensive |
| **Risk of local optima** | Higher | Lower |
| **Evaluations per generation** | Fewer | Many |
| **Memory requirements** | Low | High |
| **Pareto front coverage** | Sparse | Dense |
| **Best for** | Small design spaces, fast evaluation | Large design spaces, complex problems |
| **Typical generations** | 200-1000 | 100-500 |

**Rule of Thumb:** Population size should be proportional to design space complexity and number of objectives.

## Examples & Applications

### 1. Potato Field Optimization (Simple Linear DSE)

**Problem:**
A farmer has 100 m² and 1000 CHF budget. Optimize potato planting with/without fertilizer.

**Without Fertilizer Option:**
- Cost: 6 CHF/m²
- Yield: 3.5 kg/m²

**With Fertilizer Option:**
- Cost: 7.5 CHF/m² (6 CHF + 1.5 CHF fertilizer)
- Yield: 5 kg/m²

**Formulation:**
```
Variables: x₁ (no fertilizer), x₂ (with fertilizer)
Constraints: x₁ + x₂ ≤ 100
Objectives:
  - Maximize harvest: 3.5×x₁ + 5×x₂
  - Maximize savings: 1000 - 6×x₁ - 7.5×x₂
```

**Solution Approach:**
Used online linear optimization solver.

**Results:**
| x₁ (m²) | x₂ (m²) | Harvest (kg) | Savings (CHF) | Notes |
|---------|---------|--------------|---------------|-------|
| 0 | 99 | 495 | 258 | Maximum harvest |
| 50 | 50 | 425 | 325 | Balanced (optimal for sum) |
| 99 | 1 | 351.5 | 400.5 | Near-maximum savings |
| 166.7 | 0 | 583.3 | 0 | Maximum harvest (violates budget) |
| 100 | 0 | 350 | 400 | Maximum savings (full field) |

**Pareto Front:** Straight line connecting (400, 350) to (250, 500) in (savings, harvest) space.

**Key Learning:** Linear problems have linear Pareto fronts and can be solved exactly.

### 2. Network Processor Design (Complex Non-Linear DSE)

**Problem:**
Design a network processor for VoIP packet processing with multiple flow types:
- Flow NRT Encrypt: ESP Encaps → AH Calc → Encrypt
- Flow NRT Decrypt: Decrypt → AH Verify → ESP Decaps
- Flow NRT Forward: Classify → Route Look Up → Schedule
- Flow RT Send: Voice Encoder → RTP Tx → UDP Tx → Build IP Header → Route Look Up
- Flow RT Recv: UDP Rx → RTP Rx → Dejitter → Voice Decoder

**Available Hardware Resources:**
- DSP processors (3 instances)
- ARM9 processors (4 instances)
- Checksum accelerator
- Cipher accelerators (2 instances)
- PowerPC processors (3 instances)

**Decision Variables:**
1. **Allocation**: Which hardware resources to include (13 binary decisions)
2. **Binding**: Which tasks run on which resources (100+ binary decisions)
3. **Scheduling**: Execution order and priorities (50+ binary decisions)

**Objectives:**
- Maximize performance (packets/second) for access network flows
- Maximize performance for backbone network flows
- Minimize cost (hardware + power)

**Solution Representation:**
```
Solution_x = (Allocation_x, Binding_x, RTE_x)
           = (0,1,0,0,0,0,0,1,0,1,0,0,1, ..., 1,0,1,0, ..., 1,1,0)

Total length: ~200 binary variables
```

**Example Solutions Found:**

**Design A** (Performance-focused):
- Allocation: 2× DSP, 3× ARM9, 1× PowerPC, 1× Cipher
- Binding: Parallel task distribution
- Performance: High on both metrics
- Cost: High
- Position: Far from "savings" axis in 3D objective space

**Design B** (Cost-focused):
- Allocation: 1× DSP, 1× Cipher only
- Binding: Sequential processing
- Performance: Medium
- Cost: Low
- Position: Near "savings" axis

**Design C** (Balanced):
- Allocation: 2× DSP, 1× ARM9, 1× ARM9
- Binding: Critical tasks parallel, others sequential
- Performance: Medium-high
- Cost: Medium
- Position: On Pareto front, balanced

**EA Configuration Used:**
- Population size: 100
- Generations: 200
- Mutation rate: 2% per bit
- Crossover rate: 80%
- Selection: Binary tournament

**Results:**
- Design space size: 2^200 ≈ 10^60 possible designs
- Designs evaluated: 20,000 (100 population × 200 generations)
- Coverage: Negligible (< 10^-56 %)
- Pareto front: 23 non-dominated designs found

**Key Learning:** Even exploring tiny fraction of space, EA found high-quality diverse solutions.

### 3. Mobile Phone SoC Architecture Selection

**Problem:**
Select optimal System-on-Chip architecture for mid-range smartphone.

**Components to Allocate:**
- Application processors: 2, 4, 6, or 8 cores
- GPU: Integrated-low, Integrated-high, Discrete
- DSP: None, Single, Dual
- ISP (Image Signal Processor): None, Basic, Advanced
- NPU (Neural Processing Unit): None, Low-power, High-performance
- Modem: 4G, 5G-sub6, 5G-mmWave
- Memory: LPDDR4, LPDDR4X, LPDDR5

**Decision Variables:** 7 categorical choices → Encoded as ~25 binary variables

**Objectives:**
- Maximize benchmark performance (GeekBench score)
- Minimize power consumption (W)
- Minimize cost ($)
- Minimize area (mm²)

**Constraints:**
- Total power < 5W (thermal limit)
- Total cost < $50 (target market)

**EA Setup:**
- Population: 150
- Generations: 300
- Evaluations: 45,000 designs
- Evaluation: Performance models + power models
- Runtime: 6 hours

**Selected Pareto-Optimal Designs:**

| Design | CPU | GPU | DSP | ISP | NPU | Performance | Power (W) | Cost ($) |
|--------|-----|-----|-----|-----|-----|-------------|-----------|----------|
| Budget | 4-core | Int-low | None | Basic | None | 3200 | 2.5 | 28 |
| Balanced | 6-core | Int-high | Single | Advanced | Low | 5800 | 3.8 | 42 |
| Premium | 8-core | Discrete | Dual | Advanced | High | 8500 | 4.9 | 54 |

**Final Selection:** Balanced design chosen based on market positioning.

**Key Learning:** Multi-objective EA revealed non-obvious trade-offs (e.g., dual DSP not worth cost for target applications).

### 4. IoT Sensor Node Battery Life Optimization

**Problem:**
Design battery-powered environmental sensor node with 5-year target lifetime.

**Decision Variables:**
- MCU: 8-bit@1MHz, 16-bit@8MHz, 32-bit@48MHz
- Radio: LoRa, Zigbee, BLE, NB-IoT
- Sensing rate: 0.1 Hz, 1 Hz, 10 Hz
- Transmission rate: Every measurement, Every 10, Every 100
- Sleep mode: Deep sleep, Light sleep, Always-on
- Sensor type: Basic (low-power), Advanced (high-accuracy)

**Constraints:**
- Battery: CR2032 (240 mAh @ 3V)
- Lifetime ≥ 5 years
- Data loss < 1% (reliability)

**Energy Budget Calculation:**
```
E_total = 240 mAh × 3V × 3600 s/h = 2,592,000 J
E_per_day = 2,592,000 J / (5 × 365 days) = 1,418 J/day

Daily energy breakdown target:
- Sleep: ~1,400 J (98.7%)
- Sensing: ~10 J (0.7%)
- Transmission: ~8 J (0.6%)
```

**EA Configuration:**
- Encoding: Integer representation for categorical variables
- Population: 80
- Generations: 150
- Mutation: Randomly change category with 10% probability
- Crossover: Uniform crossover

**Pareto-Optimal Solutions Found:**

**Design 1** (Maximum lifetime):
- MCU: 8-bit @ 1 MHz
- Radio: LoRa
- Sensing: 0.1 Hz
- TX rate: Every 100 samples
- Sleep: Deep (3 μA)
- Sensor: Basic
- **Result**: 7.2 years, low data rate, basic accuracy

**Design 2** (Balanced):
- MCU: 16-bit @ 8 MHz
- Radio: BLE
- Sensing: 1 Hz
- TX rate: Every 10 samples
- Sleep: Deep (5 μA)
- Sensor: Advanced
- **Result**: 5.3 years, good data rate, high accuracy

**Design 3** (Maximum performance):
- MCU: 32-bit @ 48 MHz
- Radio: NB-IoT
- Sensing: 10 Hz
- TX rate: Every measurement
- Sleep: Light (50 μA)
- Sensor: Advanced
- **Result**: 1.1 years, high data rate, high accuracy

**Selected:** Design 2 (meets 5-year target with good performance)

**Validation:** Prototype measured 0.92 J/day → 7.7 year estimated lifetime (exceeds target)

**Key Learning:** Sleep mode dominates energy budget; DSP found counter-intuitive solution (more frequent low-power transmissions vs fewer high-power ones).

### 5. Automotive ECU Hardware/Software Partitioning

**Problem:**
Partition ADAS (Advanced Driver Assistance Systems) functions between MCU software and FPGA hardware.

**Functions:**
- Lane detection
- Object detection (YOLO CNN)
- Optical flow calculation
- Sensor fusion (Kalman filter)
- Path planning
- Vehicle control

**Objectives:**
- Minimize latency (real-time constraint: < 100 ms end-to-end)
- Minimize power (< 25 W)
- Minimize cost (< $200 hardware)

**Decision Variables:**
- For each function: Implement in SW (MCU) or HW (FPGA)
- MCU selection: ARM Cortex-A72 (fast, expensive) vs Cortex-A53 (slow, cheap)
- FPGA size: Small (50K LUTs), Medium (150K LUTs), Large (500K LUTs)

**Encoding:**
```
Solution = (SW/HW flags, MCU type, FPGA size)
         = (1,0,0,1,1,1, MCU_type_id, FPGA_size_id)

6 functions × 2 choices + 2 selection vars = 14 decision variables
```

**EA Results After 500 Generations:**

**Pure Software Solution:**
- All functions on Cortex-A72
- Latency: 145 ms (violates constraint)
- Power: 15 W
- Cost: $80
- **Status**: Infeasible (too slow)

**Pure Hardware Solution:**
- All functions in Large FPGA
- Latency: 35 ms
- Power: 28 W (violates constraint)
- Cost: $250 (violates constraint)
- **Status**: Infeasible (too expensive, high power)

**Optimal Hybrid Solution:**
- Lane detection: HW (FPGA)
- Object detection: HW (FPGA)
- Optical flow: HW (FPGA)
- Sensor fusion: SW (MCU)
- Path planning: SW (MCU)
- Control: SW (MCU)
- MCU: Cortex-A53
- FPGA: Medium (150K LUTs)
- **Results**: 88 ms latency, 22 W power, $185 cost
- **Status**: Feasible, Pareto-optimal

**Key Learning:**
- Compute-intensive vision functions benefit most from FPGA acceleration
- Control algorithms work fine in software
- Hybrid approach dominates pure solutions

### 6. Data Center Server Configuration

**Problem:**
Configure server for mixed workload (web serving + database + batch processing).

**Decision Variables:**
- CPU cores: 8, 16, 32, 64
- Memory: 64 GB, 128 GB, 256 GB, 512 GB
- Storage: HDD (4TB), SSD (1TB), NVMe (500GB)
- Network: 1 Gbps, 10 Gbps, 25 Gbps, 100 Gbps

**Objectives:**
- Maximize throughput (requests/second)
- Minimize latency (ms per request)
- Minimize cost ($)
- Minimize power (W)

**Design Space Size:** 4 × 4 × 3 × 4 = 192 configurations

**Approach:** Exhaustive evaluation (feasible due to small space)

**Results:** 192 designs evaluated, 18 Pareto-optimal found

**Example Pareto-Optimal Designs:**

| Config | Cores | Memory | Storage | Network | Throughput | Latency | Cost | Power |
|--------|-------|--------|---------|---------|------------|---------|------|-------|
| A | 8 | 64 GB | HDD | 1 Gbps | 5K req/s | 120 ms | $1,200 | 150 W |
| B | 16 | 128 GB | SSD | 10 Gbps | 12K req/s | 65 ms | $2,800 | 250 W |
| C | 32 | 256 GB | NVMe | 25 Gbps | 28K req/s | 35 ms | $6,500 | 400 W |
| D | 64 | 512 GB | NVMe | 100 Gbps | 45K req/s | 28 ms | $15,000 | 650 W |

**Key Findings:**
- Storage type dominates latency (NVMe vs HDD)
- Network bandwidth critical for throughput beyond 20K req/s
- Memory size matters for database workload
- Power scales roughly linearly with performance

**Selected:** Config B for target workload characteristics

**Key Learning:** Even small design spaces benefit from systematic DSE to understand trade-offs.

## Connections to Other Topics

### 1. Lecture 11 (DSE Intro)
- **This lecture builds on**: Problem formulation, Pareto-dominance, objectives, constraints
- **Adds**: Specific search algorithms and solution representation strategies
- **Lecture 11**: "What to find" (Pareto-optimal solutions)
- **Lecture 12**: "How to find it" (search algorithms, especially EAs)
- **Key link**: Evolutionary algorithms operationalize the DSE concepts from Lecture 11

### 2. Lecture 09 (Performance)
- **Performance models**: Used as fitness evaluation functions in DSE
- **Metrics** (throughput, latency, speedup): Become objectives in DSE
- **Bottleneck analysis**: Helps identify critical resources to allocate
- **Amdahl's Law**: Constrains achievable performance in multi-processor allocation
- **Example**: Network processor performance depends on slowest pipeline stage → influences allocation decisions

### 3. Lecture 08 (Energy)
- **Energy models**: Essential for energy objective evaluation in DSE
- **DVFS**: Can be decision variables (voltage/frequency selection)
- **Power breakdown**: Static vs dynamic power models feed into EA fitness
- **Trade-offs**: Energy vs performance captured in Pareto front
- **Example**: IoT sensor node DSE requires accurate sleep mode energy models

### 4. Lecture 10 (FPGA)
- **Hardware/Software partitioning**: Major DSE problem (allocate to FPGA or CPU?)
- **FPGA resources** (LUTs, DSPs, BRAM): Decision variables in FPGA-based DSE
- **Reconfigurability**: Enables runtime DSE and adaptive systems
- **Performance models**: FPGA execution time vs CPU execution time
- **Example**: Automotive ADAS DSE deciding which vision functions go to FPGA

### 5. Lecture 06 (Non-Functional Requirements)
- **NFRs → Objectives**: Energy, performance, cost, reliability become DSE objectives
- **Quantification**: NFRs must be quantifiable for DSE evaluation
- **Constraints**: Hard NFRs (e.g., latency < 100ms) become constraints
- **Trade-offs**: DSE reveals conflicts between NFRs through Pareto fronts
- **Example**: Real-time NFR becomes latency objective or constraint in DSE

### 6. Lecture 05 (Development Processes)
- **V-Model integration**: DSE occurs during System Design phase
- **Iteration**: EA iterations mirror design iteration cycles
- **Validation**: Selected designs validated in right side of V
- **Refinement**: Multi-fidelity evaluation parallels V-Model abstraction levels
- **Example**: Fast evaluation in early exploration, detailed validation later

### 7. Lecture 04 (Modelling)
- **System models**: Source of allocation/binding/scheduling structure
- **Task graphs**: Define binding decisions (task-to-resource mapping)
- **Architecture models**: Define allocation decision space
- **Performance simulation**: Provides fitness evaluation
- **Example**: UML/SysML models parameterize architecture template for DSE

### 8. Lecture 13 (RTOS Scheduling)
- **Scheduling**: Third component of solution representation (after allocation, binding)
- **Priority assignment**: Can be decision variable in DSE
- **Schedulability analysis**: Constraint checking in DSE
- **WCET**: Input to performance evaluation function
- **Example**: DSE finds allocation + binding + scheduling that meets all deadlines

### 9. Lecture 14 (Multi-Processor Systems)
- **Allocation problem**: How many processors? What types? → DSE decision variables
- **Task mapping**: Binding problem in multi-processor DSE
- **Communication costs**: Non-linear effects that EAs can handle
- **Load balancing**: Emerges from DSE binding and scheduling
- **Example**: Network processor DSE allocates heterogeneous processors for packet processing

### 10. Lecture 03 (Requirements)
- **Functional requirements**: Define what tasks must be allocated/bound/scheduled
- **Non-functional requirements**: Become DSE objectives and constraints
- **Requirement prioritization**: Can influence objective weights in DSE
- **Traceability**: Solutions must satisfy functional requirements
- **Example**: User requirements → formal DSE objectives (e.g., "responsive" → "latency < 200ms")

### 11. Lecture 02 (Software Paradigms)
- **Concurrency models**: Affect scheduling decision space
- **Component-based design**: Provides allocation/binding structure
- **Dataflow models**: Define dependencies for scheduling constraints
- **OO design**: Can structure solution representation
- **Example**: Active objects → tasks to bind to processors

### 12. Lecture 01 (Embedded Systems)
- **Resource constraints**: Become DSE constraints (memory, power, cost)
- **Real-time requirements**: Timing constraints in DSE
- **Application domains**: Determine relevant objectives (automotive: safety + performance; IoT: energy)
- **Heterogeneity**: Motivates diverse hardware allocation
- **Example**: Embedded constraints make DSE necessary (can't just "buy bigger hardware")

### 13. Labs (P1-P5)
- **P2 (Energy Measurement)**: Provides energy models for DSE evaluation
- **P3 (AES Multi-Processor)**: Example of allocation DSE (A53 vs R5 vs both)
- **P4 (AES Hardware)**: Hardware/software partitioning DSE
- **P5 (DSE Lab)**: Direct application of EA search algorithms
- **P1 (Hardware Bringup)**: Platform characterization provides DSE parameters

### 14. Optimization Theory
- **Linear programming**: Special case of DSE with efficient solvers
- **Convex optimization**: Another special case with guarantees
- **Metaheuristics**: EA is one family of metaheuristics (also: simulated annealing, particle swarm)
- **Multi-objective optimization**: Mathematical foundation of Pareto-based EA
- **No-free-lunch theorem**: No single best search algorithm for all problems

## Summary

**Design Space Exploration - Search Algorithms** focuses on how to efficiently find Pareto-optimal solutions in large, complex design spaces. Key takeaways:

### Core Principles
1. **Search Strategy Selection**: Choose search based on problem characteristics (linear→solver, small→exhaustive, large non-linear→EA)
2. **Solution Encoding**: Represent designs as vectors (binary, integer, real-valued) suitable for computer manipulation
3. **Population-Based Search**: EAs maintain diverse set of solutions, evolving them toward Pareto front
4. **Variation Operators**: Mutation (local changes) and recombination (mixing solutions) create offspring
5. **Selection Mechanisms**: Tournament/fitness-based for parents, Pareto-based for survival

### Key Insights
- **Trade-offs Everywhere**: Evaluation accuracy vs speed, exploration vs exploitation, population size vs generations
- **No Guarantees**: EAs approximate optimal solutions but don't guarantee finding true optimum
- **Practical Effectiveness**: Despite lack of guarantees, EAs find high-quality solutions in practice for complex DSE problems
- **Problem-Specific Tuning**: EA parameters, encodings, and operators should be adapted to problem structure
- **Multi-Fidelity Strategy**: Use fast evaluation for broad search, detailed evaluation for final candidates

### Three Pillars of Successful DSE
1. **Suitable Design Representation**: Encoding that enables efficient variation and validity checking
2. **Fast Evaluation Method**: Computing objective values quickly enough to explore many designs
3. **Effective Search Strategy**: Balancing exploration of design space with exploitation of promising regions

### Practical Application
- Linear problems: Use optimization solvers (fast, exact)
- Small spaces (< 10K): Exhaustive enumeration
- Large non-linear spaces: Evolutionary algorithms with population 50-200, generations 100-1000
- Very expensive evaluation: Small population, surrogate models, or multi-fidelity approach

The lecture demonstrates that while finding optimal embedded system designs is computationally challenging, evolutionary algorithms provide a practical and effective approach that scales to real-world complexity.

## Possible Exam Questions

### Definition & Terminology (Basic Understanding)

1. What is a search strategy in the context of DSE?
2. Define linear optimization and give an example.
3. What is solution encoding in evolutionary algorithms?
4. Define a population in an evolutionary algorithm.
5. What is a generation in the context of EAs?
6. Define mutation as a variation operator.
7. What is recombination (crossover)?
8. Explain what "selection for variation" means.
9. Define "selection for survival."
10. What is a binary representation of a solution?
11. Explain what an allocation vector represents.
12. Define the binding vector in system design DSE.
13. What is a fitness value?
14. Define offspring in evolutionary algorithms.
15. What is an infeasible solution?
16. Explain what convergence means in EA context.
17. What is premature convergence?
18. Define elitism in evolutionary algorithms.
19. What is tournament selection?
20. Explain what diversity means in a population.

### Conceptual Understanding (Explain & Describe)

21. Explain why binary encoding is commonly used for DSE solutions.
22. Describe how mutation helps prevent premature convergence.
23. Explain the difference between exploration and exploitation in EA search.
24. Describe how crossover combines information from two parent solutions.
25. Explain why EAs maintain a population rather than a single solution.
26. Describe the role of selection pressure in evolutionary algorithms.
27. Explain how non-linear relationships between decisions and objectives complicate DSE.
28. Describe the evolutionary loop and its main phases.
29. Explain why exhaustive search is infeasible for large design spaces.
30. Describe how Pareto-dominance is used in selection for survival.
31. Explain the trade-off between evaluation accuracy and search efficiency.
32. Describe how feasibility checking fits into the EA process.
33. Explain why linear optimization problems can be solved efficiently.
34. Describe the purpose of tournament selection.
35. Explain how solution representation affects EA effectiveness.

### Application & Analysis (Apply Concepts)

36. Given solution (1,0,1,1,0), apply bit-flip mutation with 20% rate. What are possible outcomes?
37. Two parents: (1,1,0,0) and (0,0,1,1). Apply single-point crossover at position 2. What are the children?
38. For potato field with x₁=30, x₂=80, does this satisfy constraint x₁+x₂≤100?
39. Calculate harvest for x₁=40, x₂=60 using f₁=3.5×x₁+5×x₂.
40. Calculate savings for same solution using f₂=1000-6×x₁-7.5×x₂.
41. A design space has 8 processors (on/off), 4 memory sizes, 3 cache configs. How many total designs?
42. Population size 100, offspring size 100. How many solutions before selection for survival?
43. With mutation rate 2% and solution length 50, what's the expected number of bit flips?
44. Given solutions A(10J, 5s), B(12J, 4s), C(9J, 6s), which are non-dominated?
45. If evaluation takes 0.1s, population=100, generations=200, what's total runtime?
46. Design space size 10^15. EA evaluates 20,000 designs. What percentage explored?
47. Binary tournament: pick 2 from {A(good), B(medium), C(poor)}. Probability of selecting A?
48. For crossover rate 80%, how many of 100 offspring created by crossover vs mutation?
49. After 50 generations with no improvement in Pareto front, what should you do?
50. Given bit string (1,1,1,1,1,1,1,1), what's probability all bits stay 1 after mutation at 5% rate?

### Comparison & Contrast (Analyze Differences)

51. Compare linear vs non-linear optimization in terms of solution methods.
52. Compare mutation vs recombination in terms of their effects on solutions.
53. Compare selection for variation vs selection for survival in terms of purpose.
54. Compare exhaustive search vs evolutionary algorithm in terms of scalability.
55. Compare binary encoding vs integer encoding for DSE.
56. Compare small vs large population sizes in EA.
57. Compare single-point vs uniform crossover.
58. Compare tournament selection vs fitness-proportional selection.
59. Compare repair strategy vs penalty strategy for handling infeasibility.
60. Compare early vs late generations in EA in terms of population diversity.

### Procedure & Process (Methodological Knowledge)

61. Describe the complete evolutionary algorithm process from initialization to termination.
62. How do you apply single-point crossover to two binary strings?
63. Describe the process of tournament selection step-by-step.
64. Explain how to check if a solution is feasible.
65. Describe how to encode an allocation decision as a binary vector.
66. How do you extract the Pareto front from a population?
67. Describe the repair process for a constraint-violating solution.
68. How do you initialize a random population for an EA?
69. Describe the process of binary mutation step-by-step.
70. How do you decide when to terminate an EA?

### Problem Solving (Practical Scenarios)

71. You have 10^20 designs to explore. What search strategy do you recommend?
72. Your EA converges after 20 generations to suboptimal solutions. What adjustments might help?
73. Evaluation takes 1 hour per design. How do you configure EA parameters?
74. All offspring solutions are infeasible. What's likely wrong?
75. Pareto front has only 3 solutions after 1000 generations. What might be wrong?
76. You need to optimize 3 objectives but have linear relationships. What approach?
77. Design space is 1000 designs. EA or exhaustive search?
78. Population loses all diversity by generation 30. How to prevent?
79. Your boss wants "the best" design from EA. How do you respond?
80. EA finds better solutions in run 1 than run 2. Is this a problem?

### Trade-offs & Design Decisions (Critical Thinking)

81. Why might you choose small population with many generations vs large population with few generations?
82. Discuss trade-off between evaluation accuracy and number of designs explored.
83. Why use both mutation and crossover rather than just one?
84. Explain why guaranteed optimality is sacrificed in favor of EA approach.
85. Discuss when linear programming is better than EA for DSE.
86. Why might binary encoding be preferred despite poor human readability?
87. Discuss tension between exploration and exploitation in EA.
88. Why might you accept suboptimal solutions from EA?
89. Explain trade-off between population diversity and convergence speed.
90. Why is selection for survival always based on Pareto-dominance rather than weighted sum?

### Real-World Application (Practical Understanding)

91. For the potato field problem, why is a linear solver sufficient?
92. For the network processor, why is EA necessary instead of linear solver?
93. In the network processor example, how many design variables are there approximately?
94. Why does adding fertilizer create a non-linear problem?
95. How would you encode a hardware/software partitioning decision for 10 functions?
96. For IoT sensor node with 5-year lifetime goal, what are key objectives?
97. In automotive ADAS, why might you partition vision functions to FPGA?
98. How does communication overhead create non-linearity in multi-processor DSE?
99. Why might heterogeneous architectures appear on Pareto front more than homogeneous?
100. For data center server config with 192 options, which search method?

### Integration & Synthesis (Connect Concepts)

101. How does solution encoding affect crossover operator design?
102. Explain relationship between problem size and recommended search strategy.
103. How does evaluation cost influence EA population size choice?
104. Explain how DSE Lecture 11 and 12 complement each other.
105. How do energy models from Lecture 08 feed into DSE evaluation?
106. Explain connection between allocation, binding, scheduling and solution encoding.
107. How does FPGA configurability relate to DSE search?
108. Explain how NFRs from Lecture 06 become DSE objectives.
109. How does scheduling analysis from Lecture 13 integrate with DSE?
110. Explain role of performance models from Lecture 09 in DSE fitness evaluation.

### Advanced Analysis (Deep Understanding)

111. Why don't EAs guarantee finding global optimum, yet work well in practice?
112. Explain how EA balances global search with local refinement.
113. Discuss why Pareto-based selection is better than weighted sum for multi-objective EA.
114. Why might increasing mutation rate help stuck population?
115. Explain how diversity maintenance prevents premature convergence.
116. Discuss why design space size alone doesn't determine EA difficulty.
117. Why might different EA runs produce different Pareto fronts?
118. Explain relationship between problem structure and effective crossover design.
119. Discuss when adding more generations no longer improves results.
120. Why might EA perform poorly on problems with many local optima?
