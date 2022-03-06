#include "utility.h"

bool operator<(const AttValPair& lhs, const AttValPair& rhs)
{
	std::string str1 = lhs.attribute + " " + lhs.value;
	std::string str2 = rhs.attribute + " " + rhs.value;
	return (str1 < str2);
}