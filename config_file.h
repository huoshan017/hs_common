#ifndef __CONFIG_FILE_H__
#define __CONFIG_FILE_H__

#include "HSConfigReader.h"

typedef HSConfigFile<int> IntKeyConfigFile;
typedef HSConfigFile<unsigned int> UintKeyConfigFile;
typedef HSConfigFile<string> StringKeyConfigFile;

#endif