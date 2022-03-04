//main.cpp
#include <iostream>
#include "provided.h"
#include "PersonProfile.h"
#include "RadixTree.h"
using namespace std;
int main()
{
    RadixTree<int> test = RadixTree<int>();
    test.insert("CANNOT", 1);
    if (test.search("CANNOT") != nullptr)
        cout << *(test.search("CANNOT")) << endl;
    test.insert("cannot", 2);
    if (test.search("cannot") != nullptr)
        cout << *(test.search("cannot")) << endl;
    test.insert("can", 2);
    test.insert("can", 30);
    if (test.search("can") != nullptr)
        cout << *(test.search("can")) << endl;
    *(test.search("can")) = 4;
    cout << *(test.search("can")) << endl;
    if (test.search("ca") == nullptr)
        cout << "NICE" << endl;
    test.insert("romane", 3);
    test.insert("romanus", 4);
    test.insert("romulus", 5);
    test.insert("rubens", 6);
    test.insert("ruber", 7);
    test.insert("rubicon", 8);
    test.insert("rubicundus", 9);
    test.insert("roman", 10);
    if (test.search("rubicundus") != nullptr)
        cout << "NICE" << endl;
    if (test.search("roman") != nullptr)
        cout << "NICE" << endl;
    if (test.search("CANNOT") != nullptr)
        cout << "NICE END" << endl;
    if (test.search("") == nullptr)
        cout << "EMPTY NOT FOUND" << endl;
    
    PersonProfile test3 = PersonProfile("Vivek", "vksogi@gmail.com");
    test3.AddAttValPair(AttValPair("hobbies", "screaming"));
    test3.AddAttValPair(AttValPair("hobbies", "crying"));
    test3.AddAttValPair(AttValPair("hobbies", "dying"));
    test3.AddAttValPair(AttValPair("favorite food", "sand"));

    for (int i = 0; i < test3.GetNumAttValPairs(); i++)
    {
        AttValPair av;
        test3.GetAttVal(i, av);
        std::cout << av.attribute << " -> " << av.value << std::endl;
    }
    test.insert("cannoter", 2);
    if (test.search("cannoter") != nullptr)
        cout << "cannoter" << endl;

}
/*
#include "PersonProfile.h"
#include "AttributeTranslator.h"
#include "MemberDatabase.h"
#include "MatchMaker.h"
#include "provided.h"
#include <iostream>
#include <string>
#include <vector>

const std::string MEMBERS_FILE    = "members.txt";
const std::string TRANSLATOR_FILE = "translator.txt";

bool findMatches(const MemberDatabase& mdb, const AttributeTranslator& at);

int main() {
    MemberDatabase mdb;
    if (!mdb.LoadDatabase(MEMBERS_FILE))
    {
        std::cout << "Error loading " << MEMBERS_FILE << std::endl;
        return 1;
    }
    AttributeTranslator at;
    if (!at.Load(TRANSLATOR_FILE))
    {
        std::cout << "Error loading " << TRANSLATOR_FILE << std::endl;
        return 1;
    }

    while (findMatches(mdb, at))
        ;

    std::cout << "Happy dating!" << std::endl;
}

bool findMatches(const MemberDatabase& mdb, const AttributeTranslator& at)
{
      // Prompt for email
    std::string email;
    const PersonProfile* pp;
    for (;;) {
        std::cout << "Enter the member's email for whom you want to find matches: ";
        std::getline(std::cin, email);
        if (email.empty())
            return false;
        pp = mdb.GetMemberByEmail(email);
        if (pp != nullptr)
            break;
        std::cout << "That email is not in the member database." << std::endl;
    }

      // Show member's attribute-value pairs
    std::cout << "The member has the following attributes:" << std::endl;
    for (int k = 0; k != pp->GetNumAttValPairs(); k++) {
        AttValPair av;
        pp->GetAttVal(k, av);
        std::cout << av.attribute << " --> " << av.value << std::endl;
    }

      // Prompt user for threshold
    int threshold;
    std::cout << "How many shared attributes must matches have? ";
    std::cin >> threshold;
    std::cin.ignore(10000, '\n');

      // Print matches and the number of matching translated attributes
    MatchMaker mm(mdb, at);
    std::vector<EmailCount> emails = mm.IdentifyRankedMatches(email, threshold);
    if (emails.empty())
        std::cout << "No member was a good enough match." << std::endl;
    else {
        std::cout << "The following members were good matches:" << std::endl;;
        for (const auto& emailCount : emails) {
            const PersonProfile* pp = mdb.GetMemberByEmail(emailCount.email);
            std::cout << pp->GetName() << " at " << emailCount.email << " with "
                      << emailCount.count << " matches!" << std::endl;
        }
    }
    std::cout << std::endl;
    return true;
}
*/