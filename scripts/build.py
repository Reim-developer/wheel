#! /usr/bin/python

from lib 		import (
	gen_or_read_options, die, cerr, StatusCode, Options,
	working_dir, change_dir_to, create_dir, cout
)
from pathlib 	import Path
from subprocess	import run
from shutil 	import rmtree

VALID_COMPILER 			= ["clang++", "g++", "default"]
VALID_MODE 	   			= ["debug", "release"]
VALID_VERBOSE  			= [True, False]
VALID_COMPILER_COMMANDS = [True, False]

def build_from_options(options: Options) -> None:
	verbose  = options.build_options.verbose  if options.build_options.verbose	in VALID_VERBOSE else True
	mode 	 = options.build_options.mode 	  if options.build_options.mode     in VALID_MODE     else "debug"

	cmake_build_type = mode.capitalize()

	build_args = ["cmake", "--build", ".", "--config", cmake_build_type]
	if verbose:
		build_args.append("--verbose")

	run(build_args)


def config_cmake_from_options(options: Options) -> None:
	compiler = options.build_options.compiler 			if options.build_options.compiler in VALID_COMPILER else "default"	
	compiler_commands = options.extra.compiler_commands if options.extra.compiler_commands in VALID_COMPILER_COMMANDS else False
	args = ["cmake", "-S", "..", "-B", "." ]

	if compiler != "default":
		COMPILER_MAP = {
			"clang++": ("clang", "clang++"),
			"g++": ("gcc", "g++")
		}
		cout(f"Selected C++ Compiler as {compiler}")
		c_compiler, cxx_compiler = COMPILER_MAP[compiler]

		args.extend([
			"-G", "Ninja", 
			f"-DCMAKE_C_COMPILER={c_compiler}",
			f"-DCMAKE_CXX_COMPILER={cxx_compiler}",
		])

	if compiler_commands:
		args.append(f"-DCMAKE_EXPORT_COMPILE_COMMANDS=ON")

	if options.flags:
		for flag in options.flags:
			cmake_flag = f"-D{flag}=ON"

			cout(f"Enabled flag: {cmake_flag}")
			args.append(cmake_flag)

	run(args, check = True)

def match_directory() -> None:
	current = working_dir()
	if current == "scripts":
		change_dir_to("..")

	build = Path("build")
	if not build.exists():
		create_dir(build)

	change_dir_to(build)

def try_read_or_generate_cfg() -> Options:
	cfg_file = "cfg.json"
	try:
		options = gen_or_read_options(cfg_file)
		return options

	except Exception as error:
		cerr(f"Cannot generate or read {cfg_file}, error: {error}")
		die(StatusCode.CONFIG_LOAD_FAILED)

def refesh_by_options(options: Options, build_dir: Path | str) -> None:
	valid_refesh_option = [True, False]
	if options.extra.refresh_cache not in valid_refesh_option:
		options.extra.refresh_cache = False
	
	build_dir = Path(build_dir) if isinstance(build_dir, str) else build_dir
	if options.extra.refresh_cache:
		try:
			if build_dir.exists():
				cout(f"Refesh cache from build dir: {build_dir.absolute()}")
				rmtree(build_dir)

		except Exception as error:
			cerr(f"Cannot refesh cache, error: {error}")
			die(StatusCode.REMOVE_PATH_FAILED)

def base_dir() -> Path:
	base_directory = Path(".").resolve()
	if base_directory.name == "scripts":
		base_directory = base_directory.parent

	return base_directory / Path("build")

def main() -> None:
	options 	= try_read_or_generate_cfg()
	refesh_by_options(options, base_dir())

	match_directory()

	config_cmake_from_options(options)
	build_from_options(options)

try:
	main()
except KeyboardInterrupt:
	cerr("Canceled.")
	die(StatusCode.CANCELED)