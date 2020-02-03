# SmartFlowController

The project was conducted as a master thesis in Tallinn University of Technology.

A device was designed for liqiud flow contol.

Hardware used:

ESP32 microcontroller,

infrared transistors 175-22-139 (SFH300-3/4),

infrared emittersOP298A,

12V 300mA peristaltic pump,

IRF520N,

LM2596 DC/DC buck regulator,

MT3608 DC/DC Step up module,

2x 18650 lithium ion batteries


Solution:

ESP32 is working as a webserver. Clients can connect through wifi to ESP32 and switch the pump on/off. The speed and on time is
set by the user in the browser through interactive sliders.

Flowrate of the liquid is measuered 3 times.

So initially the liquid is flowing with an estimated flowrate.

Liquid in the tube will be detected in point 1.

Liquids flow rate will be adjusted in Point 2.

In point 3 liquids flow rate is measured again and adjusted.

In point 4 the final measurement and adjustment is done.


Right now any changes in liquid flow and pressure are not constanlty monitored.




