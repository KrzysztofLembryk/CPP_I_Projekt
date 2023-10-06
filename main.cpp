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

int manuallyReadData(string const &_line, string* arr, size_t sizeOfArr)
{
    size_t i, j, count, n;
    bool foundWord = false;
    string line = _line + " "; // temporary solution
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

                if (count < sizeOfArr)
                    arr[count] = line.substr(i, j - i);
                
                count++;

                if(count > 3)
                    return -1;

                i = j + 1;
            }
        }
        else
            foundWord = true;

        j++;
    }

    return 0;
}

int readTimePattern(string const &inputStr, string &result)
{
    regex timePattern("0?[8-9]\\.[0-5][0-9]|1[0-9]\\.[0-5][0-9]|20\\.00");
    smatch match;

    // regex_search returns "" empty string when nothing found
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
    string arr[3];
    size_t sizeArr = 3;
    string time1, time2;
    string plates;
    string line;

    getline(cin, line);

    if(nonEmptyLine(line))
    {
        manuallyReadData(line, arr, sizeArr);
        for(int i = 0; i < 3; i++)
            cout << arr[i] << ", ";

        cout << '\n';
        readPlatePattern(arr[0], plates);
        cout << "plate: " << plates << '\n';
        readTimePattern(arr[1], time1);
        cout << "time1: " << time1 << '\n';
        readTimePattern(arr[2], time2);
        cout << "time2: " << time2 << '\n';
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