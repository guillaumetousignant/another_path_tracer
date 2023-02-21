#include "another_path_tracer.hpp"
#include <iostream>
#include <string>

auto main(int argc, char** argv) -> int {
    try {
        std::string filename;
        if (argc < 2) {
            std::cout << "Enter scene path:" << std::endl;
            while (filename.empty()) {
                std::getline(std::cin, filename);
            }
            if (filename == "q") {
                return -1;
            }
        }
        else {
            filename = argv[1];
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