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
	while (std::getline(myfile, line)) // Parse through text file 
	{
		if (line != "") // Do nothing if line is empty 
		{
			std::stringstream strm(line);
			std::string pairs[4]; // Given that the file is formatted properly, there are a guaranteed 4 tokens separated by commas 
			int pos = 0;
			while (std::getline(strm, pairs[pos], ',') && pos < 4) // Look for item separated by commas and insert into pairs array
				pos++;

			std::set<AttValPair>* result = m_tree.search(pairs[0] + " " + pairs[1]); // Concatenate the SourceAttribute with the SourceValue and check if it is already in the Radix Tree
			if (result == nullptr) // If it is not in the tree already, create a new set of Compatible AttValPairs and insert it into the RadixTree with the concatenated Source AttValPair as the key
			{
				std::set<AttValPair> ins;
				ins.insert(AttValPair(pairs[2], pairs[3])); // Insert Compatible AttValPair into set
				m_tree.insert(pairs[0] + " " + pairs[1], ins); // Insert set with Source AttValPair as key 
			}
			else
				result->insert(AttValPair(pairs[2], pairs[3])); // Otherwise, the Source AttValPair exists in the tree, insert the compatible AttValPair into the set 
		}
	}
	myfile.close();
	return true;
}

std::vector<AttValPair> AttributeTranslator::FindCompatibleAttValPairs(const AttValPair& source) const
{
	std::vector<AttValPair> ret; 
	std::set<AttValPair>* result = m_tree.search(source.attribute + " " + source.value); // O(1) since RadixTree is O(k) where k is a maximum attribute length
	if (result != nullptr) // If the source is in the tree, iterate through its appropriate set and add them all to the vector 
	{
		std::set<AttValPair>::const_iterator it;
		it = result->begin(); 
		while (it != result->end()) // O(1) since result is assumed to contain a bounded number of Compatible Attribute pairs  
		{
			ret.push_back(AttValPair(it->attribute, it->value)); // O(1)
			it++;
		}
	}
	return ret;
	// Overall time complexity: O(1)
}