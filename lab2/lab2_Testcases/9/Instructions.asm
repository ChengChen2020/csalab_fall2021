# Lab2 - Pipelined Test
# For loop summing the array and store result back to memory
# r4 has base address, r5 is loop index, r8 contains the sum
# 10/28/2021
# Yuanzhe Liu yl7897@nyu.edu

lw r2 r2 8 # r2 = mem[8] = 40                           0
lw r3 r3 12 # r3 = mem[12] = 4                          1
lw r4 r4 16 # r4 = mem[16] = 20 array base address      2

# NOP
addu r31 r31 r31 #                                      3
addu r30 r30 r30 #                                      4
addu r29 r29 r29 #                                      5
addu r28 r28 r28 #                                      6

addu r6 r5 r4 # r6 has the address we need              7

# NOP
addu r31 r31 r31 #                                      8
addu r30 r30 r30 #                                      9
addu r29 r29 r29 #                                      10
addu r28 r28 r28 #                                      11

lw r7 r6 0 #                                            12

# NOP
addu r31 r31 r31 #                                      13
addu r30 r30 r30 #                                      14
addu r29 r29 r29 #                                      15
addu r28 r28 r28 #                                      16

addu r8 r8 r7 #                                         17
addu r5 r5 r3 #                                         18

# NOP
addu r31 r31 r31 #                                      19
addu r30 r30 r30 #                                      20
addu r29 r29 r29 #                                      21
addu r28 r28 r28 #                                      22

beq r5 r2 -17 # Jump to 7                               23
sw r8 r0 60 
halt
