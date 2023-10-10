#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>

using namespace std;
using time_type = pair<int, int>;
using umap_t = unordered_map<string, time_type>;

const size_t MAX_NUBMER_OF_STRING_INPUTS = 3;
const int ERROR = -1;
const int END = 1;

namespace
{
    bool nonEmptyLine(string const &line)
    {
        return !line.empty();
    }
}


int newDataRead(string const &line, string *arr)
{
    static regex const threeArgPattern(
		"\\s*([A-Z][A-Z0-9]{2,10})\\s+(\\d\\d?\\.\\d\\d)\\s+((\\d\\d?\\.\\d\\d))\\s*");
    smatch match;
    if(!regex_match(line, match, threeArgPattern))
    {
        static regex const twoArgPattern(
		"\\s*([A-Z][A-Z0-9]{2,10})\\s+(\\d\\d?\\.\\d\\d)\\s*");
        if(!regex_match(line, match, twoArgPattern))
            return ERROR;
        
        arr[0] = match[1].str();
        arr[1] = match[2].str();
        arr[2] = "";
    }
    else
    {
        arr[0] = match[1].str();
        arr[1] = match[2].str();
        arr[2] = match[3].str();
    }
    
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

bool correctTimeOfStay(time_type const &timeStart, time_type const &timeEnd)
{
    int minuteDifference;
    
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

int calcDiffBetweenTimes(time_type t, time_type tSubstract)
{
    return (t.first - tSubstract.first) * 60 +
                           t.second - tSubstract.second;
}

int readLine(string const &line, string &plates,
             string &time1, string &time2)
{
    string arr[MAX_NUBMER_OF_STRING_INPUTS];
    size_t sizeArr = MAX_NUBMER_OF_STRING_INPUTS;   

    if (nonEmptyLine(line))
    {
        int returnCode = newDataRead(line, arr);
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
            for (int i = 0; i < MAX_NUBMER_OF_STRING_INPUTS; i++)
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

bool isParkingPaid(umap_t &map, time_type time)
{
    return true;
}

// input: plates time1 time2 OR plates time
int mainLoop()
{
    string line, plates, time1Str, time2Str;
    umap_t mapOfPlatesAndTimes;
    size_t nbrOfCurrentLine = 0;
    time_type prevTime;
    int returnCode;
    
    prevTime.first = 8;
    prevTime.second = 0;

    while (getline(cin, line))
    {
        time1Str = time2Str = "";
        nbrOfCurrentLine++;
        returnCode = readLine(line, plates, time1Str, time2Str);

        if (returnCode == ERROR)
            cerr << "ERROR " << nbrOfCurrentLine << '\n';
        else if (returnCode == END)
        {
            cout << "END\n";
            break;
        }
        else
        {
            time_type time1 = convertTime(time1Str);
            if (time2Str.empty())
            {
                if(isParkingPaid(mapOfPlatesAndTimes, time1))
                    cout << "YES " << nbrOfCurrentLine << '\n';
                else
                    cout << "NO " << nbrOfCurrentLine << '\n';
            }
            else
            {
                time_type time2 = convertTime(time2Str);
                if (correctTimeOfStay(time1, time2))
                {
                    addToHashMap(mapOfPlatesAndTimes, plates, time2);
                    cout << "OK " << nbrOfCurrentLine << '\n';
                }
                else
                {
                    cout << "TIME OF STAY LESS THAN 10 min\n";
                    cerr << "ERROR " << nbrOfCurrentLine << '\n';
                }
            }
            prevTime = time1;
        }
    }

    return 0;
}

int main()
{
    // readLine(); 
    //mainLoop();
    return 0;
}