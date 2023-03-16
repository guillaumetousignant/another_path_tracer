#include "another_path_tracer.hpp"
#include <iostream>
#include <string>

auto main(int argc, char** argv) -> int {
    try {
        const APTracer::Helpers::InputParser_t input_parser(argc, argv);

        std::string filename;
        if (input_parser.cmdOptionExists("--help") || input_parser.cmdOptionExists("-h")) {
            std::cout << "usage: another_path_tracer [-h] [--scene SCENE] [-v]" << std::endl << std::endl;
            std::cout << "Another path tracer that simulates light bouncing around a scene." << std::endl << std::endl;
            std::cout << "options:" << std::endl;
            std::cout << '\t' << "-h, --help" << '\t' << "show this help message and exit" << std::endl;
            std::cout << '\t' << "-s, --scene" << '\t' << "full path of the input scene file" << std::endl;
            std::cout << '\t' << "-v, --version" << '\t' << "show program's version number and exit" << std::endl;
            return 0;
        }
        if (input_parser.cmdOptionExists("--version") || input_parser.cmdOptionExists("-v")) {
            std::cout << "another_path_tracer.exe 1.3.0" << std::endl;
            return 0;
        }

        if (input_parser.cmdOptionExists("-s")) {
            filename = input_parser.getCmdOption("-s");
        }
        else if (input_parser.cmdOptionExists("--scene")) {
            filename = input_parser.getCmdOption("--scene");
        }
        else {
            std::cout << "Enter scene path:" << std::endl;
            while (filename.empty()) {
                std::getline(std::cin, filename);
            }
            if (filename == "q") {
                return -1;
            }
        }

        APTracer::Entities::SceneContext_t scene_context;

        scene_context.readXML(filename);
        scene_context.render();

        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "Caught exception: " << ex.what() << std::endl;
        constexpr int exception_return = 4;
        return exception_return;
    }
    catch (const std::string& ex) {
        std::cerr << "Caught exception: " << ex << std::endl;
        constexpr int string_exception_return = 5;
        return string_exception_return;
    }
    catch (const char* ex) {
        std::cerr << "Caught exception: " << ex << std::endl;
        constexpr int char_exception_return = 6;
        return char_exception_return;
    }
    catch (...) {
        std::cerr << "Caught null exception" << std::endl;
        constexpr int null_exception_return = 7;
        return null_exception_return;
    }
}
