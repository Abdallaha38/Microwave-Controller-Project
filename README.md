Microwave Oven Controller Project

Aim: This project simulates the embedded controller in a microwave oven.
Requirements:
• LCD
• Keypad
• One External Push Buttons
• TM4C123G Launch Pad

Procedure Details:
While not cooking (stopped), enter a choice to begin cooking:
• If A is pushed on the keypad (for popcorn), the LCD should show “Popcorn” and then
cook for 1 minute while the remaining cook time counts down (in seconds) on the LCD,
then clear the LCD after cooking completes.

• If B (for Beef) or C (for chicken) is pushed on the keypad, the words “Beef weight?” or
“Chicken weight?” (respectively) should appear on the LCD. After that, the user must
enter an integer value between 1 and 9 on the keypad to indicate how many kilos are
there to be defrosted.

Note that only whole kilo values are to be entered. After a valid number is entered, clear
the LCD display and show the value of the weight on the LCD for 2 seconds, and then
start cooking while the remaining cook time counts down (in seconds) on the LCD
o Beef is defrosted at a rate of 0.5 minutes per kilo.
o Chicken is defrosted at a rate of 0.2 minutes per kilo.
o If an illegal number is entered, then the LCD should show “Err” for 2 seconds,
then show previous message.
If D is pushed on the keypad, the words “Cooking Time?” should appear on the LCD.
After that the user can enter a value between 1 and 30 to indicate the cooking time
required in minutes and seconds. This value is displayed on the LCD as it is entered, right
to left.
For example, pressing 1 displays 00:01, then pressing 2 displays 00:12, then pressing 4
displays 01:24, then pressing 5 displays 12:45. Press push button SW1 to clear the LCD
display and press SW2 to start cooking and counting down (in seconds) the cooking time
on the LCD.

Start/Stop/Pause cooking conditions:
• Switch SW3 (external push button), position 1, will simulate the microwave oven door
latch, where the switch being down would be simulating the open-door situation and the
switch being up would be simulating the door closed situation. Only when the latch is
closed should the oven be able to be started.
o When SW2 is pressed, the oven starts operation.
o When SW1 (external push button) is pressed for first time, the oven operation
pauses (keeping remaining time on the display).
o When SW1 is pushed for second time after pressing it for first time, the time is
cleared and the cooking stops.
o If SW2 is pushed after the oven is paused and the door is closed, then cooking
must resume from the time it was paused.
• If it is cooking, opening the door should pause the cooking and keep the remaining time
on display.
• When the microwave is running, the array of LEDs should be on. When it is stopped,
they should go off. If paused, the array of LEDs should blink (wait time on and wait time
off) till the cooking is resumed or stopped.
When the microwave completes its function and timer has counted down to zero (regular
timed cooking or defrosting), the array of LEDs should flash 3 times (wait time on and
wait time off), and the buzzer should produce an audible tone/alarm during these 3
second times periods.

Number of Students
The project team should be between 4-6 members.

Project Delivery
1. PDF document contains the state machine diagram you implemented to simulate
Microwave controller and explains it.
a. The pdf should contain the contribution of each team member
b. The pdf should contain different snapshots covering different scenarios
mentioned in “Procedure Detailes” section
c. The pdf should contain the valid link of the recorded video for your
project
d. The pdf should contain the valid GitHub link of your project
2. You should submit your code files compressed in .zip format.
3. The video should show all the cases mentioned under “Procedure Details”
section.
4. A Demo will be held after the submission.

Project Deadline
1. The project will be submitted on 20th May.

2. The Demo will be held after that between 21st

-24th May.

Project Instructions
1. Download the kit header file from the below link to include it to use its
defined macros in your code.

https://drive.google.com/file/d/1Gyt1VkYgfyEYHeF1VL6ivI9W2FQkB-
GQ/view?usp=sharing

2. Your implementation should be in embedded C.
3. The demo video should be taken as one shot without cuts or edits.
Evaluation
1. 25% of the marks for individual contribution specially the GitHub repository
contribution.
2. 75% of the marks for the project team.
Note: A team member without contribution on GitHub repo will get ZERO.


Notes:
1. You can make your GitHub repo private until submission and then adjust it to be
public to be evaluated.
2. The timer of Microwave controller should be implemented using Systick Timer.
3. The maximum bonus marks of the project are 5 marks.
4. You would gain bonus mark if you wrote your project in a good coding style.
5. Bonus marks will be provided if extra or sophisticated functionalities have been
added. E.g. implementing the LCD interfacing without using built-in LCD driver.
