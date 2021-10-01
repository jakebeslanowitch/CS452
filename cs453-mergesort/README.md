## Project Number/Title 

* Author: Jake Beslanowitch
* Class: CS452/CS552 [Operating Systems] Section #1

## Overview

Within this project, I implemented a multithreaded version of the mergesort algorithm that uses the pthread library and takes in an array of integers and sorts them according to the algorithm. Following the completion of the program, the output will contain the time (in seconds) that it took for serial mergesort and parallel mergesort to finish sorting the given array of integers. 

## Manifest
- lab.c
    - Source file that implements all of the sort functions and helper functions
- lab.h
    - This is header file, which is used to define function protypes and my struct arg_threads 
- mytests.c
    - Driver class that takes in command line arguments and outputs the result  from parellel mergesort and serial mergersort 
- makefile
    - File for compling/building the project
- runval.sh
    - A provided script which will run valgrind to check the program for memory leaks

## Building the project

This program is accessible to run, and the only requirement is that the machine that is trying to execute the program needs to have avaible and permissions to run gcc. After verification that gcc is present on the machine. A simple make clean and make/make all can be ran to complie and build the program to be ready for the user to interact with it. 

## Features and usage

The main features that this program contains are two different sorting algorithms, which are serial mergesort and parallel mergesort. Also, as stated in the Overview section, this project is to be ran with command line arguments. There are a total of 2 required arguments with a third argument that is optional which, represents the seed desired. Users of this program will only need to input integers into the command line arguments and no other types. 

## Testing

To begin my testing for this project, I first tried to compile/build the project with gcc and make. GCC was super helpful for finding syntax errors for me that were present within my project. After I got my project complied and running, I was able to fix a bunch of errors within my logic with multiple print statements which allowed me to see the value of certain variables at specfic places in my code. To finally do my last testing on this project, I tested multiple different inputs into the command line arguments to ensure that different integer values would still work within the program and not crash it. Also, at the end of my testing, the script runval.sh which runs valgrind, checking for memory leaks was also super successful in helping me ensure my program was free of any errors and memory leaks.

## Known Bugs

As my program is, in it's current state there are no known bugs that I came across and didn't have time to fix. However, there were a lot of bugs that I had to fix in order to get my program up and running. Another large bug that I had for a long time was the error "segmentation fault", which was throwing me for a loop for a while before I realized that within my check for my base case, I was not returning NULL, which forced my program to throw me the segmentation fault error. Something that was interseting to me and I thought was a bug was when I executed valgrind within my project with a super large array size, it gave me weird errors stating that I hit the max number of threads but when I spoke with the professor about this, he stated that it wasn't an issue within my program, however, I just passing too big of an array to the runval.sh script. 

## Reflection and Self Assessment

Throughout this project, I had a lot strengths but I also had some weaknesses that I encountered while trying to come to a working multithread version of mergesort. One of my main issues that I encountered was when I first started using my struct within my lab.c file, I failed to realize that each time I wanted to call my struct and assign a member of the struct to it, I needed to be casting a pointer to the thread followed by the member. However, after some time and doing some research, I realized that a pointer was needed each time that I called my struct variable, which in my instance was the variable thread. Another small issue I encountered was getting my recursion to work properly when I called pthread_create and this was because I needed to add a loop where we create the thread based on the index. Some of the strengths that I had within this project was having a good previous understanding of how mergesort performed it's sort, which helped me a lot prior to even writing any code. Another one of my strengths that I had within this project was working with pointers. Before we started this project, I made sure to go through most of my CS253 programming assignments to make sure that I still had a good fundmental understanding of pointers and this helped me tremedsouly with this project because pointers within C can get a little confusing to me sometimes but going back to review and also trying to write some addtional examples helped me a lot with this project. This project was definilty very challenging at times, however, doing some outside research, reviewing on some prior C material, and refreshing myself on C helped me be able to work through this project with no major issues. 

## Sources used

I used this website to get some addtional examples of different sorts.  

https://www.cs.iusb.edu/~danav/teach/b424/b424_10_mergesort.html
