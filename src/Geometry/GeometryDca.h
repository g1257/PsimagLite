/*
Copyright (c) 2014, UT-Battelle, LLC
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

#ifndef LANCZOS_GEOMETRY_DCA_H
#define LANCZOS_GEOMETRY_DCA_H
#include "Io/IoNg.h"
#include "Vector.h"

namespace PsimagLite
{

template <typename RealType_, typename GeometryType_>
class GeometryDca
{

public:

	typedef GeometryType_ GeometryType;
	typedef RealType_ RealType;

	GeometryDca(const GeometryType& g, SizeType orbitals)
	    : enabled_(false)
	    , message_(g.label(0) + " with " + ttos(orbitals))
	{
		if (g.label(0) == "star" && orbitals == 4)
			enabled_ = true;
	}

	SizeType kSum(SizeType k1, SizeType k2) const
	{
		if (!enabled_)
			printErrorAndDie("kSum");
		return (k1 ^ k2);
	}

	SizeType kSustract(SizeType k1, SizeType k2) const
	{
		if (!enabled_)
			printErrorAndDie("kSustract");
		return (k1 ^ k2);
	}

	void write(String label1, IoNg::Out::Serializer& io) const
	{
		String label = label1 + "/GeometryDca";
		io.createGroup(label);
		io.write(label + "/enabled_", enabled_);
		io.write(label + "/message_", message_);
	}

private:

	void printErrorAndDie(String func) const
	{
		String str("GeometryDca:: Only valid for 2x2 clusters\n");
		str += "Not for " + message_ + " orbitals.\n";
		str += "\tWhile calling function " + func + "\n";
		throw RuntimeError(str);
	}

	bool enabled_;
	String message_;
}; // class GeometryDca

} // namespace PsimagLite

#endif
