from dataclasses 	import dataclass, asdict
from json 			import dump
from pathlib		import Path
from lib 			import cout

@dataclass
class ProjectStructureMetadata:
	scripts_dir: str = "scripts"
	build_dir: 	 str = "build"
	root_dir: 	 str = "wheel"
	test_dir:	 str = "tests"

@dataclass
class Metadata:
	project_structure_metadata: ProjectStructureMetadata

	@staticmethod
	def show_default() -> None:
		cout(asdict(
			Metadata(ProjectStructureMetadata())
		))

	@staticmethod
	def generate_default(path: str | Path) -> ProjectStructureMetadata:
		with open(file = path, mode = "w", encoding = "utf-8") as medatada_file:
			dump(asdict(
				Metadata(ProjectStructureMetadata())
			), medatada_file, indent = 4)

		return ProjectStructureMetadata()
	