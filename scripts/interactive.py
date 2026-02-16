# !/usr/bin/python

from lib 		import (
	cerr, cout, StatusCode, die, working_dir, change_dir_to,
	gen_or_read_options
)
from pathlib 	import Path
from typing 	import Callable
from sys 		import stdout
from shutil		import copy2

def format_table(headers: list[str], rows: list[list[str]]) -> str:
	all_data 		= [headers] + rows
	column_widths 	= [max(len(str(cell)) for cell in column) for column in zip(*all_data)]
	padding			= 2
	separator		= "+" + "+".join("-" * (width + padding) for width in column_widths) + "+"

	def format_row(rows: list[str]) -> str:
		cells = [str(cell).ljust(width) for cell, width in zip(rows, column_widths)]
		return "|" + "|".join(f"{cell} " for cell in cells) + "|"
	
	table_lines = [
		separator,
		format_row(headers),
		*[format_row(row) for row in rows],
		separator,
	]

	return "\n".join(table_lines)

def send_help(extra: str) -> None:
	cout("List of available commands:")
	
	headers 			 				= ["name", "aliases", "description"]
	rows: list[list[str]] 				= []
	seen_commands:	set[str]			= set()
	unique_commands: list[CommandInfo] 	= []

	for command in COMMAND_REGISTRY.values():
		if command.name not in seen_commands:
			seen_commands.add(command.name)
			unique_commands.append(command)

	for command in unique_commands:
		aliaeses_display = f"[{', '.join(command.aliases)}]" if command.aliases else "-"

		rows.append([
			command.name,
			aliaeses_display,
			command.description
		])

	rows.sort(key = lambda x: x[0])
	cout(format_table(headers, rows))

def copy_compiler_commands(extra: str) -> None:
	compiler_commands_file 	= Path("../compile_commands.json") if working_dir() == "scripts" else Path("compile_commands.json")
	build_dir 				= Path("../build") if working_dir() == "scripts" else Path("build")
	root_dir 				= Path("..") if working_dir() == "scripts" else Path(".")

	if not build_dir.exists():
		cerr(f"Build directory not found for find & copy: {build_dir.absolute()}")
		return 
	
	compiler_commands_path = Path(f"{build_dir}/{compiler_commands_file}")
	try:
		copy2(compiler_commands_path.absolute(), f"{compiler_commands_file.absolute()}")
		cout(f"Successfully copied {compiler_commands_file.absolute()} to {root_dir.absolute()}")

	except Exception as error:
		cerr(f"Cannot copy {compiler_commands_path.absolute()} to {compiler_commands_file.absolute()} , error: {error}")

def find_config(extra: str) -> None:
	if working_dir() == "scripts":
		change_dir_to("..")

	config_path = Path("cfg_2.json")
	if not config_path.exists():
		cerr(f"configuration file {config_path} not found, generate now? Y/N")

		while True:
			prompt = input("[y/n]: ")
			if len(prompt.strip()) == 0:
				continue

			match prompt.lower():
				case "n" | "no":
					cout("Canceled.")
					break

				case "y" | "yes":
					try:
						gen_or_read_options(config_path)
						cout(f"Generate configuration success, as: {config_path.absolute()}")
						break

					except Exception as error:
						cerr(f"Cannot generate configuration file, error: {error}")
						break

				case _:
					cerr(f"Option {prompt} is invalid")
					break
		return
	
	cout(f"Found configuration path as {config_path.absolute()}")

def clear_console(extra: str) -> None:
	print("\033[2J\033[H", end = "")
	stdout.flush()

def exit(extra: str) -> None:
	die(StatusCode.SUCCESS)

class CommandInfo:
	def __init__(self, name: str, aliases: list[str], 
			  	description: str, handler: Callable[[str], None]) -> None:
		self.name 			= name
		self.description 	= description
		self.aliases		= aliases
		self.handler 		= handler

	@property
	def all_commands(self) -> list[str]:
		return [self.name] + self.aliases

COMMAND_REGISTRY: dict[str, CommandInfo] = {}
def register_command(command_info: CommandInfo) -> None:
	for alias in command_info.all_commands:
		COMMAND_REGISTRY[alias.lower()] = command_info

register_command(CommandInfo(
	name 		= "help",
	aliases 	= ["help", "h", "?"],
	description = "show the help message",
	handler		= send_help
))
register_command(CommandInfo(
	name 		= "quit",
	aliases		= ["quit", "exit", "q"],
	description	= "exit interactive mode",
	handler		= exit
))
register_command(CommandInfo(
	name 		= "config",
	aliases		= ["config", "cfg"],
	description	= "find or generate the configuration",
	handler		= find_config
))
register_command(CommandInfo(
	name  		= "clear",
	aliases		= ["clear", "cls"],
	description = "clear interactive console",
	handler		= clear_console
))
register_command(CommandInfo(
	name 		= "copy",
	aliases		= ["copy-compiler-commands", "ccc", "cpc"],
	description = "copy compiler_commands.json to root directory",
	handler		= copy_compiler_commands
))

def main_loop() -> None:
	while True:
		command = input(">>> ")
		if len(command.strip()) == 0:
			continue

		parts 	= command.split(maxsplit = 1)
		command = parts[0].lower()
		args 	= parts[1] if len(parts) > 1 else ""

		command_object = COMMAND_REGISTRY.get(command)

		if command_object:
			command_object.handler(args)

		else:
			cerr(f"command not found: {command}")

try:
	main_loop()

except KeyboardInterrupt:
	cerr()
	die(StatusCode.CANCELED)