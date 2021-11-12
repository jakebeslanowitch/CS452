## Project 3/Booga Device Driver 

* Author: Jake Beslanowitch
* Class: CS452 [Operating Systems] Section #1

## Overview

Within this project, I implemented a simple character driver that supports the open, read, write, 
and closed operations. The driver supports four minor numbers 0, 1, 2, 3. On the reading from 
/dev/booga0, /dev/booga1, /dev/booga2, and /dev/booga3 the driver gives a stream of one of the 
following phrases.

- booga! booga!
- googoo! gaagaa!
- neka! maka!
- wooga! wooga!

However, if a device tries to write to /dev/booga3, it should be terminated. 

## Manifest

- booga_load - Loads the booga device driver
- booga_unload - Unloads the booga device driver
- booga.c - main driver class for this program
- booga.h - header file for booga.c
- MakeFile - Builds the project
- booga-test1.sh - Tests booga drivers
- booga-test2.sh - Tests booga drivers
- test-booga.c - Tests booga drivers

## Building the project

In order to build this project, we run:

$ make clean
$ make all

After we have built the project, then we need to load a device manager with:

$ sudo ./booga_load

Once completed running and testing the program:

$ sudo ./booga_unload
$ make clean

## Features and usage

In order to successful run this program, the user should use ./test-booga.c read|write. The two main functionalies of this program are 
the read and write operations. On the read method, a random phrase is made that the device reads in and on the write method, we write to dev/null unless the option /dev/booga3 is choosen which, should return a termination. 


## Testing

For the testing aspect of this program, I did not do a lot of testing other than using the provided test files
within the project. Since we were working within a kernel space, I wasn't able to use the debugger as if we were using/in the user space
like most of my projects are usually. Most of the errors that I encountered while trying to test/run the program was within my read method, and most of the errors that I faced were syntax errors but I also experienced a hanging issue and seg faults along the procress of trying to successful get the program to work properly. 

## Known Bugs

No known bugs that are present within my program.

## Reflection and Self Assessment

Discuss the issues you encountered during development and testing. What
problems did you have? What did you have to research and learn on your
own? What kinds of errors did you get? How did you fix them?

What parts of the project did you find challenging? Is there anything that
finally "clicked" for you in the process of working on this project? How well
did the development and testing process go for you?

Overall, this was a very challenging project and it took me a long time to get more knowledge on 
working within the kernel space. After some time researching, I became more familar with the kernel
space and semaphores.The part of the project that I found most challenging was the read operation 
and this is because I first had to do a lot of prior research to get to a point where I was ready 
to start writing code and just in general, I feel like the I spent a lot more time on the read operation.
I also encountered a bunch of errors within the read method which spanned anywhere from syntax errors 
to kernel crashes. I felt like the open and write methods were a little easier but they still offered good 
challenges. Another big issue that I ran into was using the get_random_bytes function and after failing 
some test runs I was able to get my get_random_bytes function working properly by changing where I was calling it. 
The last major problem that I encountered was I was experiencing a hang within my program and I was stuck on 
this issue for a long time until I went to the tutoring hours and I realized tnat I was using my semaphores in the 
wrong place and after I fixed that, my program complied and ran as expected without any bugs.

## Sources used

https://www.softprayog.in/programming/posix-semaphores
https://lwn.net/Kernel/LDD3/


