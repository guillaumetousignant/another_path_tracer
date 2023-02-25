#include "functions/NextFilename.hpp"
#include <fstream>
#include <iomanip>
#include <sstream>

auto exists_test(const std::string& name) -> bool {
    const std::ifstream file(name.c_str());
    return file.good();
}

auto APTracer::next_filename(const std::string& prefix) -> std::string {
    std::string name;
    std::string extension;

    const size_t point = prefix.find_last_of('.');

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
    std::string filename   = name + index_ss.str().append(extension);
    std::string filename_L = name + index_ss.str().append("_L").append(extension);
    std::string filename_R = name + index_ss.str().append("_R").append(extension);
    std::string filename_S = name + index_ss.str().append("_S").append(extension);

    while (exists_test(filename) || exists_test(filename_L) || exists_test(filename_R) || exists_test(filename_S)) {
        ++index;
        std::ostringstream index_ss1;
        index_ss1 << std::setw(3) << std::setfill('0') << index;
        filename   = name + index_ss1.str().append(extension);
        filename_L = name + index_ss1.str().append("_L").append(extension);
        filename_R = name + index_ss1.str().append("_R").append(extension);
        filename_S = name + index_ss1.str().append("_S").append(extension);
    }
    return filename;
}