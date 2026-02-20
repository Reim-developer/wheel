#! /usr/bin/python

from os 		import getcwd, chdir
from os.path 	import basename
from pathlib	import Path
from sys 		import argv
from enum 		import StrEnum
from subprocess	import run
from lib 		import die, StatusCode, cout, cerr
from typing 	import Optional

class __CMakeBuildTarget(StrEnum):
	TAB_ANALYZER_TEST   				= "tab_analyzer_test"
	LEXER_EOF_TEST 	 					= "lexer_eof_test"
	LEXER_NEWLINE_TEST  				= "lexer_newline_test"
	LEXER_TAB_TEST						= "lexer_tab_test"
	LEXER_COMMENT_TEST  				= "lexer_comment_test"
	LEXER_SPACE_TEST 					= "lexer_space_test"
	LEXER_CURSOR_TEST   				= "lexer_cursor_test"
	LEXER_OPERATOR_TEST 				= "lexer_operator_test"
	LEXER_LITERAL_TEST 					= "lexer_literal_test"
	PARSER_ARENA_TEST 					= "parser_arena_test"
	PARSER_SYMBOL_INTERNER_TEST 		= "parser_symbol_interner_test"
	PARSER_VARIABLE_DECLARATION_TEST 	= "parser_variable_declaration_test"
	PARSER_AST_TEST 					= "parser_ast_test"
	UTILS_NOT_NULL_TEST					= "utils_not_null_test"
	MEMORY_SMALL_VEC_TEST 				= "memory_small_vec_test"

class __TargetConfig(StrEnum):
	DEBUG = "Debug"

def __mkdir_build() -> None:
	build_dir = Path("build")

	if not build_dir.exists():
		cout(f"Create new dir: <{build_dir}>")
		try:
			build_dir.mkdir(exist_ok = True)

		except Exception as error:
			cerr(f"Failed to create dir: <{build_dir}> ", f"error: {error}")
			die(code = StatusCode.CREATE_DIR_FAILED)

def __gen_cmake() -> None:
	cmake_cache = Path("CMakeCache.txt")
	if not cmake_cache.exists():
		try:
			run(args = ["cmake", ".."])

		except Exception as error:
			cerr(f"Cannot generate cmake, error: {error}")
			die(code = StatusCode.GENERATE_CMAKE_FAILED)


def __cmake_build_target(target: str, config: __TargetConfig, compiler_target: Optional[str] = None) -> None:
	try:
		run(args = ["cmake", "--build", ".", "--target", target, "--config", config])

	except Exception as error:
		cerr(f"Failed to run process with args: <{target}>, error: {error}")
		die(code = StatusCode.SUBPROCESS_FAILED)

def __run_test(config: __TargetConfig, target: Optional[str] = None) -> None:
	try:
		args = (
			["ctest", "--verbose", "-C", config] if target is None else 
			["ctest", "-R", target, "-C", config, "--verbose"]
		)

		run(args = args)

	except Exception as error:
		cerr(f"Failed to run test process, error: {error}")
		die(code = StatusCode.SUBPROCESS_FAILED)

def __switch_working_dir() -> None:
	scripts_dir = "scripts"
	build_dir   = "build"
	current 	= getcwd()

	try:
		if basename(current) ==  scripts_dir:
			cout(f"Change directory to: <{scripts_dir}>")
			chdir("..")

		__mkdir_build()
		chdir(build_dir)
	except Exception as error:
		cerr(f"Failed to switch to directory: <{build_dir}>, error {error}")
		die(code = StatusCode.CHANGE_DIR_FAILED)

def main() -> None:
	__switch_working_dir()
	__gen_cmake()

	if len(argv) < 2:
		for target in __CMakeBuildTarget:		
			__cmake_build_target(target.value, __TargetConfig.DEBUG)

		__run_test(config = __TargetConfig.DEBUG)

	else:
		test_target = argv[1]

		
		if test_target.upper() not in __CMakeBuildTarget.__members__:
			cerr((
				f"[!] Invalid test target: {test_target}\n"
				f"[+] Available {len(__CMakeBuildTarget.__members__)} test(s) "
				f"target: {(" | ".join(__CMakeBuildTarget.__members__))}"
			))
			die(StatusCode.ARGUMENT_NOT_FOUND)

		__cmake_build_target(target = test_target, config = __TargetConfig.DEBUG)
		__run_test(__TargetConfig.DEBUG, test_target)

try:
	main()

except KeyboardInterrupt:
	cerr("Canceled.")
	die(StatusCode.CANCELED)