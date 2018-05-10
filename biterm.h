#ifndef _BITERM_H
#define _BITERM_H

#include <cmath>
#include <string>
#include <sstream>
#include <cassert>
#include<algorithm>

using namespace std;

class Biterm {
private:
  int wi;
  int wj;
  int z;						// topic assignment
  double weight;
  
public: 
  Biterm(int w1, int w2, double w): z(-1), weight(w) {

	wi = min(w1, w2);
	wj = max(w1, w2);
  }

  // s format:wi-wj:z
  Biterm(string s) {
	istringstream iss(s);
	iss >> wi >> wj >> weight >> z;
  }
  
  int get_wi() const {return wi;}
  int get_wj() const {return wj;}
  double get_weight() {return weight;}
  
  int get_z() const {return z;}
  void set_z(int k) {z = k;}
  void reset_z() {z = -1;}
  
  string str() const {
	ostringstream os;
	os << wi << '\t' << wj << '\t' << weight << '\t' << z;
	return os.str();
  }  
};

#endif
