#include "NextFilename.h"
#include <sstream>
#include <iomanip>
#include <fstream>

bool exists_test (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

std::string next_filename(const std::string &prefix) {
    size_t point;
    std::string name, extension;
    std::string filename, filename_L, filename_R, filename_S;
    unsigned int index;

    point = prefix.find_last_of(".");

    if (point != std::string::npos){
        name = prefix.substr(0, point);
        extension = prefix.substr(point);
    }
    else{
        name = prefix;
        extension = ".png";
    }

    index = 1;
    std::ostringstream index_ss;
    index_ss << std::setw(3) << std::setfill('0') << index;
    filename = name + index_ss.str() + extension;
    filename_L = name + index_ss.str() + "_L" + extension;
    filename_R = name + index_ss.str() + "_R" + extension;
    filename_S = name + index_ss.str() + "_S" + extension;

    while (exists_test(filename) || exists_test(filename_L) || exists_test(filename_R) || exists_test(filename_S)){
        ++index;
        std::ostringstream index_ss1;
        index_ss1 << std::setw(3) << std::setfill('0') << index;
        filename = name + index_ss1.str() + extension;
        filename_L = name + index_ss1.str() + "_L" + extension;
        filename_R = name + index_ss1.str() + "_R" + extension;
        filename_S = name + index_ss1.str() + "_S" + extension;
    }
    return filename;
}