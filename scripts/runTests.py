#!/usr/bin/python

import os
import sys	

# Add the 'src' directory to the module search and PYTHONPATH
sys.path.append(sys.path[0] + os.sep + "src")

import glob
from optparse import OptionParser
from subprocess import Popen, PIPE
from Debug import Debug

# Global variables
parser                  = OptionParser(add_help_option=False)
helpShortFlag           = "-h"
generatedFilesDirectory = "generatedFiles"

# Add options to the command line
parser.add_option("--clean",
                  action="store_true",
                  dest="clean",
                  help="Remove generated files.",
                  default=False)

parser.add_option("--run",
                  action="store_true",
                  dest="run",
                  help="Run the tests.",
                  default=False)

parser.add_option(helpShortFlag,
                  "--help",
                  action="help",
                  help="Display this help message.")

parser.add_option("-v",
                 "--verbose",
                 action="store_true",
                 dest="verbose",
                 help="Be verbose.",
                 default=False)

parser.add_option("-T",
		  "--tests",
		  dest="tests",
		  action="append",
		  help="Run only this test number. (To run multiple tests, specify this option multiple times.)")

# Parse command-line options
(opts, args) = parser.parse_args(sys.argv[1:])

# Add debug support
debug = Debug(opts.verbose)

def testInteger(message, data):
	try:
		int(data)
	except ValueError:
		debug.exitMessage(message)

# Ensure the test numbers supplied by the user are indeed integers
if opts.tests:
	for testNum in opts.tests:
		testInteger("Supplied test number '%s' is not an integer" % (testNum), testNum)

# Cleans out files generated during the compilation process
def clean ():
	filesToRemove = []
	filesToRemove.extend(glob.glob('BLANK*.[fF?]*'))
	filesToRemove.extend(glob.glob('[!^BLANK]*_postprocessed.[fF?]*'))
	filesToRemove.extend(glob.glob('*.rmod'))
	filesToRemove.extend(glob.glob('*.mod'))
	filesToRemove.extend(glob.glob('hs_err_pid*.log'))
	filesToRemove.extend(glob.glob('~*'))
	filesToRemove.extend(glob.glob('rose*.[fF?]*'))
	filesToRemove.extend(glob.glob('rose*.CUF'))

	for file in filesToRemove:
		if opts.verbose:
			print("Removing file: '" + file + "'") 
		os.remove(file)

def generateCUDAMakefile (generatedFiles, testNum):
	from FileDependencies import getBaseFileName, determineModuleDependencies
	from Graph import Graph

	debug.verboseMessage("Generating Makefile for CUDA")

	CObjectFiles        = ['op_support.o', 'op_seq.o', 'debug.o']
	fortranSupportFiles = ['cudaConfigurationParams.F95', 'op2_c.F95']
	op2Directory        = "OP2_DIR"
	linkTarget          = "link"
	fortranTarget       = "fortranFiles"
	executableName      = "Test" + testNum

	# Work out the dependencies between modules 
	g = determineModuleDependencies(generatedFiles)

	# Create the Makefile  
	CUDAMakefile = open("Makefile." + testNum, "w")
	   
	# Makefile variables
	CUDAMakefile.write("FC      = pgfortran\n")
	CUDAMakefile.write("FC_OPT  = -Mcuda=cuda3.1 -fast -O2 -Mcuda=ptxinfo -Minform=inform\n")
	CUDAMakefile.write("OUT     = %s\n" % (executableName))
	CUDAMakefile.write("%s = \n\n" % (op2Directory))

	# PHONY targets
	CUDAMakefile.write("# Phony targets\n")
	CUDAMakefile.write(".PHONY: all clean\n\n")
	CUDAMakefile.write("all: %s %s\n\n" % (fortranTarget, linkTarget))
	CUDAMakefile.write("clean:\n\t")
	CUDAMakefile.write("rm -f *.o *.mod *.MOD $(OUT)\n\n")

	# Fortran target
	fortranLine = ""
	for f in fortranSupportFiles:
		fortranLine += f + " "
	for f in generatedFiles:
		fortranLine += os.path.basename(f) + " "

	CUDAMakefile.write(fortranTarget + ": ")
	CUDAMakefile.write(fortranLine + "\n\t")
	CUDAMakefile.write("$(FC) $(FC_OPT) -c ")
	CUDAMakefile.write(fortranLine + "\n\n")

	# Link target
	linkLine = ""	
	for f in CObjectFiles:
		linkLine += "$(%s)/%s " % (op2Directory, f)
	for f in fortranSupportFiles:
		linkLine += f[:-4] + ".o "
	for f in generatedFiles:
		basename = os.path.basename(f)
		linkLine += basename[:-4] + ".o "

	CUDAMakefile.write(linkTarget + ": ")
	CUDAMakefile.write(linkLine + "\n\t")
	CUDAMakefile.write("$(FC) $(FC_OPT) ")
	CUDAMakefile.write(linkLine + "-o " + executableName + "\n\n")

	CUDAMakefile.close()

	return CUDAMakefile.name

# Runs the compiler
def runTests ():
	from FormatFortranCode import FormatFortranCode	

	testsFile = 'testsToRun'
	if not os.path.isfile(testsFile):
		debug.exitMessage("Unable to find file containing tests to run. It should be called '%s'" % (testsFile))

	configFile = 'config'
	if not os.path.isfile(configFile):
		debug.exitMessage("Unable to find configuration file '%s' with the absolute path to source-to-source translator" % (configFile))

	translatorPath   = None
	translatorString = 'translator'
	op2Path          = None
	op2String        = 'OP2_DIR'
	for line in open(configFile, 'r'):
		line = line.strip()
		words = line.split('=')
		if line.startswith(translatorString):
			translatorPath = words[1].strip()
			if not os.path.isfile(translatorPath):
				debug.exitMessage("The translator binary '" + translatorPath + "' does not exist")
		elif line.startswith(op2String):
			op2Path = words[1].strip()
			if not os.path.isdir(op2Path):
				debug.exitMessage("The OP2_DIR '" + op2Path + "' specified in the config file is not a directory")

	if translatorPath is None:
		debug.exitMessage("You did not specify a path to the translator. Use 'translator=<path/to/translator>' in the configuration file")
	if op2Path is None:
		debug.exitMessage("You did not specify where the OP2 library, ISO_C_BINDING library, and User Kernels are to be found. Use 'OP2_DIR=<dir> in the configuration file'")

	failLexeme = 'FAIL'
	passLexeme = 'PASS'
	
	for line in open(testsFile, 'r'):
		tokens      = line.split(' ')
		testNum     = tokens[0].strip()
		FortranFile = tokens[1].strip()
		testResult  = tokens[2].strip()	
		run         = False

		testInteger("The test number '%s' in the file '%s' is not an integer" % (testNum, testsFile), testNum)

		if opts.tests:
			# If the user has specified to run certain tests then only run those
			if testNum in opts.tests:
				run = True
		else:
			# Otherwise run everything
			run = True
				
		if run:
			debug.verboseMessage("========== Test %s ==========" % (testNum))
			if testResult != failLexeme and testResult != passLexeme:
				debug.exitMessage("The outcome of testing '%s' is '%s' which is not understood. It either has to be '%s' or '%s'" % (FortranFile, testResult, failLexeme, passLexeme))

			if not os.path.isfile(FortranFile):
				debug.exitMessage("File '" + FortranFile + "' does not exist")		

			filesToCompile = ['ISO_C_BINDING.F95', 'OP2.F95', 'UserKernels.F95']

			cmd = translatorPath + ' --CUDA '
			for f in filesToCompile:
				cmd += op2Path + os.sep + f + ' '

			cmd += FortranFile

			debug.verboseMessage("Running: '" + cmd + "'")

			# Run the compiler in a bash shell as it needs the environment variables such as
			# LD_LIBRARY_PATH
			proc = Popen(cmd,
			     	     shell=True,
			     	     executable='/bin/bash',
			     	     stderr=PIPE,
			     	     stdout=PIPE)

			# Grab both standard output and standard error streams from the process
			stdoutLines, stderrLines = proc.communicate()

			if proc.returncode != 0:
				if testResult == failLexeme:
					debug.verboseMessage("Test on file '%s' passed" % (FortranFile))
				else:
					debug.verboseMessage("Test on file '%s' did NOT pass" % (FortranFile))
			else:
				if testResult == passLexeme:				
					debug.verboseMessage("Test on file '%s' passed" % (FortranFile))

					# The files generated by our compiler
					files = []
	 				files.append("rose_cuda_code.CUF")
					files.append("rose_" + FortranFile)
					f = FormatFortranCode (files) 
			
					if not os.path.exists(generatedFilesDirectory):
						os.makedirs(generatedFilesDirectory)

					renamedFiles = []
					j = 0
					for f in files:
						j = j + 1
						destName = generatedFilesDirectory + os.sep + "Test" + testNum + "File" + str(j) + f[-4:]
						debug.verboseMessage("Keeping file '%s'" % (destName))
						os.rename(f, destName)
						renamedFiles.append(destName)
				
					makefile = generateCUDAMakefile(renamedFiles, testNum)
					destName = generatedFilesDirectory + os.sep + makefile
					debug.verboseMessage("Generating Makefile '%s'" % (destName))				
					os.rename(makefile, destName)
				else:
					debug.verboseMessage("Test on file '%s' did NOT pass" % (FortranFile))
			debug.verboseMessage("")

if opts.clean:
	clean()

if opts.run:
	runTests()

if not opts.clean and not opts.run:
	debug.exitMessage("No actions selected. Use %s for options" % helpShortFlag)