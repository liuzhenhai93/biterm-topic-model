#include <cassert>
#include <iostream>
#include <string>
#include <cmath>
#include <limits>
#include <ctime>
#include <algorithm>

#include "sampler.h"
#include "str_util.h"
#include "model.h"

// n_status: number of latent status for each biterm
void Model::init(string dfile) {
  cout << "init" << endl;
  load_docs( dfile);
  vector<Biterm>& bs = *bps;
  nwz.resize(W, K);
  nb_z.resize(K);
  _pz.resize(K);
  _pw_z.resize(K, W);
  
  // random initialize
  for (vector<Biterm>::iterator b = bs.begin(); b != bs.end(); ++b) {
	int k = Sampler::uni_sample(K);
	assign_biterm_topic(*b, k);
  }
}

// input, each line is a doc
// format: wid  wid  wid ...
void Model::load_docs(string dfile) {
  cout << "load docs: " << dfile << endl;
  ifstream rf( dfile.c_str() );
  if (!rf) {
	cout << "file not find:" << dfile << endl;
	exit(-1);
  }
  bps = Bis(new vector<Biterm>());
  vector<Biterm>& bs = *bps;
  string line;
  while(getline(rf, line)) {
	Doc doc(line);
	doc.gen_biterms(bs);
  }
}

void Model::loadmodel(string dir)
{
	
		//if (type != "lda") {
			string pt = dir + "pz.k" + str_util::itos(K);
			cout << "load p(z):" << pt << endl;
			_pz.load(pt);
			//assert(abs(_pz.sum() - 1) < 1e-4);
		//}

		string pt2 = dir + "pw_z.k" + str_util::itos(K);
		cout << "load p(w|z):" << pt2 << endl;
		_pw_z.load(pt2);
		printf("n(z)=%d, n(w)=%d\n", _pw_z.rows(), _pw_z.cols());
		assert(_pw_z.rows() > 0 && abs(_pw_z[0].sum() - 1) < 1e-4);
	
}
void Model::train(string traindata)
{
	init(traindata);
	vector<Biterm>& bs = *bps;
	cout << "Begin iteration" << endl;
	for (int it = 1; it < n_iter + 1; ++it) {
		cout << "iter " << it << ", n(b):" << bs.size() << endl;

		for (int b = 0; b < bs.size(); ++b)
			update_biterm(bs[b]);
		if (it % save_step == 0)
			savemodel("model"+ str_util::itos(it));
	}
	savemodel("model");
	//biterms Çå³ý
	bps = nullptr;
}
vector<double> Model::predict(vector<int>& doc){
	cout << "predict";
	     Pvec<double> pz_d(K);
		doc_infer_sum_b(doc, pz_d);
		return pz_d.to_vector();
}
vector<double> Model::predict(vector<int>& doc,string type) {
	Pvec<double> pz_d(K);
	if (type == "sub_w")
		doc_infer_sum_w(doc, pz_d);
	else if (type == "lda")
		doc_infer_lda(doc, pz_d);
	else if (type == "mix")
		doc_infer_mix(doc, pz_d);
	else {
		doc_infer_sum_b(doc, pz_d);
	}
	return pz_d.to_vector();
}
// sample procedure for ith biterm 
double Model::update_biterm(Biterm& bi) {
  reset_biterm_topic(bi);
  
  // compute p(z|b)
  Pvec<double> pz;
  compute_pz_b(bi, pz);

  // sample topic for biterm b
  int k = Sampler::mult_sample(pz.to_vector());
  assign_biterm_topic(bi, k);

  return pz.max();
}

// reset topic assignment of biterm i
void Model::reset_biterm_topic(Biterm& bi) {
  int w1 = bi.get_wi();
  int w2 = bi.get_wj();
  int k = bi.get_z();
  double weight = bi.get_weight();
  
  nb_z[k] -= weight;	// update number of biterms in topic K
  nwz[w1][k] -= weight;	// update w1's occurrence times in topic K
  nwz[w2][k] -= weight;
  assert(nb_z[k] > -10e-7 && nwz[w1][k] > -10e-7 && nwz[w2][k] > -10e-7);
  
  bi.reset_z();
}

// compute p(z|w_i, w_j)
void Model::compute_pz_b(Biterm& bi, Pvec<double>& pz) {
  pz.resize(K);
  
  int w1 = bi.get_wi();
  int w2 = bi.get_wj();
  
  double pw1k, pw2k;
  for (int k = 0; k < K; ++k) {
	// avoid numerical problem by mutipling W
	double deno = W / (2 * nb_z[k] + W * beta);
	
	pw1k = (nwz[w1][k] + beta) * deno;
	pw2k = (nwz[w2][k] + beta) * deno;
	pz[k] = (nb_z[k] + alpha) * pw1k * pw2k;
  }
  pz.normalize();
}

// assign topic k to biterm i
void Model::assign_biterm_topic(Biterm& bi, int k) {
  int w1 = bi.get_wi();
  int w2 = bi.get_wj();
  double weight = bi.get_weight();
  
  bi.set_z(k);
  nb_z[k] += weight;
  nwz[w1][k] += weight;
  nwz[w2][k] += weight;
}

void Model::update_all() {
  update_pz();
  update_pw_z();
  //save_model();
}

// p(z) is determinated by the overall proportions
// of biterms in it
void Model::update_pz() {
	//Pvec<double> pz(K);	          // p(z) = theta
	for (int k = 0; k < K; k++)
		_pz[k] = (nb_z[k] + alpha);

	_pz.normalize();

}


void Model::update_pw_z() {
  // p(w|z) = phi, size K * M
  for (int k = 0; k < K; k++) {
	for (int m = 0; m < W; m++) 
	  _pw_z[k][m] = (nwz[m][k] + beta);

	_pw_z[k].normalize();
  }
  /*
  string pt = dir + "pw_z.k" + str_util::itos(K);
  cout << "write p(w|z): " << pt << endl;
  pw_z.write(pt);
  */
}

// format:wi   wj    z 
void Model::savemodel(string dir) {
	save_pz(dir);
	save_pw_z(dir);
}

// p(z) is determinated by the overall proportions
// of biterms in it
void Model::save_pz(string dir) {
	//Pvec<double> pz(K);	          // p(z) = theta
	for (int k = 0; k < K; k++)
		_pz[k] = (nb_z[k] + alpha);

	_pz.normalize();

	string pt = dir + "pz.k" + str_util::itos(K);
	cout << "write p(z): " << pt << endl;
	_pz.write(pt);
}

void Model::save_pw_z(string dir) {
	//Pmat<double> pw_z(K, W);   // p(w|z) = phi, size K * M
	for (int k = 0; k < K; k++) {
		for (int m = 0; m < W; m++)
			_pw_z[k][m] = (nwz[m][k] + beta);

		_pw_z[k].normalize();
	}
	string pt = dir + "pw_z.k" + str_util::itos(K);
	cout << "write p(w|z): " << pt << endl;
	_pw_z.write(pt);
}

// p(z|d) = \sum_b{ p(z|b)p(b|d) }
void Model::doc_infer_sum_b(const Doc& doc, Pvec<double>& pz_d) {
	pz_d.assign(K, 0);

	if (doc.size() == 1) {
		// doc is a single word, p(z|d) = p(z|w) \propo p(z)p(w|z)
		for (int k = 0; k < K; ++k)
			pz_d[k] = _pz[k] * _pw_z[k][doc.get_w(0)];
	}
	else {
		// more than one words
		vector<Biterm> bs;
		doc.gen_biterms(bs);

		int W = _pw_z.cols();
		for (int b = 0; b < bs.size(); ++b) {
			int w1 = bs[b].get_wi();
			int w2 = bs[b].get_wj();

			// filter out-of-vocabulary words
			if (w2 >= W) continue;

			// compute p(z|b) \propo p(w1|z)p(w2|z)p(z)
			Pvec<double> pz_b(K);
			for (int k = 0; k < K; ++k) {
				assert(_pw_z[k][w1]>0 && _pw_z[k][w2]>0);
				pz_b[k] = _pz[k] * _pw_z[k][w1] * _pw_z[k][w2];
			}
			pz_b.normalize();

			// sum for b, p(b|d) is unifrom
			for (int k = 0; k < K; ++k)
				pz_d[k] += pz_b[k];
		}
	}

	pz_d.normalize();
}

// p(z|d) = \sum_w{ p(z|w)p(w|d) }
void Model::doc_infer_sum_w(const Doc& doc, Pvec<double>& pz_d) {
	pz_d.assign(K, 0);

	int W = _pw_z.cols();
	const vector<int>& ws = doc.get_ws();

	for (int i = 0; i < ws.size(); ++i) {
		int w = ws[i];
		if (w >= W) continue;

		// compute p(z|w) \propo p(w|z)p(z)
		Pvec<double> pz_w(K);
		for (int k = 0; k < K; ++k)
			pz_w[k] = _pz[k] * _pw_z[k][w];

		pz_w.normalize();

		// sum for b, p(b|d) is unifrom
		for (int k = 0; k < K; ++k)
			pz_d[k] += pz_w[k];
	}
	pz_d.normalize();
}

// pure inference without changing training parameters
// test doc random initialize, and then iteratively update topic assignments
// of words in test documents
// compute p(z|d) = \sum_{w \in d} p(z|d,w) -- words are i.i.d in a documents
// inference smooth not enough!!!
void Model::doc_infer_lda(const Doc& doc, Pvec<double>& pz_d) {
	// random init p(z|d)
	pz_d.rand_init();

	int W = _pw_z.cols();

	const vector<int>& ws = doc.get_ws();
	int n_iter = 10;

	for (int it = 1; it < n_iter + 1; ++it) {
		//update pz_d
		Pvec<double> new_pz_d(K);

		for (int i = 0; i < ws.size(); ++i) {
			int w = ws[i];
			if (w >= W) continue;

			Pvec<double> pz_dw(K);

			compute_pz_dw(w, pz_d, pz_dw);
			new_pz_d += pz_dw;
		}

		new_pz_d.normalize();
		pz_d = new_pz_d;
	}
}

void Model::doc_infer_mix(const Doc& doc, Pvec<double>& pz_d) {
	pz_d.resize(K);
	for (int k = 0; k < K; ++k)
		pz_d[k] = _pz[k];

	const vector<int>& ws = doc.get_ws();
	int W = _pw_z.cols();
	for (int i = 0; i < ws.size(); ++i) {
		int w = ws[i];
		if (w >= W) continue;

		for (int k = 0; k < K; ++k)
			pz_d[k] *= (_pw_z[k][w] * W);
	}

	// sum for b, p(b|d) is unifrom
	pz_d.normalize();
}
// compute p(z | d, w) \proto p(w | z)p(z | d)
void Model::compute_pz_dw(int w, const Pvec<double>& pz_d, Pvec<double>& p) {
	p.resize(K);

	for (int k = 0; k < K; ++k)
		p[k] = _pw_z[k][w] * pz_d[k];

	p.normalize();
}
