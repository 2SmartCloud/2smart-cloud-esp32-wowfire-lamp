## Firmware uploading 

### How to write firmware on device using  `platformio`

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

CLI guide https://docs.platformio.org/en/latest/core/userguide/index.html



### How to write firmware on device using  `esptool`

1. Install the latest version of `esptool`.

3. Connect esp32 to your computer. WARNING - DO NOT connect usb when led strip or matrix is connected.

4. Open the `Firmware` folder.

5. Upload the firmware using next commands:

```esptool.py --chip esp32 --port "/dev/ttyUSB0" --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_size detect 2686976 spiffs.bin```

```esptool.py --chip esp32 --port "/dev/ttyUSB0" --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size detect 0x1000 bootloader_dio_40m.bin 0x8000 partitions.bin 0xe000 boot_app0.bin 0x10000 firmware.bin```

5. If everything is okay it should start in AP mode and blink once in a second.

Change the `--port` argument according to your OS.

If you have error "can't open device "/dev/ttyUSB0": Permission denied" follow Link https://qna.habr.com/q/526674


