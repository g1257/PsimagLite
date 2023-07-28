#include "Vector.h"
#include <cstdlib>
#include <iostream>

extern "C" void dsyev_(char*, char*, int*, double*, int*, double*, double*, int*, int*);

int main()
{
	int n = 100;
	PsimagLite::Vector<double>::Type m(n * n);
	// fill "matrix"
	for (SizeType i = 0; i < SizeType(n * n); i++)
		m[i] = 5 * drand48();
	// symmetrize:
	for (SizeType i = 0; i < SizeType(n); i++)
		for (SizeType j = i + 1; j < SizeType(n); j++)
			m[i + j * n] = m[j + i * n];

	PsimagLite::Vector<double>::Type eigs(n);
	char jobz = 'V';
	char uplo = 'U';
	int lda = n;
	PsimagLite::Vector<double>::Type work(3);
	int info = 0;
	int lwork = -1;

	// query:
	dsyev_(&jobz, &uplo, &n, &(m[0]), &lda, &(eigs[0]), &(work[0]), &lwork, &info);
	if (info != 0) {
		std::cerr << "diag: dsyev_: failed with info=" << info << "\n";
		return 1;
	}
	lwork = int(work[0]) + 1;
	work.resize(lwork + 1);

	// real work:
	dsyev_(&jobz, &uplo, &n, &(m[0]), &lda, &(eigs[0]), &(work[0]), &lwork, &info);
	if (info != 0) {
		std::cerr << "diag: dsyev_: failed with info=" << info << "\n";
		return 1;
	}
}
