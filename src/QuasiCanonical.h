#ifndef QUASICANONICAL_H
#define QUASICANONICAL_H
#include "Vector.h"
#include "PsimagLite.h"
#include "CmplxOrReal.h"

namespace PsimagLite {

// level one parens only
// TODO FIXME : Generalize to multiple levels
template<typename ComplexOrRealType>
class QuasiCanonical {

public:

	typedef Vector<String>::Type VectorStringType;
	typedef Vector<bool>::Type VectorBoolType;
	typedef typename Vector<ComplexOrRealType>::Type VectorType;
	typedef typename Real<ComplexOrRealType>::Type RealType;
	typedef std::complex<RealType> ComplexType;

	QuasiCanonical(String str)
	    : str_(str)
	{
		const SizeType len = str_.length();
		String tempBuffer;
		String status = "closed";
		for (SizeType i = 0; i < len; ++i) {
			if (str_[i] == '(') {
				if (status == "open")
					throw RuntimeError("Too many parens levels (one only supported for now)\n");
				status = "open";
				continue;
			}

			if (str_[i] == ')') {
				if (status == "closed")
					throw RuntimeError("Unbalanced parens, closed\n");
				status = "closed";
				mainBuffer_ += "@" + ttos(ats_.size()) + "@";
				ats_.push_back(tempBuffer);
				tempBuffer = "";
				continue;
			}

			if (status == "closed") {
				mainBuffer_ += str_[i];
			} else {
				tempBuffer += str_[i];
			}
		}

		if (status == "open")
			throw RuntimeError("Unbalanced parens, open\n");

		split(terms_, mainBuffer_, "+");
		cachedValues_.resize(ats_.size());
		cachedAvailable_.resize(ats_.size(), false);
	}

	SizeType numberOfTerms() const { return terms_.size(); }

	const String& term(SizeType i) const
	{
		assert(i < terms_.size());
		return terms_[i];
	}

	int scalarIndex(String str) const
	{
		const SizeType len = str.length();
		if (len < 3) return -1;
		if (str[0] != '@' || str[len - 1] != '@')
			return -1;

		String snumber = str.substr(1, len - 2);
		SizeType number = PsimagLite::atoi(snumber);
		if (number >= ats_.size())
			return -1;
		return number;
	}

	ComplexOrRealType scalarFromIndex(SizeType ind) const
	{
		assert(ind < cachedAvailable_.size());
		assert(ind < cachedValues_.size());

		if (cachedAvailable_[ind])
			return cachedValues_[ind];

		String content = ats_[ind];
		VectorStringType terms;
		split(terms, content, "+");
		ComplexOrRealType sum = 0;
		const SizeType n = terms.size();
		for (SizeType i = 0; i < n; ++i) {
			sum += pureRealOrPureImag(terms[i]);
		}

		assert(ind < cachedValues_.size());
		cachedValues_[ind] = sum;
		cachedAvailable_[ind] = true;
		return sum;
	}

	static bool isPureComplex(String t)
	{
		if (t == "i") return true;

		const SizeType n = t.length();
		if (n < 2) return false;
		String tmp = t.substr(0, n - 1);
		return isRealScalar(tmp);
	}

	static ComplexOrRealType pureComplex(String t)
	{
		static const bool isComplex = IsComplexNumber<ComplexOrRealType>::True;
		if (!isComplex)
			err("i = sqrt(-1) found in code path that is real\n");

		CpmlxOrReal<RealType, (isComplex) ? 1 : 0> cmplxOrReal(t);
		return cmplxOrReal.value();
	}

	static bool isRealScalar(String termStr)
	{
		const SizeType n = termStr.length();
		if (n == 0)
			err("CanonicalExpression: term must not be empty\n");

		for (SizeType i = 0; i < n; ++i) {
			char c = termStr[i];
			bool isDigit = (c >= '0' && c <= '9');
			if (c == '.' || c == '-' || c == '+' || isDigit)
				continue;
			return false;
		}

		return true;
	}

private:

	static ComplexOrRealType pureRealOrPureImag(String t)
	{
		static const bool isComplex = IsComplexNumber<ComplexOrRealType>::True;
		CpmlxOrReal<RealType, (isComplex) ? 1 : 0> cmplxOrReal(t);
		return cmplxOrReal.value();
	}

	String str_;
	String mainBuffer_;
	VectorStringType ats_;
	VectorStringType terms_;
	mutable VectorType cachedValues_;
	mutable VectorBoolType cachedAvailable_;
};
}
#endif // QUASICANONICAL_H
