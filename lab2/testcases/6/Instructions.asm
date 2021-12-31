# Lab2 - Pipelined Test
# Branch test with load (no dependency)
# Beq behaves like branch not equal. hit 15 if failed. Otherwise should hit 16
# 10/28/2021
# Yuanzhe Liu yl7897@nyu.edu

lw r1 r1 8 # r1 = mem[8] = 36       0
lw r2 r2 12 # r2 = mem[12] = 36     1

# NOP
addu r31 r31 r31 #                  2
addu r30 r30 r30 #                  3
addu r29 r29 r29 #                  4
addu r28 r28 r28 #                  5

beq r1 r2 7 # Not Taken             6     
lw r3 r3 16 # r3 = mem[16] = 25     7
lw r4 r4 20 # r4 = mem[20] = 26     8

# NOP
addu r31 r31 r31 #                  9
addu r30 r30 r30 #                  10
addu r29 r29 r29 #                  11
addu r28 r28 r28 #                  12

beq r3 r4 5 # Taken                 13
lw r10 r10 4 # r10 = mem[4] = -1    14
halt #                              15
halt #                              16
halt #                              17
halt #                              18
lw r10 r10 0 # r10 = mem[0] = 1     19
halt #                              20
