## Project Number/Title 

* Author: Jake Beslanowitch
* Class: CS452/CS552 [Operating Systems] Section #1

## Overview

Within this project, we create a thread-safe version of an existing list library that we test againest the producers-consumers problem.

## Manifest

ThreadsafeBoundedList.c - source file that implements all functions for ThreadsafeBoundedList.c 
ThreadsafeBoundedList.h - header file for ThreadsafeBoundedList

## Building the project

In order to run this program, the user of the program will need to implement the command "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./list/lib:./lib", within their wrapper-library in order for everything to run smoothly. 

This program can be built from the terminal within a command line with:
- make clean
- make all

To run the program:
- ./test-pc.sh <poolsize> <#items/producer> <#producers> <#consumers> <sleep interval(microsecs)>

## Features and usage

The main features that this program contains is the ability to create a monitor version from a doubly linked list library implementation. Within, the doubly linked list contains methods where we can add or remove from a certain location within the list. This provides the program with the functionally to be able to be a thread-safe version of a doubly linked list which, allows for multithreading to occur. 

## Testing

To begin testing for this project, I first waited till I was able to get to a point where I believed I was successful in my implemention and then I tested it. After some time fixing grammacial errors, I was able to get to a successful complied program that would output me some results. However, I did have to implement multiple println's throughout my code to see what values certain variables had within certain locatons of my program. To finally test my program, I tested multiple inputs from the command line/terminal to ensure that I had a working version of the threaded-safe version of the existing list library. After successfully confirming that I was recieving the correct output by checking that all producers were consumed,then I could confirm that I reached a working version of the project. 

## Known Bugs

As my program is, in it's current state there are no known bugs that I encountered that I didn't fix in the final version of my program. Some of the known bugs that I encountered while working through the project was setting up my add and remove methods. I experienced a hang in my program which, prevented my program from executing any futher and upon fixin this errors within my logic, I was able to elimate the hang my program was experiencing and I was able to get the full output from the test script that was ran. 

## Reflection and Self Assessment

To begin this program, I first started by cloning the repo... into my directory then I was able to locate the ThreadsafeBoundedList.h within the wrapper-library folder and I implemented my ThreadsafeBoundedList.c within the same folder so that I could implement the various methods for the thread-safe version. After getting my ThreadsafeBoundedList.c to a point where I believed that it was at a working version, I ended up running the test script to check what outputs I was recieving. For the longest time, I was stuck on a hanging part within my code that would put a halt to my program after it reached the second consumer and tried to continue along. After a lot of time of researching and thinking about what the best possible solution was, I came to the conclusion that I was not setting the correct flags where I needed too and that solved my problem on why my progam was hanging. Overall, I enjoyed this project and it was a challenge to implement the threaded-safe version with also implementing locks and conditnals within our code. This project provided me with a lot of information regarding mutexs and it was nice applying all the information we covered within class to a program we can see successful works.  

## Sources used

- https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread_cond_wait.html
- https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread_mutex_lock.html
- https://pubs.opengroup.org/onlinepubs/009604599/functions/pthread_mutex_destroy.html
- https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread_cond_signal.html
- https://pages.mtu.edu/~shene/NSF-3/e-Book/MUTEX/mutex-basics.html

