# Introduction to Embedded Linux

## stm32mp157c-dk1

ShawnHymel [7 part series](https://www.youtube.com/watch?v=9vsu67uMcko&list=PLEBQazB0HUyTpoJoZecRK6PpDG31Y7RPB)

The stm32mp1 workshop [14 part series](https://www.youtube.com/watch?v=yD8M_UK4AgQ&list=PLnMKNibPkDnFeV4sBarnsAtFTg7h6V-9_)

1. Buildroot
2. Yocto Project
3. Flash SD card and Boot process
4. Yocto custom system
5. Patch device tree
6. Custom applications
7. Enable networking


&nbsp;


## Part 1

[Tutorial 1](https://www.digikey.co.nz/en/maker/projects/intro-to-embedded-linux-part-1-buildroot/a73a56de62444610a2187cd9e681c3f2)

### Bare-metal programming - MCU

* Controlling smaller tasks - i.e., motors and sensors
* Low overhead
* Easy to design and make drivers

### Real-time OS (RTOS)

* Scheduling - helps control these small tasks

### Embedded Linux - MPU

* Controlling complex tasks - i.e., Camera & Computer Vision, memory management
* Large overhead
* Extremely complex to design your own SOM - use one as reference (already verified by other engineers)
  * otherwise have to write you own drivers (takes a long time and a lot more complex than MCU).
    * If you want to design your own SOM

### Build Root vs Open Wrt vs Yocto Project

* Build Root is easy to use but is less customisable than the yocto project
* The yocto project is more complex but is great for creating customisable Linux distributions for your project needs

### Development 

* Cross compile the binaries from your host machine to the stm32mp157

&nbsp;


## Part 2 - Yocto Project (Install)

[Tutorial 2](https://www.digikey.co.nz/en/maker/projects/intro-to-embedded-linux-part-2-yocto-project/2c08a1ad09d74f20b9844e566d332da4)

### Definitions

Terminology in the Yocto Project can be a little confusing. These definitions should help you along the way:

* **OpenEmbedded:** build system and community
* **The Yocto Project:** umbrella project and community
* **Metadata:** files containing information about how to build an image
* **Recipe:** file with instructions to build one or more packages
* **Layer:** directory containing grouped metadata (start with “meta-”)
* **Board support package (BSP):** layer that defines how to build for board (usually maintained by vendor)
* **Distribution:** specific implementation of Linux (kernel version, rootfs, etc.)
* **Machine:** defines the architecture, pins, buses, BSP, etc.
* **Image:** output of build process (bootable and executable Linux OS)

## Download Layers

### Poky, meta-st-stm32mp, and meta-openembedded

* Poky
  * Reference distribution that includes the metadata and useful tools for building the yocto project
  * `https://github.com/yoctoproject/poky`
  * `git clone https://github.com/yoctoproject/poky.git`
* meta-st-stm32mp
  * BSP layer for building the yocto project for the stm32mp
  * `https://github.com/STMicroelectronics/meta-st-stm32mp`
  * `git clone https://github.com/STMicroelectronics/meta-st-stm32mp.git`
* meta-openembedded
  * Open embedded core layer
  * `https://github.com/openembedded/meta-openembedded`
  * `git clone https://github.com/openembedded/meta-openembedded.git`

* The default Stm32mp157 (BSP) branch is the Dunfell branch
  * `cd poky/` then `git checkout dunfell`
  * `cd meta-st-stm32mp/` then `git checkout dunfell`
  * `cd meta-openembedded/` then `git checkout dunfell`

### Configure Build - Setup

* Create a build environment (to give us access to tools like *bitbake*). Note that the `oe-init-build-env` script will create an initial `build-mp1/` folder and automatically change directories to it.
  * `source poky/oe-init-build-env build-mp1`
  * OpenEmbedded build environment
* View layers in build-mp1
  * `bitbake-layers show-layers`
* Update the `bblayers.conf`
  * `cd ~/Projects/yocto/build-mp1/`
  * `vi conf/bblayers.conf`

### Configure Kernel

```
    cd ~/Projects/yocto/
    
    source poky/oe-init-build-env build-mp1

    bitbake -c menuconfig virtual/kernel

    # clean
    bitbake -c cleanall core-image-minimal
```

### Build Image

[avaliable images](https://docs.yoctoproject.org/ref-manual/images.html)

```
    source poky/oe-init-build-env build-mp1
    cd ~/Projects/yocto/build-mp1/

    bitbake -c cleanall custom-image
    bitbake custom-image

    ls tmp/deploy/images/stm32mp1
```


&nbsp;


## Part 3 - Flash binaries to SD card and eMMC

### Method 1 (tutorial) - manual

[Tutorial 3](https://www.digikey.co.nz/en/maker/projects/intro-to-embedded-linux-part-3-flash-sd-card/a1a20abb6b144b008310a69b79034f1b)

* The tutorial flashes the binaries to the SD card using the `fdisk` tool - the binaries are copied as such:

```
  # Create partitions as such, sdb was the disk type I used

  Partition 1: fsbl1 -- sdb1
  Partition 2: fsbl2
  Partition 3: fip
  Partition 4: bootfs
  Partition 5: rootfs

  sudo dd if=arm-trusted-firmware/tf-a-stm32mp157c-dk2-sdcard.stm32 of=/dev/sdb1
  sudo dd if=arm-trusted-firmware/tf-a-stm32mp157c-dk2-sdcard.stm32 of=/dev/sdb2
  sudo dd if=fip/fip-stm32mp157c-dk2-trusted.bin of=/dev/sdb3
  sudo dd if=st-image-bootfs-poky-stm32mp1.ext4 of=/dev/sdb4 bs=1M
  sudo dd if=core-image-minimal-stm32mp1.ext4 of=/dev/sdb5 bs=1M
```

### Method 2 (STM32_Programmer_CLI) - automatic

[STM32CubeProgrammer wiki](https://wiki.st.com/stm32mpu/wiki/STM32CubeProgrammer)

* The benifet of using this tool is that it allows us to copy the binaries to a bunch of different medias.
  * Such as eMMC
* Requirements
  * download the `STM32CubeProgrammer`
  * `./STM32_Programmer_CLI -c port=<DEVICE_PORT_LOCATION> -w [<file.tsv>]`
* Environment variable setup (per shell)
  ```
    export PATH=/home/jonathan/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/:$PATH
  ```

#### Flashing

```
  # Binary directory
  cd ~/Projects/yocto/build-mp1/tmp/deploy/images/stm32mp1/

  # Export variable
  export PATH=/home/jonathan/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/:$PATH

  # Change PD to P - look at error fix below
  nano flashlayout_custom-image/trusted/FlashLayout_sdcard_stm32mp157c-dk2-trusted.tsv

  ## Make sure the boot bits have been set
  # SD card - 000
  STM32_Programmer_CLI -c port=usb1 -w flashlayout_custom-image/trusted/FlashLayout_sdcard_stm32mp157c-dk2-trusted.tsv

  # eMMC
  STM32_Programmer_CLI -c port=usb1 -w flashlayout_custom-image/trusted/FlashLayout_emmc_stm32mp157c-odyssey-trusted.tsv
```

##### fip partition error fix

```
  Error: 
  Message from Embedded Flash Loader : fip (0x6): MMC erase failed

  ** Change Opt on row 0x06 (fip) from PD to P in the flashlayout .tsv file

  # FlashLayout_sdcard_stm32mp157c-dk2-trusted.tsv
  PD	0x06	fip	Binary	mmc0	0x00084400	fip/fip-stm32mp157c-dk2-trusted.bin
  --- to ---
  P	0x06	fip	Binary	mmc0	0x00084400	fip/fip-stm32mp157c-dk2-trusted.bin

  # Reset board
```


&nbsp;


## Part 4 - Custom Layer and image with the Yocto Project

[Tutorial 4](https://www.digikey.co.nz/en/maker/projects/intro-to-embedded-linux-part-4-create-custom-layer-and-image-in-yocto/aac0ab17e0c64ae482675abea00b328d)



&nbsp;


## Part 5 - Enabling I2C with patches

[Tutorial 5](https://www.digikey.co.nz/en/maker/projects/intro-to-embedded-linux-part-5-how-to-enable-i2c-in-the-yocto-project/6843bbf9a83c4c96888fccada1e7aedf)

* Communicating with a complex i2c & spi device is better controlled with writing driver code directly in the MCU.

* The below example shows how to make adjustments to the 'stm32mp157c-dk2.dts' file in yocto (this is for the dk2 board)
```
  cd ~/Projects/yocto/build-mp1/

  cp build-mp1/tmp/work-shared/stm32mp1/kernel-source/arch/arm/boot/dts/stm32mp157c-dk2.dts ~/Projects/yocto/meta-custom/recipes-kernel/linux/stm32mp1/0002/

  cd .../0002/
  cp ... ....orig
  git diff --no-index ...'new version' ...'.orig' > ...'.patch'
  # Build project

  // reverse-applied fix -- add -R
  git diff --no-index -R stm32mp1.c stm32mp1.c.orig > 0001-comment-out-usb-power.patch

```

&nbsp;



## Part 6 - Add a Custom Application

[Tutorial 6](https://www.digikey.com/en/maker/projects/intro-to-embedded-linux-part-6-add-custom-application-to-yocto-build/509191cec6c2418d88fd374f93ea5dda)




&nbsp;


## Populating SDK
```
  ---- 
  cd ~/Projects/yocto/
  source poky/oe-init-build-env build-mp1
  bitbake custom-image
  bitbake custom-image -c populate_sdk
  cd tmp/deploy/sdk/
  /home/jonathan/Projects/yocto/SDK/
  Y
```

* Directory to rootfs layout on the Embedded board - for flashing
  * `/home/jonathan/Projects/yocto/build-mp1/tmp/work/stm32mp1-poky-linux-gnueabi/custom-image/1.0-r0/rootfs`
## Resources

[Yocto Project documentation](https://docs.yoctoproject.org/3.1.20/)
[All-in-one 'Mega' Manual](https://docs.yoctoproject.org/singleindex.html)

