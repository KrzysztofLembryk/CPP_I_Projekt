#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>

using namespace std;

using umap_t = unordered_map<string, pair<string, string>>;


namespace 
{
    bool nonEmptyLine(string const & line) 
    {
        return !line.empty();
    }
}

// BUG - RZE699 19.45 19.49, finds unnecessary match with 9 from plate number
pair<string, string> readTimePattern(string const & line)
{
    regex timePattern("0?[8-9].[0-5][0-9]|1[0-9].[0-5][0-9]|20.00");
    smatch match;
    string::const_iterator searchStart(line.cbegin());
    string times[2];
    int i = 0;
    // regex_searach returns "" empty string when nothing found
    while (regex_search(searchStart, line.cend(), match, timePattern) && i < 2)
    {
        times[i] = match.str();
        searchStart = match.suffix().first;
        i++;
    }

    pair<string, string> timesPair(times[0], times[1]);

    return timesPair;
}

string readPlatePattern(string const & line)
{
    regex plateNumberPattern("[A-Z][A-Z0-9]{2,10}");
    smatch match;
    // when we find sought times we check whether they are ok
    
    regex_search(line, match, plateNumberPattern);
    
    return match.str();
}

void readline() //(umap_t & mapp)
{
    pair<string, string> p;
    string plates;
    string line;
    getline(cin, line);
    if(nonEmptyLine(line))
    {
        plates = readPlatePattern(line);
        p = readTimePattern(line);
        cout << "plate number: " << plates << ", times: "
        << p.first << ", " << p.second << '\n';
    }
    

}

bool addToHashMap()
{
    umap_t mapOfPlatesAndTimes;
}



int main()
{
    readline();

    return 0;
}