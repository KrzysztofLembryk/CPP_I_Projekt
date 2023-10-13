#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>
#include <queue>

using namespace std;
using time_type = int;
using umap_t = unordered_map<string, time_type>;
using pqueue_t = std::priority_queue<pair<time_type, string>>;

namespace
{
    const size_t MAX_NUBMER_OF_STRING_INPUTS = 3;
    const int ERROR = -1;

    bool nonEmptyLine(string const &line)
    {
        return !line.empty();
    }

    // Function gets input string of data, checks if data is in correct
    // format, if not it returns ERROR, otherwise it saves read data to
    // string array arr[].
    int checkPatternAndReadData(string const &line, string *arr)
    {
        string pattern1 = "\\s*([A-Z][A-Z0-9]{2,10})\\s+";
        string pattern2 = "(\\d\\d?\\.\\d\\d)\\s";
        static regex const threeArgPattern(pattern1 + pattern2 + "+" +
                                           pattern2 + "*");
        smatch match;

        if (!regex_match(line, match, threeArgPattern))
        {
            static regex const twoArgPattern(pattern1 + pattern2 + "*");

            if (!regex_match(line, match, twoArgPattern))
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

    int correctTimePattern(string const &inputStr)
    {
        smatch match;
        regex timePattern("0?[8-9]\\.[0-5][0-9]|1[0-9]\\.[0-5][0-9]|20\\.00");

        if (inputStr.empty())
            return 0;

        // regex_search returns "" empty string when nothing found
        if (!regex_match(inputStr, match, timePattern))
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

        return minuteDifference >= 10 && minuteDifference < 720;
    }

    // Function handles wrong input data, zzz command, and if data is correct
    // it is saved to plates, time1 and time2 variables.
    int readLine(string const &line, string &plates,
                 string &time1, string &time2)
    {
        string arr[MAX_NUBMER_OF_STRING_INPUTS];

        if (nonEmptyLine(line))
        {
            int returnCode = checkPatternAndReadData(line, arr);

            if (returnCode == ERROR)
                return ERROR;
            else
            {
                if (correctTimePattern(arr[1]) == ERROR ||
                    correctTimePattern(arr[2]) == ERROR)
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

    // Function checks to which map and queue new plates with new time of stay
    // should be added. If given plates already exist in one of the maps,
    // function updates time of stay if necessary. Function also handles
    // situation where car stays for another day.
    void checkWhereAndAddNewPlates(string const &plates, time_type timeStart,
                                   time_type timeEnd, umap_t &map1, umap_t &map2,
                                   pqueue_t &queue1, pqueue_t &queue2)
    {
        if (timeStart < timeEnd)
        {
            if (!map1.contains(plates))
            {
                map1.insert({plates, timeEnd});
                queue1.push({timeEnd, plates});
            }
            else if (map1.at(plates) < timeEnd)
            {
                map1.at(plates) = timeEnd;
                queue1.push({timeEnd, plates});
            }
        }
        else
        {
            if (!map2.contains(plates))
            {
                map2.insert({plates, timeEnd});
                queue2.push({timeEnd, plates});
            }
            else if (map2.at(plates) < timeEnd)
            {
                map2.at(plates) = timeEnd;
                queue2.push({timeEnd, plates});
            }

            // timeStart > timeEnd so car can stay for whole current day.
            // Current day map is map1.
            if (!map1.contains(plates))
                map1.insert({plates, 20 * 60});
            else
                map1.at(plates) = 20 * 60;
        }
    }

    void addToHashMap(string const &plates, time_type timeStart, time_type timeEnd,
                      umap_t &map1, umap_t &map2,
                      pqueue_t &queue1, pqueue_t &queue2,
                      bool newDay)
    {
        if (!newDay)
            checkWhereAndAddNewPlates(plates, timeStart, timeEnd, map1, map2,
                                      queue1, queue2);
        else
            checkWhereAndAddNewPlates(plates, timeStart, timeEnd, map2, map1,
                                      queue2, queue1);
    }

    int isParkingPaid(string const &plates, time_type currentTime,
                      umap_t const &map1, umap_t const &map2, size_t currentLine)
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

    // This function handles all errors and prints appropriate messages.
    // It reads input and acts accordingly to the command. It handles
    //
    int mainLoop()
    {
        string line, plates, time1Str, time2Str;
        umap_t platesTimesMAP1, platesTimesMAP2;
        pqueue_t queue1, queue2;
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
            else
            {
                time_type time1 = convertTime(time1Str);

                if (time1 < prevTime)
                {
                    if (!newDay)
                    {
                        platesTimesMAP1.clear();
                        queue1 = pqueue_t();
                    }
                    else
                    {
                        platesTimesMAP2.clear();
                        queue2 = pqueue_t();
                    }

                    newDay = !newDay;
                }

                if (!newDay)
                {
                    while (!queue1.empty() and queue1.top().first < time1)
                    {
                        platesTimesMAP1.erase(queue1.top().second);
                        queue1.pop();
                    }
                }
                else
                {
                    while (!queue2.empty() and queue2.top().first < time1)
                    {
                        platesTimesMAP2.erase(queue2.top().second);
                        queue2.pop();
                    }
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
                    time_type time2 = convertTime(time2Str);

                    if (correctTimeOfStay(time1, time2))
                    {
                        addToHashMap(plates, time1, time2, platesTimesMAP1,
                                     platesTimesMAP2, queue1, queue2, newDay);
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

}

int main()
{
    mainLoop();
    return 0;
}
