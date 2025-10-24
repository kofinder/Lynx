
#ifndef LYNX_FILE_DATA_TYPE_HPP
#define LYNX_FILE_DATA_TYPE_HPP

#include <string>

struct File {

    std::string path;

    bool isOpen = false;
    
    File(const std::string& path) : path(path) {}

    const std::string& getPath() const { return path; }

};

#endif
