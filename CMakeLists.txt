cmake_minimum_required(VERSION 3.24)
project(SysProg C)

set(CMAKE_C_STANDARD 11)

include_directories(vorgabe/lib)
include_directories(vorgabe/tests)

add_executable(SysProg
    vorgabe/lib/colors.h
    vorgabe/lib/FCFS.h
    vorgabe/lib/HRRN.h
    vorgabe/lib/LCFSPR.h
    vorgabe/lib/MLF.h
    vorgabe/lib/PRIONP.h
    vorgabe/lib/process.h
    vorgabe/lib/queue.h
    vorgabe/lib/RR.h
    vorgabe/lib/scheduler.h
    vorgabe/lib/SRTN.h
    vorgabe/src/colors.c
    vorgabe/src/FCFS.c
    vorgabe/src/HRRN.c
    vorgabe/src/LCFSPR.c
    vorgabe/src/MLF.c
    vorgabe/src/PRIONP.c
    vorgabe/src/queue.c
    vorgabe/src/RR.c
    vorgabe/src/scheduler.c
    vorgabe/src/SRTN.c
    vorgabe/tests/helpers.h
    vorgabe/tests/test_HRRN_simple.c
    vorgabe/tests/test_LCFSPR_simple.c
    vorgabe/tests/test_MLF_simple.c
    vorgabe/tests/test_PRIONP_simple.c
    vorgabe/tests/test_queue_complex.c
    vorgabe/tests/test_queue_simple.c
    vorgabe/tests/test_RR_complex.c
    vorgabe/tests/test_RR_simple.c
    vorgabe/tests/test_SRTN_simple.c)
