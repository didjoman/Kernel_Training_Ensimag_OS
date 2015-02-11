Kernel_Training_Ensimag_OS
=================

Building of a mini kernel :
---------------------------
The aim of this project is to explore and develop the basic features of an OS. In order to do so, we write low-level code that creates interactions between the system and the hardware.

This project contains 3 main objectives : 
- Write a pilot for a standard PC screen.
- Manage the time by using the hardware clock of the computer.
- Conception of a simple scheduling mechanism (Round Robin).


Note : 
------
This project is a course project at Ensimag. Therefore, a lot of files are provided by the teachers so that we could start quickly.

Here is a description of the files I have implemented and their goals :

  put_bytes.c/.h : enables to print things on the screen

  uptime.c/.h : used to manage the clock interruptions

  process.c/.h : defines a data struct + functions to create/ destroy a process.

  process_queue.c/.h : data structure to store the processes (FIFO)

  process_priority_queue.c/.h : data structure to store the processes (FIFO organized by priority order)

  scheduler.c/.h : preemptive scheduler (Round Robin) used to manage the processes and to equally distribute the
                  CPU time between all processes

  start.c : Initialises the scheduler & processes and launches the execution of CPU.
