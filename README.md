# subaru_multi_gauge
Arduino-powered gauge that can measure anything sent via OBDII from the car. Adapting for use in Subaru vehicles to give more info to the adventure enthusaist.

This device uses Arduino's easy-to-use single-board microcontroller to pull information from a vehicle's CAN bus via the OBDII port to display important information. The device requests a specific PID from the vehicle's CAN-bus (which returns an array of hex values), and converts and formats it into meaningful information.

The data that can be acquired from the CAN bus varys by vehicle; some of the data I've found that can be useful is listed here by PID:
Calculated engine load: 0x04
Engine coolant temp: 0x05
Engine RPM: 0x0C
Vehicle speed: 0x0D
Intake air temp: 0x0F
MAF air flow rate: 0x10
Throttle position: 0x11

A full list of PIDs can be found here: https://en.wikipedia.org/wiki/OBD-II_PIDs

There are many applications for this device, as information provided to the driver depends on what the manufacturer decides to make available on the display and decides how approximate or exact the data displayed is. For example, the first-gen Subaru XV Crosstrek displays 4 different coolant temperature states with one light: blue light, light off, red blinking light, red light; where blue light is "cold", light off is "warmed up", blinking red light is "close to overheating", and solid red light is "overheating". While this is useful information, some drivers may find it useful to have a greater understanding of how the system is performing.

Since you can program the Arduino to not only pull data but do calculations on the data pulled, you can also calculate information in-device before it's displayed such as instantaneous MPG, average RPM over a certain distance, temperature change, etc.

You can also interface the Arduino with many different kinds of displays and sensors which really opens up how the driver can interact with the device.
