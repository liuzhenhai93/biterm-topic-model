#ifndef _STR_UTIL_H
#define _STR_UTIL_H

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

namespace str_util {
  // Trims spaces off the end and the beginning of the given string.
  inline string trim(string& str){
    string::size_type pos = str.find_last_not_of(" \n\t");
    if (pos != string::npos){
	  str.erase(pos + 1);
	  pos = str.find_first_not_of(" \n\t");
	  if (pos != string::npos) {
		str.erase(0, pos);
	  }
    }
    else{
	  // There is nothing else but whitespace in the string
	  str.clear();
    }
    return str;
  }

  // split a string by delimiter
  inline vector<string> split(string str, char delim=' ') {
    vector<string> vec;
    istringstream iss(str);
    string tmps;
    
    while (getline(iss, tmps, delim))
	  vec.push_back(tmps);
	
    return vec;
  }

  // turn a int to string
  inline string itos(int n) {
	ostringstream ss;
	ss << n;
	return ss.str();
  }
}

#endif
