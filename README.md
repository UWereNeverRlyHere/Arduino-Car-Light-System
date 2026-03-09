This is a pet project built to experiment with hardware and state-machine logic on microcontrollers.
As my primary background is in high-level C#/.NET backend development, the C++ implementation here prioritizes functional logic and integration over strict embedded constraints.

⚙️ How It Works 
The project is built around a non-blocking, event-driven architecture. Instead of relying on delay() functions that freeze the microcontroller, the main loop() utilizes state machines and timers (tick() methods). This allows the system to simultaneously process physical button presses, control mechanical relays for headlights, and smoothly animate RGB LEDs without any lag.

The state of the decorative RGB lighting (colors and active modes) is automatically saved to the microcontroller's EEPROM memory. This ensures that the system remembers the driver's personalized lighting preferences even after the car's power is completely turned off and restarted.

🏗️ Core Components
The codebase is organized using Object-Oriented Programming (OOP) principles to separate hardware interactions from business logic:

LightControl: The base class that encapsulates the state and logic for a single binary light source or relay (On/Off). It tracks current states and prevents redundant signal writing to the pins.

LightSwitcher & SkullSwitcher: High-level controllers managing specific lighting zones (main headlights/marker lights vs. decorative RGB "Skull" elements). They process complex inputs from the car's controls.

HoldingTimeHelper: A custom timing utility designed to differentiate between short clicks and long holds of physical buttons, enabling multi-functional inputs from a single switch without blocking the execution thread.

GRGB: A handler for the RGB LEDs, managing smooth PWM color transitions and interacting with the EEPROM for state persistence.

🛠️ Configuration 
All hardware-specific configurations are strictly separated from the logic classes.
Pin mappings (inputs from buttons, outputs to relays, PWM pins for LEDs) and default system parameters are centralized in the GlobalPins.h file. This makes porting the code to a different Arduino board or modifying the wiring extremely simple, requiring zero changes to the core engine.