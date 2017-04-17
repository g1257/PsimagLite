/*
Copyright (c) 2009-2013, UT-Battelle, LLC
All rights reserved

[PsimagLite, Version 1.0.0]

*********************************************************
THE SOFTWARE IS SUPPLIED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED.

Please see full open source license included in file LICENSE.
*********************************************************

*/
#ifndef MATRIX_H_
#define MATRIX_H_

#include <unistd.h>
#include "Vector.h"
#include <stdexcept>
#include <iostream>
#include "LapackExtra.h"
#include "LAPACK.h"
#include "Complex.h"
#include <cassert>
#include "TypeToString.h"
#include "Mpi.h"
#include "BoostSerializationHeaders.h"

namespace PsimagLite {

template<typename RealType>
void expComplexOrReal(RealType& x,const RealType& y)
{
	x = exp(y);
}

template<typename RealType>
void expComplexOrReal(std::complex<RealType>& x,const RealType& y)
{
	x = std::complex<RealType>(cos(y),sin(y));
}

template<typename T2>
class MatrixNonOwned;

template<typename T>
class  Matrix  {
public:
	typedef T value_type; // legacy name

	Matrix()
	    : nrow_(0), ncol_(0)
	{}

	Matrix(SizeType nrow,SizeType ncol)
	    : nrow_(nrow),ncol_(ncol),data_(nrow*ncol, 0)
	{}

	template<typename RealType>
	Matrix(const Matrix<RealType>& m,
	       typename EnableIf<!IsComplexNumber<RealType>::True,int>::Type = 0)
	{
		nrow_=m.n_row();
		ncol_=m.n_col();
		data_.resize(nrow_*ncol_);
		for (SizeType i=0;i<nrow_;i++)
			for (SizeType j=0;j<ncol_;j++)
				data_[i+j*nrow_] = m(i,j);
	}

	// ctor closures
	Matrix(const std::ClosureOperator<Matrix,Matrix,std::ClosureOperations::OP_MULT>& c)
	{
		const T f1 = 1.0;
		matrixMatrix(c.r1,c.r2,f1);
	}

	template<typename T1>
	Matrix(const std::ClosureOperator<
	       std::ClosureOperator<T1,Matrix,std::ClosureOperations::OP_MULT>,
	       Matrix,
	       std::ClosureOperations::OP_MULT>& c,
	       typename EnableIf<Loki::TypeTraits<T1>::isArith,int>::Type = 0)
	{
		*this = c.r1.r1*c.r1.r2*c.r2;
	}

	template<typename T1>
	Matrix(const std::ClosureOperator<T1,
	       std::ClosureOperator<Matrix,Matrix,std::ClosureOperations::OP_PLUS>,
	       std::ClosureOperations::OP_MULT>& c,
	       typename EnableIf<Loki::TypeTraits<T1>::isArith,int>::Type = 0)
	{
		*this = c.r1*(c.r2.r1+c.r2.r2);
	}
	// end all ctors

	template<class Archive>
	void serialize(Archive& ar, const unsigned int)
	{
		ar & nrow_;
		ar & ncol_;
		ar & data_;
	}

	template<typename SomeMemResolvType>
	SizeType memResolv(SomeMemResolvType& mres,
	                   SizeType,
	                   String msg) const
	{
		String str = msg;
		str += "Matrix";
		const char* start = (const char *)&nrow_;
		const char* end = (const char*)&ncol_;
		SizeType total = mres.memResolv(&nrow_,end-start,str + " nrow_");

		start = end;
		end = (const char*)&data_;
		total += mres.memResolv(&ncol_,end-start,str + " ncol_");

		total += mres.memResolv(&data_,sizeof(*this)-total, str+" data_");

		return total;
	}

	// default assigment operator is fine

	SizeType n_row() const { return nrow_; } // legacy name

	SizeType n_col() const { return ncol_; } // legacy name

	SizeType rows() const { return nrow_; }

	SizeType cols() const { return ncol_; }

	const T& operator()(SizeType i,SizeType j) const
	{
		assert(i<nrow_ && j<ncol_);
		assert(i+j*nrow_<data_.size());
		return data_[i+j*nrow_];
	}

	T& operator()(SizeType i,SizeType j)
	{
		assert(i<nrow_ && j<ncol_);
		assert(i+j*nrow_<data_.size());
		return data_[i+j*nrow_];
	}

	bool operator==(const Matrix<T>& other) const
	{
		return (nrow_ == other.nrow_ &&
		        ncol_ == other.ncol_ &&
		        data_ == other.data_);
	}

	void resize(SizeType nrow,SizeType ncol)
	{
		if (nrow == nrow_ && ncol == ncol_) return;

		if (nrow_!=0 || ncol_!=0) throw
			RuntimeError("Matrix::resize(...): only applies when Matrix is empty\n");
		reset(nrow,ncol);
	}

	void reset(SizeType nrow,SizeType ncol)
	{
		nrow_=nrow; ncol_=ncol;
		data_.resize(nrow*ncol, 0);
	}

	Matrix<T>& operator+=(const Matrix<T>& other)
	{
		// domain checking ???
		for (SizeType i=0;i<ncol_*nrow_;i++)
			data_[i] += other.data_[i];
		return *this;
	}

	Matrix<T>& operator-=(const Matrix<T>& other)
	{
		// domain checking ???
		for (SizeType i=0;i<ncol_*nrow_;i++)
			data_[i] -= other.data_[i];
		return *this;
	}

	Matrix<T>& operator*=(const T& value)
	{
		for (SizeType i=0;i<ncol_*nrow_;i++)
			data_[i] *= value;
		return *this;
	}

	void print(std::ostream& os,const double& eps) const
	{
		os<<nrow_<<" "<<ncol_<<"\n";
		for (SizeType i=0;i<nrow_;i++) {
			for (SizeType j=0;j<ncol_;j++) {
				T val = data_[i+j*nrow_];
				if (PsimagLite::norm(val)<eps) val=0.0;
				os<<val<<" ";
			}
			os<<"\n";
		}
	}

	void print(int fd) const
	{
		::write(fd,(const void*)&ncol_,sizeof(ncol_));
		::write(fd,(const void*)&nrow_,sizeof(nrow_));
		::write(fd,(const void*)&(data_[0]),sizeof(T)*nrow_*ncol_);

	}

	void read(int fd)
	{
		::read(fd,&ncol_,sizeof(ncol_));
		::read(fd,&nrow_,sizeof(nrow_));
		data_.resize(nrow_*ncol_);
		::read(fd,&(data_[0]),sizeof(T)*nrow_*ncol_);
	}

	void setTo(const T& val)
	{
		for (SizeType i=0;i<data_.size();i++) data_[i]=val;
	}

	void send(int root,int tag,MPI::CommType mpiComm)
	{
		MPI::send(nrow_,root,tag,mpiComm);
		MPI::send(ncol_,root,tag+1,mpiComm);
		MPI::send(data_,root,tag+2,mpiComm);
	}

	void recv(int root,int tag,MPI::CommType mpiComm)
	{
		MPI::recv(nrow_,root,tag,mpiComm);
		MPI::recv(ncol_,root,tag+1,mpiComm);
		MPI::recv(data_,root,tag+2,mpiComm);
	}

	void bcast(int root,MPI::CommType mpiComm)
	{
		MPI::bcast(nrow_,root,mpiComm);
		MPI::bcast(ncol_,root,mpiComm);
		MPI::bcast(data_,root,mpiComm);
	}

	// start closure members

	template<typename T1>
	Matrix& operator+=(const std::ClosureOperator<T1,Matrix,std::ClosureOperations::OP_MULT>& c)
	{
		nrow_ = c.r2.nrow_;
		ncol_ = c.r2.ncol_;
		this->data_ += c.r1*c.r2.data_;
		return *this;
	}

	template<typename T1>
	Matrix& operator+=(const std::ClosureOperator<Matrix,T1,std::ClosureOperations::OP_MULT>& c)
	{
		nrow_ = c.r1.nrow_;
		ncol_ = c.r1.ncol_;
		this->data_ += c.r2*c.r1.data_;
		return *this;
	}

	template<typename T1>
	Matrix& operator=(const std::ClosureOperator<T1,Matrix,std::ClosureOperations::OP_MULT>& c)
	{
		nrow_ = c.r2.nrow_;
		ncol_ = c.r2.ncol_;
		this->data_ <= c.r1*c.r2.data_;
		return *this;
	}

	template<typename T1>
	Matrix& operator=(const std::ClosureOperator<Matrix,T1,std::ClosureOperations::OP_MULT>& c)
	{
		nrow_ = c.r1.nrow_;
		ncol_ = c.r1.ncol_;
		this->data_ <= c.r2*c.r1.data_;
		return *this;
	}

	template<typename T1>
	Matrix& operator=(const std::ClosureOperator<
	                  std::ClosureOperator<T1,Matrix,std::ClosureOperations::OP_MULT>,
	                  Matrix,
	                  std::ClosureOperations::OP_MULT>& c)
	{
		matrixMatrix(c.r1.r2,c.r2,c.r1.r1);
		return *this;
	}

	template<typename T1>
	Matrix& operator=(const std::ClosureOperator<T1,
	                  std::ClosureOperator<Matrix,Matrix,std::ClosureOperations::OP_PLUS>,
	                  std::ClosureOperations::OP_MULT>& c)
	{
		this->nrow_ = c.r2.r1.nrow_;
		this->ncol_ = c.r2.r1.ncol_;
		this->data_ <= c.r1*(c.r2.r1.data_ + c.r2.r2.data_);
		return *this;
	}

	template<typename T1>
	Matrix& operator=(const std::ClosureOperator<T1,
	                  std::ClosureOperator<Matrix,Matrix,std::ClosureOperations::OP_MINUS>,
	                  std::ClosureOperations::OP_MULT>& c)
	{
		this->nrow_ = c.r2.r1.nrow_;
		this->ncol_ = c.r2.r1.ncol_;
		this->data_ <= c.r1*(c.r2.r1.data_ - c.r2.r2.data_);
		return *this;
	}

	Matrix& operator=
	(const std::ClosureOperator<Matrix,Matrix,std::ClosureOperations::OP_PLUS>& c)
	{
		nrow_ = c.r1.nrow_;
		ncol_ = c.r1.ncol_;
		assert(nrow_ == c.r2.nrow_);
		assert(ncol_ == c.r2.ncol_);
		this->data_ <= c.r1.data_ + c.r2.data_;
		return *this;
	}

	Matrix& operator=
	(const std::ClosureOperator<Matrix,Matrix,std::ClosureOperations::OP_MINUS>& c)
	{
		nrow_ = c.r1.nrow_;
		ncol_ = c.r1.ncol_;
		assert(nrow_ == c.r2.nrow_);
		assert(ncol_ == c.r2.ncol_);
		this->data_ <= c.r1.data_ - c.r2.data_;
		return *this;
	}

	template<typename T1>
	Matrix& operator=
	(const std::ClosureOperator<Matrix,
	 std::ClosureOperator<T1,Matrix,std::ClosureOperations::OP_MULT>,
	 std::ClosureOperations::OP_PLUS>& c)
	{
		nrow_ = c.r1.nrow_;
		ncol_ = c.r1.ncol_;
		assert(nrow_ == c.r2.r2.nrow_);
		assert(ncol_ == c.r2.r2.ncol_);
		this->data_ <= c.r1.data_ + c.r2.r1*c.r2.r2.data_;
		return *this;
	}

	template<typename T1>
	Matrix& operator=
	(const std::ClosureOperator<
	 std::ClosureOperator<T1,Matrix,std::ClosureOperations::OP_MULT>,
	 Matrix,
	 std::ClosureOperations::OP_PLUS>& c)
	{
		nrow_ = c.r2.nrow_;
		ncol_ = c.r2.ncol_;
		assert(nrow_ == c.r1.r2.nrow_);
		assert(ncol_ == c.r1.r2.ncol_);
		this->data_ <= c.r2.data_ + c.r1.r1*c.r1.r2.data_;
		return *this;
	}

	Matrix& operator=
	(const std::ClosureOperator<Matrix<T>,Matrix,std::ClosureOperations::OP_MULT>& c)
	{
		const Matrix<T>& a = c.r1;
		const Matrix<T>& b = c.r2;
		const T f1 = 1.0;
		matrixMatrix(a,b,f1);

		return *this;
	}

	// end closure members

	template<typename T2>
	friend class MatrixNonOwned;

private:

	template<typename T1>
	void matrixMatrix(const Matrix<T>& a, const Matrix<T>& b, const T1& t1)
	{
		Matrix<T> m(a.n_row(), b.n_col());
		assert(a.n_col()==b.n_row());
		for (SizeType i=0;i<a.n_row();i++) {
			for (SizeType j=0;j<b.n_col();j++) {
				T sum = 0.0;
				for (SizeType k=0;k<a.n_col();k++) {
					sum += a(i,k) * b(k,j);
				}

				m(i,j) = sum*t1;
			}
		}

		*this = m; // copy is needed in case a or b are *this matrix
	}

	SizeType nrow_,ncol_;
	typename Vector<T>::Type data_;
}; // class Matrix

// start in Matrix.cpp
void geev(char jobvl,
          char jobvr,
          Matrix<std::complex<double> >& a,
          Vector<std::complex<double> >::Type & w,
          Matrix<std::complex<double> >& vl,
          Matrix<std::complex<double> >& vr);

void diag(Matrix<double> &m,Vector<double> ::Type& eigs,char option);

void diag(Matrix<std::complex<double> > &m,Vector<double> ::Type&eigs,char option);

void diag(Matrix<float> &m,Vector<float> ::Type& eigs,char option);

void diag(Matrix<std::complex<float> > &m,Vector<float> ::Type& eigs,char option);

void inverse(Matrix<std::complex<double> >& m);

void inverse(Matrix<double>& m);
// end in Matrix.cpp

template<typename T>
class IsMatrixLike {
public:
	enum { True = false};
};

template<typename T>
class IsMatrixLike<Matrix<T> > {
public:
	enum { True = true};
};

template<typename T>
std::ostream &operator<<(std::ostream &os,Matrix<T> const &A)
{
	SizeType i,j;
	os<<A.n_row()<<" "<<A.n_col()<<"\n";
	for (i=0;i<A.n_row();i++) {
		for (j=0;j<A.n_col();j++) os<<A(i,j)<<" ";
		os<<"\n";
	}
	return os;
}

template<typename T>
void mathematicaPrint(std::ostream& os,const Matrix<T>& A)
{
	os<<"m:={";
	for (SizeType i=0;i<A.n_row();i++) {
		os<<"{";
		for (SizeType j=0;j<A.n_col();j++) {
			os<<A(i,j);
			if (j+1<A.n_col()) os<<",";
		}
		os<<"}";
		if (i+1<A.n_row()) os<<",\n";
	}
	os<<"}\n";
}

template<typename T>
void symbolicPrint(std::ostream& os,const Matrix<T>& A)
{
	SizeType i,j;
	os<<A.n_row()<<" "<<A.n_col()<<"\n";
	typename Vector<T>::Type values;
	String s = "symbolicPrint: Not enough characters\n";
	SizeType maxCharacters = 25;
	for (i=0;i<A.n_row();i++) {
		for (j=0;j<A.n_col();j++) {

			const T& val = A(i,j);
			if (PsimagLite::norm(val)<1e-6) {
				os<<" 0 ";
				continue;
			}

			SizeType k=0;
			for (;k<values.size();k++)
				if (PsimagLite::norm(values[k]-val)<1e-6) break;
			bool b1 = (k==values.size());

			SizeType k2 = 0;
			for (;k2<values.size();k2++)
				if (PsimagLite::norm(values[k2]+val)<1e-6) break;

			bool b2 = (k2==values.size());

			if (b1) {
				if (b2) {
					values.push_back(val);
					if (values.size()>maxCharacters)
						throw RuntimeError(s.c_str());
					char chark = k + 65;
					os<<" "<<chark<<" ";
				} else {
					char chark = k2 + 65;
					os<<"-"<<chark<<" ";
				}
			} else {
				char chark = k + 65;
				os<<" "<<chark<<" ";
			}
		}
		os<<"\n";
	}
	os<<"---------------------------\n";
	for (SizeType i=0;i<values.size();i++) {
		char chari = i + 65;
		os<<chari<<"="<<values[i]<<" ";
	}
	os<<"\n";
}

template<typename T>
void printNonZero(const Matrix<T>& m,std::ostream& os)
{
	os<<"#size="<<m.n_row()<<"x"<<m.n_col()<<"\n";
	for (SizeType i=0;i<m.n_row();i++) {
		SizeType nonzero = 0;
		for (SizeType j=0;j<m.n_col();j++) {
			const T& val = m(i,j);
			if (val!=static_cast<T>(0)) {
				os<<val<<" ";
				nonzero++;
			}
		}
		if (nonzero>0) os<<"\n";
	}
	os<<"#diagonal non-zero\n";
	for (SizeType i=0;i<m.n_row();i++) {
		const T& val = m(i,i);
		if (val!=static_cast<T>(0)) {
			os<<val<<" ";
		}
	}
	os<<"\n";
}

template<typename T>
std::istream& operator>>(std::istream& is, Matrix<T>& A)
{
	SizeType nrow=0,ncol=0;
	is >> nrow;
	is >> ncol;

	if (is) {
		A.reset(nrow,ncol);
		for (SizeType j=0; j<A.n_row(); j++) for (SizeType i=0; i<A.n_col(); i++) {
			is >> A(j,i);
		}
	}
	if (!is) {
		String str("ERROR istream& operator >> ");
		str += "(std::istream&, Matrix<T>&): read past end stream";
		throw RangeError(str);
	}
	return is;
}

template<typename T>
bool isHermitian(Matrix<T> const &A,bool verbose=false)
{
	SizeType n=A.n_row();
	double eps=1e-6;
	if (n!=A.n_col())
		throw RuntimeError("isHermitian called on a non-square matrix.\n");
	for (SizeType i=0;i<n;i++) for (SizeType j=0;j<n;j++) {
		if (PsimagLite::norm(A(i,j)-PsimagLite::conj(A(j,i)))>eps) {
			if (verbose) {
				std::cerr<<"A("<<i<<","<<j<<")="<<A(i,j);
				std::cerr<<" A("<<j<<","<<i<<")="<<A(j,i)<<"\n";
			}
			return false;
		}
	}
	return true;
}

template<typename T>
bool isTheIdentity(Matrix<T> const &a)
{

	for (SizeType i=0;i<a.n_row();i++) {
		for (SizeType j=0;j<a.n_col();j++) {
			if (i!=j && PsimagLite::norm(a(i,j))>1e-6)  {
				std::cerr<<"a("<<i<<","<<j<<")="<<a(i,j)<<"\n";
				return false;
			}
		}
	}

	for (SizeType i=0;i<a.n_row();i++)
		if (PsimagLite::norm(a(i,i)-static_cast<T>(1.0))>1e-6) return false;

	return true;
}

template<typename T>
bool isZero(Matrix<std::complex<T> > const &a)
{

	for (SizeType i=0;i<a.n_row();i++)
		for (SizeType j=0;j<a.n_col();j++)
			if (PsimagLite::norm(a(i,j))>0) return false;
	return true;
}

template<typename T>
bool isZero(const Matrix<T>& m)
{
	for (SizeType i=0;i<m.n_row();i++)
		for (SizeType j=0;j<m.n_col();j++)
			if (fabs(m(i,j))>0) return false;
	return true;
}

// closures start

template<typename T1,typename T2>
typename EnableIf<(IsMatrixLike<T1>::True || IsMatrixLike<T2>::True)
&& !std::IsClosureLike<T1>::True && !std::IsClosureLike<T2>::True,
std::ClosureOperator<T1,T2,std::ClosureOperations::OP_MULT> >::Type operator*(const T1& a,
                                                                              const T2& b)
{
    return std::ClosureOperator<T1,T2,std::ClosureOperations::OP_MULT>(a,b);
}

template<typename T>
std::ClosureOperator<Matrix<T>,Matrix<T>,std::ClosureOperations::OP_PLUS>
operator+(const Matrix<T>& a,const Matrix<T>& b)
{
    return std::ClosureOperator<Matrix<T>,Matrix<T>,std::ClosureOperations::OP_PLUS>(a,b);
}

template<typename T>
std::ClosureOperator<Matrix<T>,Matrix<T>,std::ClosureOperations::OP_MINUS>
operator-(const Matrix<T>& a,const Matrix<T>& b)
{
    return std::ClosureOperator<Matrix<T>,Matrix<T>,std::ClosureOperations::OP_MINUS>(a,b);
}

template<typename T,typename A>
void operator<=(std::vector<T,A>& v, const std::ClosureOperator<Matrix<T>,
                std::vector<T,A>,
                std::ClosureOperations::OP_MULT>& c)
{
	const std::vector<T,A>& b = c.r2;
	const Matrix<T>& a = c.r1;
	assert(a.n_col()==b.size());
	v.resize(a.n_row());
	for (SizeType i=0;i<a.n_row();i++) {
		T sum = 0;
		for (SizeType j=0;j<b.size();j++) sum += a(i,j)*b[j];
		v[i] = sum;
	}
}

template<typename T,typename A>
void operator<=(std::vector<T,A>& v, const std::ClosureOperator<std::vector<T,A>,
                Matrix<T>,
                std::ClosureOperations::OP_MULT>& c)
{
	const std::vector<T,A>& b = c.r1;
	const Matrix<T>& a = c.r2;
	assert(a.n_row()==b.size());
	v.resize(a.n_col());
	for (SizeType i=0;i<a.n_col();i++) {
		T sum = 0;
		for (SizeType j=0;j<b.size();j++) sum += b[j] * a(j,i);
		v[i] = sum;
	}
}

template<typename T>
Matrix<T> multiplyTransposeConjugate(const Matrix<T>& O1,
                                     const Matrix<T>& O2,
                                     char modifier='C')
{
	SizeType n=O1.n_row();
	Matrix<T> ret(n,n);
	if (modifier=='C') {
		for (SizeType s=0;s<n;s++)
			for (SizeType t=0;t<n;t++)
				for (SizeType w=0;w<n;w++)
					ret(s,t) += PsimagLite::conj(O1(w,s))*O2(w,t);
	} else {
		for (SizeType s=0;s<n;s++)
			for (SizeType t=0;t<n;t++)
				for (SizeType w=0;w<n;w++)
					ret(s,t) += O1(w,s)*O2(w,t);
	}
	return ret;
}

template<class T>
void transposeConjugate(Matrix<T>& m2,const Matrix<T>& m)
{
	m2.resize(m.n_col(),m.n_row());
	for (SizeType i=0;i<m2.n_row();i++)
		for (SizeType j=0;j<m2.n_col();j++)
			m2(i,j)=PsimagLite::conj(m(j,i));

}

template<typename T>
Matrix<T> transposeConjugate(const Matrix<T>& A)
{
	Matrix<T> ret(A.n_col(),A.n_row());
	for (SizeType i=0;i<A.n_col();i++)
		for (SizeType j=0;j<A.n_row();j++)
			ret(i,j)=PsimagLite::conj(A(j,i));
	return ret;
}

template<typename T>
void exp(Matrix<T>& m)
{
	SizeType n = m.n_row();
	typename Vector<typename Real<T>::Type>::Type eigs(n);
	diag(m,eigs,'V');
	Matrix<T> expm(n,n);
	for (SizeType i=0;i<n;i++) {
		for (SizeType j=0;j<n;j++) {
			T sum = 0;
			for (SizeType k=0;k<n;k++) {
				typename Real<T>::Type alpha = eigs[k];
				T tmp = 0.0;
				expComplexOrReal(tmp,alpha);
				sum+= PsimagLite::conj(m(i,k))*m(j,k)*tmp;
			}
			expm(i,j) = sum;
		}
	}
	m = expm;

}

template<typename T>
T norm2(const Matrix<T>& m)
{
	T sum = 0;
	for (SizeType i=0;i<m.n_row();i++)
		for (SizeType j=0;j<m.n_col();j++)
			sum += m(i,j) * m(i,j);
	return sum;
}

template<typename T>
T norm2(const Matrix<std::complex<T> >& m)
{
	T sum = 0;
	for (SizeType i=0;i<m.n_row();i++)
		for (SizeType j=0;j<m.n_col();j++)
			sum += PsimagLite::real(PsimagLite::conj(m(i,j)) * m(i,j));
	return sum;
}

template<typename T>
void outerProduct(Matrix<T>& A,const Matrix<T>& B,const Matrix<T>& C)
{
	SizeType ni = B.n_row();
	SizeType nj = B.n_col();

	A.resize(B.n_row()*C.n_row(),B.n_col()*C.n_col());

	for (SizeType i1 = 0; i1 < B.n_row(); ++i1)
		for (SizeType j1 = 0; j1 < B.n_col(); ++j1)
			for (SizeType i2 = 0; i2 < C.n_row(); ++i2)
				for (SizeType j2 = 0; j2 < C.n_col(); ++j2)
					A(i1+i2*ni,j1+j2*nj) = B(i1,j1) * C(i2,j2);
}

// closures end

template<typename VectorLikeType>
void svd(char jobz,Matrix<double> &a,VectorLikeType& s,Matrix<double>& vt)
{
#ifdef NO_LAPACK
	throw RuntimeError("svd: dgesdd_: NO LAPACK!\n");
#else
	if (jobz != 'A' && jobz != 'S') {
		String msg("svd: jobz must be either A or S");
		String jobzString = " ";
		jobzString[0] = jobz;
		throw RuntimeError(msg + ", not " + jobzString + "\n");
	}

	int m = a.n_row();
	int n = a.n_col();
	std::cerr<<"Trying svd(...) "<<m<<"x"<<n<<"\n";
	int lda = m;
	int min = (m<n) ? m : n;

	s.resize(min);
	int ldu = m;
	int ucol = (jobz == 'A') ? m : min;
	Matrix<double> u(ldu,ucol);
	int ldvt = (jobz == 'A') ? n : min;
	vt.resize(ldvt,n);

	Vector<double>::Type work(100,0);
	int info = 0;
	Vector<int>::Type iwork(8*min,0);

	// query optimal work
	int lwork = -1;
	psimag::LAPACK::dgesdd_(&jobz,
	                        &m,
	                        &n,
	                        &(a(0,0)),
	                        &lda,
	                        &(s[0]),
	        &(u(0,0)),
	        &ldu,
	        &(vt(0,0)),
	        &ldvt,
	        &(work[0]),
	        &lwork,
	        &(iwork[0]),
	        &info);
	if (info!=0) {
		String str(__FILE__);
		str += " " + ttos(__LINE__);
		str += " svd(...) failed with info=" + ttos(info) + "\n";
		throw RuntimeError(str.c_str());
	}
	lwork = int(work[0]);
	work.resize(lwork+10);
	// real work:
	psimag::LAPACK::dgesdd_(&jobz,
	                        &m,
	                        &n,
	                        &(a(0,0)),
	                        &lda,
	                        &(s[0]),
	        &(u(0,0)),
	        &ldu,
	        &(vt(0,0)),
	        &ldvt,
	        &(work[0]),
	        &lwork,
	        &(iwork[0]),
	        &info);
	if (info!=0) {
		String str(__FILE__);
		str += " " + ttos(__LINE__);
		str += " svd(...) failed with info=" + ttos(info) + "\n";
		throw RuntimeError(str.c_str());
	}
	a = u;
#endif
}

#ifdef USE_MPI
namespace MPI {

template<typename SomeMatrixType>
typename EnableIf<IsMatrixLike<SomeMatrixType>::True &
Loki::TypeTraits<typename SomeMatrixType::value_type>::isArith,
void>::Type allReduce(SomeMatrixType& v,
                      MPI_Op op = MPI_SUM,
                      CommType mpiComm = MPI_COMM_WORLD)
{
	SomeMatrixType recvbuf = v;
	MPI_Datatype datatype = MpiData<typename SomeMatrixType::value_type>::Type;
	SizeType total = v.n_row() * v.n_col();
	int errorCode = MPI_Allreduce(&(v(0,0)),&(recvbuf(0,0)),total,datatype,op,mpiComm);
	checkError(errorCode,"MPI_Allreduce",mpiComm);
	v = recvbuf;
}

template<typename SomeMatrixType>
typename EnableIf<IsMatrixLike<SomeMatrixType>::True &
IsComplexNumber<typename SomeMatrixType::value_type>::True,
void>::Type allReduce(SomeMatrixType& v,
                      MPI_Op op = MPI_SUM,
                      CommType mpiComm = MPI_COMM_WORLD)
{
	SomeMatrixType recvbuf = v;
	MPI_Datatype datatype = MpiData<typename SomeMatrixType::value_type::value_type>::Type;
	SizeType total = v.n_row() * v.n_col();
	int errorCode = MPI_Allreduce(&(v(0,0)),&(recvbuf(0,0)),2*total,datatype,op,mpiComm);
	checkError(errorCode,"MPI_Allreduce",mpiComm);
	v = recvbuf;
}

} // namespace MPI
#endif // USE_MPI

} // namespace PsimagLite
#endif

