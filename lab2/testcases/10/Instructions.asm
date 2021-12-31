# Lab2 - Pipelined Test
# Searching element in the array
# r4 has base address, r5 is loop index, mem[4] contains the element, mem[0] uses for return target address, 0 if not found
# 10/28/2021
# Yuanzhe Liu yl7897@nyu.edu

lw r1 r1 4 # r1 = mem[4] = 25 (element we want to find) 0
lw r2 r2 8 # r2 = mem[8] = 40                           1
lw r3 r3 12 # r3 = mem[12] = 4                          2
lw r4 r4 16 # r4 = mem[16] = 20 array base address      3

# NOP
addu r31 r31 r31 #                                      4
addu r30 r30 r30 #                                      5
addu r29 r29 r29 #                                      6
addu r28 r28 r28 #                                      7

addu r6 r5 r4 # r6 has the address we need              8

# NOP
addu r31 r31 r31 #                                      9
addu r30 r30 r30 #                                      10
addu r29 r29 r29 #                                      11
addu r28 r28 r28 #                                      12

lw r7 r6 0 #                                            13

# NOP
addu r31 r31 r31 #                                      14
addu r30 r30 r30 #                                      15
addu r29 r29 r29 #                                      16
addu r28 r28 r28 #                                      17

beq r7 r1 2 #                                           18
sw r6 r0 0 #                                            19
halt #  Reach Here if we find it                        20
addu r5 r5 r3 #                                         21

# NOP
addu r31 r31 r31 #                                      22
addu r30 r30 r30 #                                      23
addu r29 r29 r29 #                                      24
addu r28 r28 r28 #                                      25

beq r5 r2 -19 # Jump to 8                               26
halt # Element not in the array, r0 = 0.
