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
/** \ingroup PsimagLite */
/*@{*/

/*!
 *
 *
 */
#ifndef MEMORY_CPU_H
#define MEMORY_CPU_H

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace PsimagLite
{

class MemoryCpu
{

public:

	void deallocate(void* p)
	{
		assert(p);
		free(p);
		std::ostringstream msg;
		msg << "// Freed " << p;
		std::cout << msg.str() << "\n";
		p = 0;
	}

	void* allocate(size_t x)
	{
		void* p = malloc(x);
		std::ostringstream msg;
		msg << "// Allocated starting at " << p << " " << x << " bytes";
		std::cout << msg.str() << "\n";
		return p;
	}

}; // class MemoryCpu

MemoryCpu globalMemoryCpu;
} // namespace PsimagLite

/*@}*/
#endif // MEMORY_CPU_H
