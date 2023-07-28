// BEGIN LICENSE BLOCK
/*
Copyright (c) 2011 , UT-Battelle, LLC
All rights reserved

[PsimagLite, Version 1.0.0]

------------------------------------------------------------------
THE SOFTWARE IS SUPPLIED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED.

Please see full open source license included in file LICENSE.
------------------------------------------------------------------

*/
// END LICENSE BLOCK

#include "Range.h"
#include "String.h"
#include "Vector.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

typedef double RealType;
#ifdef USE_MPI
#include "ConcurrencyMpi.h"
typedef PsimagLite::ConcurrencyMpi<RealType> ConcurrencyType;
#else
#include "ConcurrencySerial.h"
typedef PsimagLite::ConcurrencySerial<RealType> ConcurrencyType;
#endif

typedef PsimagLite::Vector<RealType>::Type VectorType;

void setVectors(PsimagLite::Vector<VectorType>::Type& vec, SizeType total1, SizeType total2)
{
	VectorType series(total1 * total2);
	VectorType tmp(total2);
	SizeType k = 0;
	for (SizeType j = 0; j < total1; j++) {
		for (SizeType i = 0; i < total2; i++) {
			RealType val = (k < 2) ? 1 : series[k - 1] + series[k - 2];
			tmp[i] = val;
			series[k++] = val;
		}
		vec.push_back(tmp);
	}
}

int main(int argc, char* argv[])
{
	if (argc < 4) {
		PsimagLite::String s(argv[0]);
		s += ": Needs total1, total2 and segmentSize as args\n";
		throw PsimagLite::RuntimeError(s.c_str());
	}
	typedef ConcurrencyType::CommType CommType;
	ConcurrencyType concurrency(argc, argv);

	SizeType total1 = atoi(argv[1]);
	SizeType total2 = atoi(argv[2]);
	SizeType ySize = atoi(argv[3]);
	std::pair<CommType, CommType> comm = concurrency.newCommFromSegments(ySize);

	PsimagLite::Range<ConcurrencyType> range(0, total1, concurrency, comm.second);

	PsimagLite::Vector<VectorType>::Type vec;
	setVectors(vec, total1, total2);
	VectorType sum(total1);
	while (!range.end()) {
		SizeType i = range.index();
		VectorType& v = vec[i];
		PsimagLite::Range<ConcurrencyType> range2(0, total2, concurrency, comm.first);
		while (!range2.end()) {
			SizeType j = range2.index();
			sum[i] += v[j];
			std::cout << "i=" << i << " j=" << j
				  << " comm1.rank=" << concurrency.rank(comm.first);
			std::cout << " comm2.rank=" << concurrency.rank(comm.second)
				  << " world.rank=" << concurrency.rank() << "\n";
			range2.next();
		}
		concurrency.reduce(sum, comm.first);
		if (concurrency.root(comm.first))
			std::cout << "sum[" << i << "]=" << sum[i] << "\n";
		range.next();
	}
}
