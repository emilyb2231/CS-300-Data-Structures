//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Emily Bolyard
// Version     : 1.0
// Description : CS 300 Project Two
//============================================================================


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>

using namespace std;

const unsigned int DEFAULT_SIZE = 179;

// Define a structure for the course
struct Course {
    string courseId;
    string courseNumber;
    string courseTitle;
};

// Hash Table class
class HashTable {
private:
    struct Node {
        Course course;
        Node* next;
        Node(Course aCourse) : course(aCourse), next(nullptr) {}
    };

    vector<Node*> nodes;
    unsigned int tableSize;

    unsigned int hash(int key);

public:
    HashTable(unsigned int size = DEFAULT_SIZE);
    ~HashTable();
    void Insert(Course course);
    void PrintSorted(); // Changed function name
    void LoadFromCSV(const string& csvPath);
    Course Search(const string& courseId);  // Search function
};

// Constructor
HashTable::HashTable(unsigned int size) : tableSize(size) {
    nodes.resize(tableSize, nullptr);
}

// Destructor
HashTable::~HashTable() {
    for (auto nodePtr : nodes) {
        while (nodePtr) {
            Node* temp = nodePtr;
            nodePtr = nodePtr->next;
            delete temp;
        }
    }
}

unsigned int HashTable::hash(int key) {
    return key % tableSize;
}

// Insert a course
void HashTable::Insert(Course course) {
    unsigned int key = hash(atoi(course.courseId.c_str()));
    Node* newNode = new Node(course);

    if (nodes[key] == nullptr) {
        nodes[key] = newNode;
    }
    else {
        Node* current = nodes[key];
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Print sorted courses
void HashTable::PrintSorted() {
    vector<Course> courses;

    // Put all courses into a vector
    for (const auto& nodePtr : nodes) {
        Node* current = nodePtr;
        while (current != nullptr) {
            courses.push_back(current->course);
            current = current->next;
        }
    }

    // Sort the courses by courseId
    sort(courses.begin(), courses.end(), [](const Course& a, const Course& b) {
        return a.courseId < b.courseId;
        });

    // Print sorted courses
    for (const auto& course : courses) {
        cout << course.courseId << ", " << course.courseTitle << endl;
    }
}

// Load courses from a CSV file
void HashTable::LoadFromCSV(const string& csvPath) {
    ifstream inFS(csvPath);
    if (!inFS.is_open()) {
        cout << "Could not open file." << endl;
        return;
    }

    string line;
    while (getline(inFS, line)) {
        stringstream ss(line);
        Course aCourse;
        string token;
        vector<string> stringTokens;

        while (getline(ss, token, ',')) {
            stringTokens.push_back(token);
        }

        if (stringTokens.size() < 3) {
            cout << "Error" << endl;
            continue;
        }

        aCourse.courseId = stringTokens[0];
        aCourse.courseTitle = stringTokens[1];
        aCourse.courseNumber = stringTokens[2];

        Insert(aCourse);
    }

    inFS.close();
}



// Search for a course
Course HashTable::Search(const string& courseId) {
    unsigned int key = hash(atoi(courseId.c_str()));
    Node* current = nodes[key];

    while (current != nullptr) {
        if (current->course.courseId == courseId) {
            return current->course; // Return the found course
        }
        current = current->next;
    }
    return Course(); // Return an empty Course object if not found
}

// Main function
int main() {
    HashTable courseTable;

    while (true) {
        cout << "Menu:\n1. Load Data Structure\n2. Print Course List\n3. Print Course\n9. Exit\nWhat would you like to do?: ";
        int choice;
        cin >> choice;

        switch (choice) {
        case 1: {
            string csvPath;
            cout << "Enter the path to the CSV file: ";
            cin >> csvPath;
            courseTable.LoadFromCSV(csvPath);
            break;
        }
        case 2:
            courseTable.PrintSorted(); // Call the new PrintSorted function
            break;
        case 3: {
            string aKey;
            cout << "Enter course ID to search: ";
            cin >> aKey;
            Course course = courseTable.Search(aKey);
            if (!course.courseId.empty()) {
                cout << course.courseId << ", " << course.courseTitle << "\nPrerequisites: " << course.courseNumber << endl;
            }
            else {
                cout << "Course ID " << aKey << " not found.\n";
            }
            break;
        }
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            return 0; // Exit program
        default:
            cout << "Not a valid option.\n";
        }
        cin.clear();
        cin.ignore(); // Clear the input 
    }

    return 0;
}
