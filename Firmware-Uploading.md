## Firmware uploading 

### How to write firmware on device


Go to your project folder, and then go to the firmware folder, in this folder open a command prompt.

1. Need have:  
     `python` (>= v3) installed. You can control it in terminal      
    ```
    python --version
    ```    

    `platformio` (>= v5.1.1)
    ```
    pip install -U platformio
    ```    

2. Have connected device to your computer.

3. Device should be listed in /dev as one of this:

    ```
    (Linux)
    /dev/ttyUSB0

    (OSX)
    /dev/cu.SLAB_USBtoUART
    /dev/cu.usbserial-0001
    ```

4. build and write

    ```
    pio run -t uploadfs
    pio run -t upload
    ```

5. If everything is okay it should start in AP mode and blink once in a second.

If you want just build 
    ```
    pio run 
    ```
If you have error "can't open device "/dev/ttyUSB0": Permission denied" follow Link https://qna.habr.com/q/526674

CLI guide https://docs.platformio.org/en/latest/core/userguide/index.html
