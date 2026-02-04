#! /usr/bin/python
from sys 			import argv
from os.path 		import basename
from lib			import (
	cout, BuildOptions, die, StatusCode, working_dir,
	change_dir_to, cerr, create_dir, BuildExtraOptions
)
from pathlib		import Path
from subprocess		import run

def show_help() -> None:
	cout((
		f"[?] Usage: {basename(__file__)} [Build Option]\n"
		f"[+] Available Build Option {(" | ".join(BuildOptions.__members__))}"
	))

def build_from_mode(mode: str, build_option: str, 
					refesh_cache: bool, verbose: bool) -> None:
	scripts_dir = "scripts"
	current_dir = working_dir()

	if current_dir == scripts_dir:
		try:
			change_dir_to("..")

		except Exception as error:
			cerr(f"Cannot change directory to '{scripts_dir}', error: {error}")
			die(StatusCode.CHANGE_DIR_FAILED)

	build_path = Path("build")
	if not build_path.exists():
		try:
			create_dir(build_path)

		except Exception as error:
			cerr(f"Create '{build_path}' failed, error: {error}")
			die(StatusCode.CREATE_DIR_FAILED)

	try:
		change_dir_to(build_path)
	except Exception as error:
		cerr(f"Cannot change directory to '{scripts_dir}', error: {error}")
		die(StatusCode.CHANGE_DIR_FAILED)

	if not Path("CMakeCache.txt").exists() or refesh_cache:
		try:

			run(["cmake", f"-D{build_option}=ON", ".."])

		except Exception as error:
			cerr(f"Failed to build from CMake, with error: {error}")
			die(StatusCode.SUBPROCESS_FAILED)

	try:
		build_args = ["cmake", "--build", ".", f"--config", mode]
		if verbose:
			build_args.append("--verbose")
			
		run(build_args)

	except Exception as error:
		cerr(f"Failed to build from CMake, with error: {error}")
		die(StatusCode.SUBPROCESS_FAILED)

def parse_from_cli() -> None:
	if len(argv) < 2:
		show_help()
		die(StatusCode.MISSING_ARGUMENT)
	
	if argv[1].upper() not in BuildOptions.__members__:
		show_help()
		die(StatusCode.ARGUMENT_NOT_FOUND)

	refesh_cache = False
	verbose 	 = False
	if len(argv) > 2:
		match argv[2]:
			case BuildExtraOptions.REFESH_CACHE:
				refesh_cache = True
			
			case BuildExtraOptions.VERBOSE:
				verbose 	 = True

			case _:
				cout((
					f"[+] Extra argument not found: {argv[2]}\n"
					f"[+] Available argument(s):\n"
					f"[+] {(" | ".join(BuildExtraOptions.__members__.values()))}"
				))
				die(StatusCode.ARGUMENT_NOT_FOUND)

	build_mode = "Debug" if argv[1].upper() == BuildOptions.WHEEL_DEBUG else "Release"
	build_from_mode(build_mode, argv[1], refesh_cache, verbose)

parse_from_cli()