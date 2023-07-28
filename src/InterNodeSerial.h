#ifndef INTER_NODE_SERIAL_H
#define INTER_NODE_SERIAL_H
#include "Mpi.h"
#include "Vector.h"

namespace PsimagLite
{

template <typename = int>
class InterNode
{

public:

	InterNode(MPI::CommType) { }

	template <typename SomeLambdaType>
	void parallelFor(SizeType start, SizeType end, const SomeLambdaType& lambda)
	{
		for (SizeType i = start; i < end; ++i)
			lambda(i, 0);
	}

	SizeType size() const { return 1; }

	String name() const { return "serial"; }
};
} // namespace PsimagLite
#endif // INTER_NODE_SERIAL_H
