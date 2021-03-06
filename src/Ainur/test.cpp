#include "Ainur.h"
#include <vector>
#include <complex>

void partiallyReadSomething(const PsimagLite::Ainur& ainur)
{
	SizeType n = 0;
	ainur.readValue(n, "TotalNumberOfSites");
	std::cout<<"Read: TotalNumberOfSites="<<n<<"\n";

	std::vector<std::complex<double> > v;
	//std::vector<double> v;
	ainur.readValue(v, "myv");
	std::cout<<v;

	PsimagLite::String m;
	ainur.readValue(m, "Model");
	std::cout<<m<<"\n";
}

int main(int argc, char** argv)
{
	if (argc == 1) return 1;

	PsimagLite::String str;
	for (int i = 1; i < argc; ++i) {
		std::ifstream fin(argv[i]);
		PsimagLite::String str2;

		fin.seekg(0, std::ios::end);
		str2.reserve(fin.tellg());
		fin.seekg(0, std::ios::beg);

		str2.assign((std::istreambuf_iterator<char>(fin)),
		           std::istreambuf_iterator<char>());
		fin.close();

		str += str2;
	}


	PsimagLite::Ainur ainur(str);
	// ainur.printAll(std::cout);
	partiallyReadSomething(ainur);

}
