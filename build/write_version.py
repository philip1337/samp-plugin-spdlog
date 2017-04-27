import shutil
import sys
import os
import subprocess
import socket
import datetime

def GetRevision(path):
	git_dir = os.path.join(path, ".git")
	try:
		return subprocess.check_output(["git", "--git-dir", git_dir, "rev-parse", "HEAD"]).strip()
	except:
		return "UNKNOWN_VERSION"

def GetBranchName(path):
	git_dir = os.path.join(path, ".git")
	try:
		return subprocess.check_output(["git", "--git-dir", git_dir, "symbolic-ref", "--short", "HEAD"]).strip()
	except:
		return "UNKNOWN_BRANCH"

def ParseIncludeFile(srcInclude, dstInclude, rev):
	with open(srcInclude, "r") as sF:
		with open(dstInclude, "w+") as dF:
			for line in sF:
				dF.write(line.replace("{{GIT_REVISION}}", rev))

def main(srcDir, srcInclude, dstInclude):
	# Get rev
	rev = GetRevision(srcDir)
	branch = GetBranchName(srcDir)
	hostname = socket.getfqdn()

	# Include file
	ParseIncludeFile(srcInclude, dstInclude, rev)

	lines = (
		"#define BUILD_MACHINE \"{0}\"".format(hostname),
		"#define BUILD_BRANCH \"{0}\"".format(branch),
		"#define BUILD_REV \"{0}\"".format(rev),
		"#define BUILD_DATE \"{0}\"".format(datetime.datetime.now())
	)

	with open("{}/src/Version.hpp".format(srcDir), "w+") as f:
		f.write("\n".join(lines))

if __name__ == "__main__":
	main(*sys.argv[1:])
	