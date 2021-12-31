import shutil
import os
import subprocess
import time
import filecmp
from os.path import exists
from sys import platform

TRres = "trace.out"
TRans = "trace.ans"
makefile = "Makefile"
codeName = "cachesimulator.cpp"
programName = "cachesimulator"
testNum = 3

def testCase(curDir, programName):
	if platform == 'win32':
		programName = 'cachesimulator.exe'
	else:
		programName = 'cachesimulator'
	os.chdir(curDir)

	## Run program
	try:
		run_process = subprocess.Popen(["./" + programName, "cacheconfig.txt", "trace"], stderr=subprocess.STDOUT)
		time.sleep(5)
		if run_process.poll() != 0:
			try:
				run_process.terminate()
			except:
				return 0
	except:
		return 0

	## Remove program after call
	try:
		os.remove(programName)
	except:
		pass

	f2 = open(TRans, "r")
	try:
		f1 = open(TRres, "r")
	except:
		f1 = ""
		import re
		for f in os.listdir("./"):
			if re.match(".*out.*", f):
				f1 = f
				break
		try:
			f1 = open(f1, "r")
		except:
			return 0

	f1_outputs = f1.readlines()
	f2_outputs = f2.readlines()
	lineNum = len(f2_outputs)
	crtNum  = 0

	outputNum = len(f1_outputs)
	for i in range(lineNum):
		if i >= outputNum:
			break
		if f1_outputs[i] == f2_outputs[i]:
			crtNum += 1
	f1.close()
	os.remove(TRres)
	return 100 * crtNum / float(lineNum)


def grade():
	if platform == 'win32':
		programName = 'cachesimulator.exe'
	else:
		programName = 'cachesimulator'

	# Compile cpp code to excutable file
	make_process = subprocess.Popen("make", stderr=subprocess.STDOUT)
	scores = []
	if make_process.wait() != 0:
		print("Make fail")
		try:
			make_process.terminate()
		except:
			pass

	listFiles = [f for f in os.listdir(os.getcwd())]
	for item in listFiles:
		if item == programName:
			os.rename(os.path.join(os.getcwd(), item), programName)
			break

	## Run testcases
	srcProgram = os.path.join(os.getcwd(), programName)
	for i in range(testNum):
		path = str(os.getcwd()) + "/testcases/" + str(i + 1)
		dstProgram = os.path.join(path, programName)
		shutil.copy(srcProgram, dstProgram)

		curDir = os.getcwd()
		res = testCase(path, programName)
		os.chdir(curDir)

		print("TESTCASE " + str(i) + " Result: " + str(round(res, 2)))
		scores.append(res)

	## Delete environment for steduent and go back to grade dir
	return True, scores

if __name__ == "__main__":
	print("ECE 6913 Lab3 Test")
	print("Platform: ", platform)
	isCompile, Scores = grade()

	score = 0
	if isCompile:
		score = 60
	for test_number in range(0, testNum):
		if test_number == 2:
			score += 20 * Scores[test_number] / 100
		else:
			score += 10 * Scores[test_number] / 100
	print("Scores For Lab3: ", score)

