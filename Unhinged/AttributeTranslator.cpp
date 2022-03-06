#include <fstream>
#include <sstream>
#include "AttributeTranslator.h"
#include "utility.h"

bool AttributeTranslator::Load(std::string filename)
{
	std::ifstream myfile;
	myfile.open(filename);
	if (!myfile)
		return false;
	
	std::string line; 
	while (std::getline(myfile, line))
	{
		if (line != "")
		{
			std::stringstream strm(line);
			std::string pairs[4];
			int pos = 0;
			while (std::getline(strm, pairs[pos], ',') && pos < 4)
			{
				pos++;
			}

			std::set<AttValPair>* result = m_tree.search(pairs[0] + " " + pairs[1]);
			if (result == nullptr)
			{
				std::set<AttValPair> ins;
				ins.insert(AttValPair(pairs[2], pairs[3]));
				m_tree.insert(pairs[0] + " " + pairs[1], ins);
			}
			else
				result->insert(AttValPair(pairs[2], pairs[3]));
		}
	}
	return true;
}

std::vector<AttValPair> AttributeTranslator::FindCompatibleAttValPairs(const AttValPair& source) const
{
	std::vector<AttValPair> ret; 
	std::set<AttValPair>* result = m_tree.search(source.attribute + " " + source.value);
	if (result != nullptr)
	{
		std::set<AttValPair>::const_iterator it;
		it = result->begin();
		while (it != result->end())
		{
			ret.push_back(AttValPair(it->attribute, it->value));
			it++;
		}
	}
	return ret;
}