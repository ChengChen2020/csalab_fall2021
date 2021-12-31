# Lab2 - Pipelined Test
# Basic memory read and algebra (no dependency)
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

addu r31 r31 r31 # NOP

addu r17 r2 r4
addu r18 r6 r8
addu r19 r10 r12
addu r20 r14 r16

# NOP
addu r31 r31 r31
addu r30 r30 r30
addu r29 r29 r29
addu r28 r28 r28

subu r20 r20 r19
subu r18 r18 r17

# NOP
addu r27 r27 r27
addu r26 r26 r26
addu r25 r25 r25
addu r24 r24 r24

subu r20 r20 r18
halt