//main.cpp
#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>
#include "provided.h"
#include "PersonProfile.h"
#include "AttributeTranslator.h"
#include "RadixTree.h"
#include "MemberDatabase.h"
#include "MatchMaker.h"
using namespace std;

/*
int main()
{
    MemberDatabase database;
    if (database.LoadDatabase("membersShorter.txt"))
    {
        cout << "W" << endl;
        AttValPair test = AttValPair("trait", "amoral");
        vector<std::string> matches = database.FindMatchingMembers(test);
        for (int i = 0; i < matches.size(); i++)
            cout << matches.at(i) << endl;

        const PersonProfile* prof = database.GetMemberByEmail(matches[0]);
        cout << prof->GetEmail() << endl << prof->GetName() << endl << prof->GetNumAttValPairs() << endl;
        for (int i = 0; i < prof->GetNumAttValPairs(); i++)
        {
            AttValPair par;
            prof->GetAttVal(i, par);
            cout << par.attribute << " " << par.value << endl;
        }
    }
}*/

/*
int main()
{
    AttributeTranslator translator;
    if (translator.Load("translator.txt"))
    {
        cout << "W" << endl;
        vector<AttValPair> test = translator.FindCompatibleAttValPairs(AttValPair("trait", "disruptive"));
        for (int i = 0; i < test.size(); i++)
        {
            cout << test[i].attribute << " " << test[i].value << " " << endl;
        }
    }
}*/


/*int main()
{
    RadixTree<int> test = RadixTree<int>();
    test.insert("rubicundus", 1);
    test.insert("rubicon", 2);
    test.insert("ruber", 3);
    test.insert("rubens", 4);
    test.insert("romulus", 5);
    test.insert("romanus", 6);
    test.insert("roman", 8);
    test.insert("romane", 7);
    int* v;
    v = test.search("rubicundus");
    assert(v != nullptr && *v == 1);
    v = test.search("rubicon");
    assert(v != nullptr && *v == 2);
    v = test.search("ruber");
    assert(v != nullptr && *v == 3);
    v = test.search("rubens");
    assert(v != nullptr && *v == 4);
    v = test.search("romulus");
    assert(v != nullptr && *v == 5);
    v = test.search("romanus");
    assert(v != nullptr && *v == 6);
    v = test.search("romane");
    assert(v != nullptr && *v == 7);
    v = test.search("roman");
    assert(v != nullptr && *v == 8);
    
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

    RadixTree<int> test2 = RadixTree<int>();
    test2.insert("slowly", 5);
    test2.insert("slow", 4);
    test2.insert("toasting", 3);
    test2.insert("toaster", 2);
    test2.insert("test", 1);

}*/
/*
#include "PersonProfile.h"
#include "AttributeTranslator.h"
#include "MemberDatabase.h"
#include "MatchMaker.h"
#include "provided.h"
#include <iostream>
#include <string>
#include <vector>
*/

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
