/*
Copyright (c) 2014, UT-Battelle, LLC
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

/*! \file Star.h
 *
 *  DOC NEEDED FIXME
 */
#ifndef GEOMETRY_STAR_H
#define GEOMETRY_STAR_H
#include "GeometryBase.h"

namespace PsimagLite {

template<typename ComplexOrRealType, typename InputType>
class Star : public GeometryBase<ComplexOrRealType, InputType> {

public:

	enum { DIRECTION_S};

	Star() {}

	Star(SizeType linSize,InputType&)
	    : linSize_(linSize)
	{}

	template<class Archive>
	void write(Archive & ar, const unsigned int)
	{
		ar & boost::serialization::base_object<GeometryBase<ComplexOrRealType, InputType> >(*this);
		ar & linSize_;
	}

	virtual SizeType maxConnections() const
	{
		return linSize_ - 1;
	}

	virtual SizeType dirs() const { return 1; }

	SizeType handle(SizeType i,SizeType j) const
	{
		return (i<j) ? j-1 : i-1;
	}

	SizeType getVectorSize(SizeType dirId) const
	{
		if (dirId!=DIRECTION_S) throw RuntimeError("Star must have direction 0\n");
		return linSize_-1;
	}

	bool connected(SizeType i1,SizeType i2) const
	{
		if (i1 == i2) return false;
		return (i1 == 0 || i2 == 0);
	}

	// assumes i1 and i2 are connected
	SizeType calcDir(SizeType,SizeType) const
	{
		return DIRECTION_S;
	}

	bool fringe(SizeType i,SizeType,SizeType emin) const
	{
		return (i == emin);
	}

	// siteNew2 is fringe in the environment
	SizeType getSubstituteSite(SizeType,SizeType,SizeType siteNew2) const
	{
		return siteNew2;
	}

	String label() const
	{
		return "star";
	}

	SizeType findReflection(SizeType) const
	{
		throw RuntimeError("findReflection\n");
	}

	SizeType length(SizeType i) const
	{
		assert(i==0);
		return linSize_;
	}

	SizeType translate(SizeType,SizeType dir,SizeType) const
	{
		assert(dir==0);

		throw RuntimeError("translate\n");
	}

private:

	SizeType linSize_;
}; // class Star
} // namespace PsimagLite

/*@}*/
#endif // GEOMETRY_STAR_H

