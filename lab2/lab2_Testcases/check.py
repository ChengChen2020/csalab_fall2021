import os
import shutil
import filecmp

def check(filetype):
    return filecmp.cmp(os.path.join(path, filetype + '.txt'), os.path.join(path, filetype + '_grading.txt'), shallow=False)

allFiles = os.walk(os.getcwd())
for path, dir_list, file_list in allFiles:
    if path == os.getcwd():
        continue
    if check('dmemresult') != True or check('RFresult') != True:
        print(path)
    with open(os.path.join(path, 'stateresult.txt'), 'r') as test, open(os.path.join(path, 'stateresult_grading.txt'), 'r') as reference:
        for line1 in test:
            for line2 in reference:
                if line1 != line2 and line2.split()[1] != 'X':
                    print(line1, line2)
                break
print("PASS")
