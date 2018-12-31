#ifndef debug_h_INCLUDED
#define debug_h_INCLUDED

#include <fstream>
namespace debug {

std::ofstream f("log");
void log(std::string txt) {
	f << txt;
	f.flush();
}

}

#endif // debug_h_INCLUDED

