# Lab2 - Pipelined Test
# Load Indirect (no dependency)
# First load the address, than load the value from that address.
# 10/28/2021
# Yuanzhe Liu yl7897@nyu.edu

lw r1 r0 0 # r1 = 4

# NOP
addu r31 r31 r31
addu r30 r30 r30
addu r29 r29 r29
addu r28 r28 r28

lw r2 r1 0 # r2 = mem[r1] = mem[4] = 16

# NOP
addu r31 r31 r31
addu r30 r30 r30
addu r29 r29 r29
addu r28 r28 r28

lw r3 r2 4 # r3 = mem[r2 + 4] = mem[16 + 4] = mem[20] = 47

halt