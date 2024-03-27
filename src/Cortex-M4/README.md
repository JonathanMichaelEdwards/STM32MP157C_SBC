# Dual UV & White Lighting driver

* STM32MP157CA Cortex M4 example with ADC1 & ADC2, GPIOs, and DAC

## Contents
* `.vscode/`
  * Settings for the IDE
* `build/`
  * Build object files
* `drivers/`
  * BSP  - Board support package
  * CMSIS - Content Management Interoperability Services
  * HAL  - Hardware abstraction layer
* `libs/`
  * 3rd party sources
* `inc/`
  * Source includes
* `src/`
  * Source files
* `scripts/`
  * Shell scripts for testing


## Build & Run

1. Build the project
   * `make clean && make -j 8`
2. Run the application
   * `make program`
  

### Manual Run 

1. Load the .elf file using uboots ums USB PROG
  ```
  >> reboot
  STM32MP> ums 0 mmc 1
  sudo cp Control_Monitor.elf /media/jonathan/rootfs/usr/bin/
  sudo cp controller_M4.sh /media/jonathan/rootfs/usr/bin/
  
  Ctrl+C to exit
  STM32MP> run distro_bootcmd
  ```

1. Enable and wakeup sources so M4 i2c lines can run while the A7 is sleeping
   * Data printed over UART4 (USB SERIAL)
   * PHT data isn't printed over UART4 when A7 sleeps - this will be stored on the EEPROM
   * Enable shared I2C2 line power wakeup
2. A7 Sleeps while M4 is Up
   * A7 resumes when rtc counter is 0
3. LEDs are disabled
   * A7 is UP

  ```
  cd /usr/bin/; sh controller_M4.sh Control_Monitor.elf
  or
  cd /usr/bin/; sh controller_M4.sh stop
  ```

## Config USB Gadget Ethernet Interface
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
  
