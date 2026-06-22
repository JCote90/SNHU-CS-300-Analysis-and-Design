//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Joseph Cote
// Description : CS 300 Project Two - ABCU Advising Assistance Program
//============================================================================

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

// Course object stored in the data structure
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;
};

// Convert a string to uppercase for consistent course searches
string toUpperCase(string text) {
    for (char& ch : text) {
        ch = static_cast<char>(toupper(static_cast<unsigned char>(ch)));
    }
    return text;
}

// Remove leading and trailing whitespace
string trim(string text) {
    size_t start = text.find_first_not_of(" \t\r\n");
    size_t end = text.find_last_not_of(" \t\r\n");

    if (start == string::npos) {
        return "";
    }

    return text.substr(start, end - start + 1);
}

// Split one CSV line by commas
vector<string> splitLine(string line) {
    vector<string> tokens;
    string token;
    stringstream stream(line);

    while (getline(stream, token, ',')) {
        tokens.push_back(trim(token));
    }

    return tokens;
}

// Load course data into a map.
// map keeps keys sorted alphanumerically automatically.
bool loadCourses(string fileName, map<string, Course>& courses) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: Could not open file: " << fileName << endl;
        return false;
    }

    courses.clear();

    string line;
    int lineNumber = 0;

    while (getline(file, line)) {
        lineNumber++;

        if (trim(line).empty()) {
            continue;
        }

        vector<string> tokens = splitLine(line);

        if (tokens.size() < 2) {
            cout << "Error: Invalid file format on line " << lineNumber << "." << endl;
            courses.clear();
            file.close();
            return false;
        }

        Course course;
        course.courseNumber = toUpperCase(tokens.at(0));
        course.title = tokens.at(1);

        for (size_t i = 2; i < tokens.size(); ++i) {
            course.prerequisites.push_back(toUpperCase(tokens.at(i)));
        }

        courses[course.courseNumber] = course;
    }

    file.close();

    // Validate that every listed prerequisite exists as a course.
    for (const auto& pair : courses) {
        for (const string& prerequisite : pair.second.prerequisites) {
            if (courses.find(prerequisite) == courses.end()) {
                cout << "Error: " << pair.first << " lists missing prerequisite "
                    << prerequisite << "." << endl;
                courses.clear();
                return false;
            }
        }
    }

    cout << courses.size() << " courses loaded successfully." << endl;
    return true;
}

// Print all courses in alphanumeric order
void printCourseList(const map<string, Course>& courses) {
    if (courses.empty()) {
        cout << "Error: Load course data first." << endl;
        return;
    }

    cout << endl;
    cout << "Here is a sample schedule:" << endl;
    cout << endl;

    for (const auto& pair : courses) {
        cout << pair.second.courseNumber << ", " << pair.second.title << endl;
    }
}

// Print one course and its prerequisite course numbers and titles
void printCourseInformation(const map<string, Course>& courses) {
    if (courses.empty()) {
        cout << "Error: Load course data first." << endl;
        return;
    }

    string courseNumber;
    cout << "What course do you want to know about? ";
    cin >> courseNumber;

    courseNumber = toUpperCase(courseNumber);

    auto courseIterator = courses.find(courseNumber);

    if (courseIterator == courses.end()) {
        cout << "Error: Course not found." << endl;
        return;
    }

    Course course = courseIterator->second;

    cout << endl;
    cout << course.courseNumber << ", " << course.title << endl;
    cout << "Prerequisites: ";

    if (course.prerequisites.empty()) {
        cout << "None" << endl;
    }
    else {
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            string prerequisiteNumber = course.prerequisites.at(i);
            Course prerequisiteCourse = courses.at(prerequisiteNumber);

            cout << prerequisiteCourse.courseNumber << " - " << prerequisiteCourse.title;

            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }

        cout << endl;
    }
}

// Print menu
void printMenu() {
    cout << endl;
    cout << "Menu:" << endl;
    cout << "  1. Load Data Structure" << endl;
    cout << "  2. Print Course List" << endl;
    cout << "  3. Print Course" << endl;
    cout << "  9. Exit" << endl;
    cout << "What would you like to do? ";
}

int main() {
    map<string, Course> courses;
    int choice = 0;
    string fileName;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        printMenu();

        if (!(cin >> choice)) {
            cout << "Error: Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
        case 1:
            cout << "Enter the course data file name: ";
            cin >> fileName;
            loadCourses(fileName, courses);
            break;

        case 2:
            printCourseList(courses);
            break;

        case 3:
            printCourseInformation(courses);
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option." << endl;
            break;
        }
    }

    return 0;
}

