/*
Copyright (c) 2009-2013, UT-Battelle, LLC
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

/*! \file LanczosOrDavidsonBase.h
 *
 *  Virtual class to select lanczos or davidson
 *
 */

#ifndef LANCZOS_OR_DAVIDSON_BASE_H
#define LANCZOS_OR_DAVIDSON_BASE_H
#include <cassert>

namespace PsimagLite
{

template <typename SolverParametersType, typename MatrixType_, typename VectorType>
class LanczosOrDavidsonBase
{

public:

	typedef typename SolverParametersType::RealType RealType;
	typedef typename Vector<RealType>::Type VectorRealType;
	typedef typename Vector<VectorType>::Type VectorVectorType;
	typedef MatrixType_ MatrixType;

	// To avoid compiler warnings
	virtual ~LanczosOrDavidsonBase() { }

	virtual void computeOneState(RealType&, VectorType&, const VectorType&, SizeType) = 0;

	virtual void computeAllStatesBelow(VectorRealType&, VectorVectorType&, const VectorType&, SizeType) = 0;

	static bool isReorthoEnabled(const SolverParametersType& params)
	{
		if (params.options.find("reortho") == PsimagLite::String::npos)
			return false;

		bool canReortho = (params.lotaMemory);

		if (!canReortho) {
			PsimagLite::String str("LanczosOrDavidsonBase: Reortho "
					       "requested but cannot");
			str += "Suggestion: Delete reortho from input or set "
			       "lotaMemory=true\n";
			throw PsimagLite::RuntimeError(str);
		}

		return true;
	}

}; // class LanczosOrDavidsonBase

} // namespace PsimagLite

/*@}*/
#endif // LANCZOS_OR_DAVIDSON_BASE_H
