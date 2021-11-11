# Lab2 - Pipelined Test
# Basic EX-MEM forwarding - Rs - Basic Algebra
# EX-MEM Rs datapath validation
# 11/1/2021
# Yuanzhe Liu yl7897@nyu.edu

lw r1 r1 4 # r1 = mem[4] = 1 (element we want to find) 0
lw r2 r2 8 # r2 = mem[8] = 2                           1
lw r3 r3 12 # r3 = mem[12] = 4

# NOP
addu r31 r31 r31 #                                      
addu r30 r30 r30 #                                      
addu r29 r29 r29 #                                      
addu r28 r28 r28 #                                      

addu r4 r1 r2 # rd - rs
addu r31 r31 r31 # 
addu r5 r4 r3

halt


