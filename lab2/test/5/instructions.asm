# Lab2 - Pipelined Test
# Store Indirect (no dependency)
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

lw r3 r0 8 # r3 = mem[8] = 32

# NOP
addu r31 r31 r31
addu r30 r30 r30
addu r29 r29 r29
addu r28 r28 r28

addu r3 r2 r3 # r3 = 48
lw r4 r0 16 # r4 = mem[16] = 24

# NOP
addu r31 r31 r31
addu r30 r30 r30
addu r29 r29 r29
addu r28 r28 r28

sw r3 r4 8 # mem[r4 + 8] = mem[32] = r3 = 48

halt