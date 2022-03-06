#ifndef MATCHMAKER_H
#define MATCHMAKER_H

#include "MemberDatabase.h"
#include "AttributeTranslator.h"
#include "provided.h"
#include <string>
#include <vector>

class MatchMaker
{
	public:
		MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at);
		~MatchMaker() {}
		std::vector<EmailCount> IdentifyRankedMatches(std::string email, int threshold) const;

	private:
		const MemberDatabase* m_database;
		const AttributeTranslator* m_translator;
};

#endif