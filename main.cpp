#include <iostream>
#include <string>
#include <regex>

using namespace std;


string readLine()
{
    string line;
    regex regexNumberPlatePattern("[A-Z][A-Z0-9]{2,10}");

    // when we find sought times we check whether they are ok
    regex regexTimePattern("(0?[8-9].[0-5][0-9])|(1[0-9].[0-5][0-9])|(20.00)");
    smatch numberPlateMatch;
    while (getline(cin, line))
    {
        regex_search(line, numberPlateMatch, regexTimePattern);
        
        for(auto x : numberPlateMatch)
            cout << "Found " << x << " ";
    }

}



int main()
{
    readLine();

    return 0;
}