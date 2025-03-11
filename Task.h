#include <iomanip>
#include <string>

using namespace std;

class Date
{
public:
    int month;
    int day;
    int year;
    bool operator<(const Date&);
    string ToString();
};

bool Date::operator<(const Date& d)
{
    return (year < d.year) || (year == d.year && month < d.month) || (year == d.year && month == d.month && day < d.day);
}

string Date::ToString()
{
    stringstream ss;

    ss << setfill('0') << setw(2);
    ss << month << "/";
    ss << setfill('0') << setw(2);
    ss << day << "/";
    ss << year;

    return ss.str();
}

class Task
{
private:
    string name;
    Date dueDate;

public:
    Task(string, Date*);
    string GetName();
    string GetDueDateAsString();
    Date GetDueDate();
};

Task::Task(string n, Date* d)
{
    name = n;
    dueDate.month = d->month;
    dueDate.day = d->day;
    dueDate.year = d->year;
}

string Task::GetName()
{
    return name;
}

string Task::GetDueDateAsString()
{
    return dueDate.ToString();
}

Date Task::GetDueDate()
{
    return dueDate;
}