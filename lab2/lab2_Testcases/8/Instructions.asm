# Lab2 - Pipelined Test
# Load multiple and store multiple (no dependency)
# load multiple value from memory to the register than store them to the memory, mimic the LTM and STM in arm assembly.
# 10/28/2021
# Yuanzhe Liu yl7897@nyu.edu

lw r1 r1 4 # Base address of array A r1 = 12
lw r2 r2 8 # Base address of array B r2 = 20
lw r3 r3 0 # r3 = mem[0] = 33
lw r4 r1 0 # r4 = mem[r1]
lw r5 r1 4 # r5 = mem[r1 + 4]
lw r6 r1 8 # r6 = mem[r1 + 8]
lw r7 r1 12 # r7 = mem[r1 + 12]
addu r4 r4 r3
addu r5 r5 r3
addu r6 r6 r3
addu r7 r7 r3
sw r4 r2 0
sw r5 r2 0
sw r6 r2 0
sw r7 r2 0
halt
