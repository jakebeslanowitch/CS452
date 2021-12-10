# Project 4 - Memory Manager (CS453: Operating systems) 

* Author: Jake Beslanowitch
* Class: CS452/CS552 [Operating Systems] Section #1

## Overview


## Manifest

- Buddy.c
	- Implements memory manager for buddy-preload
- Buddy.c
	- Implements memory manager for buddy-non-preload

## Building the project

In order to run this program, the user of the program will need to implement the command:

"export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./libbuddy.so"

## Features and usage

The main features that are present within this project are the abilties to give users of the program a chance to use malloc, calloc, realloc, free, etc.

This program can be built from the terminal within a command line with:
- make clean
- make all

Running directions for the project are in testing, in which
we allocated memory and free memory. 

## Testing
Contains two performance test files: buddy-test.c and malloc-test.c that run identical tests using the two different allocators. You should not modify these two files! In addition to using printf statements to track different errors, there is a unit test file buddy-unit-test.c that includes test cases that I used to test with my project. Another huge thing that I did with this project that was not supported in the last project because of working in the kernel space was getting to use GDB!!!! Life safer when it comes to debugging and fixing mistakes fast. Although, the printf statement debugging is effective, it is a lot slower than using GDB. Overall, I had a lot of useful resources to make debugging easier with this project. However, I defintely needed it because I have never in my life experienced so many seg faults while working on one project haha. 

Interposing allowed us to add our library in as a shim. Then, when a user calls malloc/free/realloc/etc. my versions are called instead of the C standard library versions.  To implement interposing, my buddy system allocator needed to implement the same interface as malloc (and with the same function names and signature).

Use the following command to interpose for malloc using libbuddy.so:

```console
    LD_PRELOAD=./libbuddy.so ./mydash
```

Note that to time the interposed version, use the following command:

```console
time LD_PRELOAD=./libbuddy.so ./malloc-test <appropriate arguments>
```
Addtional Testing Cases:
	Use path to addtional test cases. 
		$ ./buddy-test 
		$ ./buddy-unit-test [print out option]
		$ ./malloc-test 

## Known Bugs

The main known bug that is present within my program occurs sometimes when I try to run my malloc vs the mytests present within my mergesort program. The output will sometimes result in me segmentation faulting. This bug was a part of the interposing aspect of the project, however, not everytime when I ran mytests would I be left with an output of segmentation fault. 


## Extra Credit 

### Buddy System Performance
Test your buddy system implementation against malloc and make sure that it outperforms it. For the purposes of measuring performance, use the buddy-test.c and malloc-test.c code provided in the sample code for this project. Here is performance comparison for the reference solution (tested on onyx, compiled with -O2 optimizer flag)

```console
[user@onyx buddy-non-preload]$ time buddy-test 20000000 1234 s
real    0m1.348s
user    0m1.345s
sys     0m0.002s

[user@onyx buddy-non-preload]$ time malloc-test 20000000 1234 s      
real    0m2.090s
user    0m2.088s
sys     0m0.001s
```
After running my program againest the buddy-test.c and malloc-test.c, comparing the provided onyx/console malloc output and when I run both of these time tests I get a quicker/faster speed than regular. 

// malloc
[jakebeslanowitch@onyxnode61 buddy-preload]$ time malloc-test 20000000 1234 s

real	0m1.646s
user	0m1.642s
sys	    0m0.000s

// buddy
[jakebeslanowitch@onyxnode61 buddy-non-preload]$ time buddy-test 20000000 1234 s

real	0m1.142s
user	0m1.053s
sys	0m0.032s


## Reflection and Self Assessment

Discuss the issues you encountered during development and testing. What
problems did you have? What did you have to research and learn on your
own? What kinds of errors did you get? How did you fix them?

What parts of the project did you find challenging? Is there anything that
finally "clicked" for you in the process of working on this project? How well
did the development and testing process go for you?

## Sources used

- https://fileadmin.cs.lth.se/cs/education/EDA050/F05.pdf
- http://homepage.divms.uiowa.edu/~jones/opsys/notes/27.shtml
- http://cs.boisestate.edu/~amit/teaching/552/handouts/memory-management-handout.pdf
- https://www.lix.polytechnique.fr/~liberti/public/computing/prog/c/C/FUNCTIONS/format.html
- https://www.geeksforgeeks.org/buddy-memory-allocation-program-set-1-allocation/
- http://www.qnx.com/developers/docs/6.5.0/index.jsp?topic=%2Fcom.qnx.doc.neutrino_lib_ref%2Fs%2Fsbrk.html
