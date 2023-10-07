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

int manuallyReadData(string const &line, string *arr, size_t sizeOfArr)
{
    size_t i, j, count, n;
    bool foundWord = false;
    // string line = _line + " "; // temporary solution
    n = line.size();
    i = j = count = 0;

    // if there are only two strings on input, we dont want to have
    // in arr[2] random values
    for(size_t k = 0; k < sizeOfArr; k++)
        arr[k] = "";

    while (j < n && count <= 3)
    {
        if (isspace(line[j]))
        {
            // skip whitespaces
            if (i == j)
                i++;
            else if (foundWord)
            {
                foundWord = false;

                if (count < sizeOfArr)
                    arr[count] = line.substr(i, j - i);

                count++;

                if (count > 3)
                    return -1;

                i = j + 1;
            }
        }
        else
            foundWord = true;

        j++;
    }
    // case when last, second/third word is present but is not followed by whitespace
    // thus we wont go to else if(foundWord) and we wont copy this string to arr
    if (count <= 2 && !isspace(line[j - 1]))
    {
        arr[count] = line.substr(i, j - i);
        count++;
    }
    if (count < 2) // too few strings on input
        return -1;

    return 0;
}

//  0 - inputStr has good format
//  1 - inputStr is empty
// -1 - inputStr is not in time format
// -2 - inputStr is not in allowed by us time format
int readTimePattern(string const &inputStr, string &result)
{
    smatch match;
    regex timePattern("0?[0-9]\\.[0-5][0-9]|1[0-9]\\.[0-5][0-9]|2[0-3]\\.[0-5][0-9]");
    regex allowedTimePattern("0?[8-9]\\.[0-5][0-9]|1[0-9]\\.[0-5][0-9]|20\\.00");

    if(inputStr == "")
        return 1;
    
    // regex won't find that 14.444 is incorrect    
    if(inputStr.size() != 4 && inputStr.size() != 5)
        return -1;

    // first we check whether given string has correct time format 
    regex_search(inputStr, match, timePattern);
    result = match.str();

    // regex_search returns "" empty string when nothing found
    if(result == "")
        return -1;
    
    // if string has correct time pattern, we check whether it has time pattern
    // allowed by our rules 
    regex_search(inputStr, match, allowedTimePattern);
    result = match.str();

    if (result == "") 
        return -2;

    return 0;
}

int readPlatePattern(string const &inputStr, string &result)
{
    regex plateNumberPattern("[A-Z][A-Z0-9]{2,10}");
    smatch match;

    regex_search(inputStr, match, plateNumberPattern);

    result = match.str();

    if (result == "")
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

    if (nonEmptyLine(line))
    {
        if (manuallyReadData(line, arr, sizeArr) == -1)
        {
            cout << "manuallyReadData - WRONG INPUT DATA \n";
        }
        else
        {
            for (int i = 0; i < 3; i++)
                cout << arr[i] << ", ";

            cout << '\n';
            if(readPlatePattern(arr[0], plates) == -1)
            {
                cout << "WRONG PLATE PATTERN \n";
            }
            else
                cout << "plate: " << plates << '\n';
            
            if(readTimePattern(arr[1], time1) == -1 || readTimePattern(arr[2], time2) == -1)
            {
                cout << "WRONG TIME PATTERN \n";
            }
            else
            {
                cout << "time1: " << time1 << '\n';
                cout << "time2: " << time2 << '\n';
            }
            
        }
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