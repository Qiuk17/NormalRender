#include "parser.h"
#include <iostream>
int main(int argc, char** argv) {
	if (argc != 3) std::__throw_invalid_argument("two parameters must be specified.\n"
												 "Help: PA2 [path_to_scene_file] [path_to_save_image]\n");

	auto pScene = Parser::CreateSceneFromFile(argv[1], ST_OPENGL);
	pScene->render(&argc, argv);
	std::cout << "Image rendered to " << argv[2] << std::endl;
	return 0;
}
