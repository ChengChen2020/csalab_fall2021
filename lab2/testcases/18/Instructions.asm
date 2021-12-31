# Lab2 - Pipelined Test
# Advance EX-MEM forwarding - Rt - Memory
# EX-MEM Rt memory datapath validation
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

lw r4 r3 16 # r4 = mem[r3 + 16] = mem[20] = 15 
addu r31 r31 r31 #     
addu r5 r1 r4 
halt


