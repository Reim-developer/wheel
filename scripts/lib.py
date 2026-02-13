from sys 			import stdout, stderr
from enum 			import StrEnum, IntEnum
from typing 		import NoReturn
from os 			import getcwd, chdir, mkdir
from pathlib		import Path
from dataclasses	import dataclass, asdict
from json 			import loads, dump

class StatusCode(IntEnum):
	SUCCESS 				= 0
	MISSING_ARGUMENT 		= 1
	ARGUMENT_NOT_FOUND 		= 2
	CHANGE_DIR_FAILED 		= 3
	CREATE_DIR_FAILED   	= 4
	SUBPROCESS_FAILED		= 5
	GENERATE_CMAKE_FAILED	= 6
	CANCELED				= 7
	CONFIG_FILE_NOT_FOUND	= 8
	CONFIG_LOAD_FAILED		= 9
	REMOVE_PATH_FAILED		= 10

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

def remove(dir_or_path: str | Path) -> None:
	remove(dir_or_path)

@dataclass
class BuildOptions:
	verbose: 	bool 	= True
	mode: 	 	str 	= "Debug"
	compiler: 	str 	= "default"

@dataclass
class Extra:
	refresh_cache: bool = False

@dataclass()
class Options:
	build_options: BuildOptions
	extra: 		   Extra
	 
def gen_or_read_options(cfg_path: str | Path) -> Options:
	path = cfg_path if isinstance(cfg_path, Path) else Path(cfg_path)

	if not path.exists():
		json = asdict(Options(BuildOptions(), Extra()))

		with open(file = cfg_path, mode = "w", encoding = "utf-8") as new_file:
			dump(json, new_file, indent = 4)

	with open(file = cfg_path, mode = "r", encoding = "utf-8") as cfg_file:
		raw_dict = loads(cfg_file.read())

		build_options_dict = raw_dict.get("build_options", {})
		extra_dict 		   = raw_dict.get("extra", {})
	
	return Options(BuildOptions(
		verbose 	= build_options_dict.get("verbose", True),
		mode 		= build_options_dict.get("mode", "debug"),
		compiler 	= build_options_dict.get("compiler", "default")
	), Extra (refresh_cache = extra_dict.get("refresh-cache", False)))