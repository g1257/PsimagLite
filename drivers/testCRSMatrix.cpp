#include "SampleCRSMatrix.h"
#include <cstdlib>
#include <fstream>
using namespace PsimagLite;
typedef double RealType;

template<typename T>
std::ostream& operator<<(std::ostream& os,const typename Vector<T>::Type& v)
{
	os<<v.size()<<"\n";
	for (SizeType i=0;i<v.size();i++)
		os<<v[i]<<" ";
	os<<"\n";
	return os;
}

void fillRandomVector(Vector<RealType>::Type& x,RealType maxValue)
{
	unsigned int long seed = 7334211;
	srand48(seed);
	for (SizeType i=0;i<x.size();i++)
		x[i] = drand48()*maxValue;
}

template<typename T>
void testMultiply(const SampleCRSMatrix<T>& m,RealType maxValue)
{
	typename Vector<RealType>::Type x(m.rank(),0.0),y(m.rank());
	fillRandomVector(y,maxValue);
	std::cout<<"initial vector:\n";
	std::cout<<y;
	m.matrixVectorProduct(x,y);
	std::cout<<"final vector:\n";
	std::cout<<x;
}

int main(int argc,char *argv[])
{

	if (argc==3) {
		SizeType rank = std::atoi(argv[1]);
		unsigned int long seed = 343981;
		RealType ratio = std::atof(argv[2]);
		SizeType nonZeros = SizeType(ratio * rank *rank);
		RealType maxValue = 10.0;
		SampleCRSMatrix<RealType> m(rank,seed,nonZeros,maxValue);
		std::cout<<m;

		testMultiply(m,maxValue);
	} else if (argc==2) {
		std::ifstream fin(argv[1]);
		SampleCRSMatrix<RealType> m(fin);
		fin.close();
		std::cout<<m;

		RealType maxValue = 10.0;
		testMultiply(m,maxValue);
	} else {
		throw RuntimeError("Wrong number of arguments\n");
	}


}

