# uart-sandbox
Test code for UART development

loopbacktest.c
-------------------
Loop back test for auart serial port.
Need to modify the serial port definitions for you application before compiling.
To compile:
	gcc -o loopbacktest loopbacktest.c

Will need a wire to connect RX to TX signal
Does not use hardware flow control

 
multi-uart-example.c
----------------------
Communicate from one serial port to another on the same SCM (system on module)
Need to modify the serial port definitions for your application before compiling.
To compile:
	gcc -o multi-uart-example multi-uart-example.c
	
Will need to wire one port to another.
