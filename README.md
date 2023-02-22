```
-O2
Team Name:EUnS
Member 1 :yunselee:euns312510@gmail.com
Using default tracefiles in ./traces/
Measuring performance with gettimeofday().

Results for mm malloc:
trace  valid  util     ops      secs   Kops
 0       yes   94%    5694  0.000038 149842
 1       yes   89%    5848  0.000038 152292
 2       yes   95%    6648  0.000046 146110
 3       yes   95%    5380  0.000036 147397
 4       yes   49%   14400  0.000069 208092
 5       yes   74%    4800  0.000061  78689
 6       yes   75%    4800  0.000067  72180
 7       yes   96%   12000  0.000074 161290
 8       yes   89%   24000  0.000112 214095
 9       yes   29%   14401  0.000098 147551
10       yes   45%   14401  0.000043 334130
11       yes   60%      12  0.000000 120000
12       yes   92%      12  0.000000 120000
Total          76%  112396  0.000682 164683

Perf index = 45 (util) + 40 (thru) = 85/100



Team Name:EUnS
Member 1 :yunselee:euns312510@gmail.com
Using default tracefiles in ./traces/
Measuring performance with gettimeofday().

Results for mm malloc:
trace  valid  util     ops      secs   Kops
 0       yes   94%    5694  0.000197  28948
 1       yes   89%    5848  0.000150  38935
 2       yes   95%    6648  0.000175  38054
 3       yes   95%    5380  0.000138  39127
 4       yes   49%   14400  0.000361  39878
 5       yes   74%    4800  0.000191  25118
 6       yes   75%    4800  0.000198  24206
 7       yes   96%   12000  0.000272  44166
 8       yes   89%   24000  0.000424  56604
 9       yes   29%   14401  0.000230  62722
10       yes   45%   14401  0.000146  98976
11       yes   60%      12  0.000000  24000
12       yes   92%      12  0.000000  30000
Total          76%  112396  0.002481  45297

Perf index = 45 (util) + 40 (thru) = 85/100
```

#####################################################################
# CS:APP Malloc Lab
# Handout files for students
#
# Copyright (c) 2002, R. Bryant and D. O'Hallaron, All rights reserved.
# May not be used, modified, or copied without permission.
#
######################################################################

***********
Main Files:
***********

mm.{c,h}	
	Your solution malloc package. mm.c is the file that you
	will be handing in, and is the only file you should modify.

mdriver.c	
	The malloc driver that tests your mm.c file

short{1,2}-bal.rep
	Two tiny tracefiles to help you get started. 

Makefile	
	Builds the driver

**********************************
Other support files for the driver
**********************************

config.h	Configures the malloc lab driver
fsecs.{c,h}	Wrapper function for the different timer packages
clock.{c,h}	Routines for accessing the Pentium and Alpha cycle counters
fcyc.{c,h}	Timer functions based on cycle counters
ftimer.{c,h}	Timer functions based on interval timers and gettimeofday()
memlib.{c,h}	Models the heap and sbrk function

*******************************
Building and running the driver
*******************************
To build the driver, type "make" to the shell.

To run the driver on a tiny test trace:

	unix> mdriver -V -f short1-bal.rep

The -V option prints out helpful tracing and summary information.

To get a list of the driver flags:

	unix> mdriver -h

