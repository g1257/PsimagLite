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
// END LICENSE BLOCK
/** \ingroup PsimagLite */
/*@{*/

/*! \file Pthreads .h
 *
 *  A C++ pthreads class that implements the Concurrency interface
 *
 */
#ifndef PTHREADS_HEADER_H
#define PTHREADS_HEADER_H

#include <pthread.h>
#include <iostream>
#include "AllocatorCpu.h"
#include <sched.h>

template<typename PthreadFunctionHolderType>
struct PthreadFunctionStruct {
	PthreadFunctionStruct()
	    : pfh(0),threadNum(0),blockSize(0),total(0),mutex(0),cpu(0)
	{}

	PthreadFunctionHolderType* pfh;
	int threadNum;
	SizeType blockSize;
	SizeType total;
	pthread_mutex_t* mutex;
	SizeType cpu;
};

template<typename PthreadFunctionHolderType>
void *thread_function_wrapper(void *dummyPtr)
{
	PthreadFunctionStruct<PthreadFunctionHolderType> *pfs =
	        (PthreadFunctionStruct<PthreadFunctionHolderType> *) dummyPtr;

	PthreadFunctionHolderType *pfh = pfs->pfh;

	int s = sched_getcpu();
	if (s >= 0) pfs->cpu = s;

	pfh->thread_function_(pfs->threadNum,pfs->blockSize,pfs->total,pfs->mutex);

	return 0;
}

namespace PsimagLite {
template<typename PthreadFunctionHolderType>
class Pthreads  {

public:

	Pthreads(SizeType npthreads,int = 0)
	    : nthreads_(npthreads)
	{}

	void loopCreate(SizeType total,PthreadFunctionHolderType& pfh)
	{
		PthreadFunctionStruct<PthreadFunctionHolderType>* pfs;
		pfs = new PthreadFunctionStruct<PthreadFunctionHolderType>[nthreads_];
		pthread_mutex_init(&(mutex_), NULL);
		pthread_t* thread_id = new pthread_t[nthreads_];

		for (SizeType j=0; j <nthreads_; j++) {
			int ret=0;
			pfs[j].threadNum = j;
			pfs[j].pfh = &pfh;
			pfs[j].total = total;
			pfs[j].blockSize = total/nthreads_;
			if (total%nthreads_!=0) pfs[j].blockSize++;
			pfs[j].mutex = &mutex_;
			if ((ret=pthread_create(&thread_id[j],
			                        NULL,
			                        thread_function_wrapper<PthreadFunctionHolderType>,
			                        &pfs[j])))
				std::cerr<<"Thread creation failed: "<<ret<<"\n";
		}

		for (SizeType j=0; j <nthreads_; j++) pthread_join( thread_id[j], NULL);

#ifndef NDEBUG
		for (SizeType j=0; j <nthreads_; j++) {
			std::cout<<"Pthreads: Pthread number "<<j<<" runs on core number ";
			std::cout<<pfs[j].cpu<<"\n";
		}
#endif

		pthread_mutex_destroy(&mutex_);
		delete [] thread_id;
		delete [] pfs;
	}

	String name() const { return "pthreads"; }

	SizeType threads() const { return nthreads_; }

	SizeType mpiProcs() const { return 1; }

private:

	SizeType nthreads_;
	pthread_mutex_t mutex_;

}; // Pthreads class

} // namespace Dmrg

/*@}*/
#endif

