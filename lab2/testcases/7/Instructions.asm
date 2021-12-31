# Lab2 - Pipelined Test
# Counter using beq (no dependency)
# Beq behaves like branch not equal. r3 = i. 
# 10/28/2021
# Yuanzhe Liu yl7897@nyu.edu

lw r1 r1 8 # r1 = mem[8] = 1        0
lw r2 r2 12 # r2 = mem[12] = 10     1

# NOP
addu r31 r31 r31 #                  2
addu r30 r30 r30 #                  3
addu r29 r29 r29 #                  4
addu r28 r28 r28 #                  5

beq r3 r1 5 # go to 12               6

# NOP
addu r31 r31 r31 #                  7
addu r30 r30 r30 #                  8
addu r29 r29 r29 #                  9
addu r28 r28 r28 #                  10

beq r1 r2 10 #go to 22 when r3 = 10 11

# NOP
addu r31 r31 r31 #                  12
addu r30 r30 r30 #                  13
addu r29 r29 r29 #                  14
addu r28 r28 r28 #                  15

addu r3 r3 r1 #                     16

# NOP
addu r31 r31 r31 #                  17
addu r30 r30 r30 #                  18
addu r29 r29 r29 #                  19
addu r28 r28 r28 #                  20

beq r1 r2 -20 #                     21
lw r4 r4 16 # r4 = mem[16] = 1      22
halt
