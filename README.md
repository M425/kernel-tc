kernel-tc
=========

Porting of tc (traffic control) tool in the Linux Kernel

#Description

the module is written for Kernel version 3.4.6
the main module is my_tc.
The flexible and dynamic behavior of tc is still running in this kernel version. Indeed each scheduler, queue ecc... is compiled as a separeted module, and is loaded on demand.
The mtc_launcher is just a module which tests that everything work well.
This has actually been my bachelor thesis work. If you need further explainations I can forward you my thesis (PDF).



##Installation
The modules are build on top of FLAVIA Framework (http://www.ict-flavia.eu/). So is required to install the FLAVIA Framework patch of the Linux kernel in order to run the module(s).
The files in the root are actually another (remember FLAVIA) patch for mac80211 module. I needed few hooks in order to correctly carry out the task.
Makefiles should do the compiling job.
