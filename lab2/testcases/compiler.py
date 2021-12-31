import os

def decipherIType(trailer):
    if int(trailer[2]) < 0:
        result = format(int(trailer[1][1:]), "05b") + format(int(trailer[0][1:]), "05b") + format(~(-int(trailer[2]) )+ 1 & 0xFFFF, "016b")
    else:
        result = format(int(trailer[1][1:]), "05b") + format(int(trailer[0][1:]), "05b") + format(int(trailer[2]), "016b")
    print(result)
    return result

def decipherRType(trailer):
    result = format(int(trailer[1][1:]), "05b") + format(int(trailer[2][1:]), "05b") + format(int(trailer[0][1:]), "05b")
    return result

def simulator(curDir):
    with open(os.path.join(curDir , 'imem.txt'), 'w') as outputFile, open(os.path.join(curDir , 'instructions.asm'), 'r') as inputFile:
        print(curDir)
        for instr in inputFile.readlines():
            instr = instr.split("#")[0]
            if len(instr) < 4:
                continue
            instrComps = instr.split()
            if len(instrComps) == 0 or  instrComps[0].lower().strip() == "halt":
                [outputFile.write("11111111"+ '\n') for i in range(4)]
                continue
            elif instrComps[0].lower().strip() == "nop":
                [outputFile.write("00000000"+ '\n') for i in range(4)]
                continue
            elif instrComps[0].lower().strip() == "j" or instrComps[0].lower().strip() == "j":
                opcode = "000010"
                jAddr = instrComps[1]
                toWrite = opcode + format(int(jAddr), "026b")

            elif instrComps[0].lower().strip() == "addiu":
                toWrite = "001001"+ decipherIType(instrComps[1:])
            elif instrComps[0].lower().strip() == "beq":
                toWrite = "000100" + decipherIType(instrComps[1:])
            elif instrComps[0].lower().strip() == "lw":
                toWrite = "100011" + decipherIType(instrComps[1:])
            elif instrComps[0].lower().strip() == "sw":
                toWrite = "101011" + decipherIType(instrComps[1:])
            elif instrComps[0].lower().strip() == "addu":
                toWrite = "000000" + decipherRType(instrComps[1:]) + "00000" + "100001"
            elif instrComps[0].lower().strip() == "subu":
                toWrite = "000000" + decipherRType(instrComps[1:]) + "00000" + "100011"
            elif instrComps[0].lower().strip() == "and":
                toWrite = "000000" + decipherRType(instrComps[1:]) + "00000" + "100100"
            elif instrComps[0].lower().strip() == "or":
                toWrite = "000000" + decipherRType(instrComps[1:]) + "00000" + "100101"
            elif instrComps[0].lower().strip() == "nor":
                toWrite = "000000" + decipherRType(instrComps[1:]) + "00000" + "100111"
            else:
                raise ValueError("Invalid command")

            print("Binary: " + toWrite)
            if len(toWrite) == 32:
                [outputFile.write(toWrite[i:i+8] + '\n') for i in range(0,31,8)]
            else:
                raise ValueError("wrong length")

if __name__ == '__main__':
    path = os.getcwd()
    import shutil
    curDir = os.path.abspath(os.path.dirname(os.getcwd()))
    allFiles = os.walk(os.getcwd())
    srcDmemFile = os.path.join(os.getcwd(), "dmem.txt")
    for path, dir_list, file_list in allFiles:
        if path == os.getcwd():
            continue
        simulator(path)

