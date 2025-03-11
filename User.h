#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Task.h"

using namespace std;

class User
{
private:
    string username;
    string password;
    string name;
    vector<Task> tasks;

public:
    User(string);
    bool Exists();
    bool CanLogin(string);
    bool AddTask(string, Date*);
    bool RemoveTask(string, Date*);
    string GetName();
    string GetTasksDueOn(Date*);
    string GetTasksSortedByName();
    string GetTasksSortedByDate();
    void Register(string, string);
    void Remove(string);
    void RefreshTasks();
};

User::User(string u)
{
    username = u;
}

bool User::Exists()
{
    const auto delimiter = ';';
    auto isFound = false;
    string line;
    ifstream input;

    input.open("Users.dat");

    if (input.is_open())
    {
        while (getline(input, line))
        {
            string s;
            stringstream ss(line);
            vector<string> tokens;

            while (getline(ss, s, delimiter))
            {
                tokens.push_back(s);
            }

            if (tokens[0] == username)
            {
                isFound = true;
                password = tokens[1];
                name = tokens[2];
                RefreshTasks();
                break;
            }
        }
    }

    input.close();

    return isFound;
}

bool User::CanLogin(string p)
{
    return password == p;
}

bool User::AddTask(string n, Date* d)
{
    const auto delimiter = ';';
    auto isFound = false;
    string line;
    ifstream input;
    ofstream output;
    Task task(n, d);

    input.open("Tasks.dat");

    if (input.is_open())
    {
        while (getline(input, line))
        {
            string s;
            stringstream ss(line);
            vector<string> tokens;

            while (getline(ss, s, delimiter))
            {
                tokens.push_back(s);
            }

            isFound =
                tokens.size() > 0
                && tokens[0] == username
                && tokens[1] == task.GetName()
                && tokens[2] == task.GetDueDateAsString();

            if (isFound) break;
        }
    }

    input.close();

    if (!isFound)
    {
        output.open("Tasks.dat", ios::app);

        if (output.is_open())
        {
            output << endl << username << ";" << task.GetName() << ";" << task.GetDueDateAsString() << ";";
            RefreshTasks();
        }

        output.close();
    }

    return !isFound;
}

bool User::RemoveTask(string n, Date* d)
{
    const auto delimiter = ';';
    auto isFound = false, isAlreadyFound = false;
    string line;
    vector<string> lines;
    ifstream input;
    ofstream output;
    Task task(n, d);

    input.open("Tasks.dat");

    if (input.is_open())
    {
        while (getline(input, line))
        {
            string s;
            stringstream ss(line);
            vector<string> tokens;

            while (getline(ss, s, delimiter)) tokens.push_back(s);

            isFound =
                !isAlreadyFound
                && tokens.size() > 0
                && tokens[0] == username
                && tokens[1] == task.GetName()
                && tokens[2] == task.GetDueDateAsString();

            if (isFound) isAlreadyFound = true;
            else lines.push_back(line);
        }
    }

    input.close();

    if (isAlreadyFound)
    {
        output.open("Tasks.tmp");

        if (output.is_open())
        {
            for (auto line = lines.begin(); line != lines.end(); ++line)
            {
                auto eol = line == lines.end() - 1 ? "" : "\n";
                output << *line << eol;
            }
        }

        output.close();
        RefreshTasks();
        remove(string("Tasks.dat").c_str());
        rename("Tasks.tmp", "Tasks.dat");
    }

    return isAlreadyFound;
}

string User::GetName()
{
    return name;
}

string User::GetTasksDueOn(Date* d)
{
    stringstream ss;
    RefreshTasks();

    for (auto task = tasks.begin(); task != tasks.end(); ++task)
    {
        if (task->GetDueDateAsString() == d->ToString())
            ss << task->GetName() << endl;
    }

    return ss.str();
}

string User::GetTasksSortedByName()
{
    stringstream ss;
    RefreshTasks();
    vector<Task> sortedTasks = tasks;
    sort(sortedTasks.begin(), sortedTasks.end(), [](Task t1, Task t2) { return t1.GetName() < t2.GetName(); });

    for (auto sortedTask = sortedTasks.begin(); sortedTask != sortedTasks.end(); ++sortedTask)
    {
        ss << setw(25);
        ss << left << sortedTask->GetName();
        ss << setw(10);
        ss << left << sortedTask->GetDueDateAsString() << endl;
    }

    return ss.str();
}

string User::GetTasksSortedByDate()
{
    stringstream ss;
    RefreshTasks();
    vector<Task> sortedTasks = tasks;
    sort(sortedTasks.begin(), sortedTasks.end(), [](Task t1, Task t2) { return t1.GetDueDate() < t2.GetDueDate(); });

    for (auto sortedTask = sortedTasks.begin(); sortedTask != sortedTasks.end(); ++sortedTask)
    {
        ss << setw(25);
        ss << left << sortedTask->GetName();
        ss << setw(10);
        ss << left << sortedTask->GetDueDateAsString() << endl;
    }

    return ss.str();
}

void User::Register(string n, string p)
{
    ofstream output;

    name = n;
    password = p;
    output.open("Users.dat", ios::app);

    if (output.is_open())
    {
        output << endl << username << ";" << password << ";" << name << ";";
    }

    output.close();
}

void User::Remove(string u)
{
    const auto delimiter = ';';
    auto isFound = false, isAlreadyFound = false;
    string line;
    vector<string> lines;
    ifstream input;
    ofstream output;

    input.open("Users.dat");

    if (input.is_open())
    {
        while (getline(input, line))
        {
            string s;
            stringstream ss(line);
            vector<string> tokens;

            while (getline(ss, s, delimiter))
            {
                tokens.push_back(s);
            }

            isFound =
                !isAlreadyFound
                && tokens.size() > 0
                && tokens[0] == u;

            if (isFound) isAlreadyFound = true;
            else lines.push_back(line);
        }
    }

    input.close();

    if (isAlreadyFound)
    {
        output.open("Users.tmp");

        if (output.is_open())
        {
            for (auto line = lines.begin(); line != lines.end(); ++line)
            {
                auto eol = line == lines.end() - 1 ? "" : "\n";
                output << *line << eol;
            }
        }

        output.close();
        remove(string("Users.dat").c_str());
        rename("Users.tmp", "Users.dat");
    }
}

void User::RefreshTasks()
{
    const auto delimiter = ';';
    string line;
    ifstream input;

    input.open("Tasks.dat");

    if (input.is_open())
    {
        vector<Task>().swap(tasks);

        while (getline(input, line))
        {
            string s;
            stringstream ss(line);
            vector<string> tokens;

            while (getline(ss, s, delimiter))
            {
                tokens.push_back(s);
            }

            if (tokens.size() > 0 && tokens[0] == username)
            {
                Date dueDate;
                dueDate.month = stoi(tokens[2].substr(0, 2));
                dueDate.day = stoi(tokens[2].substr(3, 2));
                dueDate.year = stoi(tokens[2].substr(6, 4));

                Task task(tokens[1], &dueDate);
                tasks.push_back(task);
            }
        }
    }

    input.close();
}