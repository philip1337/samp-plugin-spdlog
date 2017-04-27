import os
import shutil
import sys

def build(name, binDir):
	shutil.make_archive(name, 'zip', binDir)

if __name__ == "__main__":
	main(*sys.argv[1:])
	