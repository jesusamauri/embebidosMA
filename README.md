Laboratorio de Sistemas embebidos 2015  by Jesus und Martin 
This is a collection of laboratories made with a raspberryPi
----------------------------------------------------------
------------------ Tecnológico de Monterrey ---------------

School of Engineering and Information Technology
Department of Electrical and Computer Engineering
Embedded Systems Laboratory
Laboratory: Introduction to embedded development under linux
----------------------------------------------------------
Objective
Students will be capable of writing a simple structure of a Makefile.
Students will run the Blink project on the Raspberry Pi on Bare Metal while
testing their own Makefile.

Activity 1. Download and install.

1. First of all, we need to make sure that your Ubuntu version is fully updated with
the essential drivers and libraries. Open a Terminal and enter the following
command:
$ sudo apt-get update
[Enter the admin password if needed]

2. If your Ubuntu version is 32bits, you may skip this step. The ARM compiler
needed for this project requires a 32 bit library configuration. To install this patch
go back to the Terminal and enter the following command:
$ sudo apt-get install ia32-libs
[Enter the admin password if needed]

3. Time to download the toolchain. Enter the next command:
$ sudo add-apt-repository ppa:terry.guo/gcc-arm-embedded
[Enter the admin password if needed]
...
Press [ENTER] to continue and wait a couple of seconds.

4. Once again, update your Ubuntu so that we obtain the latest data form the
repository:
$ sudo apt-get update
[Enter the admin password if needed]
This might take 1 to 5 minutes.

5. And finally:
$ sudo apt-get install gcc-arm-none-eabi
[Enter the admin password if needed]
This might take 1 to 5 minutes.6. Go to the directory “/usr/bin” and search for a file named arm-none-eabi-gcc.

7. Now, is time to test the toolchain. From Terminal, insert the next command to go
to a subfolder inside the complier:
$ cd /usr/bin
You can type the command ls to see all the different compilation tools that this
package has. For the time being, we will just use the arm-none-eabi-gcc and
arm-none-eabi-objcopy. Test the compiler by writing the next command:
$ ./arm-none-eabi-gcc
You should get the following message:
This is a good sign.

5. ALTERNATIVE. To avoid using long paths, we can add the location of the
programs to the Environmental variables of our Linux System. Like this:
$ export PATH=$PATH:/usr/bin
