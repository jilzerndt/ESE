# Exercise 3 – Test & Integration
03.10.2025, kuex


After successfully implementing the washing machine control system in last week’s exercise, it’s time
to bring your code to the real HW. Rewrite your code so that it no longer relies on buttons,
dipswitches, and LEDs to mock your washing machine, but use the HW interfaces as described in
Annex A (or as seen in dominics implementation in dominic/03_U2/project_CT_board_version)

Tasks:
- Update your implementation of the state machine according to your own specification from
last week’s exercise by further extending the Keil uvision project available on Moodle
- Create the real HW access functions as described in Annex A by implementing the same API
you defined last week.
- Test your implementation of the washing machine using the real HW

## Annex A

### Inputs and Outputs

All inputs are “active-low”, i.e. a logical “0” at the input “Door Closed” signals to the control unit that
the door is closed. Edges indicate events. Hence, a falling edge on the signal “Door Closed” indicates
that the door has been closed, a rising edge indicates the door's opening.
All outputs are “active-low”, too. E.g., if the motor shall run at slow speed, output 3 shall be a “0”
and consequently, output 2 shall be a “1”.

Just do the same thing as in Dominics implementation.

The connector can be accessed through the following memory addresses:
- Input: 0x60000410
- Output: 0x60000400

See also ennis.zhaw.ch for programming examples and further details:
https://ennis.zhaw.ch/wiki/doku.php?id=ctboard:peripherals:gpio_cpld