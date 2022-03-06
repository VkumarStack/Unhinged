#include "MemberDatabase.h"
#include "PersonProfile.h"
#include <fstream>
#include <sstream>

MemberDatabase::~MemberDatabase()
{
	for (int i = 0; i < m_emails.size(); i++)
	{
		delete (*m_profileTree.search(m_emails[i]));
	}
}

bool MemberDatabase::LoadDatabase(std::string filename)
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
			std::string name = line;
			std::getline(myfile, line);
			std::string email = line;
			std::getline(myfile, line);
			PersonProfile* prof = new PersonProfile(name, email);
			PersonProfile** search = m_profileTree.search(email);
			if (search != nullptr)
			{
				delete prof;
				return false;
			}

			m_profileTree.insert(email, prof);
			m_emails.push_back(email);

			int atts = std::stoi(line);
			std::getline(myfile, line);
			for (int i = 0; i < atts; i++)
			{
				std::stringstream strm(line);
				std::string att;
				std::string val;
				std::getline(strm, att, ',');
				std::getline(strm, val, ',');
				AttValPair pair = AttValPair(att, val);
				std::set<std::string>* setEmails = m_emailTree.search(att + " " + val);
				if (setEmails == nullptr)
				{
					std::set<std::string> emails;
					emails.insert(email);
					m_emailTree.insert(att + " " + val, emails);
				}
				else
					setEmails->insert(email);
				
				(*m_profileTree.search(email))->AddAttValPair(pair);
				std::getline(myfile, line);
			}
		}
	}
	return true;
}

std::vector<std::string> MemberDatabase::FindMatchingMembers(const AttValPair& input) const
{
	std::vector<std::string> ret;
	std::set<std::string>* result = m_emailTree.search(input.attribute + " " + input.value);
	if (result != nullptr)
	{
		std::set<std::string>::const_iterator it;
		it = result->begin();
		while (it != result->end())
		{
			ret.push_back(*it);
			it++;
		}
	}
	return ret;
}

const PersonProfile* MemberDatabase::GetMemberByEmail(std::string email) const
{
	return *m_profileTree.search(email);
}