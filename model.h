
#ifndef _MODEL_H
#define _MODEL_H

#include <vector>
#include <fstream>
#include <memory>
#include "biterm.h"
#include "doc.h"
#include "pvec.h"
#include "pmat.h"

using namespace std;

class Model {
public:
  //vector<Biterm> bs;
  typedef shared_ptr<vector<Biterm>> Bis;
  Bis bps=nullptr;
protected:
  int W;				// vocabulary size
  int K;				// number of topics
  int n_iter;			// maximum number of iteration of Gibbs Sampling
  int save_step;

  double alpha;			// hyperparameters of p(z)
  double beta;			// hyperparameters of p(w|z)
  //string dfile;			// docs file
  //string dir;			// result dir
  
  // sample recorders
  Pvec<double> nb_z;	// n(b|z), size K*1
  Pmat<double> nwz;	// n(w,z), size M*K
  Pvec<double> _pz;	    // p(z) = theta
  Pmat<double> _pw_z;   // p(w|z) = phi, size K * M
 public:
  Model(int K, int W, double a, double b, int n_iter, int save_step) : K(K), W(W), alpha(a), beta(b),
	  n_iter(n_iter),
	  save_step(save_step) {
	  srand(time(NULL));
	  cout << "initial model";
  }
  ~Model() {
	  cout << "delete model"<<endl;
	  cout << W << endl;
	  cout << K << endl;

  }
  
  // run estimate and inference procedures
 // void run();
  void train(string traindata);
  void savemodel(string dir);			// save model
  void loadmodel(string dir);   // load from saved model
  vector<double> predict(vector<int>&);
  vector<double> predict(vector<int>&,string type);
 
  
 protected:  
  // intialize memeber varibles and biterms
  void init(string dfile);		// load from docs
  void load_docs(string dfile);
  
  
  
  // update estimate of a biterm
  double update_biterm(Biterm& bi);
  
  // reset topic proportions for biterm b
  void reset_biterm_topic(Biterm& bi);
  
  // assign topic proportions for biterm b
  void assign_biterm_topic(Biterm& bi, int k);
  
  // compute condition distribution p(z|b)
  void compute_pz_b(Biterm& bi, Pvec<double>& p);
  // compute condition distribution p(z|w, d) with p(w|z) fixed
  void compute_pz_dw(int w, const Pvec<double>& pz_d, Pvec<double>& p);

  void update_all();
  void update_pz();
  void update_pw_z();
  void save_pz(string dir);
  void save_pw_z(string dir);
  void doc_infer_sum_b(const Doc& doc, Pvec<double>& pz_d);
  void doc_infer_sum_w(const Doc& doc, Pvec<double>& pz_d);
  void doc_infer_lda(const Doc& doc, Pvec<double>& pz_d);
  void doc_infer_mix(const Doc& doc, Pvec<double>& pz_d);
};

#endif
