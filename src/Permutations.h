/*
Copyright (c) 2009-2012, UT-Battelle, LLC
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
/** \ingroup DMRG */
/*@{*/

/*! \file Permutations.h
 *
 * Raw computations for a free Hubbard model
 *
 */
#ifndef PERMUTATIONS_H_H
#define PERMUTATIONS_H_H

#include "Complex.h" // in PsimagLite
#include "Sort.h"

namespace PsimagLite
{

template <typename ContainerType>
class Permutations
{
	typedef typename ContainerType::value_type FieldType;

public:

	typedef FieldType value_type;

	Permutations(const ContainerType& orig)
	    : data_(orig.size())
	{
		for (SizeType i = 0; i < data_.size(); i++)
			data_[i] = orig[i];
		Sort<typename Vector<SizeType>::Type> mysort;
		typename Vector<SizeType>::Type iperm(data_.size());
		mysort.sort(data_, iperm);
	}

	/*1. Find the largest index k such that a[k] < a[k + 1].
	 * If no such index exists, the permutation is the last permutation.
	 *
	   2. Find the largest index l such that a[k] < a[l]. Since k + 1
	   is such an index, l is well defined and satisfies k < l.

	   3. Swap a[k] with a[l].

	   4. Reverse the sequence from a[k + 1] up to and including the
	   final element a[n-1].
	   */
	bool increase()
	{
		if (data_.size() == 0)
			return false;
		int k = largestk();
		if (k < 0)
			return false;

		int l = largestl(k);
		std::swap(data_[k], data_[l]);
		SizeType c = data_.size() - 1;
		if (SizeType(k) + 1 >= data_.size() - 1)
			return true;
		typename Vector<SizeType>::Type tmp = data_;
		for (SizeType i = k + 1; i < data_.size(); i++)
			data_[c--] = tmp[i];
		return true;
	}

	SizeType operator[](SizeType i) const
	{
		// if (i>=data_.size()) throw RuntimeError("Permutations
		// error\n");
		return data_[i];
	}

	SizeType size() const { return data_.size(); }

private:

	int largestk() const
	{
		int saved = -1;
		SizeType tot = data_.size() - 1;
		for (SizeType i = 0; i < tot; i++) {
			if (data_[i] < data_[i + 1])
				saved = i;
		}
		return saved;
	}

	SizeType largestl(SizeType k) const
	{
		SizeType saved = 0;
		for (SizeType i = 0; i < data_.size(); i++) {
			if (data_[k] < data_[i])
				saved = i;
		}
		return saved;
	}
	typename Vector<SizeType>::Type data_;

}; // Permutations

template <typename T>
std::ostream& operator<<(std::ostream& os, const Permutations<T>& ig)
{
	for (SizeType i = 0; i < ig.size(); i++)
		os << ig[i] << " ";
	return os;
}
} // namespace PsimagLite

/*@}*/
#endif // PERMUTATIONS_H_H
