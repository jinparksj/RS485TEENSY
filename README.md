# RS485TEENSY
__RS-485 Library__ (__Full Mesh__ (Multi-drop Bus)) with __Teensy3.5__ which is __32 bit 120 MHz ARM Cortex-M4__ processor with floating point unit 

---

**1. Summary**
- It is designed for RS-485 communication in BUS terminating 120ohm resistors at each end.
- It is originally designed to communicate two Teensy3.5 and six Arduino Mega.
- Each Teensy and Arduino Mega have IDs, such as 0, 1, 2, ..., 7.
- For example, first Teensy has ID 0 and first Arduino Mega has ID 1.
- Based on the ID, each module takes serial packets and sends packet.
- It has noise and error corruption filter based on ASCII number.
- It has Parity Checker.


**2. Important Notice**
- I tested 2M, 250K, and 9.6K baudrate. RS-485 communication with Arduino Mega and Teensy3.5 with 2M bps shows error.
- Error at 2M bps seems to be caused by Arduino Mega.
- 250K bps seems fine and shows optimized results with 24-hrs testing in vulnerable conditions.

 