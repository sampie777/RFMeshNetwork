# RF Mesh Network

**UNDER DEVELOPMENT**

_Goal: create a mesh network of RF devices which passes messages through without knowledge of the origin or destination of the message so that by chaining these devices, messages can be sent through the whole network, no matter the size._

## Concept

The idea is that every device runs the same networking code (which will be the main contents of this repository) in order to make the network modular. So adding/removing devices won't require any changes in the network or on other devices. Preferable, the added device also won't require any configuration. 

Messages are been send and relayed by other devices. As the devices itself don't know their environment (e.g. other devices in the mesh), their messages are been broadcasted, so is every relayed message. Each device keeps an in-memory list of received messages in order to prevent resending an already sent message. This list acts like a rolling cache containing only the latest messages. 

Using this concept, a device A can send a message destined for device C even if they are out of range of each other, given device B is in range of both. Device B will receive A's message and broadcast it so that C will receive A's message.

The use of 433 MHz improves the device's range, when comparing to WiFi devices. Using custom antennas, the range can be improved even more. Also, 433 MHz is free to use and the required hardware is cheap.  

### Use cases

#### Farming

The use case which gave me this idea is that of an irrigation system on a farm. Modernizing this system will require lots of electronic valves and sensors spread across a large area of land. Most devices won't be in reach of a central placed router, but they will be in reach of each other at one point or another. Using this mesh network, a central server (which acts like just any other device in the mesh) will be able to send messages to the various valves and receive messages from the various sensors, no matter how far they are from the server, as long as there are other devices covering the area between. 

This eliminates the use of cables or WiFi repeaters (as the devices themselves act like repeaters). Only the power for the devices will need another solution. One can think of batteries or solar cells in order to remove the need of cables.  

#### IOT

The same can be done for IOT devices. Instead of connecting every new device with your WiFi and let all those devices use up your router's address space, one can swap these devices out for devices using a 433 MHz RF mesh network. Some products, like SOnOff, already use this frequency in some of their products. 

## Hardware

For this project, an ESP32 is used, because of it's WiFi capabilities. The network itself is based on 433 MHz transceivers. WiFi is handy during development and won't be part of the final product. 

- ESP32 dev board (or any Arduino if WiFi is not necessary)
- 433 MHz transmitter [module ZF-3](https://www.botshop.co.za/product/rf-433mhz-transmitter-module-zf-3/)
- 433 MHz receiver [module ZR4](https://www.botshop.co.za/product/rf-433mhz-receiver-module-zr4/)

> Make sure to attach antennas to the transmitter and receiver, otherwise they might not work. Also note the difference in pin layout of the transmitter (GND - VCC - DATA) vs receiver (GND - DATA - DATA - VCC).

## Software

The code makes use of a Mesh object. This object is initialized with three parameters: GPIO pin for the transmitter, GPIO pin for the receiver (must be an interrupt pin and be defined by the interrupt pin name, e.g. 0 for Arduino D2 or 13 for ESP32 GPIO13), and the device mode (receive only, transmit only, receive and transmit). 

The software is still under development. Features like CRC and a sturdy message protocol must be implemented (correctly). 
