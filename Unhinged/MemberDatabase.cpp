#include "MemberDatabase.h"
#include "PersonProfile.h"
#include <fstream>
#include <sstream>
#include <iostream>

MemberDatabase::~MemberDatabase()
{
	for (int i = 0; i < m_emails.size(); i++)
		delete (*m_profileTree.search(m_emails[i])); // Search for PersonProfile pointer and then delete its dynamically allocated memory
}

bool MemberDatabase::LoadDatabase(std::string filename)
{
	std::ifstream myfile;
	myfile.open(filename);
	if (!myfile)
		return false;

	std::string line;
	while (std::getline(myfile, line)) // Parse through text file 
	{
		if (line != "")
		{
			std::string name = line; // In a properly formatted file, the first line has the name
			std::getline(myfile, line); // Go to the next line
			std::string email = line; // This line has the email address
			std::getline(myfile, line); // Go to the next line 
			
			PersonProfile** search = m_profileTree.search(email); // Check if the email exists in the database already 
			if (search != nullptr) // If it does, return false
			{
				myfile.close();
				return false;
			}

			PersonProfile* prof = new PersonProfile(name, email); // Create a dynamically allocated PersonProfile with the given information
			m_profileTree.insert(email, prof); // Map email address to profile
			m_emails.push_back(email); // Push back email address to vector 

			int atts = std::stoi(line); // This line has the number of AttValPairs, which is converted to an int using stoi

			std::getline(myfile, line); // Go to next line, which starts at the list of AttValPairs 
			for (int i = 0; i < atts; i++) // The next atts lines have AttValPairs
			{
				std::stringstream strm(line);
				std::string att;
				std::string val;
				std::getline(strm, att, ',');
				std::getline(strm, val, ',');

				AttValPair pair = AttValPair(att, val);
				std::vector<std::string>* vecEmails = m_emailTree.search(att + " " + val); // Check if the concatenated AttValPair exists in the RadixTree already
				if (vecEmails == nullptr) // If it does not, create a vector of strings representing emails, insert the current email into the vector, and insert the vector into AttValPair to Emails tree
				{
					std::vector<std::string> emails;
					emails.push_back(email);
					m_emailTree.insert(att + " " + val, emails);
				}
				else // Otherwise, insert the email into the found vector of emails 
					vecEmails->push_back(email);
				
				prof->AddAttValPair(pair); // For the current PersonProfile, add the current AttValPair
				std::getline(myfile, line); // Go to next line
			}
		}
	}
	myfile.close();
	return true;
}

std::vector<std::string> MemberDatabase::FindMatchingMembers(const AttValPair& input) const
{
	std::vector<std::string> ret;
	// O(1) for searching through the RadixTree 
	std::vector<std::string>* result = m_emailTree.search(input.attribute + " " + input.value); // Get a vector of all matching members with the input AttValPair
	if (result != nullptr) // Transfer all string data from the vector into the return vector 
		ret = *result; // O(M), where M is the number of members with the attribute value pair
	return ret;
	// Overall time complexity: O(M)
}

const PersonProfile* MemberDatabase::GetMemberByEmail(std::string email) const
{
	// Search through the RadixTree to see if a pointer to a PersonProfile is found 
	PersonProfile** ret = m_profileTree.search(email); // O(1) since searching through a RadixTree
	if (ret == nullptr) // Return nullptr if it is not found 
		return nullptr; // Don't want to dereference a nullptr 
	else
		return *ret; // Otherwise return the PersonProfile pointer
	// Overall time complexity: O(1)
}