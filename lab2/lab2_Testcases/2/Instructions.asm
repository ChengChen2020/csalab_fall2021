# Lab2 - Pipelined Test
# Basic memory read and algebra (no dependency)
# 10/28/2021
# Yuanzhe Liu yl7897@nyu.edu
lw r1 r1 0 # r1 = mem[0] = 1 
lw r2 r2 4 # r2 = mem[4] = 2
lw r3 r3 8 # r3 = mem[8] = 3
lw r4 r4 12 # r4 = mem[12] = 4
addu r4 r0 r1

# NOP
addu r31 r31 r31
addu r30 r30 r30
addu r29 r29 r29
addu r28 r28 r28

addu r4 r4 r2

# NOP
addu r31 r31 r31
addu r30 r30 r30
addu r29 r29 r29
addu r28 r28 r28

addu r4 r4 r3

# NOP
addu r31 r31 r31
addu r30 r30 r30
addu r29 r29 r29
addu r28 r28 r28

subu r4 r4 r0

# NOP
addu r31 r31 r31
addu r30 r30 r30
addu r29 r29 r29
addu r28 r28 r28

subu r4 r4 r1

# NOP
addu r31 r31 r31
addu r30 r30 r30
addu r29 r29 r29
addu r28 r28 r28

subu r4 r4 r2

# NOP
addu r31 r31 r31
addu r30 r30 r30
addu r29 r29 r29
addu r28 r28 r28

subu r4 r4 r3 
halt