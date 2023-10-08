#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>

using namespace std;

using umap_t = unordered_map<string, pair<string, string>>;

const int ERROR = -1;
const int END = 1;

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
    for (size_t k = 0; k < sizeOfArr; k++)
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
                    return ERROR;

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
    if (count == 1 && arr[0] == "zzz")
        return END;
    if (count < 2) // too few strings on input
        return ERROR;

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

    if (inputStr.empty())
        return 0;

    // regex won't find that 14.444 is incorrect
    if (inputStr.size() != 4 && inputStr.size() != 5)
        return ERROR;

    // first we check whether given string has correct time format
    // regex_search(inputStr, match, timePattern);
    // result = match.str();

    // regex_search returns "" empty string when nothing found
    // if (result.empty())
    //    return ERROR;

    // if string has correct time pattern, we check whether it has time pattern
    // allowed by our rules
    regex_search(inputStr, match, allowedTimePattern);
    result = match.str();

    if (result.empty())
        return ERROR;

    return 0;
}

int readPlatePattern(string const &inputStr, string &result)
{
    regex plateNumberPattern("[A-Z][A-Z0-9]{2,10}");
    smatch match;

    if (inputStr.size() > 11 || inputStr.size() < 3)
        return ERROR;

    regex_search(inputStr, match, plateNumberPattern);

    result = match.str();

    if (result.empty())
        return ERROR;

    return 0;
}

int convertHours(string const &time)
{
    if (time.size() == 5)
        return stoi(time.substr(0, 2));
    return stoi(time.substr(0, 1));
}

int convertMinutes(string const &time)
{
    if (time.size() == 5)
        return stoi(time.substr(3, 2));
    return stoi(time.substr(2, 2));
}

bool calcTimeOfStay(string const &time1, string const &time2)
{
    int hourStart, minuteStart, hourEnd, minuteEnd;
    int minuteDifference;

    hourStart = convertHours(time1);
    hourEnd = convertHours(time2);
    minuteStart = convertMinutes(time1);
    minuteEnd = convertMinutes(time2);

    if (hourStart <= hourEnd && minuteStart < minuteEnd)
    {
        minuteDifference = (hourEnd - hourStart) * 60 +
                           minuteEnd - minuteStart;

        cout << "min diff: " << minuteDifference << '\n';

        return minuteDifference >= 10;
    }
    else
    {
        minuteDifference = (20 - hourStart) * 60 - minuteStart +
                           (hourEnd - 8) * 60 + minuteEnd;

        cout << "min diff: " << minuteDifference << '\n';

        return minuteDifference >= 10;
    }
}

int readLine(string const &line, string &plates,
             string &time1, string &time2)
{
    string arr[3];
    size_t sizeArr = 3;

    if (nonEmptyLine(line))
    {
        if (manuallyReadData(line, arr, sizeArr) == ERROR)
        {
            cout << "manuallyReadData - WRONG INPUT DATA \n";
            return ERROR;
        }
        else if (manuallyReadData(line, arr, sizeArr) == END)
        {
            cout << "End of programme\n";
            return END;
        }
        else
        {
            for (int i = 0; i < 3; i++)
                cout << arr[i] << ", ";

            cout << '\n';
            if (readPlatePattern(arr[0], plates) == ERROR)
            {
                cout << "WRONG PLATE PATTERN \n";
            }
            else
                cout << "plate: " << plates << '\n';

            if (readTimePattern(arr[1], time1) == ERROR || readTimePattern(arr[2], time2) == ERROR)
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
    else
        return ERROR;
    return 0;
}

int mainLoop()
{
    string line, plates, time1, time2;
    umap_t mapOfPlatesAndTimes;

    while (getline(cin, line))
    {
        readLine(line, plates, time1, time2);
    }

    return 0;
}

int main()
{
    // readLine();

    return 0;
}