#ifndef APTRACER_NEXTFILENAME_H
#define APTRACER_NEXTFILENAME_H

#include <string>

namespace APTracer {
    /**
     * @brief Returns the next available filename based on a prefix, appending a numerical suffix until no file exists with the name.
     * 
     * Used to save images sequentially. Check for existence of images with the same name, or stereo images. Can be fed a filename 
     * with an extension. If there is no extension in the prefix, '.png' is used.
     * 
     * @param prefix Start of the filename, to be appended with a sequential number. Can have an extension.
     * @return std::string Next sequential valid free filename.
     */
    std::string next_filename(const std::string &prefix);
}

#endif