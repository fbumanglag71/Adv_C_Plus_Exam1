#include "input.h"   //unique input header file 
#include <iostream>  //standard input-output stream
#include <fstream>   //for file operations
#include <vector>    //to use vector container
#include <string>    //to use string class
#include <regex>     //for regular expressions
#include <cstring>   //for c-style string operations
#include <cstdlib>   //for system() function to clear screen
#include <cctype>    //for toupper() function
#include <limits>    //for numeric_limits usage
#include <cstdio>    //for remove() function

using namespace std;  //simplifies code by removing std::

const int MAX_STUDENTS = 100;  //predefined max number of students (can be adjusted)
int currentStudentCount = 0;   //tracks the number of students currently in the array


//structure to hold student data
struct Student {
    string studentID;
    string name;
    string birthDate;
    int collegeLevel = 0;  //default to 0 (unknown)
    string term;
    bool ignoreLowestScore = false;  //default to false (0)

    //additional fields for scores
    float discussionScores[10] = { 0.0 };  //array to hold 10 discussion scores
    float quizScores[10] = { 0.0 };        //array to hold 10 quiz scores
    float projectScores[10] = { 0.0 };     //array to hold 10 project scores
    float examScores[3] = { 0.0 };         //array to hold 3 exam scores
    float discussionAvg = 0.0;             //average discussion score
    float quizAvg = 0.0;                   //average quiz score
    float projectAvg = 0.0;                //average project score
    float examAvg = 0.0;                   //average exam score
    char letterGrade = 'F';                //letter grade
};


//vector to hold students
vector<Student> students;


//structure to hold student data
struct StudentArray {
    string studentID;
    string name;
    string birthDate;
    int collegeLevel = 0;  //default to 0 (unknown)
    string term;
    bool ignoreLowestScore = false;  //default to false (0)

    //additional fields for scores
    float* discussionScores;  //dynamically allocated array for discussion scores
    float* quizScores;        //dynamically allocated array for quiz scores
    float* projectScores;     //dynamically allocated array for project scores
    float* examScores;        //dynamically allocated array for exam scores
    float discussionAvg = 0.0;  //average discussion score
    float quizAvg = 0.0;        //average quiz score
    float projectAvg = 0.0;     //average project score
    float examAvg = 0.0;        //average exam score
    char letterGrade = 'F';     //letter grade

    //constructor for dynamic memory allocation
    StudentArray() {
        discussionScores = new float[10]();  //allocate memory for 10 discussion scores
        quizScores = new float[10]();        //allocate memory for 10 quiz scores
        projectScores = new float[10]();     //allocate memory for 10 project scores
        examScores = new float[3]();         //allocate memory for 3 exam scores
    }

    //destructor to free dynamic memory
    ~StudentArray() {
        delete[] discussionScores;
        delete[] quizScores;
        delete[] projectScores;
        delete[] examScores;
    }
};

//dynamically allocate memory for an array of StudentArray objects
StudentArray** students2 = new StudentArray * [MAX_STUDENTS];

//function prototypes
void addNewStudentArray();
void readOrCreateFileArray();
void updateStudentFormArray(int index);
void arrayImplementationMenu();
void deleteStudentArray(int index);
void writeStudentDataToFileArray();
void setTextColor(const char* colorCode);
void resetTextColor();
bool validateInput(const string& input, const regex& pattern, const string& errorMessage);


// file //
struct StudentFile {
    string studentID;
    string name;
    string birthDate;
    int collegeLevel = 0;
    string term;
    bool ignoreLowestScore = false;
    float* discussionScores;
    float* quizScores;
    float* projectScores;
    float* examScores;
    float discussionAvg = 0.0;
    float quizAvg = 0.0;
    float projectAvg = 0.0;
    float examAvg = 0.0;
    char letterGrade = 'F';

    StudentFile() {
        discussionScores = new float[10]();
        quizScores = new float[10]();
        projectScores = new float[10]();
        examScores = new float[3]();
    }

    ~StudentFile() {
        delete[] discussionScores;
        delete[] quizScores;
        delete[] projectScores;
        delete[] examScores;
    }
};

StudentFile** students3 = new StudentFile * [MAX_STUDENTS];

void addNewStudentFile();
void readOrCreateFileFile();
void updateStudentFormFile(int index);
void fileImplementationMenu();
void deleteStudentFile(int index);
void writeStudentDataToFileFile();
bool validateInput(const string& input, const regex& pattern, const string& errorMessage);
void setTextColor(const char* colorCode);
void resetTextColor();



//function to set text color using ansi escape codes
void setTextColor(const char* colorCode) {
    cout << "\033[" << colorCode << "m";
}

//function to reset text color to default
void resetTextColor() {
    cout << "\033[0m";
}

//function to validate input using regex and display error messages
bool validateInput(const string& input, const regex& pattern, const string& errorMessage) {
    if (!regex_match(input, pattern)) {
        setTextColor("91");  //set text color to red for error
        cout << errorMessage << endl;
        resetTextColor();
        return false;
    }
    return true;
}

//function to validate date format (mm/dd/yyyy)
bool validateDateFormat(const string& date) {
    regex datePattern("^\\d{2}/\\d{2}/\\d{4}$");  //mm/dd/yyyy format
    return regex_match(date, datePattern);
}

//function to validate term format (nnnnxx where xx is fa, sp, su)
bool validateTermFormat(const string& term) {
    regex termPattern("^\\d{4}(FA|SP|SU)$");  //nnnnxx format with valid suffixes
    return regex_match(term, termPattern);
}

//function to handle option #2 - add a new student to the vector
void addNewStudent() {
    system("cls");

    setTextColor("93");  //set text color to light yellow for prompt
    cout << "\tAdd a new student to the vector\n";
    cout << string(75, char(196)) << "\n";
    resetTextColor();  //reset color after displaying menu

    Student newStudent;
    string input;

    //1. specify a new student id number (ccnnnnnn)
    regex idPattern("^[A-Z]{2}\\d{6}$");  //ccnnnnnn format
    do {
        cout << "\tSpecify a new Student ID number (CCnnnnnn)  : ";
        getline(cin, input);
    } while (!validateInput(input, idPattern, "\tERROR: Student ID must be in the format CCnnnnnn (e.g., AA123456)."));

    newStudent.studentID = input;

    //2. specify a name
    cout << "\tSpecify a name  : ";
    getline(cin, newStudent.name);

    //3. specify a birth date (mm/dd/yyyy)
    regex birthDatePattern("^\\d{2}/\\d{2}/\\d{4}$");  //mm/dd/yyyy format
    do {
        cout << "\tSpecify a birth date (mm/dd/yyyy): ";
        getline(cin, input);
    } while (!validateInput(input, birthDatePattern, "\tERROR: Birth date must be in the format mm/dd/yyyy (e.g., 09/04/1971)."));

    newStudent.birthDate = input;

    //4. specify a college level (0-unknown, 1-freshman, 2-sophomore, 3-junior, or 4-senior)
    do {
        cout << "\tSpecify a college level (0-unknown, 1-Freshman, 2-Sophomore, 3-Junior, or 4-Senior): ";
        getline(cin, input);
    } while (!validateInput(input, regex("^[0-4]$"), "\tERROR: College level must be a number between 0 and 4."));

    newStudent.collegeLevel = stoi(input);

    //5. specify a term (nnnncc)
    regex termPattern("^\\d{4}(SU|SP|FA)$");  //nnnnsu, nnnnsp, nnnnfa
    do {
        cout << "\tSpecify a term (nnnnCC): ";
        getline(cin, input);
    } while (!validateInput(input, termPattern, "\tERROR: Term must be in the format nnnnCC where CC is 'SU', 'SP', or 'FA'."));

    newStudent.term = input;

    //6. specify ignore the lowest score of assignments (0-false or 1-true)
    do {
        cout << "\tSpecify ignore the lowest score of assignments: (0-false or 1-true) ";
        getline(cin, input);
    } while (!validateInput(input, regex("^[01]$"), "\tERROR: Must be 0 (false) or 1 (true)."));

    newStudent.ignoreLowestScore = (input == "1");

    //add the new student to the vector
    students.push_back(newStudent);

    //confirm the student was added
    cout << "\n\tA new student record has been added to the vector.\n";
    system("pause");
}

//function to handle option #1 - read or create a binary file specified by the user
void readOrCreateFile() {
    system("cls");

    setTextColor("93");  //set text color to light yellow for prompt
    cout << "\tRead data from binary file and store into the vector\n";
    cout << string(75, char(196)) << "\n";
    resetTextColor();  //reset color after displaying menu

    //prompt the user to enter the binary data file name
    string fileName;
    cout << "\tSpecify the binary data file name (e.g., COURSES.DAT): ";
    cin >> fileName;  //use cin to read the input without getline to avoid issues

    //check if the user entered the correct file name (courses.dat)
    if (fileName != "COURSES.DAT") {
        //if the file name is incorrect, show an error message in red
        setTextColor("91");  //set text color to red for error
        cout << "\n\tERROR: Binary data file must be named 'COURSES.DAT'.\n";
        resetTextColor();  //reset color after displaying error
        system("pause");
        return;
    }

    //check if the file exists
    ifstream inFile(fileName);
    if (inFile) {
        inFile.close();  //close the file if it exists
        if (remove(fileName.c_str()) != 0) {
            setTextColor("91");  //red text for error
            cout << "\n\tERROR: Unable to delete existing " << fileName << " file.\n";
            resetTextColor();
        }
        else {
            cout << "\n\tAn existing " << fileName << " file has been deleted. Re-create the" << fileName << "\n";
        }
    }
    else {
        //if the file doesn't exist, create it
        ofstream outFile(fileName, ios::binary);
        if (!outFile) {
            setTextColor("91");  //red text for error
            cout << "\n\tERROR: Unable to create " << fileName << " file.\n";
            resetTextColor();
        }
        else {
            setTextColor("32");  //green text for success message
            cout << "\n\tCompleted read in student data from binary file, COURSES.DAT, and stored into the vector.\n";
            resetTextColor();  //reset text color after message
            outFile.close();  //close the newly created file
        }
    }

    system("pause");
}


//function to update the student details in the update student form
void updateStudentForm(int index) {
    system("cls");
    bool changesCommitted = false;
    bool validInput = false;
    char option;

    do {
        system("cls");

        //display the student's current details
        setTextColor("97");  //white for headers
        cout << "\n\tStudent ID    : ";
        setTextColor("32");  //green for student data
        cout << students[index].studentID << "\n";

        setTextColor("97");  //white for headers
        cout << "\tName          : ";
        setTextColor("32");  //green for student data
        cout << students[index].name << "\n";

        setTextColor("97");  //white for headers
        cout << "\tDate of birth : ";
        setTextColor("32");  //green for student data
        cout << students[index].birthDate << "\n";

        setTextColor("97");  //white for headers
        cout << "\tClass Level   : ";
        string classLevels[] = { "Unknown", "Freshman", "Sophomore", "Junior", "Senior" };
        setTextColor("32");  //green for student data
        cout << classLevels[students[index].collegeLevel] << "\n";

        setTextColor("97");  //white for headers
        cout << "\tTerm          : ";
        setTextColor("32");  //green for student data
        cout << students[index].term << "\n";

        setTextColor("97");  //white for headers
        cout << "\n\tLETTER GRADE  : ";
        setTextColor("32");  //green for student data
        cout << students[index].letterGrade << " (" << students[index].examAvg << "%)\n";

        setTextColor("97");  //white for headers
        cout << "\tIgnore lowest score  : ";
        setTextColor("32");  //green for student data
        cout << (students[index].ignoreLowestScore ? "true" : "false") << "\n";

        //display menu
        setTextColor("93");  //light yellow for menu
        cout << "\n\tEdit Student Menu\n";
        cout << string(75, char(205)) << "\n";
        cout << "\t2. Change Name\n";
        cout << "\t3. Change Birth Date\n";
        cout << "\t4. Change College Level\n";
        cout << "\t5. Change Term\n";
        cout << "\t6. Change Ignore Lowest Score setting\n";
        cout << "\t7. Change Discussion scores\n";
        cout << "\t8. Change Quiz scores\n";
        cout << "\t9. Change Project scores\n";
        cout << "\t10. Change Exam scores\n";
        cout << string(75, char(196)) << "\n";
        cout << "\t0. Uncommit the change(s)\n";
        cout << "\t1. Commit the change(s)\n";
        cout << string(75, char(205)) << "\n";

        //option input
        setTextColor("97");  //white for option prompt
        cout << "\tOption: ";
        cin >> option;
        cin.ignore();  //clear newline character from input buffer

        switch (option) {
        case '2': {
            //prompt the user to specify a new name
            cout << "\tSpecify a new name  : ";
            string newName;

            //read the new name input using getline to capture full name with spaces
            getline(cin, newName);

            //display the old and new name for confirmation
            cout << "\tName changed from " << students[index].name << " to " << newName << "\n";

            //update the student's name in the vector
            students[index].name = newName;
            break; //exit the case
        }

        case '3': {
            //case to change the birth date of a student
            //prompts the user to enter a new birth date in the format "mm/dd/yyyy"
            //validates the input to ensure the correct format is entered
            //if the input format is invalid, an error message is displayed and input is requested again
            //once a valid format is entered, the birth date is updated in the student record
            string newBirthDate;
            do {
                cout << "\tSpecify a new birth date (mm/dd/yyyy): ";
                getline(cin, newBirthDate);
                if (!validateDateFormat(newBirthDate)) {
                    cout << "ERROR: Invalid input. Date format must be MM/DD/YYYY.\n";
                }
            } while (!validateDateFormat(newBirthDate));
            cout << "\tBirth date changed from " << students[index].birthDate << " to " << newBirthDate << "\n";
            students[index].birthDate = newBirthDate;
            break;
        }
        case '4': {
            //case to change the college level of a student
            //prompts the user to enter a new college level (0 for unknown, 1 for Freshman, 2 for Sophomore, 3 for Junior, or 4 for Senior)
            //validates the input to ensure it is between 0 and 4
            //if the input is outside the valid range, an error message is displayed and input is requested again
            //once a valid level is entered, the college level is updated in the student record and the change is displayed

            string newLevel;
            do {
                cout << "\tSpecify a new college level (0-unknown, 1-Freshman, 2-Sophomore, 3-Junior, or 4-Senior): ";
                getline(cin, newLevel);
                if (newLevel < "0" || newLevel > "4") {
                    cout << "ERROR: Invalid input. Must be between 0 and 4.\n";
                }
            } while (newLevel < "0" || newLevel > "4");
            int level = stoi(newLevel);
            cout << "\tCollege level changed to " << classLevels[level] << "\n";
            students[index].collegeLevel = level;
            break;
        }
        case '5': {
            // case to change the term for a student
            // prompts the user to enter a new term in the format nnnnXX (where XX can be 'FA', 'SU', or 'SP')
            // validates the input to ensure it matches the required format
            // if the input does not match the format, an error message is displayed and input is requested again
            // once a valid term is entered, the term is updated in the student record and the change is displayed
            string newTerm;
            do {
                cout << "\tSpecify a new term (nnnnXX): ";
                getline(cin, newTerm);
                if (!validateTermFormat(newTerm)) {
                    cout << "ERROR: Invalid input. Term must be in the format nnnnXX where XX is 'FA', 'SU', or 'SP'.\n";
                }
            } while (!validateTermFormat(newTerm));
            cout << "\tTerm changed from " << students[index].term << " to " << newTerm << "\n";
            students[index].term = newTerm;
            break;
        }
        case '6': {
            //initiate variable to store user input for ignoring lowest score setting
            string newIgnore;

            //continuously prompt user until a valid input (1 or 0) is received
            do {
                cout << "\tSpecify ignore the lowest score of assignments: (1-true or 0-false): ";
                getline(cin, newIgnore); //use getline to capture full input as string

                //validate input, ensure it is either '1' or '0'
                if (newIgnore != "1" && newIgnore != "0") {
                    cout << "ERROR: Invalid input. Must be 1 (true) or 0 (false).\n";
                }
            } while (newIgnore != "1" && newIgnore != "0"); //repeat until input is valid

            //output the change for user confirmation
            cout << "\tIgnore lowest score changed to " << (newIgnore == "1" ? "true" : "false") << "\n";

            //update the student's record with new setting
            students[index].ignoreLowestScore = (newIgnore == "1");

            break; //exit this case block
        }

        case '7': {
            //initiate variable for discussion score index
            int discussionIndex;
            //prompt user to specify the discussion score index to update
            do {
                cout << "\tSpecify the index (0...9) of the discussion array: ";
                cin >> discussionIndex;
                //loop to ensure input index is within the valid range (0 to 9)
            } while (discussionIndex < 0 || discussionIndex > 9);

            //declare variable to store the new score
            float newScore;
            //prompt user to enter the new score for the specified index
            do {
                cout << "\tSpecify a new score#" << discussionIndex << " : ";
                cin >> newScore;
                //validate new score is within the valid range (0.0 to 10.0)
                if (newScore < 0 || newScore > 10) {
                    cout << "ERROR: Invalid input. Must be from 0.0 to 10.0.\n";
                }
            } while (newScore < 0 || newScore > 10);
            //output confirmation of the score change
            cout << "\tdiscussion[" << discussionIndex << "] changed from "
                << students[index].discussionScores[discussionIndex] << " to " << newScore << ".\n";
            //update the score in the student's record
            students[index].discussionScores[discussionIndex] = newScore;
            break;
        }

        case '8': {
            //initiate variable for quiz score index
            int quizIndex;
            //prompt user to specify the quiz score index to update
            do {
                cout << "\tSpecify the index (0...9) of the quiz array: ";
                cin >> quizIndex;
                //loop to ensure input index is within the valid range (0 to 9)
            } while (quizIndex < 0 || quizIndex > 9);

            //declare variable to store the new score
            float newScore;
            //prompt user to enter the new score for the specified index
            do {
                cout << "\tSpecify a new score#" << quizIndex << " : ";
                cin >> newScore;
                //validate new score is within the valid range (0.0 to 30.0)
                if (newScore < 0 || newScore > 30) {
                    cout << "ERROR: Invalid input. Must be from 0.0 to 30.0.\n";
                }
            } while (newScore < 0 || newScore > 30);
            //output confirmation of the score change
            cout << "\tquiz[" << quizIndex << "] changed from "
                << students[index].quizScores[quizIndex] << " to " << newScore << ".\n";
            //update the score in the student's record
            students[index].quizScores[quizIndex] = newScore;
            break;
        }

        case '9': {
            //initialize variable for project score index
            int projectIndex;
            //prompt user to specify the project score index to update
            do {
                cout << "\tSpecify the index (0...9) of the project array: ";
                cin >> projectIndex;
                //loop to ensure input index is within the valid range (0 to 9)
            } while (projectIndex < 0 || projectIndex > 9);

            //declare variable to store the new score
            float newScore;
            //prompt user to enter the new score for the specified index
            do {
                cout << "\tSpecify a new score#" << projectIndex << " : ";
                cin >> newScore;
                //validate new score is within the valid range (0.0 to 100.0)
                if (newScore < 0 || newScore > 100) {
                    cout << "ERROR: Invalid input. Must be from 0.0 to 100.0.\n";
                }
            } while (newScore < 0 || newScore > 100);
            //output confirmation of the score change
            cout << "\tproject[" << projectIndex << "] changed from "
                << students[index].projectScores[projectIndex] << " to " << newScore << ".\n";
            //update the score in the student's record
            students[index].projectScores[projectIndex] = newScore;
            break;
        }

        case '10': {
            //change exam scores
            int examIndex = -1; //initialize to an invalid value to ensure proper handling
            float newScore = -1.0; //initialize to an invalid value to ensure proper handling

            cout << "\tSpecify the index (0...2) of the exam array: ";
            //using a loop to validate the examIndex input
            while (!(cin >> examIndex) || examIndex < 0 || examIndex > 2) {
                cin.clear(); //clear error state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard bad input
                cout << "\tERROR: Invalid input. Must be from 0 to 2.\n";
                cout << "\tSpecify the index (0...2) of the exam array: ";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear any leftover input

            cout << "\tSpecify a new score#" << examIndex << " (0.0 to 100.0): ";
            //using a loop to validate the newScore input
            while (!(cin >> newScore) || newScore < 0.0 || newScore > 100.0) {
                cin.clear(); //clear error state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard bad input
                cout << "\tERROR: Invalid input. Must be from 0.0 to 100.0.\n";
                cout << "\tSpecify a new score#" << examIndex << " (0.0 to 100.0): ";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear any leftover input

            //perform the score update
            students[index].examScores[examIndex] = newScore;
            cout << "\texam[" << examIndex << "] changed from "
                << students[index].examScores[examIndex] << " to " << newScore << ".\n";

            //indicate that a change has been made, but do not commit yet
            changesCommitted = false;

            break;
        }


        case '0': {
            //uncommit changes
            cout << "\nNo change record(s) to the vector.\n";
            validInput = true;
            break;
        }
        case '1': {
            //commit changes
            cout << "\nChange(s) updated to student record at index(" << index << ") of the vector.\n";
            validInput = true;
            break;
        }
        default:
            cout << "\nERROR: Invalid option. Please select a valid option.\n";
            break;
        }

        system("pause");
    } while (!validInput);
}

//function to handle option #7 - write student data from vector to a binary file (courses.dat)
void writeStudentDataToFile() {
    //check if the file exists
    ifstream inFile("COURSES.DAT");
    if (inFile) {
        inFile.close();  //close the file if it exists
        if (remove("COURSES.DAT") != 0) {
            setTextColor("91");  //red text for error
            cout << "\n\tERROR: Unable to delete existing COURSES.DAT file.\n";
            resetTextColor();
            system("pause");
            return;
        }
        else {
            cout << "\n\COURSES.DAT file has been created.\n";
        }
    }

    //create and open a new file for writing
    ofstream outFile("COURSES.DAT", ios::binary | ios::out);  //open the file in binary write mode
    if (!outFile) {
        setTextColor("91");  //red text for error
        cout << "\n\tERROR: Unable to create a new COURSES.DAT file for writing.\n";
        resetTextColor();
        system("pause");
        return;
    }

    //iterate over students vector and write each student to the binary file
    for (const Student& student : students) {
        size_t idLength = student.studentID.size();  //get the size of studentid string
        outFile.write(reinterpret_cast<const char*>(&idLength), sizeof(idLength));  //write the size of studentid
        outFile.write(student.studentID.c_str(), idLength);  //write the actual studentid string

        size_t nameLength = student.name.size();  //get the size of name string
        outFile.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));  //write the size of name
        outFile.write(student.name.c_str(), nameLength);  //write the actual name string

        size_t birthDateLength = student.birthDate.size();  //get the size of birthdate string
        outFile.write(reinterpret_cast<const char*>(&birthDateLength), sizeof(birthDateLength));  //write the size of birthdate
        outFile.write(student.birthDate.c_str(), birthDateLength);  //write the actual birthdate string

        outFile.write(reinterpret_cast<const char*>(&student.collegeLevel), sizeof(student.collegeLevel));  //write collegelevel
        outFile.write(reinterpret_cast<const char*>(&student.ignoreLowestScore), sizeof(student.ignoreLowestScore));  //write ignorelowestscore

        size_t termLength = student.term.size();  //get the size of term string
        outFile.write(reinterpret_cast<const char*>(&termLength), sizeof(termLength));  //write the size of term
        outFile.write(student.term.c_str(), termLength);  //write the actual term string
    }

    outFile.close();  //close the file after writing all data
    cout << "\n\tStudent data has been successfully written to a new COURSES.DAT file.\n";
    system("pause");
}

void vectorImplementationMenu() {
    char userInput[10];
    bool validInput = false;

    do {
        system("cls");  //clear screen before showing sub-menu

        //display the vector implementation menu in light yellow
        setTextColor("93");
        cout << "\tOption A - Vector Implementation\n";
        cout << string(75, char(205)) << "\n";
        cout << "\t1. Read or create a binary file (COURSES.DAT) and store into a vector\n";
        cout << "\t2. Add a new student\n";
        cout << "\t3. Search for a student\n";
        cout << "\t4. Delete a student (5pts EXTRA)\n";
        cout << "\t5. Update a student\n";
        cout << "\t6. Display ALL student(s)\n";
        cout << "\t7. Write student data from vector to a binary file\n";
        cout << string(75, char(196)) << "\n";
        cout << "\t0. Return\n";
        cout << string(75, char(205)) << "\n";

        resetTextColor();  //reset after displaying menu

        //option prompt in white
        setTextColor("97");
        cout << "\tOption: ";
        cin >> userInput;
        cin.ignore();  //clear newline character from input buffer

        //validate input
        if (strlen(userInput) == 1 && (userInput[0] >= '0' && userInput[0] <= '7')) {
            switch (userInput[0]) {
            case '1':
                readOrCreateFile();  //call function to read or create the binary file
                break;
            case '2':
                addNewStudent();  //call function to add a new student
                break;
            case '3': {
                //search for a student by student id
                if (students.empty()) {
                    setTextColor("91");  //red text for error
                    cout << "\n\tERROR: Vector is empty.\n";
                    resetTextColor();
                }
                else {
                    string searchStudentID;
                    cout << "\n\tSearch for a student from the vector\n";
                    cout << string(75, char(196)) << "\n";

                    //get the student id input from the user
                    cout << "\tSpecify a student ID (CCnnnnnn) to search: ";
                    getline(cin, searchStudentID);

                    //validate the input format (ccnnnnnn)
                    regex idPattern("^[A-Z]{2}\\d{6}$");
                    if (!validateInput(searchStudentID, idPattern, "\tERROR: Student ID must be in the format CCnnnnnn (e.g., AA123456).")) {
                        system("pause");
                        break;
                    }

                    //iterate through the vector to search for the student id
                    bool found = false;
                    for (const auto& student : students) {
                        if (student.studentID == searchStudentID) {
                            found = true;
                            cout << "\n\tFound student record in the vector.\n";
                            break;
                        }
                    }

                    if (!found) {
                        setTextColor("91");  //red text for error
                        cout << "\n\tERROR: Student with ID [" << searchStudentID << "] not found in the vector.\n";
                        resetTextColor();
                    }
                }

                //pause and return to the menu
                system("pause");
                break;
            }

            case '4': {
                //delete a student by student id
                if (students.empty()) {
                    setTextColor("91");  //red text for error
                    cout << "\n\tERROR: Vector is empty.\n";
                    resetTextColor();
                }
                else {
                    string deleteStudentID;
                    cout << "\n\tDelete a student from the vector\n";
                    cout << string(75, char(196)) << "\n";

                    //get the student id input from the user
                    cout << "\tSpecify a student ID (CCnnnnnn) to search and delete: ";
                    getline(cin, deleteStudentID);

                    //validate the input format (ccnnnnnn)
                    regex idPattern("^[A-Z]{2}\\d{6}$");
                    if (!validateInput(deleteStudentID, idPattern, "\tERROR: Student ID must be in the format CCnnnnnn (e.g., AA123456).")) {
                        system("pause");
                        break;
                    }

                    //iterate through the vector to search for the student id
                    bool found = false;
                    for (size_t i = 0; i < students.size(); ++i) {
                        if (students[i].studentID == deleteStudentID) {
                            found = true;
                            //confirm deletion
                            char confirmDelete;
                            cout << "\tAre you sure you want to delete this student (Y/N)? ";
                            cin >> confirmDelete;
                            confirmDelete = toupper(confirmDelete);

                            if (confirmDelete == 'Y') {
                                students.erase(students.begin() + i);  //delete the student from the vector
                                cout << "\n\tDeleted student record from the vector.\n";
                            }
                            else {
                                cout << "\n\tDeletion canceled.\n";
                            }

                            break;
                        }
                    }

                    if (!found) {
                        setTextColor("91");  //red text for error
                        cout << "\n\tERROR: Student with ID [" << deleteStudentID << "] not found in the vector.\n";
                        resetTextColor();
                    }
                }

                //pause and return to the menu
                system("pause");
                break;
            }

            case '5': {
                //update a student by student id
                if (students.empty()) {
                    setTextColor("91");  //red text for error
                    cout << "\n\tERROR: Vector is empty.\n";
                    resetTextColor();
                }
                else {
                    string updateStudentID;
                    cout << "\n\tUpdate a student from the vector\n";
                    cout << string(75, char(196)) << "\n";

                    //get the student id input from the user
                    cout << "\tSpecify a student ID (CCnnnnnn) to search and update: ";
                    getline(cin, updateStudentID);

                    //validate the input format (ccnnnnnn)
                    regex idPattern("^[A-Z]{2}\\d{6}$");
                    if (!validateInput(updateStudentID, idPattern, "\tERROR: Student ID must be in the format CCnnnnnn (e.g., AA123456).")) {
                        system("pause");
                        break;
                    }

                    //iterate through the vector to search for the student id
                    bool found = false;
                    for (size_t i = 0; i < students.size(); ++i) {
                        if (students[i].studentID == updateStudentID) {
                            found = true;
                            cout << "\n\tFound student record in the vector.\n";
                            system("pause");

                            //after finding the student, open the update form
                            updateStudentForm(i);
                            break;
                        }
                    }

                    if (!found) {
                        setTextColor("91");  //red text for error
                        cout << "\n\tERROR: Student with ID [" << updateStudentID << "] not found in the vector.\n";
                        resetTextColor();
                    }
                }

                //pause and return to the menu
                system("pause");
                break;
            }

            case '6': {
                if (students.empty()) {
                    setTextColor("91");  //red text for error
                    cout << "\n\tERROR: Vector is empty.\n";
                    resetTextColor();
                }
                else {
                    setTextColor("97");  //white for the display header
                    cout << "\n\tDisplay all students from the vector\n";
                    cout << string(75, char(196)) << "\n";
                    resetTextColor();

                    //output the number of students in the vector
                    cout << "\n\tNumber of student(s) in the vector: " << students.size() << "\n\n";

                    //iterate through the vector and display all student details in the required format
                    for (const auto& student : students) {
                        setTextColor("97");  //white for headers
                        cout << "\tStudent ID    : ";
                        setTextColor("32");  //green for student id
                        cout << student.studentID << "\n";

                        setTextColor("97");  //white for headers
                        cout << "\tName          : ";
                        setTextColor("32");  //green for student name
                        cout << student.name << "\n";

                        setTextColor("97");  //white for headers
                        cout << "\tDate of birth : ";
                        setTextColor("32");  //green for date of birth
                        cout << student.birthDate << "\n";

                        //class level and term headers in white, values in green
                        setTextColor("97");  //white text for headers
                        cout << "\tClass Level   : ";
                        setTextColor("32");  //green for values
                        cout << (student.collegeLevel == 0 ? "Unknown" : (student.collegeLevel == 1 ? "Freshman" : (student.collegeLevel == 2 ? "Sophomore" : (student.collegeLevel == 3 ? "Junior" : "Senior")))) << "\n";

                        setTextColor("97");  //white text for headers
                        cout << "\tTerm          : ";
                        setTextColor("32");  //green for term value
                        cout << student.term << "\n";

                        //letter grade header in white, value in green
                        setTextColor("97");  //white text for headers
                        cout << "\n\tLETTER GRADE  : ";
                        setTextColor("32");  //green for letter grade
                        cout << student.letterGrade << " (" << student.examAvg << "%)\n";

                        //ignore lowest score, "true" in green
                        setTextColor("97");  //white text for header
                        cout << "\n\tIgnore lowest score  : ";
                        setTextColor("32");  //green for the value "true"
                        cout << (student.ignoreLowestScore ? "true" : "false") << "\n";

                        //discussion scores and average, header in white, values in green
                        setTextColor("97");  //white text for headers
                        cout << "\n\tDiscussion scores : ";
                        setTextColor("32");  //green for values
                        for (int i = 0; i < 10; i++) {
                            cout << (i != 0 ? " " : "") << student.discussionScores[i] << "  ";
                        }

                        setTextColor("97");  //white text for headers
                        cout << "\n\tAvg score : ";
                        setTextColor("32");  //green for values
                        cout << student.discussionAvg << "\n";

                        //quiz scores and average, header in white, values in green
                        setTextColor("97");  //white text for headers
                        cout << "\n\tQuiz scores       : ";
                        setTextColor("32");  //green for values
                        for (int i = 0; i < 10; i++) {
                            cout << (i != 0 ? " " : "") << student.quizScores[i] << "  ";
                        }

                        setTextColor("97");  //white text for headers
                        cout << "\n\tAvg score : ";
                        setTextColor("32");  //green for values
                        cout << student.quizAvg << "\n";

                        //project scores and average, header in white, values in green
                        setTextColor("97");  //white text for headers
                        cout << "\n\tProject scores    : ";
                        setTextColor("32");  //green for values
                        for (int i = 0; i < 10; i++) {
                            cout << (i != 0 ? " " : "") << student.projectScores[i] << "  ";
                        }

                        setTextColor("97");  //white text for headers
                        cout << "\n\tAvg score : ";
                        setTextColor("32");  //green for values
                        cout << student.projectAvg << "\n";

                        //exam scores and average, header in white, values in green
                        setTextColor("97");  //white text for headers
                        cout << "\n\tExam scores       : ";
                        setTextColor("32");  //green for values
                        for (int i = 0; i < 3; i++) {
                            cout << (i != 0 ? " " : "") << student.examScores[i] << "  ";
                        }

                        setTextColor("97");  //white text for headers
                        cout << "\n\tAvg score : ";
                        setTextColor("32");  //green for values
                        cout << student.examAvg << "\n";

                        resetTextColor();  //reset to default color after the student details are printed
                        cout << "\n";  //newline after each student's details
                    }
                }
                system("pause");
                break;
            }


            case '7': {
                if (students.empty()) {
                    setTextColor("91");  //red text for error
                    cout << "\n\tERROR: Vector is empty.\n";
                    resetTextColor();
                }
                else {
                    writeStudentDataToFile();  //write student data to courses.dat
                }
                break;
            }
            case '0':
                //if user selects 0, return to the main menu
                cout << "\nReturning to Main Menu...\n";
                validInput = true;
                break;
            default:
                break;
            }
        }
        else {
            //invalid input, display error message
            cout << "\n\tERROR: Invalid input. Must be a number from 0 to 7.\n\n";
        }

        resetTextColor();  //reset after displaying error
        system("pause");
    } while (!validInput);
}

//function to add a new student to the array
void addNewStudentArray() {
    if (currentStudentCount >= MAX_STUDENTS) {
        cout << "\n\tERROR: Maximum number of students reached.\n";
        return;
    }

    system("cls");

    setTextColor("93");  //set text color to light yellow for prompt
    cout << "\tAdd a new student to the array\n";
    cout << string(75, char(196)) << "\n";
    resetTextColor();  //reset color after displaying menu

    StudentArray* newStudent = new StudentArray;
    string input;

    //1. specify a new student id number (ccnnnnnn)
    regex idPattern("^[A-Z]{2}\\d{6}$");  //ccnnnnnn format
    do {
        cout << "\tSpecify a new Student ID number (CCnnnnnn): ";
        getline(cin, input);
    } while (!validateInput(input, idPattern, "\tERROR: Student ID must be in the format CCnnnnnn (e.g., AA123456)."));

    newStudent->studentID = input;

    //2. specify a name
    cout << "\tSpecify a name: ";
    getline(cin, newStudent->name);

    //3. specify a birth date (mm/dd/yyyy)
    regex birthDatePattern("^\\d{2}/\\d{2}/\\d{4}$");  //mm/dd/yyyy format
    do {
        cout << "\tSpecify a birth date (mm/dd/yyyy): ";
        getline(cin, input);
    } while (!validateInput(input, birthDatePattern, "\tERROR: Birth date must be in the format mm/dd/yyyy (e.g., 09/04/1971)."));

    newStudent->birthDate = input;

    //4. specify a college level (0-unknown, 1-freshman, 2-sophomore, 3-junior, or 4-senior)
    do {
        cout << "\tSpecify a college level (0-unknown, 1-Freshman, 2-Sophomore, 3-Junior, or 4-Senior): ";
        getline(cin, input);
    } while (!validateInput(input, regex("^[0-4]$"), "\tERROR: College level must be a number between 0 and 4."));

    newStudent->collegeLevel = stoi(input);

    //5. specify a term (nnnncc)
    regex termPattern("^\\d{4}(SU|SP|FA)$");  //nnnnxx
    do {
        cout << "\tSpecify a term (nnnnXX): ";
        getline(cin, input);
    } while (!validateInput(input, termPattern, "\tERROR: Term must be in the format nnnnXX where XX is 'SU', 'SP', or 'FA'."));

    newStudent->term = input;

    //6. specify ignore the lowest score of assignments (0-false or 1-true)
    do {
        cout << "\tSpecify ignore the lowest score of assignments: (0-false or 1-true): ";
        getline(cin, input);
    } while (!validateInput(input, regex("^[01]$"), "\tERROR: Must be 0 (false) or 1 (true)."));

    newStudent->ignoreLowestScore = (input == "1");

    students2[currentStudentCount] = newStudent;
    currentStudentCount++;

    //confirm the student was added
    cout << "\n\tA new student record has been added to the array.\n";
    system("pause");
}

//function to handle option #1 - read or create a binary file specified by the user
void readOrCreateFileArray() {
    system("cls");

    setTextColor("93");  //set text color to light yellow for prompt
    cout << "\tRead data from binary file and store into the array\n";
    cout << string(75, char(196)) << "\n";
    resetTextColor();  //reset color after displaying menu

    //prompt the user to enter the binary data file name
    string fileName;
    cout << "\tSpecify the binary data file name (e.g., STUDENTS.DAT): ";
    cin >> fileName;  //use cin to read the input without getline to avoid issues

    //check if the user entered the correct file name (STUDENTS.dat)
    if (fileName != "STUDENTS.DAT") {
        //if the file name is incorrect, show an error message in red
        setTextColor("91");  //set text color to red for error
        cout << "\n\tERROR: Binary data file must be named 'STUDENTS.DAT'.\n";
        resetTextColor();  //reset color after displaying error
        system("pause");
        return;
    }

    //check if the file exists
    ifstream inFile(fileName, ios::binary);
    if (inFile) {
        //if file exists, read the data into the array
        inFile.close();
        cout << "\n\tCompleted read in student data from binary file, STUDENTS.DAT, and stored into the array.\n";
    }
    else {
        //if the file doesn't exist, create it
        ofstream outFile(fileName, ios::binary);
        if (!outFile) {
            setTextColor("91");  //red text for error
            cout << "\n\tERROR: Unable to create " << fileName << " file.\n";
            resetTextColor();
        }
        else {
            setTextColor("32");  //green text for success message
            cout << "\n\tNew STUDENTS.DAT file has been created.\n";
            resetTextColor();  //reset text color after message
            outFile.close();  //close the newly created file
        }
    }

    system("pause");
}

void updateStudentFormArray(int index) {
    //make sure the index is valid
    if (index < 0 || index >= currentStudentCount) {
        cout << "\n\tERROR: Invalid student index.\n";
        return;
    }

    StudentArray* student = students2[index];
    bool validInput = false;
    char option;

    do {
        system("cls");

        //display the student's current details
        setTextColor("97");  //white for headers
        cout << "\n\tStudent ID    : ";
        setTextColor("32");  //green for student data
        cout << student->studentID << "\n";

        setTextColor("97");  //white for headers
        cout << "\tName          : ";
        setTextColor("32");  //green for student data
        cout << student->name << "\n";

        setTextColor("97");  //white for headers
        cout << "\tDate of birth : ";
        setTextColor("32");  //green for student data
        cout << student->birthDate << "\n";

        setTextColor("97");  //white for headers
        cout << "\tClass Level   : ";
        string classLevels[] = { "Unknown", "Freshman", "Sophomore", "Junior", "Senior" };
        setTextColor("32");  //green for student data
        cout << classLevels[student->collegeLevel] << "\n";

        setTextColor("97");  //white for headers
        cout << "\tTerm          : ";
        setTextColor("32");  //green for student data
        cout << student->term << "\n";

        setTextColor("97");  //white for headers
        cout << "\tIgnore Lowest Score: ";
        setTextColor("32");  //green for student data
        cout << (student->ignoreLowestScore ? "true" : "false") << "\n";

        //display menu
        setTextColor("93");  //light yellow for menu
        cout << "\n\tEdit Student Menu\n";
        cout << string(75, char(205)) << "\n";
        cout << "\t2. Change Name\n";
        cout << "\t3. Change Birth Date\n";
        cout << "\t4. Change College Level\n";
        cout << "\t5. Change Term\n";
        cout << "\t6. Change Ignore Lowest score setting\n";
        cout << "\t7. Change Discussion scores\n";
        cout << "\t8. Change Quiz scores\n";
        cout << "\t9. Change Project scores\n";
        cout << "\t10. Change Exam scores\n";
        cout << string(75, char(196)) << "\n";
        cout << "\t0. Uncommit the change(s)\n";
        cout << "\t1. Commit the change(s)\n";
        cout << string(75, char(205)) << "\n";

        //option input
        setTextColor("97");  //white for option prompt
        cout << "\tOption: ";
        cin >> option;
        cin.ignore();  //clear newline character from input buffer

        switch (option) {
        case '2': {
            //change name
            cout << "\tSpecify a new name  : ";
            string newName;
            getline(cin, newName);
            cout << "\tName changed from " << student->name << " to " << newName << "\n";
            student->name = newName;
            break;
        }
        case '3': {
            //change birth date
            string newBirthDate;
            do {
                cout << "\tSpecify a new birth date (mm/dd/yyyy): ";
                getline(cin, newBirthDate);
                if (!validateInput(newBirthDate, regex("^\\d{2}/\\d{2}/\\d{4}$"), "\tERROR: Invalid date format.")) {
                    cout << "ERROR: Invalid input. Date format must be MM/DD/YYYY.\n";
                }
            } while (!validateInput(newBirthDate, regex("^\\d{2}/\\d{2}/\\d{4}$"), "\tERROR: Invalid date format."));
            cout << "\tBirth date changed from " << student->birthDate << " to " << newBirthDate << "\n";
            student->birthDate = newBirthDate;
            break;
        }
        case '4': {
            //change college level
            string newLevel;
            do {
                cout << "\tSpecify a new college level (0-unknown, 1-Freshman, 2-Sophomore, 3-Junior, or 4-Senior): ";
                getline(cin, newLevel);
                if (newLevel < "0" || newLevel > "4") {
                    cout << "ERROR: Invalid input. Must be between 0 and 4.\n";
                }
            } while (newLevel < "0" || newLevel > "4");
            int level = stoi(newLevel);
            cout << "\tCollege level changed to " << classLevels[level] << "\n";
            student->collegeLevel = level;
            break;
        }
        case '5': {
            //change term
            string newTerm;
            do {
                cout << "\tSpecify a new term (nnnnXX): ";
                getline(cin, newTerm);
                if (!validateInput(newTerm, regex("^\\d{4}(FA|SP|SU)$"), "\tERROR: Invalid term format.")) {
                    cout << "ERROR: Invalid input. Term must be in the format nnnnXX where XX is 'FA', 'SU', or 'SP'.\n";
                }
            } while (!validateInput(newTerm, regex("^\\d{4}(FA|SP|SU)$"), "\tERROR: Invalid term format."));
            cout << "\tTerm changed from " << student->term << " to " << newTerm << "\n";
            student->term = newTerm;
            break;
        }
        case '6': {
            //change ignore lowest score setting
            string newIgnore;
            do {
                cout << "\tSpecify ignore the lowest score of assignments: (1-true or 0-false): ";
                getline(cin, newIgnore);
                if (newIgnore != "1" && newIgnore != "0") {
                    cout << "ERROR: Invalid input. Must be 1 (true) or 0 (false).\n";
                }
            } while (newIgnore != "1" && newIgnore != "0");
            cout << "\tIgnore lowest score changed to " << (newIgnore == "1" ? "true" : "false") << "\n";
            student->ignoreLowestScore = (newIgnore == "1");
            break;
        }
        case '7': {
            //change discussion scores
            int discussionIndex;
            do {
                cout << "\tSpecify the index (0...9) of the discussion array: ";
                cin >> discussionIndex;
            } while (discussionIndex < 0 || discussionIndex > 9);

            float newScore;
            do {
                cout << "\tSpecify a new score#" << discussionIndex << " : ";
                cin >> newScore;
                if (newScore < 0 || newScore > 10) {
                    cout << "ERROR: Invalid input. Must be from 0.0..10.0.\n";
                }
            } while (newScore < 0 || newScore > 10);
            cout << "\tdiscussion[" << discussionIndex << "] changed from " << student->discussionScores[discussionIndex] << " to " << newScore << ".\n";
            student->discussionScores[discussionIndex] = newScore;
            break;
        }
        case '8': {
            //change quiz scores
            int quizIndex;
            do {
                cout << "\tSpecify the index (0...9) of the quiz array: ";
                cin >> quizIndex;
            } while (quizIndex < 0 || quizIndex > 9);

            float newScore;
            do {
                cout << "\tSpecify a new score#" << quizIndex << " : ";
                cin >> newScore;
                if (newScore < 0 || newScore > 30) {
                    cout << "ERROR: Invalid input. Must be from 0.0..30.0.\n";
                }
            } while (newScore < 0 || newScore > 30);
            cout << "\tquiz[" << quizIndex << "] changed from " << student->quizScores[quizIndex] << " to " << newScore << ".\n";
            student->quizScores[quizIndex] = newScore;
            break;
        }
        case '9': {
            //change project scores
            int projectIndex;
            do {
                cout << "\tSpecify the index (0...9) of the project array: ";
                cin >> projectIndex;
            } while (projectIndex < 0 || projectIndex > 9);

            float newScore;
            do {
                cout << "\tSpecify a new score#" << projectIndex << " : ";
                cin >> newScore;
                if (newScore < 0 || newScore > 100) {
                    cout << "ERROR: Invalid input. Must be from 0.0..100.0.\n";
                }
            } while (newScore < 0 || newScore > 100);
            cout << "\tproject[" << projectIndex << "] changed from " << student->projectScores[projectIndex] << " to " << newScore << ".\n";
            student->projectScores[projectIndex] = newScore;
            break;
        }
        case '10': {
            //change exam scores
            int examIndex;
            do {
                cout << "\tSpecify the index (0...2) of the exam array: ";
                cin >> examIndex;
            } while (examIndex < 0 || examIndex > 2);

            float newScore;
            do {
                cout << "\tSpecify a new score#" << examIndex << " : ";
                cin >> newScore;
                if (newScore < 0 || newScore > 100) {
                    cout << "ERROR: Invalid input. Must be from 0.0..100.0.\n";
                }
            } while (newScore < 0 || newScore > 100);
            cout << "\texam[" << examIndex << "] changed from " << student->examScores[examIndex] << " to " << newScore << ".\n";
            student->examScores[examIndex] = newScore;
            break;
        }
        case '0': {
            //uncommit changes
            cout << "\nNo changes were made to the student record.\n";
            validInput = true;  //exit the update menu
            break;
        }
        case '1': {
            //commit changes
            cout << "\nChanges have been updated to the student record at index (" << index << ").\n";
            validInput = true;  //exit the update menu
            break;
        }
        default:
            cout << "\nERROR: Invalid option. Please select a valid option.\n";
            break;
        }

        system("pause");
    } while (!validInput);
}


//function to delete a student by index
void deleteStudentArray(int index) {
    if (index < 0 || index >= currentStudentCount) {
        cout << "\n\tERROR: Invalid student index.\n";
        return;
    }

    delete students2[index];  //delete the student object
    for (int i = index; i < currentStudentCount - 1; ++i) {
        students2[i] = students2[i + 1];  //shift the students left
    }
    currentStudentCount--;
    cout << "\n\tStudent has been deleted.\n";
}

//function to write student data from array to a binary file
void writeStudentDataToFileArray() {
    ofstream outFile("STUDENTS.DAT", ios::binary);
    if (!outFile) {
        cout << "\n\tERROR: Unable to create file.\n";
        return;
    }

    for (int i = 0; i < currentStudentCount; ++i) {
        outFile.write(reinterpret_cast<char*>(students2[i]), sizeof(StudentArray));
    }
    outFile.close();
    cout << "\n\tStudent data successfully written to STUDENTS.DAT.\n";
}

void arrayImplementationMenu() {
    char userInput[10];
    bool validInput = false;

    do {
        system("cls");  //clear screen before showing sub-menu

        //display the array implementation menu in light yellow
        setTextColor("93");
        cout << "\tOption B - Dynamic Memory Allocated Array Implementation\n";
        cout << string(75, char(205)) << "\n";
        cout << "\t1. Read or create a binary file (STUDENTS.DAT) and store into an array\n";
        cout << "\t2. Add a new student\n";
        cout << "\t3. Search for a student\n";
        cout << "\t4. Delete a student (5pts EXTRA)\n";
        cout << "\t5. Update a student\n";
        cout << "\t6. Display ALL student(s)\n";
        cout << "\t7. Write student data from array to a binary file\n";
        cout << string(75, char(196)) << "\n";
        cout << "\t0. Return\n";
        cout << string(75, char(205)) << "\n";

        resetTextColor();  //reset after displaying menu

        //option prompt in white
        setTextColor("97");
        cout << "\tOption: ";
        cin >> userInput;
        cin.ignore();  //clear newline character from input buffer

        //validate input
        if (strlen(userInput) == 1 && (userInput[0] >= '0' && userInput[0] <= '7')) {
            switch (userInput[0]) {
            case '1':
                readOrCreateFileArray();  //call function to read or create the binary file
                break;
            case '2':
                addNewStudentArray();  //call function to add a new student
                break;
            case '3': {
                //search for a student by student id
                if (currentStudentCount == 0) {
                    setTextColor("91");  //red text for error
                    cout << "\n\tERROR: Array is empty.\n";
                    resetTextColor();
                }
                else {
                    string searchStudentID;
                    cout << "\n\tSearch for a student from the array\n";
                    cout << string(75, char(196)) << "\n";

                    //get the student id input from the user
                    cout << "\tSpecify a student ID (CCnnnnnn) to search: ";
                    getline(cin, searchStudentID);

                    //validate the input format (ccnnnnnn)
                    regex idPattern("^[A-Z]{2}\\d{6}$");
                    if (!validateInput(searchStudentID, idPattern, "\tERROR: Student ID must be in the format CCnnnnnn (e.g., AA123456).")) {
                        system("pause");
                        break;
                    }

                    //iterate through the array to search for the student id
                    bool found = false;
                    for (int i = 0; i < currentStudentCount; ++i) {
                        if (students2[i]->studentID == searchStudentID) {
                            found = true;
                            cout << "\n\tFound student record in the array.\n";
                            break;
                        }
                    }

                    if (!found) {
                        setTextColor("91");  //red text for error
                        cout << "\n\tERROR: Student with ID [" << searchStudentID << "] not found in the array.\n";
                        resetTextColor();
                    }
                }

                //pause and return to the menu
                system("pause");
                break;
            }

            case '4': {
                //delete a student by student id
                if (currentStudentCount == 0) {
                    setTextColor("91");  //red text for error
                    cout << "\n\tERROR: Array is empty.\n";
                    resetTextColor();
                }
                else {
                    string deleteStudentID;
                    cout << "\n\tDelete a student from the array\n";
                    cout << string(75, char(196)) << "\n";

                    //get the student id input from the user
                    cout << "\tSpecify a student ID (CCnnnnnn) to search and delete: ";
                    getline(cin, deleteStudentID);

                    //validate the input format (ccnnnnnn)
                    regex idPattern("^[A-Z]{2}\\d{6}$");
                    if (!validateInput(deleteStudentID, idPattern, "\tERROR: Student ID must be in the format CCnnnnnn (e.g., AA123456).")) {
                        system("pause");
                        break;
                    }

                    //iterate through the array to search for the student id
                    bool found = false;
                    for (int i = 0; i < currentStudentCount; ++i) {
                        if (students2[i]->studentID == deleteStudentID) {
                            found = true;
                            //confirm deletion
                            char confirmDelete;
                            cout << "\tAre you sure you want to delete this student (Y/N)? ";
                            cin >> confirmDelete;
                            confirmDelete = toupper(confirmDelete);

                            if (confirmDelete == 'Y') {
                                delete students2[i];
                                for (int j = i; j < currentStudentCount - 1; j++) {
                                    students2[j] = students2[j + 1];  //shift elements left
                                }
                                currentStudentCount--;
                                cout << "\n\tDeleted student record from the array.\n";
                            }
                            else {
                                cout << "\n\tDeletion canceled.\n";
                            }

                            break;
                        }
                    }

                    if (!found) {
                        setTextColor("91");  //red text for error
                        cout << "\n\tERROR: Student with ID [" << deleteStudentID << "] not found in the array.\n";
                        resetTextColor();
                    }
                }

                //pause and return to the menu
                system("pause");
                break;
            }

            case '5': {
                //update a student by student id
                if (currentStudentCount == 0) {
                    setTextColor("91");  //red text for error
                    cout << "\n\tERROR: Array is empty.\n";
                    resetTextColor();
                }
                else {
                    string updateStudentID;
                    cout << "\n\tUpdate a student from the array\n";
                    cout << string(75, char(196)) << "\n";

                    //get the student id input from the user
                    cout << "\tSpecify a student ID (CCnnnnnn) to search and update: ";
                    getline(cin, updateStudentID);

                    //validate the input format (ccnnnnnn)
                    regex idPattern("^[A-Z]{2}\\d{6}$");
                    if (!validateInput(updateStudentID, idPattern, "\tERROR: Student ID must be in the format CCnnnnnn (e.g., AA123456).")) {
                        system("pause");
                        break;
                    }

                    //iterate through the array to search for the student id
                    bool found = false;
                    for (int i = 0; i < currentStudentCount; ++i) {
                        if (students2[i]->studentID == updateStudentID) {
                            found = true;
                            cout << "\n\tFound student record in the array.\n";
                            system("pause");

                            //after finding the student, open the update form
                            updateStudentFormArray(i);
                            break;
                        }
                    }

                    if (!found) {
                        setTextColor("91");  //red text for error
                        cout << "\n\tERROR: Student with ID [" << updateStudentID << "] not found in the array.\n";
                        resetTextColor();
                    }
                }

                //pause and return to the menu
                system("pause");
                break;
            }

            case '6': {
                if (currentStudentCount == 0) {
                    setTextColor("91");  //red text for error
                    cout << "\n\tERROR: Array is empty.\n";
                    resetTextColor();
                }
                else {
                    setTextColor("97");  //white for the display header
                    cout << "\n\tDisplay all students from the array\n";
                    cout << string(75, char(196)) << "\n";
                    resetTextColor();

                    //output the number of students in the array
                    cout << "\n\tNumber of student(s) in the array: " << currentStudentCount << "\n\n";

                    //iterate through the array and display all student details in the required format
                    for (int i = 0; i < currentStudentCount; ++i) {
                        setTextColor("97");  //white for headers
                        cout << "\tStudent ID    : ";
                        setTextColor("32");  //green for student id
                        cout << students2[i]->studentID << "\n";

                        setTextColor("97");  //white for headers
                        cout << "\tName          : ";
                        setTextColor("32");  //green for student name
                        cout << students2[i]->name << "\n";

                        setTextColor("97");  //white for headers
                        cout << "\tDate of birth : ";
                        setTextColor("32");  //green for date of birth
                        cout << students2[i]->birthDate << "\n";

                        //class level and term headers in white, values in green
                        setTextColor("97");  //white text for headers
                        cout << "\tClass Level   : ";
                        setTextColor("32");  //green for values
                        cout << (students2[i]->collegeLevel == 0 ? "Unknown" : (students2[i]->collegeLevel == 1 ? "Freshman" : (students2[i]->collegeLevel == 2 ? "Sophomore" : (students2[i]->collegeLevel == 3 ? "Junior" : "Senior")))) << "\n";

                        setTextColor("97");  //white text for headers
                        cout << "\tTerm          : ";
                        setTextColor("32");  //green for term value
                        cout << students2[i]->term << "\n";

                        //letter grade header in white, value in green
                        setTextColor("97");  //white text for headers
                        cout << "\n\tLETTER GRADE  : ";
                        setTextColor("32");  //green for letter grade
                        cout << students2[i]->letterGrade << " (" << students2[i]->examAvg << "%)\n";

                        //ignore lowest score, "true" in green
                        setTextColor("97");  //white text for header
                        cout << "\n\tIgnore lowest score  : ";
                        setTextColor("32");  //green for the value "true"
                        cout << (students2[i]->ignoreLowestScore ? "true" : "false") << "\n";

                        //discussion scores and average, header in white, values in green
                        setTextColor("97");  //white text for headers
                        cout << "\n\tDiscussion scores : ";
                        setTextColor("32");  //green for values
                        for (int j = 0; j < 10; j++) {
                            cout << (j != 0 ? " " : "") << students2[i]->discussionScores[j] << "  ";
                        }

                        setTextColor("97");  //white text for headers
                        cout << "\n\tAvg score : ";
                        setTextColor("32");  //green for values
                        cout << students2[i]->discussionAvg << "\n";

                        //quiz scores and average, header in white, values in green
                        setTextColor("97");  //white text for headers
                        cout << "\n\tQuiz scores       : ";
                        setTextColor("32");  //green for values
                        for (int j = 0; j < 10; j++) {
                            cout << (j != 0 ? " " : "") << students2[i]->quizScores[j] << "  ";
                        }

                        setTextColor("97");  //white text for headers
                        cout << "\n\tAvg score : ";
                        setTextColor("32");  //green for values
                        cout << students2[i]->quizAvg << "\n";

                        //project scores and average, header in white, values in green
                        setTextColor("97");  //white text for headers
                        cout << "\n\tProject scores    : ";
                        setTextColor("32");  //green for values
                        for (int j = 0; j < 10; j++) {
                            cout << (j != 0 ? " " : "") << students2[i]->projectScores[j] << "  ";
                        }

                        setTextColor("97");  //white text for headers
                        cout << "\n\tAvg score : ";
                        setTextColor("32");  //green for values
                        cout << students2[i]->projectAvg << "\n";

                        //exam scores and average, header in white, values in green
                        setTextColor("97");  //white text for headers
                        cout << "\n\tExam scores       : ";
                        setTextColor("32");  //green for values
                        for (int j = 0; j < 3; j++) {
                            cout << (j != 0 ? " " : "") << students2[i]->examScores[j] << "  ";
                        }

                        setTextColor("97");  //white text for headers
                        cout << "\n\tAvg score : ";
                        setTextColor("32");  //green for values
                        cout << students2[i]->examAvg << "\n";

                        resetTextColor();  //reset to default color after the student details are printed
                        cout << "\n";  //newline after each student's details
                    }
                }
                system("pause");
                break;
            }

            case '7': {
                if (currentStudentCount == 0) {
                    setTextColor("91");  //red text for error
                    cout << "\n\tERROR: Array is empty.\n";
                    resetTextColor();
                }
                else {
                    writeStudentDataToFileArray();  //write student data to courses.dat
                }
                break;
            }

            case '0':
                //if user selects 0, return to the main menu
                cout << "\nReturning to Main Menu...\n";
                validInput = true;
                break;

            default:
                break;
            }
        }
        else {
            //invalid input, display error message
            cout << "\n\tERROR: Invalid input. Must be a number from 0 to 7.\n\n";
        }

        resetTextColor();  //reset after displaying error
        system("pause");
    } while (!validInput);
}

// file //
void updateStudentFormFile(int index) {
    // make sure the index is valid
    if (index < 0 || index >= currentStudentCount) {
        cout << "\n\tERROR: Invalid student index.\n";
        return;
    }

    StudentFile* student = students3[index];
    bool validInput = false;
    char option;

    do {
        system("cls");

        // display the student's current details
        setTextColor("97");  // white for headers
        cout << "\n\tStudent ID    : ";
        setTextColor("32");  // green for student data
        cout << student->studentID << "\n";

        setTextColor("97");  // white for headers
        cout << "\tName          : ";
        setTextColor("32");  // green for student data
        cout << student->name << "\n";

        setTextColor("97");  // white for headers
        cout << "\tDate of birth : ";
        setTextColor("32");  // green for student data
        cout << student->birthDate << "\n";

        setTextColor("97");  // white for headers
        cout << "\tClass Level   : ";
        string classLevels[] = { "Unknown", "Freshman", "Sophomore", "Junior", "Senior" };
        setTextColor("32");  // green for student data
        cout << classLevels[student->collegeLevel] << "\n";

        setTextColor("97");  // white for headers
        cout << "\tTerm          : ";
        setTextColor("32");  // green for student data
        cout << student->term << "\n";

        setTextColor("97");  // white for headers
        cout << "\tIgnore Lowest Score: ";
        setTextColor("32");  // green for student data
        cout << (student->ignoreLowestScore ? "true" : "false") << "\n";

        // display menu
        setTextColor("93");  // light yellow for menu
        cout << "\n\tEdit Student Menu\n";
        cout << string(75, char(205)) << "\n";
        cout << "\t2. Change Name\n";
        cout << "\t3. Change Birth Date\n";
        cout << "\t4. Change College Level\n";
        cout << "\t5. Change Term\n";
        cout << "\t6. Change Ignore Lowest score setting\n";
        cout << "\t7. Change Discussion scores\n";
        cout << "\t8. Change Quiz scores\n";
        cout << "\t9. Change Project scores\n";
        cout << "\t10. Change Exam scores\n";
        cout << string(75, char(196)) << "\n";
        cout << "\t0. Uncommit the change(s)\n";
        cout << "\t1. Commit the change(s)\n";
        cout << string(75, char(205)) << "\n";

        // option input
        setTextColor("97");  // white for option prompt
        cout << "\tOption: ";
        cin >> option;
        cin.ignore();  // clear newline character from input buffer

        switch (option) {
        case '2': {
            // change name
            cout << "\tSpecify a new name  : ";
            string newName;
            getline(cin, newName);
            cout << "\tName changed from " << student->name << " to " << newName << "\n";
            student->name = newName;
            break;
        }
        case '3': {
            // change birth date
            string newBirthDate;
            do {
                cout << "\tSpecify a new birth date (mm/dd/yyyy): ";
                getline(cin, newBirthDate);
                if (!validateInput(newBirthDate, regex("^\\d{2}/\\d{2}/\\d{4}$"), "\tERROR: Invalid date format.")) {
                    cout << "ERROR: Invalid input. Date format must be MM/DD/YYYY.\n";
                }
            } while (!validateInput(newBirthDate, regex("^\\d{2}/\\d{2}/\\d{4}$"), "\tERROR: Invalid date format."));
            cout << "\tBirth date changed from " << student->birthDate << " to " << newBirthDate << "\n";
            student->birthDate = newBirthDate;
            break;
        }
        case '4': {
            // change college level
            string newLevel;
            do {
                cout << "\tSpecify a new college level (0-unknown, 1-Freshman, 2-Sophomore, 3-Junior, or 4-Senior): ";
                getline(cin, newLevel);
                if (newLevel < "0" || newLevel > "4") {
                    cout << "ERROR: Invalid input. Must be between 0 and 4.\n";
                }
            } while (newLevel < "0" || newLevel > "4");
            int level = stoi(newLevel);
            cout << "\tCollege level changed to " << classLevels[level] << "\n";
            student->collegeLevel = level;
            break;
        }
        case '5': {
            // change term
            string newTerm;
            do {
                cout << "\tSpecify a new term (nnnnXX): ";
                getline(cin, newTerm);
                if (!validateInput(newTerm, regex("^\\d{4}(FA|SP|SU)$"), "\tERROR: Invalid term format.")) {
                    cout << "ERROR: Invalid input. Term must be in the format nnnnXX where XX is 'FA', 'SU', or 'SP'.\n";
                }
            } while (!validateInput(newTerm, regex("^\\d{4}(FA|SP|SU)$"), "\tERROR: Invalid term format."));
            cout << "\tTerm changed from " << student->term << " to " << newTerm << "\n";
            student->term = newTerm;
            break;
        }
        case '6': {
            // change ignore lowest score setting
            string newIgnore;
            do {
                cout << "\tSpecify ignore the lowest score of assignments: (1-true or 0-false): ";
                getline(cin, newIgnore);
                if (newIgnore != "1" && newIgnore != "0") {
                    cout << "ERROR: Invalid input. Must be 1 (true) or 0 (false).\n";
                }
            } while (newIgnore != "1" && newIgnore != "0");
            cout << "\tIgnore lowest score changed to " << (newIgnore == "1" ? "true" : "false") << "\n";
            student->ignoreLowestScore = (newIgnore == "1");
            break;
        }
        case '7': {
            // change discussion scores
            int discussionIndex;
            do {
                cout << "\tSpecify the index (0...9) of the discussion array: ";
                cin >> discussionIndex;
            } while (discussionIndex < 0 || discussionIndex > 9);

            float newScore;
            do {
                cout << "\tSpecify a new score#" << discussionIndex << " : ";
                cin >> newScore;
                if (newScore < 0 || newScore > 10) {
                    cout << "ERROR: Invalid input. Must be from 0.0..10.0.\n";
                }
            } while (newScore < 0 || newScore > 10);
            cout << "\tdiscussion[" << discussionIndex << "] changed from " << student->discussionScores[discussionIndex] << " to " << newScore << ".\n";
            student->discussionScores[discussionIndex] = newScore;
            break;
        }
        case '8': {
            // change quiz scores
            int quizIndex;
            do {
                cout << "\tSpecify the index (0...9) of the quiz array: ";
                cin >> quizIndex;
            } while (quizIndex < 0 || quizIndex > 9);

            float newScore;
            do {
                cout << "\tSpecify a new score#" << quizIndex << " : ";
                cin >> newScore;
                if (newScore < 0 || newScore > 30) {
                    cout << "ERROR: Invalid input. Must be from 0.0..30.0.\n";
                }
            } while (newScore < 0 || newScore > 30);
            cout << "\tquiz[" << quizIndex << "] changed from " << student->quizScores[quizIndex] << " to " << newScore << ".\n";
            student->quizScores[quizIndex] = newScore;
            break;
        }
        case '9': {
            // change project scores
            int projectIndex;
            do {
                cout << "\tSpecify the index (0...9) of the project array: ";
                cin >> projectIndex;
            } while (projectIndex < 0 || projectIndex > 9);

            float newScore;
            do {
                cout << "\tSpecify a new score#" << projectIndex << " : ";
                cin >> newScore;
                if (newScore < 0 || newScore > 100) {
                    cout << "ERROR: Invalid input. Must be from 0.0..100.0.\n";
                }
            } while (newScore < 0 || newScore > 100);
            cout << "\tproject[" << projectIndex << "] changed from " << student->projectScores[projectIndex] << " to " << newScore << ".\n";
            student->projectScores[projectIndex] = newScore;
            break;
        }
        case '10': {
            // change exam scores
            int examIndex;
            do {
                cout << "\tSpecify the index (0...2) of the exam array: ";
                cin >> examIndex;
            } while (examIndex < 0 || examIndex > 2);

            float newScore;
            do {
                cout << "\tSpecify a new score#" << examIndex << " : ";
                cin >> newScore;
                if (newScore < 0 || newScore > 100) {
                    cout << "ERROR: Invalid input. Must be from 0.0..100.0.\n";
                }
            } while (newScore < 0 || newScore > 100);
            cout << "\texam[" << examIndex << "] changed from " << student->examScores[examIndex] << " to " << newScore << ".\n";
            student->examScores[examIndex] = newScore;
            break;
        }
        case '0': {
            // uncommit changes
            cout << "\nNo changes were made to the student record.\n";
            validInput = true;  // exit the update menu
            break;
        }
        case '1': {
            // commit changes
            cout << "\nChanges have been updated to the student record at index (" << index << ").\n";
            validInput = true;  // exit the update menu
            break;
        }
        default:
            cout << "\nERROR: Invalid option. Please select a valid option.\n";
            break;
        }

        system("pause");
    } while (!validInput);
}


void fileImplementationMenu() {
    char userInput[10];
    bool validInput = false;

    do {
        system("cls");
        setTextColor("93");
        cout << "\tOption C - Advance File Operation implementation\n";
        cout << string(75, char(205)) << "\n";
        cout << "\t1. Specify student binary data file\n";
        cout << "\t2. Add a new student\n";
        cout << "\t3. Search for a student\n";
        cout << "\t4. Delete a student (5Pts EXTRA)\n";
        cout << "\t5. Update an existing student\n";
        cout << "\t6. Display all students\n";
        cout << string(75, char(196)) << "\n";
        cout << "\t0. Return\n";
        cout << string(75, char(205)) << "\n";

        resetTextColor();  //reset after displaying menu

        //option prompt in white
        setTextColor("97");
        cout << "\tOption: ";
        cin >> userInput;
        cin.ignore();  //clear newline character from input buffer

        //validate input
        if (strlen(userInput) == 1 && (userInput[0] >= '0' && userInput[0] <= '6')) {
            switch (userInput[0]) {
            case '1':
                readOrCreateFileFile();
                break;
            case '2':
                addNewStudentFile();
                break;
            case '3': {
                //search for a student by student id
                if (currentStudentCount == 0) {
                    setTextColor("91");  //red text for error
                    cout << "\n\tERROR: Array is empty.\n";
                    resetTextColor();
                }
                else {
                    string searchStudentID;
                    cout << "\n\tSearch for a student from the array\n";
                    cout << string(75, char(196)) << "\n";

                    //get the student id input from the user
                    cout << "\tSpecify a student ID (CCnnnnnn) to search: ";
                    getline(cin, searchStudentID);

                    //validate the input format (ccnnnnnn)
                    regex idPattern("^[A-Z]{2}\\d{6}$");
                    if (!validateInput(searchStudentID, idPattern, "\tERROR: Student ID must be in the format CCnnnnnn (e.g., AA123456).")) {
                        system("pause");
                        break;
                    }

                    //iterate through the array to search for the student id
                    bool found = false;
                    for (int i = 0; i < currentStudentCount; ++i) {
                        if (students3[i]->studentID == searchStudentID) {
                            found = true;
                            cout << "\n\tFound student record in the array.\n";
                            break;
                        }
                    }

                    if (!found) {
                        setTextColor("91");  //red text for error
                        cout << "\n\tERROR: Student with ID [" << searchStudentID << "] not found in the array.\n";
                        resetTextColor();
                    }
                }

                //pause and return to the menu
                system("pause");
                break;
            }

            case '4': {
                //delete a student by student id
                if (currentStudentCount == 0) {
                    setTextColor("91");  //red text for error
                    cout << "\n\tERROR: Array is empty.\n";
                    resetTextColor();
                }
                else {
                    string deleteStudentID;
                    cout << "\n\tDelete a student from the array\n";
                    cout << string(75, char(196)) << "\n";

                    //get the student id input from the user
                    cout << "\tSpecify a student ID (CCnnnnnn) to search and delete: ";
                    getline(cin, deleteStudentID);

                    //validate the input format (ccnnnnnn)
                    regex idPattern("^[A-Z]{2}\\d{6}$");
                    if (!validateInput(deleteStudentID, idPattern, "\tERROR: Student ID must be in the format CCnnnnnn (e.g., AA123456).")) {
                        system("pause");
                        break;
                    }

                    //iterate through the array to search for the student id
                    bool found = false;
                    for (int i = 0; i < currentStudentCount; ++i) {
                        if (students3[i]->studentID == deleteStudentID) {
                            found = true;
                            //confirm deletion
                            char confirmDelete;
                            cout << "\tAre you sure you want to delete this student (Y/N)? ";
                            cin >> confirmDelete;
                            confirmDelete = toupper(confirmDelete);

                            if (confirmDelete == 'Y') {
                                delete students3[i];
                                for (int j = i; j < currentStudentCount - 1; j++) {
                                    students3[j] = students3[j + 1];  //shift elements left
                                }
                                currentStudentCount--;
                                cout << "\n\tDeleted student record from the array.\n";
                            }
                            else {
                                cout << "\n\tDeletion canceled.\n";
                            }

                            break;
                        }
                    }

                    if (!found) {
                        setTextColor("91");  //red text for error
                        cout << "\n\tERROR: Student with ID [" << deleteStudentID << "] not found in the array.\n";
                        resetTextColor();
                    }
                }

                //pause and return to the menu
                system("pause");
                break;
            }

            case '5': {
                //update a student by student id
                if (currentStudentCount == 0) {
                    setTextColor("91");  //red text for error
                    cout << "\n\tERROR: Array is empty.\n";
                    resetTextColor();
                }
                else {
                    string updateStudentID;
                    cout << "\n\tUpdate a student from the array\n";
                    cout << string(75, char(196)) << "\n";

                    //get the student id input from the user
                    cout << "\tSpecify a student ID (CCnnnnnn) to search and update: ";
                    getline(cin, updateStudentID);

                    //validate the input format (ccnnnnnn)
                    regex idPattern("^[A-Z]{2}\\d{6}$");
                    if (!validateInput(updateStudentID, idPattern, "\tERROR: Student ID must be in the format CCnnnnnn (e.g., AA123456).")) {
                        system("pause");
                        break;
                    }

                    //iterate through the array to search for the student id
                    bool found = false;
                    for (int i = 0; i < currentStudentCount; ++i) {
                        if (students3[i]->studentID == updateStudentID) {
                            found = true;
                            cout << "\n\tFound student record in the array.\n";
                            system("pause");

                            //after finding the student, open the update form
                            updateStudentFormFile(i);
                            break;
                        }
                    }

                    if (!found) {
                        setTextColor("91");  //red text for error
                        cout << "\n\tERROR: Student with ID [" << updateStudentID << "] not found in the array.\n";
                        resetTextColor();
                    }
                }

                //pause and return to the menu
                system("pause");
                break;
            }
                    //display student information
            case '6': {
                if (currentStudentCount == 0) {
                    setTextColor("91");  //red text for error
                    cout << "\n\tERROR: Array is empty.\n";
                    resetTextColor();
                }
                else {
                    setTextColor("97");  //white for the display header
                    cout << "\n\tDisplay all students from the array\n";
                    cout << string(75, char(196)) << "\n";
                    resetTextColor();

                    //output the number of students in the array
                    cout << "\n\tNumber of student(s) in the array: " << currentStudentCount << "\n\n";

                    //iterate through the array and display all student details in the required format
                    for (int i = 0; i < currentStudentCount; ++i) {
                        setTextColor("97");  //white for headers
                        cout << "\tStudent ID    : ";
                        setTextColor("32");  //green for student id
                        cout << students3[i]->studentID << "\n";

                        setTextColor("97");  //white for headers
                        cout << "\tName          : ";
                        setTextColor("32");  //green for student name
                        cout << students3[i]->name << "\n";

                        setTextColor("97");  //white for headers
                        cout << "\tDate of birth : ";
                        setTextColor("32");  //green for date of birth
                        cout << students3[i]->birthDate << "\n";

                        //class level and term headers in white, values in green
                        setTextColor("97");  //white text for headers
                        cout << "\tClass Level   : ";
                        setTextColor("32");  //green for values
                        cout << (students3[i]->collegeLevel == 0 ? "Unknown" : (students3[i]->collegeLevel == 1 ? "Freshman" : (students3[i]->collegeLevel == 2 ? "Sophomore" : (students3[i]->collegeLevel == 3 ? "Junior" : "Senior")))) << "\n";

                        setTextColor("97");  //white text for headers
                        cout << "\tTerm          : ";
                        setTextColor("32");  //green for term value
                        cout << students3[i]->term << "\n";

                        //letter grade header in white, value in green
                        setTextColor("97");  //white text for headers
                        cout << "\n\tLETTER GRADE  : ";
                        setTextColor("32");  //green for letter grade
                        cout << students3[i]->letterGrade << " (" << students3[i]->examAvg << "%)\n";

                        //ignore lowest score, "true" in green
                        setTextColor("97");  //white text for header
                        cout << "\n\tIgnore lowest score  : ";
                        setTextColor("32");  //green for the value "true"
                        cout << (students3[i]->ignoreLowestScore ? "true" : "false") << "\n";

                        //discussion scores and average, header in white, values in green
                        setTextColor("97");  //white text for headers
                        cout << "\n\tDiscussion scores : ";
                        setTextColor("32");  //green for values
                        for (int j = 0; j < 10; j++) {
                            cout << (j != 0 ? " " : "") << students3[i]->discussionScores[j] << "  ";
                        }

                        setTextColor("97");  //white text for headers
                        cout << "\n\tAvg score : ";
                        setTextColor("32");  //green for values
                        cout << students3[i]->discussionAvg << "\n";

                        //quiz scores and average, header in white, values in green
                        setTextColor("97");  //white text for headers
                        cout << "\n\tQuiz scores       : ";
                        setTextColor("32");  //green for values
                        for (int j = 0; j < 10; j++) {
                            cout << (j != 0 ? " " : "") << students3[i]->quizScores[j] << "  ";
                        }

                        setTextColor("97");  //white text for headers
                        cout << "\n\tAvg score : ";
                        setTextColor("32");  //green for values
                        cout << students3[i]->quizAvg << "\n";

                        //project scores and average, header in white, values in green
                        setTextColor("97");  //white text for headers
                        cout << "\n\tProject scores    : ";
                        setTextColor("32");  //green for values
                        for (int j = 0; j < 10; j++) {
                            cout << (j != 0 ? " " : "") << students3[i]->projectScores[j] << "  ";
                        }

                        setTextColor("97");  //white text for headers
                        cout << "\n\tAvg score : ";
                        setTextColor("32");  //green for values
                        cout << students3[i]->projectAvg << "\n";

                        //exam scores and average, header in white, values in green
                        setTextColor("97");  //white text for headers
                        cout << "\n\tExam scores       : ";
                        setTextColor("32");  //green for values
                        for (int j = 0; j < 3; j++) {
                            cout << (j != 0 ? " " : "") << students3[i]->examScores[j] << "  ";
                        }

                        setTextColor("97");  //white text for headers
                        cout << "\n\tAvg score : ";
                        setTextColor("32");  //green for values
                        cout << students3[i]->examAvg << "\n";

                        resetTextColor();  //reset to default color after the student details are printed
                        cout << "\n";  //newline after each student's details
                    }
                }
                system("pause");
                break;
            }
            case '0':
                //if user selects 0, return to the main menu
                cout << "\nReturning to Main Menu...\n";
                validInput = true;
                break;
            default:
                cout << "\nInvalid option. Please select again.\n";
                break;
            }
        }
        else {
            cout << "\nInvalid input. Please enter a number between 0 and 6.\n";
        }
        system("pause");  // Pause to view messages
    } while (!validInput);
}

void readOrCreateFileFile() {
    string fileName = "STUDENTS.DAT";
    ifstream inFile(fileName, ios::binary | ios::ate);  // Open for reading at the end to check the file size.

    if (!inFile) {
        // File does not exist. Create a new file.
        cout << "File does not exist. Creating new file...\n";
        ofstream outFile(fileName, ios::binary);
        if (!outFile) {
            cout << "Error: Unable to create file.\n";
        }
        else {
            cout << "File created successfully.\n";
            outFile.close();
        }
    }
    else {
        // File exists. Check if it's empty or not.
        if (inFile.tellg() == 0) {
            cout << "File exists but is empty. Ready to add new data.\n";
        }
        else {
            cout << "File exists. Reading data...\n";
            // Here you might want to read data or simply notify that existing data is ready to be used.
        }
        inFile.close();
    }
}

// adds a new student if the maximum limit isn't reached
// prompts user for student details and saves them in array
// confirms student was added and increments the student count
void addNewStudentFile() {
    if (currentStudentCount >= MAX_STUDENTS) {
        cout << "Maximum number of students reached.\n";
        return;
    }
    StudentFile* newStudent = new StudentFile();
    cout << "Enter student ID: ";
    cin >> newStudent->studentID;
    cout << "Enter student name: ";
    cin >> newStudent->name;
    cout << "Enter birth date: ";
    cin >> newStudent->birthDate;
    cout << "Enter college level (1-4): ";
    cin >> newStudent->collegeLevel;
    cout << "Enter term: ";
    cin >> newStudent->term;
    cout << "Ignore lowest score (0=false, 1=true): ";
    cin >> newStudent->ignoreLowestScore;
    students3[currentStudentCount++] = newStudent;
    cout << "Student added successfully.\n";
}

// deletes a student if the index is valid
// shifts remaining students to fill the deleted position
// reduces student count and confirms deletion
void deleteStudentFile(int index) {
    if (index < 0 || index >= currentStudentCount) {
        cout << "Invalid student index.\n";
        return;
    }
    delete students3[index];
    for (int i = index; i < currentStudentCount - 1; i++) {
        students3[i] = students3[i + 1];
    }
    currentStudentCount--;
    cout << "Student deleted successfully.\n";
}

// opens file for writing student data in binary format
// writes each student's data to the file sequentially
// closes the file and confirms successful data write
void writeStudentDataToFileFile() {
    ofstream outFile("STUDENTS.DAT", ios::binary);
    if (!outFile) {
        cout << "Error: Unable to create file.\n";
        return;
    }
    for (int i = 0; i < currentStudentCount; ++i) {
        outFile.write(reinterpret_cast<char*>(students3[i]), sizeof(StudentFile));
    }
    outFile.close();
    cout << "Student data successfully written to STUDENTS.DAT.\n";
}

//main menu loop
int main() {
    char userInput[10];  //declare user input buffer outside the loop
    bool exitProgram = false;  //initialize exit flag to control program flow

    do {
        system("cls");  //clear the screen before showing the main form

        //display the main menu
        setTextColor("32");  //green text for comment block
        cout << "\tA struct (structure) is a user-defined data type in C/C++. A structure creates a data type\n"
            << "\tthat can be used to group items of possibly different types into a single type.\n\n"
            << "\tVectors are the same as dynamic arrays with the ability to resize themselves automatically\n"
            << "\twhen an element is inserted or deleted, with their storage being handled automatically\n"
            << "\tby the container.\n\n"
            << "\tDynamic memory allocation in C/C++ refers to performing memory allocation manually by\n"
            << "\ta programmer using new and delete operators.\n\n"
            << "\tA binary file is in which data is stored in the file in the same way as it is stored in\n"
            << "\tthe main memory for processing. It is stored in binary format instead of ASCII characters.\n\n";

        resetTextColor();  //reset after green text

        setTextColor("97");  //white text for program header
        cout << "\tCMPR121 - Exam#1 (Chapter 1...12) by Francisco Bumanglag\n";
        cout << string(90, char(205)) << "\n";  //replaced "===" with char(205)

        //options a, b, c, in light yellow
        setTextColor("93");
        cout << "\t\tA. Vector Container (Chapter 7)\n";
        cout << "\t\tB. Dynamic Memory Allocated Array (Chapter 9)\n";
        cout << "\t\tC. Advance File Operation (Chapter 12)\n";

        //divider and exit option in white
        setTextColor("97");
        cout << string(90, char(196)) << "\n";
        cout << "\t\tX. Exit\n";
        cout << string(90, char(205)) << "\n";

        resetTextColor();  //reset after displaying options

        //option prompt in white
        setTextColor("97");
        cout << "\t\tOption: ";
        cin >> userInput;

        //convert the input to uppercase to reduce redundancy
        char option = toupper(userInput[0]);

        //check if input is valid and switch cases
        if (strlen(userInput) == 1 && (option == 'A' || option == 'B' || option == 'C' || option == 'X')) {
            switch (option) {
            case 'A':  //handle option a
                vectorImplementationMenu();  //call vector implementation menu
                break;
            case 'B':  //handle option b
                arrayImplementationMenu();  //call dynamic memory allocated array implementation
                break;
            case 'C':  //handle option c
                fileImplementationMenu();  //call dynamic memory allocated array implementation
                break;
            case 'X':  //exit the program
                cout << "\tExiting program...\n";
                exitProgram = true;
                break;
            }
        }
        else {
            //invalid input handling
            cout << "\n\tError: Invalid input. Must be one of 'ABCX' characters.\n";
            system("pause");
        }

    } while (!exitProgram);

    return 0;  //exit the program
}
    