#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>

using namespace std;

using umap_t = unordered_map<string, pair<string, string>>;

namespace
{
    bool nonEmptyLine(string const &line)
    {
        return !line.empty();
    }
}

string *manuallyReadData(string const &line)
{
    size_t i, j, count, n;
    string arr[3];
    bool foundWord = false;

    n = line.size();
    i = j = count = 0;

    while (j < n && count <= 3)
    {
        if (isspace(line[j]))
        {
            // skip whitespaces
            if (i == j)
                i++;
            else if(foundWord)
            {
                foundWord = false;
                // if (count < 3)
                //      copying to arr[count] string: i,..., j - 1
                count++;
                i = j + 1;
            }
        }
        else
            foundWord = true;

        j++;
    }

    if(count > 3 || count < 3)
    {
        // ERROR: WRONG NUMBER OF DATA
    } 
    else
        return arr;
}

// BUG - RZE699 19.45 19.49, finds unnecessary match with 9 from plate number
pair<string, string> readTimePattern(string const &line)
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

string readPlatePattern(string const &line)
{
    regex plateNumberPattern("[A-Z][A-Z0-9]{2,10}");
    smatch match;

    regex_search(line, match, plateNumberPattern);

    return match.str();
}

void readLine() //(umap_t & mapp)
{
    pair<string, string> p;
    string plates;
    string line;
    getline(cin, line);
    if (nonEmptyLine(line))
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
    readLine();

    return 0;
}