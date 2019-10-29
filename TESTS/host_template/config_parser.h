#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <iomanip>

//------------------------------------------------------------------------------

template <typename T> std::string toString(T val)
{
    std::ostringstream oss;
    oss << val;
    return oss.str();
}

//------------------------------------------------------------------------------

template<typename T> T fromString(const std::string& s)
{
    std::istringstream iss(s);
    T res;
    if(strstr(s.c_str(),"0x")) {
        iss >> std::hex >> res;
    } else {
        iss >> res;
    }
    return res;
}

//------------------------------------------------------------------------------

template<typename T> bool get_value(const std::vector<std::string>& optionsList, const std::string& key, T& val)
{
    if(optionsList.empty())
        return -1;

    bool found = false;

    for(unsigned i=0; i<optionsList.size(); i++) {

        const std::string& opt = optionsList.at(i);
        int begin = opt.find_first_of("=", 0);
        int end = opt.length();

        if(!begin && (begin >= end))
            continue;

        std::string value = opt.substr(++begin, end);
        if(value.empty())
            continue;

        if(strstr(opt.c_str(),key.c_str())) {
            val = fromString<T>(value);
            found = true;
            break;
        }
    }

    return found;
}

//-----------------------------------------------------------------------------

bool get_options(const std::string& fname, std::vector<std::string>& optionsList);
int GetFromCommnadLine(int argc, char **argv, const char* name, int defValue);
char* GetConfigFileName(int argc, char **argv, const char* argname);

//-----------------------------------------------------------------------------

#endif // CONFIG_PARSER_H
