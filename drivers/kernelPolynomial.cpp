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

#include "ChebyshevSerializer.h"
#include "Io/IoSimple.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unistd.h>

void usage(const char* progName)
{
	std::cerr << "Usage: " << progName << " -f file  -b omega1";
	std::cerr << " -e omega2 -s omegaStep \n";
	std::cerr << "Conditions: omega1<omega2 omegaStep>0 \n";
}

int main(int argc, char* argv[])
{
	int opt = 0;
	PsimagLite::String file = "";
	typedef double RealType;
	typedef PsimagLite::Vector<RealType>::Type VectorType;

	RealType wbegin = 0;
	RealType wend = 0;
	RealType wstep = 0;
	typedef PsimagLite::ChebyshevSerializer<VectorType> ChebyshevSerializerType;
	typedef ChebyshevSerializerType::KernelParametersType KernelParametersType;
	SizeType type = KernelParametersType::JACKSON;
	RealType lambda = 0.0;
	bool makeZero = false;
	SizeType cutoff = 0;
	while ((opt = getopt(argc, argv, "f:b:e:s:c:l:zd")) != -1) {
		switch (opt) {
		case 'f':
			file = optarg;
			break;
		case 'b':
			wbegin = atof(optarg);
			break;
		case 'e':
			wend = atof(optarg);
			break;
		case 's':
			wstep = atof(optarg);
			break;
		case 'c':
			cutoff = atoi(optarg);
			break;
		case 'l':
			type = KernelParametersType::LORENTZ;
			lambda = atof(optarg);
			break;
		case 'd':
			type = KernelParametersType::DIRICHLET;
			break;
		case 'z':
			makeZero = true;
			break;
		default:
			usage(argv[0]);
			return 1;
		}
	}
	// sanity checks:
	if (file == "" || wbegin >= wend || wstep <= 0) {
		usage(argv[0]);
		return 1;
	}

	PsimagLite::IoSimple::In io(file);

	typedef PsimagLite::ChebyshevSerializer<VectorType> ChebyshevSerializerType;

	io.advance(ChebyshevSerializerType::stringMarker(),
	    PsimagLite::IoSimple::In::LAST_INSTANCE);

	ChebyshevSerializerType chebyshevSerializer(io);

	ChebyshevSerializerType::PlotParamsType params(wbegin, wend, wstep, 0.0, 0.0, 0);
	ChebyshevSerializerType::PlotDataType v;
	KernelParametersType kernelParams(type, cutoff, lambda);
	chebyshevSerializer.plot(v, params, kernelParams);
	for (SizeType x = 0; x < v.size(); x++) {
		RealType tmp = v[x].second;
		if (tmp < 0 && makeZero)
			tmp = 0;
		std::cout << v[x].first << " " << tmp << "\n";
	}
}
