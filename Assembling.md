## Assembling WOW Fire Lamp

### Complete set of accessories of lamps: 

- Esp32 devkit;
- touch button;
- clock button;
- matrix of addressable LEDs 16x16;
- 3 wires 27cm long;
- microusb connector pcb;
- body parts;

![image](static/assembling/1.JPG)

![image](static/assembling/1_1.JPG)

It is best to upload the firmware on the board before you start assembling the lamp.
Firmware uploading instructions - [Firmware-Uploading](Firmware-Uploading.md)


### Assembling
1. Prepare the touch button - unsolder the diode (if we don’t want it to glow) and put a jumper on the A pad.

![image](static/assembling/2.JPG)

![image](static/assembling/3.JPG)

2. Solder three wires to the touch button.

![image](static/assembling/4.JPG)

3. Solder the reset clock button to the back of the board.

![image](static/assembling/5.JPG)

4. Unsolder all wires from the matrix, except for the ```DIN``` connector.

![image](static/assembling/6.JPG)

5. Solder the ```DOUT``` connector soldered from the matrix to the inner side of the board.

![image](static/assembling/7.JPG)

6. Solder the Micro-usb input to the inner side of the board.

![image](static/assembling/8.JPG)

7. Solder Esp32 to the inside of the board. 

![image](static/assembling/9.JPG)

8. Connect **through the soldered micro-usb** power supply and check that the matrix is working.

![image](static/assembling/10.JPG)


## At this stage, you get a ready-to-use controller to which you can connect not only an LED matrix, but also use it as a device for controlling addressable LED strips.




9. Pass the wires of the touch button through the hole in the tube and solder to the board, the signal wire directly to pin ```D19```.

![image](static/assembling/11.JPG)

10. Install the assembled controller in the seat in the lower part of the case.

![image](static/assembling/12.JPG)

11. Installing the inner tube.
![image](static/assembling/13.JPG)

12. Put on and fix the matrix using zip ties, the matrix strips shoul place horizontally.

![image](static/assembling/14.JPG)

![image](static/assembling/15.JPG)

13. Lay the wires.

![image](static/assembling/16.JPG)

14. Put the lid on the inner tube without pressing tightly, if after assembly the lamp switches modes by itself, the lid should be slightly raised.

![image](static/assembling/17.JPG)

15. Close everything with an external case and turn on the power.

![image](static/assembling/18.JPG)



The lamp is ready for [pairing into the mobile app](https://www.youtube.com/watch?v=kvkkDE4caBU&feature=emb_imp_woyt). 
