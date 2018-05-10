#ifndef _DOC_H
#define _DOC_H

#include <string>
#include <vector>
#include <list>
#include <cassert>
#include <sstream>

#include "biterm.h"

using namespace std;

class Doc {
private:
  vector<int> ws;				// word sequence
  
public: 
  Doc(const string& s) {read_doc(s);}
  Doc(const vector<int>& ws):ws(ws) { }

  int size() const {return ws.size();}
  
  const vector<int>& get_ws() const {return ws;}
  
  const int get_w(int i) const {
	assert(i < ws.size());
	return ws[i];
  }

  void gen_biterms(vector<Biterm>& bs) const {
	if (ws.size() < 2) return;
	//double weight = double(1) / (ws.size()-1);   // for weighted BTM in our journal paper
	double weight = 1;			// for original BTM in our WWW2013 paper

	for (int i = 0; i < ws.size()-1; ++i) 
	  for (int j = i+1; j < ws.size(); ++j) 
		bs.push_back( Biterm(ws[i], ws[j], weight) );
  }

private:
  void read_doc(const string& s) {
    istringstream iss(s);
	
	int w;
    while (iss >> w)  ws.push_back(w);
  }
};
  
#endif

