## How to create custom effects on "DNA" example

For adding custom effects to this firmwate you need to:
1. Make a fork of [wowfire-lamp](https://github.com/2SmartCloud/2smart-cloud-esp32-wowfire-lamp).
  
2. Clone the project using ```git clone``` from your github account.
   
3. Create the new brnach and add your changes:
   a) add method of your effect in `lib/lenta/lenta.h`
   
   b) add the effect name into `LedStripStates`
   
   c) attach effect name for mobile application in `modes_`
   
   d) add the body of the method with a description of all the logic of the effect in `lib/lenta/lenta.cpp` . Add helper methods if needed.
      Attention! Do not forget that LED lighting is also a mode task for which the `LEDS.show ();` command must be used.
      
   e) add effect to switch in `HandleCurrentState`
   
   f) build the firmware with the pio run command (in this case, you need to be in the Firmware folder).
     
   g) ready-to-upload `firmware.bin` will be in the `Firmware/.pio/build/esp32dev` folder (enadle hidden files to find it)
   
   
  







