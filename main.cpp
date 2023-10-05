#include <iostream>
#include <string>
#include <regex>

using namespace std;


string readLine()
{
    string line;
    regex regexNumberPlatePattern("[A-Z][A-Z0-9]{2,10}");

    // when we find sought times we check whether they are ok
    regex regexTimePattern("0?[8-9].[0-5][0-9]|1[0-9].[0-5][0-9]|20.00");
    smatch match;
    while (getline(cin, line))
    {
        while(regex_search(line, match, regexTimePattern))
        {
            cout << "Found: " << match.str() << '\n';
            line = match.suffix().str();
        }
    }

}



int main()
{
    readLine();

    return 0;
}