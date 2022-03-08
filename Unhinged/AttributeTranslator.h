#ifndef ATTRIBUTETRANSLATOR_H
#define ATTRIBUTETRANSLATOR_H

#include <string>
#include <vector>
#include <set>
#include "RadixTree.h"
#include "provided.h"


class AttributeTranslator
{
	public:
		AttributeTranslator() {}
		~AttributeTranslator() {}
		bool Load(std::string filename);
		std::vector<AttValPair> FindCompatibleAttValPairs(const AttValPair& source) const;
	private:
		RadixTree<std::set<AttValPair>> m_tree; // Radix Tree of a concatenated AttValPair mapping to a set of other AttValPair objects 
};

#endif
