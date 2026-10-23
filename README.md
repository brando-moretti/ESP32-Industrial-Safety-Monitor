

#  ESP32 Industrial Safety Monitor

A hardware-software prototype demonstrating non-blocking embedded architecture, memory-safe data handling, and real-time analog threshold monitoring.

##  The Project
This system continuously samples data from an analog sensor (potentiometer/trimmer) and triggers a physical safety alert (LED) when a specific threshold is crossed. 

Unlike standard hobbyist code, this firmware is written with industrial principles: it avoids `delay()` functions entirely to prevent CPU blocking and uses memory pointers for efficient data logging.

##  Tech Stack & Hardware
* **Microcontroller:** ESP32
* **Language:** C / C++
* **Environment:** PlatformIO (VS Code)
* **Components:** 10k Trimmer, Pushbutton, LED, Breadboard

## Key Engineering Features
1. **Non-Blocking Architecture:** Utilizes `millis()` hardware timers for asynchronous sensor sampling (every 2 seconds) without halting the main loop.
2. **Memory Management:** Implements C++ `struct` and `enum` to logically organize sensor data (Value and State).
3. **Pointer-Based Logging:** Stores a rolling history of the last 5 readings, passing data to the serial monitor via memory addresses (`*` and `&`) to minimize RAM overhead.
4. **Hardware Interrupt Ready:** Pushbutton logic utilizing internal pull-up resistors (`INPUT_PULLUP`).

##  How to Run
1. Clone this repository.
2. Open the project folder in **VS Code** with the **PlatformIO** extension installed.
3. Wire the components to the ESP32 (Potentiometer to Pin 34, LED to Pin 2, Button to Pin 5).
4. Compile and upload the firmware. Set Serial Monitor to `115200` baud rate.
