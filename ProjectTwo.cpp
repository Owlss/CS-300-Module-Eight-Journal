/*
* Author - Erin MacDonald
* Date - 10/17/2026
*/


#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

// forward declarations
double strToDouble(string str, char ch);

// struct to hold Course information 
struct Course {
    string courseId; 
    string name;
    vector<string> prereq;
};


//HashTable data structve to hold Course Nodes
class HashTable {

private:
    // Define structures to hold Course
    struct Node {
        Course course;
        unsigned int key;
        Node* next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a Course
        Node(Course aCourse) : Node() {
            course = aCourse;
        }

        // initialize with a bid and a key
        Node(Course aCourse, unsigned int aKey) : Node(aCourse) {
            key = aKey;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Course course);
    void PrintAll();
    void Remove(string courseId);
    Course Search(string courseId);
    size_t Size();
  
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    Node node;
    node.key = UINT_MAX;
    for (int i = 0; i < tableSize; i++) {
        nodes.push_back(node);

    }
    // Initalize node structure by resizing tableSize
    tableSize = nodes.size();
}

/**
 * Destructor
 */
HashTable::~HashTable() {
    // erase nodes beginning
    for (int i = 0; i < tableSize - 1; i++) {
        Remove(nodes.at(i).course.courseId);
    }
}

/**
 * Calculate the hash value of a given key.
 */
unsigned int HashTable::hash(int key) {
   
    return key % tableSize;

}

/**
 * Insert a course
 */
void HashTable::Insert(Course course) {

    //Gets the numeric portion of the course
    int index = 0;
    string hashString = course.courseId;
    for (int i = 0; i < course.courseId.length();i++) {
        if (!isalpha(course.courseId.at(i))) {
            index = i;
            break;
        }
    }
    hashString = hashString.substr(index, hashString.length() - 1);
    
    //create key for the course
    int key = hash((atoi(hashString.data())));
   // cout << key << endl;
    
    //checks if no node is using current key , the insterts if so
    // else checks the node is not used
    // else will find the next open node
    if (nodes.at(key).key == UINT_MAX) {
        nodes.at(key).course = course;
        nodes.at(key).key = key;
    }
    else if (key > tableSize - 1) {
        nodes.at(key).key = UINT_MAX;
        key = UINT_MAX;
        nodes.at(key) = course;
        nodes.at(key).next = NULL;
    }
    else {
        for (int i = key + 1; i < tableSize - 1; i++) {
            if (nodes.at(i).key == UINT_MAX) {
                nodes.at(i).course = course;
                nodes.at(key).key = key;
            }
        }
        for (int i = 0; i < key; i++) {
            if (nodes.at(i).key == UINT_MAX) {
                nodes.at(i).course = course;
                nodes.at(key).key = key;
            }
        }
    }
    
}



/**
 * Print all bids
 */
void HashTable::PrintAll() {
    //vectors used for printing hashtable in order, temp holds the courseId and desc the name
    vector<string> tempVector, descVector;
    
    //assigns vaules in hashtable to vector
    Node* node;
    for (int i = 0; i < tableSize - 1; i++) {
        if (nodes.at(i).key != UINT_MAX) {
          //  cout << nodes.at(i).course.courseId << ", " << nodes.at(i).course.name << endl;
            tempVector.push_back(nodes.at(i).course.courseId);
            descVector.push_back(nodes.at(i).course.name);
            node = nodes.at(i).next;
            while (node != nullptr) {
          //      cout  << nodes.at(i).course.courseId << ", " << nodes.at(i).course.name << endl;
                tempVector.push_back(nodes.at(i).course.courseId);
                descVector.push_back(nodes.at(i).course.name);
                node = nodes.at(i).next;
            }
        }
    }

    //sorts the vectors with selection sort
    for (int i = 0; i < tempVector.size() - 1; i++) {
        int first = i;
        for (int j = i + 1; j < tempVector.size(); j++) {
            if (tempVector.at(j) < tempVector.at(first)) {
                first = j;
            }
        }
        swap(tempVector.at(i), tempVector.at(first));
        swap(descVector.at(i), descVector.at(first));
    }
     //prints the courses formated
    for (int i = 0; i < tempVector.size(); i++) {
        cout << tempVector.at(i) << ", " << descVector.at(i) << endl;
    }
    

}

/**
 * Remove a bid
 */
void HashTable::Remove(string courseId) {
    int key = hash(atoi(courseId.data()));
    nodes.erase(nodes.begin() + 1);
}

/**
 * Search for the specified courseId
 */
Course HashTable::Search(string courseId) {
    Course course;
    Node* node{};
    //gets the numreaic portion of the courseId, then hashes 
    int index = 0;
    string hashString = courseId;
    for (int i = 0; i < courseId.length();i++) {
        if (!isalpha(courseId.at(i))) {
            index = i;
            break;
        }
    }
    hashString = hashString.substr(index, hashString.length() - 1);

    int key = hash((atoi(hashString.data())));
  //  cout << key << endl;
    //if it finds the course with the key then return it, else return course 
    if (nodes.at(key).course.courseId == courseId) {
        return nodes.at(key).course;
    }
    if (nodes.at(key).course.courseId != courseId) {
        return course;
    }
    //if the node matches then reutrn regaurdless node is next node
    while (node != nullptr) {
        if (nodes.at(key).course.courseId == courseId) {
            return nodes.at(key).course;
        }
        node = nodes.at(key).next;
    }

    return course;
}

/*
* set the size
*/
size_t HashTable::Size()
{
    return nodes.size();
}

/*
* displays information for a selected course
*/
void displayCourse(Course course) {
    cout << course.courseId << ", " << course.name << endl;
    return;
}

/*
* Loads course from csv file into hashtable
*/
void loadCourse(string csvPath, HashTable* hashTable) {
    
    ifstream inFS;
    string data; // takes in data from csv
    inFS.open(csvPath);

    //vectors  used for error checking 
    vector<string> prereqCheck, courseCheck;
    
        
    //Runs when there is data in the file, will go line by line in file
        while (getline(inFS, data)) {
            Course course;
            //  cout << data << endl; ~~testing
            //read up to the first comma and set courseID, add courseID to course check, earse what was just read
            course.courseId = data.substr(0, data.find(','));
            courseCheck.push_back(data.substr(0, data.find(',')));
            data.erase(0, data.find(',') + 1);

            //   cout << data << endl; ~~testing
            //read up to the first comma and set name, earse what was just read
            course.name = data.substr(0, data.find(','));
            data.erase(0, data.find(',') + 1);
            
            //while thier is still data in the line
            while (data != "," || data != "" || data != " ") {
                //    cout << data << endl; ~~testing
                //check if the line ends in comma 
                //if so add prereq to course prereq vector and prereqCheck , easrse data added 
                if (data.find(",") != string::npos) {
                    course.prereq.push_back(data.substr(0, data.find(",")));
                    prereqCheck.push_back(data.substr(0, data.find(",")));
                 
                    data.erase(0, data.find(',') + 1);
                }
                //else add all remaining data to prereq vector and prereqCheck, earse data and break from loop
                else {
                    //        cout << data << endl; ~~testing
                    course.prereq.push_back(data);
                    prereqCheck.push_back(data.substr(0, data.find(",")));
         
                    data.erase(0, data.find("") + 1);
                    break;
                    
                }
                
            }
        //add data to hashtable 
        hashTable->Insert(course); 

        //testing to see if data is being copyed correctly 
  //cout << "ID: " << course.courseId << " Name:" << course.name << " ";
  //for (int i = 0; i < course.prereq.size(); i++) {
  //    if (course.prereq.at(i) != "") {
  //        cout << "REQ:" << course.prereq.at(i) << " " << endl;
  //    }
  //    
  //}
        //clear prereq vector 
        course.prereq.clear();
        }
        //close 
        inFS.close();

        //checks to see if all prereq course are courses on list, throws error else 
        bool check = true;
        try {
            for (int i = 0; i < prereqCheck.size(); i++) {
                
                for (int j = 0; j < courseCheck.size(); j++) {
                    if (prereqCheck.at(i) != courseCheck.at(j)) {
                        check == false;
                        break;
                    }
                }
                if (!check) {
                    throw(prereqCheck.at(i));
                    break;
                } 
            }
        }
        catch (string prereq) {
            cout << "Course '" << prereq << "' is not a course offered" << endl;
        }
}

int main(int argc, char* argv[]) {

    // Define a hash table to hold all the bids
    HashTable* Table;
    Course course;
    Table = new HashTable();
   
    string csv = "CS 300 ABCU_Advising_Program_Input.csv", courseKey;
    

    int choice = 0;
    while (choice != 9) {
        cout << "Welcome to the course planner" << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print course List" << endl;
        cout << "  3. Print course " << endl;
        cout << "  9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        switch (choice) {

        case 1:
            
            loadCourse(csv,Table);

            break;
            
        case 2:
            cout << "Here is a sample schedule: " << endl;
            Table->PrintAll();
            break;

        case 3:
            cout << "What course do you want to know about? ";
            cin >> courseKey;
            course = Table->Search(courseKey);


            if (!course.courseId.empty()) {
                displayCourse(course);
            }
            else {
                cout << "Course Id " << courseKey << " not found." << endl;
            }

            break;

        case 9:
            cout << "Thank you for using the course planner" << endl;
            break;

        default:
            cout << choice << " is not a vaild option" << endl;
            break;
        }
    }

    return 0;
}