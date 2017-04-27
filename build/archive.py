import os
import shutil
import sys

def build(name, binDir):
	shutil.make_archive(name, 'zip', os.path.join(os.getcwd(), binDir))

if __name__ == "__main__":
	build(*sys.argv[1:])
	