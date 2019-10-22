#include "parser.h"
#include <iostream>
int main(int argc, char** argv) {
	if (argc != 3) std::__throw_invalid_argument("two parameters must be specified.\n"
												 "Help: PA1 [path_to_scene_file] [path_to_save_image]\n");

	auto pScene = Parser::CreateSceneFromFile(argv[1]);
	pScene->render(argv[2]);
	std::cout << "end";
}
