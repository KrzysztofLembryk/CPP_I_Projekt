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
        {
            static regex const end("\\s*(zzz)\\s*");
            if(!regex_match(line, match, end))
                return ERROR;
            return END;
        }
        
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


int correctTimePattern(string const &inputStr)
{
    smatch match;
    regex allowedTimePattern("0?[8-9]\\.[0-5][0-9]|1[0-9]\\.[0-5][0-9]|20\\.00");

    if (inputStr.empty())
        return 0;

    // regex_search returns "" empty string when nothing found
    if(!regex_match(inputStr, match, allowedTimePattern))
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

int convertTime(string const &time)
{
    int hours = convertHours(time);
    int minutes = convertMinutes(time);
    return hours * 60 + minutes;
}

bool correctTimeOfStay(int timeStart, int timeEnd)
{
    int minuteDifference;
    
    if (timeStart <= timeEnd)
    {
        minuteDifference = timeStart - timeEnd;

        cout << "min diff: " << minuteDifference << '\n';
    }
    else
    {
        minuteDifference = 20 * 60 - timeStart  + timeEnd - 8 * 60;

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
            cout << "WRONG INPUT DATA \n";
            return ERROR;
        }
        else if (returnCode == END)
        {
            cout << "End of programme\n";
            return END;
        }
        else
        {
            cout << "read strings: ";
            for (int i = 0; i < MAX_NUBMER_OF_STRING_INPUTS; i++)
                cout << arr[i] << ", ";
            cout << '\n';

            if (correctTimePattern(arr[1]) == ERROR || correctTimePattern(arr[2]) == ERROR)
            {
                cout << "WRONG TIME PATTERN \n";
                return ERROR;
            }
            else
            {
                cout << "time1: " << time1 << '\n';
                cout << "time2: " << time2 << '\n';
                plates = arr[0];
                time1 = arr[1];
                time2 = arr[2];
            }
        }
    }
    else
        return ERROR;
    return 0;
}

int addToHashMap(umap_t &map, string const &plates, int time)
{
    return 0;
}

bool isParkingPaid(umap_t &map, int time)
{
    return true;
}

// input: plates time1 time2 OR plates time
int mainLoop()
{
    string line, plates, time1Str, time2Str;
    umap_t mapOfPlatesAndTimes;
    size_t nbrOfCurrentLine = 0;
    int prevTime;
    int returnCode;
    
    prevTime = 8 * 60;

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
            int time1 = convertTime(time1Str);
            if (time2Str.empty())
            {
                if(isParkingPaid(mapOfPlatesAndTimes, time1))
                    cout << "YES " << nbrOfCurrentLine << '\n';
                else
                    cout << "NO " << nbrOfCurrentLine << '\n';
            }
            else
            {
                int time2 = convertTime(time2Str);
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