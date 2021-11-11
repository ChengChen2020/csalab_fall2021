import os

programName = None

def runProgram(curDir):
    print(curDir)
    os.chdir(curDir)
    try:
        os.remove("./RFresult.txt")
        os.remove("./dmemresult.txt")
        os.remove("./stateresult.txt")
    except:
        pass
    os.system(programName)
    os.remove(programName)
    os.chdir(os.path.dirname(curDir))

if __name__ == '__main__':
    programName = input("Input your MIPS simulator: ")
    # programName = "../cc6858"
    path = os.getcwd()
    import shutil
    curDir = os.path.abspath(os.path.dirname(os.getcwd()))
    allFiles = os.walk(os.getcwd())
    srcProgram = os.path.join(os.getcwd(), programName)
    for path, dir_list, file_list in allFiles:
        if path == os.getcwd():
            continue
        dstProgram = os.path.join(path, programName)
        shutil.copy(srcProgram, dstProgram)
        runProgram(path)
