from sys 		import stdout, stderr
from enum 		import StrEnum, IntEnum
from typing 	import NoReturn
from os 		import getcwd, chdir, mkdir
from pathlib	import Path

class BuildOptions(StrEnum):
	WHEEL_DEBUG 	= "WHEEL_DEBUG"
	WHEEL_RELEASE 	= "WHEEL_RELEASE"

class StatusCode(IntEnum):
	SUCCESS 			= 0
	MISSING_ARGUMENT 	= 1
	ARGUMENT_NOT_FOUND 	= 2
	CHANGE_DIR_FAILED 	= 3
	CREATE_DIR_FAILED   = 4
	SUBPROCESS_FAILED	= 5

class BuildExtraOptions(StrEnum):
	REFESH_CACHE = "--refesh-cache"
	VERBOSE		 = "--verbose"

def cout(*msg: object) -> None:
	stdout.write(f" ".join(str(m) for m in msg) + "\n")
	stdout.flush()

def cerr(*msg: object) -> None:
	stderr.write(f" ".join(str(m) for m in msg) + "\n")
	stderr.flush()

def die(code: StatusCode) -> NoReturn:
	exit(code)

def working_dir() -> str:
	return getcwd()

def change_dir_to(parent: str | Path) -> None:
	chdir(parent)

def create_dir(dir: str | Path) -> None:
	mkdir(dir)