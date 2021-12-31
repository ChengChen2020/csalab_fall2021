# Lab2 - Pipelined Test
# Basic memory read (no dependency)
# 10/28/2021
# Yuanzhe Liu yl7897@nyu.edu
lw r2 r1 0 # r2 = 1
lw r4 r3 4 # r4 = 2
lw r6 r5 8 # r6 = 4
lw r8 r7 16 # r8 = 8
lw r10 r9 20 # r10 = 16
lw r12 r11 24 # r12 = 32
lw r14 r13 28 # r14 = 64
lw r16 r15 32 # r16 = 128
halt