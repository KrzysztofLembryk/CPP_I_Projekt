#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>

using namespace std;
using time_type = int;
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

    if (!regex_match(line, match, threeArgPattern))
    {
        static regex const twoArgPattern(
            "\\s*([A-Z][A-Z0-9]{2,10})\\s+(\\d\\d?\\.\\d\\d)\\s*");
        if (!regex_match(line, match, twoArgPattern))
        {
            static regex const end("\\s*(zzz)\\s*");
            if (!regex_match(line, match, end))
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
    if (!regex_match(inputStr, match, allowedTimePattern))
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

time_type convertTime(string const &time)
{
    int hours = convertHours(time);
    int minutes = convertMinutes(time);
    return hours * 60 + minutes;
}

bool correctTimeOfStay(int timeStart, int timeEnd)
{
    int minuteDifference;

    if (timeStart <= timeEnd)
        minuteDifference = timeEnd - timeStart;
    else
        minuteDifference = 20 * 60 - timeStart + timeEnd - 8 * 60;

    return minuteDifference >= 10;
}

int readLine(string const &line, string &plates,
             string &time1, string &time2)
{
    string arr[MAX_NUBMER_OF_STRING_INPUTS];
    // size_t sizeArr = MAX_NUBMER_OF_STRING_INPUTS;

    if (nonEmptyLine(line))
    {
        int returnCode = newDataRead(line, arr);
        if (returnCode == ERROR)
            return ERROR;
        else if (returnCode == END)
            return END;
        else
        {
            if (correctTimePattern(arr[1]) == ERROR || correctTimePattern(arr[2]) == ERROR)
                return ERROR;
            else
            {
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

void checkWhereAndAddNewPlates(string const &plates, time_type timeStart,
                               time_type timeEnd, umap_t &map1, umap_t &map2)
{
    if (timeStart < timeEnd)
    {
        if (map1.empty() || !map1.contains(plates))
            map1.insert({plates, timeEnd});
        else if (map1.at(plates) < timeEnd)
            map1.at(plates) = timeEnd;
    }
    else
    {
        cout << "t1 > t2\n";

            if (!map2.contains(plates))
                map2.insert({plates, timeEnd});
            else if (map2.at(plates) < timeEnd)
                map2.at(plates) = timeEnd;
        
        
        // timeStart > timeEnd so car can stay for whole current day
        // current day map is map1
        if(!map1.contains(plates))
            map1.insert({plates, 20 * 60});
        else
            map1.at(plates) = 20 * 60;

        
    }
}

void addToHashMap(string const &plates, time_type timeStart, time_type timeEnd,
                  umap_t &map1, umap_t &map2, bool newDay)
{
    if (!newDay)
    {
        cout << "day 1\n";
        checkWhereAndAddNewPlates(plates, timeStart, timeEnd, map1, map2);
    }
        
    else
        checkWhereAndAddNewPlates(plates, timeStart, timeEnd, map2, map1);
}

int isParkingPaid(string const &plates, time_type currentTime,
                  umap_t const &map1, umap_t const &map2, int currentLine)
{
    time_type endOfStayTime1, endOfStayTime2;
    endOfStayTime1 = endOfStayTime2 = 0;

    if (map1.contains(plates))
        endOfStayTime1 = map1.at(plates);
    if (map2.contains(plates))
        endOfStayTime2 = map2.at(plates);

    if (currentTime <= endOfStayTime1 || currentTime <= endOfStayTime2)
        cout << "YES " << currentLine << '\n';
    else
        cout << "NO " << currentLine << '\n';
    
    return 0;
}

// input: plates time1 time2 OR plates time
int mainLoop()
{
    string line, plates, time1Str, time2Str;
    umap_t platesTimesMAP1, platesTimesMAP2;
    size_t currentLine = 0;
    time_type prevTime;
    bool newDay = false;
    int returnCode;

    prevTime = 8 * 60;

    while (getline(cin, line))
    {
        currentLine++;
        returnCode = readLine(line, plates, time1Str, time2Str);

        if (returnCode == ERROR)
            cerr << "ERROR " << currentLine << '\n';
        else if (returnCode == END)
            break;
        else
        {
            time_type time1 = convertTime(time1Str);

            if (time1 < prevTime)
            {
                if(!newDay)
                    platesTimesMAP1.clear();
                else
                    platesTimesMAP2.clear();

                newDay = !newDay;
                // here we delete prev day hashmap
                // cause when XYZ t1 t2 and t2 < t1
                // we add this plate both to first and second hash map
                // so we wont lose the information about this plate
                // even though we deleted prev day hashmap
            }

            if (time2Str.empty())
            {
                if (!newDay)
                    returnCode = isParkingPaid(plates, time1, platesTimesMAP1,
                                               platesTimesMAP2, currentLine);
                else
                    returnCode = isParkingPaid(plates, time1, platesTimesMAP2,
                                               platesTimesMAP1, currentLine);

                if (returnCode == ERROR)
                    cerr << "ERROR " << currentLine << '\n';
            }
            else
            {
                cout << "before correct time of stay\n";
                time_type time2 = convertTime(time2Str);
                if (correctTimeOfStay(time1, time2))
                {
                    cout << "correct time of stay\n";
                    addToHashMap(plates, time1, time2, platesTimesMAP1,
                                     platesTimesMAP2, newDay);
                    cout << "OK " << currentLine << '\n';
                }
                else
                    cerr << "ERROR " << currentLine << '\n';
            }
            prevTime = time1;
        }
    }
    return 0;
}

int main()
{
    mainLoop();
    return 0;
}