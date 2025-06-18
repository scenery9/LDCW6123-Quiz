#include <iostream>
#include <string>
#include <conio.h> //Keys
#include <windows.h> //Sleep
#include <fstream> //filehandling
#include <sstream>
#include <vector>


#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13
#define KEY_ESCAPE 27

using namespace std;

//border
void DrawTopBorder();
void DrawBottomBorder() ;

//default position for menu
int z = 0;

//file handling
void autocreateFile(const string& filename, const string& content);
bool FileExists(const string& filename);
void readFile(const string& filename);
void autoappendFile(const string& filename, const string& content);
void deleteFile(const string& filename);

//Menu
void DifficultyMenu();
void MainMenu();

void menuselectionbox(string slot[], int slotsize);

bool PresstoEnter();

//Question
void DrawQuestion(string text , int typeSpeed);
void DrawQuestion_Margin(string text , int typeSpeed);



// Border initializations
const int BORDER_WIDTH = 80; // Adjustable size of border, affects every draw function.
const int NUM_SLOT = 4;
string SLOT[NUM_SLOT];

void DrawTopBorder() {
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    cout << "|" << string(BORDER_WIDTH, ' ') << "|" << endl;
}

void DrawBottomBorder() {
    cout << "|" << string(BORDER_WIDTH, ' ') << "|" << endl;
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
}

const int MENU_SIDE= 30;
const int MARGIN = 20;

void DrawMenuTop()
{
    cout << string (MENU_SIDE, ' ') << "+" << string (MARGIN, '-') << "+" << endl;
    cout << string (MENU_SIDE, ' ') << "|" << string (MARGIN, ' ') << "|" << endl;
}

void DrawMenuBottom()
{
    cout << string (MENU_SIDE, ' ') << "|" << string (MARGIN, ' ') << "|" << endl;
    cout << string (MENU_SIDE, ' ') << "+" << string (MARGIN, '-') << "+" << endl;
}


void DrawMenu_Margin(string text, int typeSpeed)
{
    int marginSpaces = 30;
    const int boxWidth = BORDER_WIDTH - 2 * marginSpaces; // = 20

        // Left margin and border
        cout << string(marginSpaces, ' ') << "|";

        // Text with typing effect
        for (char c : text) {
            cout << c << flush;
            Sleep(typeSpeed);
        }

        // Right padding and border
        int padding = boxWidth - text.length();
        if (padding > 0) {
            cout << string(padding, ' ');
        }
        cout << "|" << endl;

}


const int TITLE_MARGIN = 30;

void DrawText_Margin(string text, int typeSpeed) {
    const int marginSpaces = 10;
    const int boxWidth = BORDER_WIDTH - 2 * marginSpaces; // = 60

    istringstream iss(text);
    string word;
    vector<string> lines;
    string currentLine;

    // Build lines with word wrapping
    while (iss >> word) {
        // Break long words that exceed boxWidth
        while (word.length() > static_cast<size_t>(boxWidth)) {
            if (!currentLine.empty()) {
                lines.push_back(currentLine);
                currentLine.clear();
            }
            lines.push_back(word.substr(0, boxWidth));
            word = word.substr(boxWidth);
        }

        if (currentLine.empty()) {
            currentLine = word;
        }
        else if (currentLine.length() + 1 + word.length() <= static_cast<size_t>(boxWidth)) {
            currentLine += " " + word;
        }
        else {
            lines.push_back(currentLine);
            currentLine = word;
        }
    }
    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }

    // Print all lines with borders and typing effect
    for (const string& line : lines)
    {
        // Left margin and border
        cout << "|" << string(marginSpaces, ' ');

        // Text with typing effect
        for (char c : line) {
            cout << c << flush;
            Sleep(typeSpeed);
        }

        // Right padding and border
        int padding = boxWidth - line.length();
        if (padding >= 0) {
            cout << string(padding+marginSpaces, ' ');
        }
        cout << "|" << endl;
    }
}

void autocreateFile(const string& filename, const string& content) //new quiz
{
    ofstream file(filename, ios::trunc);//open
    if (file)
    {
        file << "\n" << content;
        file.close();
    }


    else //file.fail()
    {
        cout << "Error. Unable to add '" << filename << "'! Exiting...\n";
        system("pause");
    }
}

void autoappendFile(const string& filename, const string& content)//for create/update Quiz History
{
    ofstream file(filename, ios::app); //declare and open output file
    if (file.fail())
    {
        cout << "Error. Unable to append file '" << filename << "'! Exiting...\n" << endl;
        return;
    }

    file << content << "\n\n";
    file.close();
}

//check file existence
bool FileExists(const string& filename)
{
    // opening the file
    fstream file(filename); //declare read/write file

    if (file)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void readFile(const string& filename)
{
    string tmp;
    fstream file(filename); //declare read/write file
    if (file.fail())
    {
        cout << "Error. File not found! \n";
        return;
    }
    cout << "\nContents of '" << filename << "': \n";
    while (getline(file, tmp))
    {
        cout << tmp << endl;
    }
    file.close();
}

void deleteFile(const string& filename)
{

    //check existence
    ifstream file (filename);
    if (file)
    {
        file.close(); //prevent unwanted errors
        if (remove(filename.c_str()) == 0)
        {
           return;
        }
    }
    return;
}

void MainMenu ()
{
    int choice;

    DrawMenuTop();
    DrawMenu_Margin("     Main Menu", 0);
    DrawMenu_Margin(" ", 0);
    DrawMenu_Margin(" 1. New Quiz", 0);
    DrawMenu_Margin(" 2. History ", 0);
    DrawMenu_Margin(" 3. Exit", 0);
    DrawMenuBottom();

    cout << "\nEnter your choice (1/ 2/ 3): ";
    cin >> choice;

    while (choice < 1 || choice > 3)
    {
        cin.ignore();
        cout << "Invalid input received! Please enter your preference (1, 2 or 3): ";
        cin >> choice;
    }

    if (choice >= 1 && choice <=3)
    {
        if (choice == 1)
        {
            DifficultyMenu();
        }
        if (choice == 2)
        {
            FileExists("Quiz History.txt");
            if (FileExists("Quiz History.txt"))
            {
                system("cls");
                readFile("Quiz History.txt");
                system("pause");

                system("cls");
                MainMenu();
            }
            else
            {
                cout << "Error. File not found. Please try a quiz first. " << endl;
                system ("pause");
                system ("cls");
                MainMenu();
            }
        }
        if (choice == 3)
        {
            exit(0);
        }
    }


}

void DifficultyMenu ()
{
    system ("cls");

    DrawMenuTop();
    DrawMenu_Margin("  Difficulty Level", 0);
    DrawMenu_Margin(" ", 0);
    DrawMenu_Margin("1. Beginner", 0);
    DrawMenu_Margin("2. Intermediate", 0);
    DrawMenu_Margin("3. Advanced", 0);
    DrawMenu_Margin("4. Back to Main Menu", 0);
    DrawMenuBottom();

    int choice_lvl;
    cout << "\nPlease enter your preference (1, 2, 3 or 4): ";
    cin >> choice_lvl;

    while (choice_lvl < 1 || choice_lvl > 4)
    {
        cin.ignore();
        cout << "Invalid input received! Please enter your preference (1, 2, 3 or 4): ";
        cin >> choice_lvl;

    } ;

    if (choice_lvl >= 1 && choice_lvl <=4)
    {
        if (choice_lvl == 1)
        {
            deleteFile("Quiz History.txt");
            autocreateFile ("Quiz History.txt", "Marks obtained from Beginner's level: ");
            system ("cls");
            cout << "Beginner" << endl;

            autoappendFile("Quiz History.txt", "4/5");
            system ("pause");

            system ("cls");
            MainMenu();
        }
        if (choice_lvl == 2)
        {
            system ("cls");
            cout << "Intermediate" << endl;

            autoappendFile("Quiz History.txt", "Marks obtained from Intermediate's level: 4/5");

            system ("cls");
            MainMenu();
        }
        if (choice_lvl == 3)
        {
            system ("cls");
            cout << "Advanced" << endl;
            system ("pause");

            system ("cls");
            MainMenu();
        }
        else
        {
            system("cls");
            MainMenu();
        }
    }

    system ("pause");
}


int main()
{
    string name;

    cout << "Enter your name: ";
    getline(cin, name);

    if (name.empty())
    {
        cout << "Empty input! Please enter your name again. \n";
        cout << "Your name: ";
        getline(cin, name);
    }
        // Convert to uppercase
        for (char &c : name)
        {
            c = toupper(c);
        }

    system("cls");
    cout << "Welcome " << name << "! \n";

    MainMenu();

    return 0;
}
