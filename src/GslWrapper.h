/*
Copyright (c) 2011-2013, UT-Battelle, LLC
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

/*! \file GslWrapper.h
 *
 *  Wrapper for GSL functions and types
 */

#ifndef GSL_WRAPPER_H_
#define GSL_WRAPPER_H_

#ifdef USE_GSL
#include <gsl/gsl_errno.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_sf_expint.h>
#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_sf_result.h>
#endif

#include "Vector.h"
#include <stdexcept>

namespace PsimagLite
{

#ifndef USE_GSL
class GslWrapper
{
public:

	typedef int DummyType;
	typedef DummyType gsl_integration_workspace;
	typedef double (*GslWrapperFunctionType)(double, void*);
	typedef struct {
		double val;
		double err;
	} gsl_sf_result;

	typedef void (*gsl_error_handler_t)(const char*, const char*, int, int);

	struct gsl_function {
		GslWrapperFunctionType function;
		void* params;
	};

	gsl_error_handler_t gsl_set_error_handler(gsl_error_handler_t) const
	{

		thereSnoGsl();
		gsl_error_handler_t* fx = new gsl_error_handler_t();

		return *fx;
	}

	gsl_integration_workspace*
	gsl_integration_workspace_alloc(SizeType) const
	{
		thereSnoGsl();
		int* x = new int;
		return x;
	}

	void gsl_integration_workspace_free(gsl_integration_workspace*) const
	{
		thereSnoGsl();
	}

	int gsl_integration_qag(const gsl_function*, double, double, double, double, size_t, int, gsl_integration_workspace*, double*, double*) const
	{
		thereSnoGsl();
		return 0;
	}

	int gsl_integration_qagiu(gsl_function*, double, double, double, size_t, gsl_integration_workspace*, double*, double*) const
	{
		thereSnoGsl();
		return 0;
	}

	int gsl_integration_qagp(const gsl_function*, double*, SizeType, double, double, SizeType, gsl_integration_workspace*, double*, double*) const
	{
		thereSnoGsl();
		return 0;
	}

	int gsl_integration_qagi(gsl_function*, double, double, size_t, gsl_integration_workspace*, double*, double*) const
	{
		thereSnoGsl();
		return 0;
	}

	void printError(int) const { thereSnoGsl(); }

	int gsl_sf_lngamma_complex_e(double, double, gsl_sf_result*, gsl_sf_result*) const
	{
		thereSnoGsl();
		return 0;
	}

	int gsl_sf_Ci_e(double, gsl_sf_result*)
	{
		thereSnoGsl();
		return 0;
	}

private:

	void thereSnoGsl() const
	{
		throw RuntimeError("You need to compile with the GSL\n");
	}

}; // class GslWrapper

#else

class GslWrapper
{
public:

	typedef ::gsl_integration_workspace gsl_integration_workspace;
	typedef ::gsl_function gsl_function;
	typedef ::gsl_sf_result gsl_sf_result;

	void printError(int status) const
	{
		std::cerr << "GslWrapper: error: " << gsl_strerror(status)
			  << "\n";
	}

	gsl_error_handler_t*
	gsl_set_error_handler(gsl_error_handler_t* new_handler) const
	{
		return ::gsl_set_error_handler(new_handler);
	}

	gsl_integration_workspace*
	gsl_integration_workspace_alloc(SizeType n) const
	{
		return ::gsl_integration_workspace_alloc(n);
	}

	void gsl_integration_workspace_free(gsl_integration_workspace* w) const
	{
		return ::gsl_integration_workspace_free(w);
	}

	int gsl_integration_qagi(gsl_function* f, double epsabs, double epsrel, size_t limit, gsl_integration_workspace* workspace, double* result, double* abserr) const
	{
		return ::gsl_integration_qagi(f, epsabs, epsrel, limit, workspace, result, abserr);
	}

	int gsl_integration_qagiu(gsl_function* f, double a, double epsabs, double epsrel, size_t limit, gsl_integration_workspace* workspace, double* result, double* abserr) const
	{
		return ::gsl_integration_qagiu(f, a, epsabs, epsrel, limit, workspace, result, abserr);
	}

	int gsl_integration_qagp(const gsl_function* f, double* pts, SizeType npts, double epsabs, double epsrel, SizeType limit, gsl_integration_workspace* workspace, double* result, double* abserr) const
	{
		return ::gsl_integration_qagp(f, pts, npts, epsabs, epsrel, limit, workspace, result, abserr);
	}

	int gsl_integration_qag(const gsl_function* f, double a, double b, double epsabs, double epsrel, size_t limit, int key, gsl_integration_workspace* workspace, double* result, double* abserr) const
	{
		return ::gsl_integration_qag(f, a, b, epsabs, epsrel, limit, key, workspace, result, abserr);
	}

	int gsl_sf_lngamma_complex_e(double zr, double zi, gsl_sf_result* lnr, gsl_sf_result* arg) const
	{
		return ::gsl_sf_lngamma_complex_e(zr, zi, lnr, arg);
	}

	int gsl_sf_Ci_e(double x, gsl_sf_result* result) const
	{
		return ::gsl_sf_Ci_e(x, result);
	}

}; // class GslWrapper

#endif

} // namespace PsimagLite

/*@}*/
#endif // GSL_WRAPPER_H_
