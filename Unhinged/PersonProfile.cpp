#include "PersonProfile.h"
#include "provided.h"
using namespace std;

PersonProfile::PersonProfile(string name, string email)
{
	m_numPairs = 0;
	m_name = name;
	m_email = email;
}

void PersonProfile::AddAttValPair(const AttValPair& attval)
{
	// Determine if the Attribute is in the RadixTree
	set<string>* result = m_tree.search(attval.attribute); // O(1) since attribute length can be assumed constant 
	if (result == nullptr) // If not, create a new set of strings representing values, insert attval's value into the set, and insert the set into the RadixTree  
	{
		set<string> val = set<string>();
		val.insert(attval.value); // O(1) since the set is previously empty
		m_tree.insert(attval.attribute, val); // O(1)
		m_numPairs++;
		// Also insert the pair into the vector 
		m_pairs.push_back(AttValPair(attval.attribute, attval.value));  // O(1)
	}
	else
	{
		// If the Attribute is in the RadixTree, insert the Value into its set if it is not already there  
		if (result->find(attval.value) == result->end()) // O(1)
		{
			result->insert(attval.value); // O(log(n))
			m_numPairs++;
			m_pairs.push_back(AttValPair(attval.attribute, attval.value)); // O(1)
		}
	}
	// Overall time complexity: O(log(n))
}

bool PersonProfile::GetAttVal(int attribute_num, AttValPair& attval) const
{
	// Check if the attribute_num is valid 
	if (attribute_num >= 0 && attribute_num < m_numPairs)
	{
		// Simply retrieve it 
		attval = m_pairs[attribute_num]; // O(1)
		return true;
	}
	return false;
	// Overall time complexity: O(1)
}