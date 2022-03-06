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

	const PersonProfile* profile = m_database->GetMemberByEmail(email);
	std::set<AttValPair> compatiblePairs;
	for (int i = 0; i < profile->GetNumAttValPairs(); i++)
	{
		AttValPair pair;
		profile->GetAttVal(i, pair);
		std::vector<AttValPair> pairVec = m_translator->FindCompatibleAttValPairs(pair);
		for (int i = 0; i < pairVec.size(); i++)
			compatiblePairs.insert(pairVec[i]);
	}

	std::map<std::string, int> matches;

	std::set<AttValPair>::iterator it;
	it = compatiblePairs.begin();
	while (it != compatiblePairs.end())
	{
		std::vector<std::string> matchesVec = m_database->FindMatchingMembers(*it);
		for (int i = 0; i < matchesVec.size(); i++)
		{
			if (matchesVec[i] == profile->GetEmail())
				continue;
			std::map<std::string, int>::iterator t;
			t = matches.find(matchesVec[i]);
			if (t == matches.end())
				matches.insert(std::pair<std::string, int>(matchesVec[i], 1));
			else
				t->second++;
		}
		it++;
	}

	std::map<std::string, int>::iterator mit = matches.begin();
	while (mit != matches.end())
	{
		if (mit->second >= threshold)
			ret.push_back(EmailCount(mit->first, mit->second));
		mit++;
	}
	
	std::sort(ret.rbegin(), ret.rend());
	return ret;
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