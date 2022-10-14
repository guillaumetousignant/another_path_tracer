#include "functions/NextFilename.h"
#include <fstream>
#include <iomanip>
#include <sstream>

auto exists_test(const std::string& name) -> bool {
    std::ifstream f(name.c_str());
    return f.good();
}

auto APTracer::next_filename(const std::string& prefix) -> std::string {
    std::string name;
    std::string extension;

    size_t point = prefix.find_last_of('.');

    if (point != std::string::npos) {
        name      = prefix.substr(0, point);
        extension = prefix.substr(point);
    }
    else {
        name      = prefix;
        extension = ".png";
    }

    unsigned int index = 1;
    std::ostringstream index_ss;
    index_ss << std::setw(3) << std::setfill('0') << index;
    std::string filename   = name + index_ss.str() + extension;
    std::string filename_L = name + index_ss.str() + "_L" + extension;
    std::string filename_R = name + index_ss.str() + "_R" + extension;
    std::string filename_S = name + index_ss.str() + "_S" + extension;

    while (exists_test(filename) || exists_test(filename_L) || exists_test(filename_R) || exists_test(filename_S)) {
        ++index;
        std::ostringstream index_ss1;
        index_ss1 << std::setw(3) << std::setfill('0') << index;
        filename   = name + index_ss1.str() + extension;
        filename_L = name + index_ss1.str() + "_L" + extension;
        filename_R = name + index_ss1.str() + "_R" + extension;
        filename_S = name + index_ss1.str() + "_S" + extension;
    }
    return filename;
}