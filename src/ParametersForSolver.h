/*
Copyright (c) 2009-2011, UT-Battelle, LLC
All rights reserved

[PsimagLite, Version 1.0.0]
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

/*! \file ParametersForSolver.h
 *
 * Parameters to be able to use LanczosSolver and ChebyshevSolver together
 *
 * Note: This is a struct: Add no functions except ctor/dtor!
 */

#ifndef PARAMETERS_FOR_SOLVER_H
#define PARAMETERS_FOR_SOLVER_H
#include "ProgressIndicator.h"
#include "TridiagonalMatrix.h"
#include "Vector.h"
#include "Matrix.h"
#include "Random48.h"
#include "TypeToString.h"

namespace PsimagLite {

template<typename RealType_>
struct ParametersForSolver {

	typedef RealType_ RealType;

	static const SizeType MaxLanczosSteps = 1000000; // max number of internal Lanczos steps
	static const SizeType LanczosSteps = 200; // max number of external Lanczos steps

	ParametersForSolver()
	    : steps(LanczosSteps),minSteps(4),tolerance(1e-12),stepsForEnergyConvergence(MaxLanczosSteps),
	      options(""),oneOverA(0),b(0),Eg(0),weight(0),isign(0),lotaMemory(false),
	      threadId(0)
	{}

	template<typename IoInputType>
	ParametersForSolver(IoInputType& io,String prefix)
	    : steps(LanczosSteps),minSteps(4),tolerance(1e-12),stepsForEnergyConvergence(MaxLanczosSteps),
	      options(""),oneOverA(0),b(0),Eg(0),weight(0),isign(0),lotaMemory(false),
	      threadId(0)
	{
		try {
			io.readline(steps,prefix + "Steps=");
		} catch (std::exception&) {}

		try {
			io.readline(minSteps,prefix + "MinSteps=");
		} catch (std::exception&) {}

		try {
			io.readline(tolerance,prefix + "Eps=");
		} catch (std::exception&) {}

		try {
			io.readline(stepsForEnergyConvergence,prefix + "StepsForEnergyConvergence=");
		} catch (std::exception&) {}

		try {
			io.readline(options,prefix + "Options=");
		} catch (std::exception&) {
			options = "none";
			io.rewind();
		}

		try {
			io.readline(oneOverA,prefix + "OneOverA=");
		} catch (std::exception&) {}

		try {
			io.readline(b,prefix + "B=");
		} catch (std::exception&) {}

		try {
			io.readline(Eg,prefix + "Energy=");
		} catch (std::exception&) {}

		try {
			int x = 0;
			io.readline(x,prefix + "SaveLanczosVectors=");
			lotaMemory = (x > 0);
		} catch (std::exception&) {}
	}

	SizeType steps;
	SizeType minSteps;
	RealType tolerance;
	SizeType stepsForEnergyConvergence;
	String options;
	RealType oneOverA,b;
	RealType Eg;
	RealType weight;
	int isign;
	bool lotaMemory;
	SizeType threadId;
}; // class ParametersForSolver
} // namespace PsimagLite

/*@}*/
#endif //PARAMETERS_FOR_SOLVER_H

