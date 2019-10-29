
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>

#include "exceptinfo.h"
#include "config_parser.h"

//-----------------------------------------------------------------------------

using namespace std;

//-----------------------------------------------------------------------------

bool get_options(const std::string& fname, std::vector<std::string>& optionsList)
{
    std::fstream         ifs;

    ifs.open(fname.c_str(), std::ios::in);
    if (!ifs.is_open()) {
        throw except_info("%s, %d: %s():\n Can't open configuration file: %s\n", __FILE__, __LINE__, __FUNCTION__, fname.c_str());
    }

    optionsList.clear();

    while(!ifs.eof()) {

        std::string str;
        getline(ifs, str);

        if(!str.length())
            continue;

        // заменяем символы табуляции в строке на нули
        for(unsigned i=0; i<str.length(); i++) {
            if(str.at(i) == '\t' || str.at(i) == '\n' || str.at(i) == '\r' || str.at(i) == '#' || str.at(i) == ';')
                str[i] = '\0';
        }

        if(strstr(str.c_str(),"#")) {
            continue;
        }

        if(strstr(str.c_str(),";")) {
            continue;
        }

        if(strstr(str.c_str(),"=")) {
            optionsList.push_back(str);
        }
    }

    return !optionsList.empty();
}

//------------------------------------------------------------------------------

/**
 * 	\brief 	get value from command line
 *
 * 	format command line:
 * 	<name1> <value1> <name2> <value2>
 *
 * 	\param	argc		number of argument
 * 	\param	argv		pointers to arguments
 * 	\param	name		key of argument
 * 	\parma	defValue	default value for arguments
 *
 * 	\return   value of argument or default value of argument
 */
int GetFromCommnadLine(int argc, char **argv, const char* name, int defValue)
{
    int ret=defValue;
    for( int ii=1; ii<argc-1; ii++ ) {
        if(strcmp(argv[ii], name) == 0) {
            std::string val = argv[ii+1];
            ret = fromString<int>(val);
        }
    }
    return ret;
}

//-----------------------------------------------------------------------------

/**
 * 	\brief 	get configuration filename from command line
 *
 * 	format command line:
 * 	<arg1> <filename1> <arg2> <filename2>
 *
 * 	\param	argc		number of argument
 * 	\param	argv		pointers to arguments
 * 	\param	name		key of argument
 *
 * 	\return   pointer to filename or 0
 */
char* GetConfigFileName(int argc, char **argv, const char* argname)
{
    for( int ii=1; ii<argc-1; ii++ ) {
        if(strcmp(argv[ii], argname) == 0) {
            return argv[ii+1];
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------
