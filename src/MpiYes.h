/*
Copyright (c) 2009-2013, UT-Battelle, LLC
All rights reserved

[PsimagLite, Version 2.]
[by G.A., Oak Ridge National Laboratory]

UT Battelle Open Source Software License 11242008

OPEN SOURCE LICENSE

Subject to the conditions of this License, each
contributor to this software hereby grants, free of
charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), a
perpetual, worldwide, non-exclusive, no-charge,
royalty-free, irrevocable copyright license to use, copy,
modify, merge, publish, distribute, and/or sublicense
copies of the Software.

1. Redistributions of Software must retain the above
copyright and license notices, this list of conditions,
and the following disclaimer.  Changes or modifications
to, or derivative works of, the Software should be noted
with comments and the contributor and organization's
name.

2. Neither the names of UT-Battelle, LLC or the
Department of Energy nor the names of the Software
contributors may be used to endorse or promote products
derived from this software without specific prior written
permission of UT-Battelle.

3. The software and the end-user documentation included
with the redistribution, with or without modification,
must include the following acknowledgment:

"This product includes software produced by UT-Battelle,
LLC under Contract No. DE-AC05-00OR22725  with the
Department of Energy."

*********************************************************
DISCLAIMER

THE SOFTWARE IS SUPPLIED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER, CONTRIBUTORS, UNITED STATES GOVERNMENT,
OR THE UNITED STATES DEPARTMENT OF ENERGY BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.

NEITHER THE UNITED STATES GOVERNMENT, NOR THE UNITED
STATES DEPARTMENT OF ENERGY, NOR THE COPYRIGHT OWNER, NOR
ANY OF THEIR EMPLOYEES, REPRESENTS THAT THE USE OF ANY
INFORMATION, DATA, APPARATUS, PRODUCT, OR PROCESS
DISCLOSED WOULD NOT INFRINGE PRIVATELY OWNED RIGHTS.

*********************************************************

*/
/** \ingroup PsimagLite */
/*@{*/

/*! \file MpiYes.h
 *
 * Do NOT include this file directly, use Mpi.h instead
 *
 */
#ifndef MPIYES_HEADER_H
#define MPIYES_HEADER_H
#include "../loki/TypeTraits.h"
#include "Vector.h"
#include <algorithm>
#include <mpi.h>
#include <stdexcept>

namespace PsimagLite
{

namespace MPI
{

	typedef MPI_Comm CommType;
	extern CommType COMM_WORLD;

	template <typename T>
	struct MpiData {
		static const MPI_Datatype Type;
	};

	void checkError(int errorCode, String caller, CommType comm = COMM_WORLD);

	void init(int* argc, char** argv[]);

	void finalize();

	bool hasMpi();

	void info(std::ostream&);

	void version(std::ostream&);

	SizeType commSize(CommType mpiComm);

	SizeType commRank(CommType mpiComm);

	int barrier(CommType);

	template <typename NumericType>
	typename EnableIf<Loki::TypeTraits<NumericType>::isArith, void>::Type
	recv(NumericType& v, int source, int tag, CommType mpiComm = COMM_WORLD)
	{
		String name = "MPI_Recv";
		MPI_Datatype datatype = MpiData<NumericType>::Type;
		MPI_Status status;
		int errorCode = MPI_Recv(&v, 1, datatype, source, tag, mpiComm, &status);
		checkError(errorCode, name, mpiComm);
	}

	template <typename T1, typename T2>
	void recv(std::pair<T1, T2>& p, int source, int tag, CommType mpiComm = COMM_WORLD)
	{
		recv(p.first, source, tag, mpiComm);
		recv(p.second, source, 2 * tag, mpiComm);
	}

	template <typename SomeVectorType>
	typename EnableIf<
	    IsVectorLike<SomeVectorType>::True & Loki::TypeTraits<typename SomeVectorType::value_type>::isArith,
	    void>::Type
	recv(SomeVectorType& v, int source, int tag, CommType mpiComm = COMM_WORLD)
	{
		typedef typename SomeVectorType::value_type NumericType;
		String name = "MPI_Send";
		MPI_Datatype datatype = MpiData<NumericType>::Type;
		MPI_Status status;
		SizeType total = v.size();
		recv(total, source, tag, mpiComm);
		if (v.size() != total)
			v.resize(total);
		int errorCode = MPI_Recv(&(v[0]), v.size(), datatype, source, tag, mpiComm, &status);
		checkError(errorCode, name, mpiComm);
	}

	template <typename SomeVectorType>
	typename EnableIf<
	    IsVectorLike<SomeVectorType>::True & IsComplexNumber<typename SomeVectorType::value_type>::True,
	    void>::Type
	recv(SomeVectorType& v, int source, int tag, CommType mpiComm = COMM_WORLD)
	{
		typedef typename SomeVectorType::value_type NumericType;
		String name = "MPI_Send";
		MPI_Datatype datatype = MpiData<typename NumericType::value_type>::Type;
		MPI_Status status;
		SizeType total = v.size();
		recv(total, source, tag, mpiComm);
		if (v.size() != total)
			v.resize(total);
		int errorCode = MPI_Recv(&(v[0]), 2 * v.size(), datatype, source, tag, mpiComm, &status);
		checkError(errorCode, name, mpiComm);
	}

	template <typename NumericType>
	typename EnableIf<Loki::TypeTraits<NumericType>::isArith, void>::Type
	send(NumericType& v, int dest, int tag, CommType mpiComm = COMM_WORLD)
	{
		String name = "MPI_Send";
		MPI_Datatype datatype = MpiData<NumericType>::Type;
		int errorCode = MPI_Send(&v, 1, datatype, dest, tag, mpiComm);
		checkError(errorCode, name, mpiComm);
	}

	template <typename T1, typename T2>
	void send(std::pair<T1, T2>& p, int dest, int tag, CommType mpiComm = COMM_WORLD)
	{
		send(p.first, dest, tag, mpiComm);
		send(p.second, dest, 2 * tag, mpiComm);
	}

	template <typename SomeVectorType>
	typename EnableIf<
	    IsVectorLike<SomeVectorType>::True & Loki::TypeTraits<typename SomeVectorType::value_type>::isArith,
	    void>::Type
	send(SomeVectorType& v, int dest, int tag, CommType mpiComm = COMM_WORLD)
	{
		typedef typename SomeVectorType::value_type NumericType;
		String name = "MPI_Send";
		MPI_Datatype datatype = MpiData<NumericType>::Type;
		int total = v.size();
		send(total, dest, tag, mpiComm);
		int errorCode = MPI_Send(&(v[0]), v.size(), datatype, dest, tag, mpiComm);
		checkError(errorCode, name, mpiComm);
	}

	template <typename SomeVectorType>
	typename EnableIf<
	    IsVectorLike<SomeVectorType>::True & IsComplexNumber<typename SomeVectorType::value_type>::True,
	    void>::Type
	send(SomeVectorType& v, int dest, int tag, CommType mpiComm = COMM_WORLD)
	{
		typedef typename SomeVectorType::value_type NumericType;
		String name = "MPI_Send";
		MPI_Datatype datatype = MpiData<typename NumericType::value_type>::Type;
		int total = v.size();
		send(total, dest, tag, mpiComm);
		int errorCode = MPI_Send(&(v[0]), 2 * v.size(), datatype, dest, tag, mpiComm);
		checkError(errorCode, name, mpiComm);
	}

	template <typename SomeVectorType>
	typename EnableIf<
	    IsVectorLike<SomeVectorType>::True&(
		Loki::TypeTraits<typename SomeVectorType::value_type>::isArith | IsVectorLike<typename SomeVectorType::value_type>::True),
	    void>::Type
	pointByPointGather(SomeVectorType& v, int root = 0, CommType mpiComm = COMM_WORLD)
	{
		typedef typename SomeVectorType::value_type NumericType;
		int mpiRank = MPI::commRank(mpiComm);
		int nprocs = MPI::commSize(mpiComm);
		SizeType blockSize = static_cast<SizeType>(v.size() / nprocs);
		if (v.size() % nprocs != 0)
			blockSize++;

		if (mpiRank != root) {
			for (SizeType p = 0; p < blockSize; p++) {
				SizeType taskNumber = mpiRank * blockSize + p;
				if (taskNumber >= v.size())
					break;
				MPI::send(v[taskNumber], 0, taskNumber);
			}
		} else {
			for (int r = 0; r < nprocs; r++) {
				if (r == root)
					continue;
				for (SizeType p = 0; p < blockSize; p++) {
					SizeType taskNumber = r * blockSize + p;
					if (taskNumber >= v.size())
						break;
					NumericType value;
					MPI::recv(value, r, taskNumber);
					v[taskNumber] = value;
				}
			}
		}
	}

	template <typename SomeVectorType>
	typename EnableIf<
	    IsVectorLike<SomeVectorType>::True & !IsVectorLike<typename SomeVectorType::value_type>::True & !Loki::TypeTraits<typename SomeVectorType::value_type>::isArith & !IsPairLike<typename SomeVectorType::value_type>::True,
	    void>::Type
	pointByPointGather(SomeVectorType& v, int root = 0, CommType mpiComm = COMM_WORLD)
	{
		int mpiRank = MPI::commRank(mpiComm);
		int nprocs = MPI::commSize(mpiComm);
		SizeType blockSize = static_cast<SizeType>(v.size() / nprocs);
		if (v.size() % nprocs != 0)
			blockSize++;

		if (mpiRank != root) {
			for (SizeType p = 0; p < blockSize; p++) {
				SizeType taskNumber = mpiRank * blockSize + p;
				if (taskNumber >= v.size())
					break;
				v[taskNumber].send(0, taskNumber, mpiComm);
			}
		} else {
			for (int r = 0; r < nprocs; r++) {
				if (r == root)
					continue;
				for (SizeType p = 0; p < blockSize; p++) {
					SizeType taskNumber = r * blockSize + p;
					if (taskNumber >= v.size())
						break;
					v[taskNumber].recv(r, taskNumber, mpiComm);
				}
			}
		}
	}

	template <typename SomeVectorType>
	typename EnableIf<IsVectorLike<SomeVectorType>::True & IsPairLike<typename SomeVectorType::value_type>::True,
	    void>::Type
	pointByPointGather(SomeVectorType& v, int root = 0, CommType mpiComm = COMM_WORLD)
	{
		int mpiRank = MPI::commRank(mpiComm);
		int nprocs = MPI::commSize(mpiComm);
		SizeType blockSize = static_cast<SizeType>(v.size() / nprocs);
		if (v.size() % nprocs != 0)
			blockSize++;

		if (mpiRank != root) {
			for (SizeType p = 0; p < blockSize; p++) {
				SizeType taskNumber = mpiRank * blockSize + p;
				if (taskNumber >= v.size())
					break;
				send(v[taskNumber], 0, taskNumber, mpiComm);
			}
		} else {
			for (int r = 0; r < nprocs; r++) {
				if (r == root)
					continue;
				for (SizeType p = 0; p < blockSize; p++) {
					SizeType taskNumber = r * blockSize + p;
					if (taskNumber >= v.size())
						break;
					recv(v[taskNumber], r, taskNumber, mpiComm);
				}
			}
		}
	}

	template <typename SomeVectorType>
	typename EnableIf<
	    IsVectorLike<SomeVectorType>::True & IsVectorLike<typename SomeVectorType::value_type>::True & Loki::TypeTraits<typename SomeVectorType::value_type::value_type>::isArith,
	    void>::Type
	bcast(SomeVectorType& v, int root = 0, CommType mpiComm = COMM_WORLD)
	{
		typedef typename SomeVectorType::value_type DataType;
		String name = "MPI_Bcast";
		for (SizeType i = 0; i < v.size(); i++) {
			DataType& vv = v[i];
			int total = vv.size();
			int errorCode = MPI_Bcast(&total, 1, MPI_INTEGER, root, mpiComm);
			checkError(errorCode, name, mpiComm);
			if (static_cast<SizeType>(total) != vv.size())
				vv.resize(total);
			MPI_Datatype datatype = MpiData<typename DataType::value_type>::Type;
			errorCode = MPI_Bcast(&(vv[0]), vv.size(), datatype, root, mpiComm);
			checkError(errorCode, name, mpiComm);
		}
	}

	template <typename NumericType>
	typename EnableIf<Loki::TypeTraits<NumericType>::isArith, void>::Type
	bcast(NumericType& v, int root = 0, CommType mpiComm = COMM_WORLD)
	{
		MPI_Datatype datatype = MpiData<NumericType>::Type;
		int errorCode = MPI_Bcast(&v, 1, datatype, root, mpiComm);
		checkError(errorCode, "MPI_Bcast", mpiComm);
	}

	template <typename SomeVectorType>
	typename EnableIf<
	    IsVectorLike<SomeVectorType>::True & Loki::TypeTraits<typename SomeVectorType::value_type>::isArith,
	    void>::Type
	bcast(SomeVectorType& v, int root = 0, CommType mpiComm = COMM_WORLD)
	{
		SizeType total = v.size();
		bcast(total, root, mpiComm);
		if (total != v.size())
			v.resize(total, 0);
		MPI_Datatype datatype = MpiData<typename SomeVectorType::value_type>::Type;
		int errorCode = MPI_Bcast(&(v[0]), v.size(), datatype, root, mpiComm);
		checkError(errorCode, "MPI_Bcast", mpiComm);
	}

	template <typename SomeVectorType>
	typename EnableIf<
	    IsVectorLike<SomeVectorType>::True & IsComplexNumber<typename SomeVectorType::value_type>::True,
	    void>::Type
	bcast(SomeVectorType& v, int root = 0, CommType mpiComm = COMM_WORLD)
	{
		MPI_Datatype datatype = MpiData<typename SomeVectorType::value_type::value_type>::Type;
		int errorCode = MPI_Bcast(&(v[0]), 2 * v.size(), datatype, root, mpiComm);
		checkError(errorCode, "MPI_Bcast", mpiComm);
	}

	template <typename T1, typename T2>
	typename EnableIf<Loki::TypeTraits<T1>::isArith & Loki::TypeTraits<T2>::isArith,
	    void>::Type
	bcast(std::pair<T1, T2>& p, int root = 0, CommType mpiComm = COMM_WORLD)
	{
		T1 t1 = p.first;
		MPI_Datatype datatype = MpiData<T1>::Type;
		int errorCode = MPI_Bcast(&t1, 1, datatype, root, mpiComm);

		T2 t2 = p.second;
		MPI_Datatype datatype2 = MpiData<T2>::Type;
		errorCode = MPI_Bcast(&t2, 1, datatype2, root, mpiComm);

		p = std::pair<T1, T2>(t1, t2);
	}

	template <typename SomeVectorType>
	typename EnableIf<
	    IsVectorLike<SomeVectorType>::True & !IsVectorLike<typename SomeVectorType::value_type>::True & !Loki::TypeTraits<typename SomeVectorType::value_type>::isArith & !IsComplexNumber<typename SomeVectorType::value_type>::True,
	    void>::Type
	bcast(SomeVectorType& v, int root = 0, CommType mpiComm = COMM_WORLD)
	{
		for (SizeType i = 0; i < v.size(); i++) {
			v[i].bcast(root, mpiComm);
		}
	}

	template <typename SomeVectorType>
	typename EnableIf<
	    IsVectorLike<SomeVectorType>::True & Loki::TypeTraits<typename SomeVectorType::value_type>::isArith,
	    void>::Type
	reduce(SomeVectorType& v, MPI_Op op = MPI_SUM, int root = 0, CommType mpiComm = COMM_WORLD)
	{
		SomeVectorType w(v.size());
		MPI_Datatype datatype = MpiData<typename SomeVectorType::value_type>::Type;
		int errorCode = (mpiComm == 0)
		    ? MPI_SUCCESS
		    : MPI_Reduce(&(v[0]), &(w[0]), v.size(), datatype, op, root, mpiComm);
		checkError(errorCode, "MPI_Reduce", mpiComm);

		if (mpiComm && commRank(mpiComm) == static_cast<SizeType>(root))
			v = w;
	}

	template <typename SomeVectorType>
	typename EnableIf<
	    IsVectorLike<SomeVectorType>::True & IsComplexNumber<typename SomeVectorType::value_type>::True,
	    void>::Type
	reduce(SomeVectorType& v, MPI_Op op = MPI_SUM, int root = 0, CommType mpiComm = COMM_WORLD)
	{
		SomeVectorType w(v.size());
		MPI_Datatype datatype = MpiData<typename SomeVectorType::value_type::value_type>::Type;
		int errorCode = (mpiComm == 0)
		    ? MPI_SUCCESS
		    : MPI_Reduce(&(v[0]), &(w[0]), 2 * v.size(), datatype, op, root, mpiComm);
		checkError(errorCode, "MPI_Reduce", mpiComm);

		if (mpiComm && commRank(mpiComm) == static_cast<SizeType>(root))
			v = w;
	}

	template <typename SomeVectorType>
	typename EnableIf<
	    IsVectorLike<SomeVectorType>::True & Loki::TypeTraits<typename SomeVectorType::value_type>::isArith,
	    void>::Type
	allReduce(SomeVectorType& v, MPI_Op op = MPI_SUM, CommType mpiComm = COMM_WORLD)
	{
		SomeVectorType recvbuf = v;
		MPI_Datatype datatype = MpiData<typename SomeVectorType::value_type>::Type;
		int errorCode = MPI_Allreduce(&(v[0]), &(recvbuf[0]), v.size(), datatype, op, mpiComm);
		checkError(errorCode, "MPI_Allreduce", mpiComm);
		v = recvbuf;
	}

	template <typename SomeVectorType>
	typename EnableIf<
	    IsVectorLike<SomeVectorType>::True & IsComplexNumber<typename SomeVectorType::value_type>::True,
	    void>::Type
	allReduce(SomeVectorType& v, MPI_Op op = MPI_SUM, CommType mpiComm = COMM_WORLD)
	{
		SomeVectorType recvbuf = v;
		MPI_Datatype datatype = MpiData<typename SomeVectorType::value_type::value_type>::Type;
		int errorCode = MPI_Allreduce(&(v[0]), &(recvbuf[0]), 2 * v.size(), datatype, op, mpiComm);
		checkError(errorCode, "MPI_Allreduce", mpiComm);
		v = recvbuf;
	}

	template <typename NumericType>
	typename EnableIf<Loki::TypeTraits<NumericType>::isArith, void>::Type
	allReduce(NumericType& v, MPI_Op op = MPI_SUM, CommType mpiComm = COMM_WORLD)
	{
		int result = 0;
		MPI_Datatype datatype = MpiData<NumericType>::Type;
		int errorCode = MPI_Allreduce(&v, &result, 1, datatype, op, mpiComm);
		checkError(errorCode, "MPI_Allreduce", mpiComm);
		v = result;
	}

} // namespace MPI

} // namespace PsimagLite

/*@}*/
#endif
