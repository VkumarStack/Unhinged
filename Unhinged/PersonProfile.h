#ifndef PERSONPROFILE_H
#define PERSONPROFILE_H
#include "RadixTree.h"
#include <vector>
#include <set>
#include <string>

struct AttValPair;

class PersonProfile 
{
	public:
		PersonProfile(std::string name, std::string email);
		~PersonProfile() {}
		std::string GetName() const { return m_name; }
		std::string GetEmail() const { return m_email; }
		void AddAttValPair(const AttValPair& attval);
		int GetNumAttValPairs() const { return m_numPairs; }
		bool GetAttVal(int attribute_num, AttValPair& attval); //const;
	private:
		std::string m_name;
		std::string m_email;
		// The same attribute can have multiple different values, and since the RadixTree only allows for SINGLE Key/Value pairs to be stored,
		// a RadixTree of a set of strings representing values for that specific attribute being mapped with
		RadixTree<std::set<std::string>> m_tree; 
		int m_numPairs;
		// Attribute-Value pairs are also directly stored in a vector in the case of needing to get an arbritary index via GetAttVal
		std::vector<AttValPair> m_pairs;
};

#endif