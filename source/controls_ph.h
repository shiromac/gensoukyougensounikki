#pragma once

#include <boost/shared_ptr.hpp>


class cCommand;
typedef boost::shared_ptr<cCommand> pcCommand;

class cMemoString;
typedef boost::shared_ptr<cMemoString> pcMemoString;
#define NULLOFpcMemoString (pcMemoString((cMemoString*)(NULL)))

class cStringWindow;
typedef boost::shared_ptr<cStringWindow> pcStringWindow;



