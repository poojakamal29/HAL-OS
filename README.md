# HAL-OS

## Project Information

This operating system was developed with a team of 3 others over the course of a matter of months for ECE391 at
the University of Illinois at Urbana-Champaign (UIUC). HAL-OS is a multitasking, protected mode, x86 operating
system modeled after the linux kernel. It features:

- basic in-memory filesystem
- user / kernel privelege seperations
- user programs
- multitasking
- memory paging / virtualization
- X-mode video
- pre-emptive scheduling
- RTC, PS2 keyboard support
- multiple active shells
- system call support
- colors! (perhaps not the most important feature...)

## See the OS in action

![alt text](https://drive.google.com/uc?export=view&id=1_t4iDPaJ_WlyP60CE_7i85CKzfZvDYPe)

[Click here for a youtube video demo of various user programs on the OS](https://www.youtube.com/watch?v=FssrD9WGqA4&feature=youtu.be)*

[Click here for a youtube video demo of the multitasking features](https://www.youtube.com/watch?v=KSsCRFt93-Q&feature=youtu.be)*

*Note: different colors represent different shells in the operating system

## Understanding the files

- Linux Emulator = Files used for QEMU emulation of the operating system
- documents = documents given specifying design requirements, courtesty of ECE391 course staff. See this folder for more specific information about the operation and mechanics of the operating system
- fish = code for the "fish" user program, shown in the above demo videos. This is a user program that shows a swimming fish in ASCII text
- student-distrib = operating system source code
- syscalls = more user programs. Fish gets it's own subdirectory because it is a rather complex program

## Running the operating system yourself

For those so inclined to run this OS yourself, the OS image is under student-distrib/mp3.img. Running the OS is fairly straighforward, and it must be done in an emulator. It is ensured to be working on Qemu, but other emulators may work as well. To run it:

1. download /student-distrib/mp3.img to your own pc. This is the image file that contains the entire operating system.
2. download "/Linux Emulator/test_nodebug_mp3.bat"
3. modify test_nodebug_mp3.bat to point towards where mp3.img is stored on your own PC
4. run the batch file, and qemu should boot up and begin running the operating system. Note that we use GRUB to boot the OS.

If on boot you happen to receive a "general protection" fault, this either means that you are running on real hardware (not all hardware runs the OS correctly) or I have uploaded the wrong image. If you get this error while running in an emulator, please contact me at the provided email.
