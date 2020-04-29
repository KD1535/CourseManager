// Student Course Manager
// Displays the students current class schedule. Can add and delete classes with respective information and
// look up classes using course/class ID. Can read and write to exisiting and new files.
// Kristophe F. Dizon
// April 27, 2020

#include <iostream> // header file to access strings/cin/cout/etc
#include <fstream> // header file to access file streams
#include "course.h" // header file with Days enum, CourseInfo struct and Course struct
#include <iomanip> // header file to access setw()

using namespace std;

// Initailizes array with all data in input file
// Pre: student is an array, inData is a valid file stream
// Post: Initializes student array with all data from input file
CourseInfo *InitNode (/* IN/OUT */ifstream &inData);

// Add course to course schedule
// Pre: headPtr is pointing to a declared and initialized linked list 
// Post: appends a declared and initialized node to the end of a valid link list
void AddNode(/* IN/OUT */CourseInfo *&headPtr,/* IN */ CourseInfo *newNodePtr);

// Displays all menu 
// Pre: None
// Post: Displays options to user
int PrintMenu();

// returns enum type of Day depending on day of week M-F
// Pre: string must contain a day during the week, M-F
// Post: returns an enum of Day to be used in DaysToStr()
Days StrToDay(/* IN */ string str);

// Returns day of week 
// Pre: StrToDay() returns a valid value within enum type Days
// Post: returns a string of the day of week dependent on enum value
string DaysToStr (/* IN */string str);

// Prompt user for file name and return ifstream
// Pre: inData declared
// Post: inData contains valid ifstream for data file
void GetFileInfo(/* IN/OUT */string &fileName, /* IN/OUT */ifstream &inData);

// Stores data from program into given output file.
// Pre: fileName is initilzied with a valid file and file stream is valid
//      counter contains correct length of student array
//      student array is intialized and delcared      
// Post: data at every index in student array is output into outData
void StoreData(/* IN */string fileName, /* IN/OUT */ ofstream &outData,/* IN */CourseInfo *student);

// Removes value from list
// Pre: intialized and declared headPtr
// Post: deletes node with respective value
void DeleteNode(/* IN/OUT */CourseInfo *&headPtr);

// Create and return a new node.
// Pre: None
// Post: Returns a pointer to NodeType with component set to user input and next set to nullptr
CourseInfo *CreateNode();

// Display all values in linked list
// Pre: studentPtr is pointing to a declared and initialized linked list
// Post: loop through each node and display values
void PrintList(/* IN */CourseInfo *studentPtr);

// Find course respective to course ID
// Pre: headPtr is pointing to a declared and initialized linked list
// Post: loop through each node and find respective node
void FindCourse (/* IN */CourseInfo *headPtr);

// Capatilizes all chars in a string
// Pre: string passed is initialized and declared
// Post: returns string in all caps
string toUpperString(string &str);

int main()
{
    // input file stream
    ifstream inData; 
    // output file stream
    ofstream outData; 
    // file to read/write data to
    string fileName; 
    // pointer of CourseInfo set to null / linked list
    CourseInfo *student = nullptr; 
    // user input for menu selection
    int choice; 
    // used to check if input file has data to read
    string testRead;

    // loop menu while choice != 7 (quit)
    do
    {
        // display menu to user and choice intialized with user's menu selection
        choice = PrintMenu();
        switch (choice)
        {
            // create new file to store course info from linked list
            case 1:
                // alert user to create new filename for data
                // loop if input is of wrong type
                cout << "Enter output filename (example: schedule.txt) " << endl;
                while(!(cin >> fileName))
                    cout << "enter alphanumeric characters only" << endl;
                // open the input file stream of given file
                inData.open(fileName.c_str());
                break;
            // open exisiting course info file
            case 2:
                    // opens valid input file stream
                    GetFileInfo(fileName,inData);
                    
                    // checking if file is not empty
                    // reads information from file and stores in node
                    // amount of nodes created corresponds to amount of courses read
                    if(inData>>testRead)
                    {
                        // start at beginning of file
                        inData.clear();
                        inData.seekg(0, ios::beg);
                        // read until eof
                        while (!inData.eof())
                        {
                            // add an initialized node to the linked list
                            AddNode(student,InitNode(inData));
                        }
                    }
                    // alert user if the file opened does not contain any course data 
                    // (no nodes were added to the linked list)
                    else
                    {
                        cout << "File opened, no courses found.\n";
                    }
                    
                    break;
            case 3:
                    // add a course and store into a node that is appened to the end of linked list
                    AddNode(student,CreateNode());
                    break;
            case 4:
                    // displays all values contained in each initialized node
                    PrintList(student);
                    break;
            case 5:
                    // find course with course ID
                    FindCourse(student);
                    break;
            case 6:
                    // removes a course (node) from linked list
                    DeleteNode(student);
                    break;
            case 7:
                    // exit program
                    break;
            
            default:
                    break;
        }
    } while (choice!=7);
    // Stores all data from linked list into file that is open
    // if the pointer is not null
    if(student != nullptr)
    {
        StoreData(fileName,outData,student);
    }
    // creates a blank file if linked list is empty (student == nullptr)
    else
    {
        outData.open(fileName.c_str());
    }
    
}

CourseInfo *InitNode (/* IN/OUT */ifstream &inData)
{
    // local variable to hold and return student info
    CourseInfo* student = new CourseInfo;

    // read all data from file into student variable
    getline(inData,student->course.courseName, ',');
    
    getline(inData,student->course.courseID, ',');
    // Capatilize every letter of courseID
    toUpperString(student->course.courseID);
    
    getline(inData,student->meetingDays, ',');
    getline(inData,student->meetingTimes, ',');
    getline(inData,student->building, ',');
    getline(inData,student->room,'\n');

    // return student with data read from file
    return student;
}

void FindCourse (/* IN/OUT */CourseInfo *headPtr)
{
    // Course ID to find
    string findID;
    // user input to search again
    char choice;
    // if course was found
    bool found = false;

    CourseInfo *currPtr;

    if(headPtr == nullptr)
    {
        cout << "COURSE SCHEDULE IS EMPTY!\n";
        return;
    }

    do
    {
        currPtr = headPtr;

        cout << "Course to find: ";
        while(!(cin >> findID))
        {
            cout << "Please enter a valid course ID. (Integers/Numbers only): ";
        };
        cin.clear();
        cin.ignore(1000,'\n');
        toUpperString(findID);

        while(currPtr != nullptr)
        {
            if(currPtr->course.courseID == findID)
            {
                cout << "Course name: " << currPtr->course.courseName << ", " << setw(5) << "Course ID: " << currPtr->course.courseID << ", " << setw(5) << "Course days: " << currPtr->meetingDays << ", " 
                    << setw(5) << "Course time: " << currPtr->meetingTimes << ", " << setw(5) << "Building: " << currPtr->building << ", " << setw(5) << "Room: " << currPtr->room << endl;
                found = true;
                break;
            }
            currPtr = currPtr->next;
        }
        if (!found)
        {
            cout << "\n*** Course " << findID << " not found ***\n";
        }
        
        cout << "Continue looking-up courses (y/n): ";
        cin >> choice;
    }while(toupper(choice) != 'N');


    
}

int PrintMenu()
{
    // declaring variable to store users choice
    int choice; 
    
    // display menu to user and stores input into choice
    cout << "\nEnter a number from the following menu - Enter 'Quit' to stop" << endl;
    cout << "1. Create new course schedule \n2. Read course schedule from an existing file \n3. Add a course \n4. Print current courses \n5. Look up course with course ID \n6. Delete course \n7. Quit" << endl;
    while(!(cin >> choice) || choice > 7 || choice < 1)
    {
        cin.clear();
        cin.ignore(1000,'\n');
        cout << "\nEnter a number from the following menu - Enter 'Quit' to stop" << endl;
        cout << "1. Create new course schedule \n2. Read course schedule from an existing file \n3. Add a course \n4. Print current courses \n5. Look up course with course ID \n6. Delete course \n7. Quit" << endl;
    }
    // returns the value of choice
    return choice; 
}

// returns enum type of Day depending on day of week M-F
// Pre: string must contain a day during the week, M-F
// Post: returns an enum of Day to be used in DaysToStr()
Days StrToDay(/* IN */string str)
{
    // index str at first letter and capatilize it and check for matching cases
    switch(toupper(str.at(0)))
    {
        case 'M': return Monday;
        case 'T': if (toupper(str.at(0)) =='U') return Tuesday;
                else return Thursday;
        case 'W': return Wednesday;
        case 'F': return Friday;
        default: return None;
    }
}

// Returns day of week 
// Pre: StrToDay() returns a valid value within enum type Days
// Post: returns a string of the day of week dependent on enum value
string DaysToStr (/* IN */ string str)
{
    Days day; // enum type Days to hold the return value of StrToDay()
    day = StrToDay(str); // intializes day with respective enum value
    string courseDay;
    
    // evaluate value of enum type and return respective day of week
    switch(day)
    {
        case 1: courseDay = "Monday";
                return courseDay;
        case 2: courseDay = "Tuesday";
                return courseDay;
        case 3: courseDay = "Wednesday";
                return courseDay;
        case 4: courseDay = "Thursday";
                return courseDay;
        case 5: courseDay = "Friday";
                return courseDay;
        default: courseDay = "";
                 return courseDay;
    }           
}

void GetFileInfo(/* IN/OUT */string &fileName, /* IN/OUT */ifstream &inData)
{
    do {
        // Prompt user for file name and open file
        cout << "Enter course schedule filename: ";
       
        cin >> fileName;
        inData.open(fileName.c_str());
        if (!inData){
            cout << "Invalid file!\n\n";
            }
    } while (!inData); // Loop until valid file name provided
}

void StoreData(/* IN */string fileName, /* IN/OUT */ ofstream &outData,/* IN */CourseInfo *student)
{
    CourseInfo *currPtr = student;
    outData.open(fileName.c_str());
    if (currPtr != nullptr)
    {
        outData << currPtr->course.courseName << "," << currPtr->course.courseID << "," << currPtr->meetingDays 
                << "," << currPtr->meetingTimes << "," << currPtr->building << "," << currPtr->room;
    }
    while(currPtr->next != nullptr)
    {
        currPtr = currPtr->next;
        outData << '\n' <<currPtr->course.courseName << "," << currPtr->course.courseID << "," << currPtr->meetingDays 
                << "," << currPtr->meetingTimes << "," << currPtr->building << "," << currPtr->room;
    }
}

void AddNode(/* IN/OUT */CourseInfo *&headPtr, /* IN */CourseInfo *newNodePtr)
{  
    CourseInfo *currPtr = headPtr;

    // checking if headPtr = equal
    //      if yes, make newNodePtr the head
    // or check if headPtr is greater than newNodePtr
    //      if yes, make newNodePtr the head and switch links
    if (headPtr == nullptr)  
    {
        //newNodePtr->next = headPtr;  
        headPtr = newNodePtr;
    }  
    else
    {  
        // working node
        // starting at the head of the list
        currPtr = headPtr;
        // Traverse through links until the spot of inserting  
        while (currPtr->next!=nullptr)  
        {  
            currPtr = currPtr->next;  
        } 
        //inserting new node 
        currPtr->next = newNodePtr;  
    }
}

// Creates a node
// Pre: None
// Post: Returns a a node
CourseInfo *CreateNode()
{
    // data for new node to create
    CourseInfo *newNodePtr = new CourseInfo;
    // day course is held
    string day;
    // name of course to add
    string courseName;

    // get course name and store into student
    cin.ignore();
    cout << "Course name: ";
    getline(cin, courseName, '\n');
    newNodePtr->course.courseName = courseName;
    cout << newNodePtr->course.courseName << endl;
    cin.clear();

    // get course ID and store into student
    // loop if integers are not inputted
    cout << "Course ID: ";
    while(!(cin >> newNodePtr->course.courseID))
    {
        cout << "INVALID ENTRY. INPUT ONLY ALPHANUMERIC CHARACTERS." << endl;
        cin.clear();
        cin.ignore(1000,'\n');
    };
    toUpperString(newNodePtr->course.courseID);

    // get course meeting days and store into student
    // only one day can be stored at a time
    cout << "Meeting Days? Monday, Tuesday, Wednesday, Thursday, Friday \n**For courses more than once a week, add an additional course with the respective day change**\n" << endl;
    cin >> day;
    // DaysToStr takes user input and returns a string of appropriate day
    newNodePtr->meetingDays = DaysToStr(day);
    cin.clear();
    cin.ignore(1000,'\n');

    // get course meeting times and store into student
    cout << "Meeting times (HH:MM - HH:MM, Military time)";
    cin >> newNodePtr->meetingTimes;
    cin.clear();
    cin.ignore(1000,'\n');

    // get course building and store into student
    cout << "Course building: ";
    cin >> newNodePtr->building;
    cin.clear();
    cin.ignore(1000,'\n');   

    // get course room and store into student
    cout << "Course room: ";
    cin >> newNodePtr->room;
    cin.clear();
    cin.ignore(1000,'\n');

    // set link to nullptr
    newNodePtr->next = nullptr;

    return newNodePtr;
}

// Removes value from list
// Pre: intialized and declared headPtr
// Post: deletes node with respective value
void DeleteNode(/* IN/OUT */CourseInfo *&headPtr)
{
    // working pointer, currentPtr
    CourseInfo *currPtr = headPtr; 
    // keep track of pointer behind current
    CourseInfo *prevPtr = nullptr; 
    // flag for input validation
    bool flag = false;
    // value to delete from list 
    string delvalue; 

    // check if node is empty
    //      if yes, notify user there is nothing in the list to delete
    if(currPtr == NULL){
        cout << "\nCOURSE SCHEDULE IS EMPTY. NO CHANGES MADE.\n";
        return;
    }

    // take user input for value to delete from list and store into value variable
    cout << "Enter course ID for respective course to delete: ";
    cin >> delvalue;
    toUpperString(delvalue);


    // if currPtr is equal to value to find and prevPtr = nullptr then value to delete is the headPtr
    if (currPtr->course.courseID == delvalue && prevPtr == nullptr)
    {
        headPtr=currPtr->next;
        delete currPtr;
        cout << "\n*** COURSE " << delvalue << " REMOVED FROM SCHEDULE. ***\n";
        return;
    }
    
    // while currPtr is in a intialized list
    while (currPtr != nullptr )
    {   
        // check if currPtr is equal to value to delete
        if( currPtr->course.courseID == delvalue)
        {
            // get location of the next linked node from current pointer
            prevPtr->next=currPtr->next;
            // delete node that currPtr points to
            delete currPtr;
            // set flag to true
            // input is not invalid
            flag = true;
            cout << "\n*** COURSE " << delvalue << " REMOVED FROM SCHEDULE. ***\n";
            break;
        }
        // traversing through the list until value to delete is found
        prevPtr = currPtr;
        currPtr = currPtr->next;
    }

    // alert user if value entered is not within the list
    if (!flag)
    {
        cout << "\n*** COURSE " << delvalue << " NOT FOUND. NO CHANGES MADE. ***\n";
    }

}

void PrintList(/* IN */CourseInfo *studentPtr)
{

    if (studentPtr == nullptr)
        cout << "COURSE SCHEDULE IS EMPTY!\n";

    // Loop until we've reached the end of the list (Or it's an empty list)
    while (studentPtr != nullptr) {
        
        // Print value of current node
        cout << "Course name: " << studentPtr->course.courseName << ", " << "Course ID: " << studentPtr->course.courseID << ", " << "Course days: " << studentPtr->meetingDays << ", " 
                    << "Course time: " << studentPtr->meetingTimes << ", " << "Building: " << studentPtr->building << ", " << "Room: " << studentPtr->room << endl;
        // Advance currPtr to point at next node
        studentPtr = studentPtr->next;
    } 
}                         

string toUpperString(string &str)
{
    int size = str.size();
    char c;
    for (int i = 0; i < size; i++)
    {
        if(str[i] != toupper(str[i]))
            str[i] = toupper(str[i]);
        else
        {
            str[i] = str[i];
        }
    }
    return str;
};