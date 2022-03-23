#include "InterNode.h"
#include "PsimagLite.h"

int main(int argc, char* argv[])
{
	PsimagLite::PsiApp psiApp("internode", &argc, &argv, 1);
	if (argc < 2) {
		std::cerr<<"USAGE "<<argv[0]<<" number\n";
		return 1;
	}

	const SizeType n = atoi(argv[1]);

	// original
	for (SizeType i = 0; i < n; ++i) {
		std::cout<<i;
	}

	std::cout<<"\n--------------------------\n";

	//lambda
	PsimagLite::InterNode<> internode(PsimagLite::MPI::COMM_WORLD);

	internode.parallelFor(0, n, [](SizeType i, SizeType){std::cout << i;});
	std::cout<<"\n--------------------------\n";
	size_t len = 1024;
	char* name = new char[len + 1];
	int x = gethostname(name, len);
	if (x != 0) std::cerr<<argv[0]<<": Could not gethostname\n";
	else std::cout<<name<<"\n";
	delete [] name;
	name = nullptr;
}
