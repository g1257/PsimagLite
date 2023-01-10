#ifndef _AINUR_SPIRIT_H_
#define _AINUR_SPIRIT_H_
#include "../Vector.h"
#include "../TypeToString.h"
#include "../PsimagLite.h"
#include <iostream>
#include <string>
#include <fstream>
#include "AinurState.h"
#include "AinurLexical.h"

namespace PsimagLite {

class Ainur {

	struct Action {

		Action(String name, AinurState& state)
		    : name_(name), state_(state)
		{}

		template <typename A, typename ContextType>
		void operator()(A& attr,
		                ContextType& context,
		                bool& hit) const;

	private:

		String name_;
		AinurState& state_;
	};

	struct Action3 {

		Action3(String name, AinurState& state)
		    : name_(name), state_(state)
		{}

		template <typename A, typename ContextType>
		void operator()(A& attr,
		                ContextType& context,
		                bool& hit) const;

	private:

		String name_;
		AinurState& state_;
	};

	struct myprint
	{
		template <typename T>
		void operator()(const T &t) const
		{
			std::cout << " --------> " << t << '\n';
		}
	};

public:

	typedef std::string::iterator IteratorType;
	typedef Vector<char>::Type VectorCharType;
	typedef AinurState::VectorStringType VectorStringType;

	Ainur(String str);

	String& prefix() { return dummy_; }

	const String& prefix() const { return dummy_; }

	void printUnused(std::ostream& os) const
	{
		state_.printUnused(os);
	}

	void printAll(std::ostream& os) const
	{
		state_.printAll(os);
	}

	template<typename SomeType>
	void readValue(SomeType& t, String label) const
	{
		state_.readValue(t, dummy_ + label);
	}

	template<typename SomeMapType>
	void setMap(SomeMapType& map) const
	{
		state_.setMap(map);
	}

private:

	void initMacros()
	{
		state_.initMacros();
	}

	static bool allEmpty(IteratorType first,IteratorType last)
	{
		for (IteratorType it = first; it != last; ++it) {
			if (AinurLexical::isEmptyChar(*it)) continue;
			return false;
		}

		return true;
	}

	String dummy_;
	AinurState state_;
}; // class AinurSpirit

}
#endif // _AINUR_SPIRIT_H_
