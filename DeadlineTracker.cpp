#include <ctime>
#include <iostream>
#include <string>
#include "User.h"

using namespace std;

void HandleInput(int& input)
{
    while (true)
    {
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Incorrect input. Re-enter number: ";
            cin >> input;
        }
        else
        {
            break;
        }
    }
}

void HandleDateInput(string& dueDate, Date& due)
{
    while (true)
    {
        if (dueDate.length() != 10)
        {
            cout << "Incorrect input. Re-enter date (MM/DD/YYYY): ";
            cin >> dueDate;
            continue;
        }

        int month = stoi(dueDate.substr(0, 2)), day = stoi(dueDate.substr(3, 2)), year = stoi(dueDate.substr(6, 4));

        if (month <= 0 || month > 12 || day <= 0 || day > 31 || year <= 0 || year > 9999)
        {
            cout << "Incorrect input. Re-enter date (MM/DD/YYYY): ";
            cin >> dueDate;
            continue;
        }

        due.month = month;
        due.day = day;
        due.year = year;
        break;
    }
}

int main()
{
    while (true)
    {
        system("CLS");
        string username, password, name;
        cout << "Deadline Tracker" << endl << endl;
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        User user(username);

        if (!user.Exists())
        {
            system("CLS");
            cout << "Let's sign you up." << endl;
            cout << "Enter your name: ";
            cin >> name;
            cout << "Re-enter your password: ";
            cin >> password;
            user.Register(name, password);
            system("CLS");
        }

        int retry = 3;
        while (!user.CanLogin(password) && retry > 0)
        {
            cout << "Incorrect password. Re-enter (" << retry-- << " attempts remaining): ";
            cin >> password;
        }

        if (retry <= 0) continue;

        bool isExit = false;
        time_t now = time(0); // current date/time based on current system
        tm* d = localtime(&now); // convert now to string form
        Date date;
        date.month = (d->tm_mon) + 1;
        date.day = (d->tm_mday);
        date.year = (d->tm_year) + 1900;

        while (!isExit)
        {
            system("CLS");
            int input;
            cout << "Welcome " << user.GetName() << "!" << endl << endl;
            cout << "Here's your tasks due today (" << date.ToString() << "):" << endl;
            cout << user.GetTasksDueOn(&date) << endl;
            cout << "Select an action:" << endl;
            cout << "[1] View your tasks" << endl;
            cout << "[2] Add a task" << endl;
            cout << "[3] Remove a task" << endl;
            cout << "[4] Edit a task" << endl;
            cout << "[5] Edit profile" << endl;
            cout << "[6] Sign out" << endl << endl;
            cout << "Input number: ";
            cin >> input;

            HandleInput(input);

            string taskName, dueDate, result;
            Date due;
            switch (input)
            {
            case 1:
                system("CLS");
                cout << "View your tasks" << endl << endl;
                cout << "Select an action:" << endl;
                cout << "[1] View your tasks sorted by name" << endl;
                cout << "[2] View your tasks sorted by date" << endl;
                cout << "[3] Cancel" << endl << endl;
                cout << "Input number: ";
                cin >> input;

                while (true)
                {
                    HandleInput(input);

                    switch (input)
                    {
                    case 1:
                        system("CLS");
                        cout << "View your tasks sorted by name" << endl << endl;
                        cout << user.GetTasksSortedByName() << endl;
                        cout << "Select an action:" << endl;
                        cout << "[1] OK" << endl << endl;
                        cout << "Input number: ";
                        cin >> input;

                        while (true)
                        {
                            HandleInput(input);

                            switch (input)
                            {
                            case 1:
                                break;
                            default:
                                cout << "Incorrect input. Re-enter number: ";
                                cin >> input;
                                continue;
                            }

                            break;
                        }

                        break;
                    case 2:
                        system("CLS");
                        cout << "View your tasks sorted by date" << endl << endl;
                        cout << user.GetTasksSortedByDate() << endl;
                        cout << "Select an action:" << endl;
                        cout << "[1] OK" << endl << endl;
                        cout << "Input number: ";
                        cin >> input;

                        while (true)
                        {
                            HandleInput(input);

                            switch (input)
                            {
                            case 1:
                                break;
                            default:
                                cout << "Incorrect input. Re-enter number: ";
                                cin >> input;
                                continue;
                            }

                            break;
                        }

                        break;
                    case 3:
                        break;
                    default:
                        cout << "Incorrect input. Re-enter number: ";
                        cin >> input;
                        continue;
                    }

                    break;
                }

                break;
            case 2:
                system("CLS");
                cout << "Add a task" << endl << endl;
                cout << "Enter task name: ";
                cin >> taskName;
                cout << "This task is due on (MM/DD/YYYY): ";
                cin >> dueDate;

                HandleDateInput(dueDate, due);
                result = user.AddTask(taskName, &due)
                    ? "Task added successfully!"
                    : "Specified task already exists!";
                cout << result << endl;
                system("PAUSE");
                break;
            case 3:
                system("CLS");
                cout << "Remove a task" << endl << endl;
                cout << "Enter task name: ";
                cin >> taskName;
                cout << "This task is due on (MM/DD/YYYY): ";
                cin >> dueDate;

                HandleDateInput(dueDate, due);
                result = user.RemoveTask(taskName, &due)
                    ? "Task removed successfully!"
                    : "Specified task was not found!";
                cout << result << endl;
                system("PAUSE");
                break;
            case 4:
                system("CLS");
                cout << "Edit a task" << endl << endl;
                cout << "Enter task name: ";
                cin >> taskName;
                cout << "This task is due on (MM/DD/YYYY): ";
                cin >> dueDate;

                HandleDateInput(dueDate, due);
                
                if (user.RemoveTask(taskName, &due))
                {
                    cout << "Enter updated task name: ";
                    cin >> taskName;
                    cout << "Enter updated due date (MM/DD/YYYY): ";
                    cin >> dueDate;

                    HandleDateInput(dueDate, due);
                    result = user.AddTask(taskName, &due)
                        ? "Task updated successfully!"
                        : "Unknown error!";
                    cout << result << endl;
                }
                else
                {
                    cout << "Specified task was not found!" << endl;
                }

                system("PAUSE");
                break;
            case 5:
                system("CLS");
                cout << "Edit profile" << endl << endl;
                cout << "Re-enter your password: ";
                cin >> password;

                retry = 3;
                while (!user.CanLogin(password) && retry > 0)
                {
                    cout << "Incorrect password. Re-enter (" << retry-- << " attempts remaining): ";
                    cin >> password;
                }

                if (retry <= 0) break;

                cout << "Enter updated name: ";
                cin >> name;
                cout << "Enter updated password: ";
                cin >> password;

                user.Remove(username);
                user.Register(name, password);
                cout << "Profile updated successfully!" << endl;
                system("PAUSE");
                break;
            case 6:
                isExit = true;
                cout << "Signed out successfully!" << endl;
                system("PAUSE");
                break;
            default:
                cout << "Incorrect input. Re-enter number: ";
                cin >> input;
                continue;
            }
        }
    }
}