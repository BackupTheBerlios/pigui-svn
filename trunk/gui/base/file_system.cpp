//
// C++ Implementation: file_system
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "file_system.h"

namespace GUI {

FileSystem*(*FileSystem::instance_func)(void)=0;


FileSystem::~FileSystem()
{
}


}
