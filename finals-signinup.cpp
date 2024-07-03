#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <algorithm>

using namespace std;

void title();
void signUP(), signIN(), signOUT(), exitMenu();
void adminMenu(), addBook(), deleteBook(), updateStatus();
void userMenu(), borrowBook(), returnBook();
void displayBook(), searchBook();

void YesNoPrompt(string prompt, char origin);
bool validPassword(const string& password);
string validCourse();

char userMode = 0;
int delete_display = 0;

int main() {
   
    string choice;
    int input;

    title();
    cout << "\t\t\t\x1b[32m[1]\x1b[0m SIGN UP" << endl;
    cout << "\t\t\t\x1b[32m[2]\x1b[0m SIGN IN " << endl;
    cout << "\t\t\t\x1b[32m[3]\x1b[0m EXIT" << endl;

    while (true) {
        cout << "\nPlease type your selection: ";
        getline(cin, choice);

        // Check if the entire line consists only of digits
        bool valid_input = true;
        for (char c : choice) {
            if (!isdigit(c)) {
                valid_input = false;
                break;
            }
        }
        // Check if the input is within the valid range
        if (valid_input && choice.length() == 1 && choice >= "1" && choice <= "3") {
            input = stoi(choice);
            break;
        }
        else {
            cout << "\x1b[31m\nInvalid selection.Please choose again.\x1b[0m" << endl;
        }
    }

    switch (input) {
    case 1: signUP(); break;
    case 2: signIN(); break;
    case 3: exitMenu(); break;
    }

    return 0;
}

struct User {
    char name[50], username[50], password[20];
    string course;
};

User users[100];
int userCounter = 0;

struct Book {
    string title, author, course, status;
    int bookID, publicationYear;
    Book* next;
};

Book* head = nullptr;

void title() {
    // Simple title that is consistent throughout the entire code
    cout << "\x1b[42m------------------------------------------------------\x1b[0m" << endl;
    cout << "\x1b[42m       COLLEGE OF AURORA LIBRARY MANAGEMENT SYSTEM    \x1b[0m" << endl;
    cout << "\x1b[42m------------------------------------------------------\x1b[0m" << endl << endl;
}

void signUP() {
    // Signs user up and saves information through a text file
    User newUser;
    string confirmpass;

    system("clear");

    title();

    cout << "\x1b[32mSIGN-UP to the COA-LMS\n\x1b[0m";
    cout << "\n\x1b[34mName: \x1b[0m";
    cin.getline(newUser.name, 50);

    newUser.course = validCourse();

    // Validates whether the username is atleast 4 characters
    do {
        cout << "\x1b[34m\nUsername: \x1b[0m";
        cin.getline(newUser.username, 50);
        if (strlen(newUser.username) < 4) {
            cout << " \x1b[31m Uh oh! Username should at least have 4 characters. \x1b[0m \n";
        }
    } while (strlen(newUser.username) < 4);

    // Calls the function validPassword and will only exit the loop if the return value is true
    do {
        cout << "\x1b[34mPassword: \x1b[0m";
        cin.getline(newUser.password, 20);
    } while (!validPassword(newUser.password));

    do {
        cout << "\x1b[34m\nConfirm Password: \x1b[0m";
        getline(cin, confirmpass);

        if (confirmpass != newUser.password) {
            cout << "\x1b[31m Uh oh! Password does not match. Please try again.\x1b[0m";
        }
        else {
            break;
        }
        cout << endl;
    } while (true);

    users[userCounter++] = newUser;

    // Opens a file and stores user information
    ofstream outProfiles("userProfiles.txt", ios::app);
    if (!outProfiles) {
        cerr << " \x1b[31m Unable to open file. \x1b[0m" << endl;
        return;
    }

    for (int i = 0; i < userCounter; ++i) {
        outProfiles << users[i].name << "," << users[i].username << "," << users[i].password << "," << users[i].course << endl;
    }

    outProfiles.close();

    cout << "\n \x1b[32m Your sign-up to the LMS has been successful! \x1b[0m";

    // Calls a universal Yes or No prompt
    string prompt = "What would you like to do? ";
    char origin = 'S';

    YesNoPrompt(prompt, origin);
}

void signIN() {
    char username[50], password[20];
    bool found = false;

    system("clear");
    title();

    cout << " \x1b[32m SIGN-IN to the COA-LMS \x1b[0m" << endl;

    do {
        cout << "\x1b[34m\nUsername: \x1b[0m";
        cin.getline(username, 50);
        cout << "\x1b[34mPassword: \x1b[0m";
        cin.getline(password, 20);

        if (username[0] == '\0' || password[0] == '\0') {
            cout << "\x1b[31m \nPlease enter both username and password. \x1b[0m" << endl;
            continue;
        }

        // Hard-coded admin credentials 
        if (strcmp(username, "ADMIN") == 0 && strcmp(password, "Borealis134") == 0) {
            found = true;
            cout << "\x1b[32m \nSign-In Successful! \n\n \x1b[0m";
            system("PAUSE");
            system("clear");
            adminMenu();
            return;
        }

        found = false;

        // Opens file in read mode and compares user credentials on the text file to the file input
        ifstream inProfiles("userProfiles.txt");
        string line;

        while (getline(inProfiles, line)) {
            string comp_name, comp_username, comp_password, comp_course;

            istringstream iss(line);
            getline(iss, comp_name, ',');
            getline(iss, comp_username, ',');
            getline(iss, comp_password, ',');
            getline(iss, comp_course, ',');
            if (username == comp_username && password == comp_password) {
                found = true;
                cout << "\x1b[32m \nSign-In Successful! \n\n \x1b[0m";
                system("PAUSE");
                system("CLS");
                cout << "\x1b[32m \nWelcome, " << username << "! \x1b[0m" << endl;
                userMenu();
                continue;
            }
        }

        inProfiles.close();

        if (!found) cout << "\x1b[31m \nIncorrect username or password. Please try again. \x1b[0m" << endl;
    } while (!found);
}

void signOUT() {
    // Signs user out and calls main()
    userMode = 0;
    system("clear");
    cin.clear();
    main();
}

void adminMenu() {
    // Menu Interface exclusive for admins — can Add, Delete, Display, Search, and Update Book/s
    system("clear");
    title();
    string choice;
    int input;

    userMode = 'A';

    cout << "\x1b[32m\nWelcome, ADMIN! \x1b[0m" << endl;

    cout << "\x1b[32m[1]\x1b[0m Add a Book" << endl;
    cout << "\x1b[32m[2]\x1b[0m Delete Books" << endl;
    cout << "\x1b[32m[3]\x1b[0m Display Books" << endl;
    cout << "\x1b[32m[4]\x1b[0m Search Books" << endl;
    cout << "\x1b[32m[5]\x1b[0m Update Book Status" << endl;
    cout << "\x1b[32m[6]\x1b[0m Sign-Out" << endl;
    cout << "\x1b[32m[7]\x1b[0m Exit" << endl;



    while (true) {
        cout << "\nPlease type your selection: ";
        getline(cin, choice);

        // Check if the entire line consists only of digits
        bool valid_input = true;
        for (char c : choice) {
            if (!isdigit(c)) {
                valid_input = false;
                break;
            }
        }
        // Check if the input is within the valid range
        if (valid_input && choice.length() == 1 && choice >= "1" && choice <= "7") {
            input = stoi(choice);
            break;
        }
        else {
            cout << "\x1b[31m \nInvalid selection.Please choose again. \x1b[0m" << endl;
        }
    }

    switch (input) {
    case 1: addBook(); break;
    case 2: deleteBook(); break;
    case 3: displayBook(); break;
    case 4: searchBook(); break;
    case 5: updateStatus(); break;
    case 6: signOUT(); break;
    case 7: exitMenu(); break;
    }
}

void userMenu() {
    // Menu Interface for general users — can Borrow, Return, Display, and Search Book/s
    system("clear");
    title();
    cout << "\x1b[32m Welcome! \x1b[0m";

    userMode = 'U';

    string choice;
    int input;

    cout << "\x1b[32m\n[1]\x1b[0m Borrow Book" << endl;
    cout << "\x1b[32m[2]\x1b[0m Return Book" << endl;
    cout << "\x1b[32m[3]\x1b[0m Display Books" << endl;
    cout << "\x1b[32m[4]\x1b[0m Search Books" << endl;
    cout << "\x1b[32m[5]\x1b[0m Sign-Out" << endl;
    cout << "\x1b[32m[6]\x1b[0m Exit" << endl;

    while (true) {
        cout << "\nPlease type your selection: ";
        getline(cin, choice);

        // Check if the entire line consists only of digits
        bool valid_input = true;
        for (char c : choice) {
            if (!isdigit(c)) {
                valid_input = false;
                break;
            }
        }
        // Check if the input is within the valid range
        if (valid_input && choice.length() == 1 && choice >= "1" && choice <= "7") {
            input = stoi(choice);
            break;
        }
        else {
            cout << "\x1b[31m \nInvalid selection.Please choose again. \x1b[0m" << endl;
        }
    }


    switch (input) {
    case 1: borrowBook(); break;
    case 2: returnBook(); break;
    case 3: displayBook(); break;
    case 4: searchBook(); break;
    case 5: signOUT(); break;
    case 6: exitMenu(); break;
    }

}

void addBook() {
    char choice;

    // Opens the file for writiing in append mode — contains books information
    ofstream shelf("library_records.txt", ios::app);
    if (!shelf.is_open()) {
        cerr << " \x1b[31m Unable to open file for writing! \x1b[0m" << endl;
        return;
    }

    system("clear");
    title();

    cout << "======================================================" << endl;
    cout << "\x1b[47m                     ADD A BOOK                       \x1b[0m" << endl;
    cout << "======================================================" << endl << endl;

    // Create a new book node
    Book* newBook = new Book;
    newBook->next = nullptr;

    string temp_title, temp_author, temp_course;
    int temp_publicationYear;
    bool isValid = true;

    // Validation for title
    do {
        cout << "\x1b[34mBook Title:\x1b[0m ";
        getline(cin, temp_title);

        if (temp_title.empty() || temp_title.length() < 5) {
            isValid = false;
            cout << " \x1b[31m \nInvalid input. Please enter a title that has more than 5 letters.\n \x1b[0m" << endl;
            cin.clear();
        }
        else isValid = true;
    } while (!isValid);

    newBook->title = temp_title;

    // Validation for author
    do {
        isValid = false;
        cout << "\x1b[34mAuthor: \x1b[0m";
        getline(cin, temp_author);

        if (!(temp_author.empty() && temp_author.length() >= 3)) {
            isValid = true;
        }
        else {
            isValid = false;
        }
    } while (!isValid);

    newBook->author = temp_author;

    // Validation for course
    temp_course = validCourse();

    newBook->course = temp_course;


    // Validation for publication year
    string converted_pY;

    do {
        isValid = false;
        cout << "\x1b[34mPublication Year: \x1b[0m";
        getline(cin, converted_pY);

        // Check if the input consists only of digits and is exactly 4 characters long
        if (converted_pY.size() == 4 && all_of(converted_pY.begin(), converted_pY.end(), ::isdigit)) {
            // Convert input to integer and check the range
            temp_publicationYear = stoi(converted_pY);
            if (temp_publicationYear <= 2024) {
                isValid = true;
            }
        }

        if (!isValid) {
            cout << " \x1b[31m \nPlease input a valid publication year only. \x1b[0m" << endl;
            cin.clear();
        }
    } while (!isValid);

    newBook->publicationYear = temp_publicationYear;

    // Gets first two digits of bookID (course ID) -- finds E (ENG) and no E (CCSMA)
    // Assigns either 11 (ENG) or 22 (CCSMA) for the first two digits

    string course_bookID;
    bool findE = false;
    for (char c : temp_course) { 
        if (toupper(c) == 'E') { // Checks each letter on string for E
            findE = true;
            break;
        }
    }
    if (findE) course_bookID = "11";
    else course_bookID = "22";

    // Gets 3rd and 4th digit of bookID (inventory ID) — opens file in read mode
    ifstream shelf_inv("library_records.txt");
    if (!shelf_inv) cerr << "\x1b[31m \nUnable to open file for reading! \x1b[0m" << endl;

    int linesCounter = 0;
    string line, inventory_bookID;
    while (getline(shelf_inv, line)) {
        linesCounter++;
    }
    shelf_inv.close();
    if (linesCounter < 10) {
        inventory_bookID = "0" + to_string(linesCounter);
    }
    else {
        inventory_bookID = to_string(linesCounter);
    }

    // Gets 5th and 6th digit of bookID (year ID or the last two numbers of the year)
    string year_bookID = converted_pY.substr(converted_pY.size() - 2);

    // Automatic bookID generator
    string combined_bookID_str = course_bookID + inventory_bookID + year_bookID;
    int combined_bookID = stoi(combined_bookID_str);

    newBook->bookID = combined_bookID;

    // Status of book is by default available
    newBook->status = "AVAILABLE";

    // Add new book to the linked list
    if (head == nullptr) {
        head = newBook; // If the list is empty, the new book becomes the head
    }
    else {
        Book* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newBook; // Add the new book at the end of the list
    }

    // Write book details to the file
    shelf << newBook->title << "," << newBook->bookID << "," << newBook->author << "," << newBook->course << "," << newBook->publicationYear << "," << newBook->status << endl;
    shelf.close();

    cout << " \x1b[32m \nBook successfully added! \x1b[0m" << endl;
    delete_display = 2;
    displayBook();
    delete_display = 0;

    string prompt = "Do you want to add more books? \x1b[32m[Y/N]: \x1b[0m";
    char origin = 'A';

    YesNoPrompt(prompt, origin);
}


void deleteBook() {
    // Deletes book inside the text file
    const string filename = "library_records.txt";
    string bookID;

    system("clear");
    title();

    cout << "======================================================" << endl;
    cout << "\x1b[47m                    DELETE A BOOK                     \x1b[0m" << endl;
    cout << "======================================================" << endl << endl;

    delete_display = 2;
    displayBook();

    // Validates integer input
    do {
        cout << "\x1b[34m\n\nEnter 6-digit integer Book ID: \x1b[0m";
        getline(cin, bookID);

        if (bookID.length() != 6 || !all_of(bookID.begin(), bookID.end(), ::isdigit)) {
            cout << "\x1b[31m\nInvalid Book ID! Please enter a 6-digit integer.\x1b[0m" << endl;
            cin.clear();
        }
    } while (bookID.length() != 6 || !all_of(bookID.begin(), bookID.end(), ::isdigit));

    ifstream inFile(filename);
    ofstream outFile("temp.txt");

    string line;
    bool found = false;

    // Search for the book ID in each line of the file
    while (getline(inFile, line)) {
        // Check if the line contains the book ID
        if (line.find(bookID) != string::npos) {
            cout << "\x1b[32mBook with ID " << bookID << " deleted. \x1b[0m" << endl;
            found = true;
            continue; // Skip writing this line to the output file
        }

        // Write the line to the output file if the book ID is not found
        outFile << line << endl;
    }

    inFile.close();
    outFile.close();

    // Rename the temporary file to the original filename
    remove(filename.c_str());
    rename("temp.txt", filename.c_str());

    if (!found) {
        cout << "\x1b[31m Book with ID " << bookID << " not found.\x1b[0m" << endl;
    }

    displayBook();
    delete_display = 0;

    string prompt = "\nWould you like to delete another book? \x1b[32m[Y/N]: \x1b[0m";
    char origin = 'D';
    YesNoPrompt(prompt, origin);
}

void displayBook() {
    // Displays the file content
    ifstream shelf("library_records.txt");
    if (shelf.is_open()) {
        string line;

        cout << "\n\x1b[44m" << left << setw(10) << "Book ID" << setw(30) << "Title" << setw(18) << "Author" << setw(12) << "Course" << setw(10) << "Year" << setw(10) << "Status    .\x1b[0m" << endl;
        for (int i = 0; i < 92; i++) cout << "-";
        cout << endl;

        // Actual Display Function
        while (getline(shelf, line)) {
            // Split the line into individual fields
            string title, author, course, status;
            int bookID, publicationYear;
            char comma;
            istringstream iss(line);
            getline(iss, title, ',');  // Check this line to ensure the title is correctly read
            iss >> bookID >> comma;
            getline(iss, author, ',');
            getline(iss, course, ',');
            iss >> publicationYear >> comma;
            getline(iss, status);
            // Display the book details
            cout << left << setw(10) << bookID << setw(30) << title << setw(18) << author << setw(12) << course << setw(10) << publicationYear;
            if (status == "AVAILABLE") {
                cout << setw(10) << "\x1b[32m AVAILABLE \x1b[0m  \n";
            }
            else {
                cout << setw(10) << "\x1b[31m BORROWED \x1b[0m  \n";
            }
        }

        // Closes the text file
        shelf.close();
        for (int i = 0; i < 92; i++) cout << "-";
        cout << endl;
    }
    else {
        cerr << "\x1b[31m Unable to open file for reading!\x1b[0m" << endl;
    }

    if (delete_display != 2) {
        char origin = 'D';
        string prompt = " ";
        YesNoPrompt(prompt, origin);

    }
}

void searchBook() {
    // Searches books using either the title, author, course, status, or characters that can be found in it
    string bookTitle, author, course, status, searchterm, line;
    int bookID, publicationYear;
    char comma;
    bool itemFound = false, resultPrompt = false;
    system("clear");

    title();

    cout << "Search an Item \x1b[34m(BookID, Name, Author, Course, Publication Year, Status)\x1b[0m: ";
    cout.flush();
    if (cin.peek() == '\n')
        cin.ignore();
    getline(cin, searchterm);

    ifstream shelf("library_records.txt");
    if (shelf.is_open()) {

    }
    while (getline(shelf, line)) {
        // Searches the file
        if (line.find(searchterm) != string::npos) {
            // Condition to not keep repeating Result Found upon printing
            if (!resultPrompt) {
                cout << "\x1b[32m\nResult Found including the word '" << searchterm << "'\x1b[0m" << endl;
                cout << "\x1b[47m\n" << left << setw(10) << "Book ID" << setw(30) << "Title" << setw(18) << "Author" << setw(12) << "Course" << setw(10) << "Year" << setw(10) << "Status    .\x1b[0m" << endl;
                for (int i = 0; i < 92; i++) cout << "-";
                cout << endl;
                resultPrompt = true;
            }
            istringstream iss(line);
            getline(iss, bookTitle, ',');
            iss >> bookID >> comma;
            getline(iss, author, ',');
            getline(iss, course, ',');
            iss >> publicationYear >> comma;
            getline(iss, status);

            cout << left << setw(10) << bookID << setw(30) << bookTitle << setw(18) << author << setw(12) << course << setw(10) << publicationYear << setw(10);
            if (status == "AVAILABLE") {
                cout << setw(10) << "\x1b[32m AVAILABLE \x1b[0m  \n";
            }
            else {
                cout << setw(10) << "\x1b[31m BORROWED \x1b[0m  \n";
            }
            itemFound = true;
        }
    }
    for (int i = 0; i < 92; i++) cout << "-";
    cout << endl;
    if (!itemFound) {
        cout << "\x1b[31m\n" << searchterm << " was not found.\x1b[0m" << endl << endl;
    }

    // Closes the text file
    shelf.close();

    char menumode = 'B', origin = 'Z';
    string prompt = "Search Again? \x1b[32m[Y/N]: \x1b[0m";
    YesNoPrompt(prompt, origin);
}

void borrowBook() {
    // Borrows book — changes status from BORROWED to AVAILABLE
    char origin = 'B';
    string bookID, prompt;

    system("clear");
    title();

    cout << "======================================================" << endl;
    cout << "\x1b[47m                    BORROW A BOOK                     \x1b[0m" << endl;
    cout << "======================================================" << endl << endl;

    delete_display = 2;
    displayBook();
    delete_display = 0;

    bool bookFound = false; // Used for validating if book is found
    bool bookUpdated = false; // Used for validating if the books is updated
    bool notfound = false; // Used as a separate bool value to ensure database does not show again upon invalidity

    do {
        cout << "\x1b[34m\n\nEnter 6-digit integer Book ID: \x1b[0m";
        getline(cin, bookID);

        if (bookID.length() != 6 || !all_of(bookID.begin(), bookID.end(), ::isdigit)) {
            cout << "\x1b[31m\nInvalid Book ID! Please enter a 6-digit integer.\x1b[0m" << endl;
            cin.clear();
        }
    } while (bookID.length() != 6 || !all_of(bookID.begin(), bookID.end(), ::isdigit));

    // Opens library records in read mode and creates a temp file in write mode
    ifstream inFile("library_records.txt");
    ofstream outFile("temp.txt");

    string line;

    while (getline(inFile, line)) {
        stringstream ss(line);
        string title, author, course, publicationYear, statusDef;
        char comma;
        int currentBookID;

        getline(ss, title, ',');
        ss >> currentBookID >> comma;
        getline(ss, author, ',');
        getline(ss, course, ',');
        getline(ss, publicationYear, ',');
        getline(ss, statusDef, ',');

        if (currentBookID == stoi(bookID)) {
            if (statusDef == "AVAILABLE") {
                statusDef = "BORROWED";
                cout << "\x1b[32m\nBook with ID " << bookID << " has been borrowed.\x1b[0m" << endl;
                bookFound = true;
                notfound = true;
                bookUpdated = true;
            }
            else {
                cout << "\x1b[32m\nBook with ID " << bookID << " is already borrowed.\x1b[0m" << endl;
                notfound = true;
                inFile.close();
                outFile.close();
                remove("temp.txt");
                break;
            }
        }
        // Each line in the records file is feed into the temp file
        outFile << title << "," << currentBookID << "," << author << "," << course << "," << publicationYear << "," << statusDef << endl;
    }

    inFile.close();
    outFile.close();

    // Replaces original records file with the temp file
    if (bookUpdated) {
        remove("library_records.txt");
        rename("temp.txt", "library_records.txt");
    }
    else remove("temp.txt");

    if (!notfound) cout << "\x1b[31m\nBook with ID " << bookID << " was not found.\x1b[0m" << endl;

    if (bookFound) { //if book is not found, it will not display the database again
        delete_display = 2;
        displayBook();
        delete_display = 0;
    }

    prompt = "Do you want to borrow another book? \x1b[32m[Y/N]:\x1b[0m ";
    YesNoPrompt(prompt, origin);
}

void updateStatus() {
    // Updates book status (for admins) to either BORROWED or AVAILABLE
    char origin = 'U';
    string prompt = "Keep Changing Status? [Y/N]: ";
    string bookID;

    system("clear");
    title();

    cout << "======================================================" << endl;
    cout << "                 CHANGE BOOK STATUS                   " << endl;
    cout << "======================================================" << endl << endl;

    delete_display = 2;
    displayBook();
    delete_display = 0;

    bool bookFound = false; // Flag to track if book ID is found
    bool bookUpdated = false; // Flag to track if the book status is updated

    cout << "\nPlease ensure that the person has successfully returned a book. \vCheck for any damages and double-check before changing status." << endl;

    ifstream inFile("library_records.txt");
    ofstream outFile("temp.txt");

    do {
        cout << "\n\nEnter 6-digit integer Book ID: ";
        getline(cin, bookID);

        if (bookID.length() != 6 || !all_of(bookID.begin(), bookID.end(), ::isdigit)) {
            cout << "\x1b[31m\nInvalid Book ID! Please enter a 6-digit integer.\x1b[0m" << endl;
            cin.clear();
        }
    } while (bookID.length() != 6 || !all_of(bookID.begin(), bookID.end(), ::isdigit));

    string line;

    while (getline(inFile, line)) {
        stringstream ss(line);
        string title, author, course, publicationYear, statusDef;
        char comma;
        int currentBookID;

        getline(ss, title, ',');
        ss >> currentBookID >> comma;
        getline(ss, author, ',');
        getline(ss, course, ',');
        getline(ss, publicationYear, ',');
        getline(ss, statusDef, ',');

        if (currentBookID == stoi(bookID)) {
            bookFound = true; // Book ID found in the database

            // Change status
            if (statusDef == "BORROWED") {
                statusDef = "AVAILABLE";
                cout << "\x1b[32m\nBook with ID " << bookID << " has changed status.\x1b[0m" << endl;
                bookUpdated = true; // Status updated flag
            }
            else if (statusDef == "AVAILABLE") {
                statusDef = "BORROWED";
                cout << "\x1b[32m\nBook with ID " << bookID << " has changed status.\x1b[0m" << endl;
                bookUpdated = true; // Status updated flag
            }
        }

        outFile << title << "," << currentBookID << "," << author << "," << course << "," << publicationYear << "," << statusDef << endl;
    }

    inFile.close();
    outFile.close();

    if (!bookFound) {
        cout << "\x1b[31m\nBook with ID " << bookID << " was not found.\x1b[0m" << endl;
    }
    else if (bookUpdated) {
        // Rename temp file to update the database
        remove("library_records.txt");
        rename("temp.txt", "library_records.txt");

        delete_display = 2;
        displayBook();
        delete_display = 0;
    }

    prompt = "Do you want to change another book's status? [Y/N]: ";
    YesNoPrompt(prompt, origin);
}

void returnBook() {
    // Simple cout function that contains instructions on how to return a book (assuming this is used in the context of a library
    system("clear");
    title();

    cout << "\x1b[32mReturning a book? Here's how -\n\x1b[0m" << endl;
    cout << "\x1b[32m[1]\x1b[0m Books are returned directly at the \x1b[34mCollege of Aurora Annex Building, 14th floor at room 1401.\x1b[0m\n" << endl;
    cout << "\x1b[32m[2]\x1b[0m Library Operations are from \x1b[34m8:00 AM to 5:00 PM, Monday to Friday.\x1b[0m You can visit anytime from the said periods to return books.\n" << endl;
    cout << "\x1b[32m[3]\x1b[0m When returning books, please don't forget to \x1b[34mbring your borrow slip and ID for admin verification.\x1b[0m" << endl;
    cout << "   In any case that your borrow slip has been lost. Please refer to the librarian's desk for further assistance.\n" << endl;
    cout << "\x1b[32m\nHave additional concerns? Reach us at lms@coa.edu.ph.\n\x1b[0m";

    string prompt = " ";
    char origin = 'R';
    YesNoPrompt(prompt, origin);
}


bool validPassword(const string& password) {
    // Validates password
    bool hasUpper = false, hasLower = false, hasSymbol = false, hasNum = false;

    for (char c : password) { // Converts string to character and checks if it has: an upper and lowercase, has digits
        if (isupper(c)) hasUpper = true;
        else if (islower(c))  hasLower = true;
        else if (!isalnum(c)) hasSymbol = true;
        else if (isdigit(c)) hasNum = true;

    }
    bool isValid = hasUpper && hasLower && hasSymbol && hasNum && (password.length() >= 8);
    // Individual error messages for the conditions that were not met

    if (!isValid) {
        cout << "\x1b[31mPassword should contain at least \x1b[0m";
        bool firstRequirement = true;

        if (password.length() < 8) {
            cout << "\x1b[31m 8 characters\x1b[0m";
            firstRequirement = false;
        }
        if (!hasUpper) {
            if (!firstRequirement) {
                cout << ", ";
            }
            cout << "\x1b[31m 1 uppercase character\x1b[0m";
            firstRequirement = false;
        }
        if (!hasLower) {
            if (!firstRequirement) {
                cout << ", ";
            }
            cout << "\x1b[31m 1 lowercase character\x1b[0m";
            firstRequirement = false;
        }
        if (!hasSymbol) {
            if (!firstRequirement) {
                cout << ", ";
            }
            cout << "\x1b[31m 1 special character\x1b[0m";
            firstRequirement = false;
        }
        if (!hasNum) {
            if (!firstRequirement) {
                cout << "\x1b[31m , and \x1b[0m";
            }
            cout << "\x1b[31m 1 number\x1b[0m";
            firstRequirement = false;
        }
        cout << "." << endl;
    }
    return isValid;
}

string validCourse() {
    // Validates input on course
    string course;
    bool isValid = false;

    while (!isValid) {
        cout << "\x1b[34mCourse (e.g., BSCS): \x1b[0m";
        getline(cin, course);

        // Convert input to uppercase for case-insensitive comparison
        transform(course.begin(), course.end(), course.begin(), ::toupper);

        // The following course below is only accepted
        if (course == "BSCS" || course == "BSIT" || course == "BSMMA" || course == "BSCE"
            || course == "BSME" || course == "BSEE" || course == "BSCPE" || course == "BSECE") {
            isValid = true;
        }
        else {
            cout << "\x1b[31m\nInvalid course. Please follow the format and enter courses existing in COA.\x1b[0m" << endl;
        }
    }
    return course;
}

void YesNoPrompt(string prompt, char origin) {
    // A universal Yes or No prompt at every end of a function
    string choice, returnChoice;

    // Has an origin variable (where it originated from) and a user variable (whether it is admin or user)
    // Yes No Prompt for Sign-Up


        //prompt for signUp
    if (origin == 'S') {
        do {
            cout << "\n\n[1] Sign-In\n[2] Exit" << endl;
            cout << prompt;
            getline(cin, choice);

            if (choice == "1") {
                system("clear");
                signIN();
            }
            else if (choice == "2") {
                exitMenu();
            }
            else {
                cout << "\x1b[31m\nInvalid choice. Please choose again.\x1b[0m" << endl;
            }
        } while (true);
    }

    // YesNo Prompt for Add Book
    if (userMode == 'A' && origin == 'A') {
        do {
            cout << prompt;
            getline(cin, choice);
            if (choice.length() != 1 || (toupper(choice[0]) != 'Y' && toupper(choice[0]) != 'N')) {
                cout << "\x1b[31m\nInvalid choice. Please choose again.\x1b[0m" << endl;
                continue;
            }
            if (toupper(choice[0]) == 'Y') {
                addBook();
            }
            else if (toupper(choice[0]) == 'N') {
                break;
            }
        } while (true);
    }

    //YesNo Prompt for Delete Book
    if (userMode == 'A' && origin == 'D') {
        do {
            cout << prompt;
            getline(cin, choice);
            if (choice.length() != 1 || (toupper(choice[0]) != 'Y' && toupper(choice[0]) != 'N')) {
                cout << "\x1b[31m\nInvalid choice. Please choose again.\x1b[0m" << endl;
                continue;
            }
            if (toupper(choice[0]) == 'Y') {
                deleteBook();
            }
            else if (toupper(choice[0]) == 'N') {
                break;
            }
        } while (true);
    }

    // YesNo Prompt for Search Book
    if ((userMode == 'A' || userMode == 'U') && origin == 'Z') {
        do {
            cout << prompt;
            getline(cin, choice);
            if (choice.length() != 1 || (toupper(choice[0]) != 'Y' && toupper(choice[0]) != 'N')) {
                cout << "\x1b[31m\nInvalid choice. Please choose again.\x1b[0m" << endl;
                continue;
            }
            if (toupper(choice[0]) == 'Y') {
                searchBook();
            }
            else if (toupper(choice[0]) == 'N') {
                break;
            }
        } while (true);
    }

    // YesNo Prompt for Borrow Book
    if (userMode == 'U' && origin == 'B') {
        do {
            cout << prompt;
            getline(cin, choice);
            if (choice.length() != 1 || (toupper(choice[0]) != 'Y' && toupper(choice[0]) != 'N')) {
                cout << "\x1b[31m\nInvalid choice. Please choose again.\x1b[0m" << endl;
                continue;
            }
            if (toupper(choice[0]) == 'Y') {
                borrowBook();
            }
            else if (toupper(choice[0]) == 'N') {
                break;
            }
        } while (true);
    }

    // YesNo Prompt for Update Status
    if (userMode == 'A' && origin == 'U') {
        do {
            cout << prompt;
            getline(cin, choice);
            if (choice.length() != 1 || (toupper(choice[0]) != 'Y' && toupper(choice[0]) != 'N')) {
                cout << "\x1b[31m\nInvalid choice. Please choose again.\x1b[0m" << endl;
                continue;
            }
            if (toupper(choice[0]) == 'Y') {
                updateStatus();
            }
            else if (toupper(choice[0]) == 'N') {
                break;
            }
        } while (true);
    }

    // General Y/N prompt
    do {
        cout << "\n\x1b[32m[1]\x1b[0m Return to Menu\n\x1b[32m[2]\x1b[0m Exit";
        cout << "\n\nWhat would you like to do? ";
        getline(cin, returnChoice);
        if (returnChoice == "1") {
            if (userMode == 'A') adminMenu();
            else if (userMode == 'U') userMenu();
        }
        else if (returnChoice == "2") exitMenu();
        else {
            cout << "\x1b[31m\nInvalid choice. Please choose again.\x1b[0m" << endl;
        }
    } while (true);
}


void exitMenu() {
    // Thanks users and displays developers
    cout << "\x1b[32m \nThank you for using COA Library Management System!\x1b[0m" << endl << endl;
    cout << "\nDEVELOPERS: " << endl;
    cout << "Santos, Marianne Angelika B.\v" << "Letada, Angel A.\v" << "Damaso, Patricia Therese P.\v";
    cout << "Bautista, Kristian David R.\v" << "Facto, Janelle P. " << endl;
    exit(0);
}







