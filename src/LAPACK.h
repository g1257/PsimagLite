//-*-C++-*-
// ****************************************************************************
// * C++ wrapper for BLAS                                                     *
// *                                                                          *
// * Thomas Schulthess, ORNL, October 1999                                    *
// ****************************************************************************

#ifndef PSIMAG_LAPACK
#define PSIMAG_LAPACK

#include <complex>

/** \file LAPACK
 *  \author Thomas C. Schulthess, MSS
 */

namespace psimag {

/** \brief Namespace encapsulating psimag wrappers of LAPACK functions.
 */
namespace LAPACK {

// ============================================================================
extern "C" void sgesv_(int*,int*,float*,int*,int*,float*,int*,int*);

extern "C" void dgesv_(int*,int*,double*,int*,int*,double*,int*,int*);

extern "C" void cgesv_(int*,int*,std::complex<float>*,int*,int*,std::complex<float>*,
                       int*,int*);
extern "C" void zgesv_(int*,int*,std::complex<double>*,int*,int*,std::complex<double>*,
                       int*,int*);

//MSS
extern "C" int  dgetrf_(int*, int*, double*, int*, int*, int*);

extern "C" int  sgetrf_(int*, int*, float*, int*, int*, int*);

extern "C" int  zgetrf_(int*, int*, std::complex<double>*, int*, int*, int*);

extern "C" int  cgetrf_(int*, int*, std::complex<float>*, int*, int*, int*);

extern "C" int  zgetri_(int*,
                        std::complex<double>*,
                        int*,
                        int*,
                        std::complex<double>*,
                        int*,
                        int*);

extern "C" int  cgetri_(int*,
                        std::complex<float>*,
                        int*,
                        int*,
                        std::complex<float>*,
                        int*,
                        int*);

extern "C" int  dgetri_(int*, double*, int*, int*,  double*, int*, int*);

extern "C" int  sgetri_(int*, float*, int*, int*,  float*, int*, int*);

extern "C" int  dgesdd_(char* jobz,
                        int* m,
                        int* n,
                        double* a,
                        int* lda,
                        double* s,
                        double* u,
                        int* ldu,
                        double* vt,
                        int* ldvt,
                        double* work,
                        int* lwork,
                        int* iwork,
                        int* info);

extern "C" int  sgesdd_(char* jobz,
                        int* m,
                        int* n,
                        float* a,
                        int* lda,
                        float* s,
                        float* u,
                        int* ldu,
                        float* vt,
                        int* ldvt,
                        float* work,
                        int* lwork,
                        int* iwork,
                        int* info);

extern "C" int  zgesdd_(char* jobz,
                        int* m,
                        int* n,
                        std::complex<double>* a,
                        int* lda,
                        double* s,
                        std::complex<double>* u,
                        int* ldu,
                        std::complex<double>* vt,
                        int* ldvt,
                        std::complex<double>* work,
                        int* lwork,
                        double* rwork,
                        int* iwork,
                        int* info);

extern "C" int  cgesdd_(char* jobz,
                        int* m,
                        int* n,
                        std::complex<float>* a,
                        int* lda,
                        float* s,
                        std::complex<float>* u,
                        int* ldu,
                        std::complex<float>* vt,
                        int* ldvt,
                        std::complex<float>* work,
                        int* lwork,
                        float* rwork,
                        int* iwork,
                        int* info);

extern "C" int  dgesvd_(char* jobz,
                        char*,
                        int* m,
                        int* n,
                        double* a,
                        int* lda,
                        double* s,
                        double* u,
                        int* ldu,
                        double* vt,
                        int* ldvt,
                        double* work,
                        int* lwork,
                        int* info);

extern "C" int  sgesvd_(char* jobz,
                        char*,
                        int* m,
                        int* n,
                        float* a,
                        int* lda,
                        float* s,
                        float* u,
                        int* ldu,
                        float* vt,
                        int* ldvt,
                        float* work,
                        int* lwork,
                        int* info);

extern "C" int  zgesvd_(char* jobz,
                        char*,
                        int* m,
                        int* n,
                        std::complex<double>* a,
                        int* lda,
                        double* s,
                        std::complex<double>* u,
                        int* ldu,
                        std::complex<double>* vt,
                        int* ldvt,
                        std::complex<double>* work,
                        int* lwork,
                        double* rwork,
                        int* info);

extern "C" int  cgesvd_(char* jobz,
                        char*,
                        int* m,
                        int* n,
                        std::complex<float>* a,
                        int* lda,
                        float* s,
                        std::complex<float>* u,
                        int* ldu,
                        std::complex<float>* vt,
                        int* ldvt,
                        std::complex<float>* work,
                        int* lwork,
                        float* rwork,
                        int* info);

extern "C" void ilaver_(int*, int*, int*);

extern "C" void dstedc_(char*,
                        int*,
                        double*,
                        double*,
                        double*,
                        int*,
                        double*,
                        int*,
                        int*,
                        int*,
                        int*);

extern "C" void sstedc_(char*,
                        int*,
                        float*,
                        float*,
                        float*,
                        int*,
                        float*,
                        int*,
                        int*,
                        int*,
                        int*);

extern "C" void zstedc_(char*,
                        int*,
                        double*,
                        double*,
                        std::complex<double>*,
                        int*,
                        std::complex<double>*,
                        int*,
                        double*,
                        int*,
                        int*,
                        int*,
                        int*);

extern "C" void cstedc_(char*,
                        int*,
                        float*,
                        float*,
                        std::complex<float>*,
                        int*,
                        std::complex<float>*,
                        int*,
                        float*,
                        int*,
                        int*,
                        int*,
                        int*);

extern "C" void dsterf_(int*, double*, double*, int*);

extern "C" void ssterf_(int*, float*, float*, int*);

extern "C" void dsteqr_(char*, int*, double*, double*, double*, int*, double*, int*);

extern "C" void ssteqr_(char*, int*, float*, float*, float*, int*, float*, int*);

// ============================================================================

inline void STERF(int* n, double* d, double* e, int* info)
{
	dsterf_(n, d, e, info);
}

inline void STERF(int* n, float* d, float* e, int* info)
{
	ssterf_(n, d, e, info);
}

inline void STEQR(char jobz,
                  int n,
                  double* d,
                  double* e,
                  double *z,
                  int ldz,
                  double *work,
                  int *info)
{
	dsteqr_(&jobz, &n, d, e, z, &ldz, work, info);
}

inline void STEQR(char jobz,
                  int n,
                  float* d,
                  float* e,
                  float *z,
                  int ldz,
                  float *work,
                  int *info)
{
	ssteqr_(&jobz, &n, d, e, z, &ldz, work, info);
}

inline void GESV(int ma,
                 int mb,
                 float* a,
                 int lda,
                 int* pivot,
                 float* b,
                 int ldb,
                 int& info)
{
	sgesv_(&ma,&mb,a,&lda,pivot,b,&ldb,&info);
}

inline void GESV(int ma,
                 int mb,
                 double* a,
                 int lda,
                 int* pivot,
                 double* b,
                 int ldb,
                 int& info)
{
	dgesv_(&ma,&mb,a,&lda,pivot,b,&ldb,&info);
}

inline void GESV(int ma,
                 int mb,
                 std::complex<float>* a,
                 int lda,
                 int* pivot,
                 std::complex<float>* b,
                 int ldb,
                 int& info)
{
	cgesv_(&ma,&mb,a,&lda,pivot,b,&ldb,&info);
}

inline void GESV(int ma,
                 int mb,
                 std::complex<double>* a,
                 int lda,
                 int* pivot,
                 std::complex<double>* b,
                 int ldb,
                 int& info)
{
	zgesv_(&ma,&mb,a,&lda,pivot,b,&ldb,&info);
}

inline void GETRF(int ma, int na, double* a, int lda, int* pivot, int& info)
{
	dgetrf_(&ma,&na,a,&lda,pivot,&info);
}

inline void GETRF(int ma,int na,std::complex<double>* a,int lda,int* pivot,int& info)
{
	zgetrf_(&ma,&na,a,&lda,pivot,&info);
}

inline void GETRF(int ma, int na, float* a, int lda, int* pivot, int& info)
{
	sgetrf_(&ma,&na,a,&lda,pivot,&info);
}

inline void GETRF(int ma,int na,std::complex<float>* a,int lda,int* pivot,int& info)
{
	cgetrf_(&ma,&na,a,&lda,pivot,&info);
}

inline void GETRI(int na,
                  double* a,
                  int lda,
                  int* pivot,
                  double* work,
                  int lwork,
                  int& info)
{
	dgetri_(&na,a,&lda,pivot,work,&lwork,&info);
}

inline void GETRI(int na,
                  std::complex<double>* a,
                  int lda,
                  int* pivot,
                  std::complex<double>* work,
                  int lwork,
                  int& info)
{
	zgetri_(&na,a,&lda,pivot,work,&lwork,&info);
}

inline void GETRI(int na,
                  float* a,
                  int lda,
                  int* pivot,
                  float* work,
                  int lwork,
                  int& info)
{
	sgetri_(&na,a,&lda,pivot,work,&lwork,&info);
}

inline void GETRI(int na,
                  std::complex<float>* a,
                  int lda,
                  int* pivot,
                  std::complex<float>* work,
                  int lwork,
                  int& info)
{
	cgetri_(&na,a,&lda,pivot,work,&lwork,&info);
}


inline void GESDD(char* jobz,
                  int* m,
                  int* n,
                  double* a, // T*,
                  int* lda,
                  double* s,
                  double* u, //T*,
                  int* ldu,
                  double* vt, // T*,
                  int* ldvt,
                  double* work, // T*,
                  int* lwork,
                  double*, // nothing
                  int* iwork,
                  int *info)
{
	dgesdd_(jobz, m, n, a, lda, s, u, ldu, vt, ldvt, work, lwork, iwork, info);
}

inline void GESDD(char* jobz,
                  int* m,
                  int* n,
                  float* a, // T*,
                  int* lda,
                  float* s,
                  float* u, //T*,
                  int* ldu,
                  float* vt, // T*,
                  int* ldvt,
                  float* work, // T*,
                  int* lwork,
                  float*, // nothing
                  int* iwork,
                  int *info)
{
	sgesdd_(jobz, m, n, a, lda, s, u, ldu, vt, ldvt, work, lwork, iwork, info);
}

inline void GESDD(char* jobz,
                  int* m,
                  int* n,
                  std::complex<double>* a, // T*,
                  int* lda,
                  double* s,
                  std::complex<double>* u, //T*,
                  int* ldu,
                  std::complex<double>* vt, // T*,
                  int* ldvt,
                  std::complex<double>* work, // T*,
                  int* lwork,
                  double* rwork, // nothing
                  int* iwork,
                  int *info)
{
	zgesdd_(jobz, m, n, a, lda, s, u, ldu, vt, ldvt, work, lwork, rwork, iwork, info);
}


inline void GESDD(char* jobz,
                  int* m,
                  int* n,
                  std::complex<float>* a, // T*,
                  int* lda,
                  float* s,
                  std::complex<float>* u, //T*,
                  int* ldu,
                  std::complex<float>* vt, // T*,
                  int* ldvt,
                  std::complex<float>* work, // T*,
                  int* lwork,
                  float* rwork, // nothing
                  int* iwork,
                  int *info)
{
	cgesdd_(jobz, m, n, a, lda, s, u, ldu, vt, ldvt, work, lwork, rwork, iwork, info);
}

inline void GESVD(char* jobz,
                  char *jobvt,
                  int* m,
                  int* n,
                  double* a, // T*,
                  int* lda,
                  double* s,
                  double* u, //T*,
                  int* ldu,
                  double* vt, // T*,
                  int* ldvt,
                  double* work, // T*,
                  int* lwork,
                  double*, // nothing
                  int *info)
{
	dgesvd_(jobz, jobvt, m, n, a, lda, s, u, ldu, vt, ldvt, work, lwork, info);
}

inline void GESVD(char* jobz,
                  char *jobvt,
                  int* m,
                  int* n,
                  float* a, // T*,
                  int* lda,
                  float* s,
                  float* u, //T*,
                  int* ldu,
                  float* vt, // T*,
                  int* ldvt,
                  float* work, // T*,
                  int* lwork,
                  float*, // nothing
                  int *info)
{
	sgesvd_(jobz, jobvt, m, n, a, lda, s, u, ldu, vt, ldvt, work, lwork, info);
}

inline void GESVD(char* jobz,
                  char *jobvt,
                  int* m,
                  int* n,
                  std::complex<double>* a, // T*,
                  int* lda,
                  double* s,
                  std::complex<double>* u, //T*,
                  int* ldu,
                  std::complex<double>* vt, // T*,
                  int* ldvt,
                  std::complex<double>* work, // T*,
                  int* lwork,
                  double* rwork, // nothing
                  int *info)
{
	zgesvd_(jobz, jobvt, m, n, a, lda, s, u, ldu, vt, ldvt, work, lwork, rwork, info);
}


inline void GESVD(char* jobz,
                  char *jobvt,
                  int* m,
                  int* n,
                  std::complex<float>* a, // T*,
                  int* lda,
                  float* s,
                  std::complex<float>* u, //T*,
                  int* ldu,
                  std::complex<float>* vt, // T*,
                  int* ldvt,
                  std::complex<float>* work, // T*,
                  int* lwork,
                  float* rwork, // nothing
                  int *info)
{
	cgesvd_(jobz, jobvt, m, n, a, lda, s, u, ldu, vt, ldvt, work, lwork, rwork, info);
}

inline bool isThreadSafe()
{
	int major = 0;
	int minor = 0;
	int patch = 0;
	ilaver_(&major, &minor, &patch);
	if (major < 3) return false;
	return (minor >= 3);
}
}      /* namespace LAPACK */
}      /* namespace psimag */

#endif // PSIMAG_LAPACK_H
