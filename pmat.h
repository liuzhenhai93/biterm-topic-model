/*
 * (dense) Positive Matrix, each row is a Pvec
 *
 *  Created on: 2012-7-31
 *      Author: xhcloud@gmail.com
 */
#ifndef _PMAT_H
#define _PMAT_H

#include <vector>
#include <string>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <exception>
#include <ctime>

#include "pvec.h"

#define EXIT_ERR(s1, s2) {printf("[Error] %s%s\n", s1, s2);	\
	exit(EXIT_FAILURE);}

using namespace std;

template<class T>
class Pmat
{
private:
  vector<Pvec<T> > array;

public:
  Pmat(){}
  Pmat(int M, int N);
  Pmat(int M, int N, T v);
  Pmat(const Pmat& ma):array(ma.array){}

  void resize(int M, int N);
  void resize(int M, int N, T v);
  void rand_init();				// each entry (0,1]
  void load(const string& inf);
  void load_tmat(const string& inf);
  
  const int rows() const {return array.size();}
  const int size() const {return rows();}
  const int cols() const {return rows()?array[0].size():0;}
  Pvec<T> &operator[] (int m){return array[m];}
  const Pvec<T> &operator[] (int m) const {return array[m];}

  Pmat<T> operator+(const T & v){
	Pmat<T> tp(this->rows(), this->cols());
	for (int i=0; i<this->rows(); ++i)
	  tp[i] = array[i] + v;
	return tp;
  }

  Pmat<T>& operator+=(const T & v){
	for (int i=0; i<this->rows(); ++i)
	  array[i] += v;
	return *this;
  }

  Pmat<T> operator+(const Pmat<T>& v){
	Pmat<T> tp(this->rows(), this->cols());
	for (int i=0; i<this->rows(); ++i)
	  tp[i] = array[i] + v[i];
	return tp;
  }

  Pmat<T> &operator+=(const Pmat<T>& v){
	for (int i=0; i<this->rows(); ++i)
	  array[i] += v[i];
	return *this;
  }

  Pmat<T> operator-(const T & v){
	Pmat<T> tp(this->rows(), this->cols());
	for (int i=0; i<this->rows(); ++i)
	  tp[i] = array[i] - v;
	return tp;
  }

  Pmat<T>& operator-=(const T & v){
	for (int i=0; i<this->rows(); ++i)
	  array[i] -= v;
	return *this;
  }

  Pmat<T> operator-(const Pmat<T>& v){
	Pmat<T> tp(this->rows(), this->cols());
	for (int i=0; i<this->rows(); ++i)
	  tp[i] = array[i] - v[i];
	return tp;
  }

  Pmat<T> &operator-=(const Pmat<T>& v){
	for (int i=0; i<this->rows(); ++i)
	  array[i] -= v[i];
	return *this;
  }

  Pmat<T> operator*(const T & v){
	Pmat<T> tp(this->rows(), this->cols());
	for (int i=0; i<this->rows(); ++i)
	  tp[i] = array[i] * v;
	return tp;
  }

  void add_row(const Pvec<T>& v) {array.push_back(v);}
  void add_col(const Pvec<T>& v);
  
  T sum();                // sum of all elements
  void normalize(); 	  // normalize to all entries sum to 1
  void normr(double c=0);	  // row sum 1 normalization
  void normc();	  // col sum 1 normalization

  Pmat<T> transpose() const;
  string str();  
  void write(const string& pt);
};


template<class T>
Pmat<T>::Pmat(int M, int N):array(M) {
  for (int i = 0; i < M; i++)
	array[i].resize(N);
}

template<class T>
Pmat<T>::Pmat(int M, int N, T v):array(M) {
  for (int i = 0; i < M; i++)
	array[i].resize(N, v);
}

template<class T>
void Pmat<T>::resize(int M, int N) {
  array.resize(M);
  for (int i = 0; i < M; i++)
	array[i].resize(N);
}

template<class T>
void Pmat<T>::resize(int M, int N, T v) {
  array.resize(M);
  for (int i = 0; i < M; i++)
	array[i].resize(N, v);
}

template<class T>
void Pmat<T>::rand_init() {
  for (int i = 0 ; i < rows() ; ++i)
	array[i].rand_init();
}

// input format: v v v ...
// dimensions of the matrix are determinated by input data
template<class T>
void Pmat<T>::load(const string& inf) {
  ifstream rf(inf.c_str());
  if (!rf) 
	EXIT_ERR("file not find:", inf.c_str());
  
  try {
	string line;
	while (getline(rf, line)) {
	  // add a new row
	  Pvec<T> r(line);
	  add_row(r);
	}
  }
  catch (...) {
	EXIT_ERR("Err file:", inf.c_str());
  }
}

// load a transpose matrix
template<class T>
void Pmat<T>::load_tmat(const string& inf) {
  ifstream rf(inf.c_str());
  if (!rf) 
	EXIT_ERR("file not find:", inf.c_str());
  
  try {
	string line;
	while (getline(rf, line)) {
	  // add a new row
	  Pvec<T> r(line);
	  add_col(r);
	}
  }
  catch (...) {
	EXIT_ERR("Err file:", inf.c_str());
  }
}

template<class T>
void Pmat<T>::add_col(const Pvec<T>& v) {
  if (array.size() == 0)
	array.resize(v.size());
  else
	assert(array.size()==v.size());

  for (int i=0; i<array.size(); ++i)
	array[i].push_back(v[i]);
}

template<class T>
T Pmat<T>::sum() {
  T s=0;
  for (int i = 0 ; i < rows() ; ++i)
	s += array[i].sum();
  return s;
}

template<class T>
void Pmat<T>::normalize() {
  double eps = 1e-30;
  double smoother = eps * rows() * cols();
  
  T s = this->sum();
  for (int i=0 ; i<rows() ; ++i)
	for (int j=0; j<cols() ; ++j)
	  array[i][j] = (array[i][j] + eps)/(s + smoother);
}

// normalize elements in a row
template<class T>
void Pmat<T>::normr(double c) {
  for (int i = 0 ; i < rows() ; ++i)
	array[i].normalize(c);
}

// normalize elements in a row
template<class T>
void Pmat<T>::normc() {
  for (int j = 0; j < cols() ; ++j) {
	T s=0;
	for (int i = 0; i < rows(); ++i) 
	  s += array[i][j];
	
	for (int i = 0; i < rows() ; ++i)
	  array[i][j] /= s;
  }
}

template<class T>
Pmat<T> Pmat<T>::transpose() const {
  int N = rows();
  int M = cols();
  Pmat<T> tmat(M, N);

  for (int i=0; i<N; ++i)
	for (int j=0; j<M; ++j)
	  tmat[j][i] = array[i][j];

  return tmat;
}

template<class T>
string Pmat<T>::str() {
  ostringstream os;
  int i;
  for (i=0; i<rows(); ++i) 
	os << array[i].str() << endl;

  return os.str();
}

template<class T>
void Pmat<T>::write(const string& pt) {
  ofstream wf(pt.c_str());
  if (!wf) {
	cout << "Path not exists:" << pt << endl;
	wf.open("pmat.txt");
  }
  
  for (int i=0; i<rows(); ++i) 
	wf << array[i].str() << endl;
}

#endif
