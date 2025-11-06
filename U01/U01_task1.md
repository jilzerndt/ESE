# Task 1: Requirements

**Given:** Standard Crossroad Scenario (see pdf for graphic)

## Tasks:

### Formulate 5 traffic light control system requirements concerning its correct operation:

1. No Green Lights at Same Time:
The traffic light system must make sure that the north-south and east-west directions never both have green lights at the same time, otherwise cars might crash or there could be a situation where the cars basically "deadlock" themselves.

2. Yellow Light Before Red
When a traffic light changes from green to red, it must show yellow for 3 seconds first so drivers have time to slow down.

3. Minimum Green Time
Each direction should get at least 15 seconds of green light so enough cars can pass through the intersection.

4. Red + Yellow Before Green
Before the light turns green, it should show red and yellow together for 2 seconds to warn drivers that green is coming soon.

5. Safety Gap Between Switches
When switching from one direction to another, all lights should be red for 2 seconds to clear the intersection.

### Hand the requirements over to a colleague and ask for honest feedback: (per requirement)

**Reviewer: Dominic Odermatt**

1. Good start, but what happens if there's a system failure? Should specify a safe default.

2. Where do the 3 seconds come from (traffic regulations)?

3. again why 15 seconds? And shouldn't there be a maximum time too? What if one direction stays green forever?

4. ok, but same question as 2, is this based on regulations?

5. What exactly is meant by "from one direction to another"? Should specify what actions are expected from drivers.

General Feedback: 
- What about the pedestrian crossings?

### Improve your requirements according to the feedback received:

1. No Green Lights at Same Time:
The traffic light system must make sure that the north-south and east-west directions never both have green lights at the same time, otherwise cars might crash or there could be a situation where the cars basically "deadlock" themselves. In case of system failure: all lights flash yellow to indicate that drivers must proceed with caution and follow standard right-of-way rules.

2. Yellow Light Before Red
When a traffic light changes from green to red, it must show yellow for 3 seconds first so drivers have time to slow down. 
Duration is based on the traffic regulations I found.

3. Minimum Green Time
Each direction should get at least 15 seconds of green light so enough cars can pass through the intersection.
Duration is based on the traffic regulations I found. Maximum green time could be something like 60 seconds to prevent starvation of the other direction.

4. Red + Yellow Before Green
Before the light turns green, it should show red and yellow together for 2 seconds to warn drivers that green is coming soon.
Duration is based on the traffic regulations I found.

5. Safety Gap Between Switches
When switching from one direction to another, all lights should be red for 2 seconds to clear the intersection.
Direction change means switching from NW to SE for example. Or any two directions that might have conflicting traffic flows. 

6. Pedestrians
The traffic light system must include pedestrian crossing signals that coordinate with vehicle traffic lights to ensure safe crossing times for pedestrians. When a pedestrian signal is green, the corresponding vehicle traffic light must be red.