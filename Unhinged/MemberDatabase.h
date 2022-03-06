#ifndef MEMBERDATABASE_H
#define MEMBERDATABASE_H

#include <string>
#include <vector>
#include <set>
#include "RadixTree.h"
#include "provided.h"

class PersonProfile;

class MemberDatabase
{
	public:
		MemberDatabase() {}
		~MemberDatabase();
		bool LoadDatabase(std::string filename);
		std::vector<std::string> FindMatchingMembers(const AttValPair& input) const;
		const PersonProfile* GetMemberByEmail(std::string email) const;
	private:
		RadixTree<std::set<std::string>> m_emailTree; // Attribute-Value Pairs -> Emails
		RadixTree<PersonProfile*> m_profileTree; // Emails -> Member Profiles
		std::vector<std::string> m_emails;
};

#endif
