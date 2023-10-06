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

    if(count != 3)
    {
        // ERROR: WRONG NUMBER OF DATA
    } 
    else
        return arr;
}

// BUG - RZE699 19.45 19.49, finds unnecessary match with 9 from plate number
int readTimePattern(string const &inputStr, string &result)
{
    regex timePattern("0?[8-9]\\.[0-5][0-9]|1[0-9]\\.[0-5][0-9]|20\\.00");
    smatch match;
    //string::const_iterator searchStart(line.cbegin());
    string times;
    //int i = 0;

    // regex_searach returns "" empty string when nothing found
    regex_search(inputStr, match, timePattern);
    result = match.str();
    
    if(result == "") // regex didnt find correct pattern
        return -1;

    return 0;
}

int readPlatePattern(string const &inputStr, string &result)
{
    regex plateNumberPattern("[A-Z][A-Z0-9]{2,10}");
    smatch match;

    regex_search(inputStr, match, plateNumberPattern);

    result = match.str();

    if(result == "")
        return -1;

    return 0;
}

void readLine() //(umap_t & mapp)
{
    string time1, time2;
    string plates;
    string line;

    getline(cin, line);

    if (nonEmptyLine(line))
    {
        readPlatePattern(line, plates);
        readTimePattern(line, time1);
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