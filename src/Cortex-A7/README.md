
# Imaging
 
This project directory contains 3 applications and adds on from `Imaging-V2/`:
1. CaptureImage
    * Capture Image application captures, processes, and stores an image.
2. SerialCommunication
    * Serial terminal application monitors, manages, and controls the client (stm32mp157) machine via UART4
    * Automates the Serial Communication side (Earth to satellite automatic COM)
3. CrystalDetection
    * This subproject can either run the autofocus or crystal detection algorithm   
4. `usr/`
    * This contains all the binary applications to run on the stm32mp157
      * The `make send_project` command does this automatically
* The `scripts/ground_controller.sh` script runs the above features and handles how files are stored on the stm32mp157 Linux system.  

Three Makefiles are used for `Imaging-V4/`. The main Makefile named `Makefile` controls the three other makefiles which are used for building each individual application.


&nbsp;
&nbsp;


## Application
* This application uses <u>termios</u> to communicate automatically between the server and client and modularieses it further. 
* This whole project further implements a basic *Corner edge detection* algorithm and an *autofocus* algorithm.
  * The *Corner edge detection* algorithm detects the triangular outline of the wells
    * This will be helpful with the changing between samples as if a slip from the stepper motor occurs:
        * The closed loop feedback will correct the system control
    * Autofocus algorithm
  * Application process example - when running `make program`
    * See the building and running commands section for more. 
* This captures a <u>single image</u>, processes it, looks for errors, and then sends it back to the host computer if it can. 
* The below shows the task process if an image can be correctly processed which makes use of the `scripts/Imaging.sh` script.
  * The application `SerialCommunication/` automates the following process. The below shows an example for how a Host machine can fully control the SatBus.
    * The SatBus is the Host Linux machine in this example.
    * For Ethernet not working
      ```
      #mount (get file) - enter uboot
      >> reboot
      STM32MP> ums 0 mmc 1
      Ctrl+C to exit
      STM32MP> run distro_bootcmd
      ```

1. Detect for reboot, flush device, and reset Devices
2. Detect new sample and autofocus
   * Detection   
3. Capture, process, and store Image
   * CaptureImage
4. Corner edge detection
   * Detection  
5. Config Ethernet
6. Request Image from client machine
7. Disable Ethernet
8. Exits program via timeout


&nbsp;
&nbsp;


## Contents
* `.vscode/`
  * Settings for the IDE
  * Action buttons are used to better control the project
* `CaptureImage/`
  * `build/`
    * Build object files and application
  * `inc/`
    * Source includes
  * `libs/`
    * Contains Alvium camera libraries
  * `src/`
    * Source files - Functions utalises the **drivers**, and other **includes** to create the application file.
  * **`CaptureImage.mk`**
    * Builds object files for this directory
* `Detection/`
  * `build/`
    * Build object files and application
  * `inc/`
    * Source includes
  * `src/`
    * Source files - Functions utalises the **drivers**, and other **includes** to create the application file.
  * **`Detection.mk`**
    * Builds object files for this directory
* `SerialCommunication/`
  * `build/`
    * Build object files and application
  * `inc/`
    * Source includes
  * `src/`
    * Source files - Functions utalises the **drivers**, and other **includes** to create the application file.
  * **`SerialCommunication.mk`**
    * Builds object files for this directory
* `images/`
  * Stores the resulting image
* `Imaging/`
  * `config/`
    * Camera setup enviroment GENICAM_GENTL32_PATH scripts
  * `libs/`
    * Contains Alvium camera libraries and OpenCV C++ libraries
  * `settings/`
    * Camera load settings
  * *`Application binaries`*
    * CaptureImage
    * Detection
  * **`CaptureImage.sh`**
    * Script controls the functionallity of the *CaptureImage* application on the stm32mp157 
* `scripts/`
  * Shell scripts
* `settings/`
  * Camera load settings 
* **`Makefile`**
  * Controls the whole project


&nbsp;
&nbsp;


## Application Features

#### CaptureImage Features
* `settings/camera_settings_color.xml`
  * Use the Camera in **BayerRG8** pixel format
* `settings/camera_settings.xml`
  * Use the Camera in **Mono8** pixel format
* These files were created and modicied using the VimbaViewer software
  * Read more in *Camera Settings* in the *Additonal Commands & Resources* below
* Example of use:
  1. Set `ARM` flag to `0`
       * This flag is espically useful for testing the Camera alone on your Linux host machine.
  2. `make`
  3. `CaptureImage/build/CaptureImage settings/camera_settings_color.xml`

#### Detection Features
* Example of use:
  1. Set `ARM` flag to `0`
       * This flag is espically useful for testing the detection algorithms alone.
  2. `make`
  3. `Detection/build/Detection`
c
#### SerialCommunication Features
* Logs clients serial output to a `scripts/log/monitor.log` file 
  * This replaces having to use `minicom`
    * This is still avalible via `make monitor_uart`
* `-Q`
  * Exit application
* `-L`
  * Program is locally run on the stm32mp157
* `-t`
  * Max timeout limit, example:
    * `SerialCommunication/build/SerialCommunication -t 10`
* `-E`
  * Config Ethernet
* `-B`
  * Booting
* `-D`
  * Disable Ethernet
* `-I`
  * Check Ethernet status
* `-M`
  * Use the Camera in **Mono8** pixel format
  * Greyscale (20 MB) .bmp image
* `-C`
  * Use the Camera in **BayerRG8** pixel format
  * Color (60 MB) .bmp image
* Example of use:
  1. Set `ARM` flag to `1`
  2. `make`
  3. `SerialCommunicationmunication/build/SerialCommunicationmunication -E`

#### Makefile Features
* `Makefile`
  * `ARM` flag to `0` compiles to x86_64 (Linux)
  * `ARM` flag to `1` cross-compiles to armv7 (stm32mp157)
  * `HOME` & `PATH_PROJ_DIR`
    * Change this to your home and path directory to your `STM32MPU_workspace`
  * `SSH_IPv4`
    * Ethernet IPv4 setting
  * `DEBUG`
    * Turns on or off debug mode for CaptureImage
  * `SETUP`
    * Environment setup for cross compiling to the stm32mp157


&nbsp;
&nbsp;


## Commands

#### Building and Running Commands 

* `make`
  * Builds the project
* `make clean`
  * Cleans the project
* `make program`
  * Runs the project via the `scripts/ground_controller.sh` script.
    * Uses Ethernet
  * Runs the project via the A7 terminal:
    ```
    cd /usr/bin/
    sh controller_A7.sh
    ```
  * Ethernet is used for sending and recieving files and folders to and from the stm32mp157:
    * Update software file - `make send`
      * `scp <file_to_send> root@<ip_address>:/<loc_store>`
    * Recieve image - `make receive`
      * `scp root@<ip_address>:/<file_to_send> <loc_store>`
  * UART is used for Controlling to and from the stm32mp157
  ```
       (Linux)                                    (stm32mp157)
        Host                                         Client 
   ______________      Ethernet      ____________________________________
  |              |    <-------->    |                   +                |
  |  Imaging.sh  |       UART       |  CaptureImage.sh  |  CaptureImage  |
  |______________|    <-------->    |___________________+________________|
                                      (Control script)     (Camera app)
  ```

&nbsp;

#### Communication 

* `make monitor_uart`
  * Opens a new serial monitor in `minicom`
    * Which utilises the `monitor.sh` script and logs output to a `scripts/log/monitor.log` file 
  * UART4 is used as the **main control source** 
    * Only one instance can be run at a time
  
* `make monitor_usb0`
  * Opens a new Ethernet port
  * make sure the `usb0 inet` is configured
  * Multiple ports can be running at the same time

#### Config USB Gadget Ethernet Interface
```
root@stm32mp1:~# cd /usr/bin/                                                         
root@stm32mp1:/usr/bin# sh stm32_usbotg_eth_config.sh restart                         
Stop usb gadget                                                                       
Nothing to do                                                                         
Start usb gadget                                                                      
[  109.173862] using random self ethernet address                                     
[  109.176870] using random host ethernet address                                     
[  109.223487] usb0: HOST MAC b2:36:be:6d:21:93                                       
[  109.227829] usb0: MAC 7a:3b:81:36:f6:f7                                            
[  109.230277] dwc2 49000000.usb-otg: bound driver configfs-gadget                    
[  109.453360] dwc2 49000000.usb-otg: new device is high-speed                        
root@stm32mp1:/usr/bin# [  109.581493] dwc2 49000000.usb-otg: new device is high-speed
[  109.654587] dwc2 49000000.usb-otg: new address 20                                  
[  109.680992] IPv6: ADDRCONF(NETDEV_CHANGE): usb0: link becomes ready                
```


#### Additional Commands & Resources

* **<u>Login</u>**
  * stm32mp1 login: root


* **<u>Config Remote Ethernet Connection</u>**
  
  *root@stm32mp1:/#* `ifconfig eth0 169.254.128.28 broadcast 169.254.255.255 netmask 255.255.0.0`
    * ifconfig <interface_name> <ip_address> netmask <netmask_address>
      * Read `man ifconfig` for more
      * Make sure **Link-Local Only** is set instead of **DHCP** in the IPv4 Address settings on the **Host** Machine if using a static IPv4 address

   option 2

  `make monitor_uart` - via minicom (automatically configs Ethernet for manual use)

  * Sleep and wake
   * ifconfig eth0 down
   * ifconfig eth0 up

* **<u>*Cortex-A7* Wakeup sources sleep with rtc</u>**
  * Option 1 (dk2)
    `rtcwake -m mem -s 10` (not avaliable on the odyssey)
  * Option 2 (odyssey and future REVs)
    ```
    ## resets the alarm
    echo 0 > /sys/class/rtc/rtc0/wakealarm
      
    1. runs the alarm (Wakes up from the last operating point)
    echo enabled > /sys/devices/platform/soc/40010000.serial/tty/ttySTM0/power/wakeup
    echo enabled > /sys/devices/platform/soc/40010000.serial/power/wakeup
    echo 0 > /sys/class/rtc/rtc0/wakealarm; echo +10 > /sys/class/rtc/rtc0/wakealarm; echo mem > /sys/power/state

    2. Shutdown then reboot system
    echo 0 > /sys/class/rtc/rtc0/wakealarm; echo +10 > /sys/class/rtc/rtc0/wakealarm; poweroff -n
    ```

  * Debugging
    ```
    grep -i rtc /var/log/messages  ---- checks kernal logs
    ```
  * Checking rtc timeing log
    ```
    cat /proc/driver/rtc
    ```
  * System Panic
    If this occurs don't panic, the watchdog timer has been enabled to reboot the system.

* **<u>Stop, Load, and Start *Cortex-M4*</u>**

  ```
  echo stop > /sys/class/remoteproc/remoteproc0/state
  cd /lib/firmware && echo <name_of_mcu_elf_file> > /sys/class/remoteproc/remoteproc0/firmware
  echo start > /sys/class/remoteproc/remoteproc0/state	
  ```

* **<u>Stalls and Resets</u>**
  * If UART4 stalls - use the command `reset` or unplug host computer cable
    * In the actual application this will sort itself out via...
  * A power cycle will happen daily
  * If a system reset occurs unexpectadly, need to be able to enter the last given task.
  * Error example fix
    ```
    STM32MP> <command>
    STM32MP> reset
    ```
  * If a `Segmentation fault (core dumped)` or any memory error occurs on the stm32mp157
    * Reboot system

* **<u>Resource busy fix</u>**
  
  >> minicom: cannot open /dev/ttyACM0: Device or resource busy
  
  `fuser -k /dev/ttyACM0`
    * This kills the hanging process

## STM32MP15 Application Notes
https://wiki.st.com/stm32mpu/wiki/STM32MP15_resources#AN5109 

&nbsp;

# Camera

* **<u>First time Camera setup</u>**
  1. Export the <u>USB TL</u> GENICAM_GENTL32_PATH or GENICAM_GENTL64_PATH 
      ```
      x86_64 - Linux
      -----
      cd Software/libs/Vimba_6_0-x86_64/VimbaUSBTL
      sh Install.sh
      reboot

      armv7 - stm32mp157
      -----
      cd /usr/etc/config/
      sh install.sh
      reboot

      # (note) Make sure the following directories were created:
      PROFILE_FOLDER=/etc/profile.d
      UDEV_FOLDER=/etc/udev/rules.d

      ```
  2. Reboot computer - Only have to do this once
  * Further information about the Camera can be found in `Software/libs/`
    * `Vimba_4_0-armv7` for stm32mp157 development
    * `Vimba_6_0-x86_64` for Linux development

* **<u>Camera Usage</u>**
  ```
    mkdir /usr/etc/images/tmp/
    gpioset gpiochip3 4=1; sleep 5
    cd /usr/bin && sh capture_image.sh -C && sh capture_image.sh -M
    gpioset gpiochip3 4=0
  ```

* **<u>Camera settings</u>**
  * Camera `.xml` settings can be created by using the `VimbaViewer` on your machine.
  * Linux host machine example:
    ```
    cd Software/libs/Vimba_6_0-x86_64/Tools/Viewer/Bin/x86_64bit
    ./VimbaViewer or click
    ```
  * <u>Throughput:</u>
    * A lower `Device Link Throuput Limit` results in a brighter image
    * Camera is set at 25 MB
    * USB2 has a throughput limit of 60 MB
    * RGB8 requires USB3 speeds because a full scale image (5496 x 3672) requires a minimum throughput of 70 MB
    * The Armv7 32-bit ARM Coretex-A7 can only handle USB2 speeds not USB3
      * Would need to choose an Armv8 processor that can handle USB3 speeds
      * Armv7 cannot handle these speeds 
    * <u>BayerRG8 & Mono8</u> requires a minimum throughput of 24 MB 
      * Which USB2 can handle comfortably


# Power states

### State 1 - A7 + M4 Processing

* MPU (ON) MCU (ON/OFF) - after login & idle (Ethernet disabled)
  * ~680mW (~136mA)

### State 2 - Sleeping

* MPU (SLEEP) MCU (OFF) (A)
  * ~30mW (~6mA)
  * System reboots when finished sleeping
  ```
  echo 0 > /sys/class/rtc/rtc0/wakealarm; echo +20 > /sys/class/rtc/rtc0/wakealarm; poweroff -n
  ```
* MPU (SLEEP) MCU (OFF) (B)
  * ~30mW (~6mA)
  * System resumes when finished sleeping
  ```
    echo enabled > /sys/devices/platform/soc/40010000.serial/tty/ttySTM0/power/wakeup
    echo enabled > /sys/devices/platform/soc/40010000.serial/power/wakeup
    echo 0 > /sys/class/rtc/rtc0/wakealarm; echo +20 > /sys/class/rtc/rtc0/wakealarm; echo mem > /sys/power/state
  ```

### State 3 - M4 Processing 

* MPU (SLEEP) MCU (ON)
  * ~110mW (~22mA)
  ```
  cd /usr/bin/
  sh controller_M4.sh gpio.elf

  echo 0 > /sys/class/rtc/rtc0/wakealarm; echo +20 > /sys/class/rtc/rtc0/wakealarm && echo mem > /sys/power/state
  ```

### State 4 - M4 Processing to Sleep

* MPU (SLEEP) MCU (ON -> SLEEP)
  * ~ 110mW -> ~30mW (~22mA -> ~6mA)
  ```
  cd /usr/bin/
  sh controller_M4.sh PWR_STOP_CoPro_CM4.elf
  echo 0 > /sys/class/rtc/rtc0/wakealarm; 
  echo +20 > /sys/class/rtc/rtc0/wakealarm && echo mem > /sys/power/state
  ```

### Summary

Save More than 84% in power consumption when using the M4 over the A7 environmental monitoring tasks (i.e., while the A7 is sleeping)
* With proper Kernal configuration, the PMIC should be able to be controlled by Vbat - this feature has not yet been implemented in the kernel 

