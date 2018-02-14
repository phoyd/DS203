﻿Notes for this fork
===================

This fork builds for old hardware revisions (2.70) and a Ubuntuish distro.

### Build Requirements

Building and installing the image requires a arm-none-eabi-gcc toolchain and mtools. For a recent Ubuntu/Mint/Debian that's simply:

```
apt-get install arm-none-eabi-gcc binutils-arm-none-eabi libnewlib-arm-none-eabi mtools
```

### Building

Enter `Build`. Run `make SLOT=3` (or simply `make`, SLOT=3 is the default). This results in a file named `dso_3.hex` which can be installed to slot 3.

### Installing

Installing to the DSO Quad involves booting up the device with the "play" button pressed and attached to a computer. 
This should put the thing in to Device Firmware Upgrade mode (DFU). In DFU mode, the thing acts as a 512 kb virtual flash disk, which monitors all writes to it and installs the files written to it into the internal flash. After that, the files file uploaded will be renamed 
*by the dso* to indicate a succesful write (or an error).

That is obviously a horrible idea and it doesn't really work, because: 

* There is no way to indicate that writing is completed.
* The flash disk emulator modifies the file system *while being mounted*.
* caching

Eventually, updating the firmware by writing to the disk never really worked for me (not even with mount -o sync). 
So I'm using mtools now (https://www.gnu.org/software/mtools/) to access the flash disk, because they seem are userspace and are much less trouble right now. 
They are usually part of the default install of Ubuntu/Debian/Mint, if not `apt-get mtools` will pull them. 

mtools operates on drive letters, so you will need to declare a drive letter for the DFU disk. If you have "disk-by-id" devices, then it's simple as that:

```
$ more ~/.mtoolsrc 
drive s: file="/dev/disk/by-id/usb-Vertual_DFU_Disk-0:0" exclusive
```

`scripts/copy.sh <hex-file>` will figure the right drive letter out and copy the file to your DSO Quad. If anything goes well the hex file will be listed with a ".rdy" suffix or ".err" if the upload broke somehow. "broke somehow" seems to be unreliably reported, you'd need to check if your image boots anyway.
 
### Changes

* Changed the default font to Terminus (which is much better to read IMHO)
* Build/mingw_win32 contains a makefile for the  i686-w64-mingw32-gcc toolchain that comes with Ubuntu. It builds the Windows GUI Simulator, which can be run unter Wine.

### Screenshot

![Terminus](Man/terminus.png)

---

DS203 customized firmware
======================

DS203 is a nice open source oscilloscope. And this is an attempt to design improved GUI for the scope offering rich functions, better than those provided by original not-so-user-friendly firmware. The source code is in C++ for better code organisation and it is possible to compile it on Windows and Linux machine.  For compiling the firmware for ARM Cortex M3 target device, there is a script in "Build" folder (arm_win32 for win32 platform, arm_linux for linux platform)

### For owners of DS203 who would like to try this firmware:

1. download hex file from Bin/GABOUI_1.hex (the number specifies in which slot the app will be loaded)
2. power on your oscilloscope while holding the first button
3. on device's display, there should be displayed "DFU tool" and a new USB disk should appear in "My computer"
4. copy the hex file to your oscilloscope
5. after successful upload, on the USB disk will disconnect itself and reconnect again. Sometimes it will show some error message during uploading (don't worry, the computer has probably uploaded the file successfully, there is probably some bug in the DFU program) 
6. turn off
7. turn on without holding any button

### For developers that want to build the firmware, but don't want to use github:

1. download whole repository by clicking on "Download ZIP" button on right
2. and run the build script Build/arm_win32/_makefile.bat

For building this project you need ARM GCC toolchain. The building script will automatically download one for you, if you want to use your own toolchain, just modify the CBASE value in makefile.bat. The official download link for the toolchain is https://sourcery.mentor.com/sgpp/lite/arm/portal/package8735/public/arm-none-eabi/arm-2011.03-42-arm-none-eabi-i686-mingw32.tar.bz2 it is about 36MB. If the link will be dead or you will have troubles unpacking tar.bz2, you can download stripped version of that toolchain (13MB) from my webpage http://pub.valky.eu/arm-2011.03-lite.zip

For developers that would like to contribute to this project:
Just contact me, or make a modification of the source code and push me a request.

#### Building Issues

Compiling with the version 4.8 toolchain from the "GNU Tools for ARM Embedded Processors"  (https://launchpad.net/gcc-arm-embedded) project yields broken binaries which hardfault on boot. The 4.6-2012-q4-update release works however.

### Simulator Builds

For speeding up the development process, you can compile this application as native Win32 app, this offers you the ability to test new features on PC before compiling and uploading the firmware onto DS203 device. For windows users, there is a Visual Studio 2010 project file (Build/desktop_win32). The native App can be also be cross compiled under Linux using the makefile in Build/mingw32. The resulting executable can be run and tested under Wine. 

Controls:
======================
The DS203 device has 4 push buttons and two slide buttons. The function of first three buttons is following:

- PlayPause button - Enter
- Square button - Escape
- Circle button - Show toolbox
- Triagle button - User assignable button
- The two slider buttons are used for navigation and value change.

Screenshots:
======================
![Image 1](Man/Screenshot/uart_decoder_1.png)

![Image 2](Man/Screenshot/midi.png)

![Image 3](Man/Screenshot/cursor.png)
           
![Image 4](Man/Screenshot/waveman_meas.png)

![Image 5](Man/Screenshot/spectrum.png)

![Image 6](Man/Screenshot/about.png)

![Image 7](Man/Screenshot/calibration.png)

![Image 12](Man/Screenshot/envelope.png)

![Image 8](Man/Screenshot/svgexport.png)

![Image 9](Man/Screenshot/wave_export_ac.png)

![Image 10](Man/Screenshot/tuner.png)

![Image 11](Man/Screenshot/android.jpg)

![Image 12](Man/Screenshot/pid1.png)

![Image 13](Man/Screenshot/pid3.png)

![Image 14](Man/Screenshot/snake.png)

![Image 15](Man/Screenshot/sincinterpolator.png)

![Image 16](Man/Screenshot/onewire.jpg)

Feature highlights:
======================

- UART, MIDI, CANBUS decoder
- Mask tests, pass/fail calculations (create envelope of your signal and catch disturbances which exceed this envelope)
- Advanced mathematical operators (3 operands)
- User defined FIR filter (in Oscilloscope/Math)
- Cursors with lock-to-channel function
- Spectral analysis
- PID Controller
- Export to Wave (use your favourite sound editing program for signal examination)
- Export to CSV file
- Export to interactive SVG
- Chromatic tuner utility with very precise frequency counter
- Remote control with android device over UART/bluetooth (or with DsoControl application)
- Touchscreen support
- Serial port monitor with selectable baud rate
- Snake game :)
- OneWire implementation through TX pin of uart, currently compatible with DS1820, DS18S20, DS18B20

Todo list:
======================
- finish oscilloscope module - 92% done
- calibration - 80% done
- fft view - 80% done
- signal generator - 90% done
- connection with android tablet/phone - 90% done
- optimize ROM usage (many bitmaps are stored as 8bits per pixel) - 0%

Calibration:
======================

This firmware finally implements a simple method for analog channel calibration.  It's good idea to calibrate the DAC (wave out) first, it can be handy if you dont have regulated power supply. The device should be connected to a PC during calibration.
 - Calibrating Adc: 
   1. Go to User app/Calib tab, select Adc menu item
   2. Connect volt meter to Waveout
   3. Change the value (four digits) next to 0.5V label until you get exactly 0.5V on your voltmeter or multimeter.
   4. Do the same for 1.5V value
   5. Click on "Use labels", the dialog will hide
   6. Now you can check proper DAC calibration by changing the voltage shown on Adc menu item with navigation buttons, the voltage shown on display shoud match the voltage measured on the WaveOut connector within the error +-0.001V
 - Calibrating analog channel
   1. Enter "Analog-Simple" calibration dialog
   2. Select desired channel and resolution
   3. Connect the probe to selected channel and connect tip of the probe with the crocodile (ground).
   4. Select "Reset Vpos" and wait a few seconds
   5. Connect the probe to a precise power supply and by changing the K value, try to match Vin value with the real voltage of power supply. If you dont have any regulated PSU, you can connect the probe with WaveOut 
   6. Hit save and try changing the input voltage and check whether you get correct reading at the Vin value. Use a sensible voltage while matching the K value, for example - if you are calibrating 200mV range, use 4x200mV = 800mV at input.
 - After finishing, select "Save calib data"


Development:
======================

If you want to build this firmware with your own modules, follow the instructions in man/Readme.txt.


Remote control with android phone
======================

The firmware implements simple expression evaluator which can be used to control the device remotely. In Resources folder you can find android project (including .apk executable) which demonstrates this feature through bluetooth wireless connection. There is also PC application (Resources/DsoControl) that can be used as remote control by sending the mouse cursor position and keystrokes through bluetooth or UART.
Read more here: https://github.com/gabonator/DS203/tree/master/Man/SerialOutput


References:
======================

- Author's webpage - www.valky.eu
- Seeedstudio forums - http://www.seeedstudio.com/forum/viewforum.php?f=26
- DSO Quad wiki pages - http://www.seeedstudio.com/wiki/DSO_Quad

Alternative firmwares and useful resouces:
======================

 - Petteri Aimonen - http://essentialscrap.com/dsoquad/
 - Pedro Simoes - https://github.com/pmos69/dso203_gcc
 - Marco Sinatti - https://sites.google.com/site/marcosinatti/elettronica/dso

Credits and contributors:
======================
Please let me know if I forgot to mention someone who contributed to this project... 

 - krist10an
 - Frederic Borry
 - Francesco Gugliuzza
 - Florian Bauer
 - Michal Hrouda

 
