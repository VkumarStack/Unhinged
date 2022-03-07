#include "MatchMaker.h"
#include "PersonProfile.h"
#include "utility.h"
#include <set>
#include <algorithm>

MatchMaker::MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at)
{
	m_database = &mdb;
	m_translator = &at;
}

std::vector<EmailCount> MatchMaker::IdentifyRankedMatches(std::string email, int threshold) const
{
	std::vector<EmailCount> ret;

	const PersonProfile* profile = m_database->GetMemberByEmail(email); // First, find the PersonProfile associated with the email
	if (profile == nullptr) // If the profile is not found, return the empty vector 
		return ret;

	std::set<AttValPair> compatiblePairs; // Set of all Compatible AttValPairs given the Profile's Source AttValPairs
	for (int i = 0; i < profile->GetNumAttValPairs(); i++) // Iterate through each of the Profile's Source AttValPairs, translate into Compatible AttValPairs, and insert into the set 
	{
		AttValPair pair;
		profile->GetAttVal(i, pair);
		std::vector<AttValPair> pairVec = m_translator->FindCompatibleAttValPairs(pair); // Get a vector of Compatible AttValPairs from the current Source AttValPair
		for (int i = 0; i < pairVec.size(); i++)
			compatiblePairs.insert(pairVec[i]); // Insert into the compatiblePairs set; since this is a set, it is guaranteed that they are unique
	}

	std::map<std::string, int> matches; // Map of emails to the number of Compatible AttValPairs for that given email

	std::set<AttValPair>::iterator it;
	it = compatiblePairs.begin();
	while (it != compatiblePairs.end()) // Iterate through the set of Compatible AttValPairs
	{
		std::vector<std::string> matchesVec = m_database->FindMatchingMembers(*it); // Find all emails that have the Compatible AttValPair
		for (int i = 0; i < matchesVec.size(); i++) // Iterate through the vector of emails 
		{
			if (matchesVec[i] == profile->GetEmail()) // A person cannot self-match
				continue;
			std::map<std::string, int>::iterator t;
			t = matches.find(matchesVec[i]); // Check if the email is already in the map 
			if (t == matches.end())
				matches.insert(std::pair<std::string, int>(matchesVec[i], 1)); // If not, insert the email into the Map with a value of 1 (since they are compatible with 1 so far) 
			else
				t->second++; // Otherwise, increment the compatibility number
		}
		it++;
	}

	std::map<std::string, int>::iterator mit = matches.begin(); // Insert all matches in the map greater than or equal to the threshold into the vector to be returned
	while (mit != matches.end())
	{
		if (mit->second >= threshold)
			ret.push_back(EmailCount(mit->first, mit->second));
		mit++;
	}
	
	std::sort(ret.rbegin(), ret.rend()); // Sort this vector in reverse (as defined by the comparison operator)
	return ret; // Return it
}

bool operator<(const EmailCount& lhs, const EmailCount& rhs)
{
	if (lhs.count == rhs.count)
	{
		return lhs.email > rhs.email;
	}
	else
	{
		return lhs.count < rhs.count;
	}
}