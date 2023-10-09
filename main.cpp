#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>

using namespace std;

using umap_t = unordered_map<string, pair<string, string>>;
using time_type = pair<int, int>;

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

int readTimePattern(string const &inputStr, string &result)
{
    smatch match;
    regex allowedTimePattern("0?[8-9]\\.[0-5][0-9]|1[0-9]\\.[0-5][0-9]|20\\.00");

    if (inputStr.empty())
        return 0;

    // regex_search returns "" empty string when nothing found
    if(!regex_match(inputStr, match, allowedTimePattern))
        return ERROR;
    
    result = match.str();

    //if (result.empty())
    //    return ERROR;

    return 0;
}

int readPlatePattern(string const &inputStr, string &result)
{
    regex plateNumberPattern("[A-Z][A-Z0-9]{2,10}");
    smatch match;

    if(!regex_match(inputStr, match, plateNumberPattern))
        return ERROR;

    result = match.str();

    //if (result.empty())
    //    return ERROR;

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

time_type convertTime(string const &time)
{
    time_type p;
    p.first = convertHours(time);
    p.second = convertMinutes(time);
    return p;
}

bool correctTimeOfStay(string const &time1, string const &time2)
{
    time_type timeStart, timeEnd;
    int minuteDifference;
    timeStart = convertTime(time1);
    timeEnd = convertTime(time2);
    
    if (timeStart.first <= timeEnd.first && timeStart.second < timeEnd.second)
    {
        minuteDifference = (timeEnd.first - timeStart.first) * 60 +
                           timeEnd.second - timeStart.second;

        cout << "min diff: " << minuteDifference << '\n';
    }
    else
    {
        minuteDifference = (20 - timeStart.first) * 60 - timeStart.second +
                           (timeEnd.first - 8) * 60 + timeEnd.second;

        cout << "min diff: " << minuteDifference << '\n';
    }
    return minuteDifference >= 10;
}

int readLine(string const &line, string &plates,
             string &time1, string &time2)
{
    string arr[3];
    size_t sizeArr = 3;

    if (nonEmptyLine(line))
    {
        int returnCode = manuallyReadData(line, arr, sizeArr);
        if (returnCode == ERROR)
        {
            cout << "manuallyReadData - WRONG INPUT DATA \n";
            return ERROR;
        }
        else if (returnCode == END)
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

int addToHashMap(umap_t &map, string const &plates, time_type const &time)
{
    return 0;
}

int checkIfParkingPaid(umap_t &map, time_type time)
{
    return 0;
}

// input: plates time1 time2 OR plates time
int mainLoop()
{
    string line, plates, time1Str, time2Str;
    time_type time2;
    umap_t mapOfPlatesAndTimes;
    int returnCode;
    size_t nbrOfCurrentLine = 0;

    while (getline(cin, line))
    {
        nbrOfCurrentLine++;
        returnCode = readLine(line, plates, time1Str, time2Str);

        if (returnCode == ERROR)
            cerr << "BLAD";
        else if (returnCode == END)
        {
            cout << "END\n";
            break;
        }
        else
        {
            // time2Str empty means we check if for given plates
            // parking is paid
            if (time2Str.empty())
            {
                checkIfParkingPaid(mapOfPlatesAndTimes, convertTime(time1Str));
            }
            else
            {
                if (correctTimeOfStay(time1Str, time2Str))
                {
                    addToHashMap(mapOfPlatesAndTimes, plates, convertTime(time2Str));
                }
            }
        }
    }

    return 0;
}

int main()
{
    // readLine();

    return 0;
}