#! /usr/bin/python

from os 		import getcwd, chdir
from os.path 	import basename
from pathlib	import Path
from sys 		import stdout, stderr, exit
from typing 	import NoReturn
from enum 		import IntEnum, StrEnum
from subprocess	import run

class __CMakeBuildTarget(StrEnum):
	ALLOCATOR_TEST 				= "allocator_test"
	TAB_ANALYZER_TEST   		= "tab_analyzer_test"
	LEXER_EOF_TEST 	 			= "lexer_eof_test"
	LEXER_NEWLINE_TEST  		= "lexer_newline_test"
	LEXER_TAB_TEST				= "lexer_tab_test"
	LEXER_COMMENT_TEST  		= "lexer_comment_test"
	LEXER_SPACE_TEST 			= "lexer_space_test"
	LEXER_CURSOR_TEST   		= "lexer_cursor_test"
	LEXER_PLUS_OPERATOR_TEST 	= "lexer_plus_operator_test"

class __TargetConfig(StrEnum):
	DEBUG = "Debug"

class __ExitCode(IntEnum):
	CREATE_DIR_FAILED  			= 1
	RUN_PROCESS_FAILED 			= 2
	SWITCH_DIR_FAILED  			= 3
	TEST_FAILED		   			= 4
	GENERATE_CMAKE_CACHE_FAILED = 5

def __cout(*msg: object) -> None:
	stdout.write(f" ".join(str(m) for m in msg) + "\n")
	stdout.flush()

def __cerr(*msg: object) -> None:
	stderr.write(f" ".join(str(m) for m in msg) + "\n")
	stderr.flush()
	exit(1)

def __die(code: __ExitCode) -> NoReturn:
	exit(code)

def __mkdir_build() -> None:
	build_dir = Path("build")

	if not build_dir.exists():
		__cout(f"Create new dir: <{build_dir}>")
		try:
			build_dir.mkdir(exist_ok = True)

		except Exception as error:
			__cerr(f"Failed to create dir: <{build_dir}> ", f"error: {error}")
			__die(code = __ExitCode.CREATE_DIR_FAILED)

def __gen_cmake() -> None:
	cmake_cache = Path("CMakeCache.txt")
	if not cmake_cache.exists():
		try:
			run(args = ["cmake", ".."])

		except Exception as error:
			__cerr(f"Cannot generate cmake, error: {error}")
			__die(code = __ExitCode.GENERATE_CMAKE_CACHE_FAILED)

def __cmake_build_target(target: str, config: __TargetConfig) -> None:
	try:
		run(args = ["cmake", "--build", ".", "--target", target, "--config", config])

	except Exception as error:
		__cerr(f"Failed to run process with args: <{target}>, error: {error}")
		__die(code = __ExitCode.RUN_PROCESS_FAILED)

def __run_test(config: __TargetConfig) -> None:
	try:
		run(args = ["ctest", "--verbose", "-C", config])

	except Exception as error:
		__cerr(f"Failed to run test, error: {error}")
		__die(code = __ExitCode.TEST_FAILED)

def __switch_working_dir() -> None:
	scripts_dir = "scripts"
	build_dir   = "build"
	current = getcwd()

	try:
		if basename(current) ==  scripts_dir:
			__cout(f"Change directory to: <{scripts_dir}>")
			chdir("..")

		__mkdir_build()
		chdir(build_dir)
	except Exception as error:
		__cerr(f"Failed to switch to directory: <{build_dir}>, error {error}")
		__die(code = __ExitCode.SWITCH_DIR_FAILED)

def main() -> None:
	__switch_working_dir()
	__gen_cmake()
	
	for target in __CMakeBuildTarget:		
		__cmake_build_target(target.value, __TargetConfig.DEBUG)

	__run_test(config = __TargetConfig.DEBUG)

main()