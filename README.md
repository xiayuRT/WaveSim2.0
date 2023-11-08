# WaveSim 2.0
Firmware code and UI code developed for WaveSim 2.0.

## Folder Structure
```
├── GUI
│   ├── src
│   ├── inc
│   ├── resource
├── CLI
│   ├── src
│   ├── Makefile
├── common
│   ├── src
│   ├── inc
├── external
│   ├── inc-pub
├── config
└── .gitignore
	-
```

# Raspberry-PI Programming Environment Building

## 1. Device Info
#### Name: Raspberry-Pi 3B+
#### Storage: 32GB
#### OS:  
```
PRETTY_NAME="Raspbian GNU/Linux 11 (bullseye)"
NAME="Raspbian GNU/Linux"
VERSION_ID="11"
VERSION="11 (bullseye)"
VERSION_CODENAME=bullseye
ID=raspbian
ID_LIKE=debian
HOME_URL="http://www.raspbian.org/"
SUPPORT_URL="http://www.raspbian.org/RaspbianForums"
BUG_REPORT_URL="http://www.raspbian.org/RaspbianBugs"
```

## 2. Teknic Linux Software (sFoundation + SC4-Hub USB Driver) 
### Preparation Work:
Download the Linux_Software file: https://teknic.com/files/downloads/Linux_Software.tar.gz

Description: this file is required to run ClearPath-SC motors on a Linux computer. Contains the sFoundation source, example files, and a module for accessing the SC4-HUB via USB. Also contains instructions for how to use the files.

## A. sFoundation:
This is the libraries in C++ to control the Clear Path motor (base motor in wave simulator).

#### 0. Before You Start...
(All the softwares below are pre-installed on Pi if you choose the full version OS.)
* Make sure the required software is installed on Pi:
    * tar (to extract package contents)
    tested: 1.32, 1.3
    * make (to build sFoundation)
    tested: 4.3, 4.2.1
    * g++ (to build sFoundation; must support C++ 11)
    tested: 10.2.1, 9.3.0, 8.3.0
    * ldconfig (for installing sFoundation)
    tested: 2.32, 2.31, 2.28

* Make sure you have permission:
    * To install sFoundation shared library for system use, you will need write
   access to /usr/local/lib
    * To install the SC4-Hub USB driver, you will need root access

#### 1. Serial Port Permissions
* Add your user to the dialout group:
```
$ sudo usermod -aG dialout <your_username>
```
* Make sure your user is now in the dialout group
```
$ groups
```
yourUsername adm dialout sudo cdrom ...

Note: may reboot to make this neew permission take effect

#### 2. Build sFoundation Shared Library
* Extract the sFoundation source
```
$ tar -xvf sFoundation.tar
```
* Navigate to the sFoundation directory
```
$ cd sFoundation
```
* Run make. This will build libsFoundation20.so in the current directory
```
$ make
```

#### 3. Install sFoundation Share Library
* Copy MNuserDriver.xml and libsFoundation20.so to /usr/local/lib
```
$ sudo cp {MNuserDriver20.xml,libsFoundation20.so} /usr/local/lib
```
* Run ldconfig to set up the proper symbolic links and add the sFoundation library to the linker search path
```
$ sudo ldconfig
```
* For sanity, verify that the system detected the new shared library
```
$ ldconfig -p | grep "sFoundation"
```
The expected output would be something like this:
  libsFoundation20.so.1 (libc6,x86-64) => /usr/local/lib/libsFoundation20.so.1

> Note: ldconfig is typically in /usr/sbin/ldconfig. On some distributions,
  including Debian, /usr/sbin/ is not in the default search path for
  standard users. You might need to specify the full path.
  $ /usr/sbin/ldconfig -p | grep "sFoundation"

> Note: If the above command has no output, ldconfig might not be configured
to search /usr/local/lib by default. This seemed to be the case in Fedora 33.
1. Make sure /usr/local/lib is indeed absent from the ld search 
   path. You should get no output.
   $ grep -re "/usr/local/lib" /etc/ld.so.c*
   > Note: If you did get output, make sure the appropriate files
     are included in /etc/ld.so.conf
2. Make sure /usr/local/lib is explicitly listed or included in 
   /etc/ld.so.conf. You could create a new file in an included 
   directory under /etc/ld.so.conf.d/, or simply append to 
   /etc/ld.so.conf.
     $ echo "/usr/local/lib" | sudo tee -a /etc/ld.so.conf
3. Try running sudo ldconfig again, then ldconfig -p. You should get
   output this time.

### B. SC4-Hub USB Driver:
This need to be installed to make the Raspberry-Pi work with SC4-Hub through seriel communication.

#### 1. Make sure the kernel headers are installed on your system.
Raspberry Pi: Headers are typically stored in /usr/src/linux-headers-{kernelVersion}.
See if they exist using ls.
```
$ ls /usr/src/linux-headers-$(uname -r)
```
If they don't exist. install the raspberrypi-kernel-headers package using apt:
```
 $ sudo apt install raspberrypi-kernel-headers
```

#### 2. Install the driver.
* Extract the tarball and change to the ExarKernelDriver directory.
```
$ tar -xvf Teknic_SC4Hub_USB_Driver.tar
$ cd ExarKernelDriver
```
* Make sure Install_DRV_SCRIPT.sh is an executable
```
 $ chmod +x Install_DRV_SCRIPT.sh
```
* Enter Root Terminal and Run the Driver Install script
```
$ sudo su
$ ./Install_DRV_SCRIPT.sh
```
* If it cannot be installed successfully, follow the instructions below.

#### 3. *Debugging (Important!!!)
Try with this modification or contact teknic service for the newest driver.
* Changes on the lines: 1650, 1671, 1702

Replace:
```
ASYNCB_INITIALIZED
```
with 
```
tty_port_initialized
```

* Insert the function to line 1783:
```
static inline struct tty_driver *alloc_tty_driver(unsigned int lines)
{
	struct tty_driver *ret=tty_alloc_driver(lines, NULL);
	if (IS_ERR(ret))
		return NULL;
	return ret;
}
```

## 3. PIGPIO Library
Ref: https://abyz.me.uk/rpi/pigpio/download.html
### Download and install latest version
```
wget https://github.com/joan2937/pigpio/archive/master.zip
unzip master.zip
cd pigpio-master
make
sudo make install
```
### Link in g++/gcc
```
-lpigpio
```
* Note: Use 'sudo' to run the program.

## 4. QT5
#### Update the system
```
sudo apt update
```
#### Install the QT creator
```
sudo apt-get install qtcreator
```
#### Install necessary develop tools
```
sudo apt-get install qtbase5-dev qtchooser qt5-qmake qtbases5-dev-tools qtdeclarative5-dev
```

## 5. Pi Boot to Run the Program
#### Open the crontab
```
sudo crontab -e
```
#### Add the command want to run on boot at the end of file
```
@reboot sudo startx 'path to executable file'
```
Note: 'startx' is used to run the GUI.
