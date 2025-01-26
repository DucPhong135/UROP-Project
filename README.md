# Eye Movement-Based Game Control Using EOG Signals

## Overview
This project explores the use of Electrooculography (EOG) signals to detect eye movements in four directions: up, down, left, and right. By leveraging these signals, the project enables hands-free control of a game, such as **2048** or **Snake**. The system translates eye movements into actionable commands, creating an innovative and accessible interface for gaming and potentially other applications.

This project was undertaken as part of the **Undergraduate Research Opportunities Programme (UROP)** at the **National University of Singapore (NUS)**.

---

## Features
- **EOG Signal Acquisition:** Captures eye movement data using EOG sensors.
- **Direction Detection:** Classifies eye movements into four directions: up, down, left, and right.
- **Game Integration:** Maps detected movements to game controls for interactive play.
- **Hands-Free Interaction:** Promotes accessibility for users with limited motor abilities.

---

## System Architecture

1. **EOG Signal Acquisition:**
   - Hardware setup includes EOG electrodes placed around the eyes to measure voltage changes caused by ocular movements.
   - Signal is amplified using a bio-amplifier and processed using an Arduino-compatible microcontroller.

2. **Signal Processing and Direction Detection:**
   - The Arduino processes the raw EOG signal and sends the data to a Python script via serial communication.
   - The Python script simply forwards the detected commands to simulate arrow key presses.

3. **Game Control Interface:**
   - The detected eye movements are mapped to game controls (e.g., arrow keys for **2048** or movement keys for **Snake**).
   - Communication with the game is established via the simulated keyboard input.

---

## Requirements

### Hardware:
- Arduino-compatible microcontroller.
- Bio-amplifier for signal preprocessing.
- Electrodes for EOG signal measurement.
- Proper cables and connectors.

### Software:
- Arduino IDE for programming the microcontroller.
- Python 3.x (basic setup for serial communication and keyboard input).

---

## Setup and Installation

1. **Hardware Setup:**
   - Attach electrodes around the eyes following EOG signal placement guidelines. Example placements:

     **Horizontal Electrode Placement:**
     ![image](https://github.com/user-attachments/assets/d22ad764-3116-4f31-b906-622bcbe07e42)


     **Vertical Electrode Placement:**
     ![image](https://github.com/user-attachments/assets/7551db7a-bd91-4c22-b9fc-f049812a3f0e)


   - Connect the electrodes to the bio-amplifier, and then to the Arduino-compatible microcontroller.
   - Ensure proper grounding to minimize noise.

2. **Run the Application:**
   - Upload the Arduino code to your microcontroller via the Arduino IDE.
   - Start the Python script to process incoming signals and simulate keyboard input:
     ```bash
     python game_controller.py
     ```

---

## How It Works

1. **Calibrate the System:**
   - Run the calibration module on the Arduino to record EOG signals for each direction.
   - Adjust thresholds and save calibration data for consistent detection.

2. **Detect Eye Movements:**
   - The Arduino processes real-time EOG signals and classifies the direction of eye movement.

3. **Control the Game:**
   - Eye movements are mapped to game actions (e.g., move up, down, left, right).
   - Play **2048** or **Snake** using only your eye movements.

---

## Project Goals
- To demonstrate the feasibility of EOG signals for hands-free game control.
- To provide an accessible interface for individuals with mobility impairments.
- To explore potential applications of EOG technology beyond gaming, such as in assistive devices and smart home controls.

---

## Challenges and Future Work

### Challenges:
- Reducing noise and artifacts in EOG signals.
- Achieving high accuracy in direction detection.
- Ensuring compatibility with different hardware setups.

### Future Work:
- Enhance the system with advanced filtering techniques for better signal clarity.
- Extend functionality to control more complex games or applications.
- Miniaturize the hardware for portability.

---

## Acknowledgments
This project was supported by the **Undergraduate Research Opportunities Programme (UROP)** at the **National University of Singapore (NUS)**. Special thanks to the project supervisor Chua Dingjuan for guidance and support.
---

