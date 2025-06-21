#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include <vector>
#include <variant>

#include <conio.h>

#define KEY_ENTER 13
#define KEY_ESCAPE 27

using namespace std;

//--- Declaration ---
// Forward declaration for MainMenu to avoid scope issues
void MainMenu();

// Quiz logic
void scoresheet();

// Border sizes
const int BORDER_WIDTH = 80;
const int MENU_SIDE = 30;
const int MARGIN = 20;
const int TITLE_MARGIN = 30;
const int NUM_SLOT = 4;
string SLOT[NUM_SLOT];

// --- Border Drawing ---
void DrawTopBorder() {
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
    cout << "|" << string(BORDER_WIDTH, ' ') << "|" << endl;
}

void DrawBottomBorder() {
    cout << "|" << string(BORDER_WIDTH, ' ') << "|" << endl;
    cout << "+" << string(BORDER_WIDTH, '-') << "+" << endl;
}

void DrawMenuTop() {
    cout << string(MENU_SIDE, ' ') << "+" << string(MARGIN, '-') << "+" << endl;
    cout << string(MENU_SIDE, ' ') << "|" << string(MARGIN, ' ') << "|" << endl;
}

void DrawMenuBottom() {
    cout << string(MENU_SIDE, ' ') << "|" << string(MARGIN, ' ') << "|" << endl;
    cout << string(MENU_SIDE, ' ') << "+" << string(MARGIN, '-') << "+" << endl;
}

void DrawMenu_Margin(string text, int typeSpeed) {
    int marginSpaces = 30;
    const int boxWidth = BORDER_WIDTH - 2 * marginSpaces;
    cout << string(marginSpaces, ' ') << "|";
    for (char c : text) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(typeSpeed));
    }
    int padding = boxWidth - text.length();
    if (padding > 0) {
        cout << string(padding, ' ');
    }
    cout << "|" << endl;
}

void DrawText_Margin(string text, int typeSpeed) {
    const int marginSpaces = 10;
    const int boxWidth = BORDER_WIDTH - 2 * marginSpaces; //60 text width

    vector<string> lines; //store
    string currentLine; //currentoutput
    string word; //read

    DrawTopBorder();

    for (size_t i = 0; i <= text.length(); ++i) {
        char c = text[i];

        if (c == '\n' || c == '\0' || c == '^') //handle newline and end of line
        {
            // Add word to current line or push to newline
            if (!word.empty()) {
                if (currentLine.length() + word.length() + (currentLine.empty() ? 0 : 1) <= boxWidth) {
                    if (!currentLine.empty())
                    {
                        currentLine += " ";
                    }
                    currentLine += word;
                }
                else
                {
                    if (!currentLine.empty())
                    {
                        lines.push_back(currentLine);
                    }

                    currentLine = word;
                }
                word.clear();
            }
            if (!currentLine.empty()) // if there's text in line, push to current
                {
                    lines.push_back(currentLine);
                }

            currentLine.clear();

            // treat newline as empty line according to need
            if (c == '^') lines.push_back("");
        }
        else if (isspace(c))//detect space/tab
        {
            if (!word.empty())
            {
                if (currentLine.length() + word.length() + (currentLine.empty() ? 0 : 1) <= boxWidth)//check if text fits in 60
                {
                    if (!currentLine.empty()) currentLine += " ";
                    currentLine += word;
                }
                else
                {
                    if (!currentLine.empty()) lines.push_back(currentLine);
                    currentLine = word;
                }
                word.clear();
            }
        }
        else
        {
            word += c;
        }

        // Handle long word breaking
        while (word.length() > static_cast<size_t>(boxWidth)) {
            if (!currentLine.empty()) {
                lines.push_back(currentLine);
                currentLine.clear();
            }
            lines.push_back(word.substr(0, boxWidth));
            word = word.substr(boxWidth);
        }
    }

    if (!currentLine.empty()) lines.push_back(currentLine);

    // Display
    for (const string& line : lines)
    {
        cout << "|" << string(marginSpaces, ' ');
        for (char c : line) {
            cout << c << flush;
            this_thread::sleep_for(chrono::milliseconds(typeSpeed));
        }
        int padding = boxWidth - line.length();
        if (padding >= 0)
        {
            cout << string(padding + marginSpaces, ' '); //spacing for box border
        }
        cout << "|" << endl;
    }

    DrawBottomBorder();
}

// --- File Handling ---
void autocreateFile(const string& filename, const string& content)
{
    ofstream file(filename, ios::trunc);
    if (file) {
        file << "\n" << content;
        file.close();
    } else {
        cout << "Error. Unable to add '" << filename << "'! Exiting...\n";
        system("pause");
    }
}

void autoappendFile(const string& filename, const string& content)
{
    ofstream file(filename, ios::app);
    if (file.fail()) {
        cout << "Error. Unable to append file '" << filename << "'! Exiting...\n" << endl;
        return;
    }
    file << content << "\n\n";
    file.close();
}

bool FileExists(const string& filename)
{
    fstream file(filename);
    return file.good();
}

void readFile(const string& filename)
{
    string tmp;
    fstream file(filename);
    if (file.fail()) {
        cout << "Error. File not found! \n";
        return;
    }
    cout << "\nContents of '" << filename << "': \n";
    while (getline(file, tmp)) {
        cout << tmp << endl;
    }
    file.close();
}

void deleteFile(const string& filename) {
    ifstream file(filename);
    if (file) {
        file.close();
        remove(filename.c_str());
    }
}

bool isValidChoice3(string check, int& choice) {
    // Check if the entire string is a valid integer and no extra characters exist
    istringstream iss(check);
    if (!(iss >> choice)) return false; // Not a number
    string leftover;
    if (iss >> leftover) return false; // Extra junk after number
    return (choice == 1 || choice == 2 || choice == 3); // Valid range
}

// --- Name Input ---
string getName()
{
    string name;
    cout << "Enter your name: ";
    getline(cin, name);

    if (name.empty()) {
        cout << "Empty input! Please enter your name again. \n";
        cout << "Your name: ";
        getline(cin, name);
    }

    for (char& c : name) {
        c = toupper(c);
    }

    system("cls");
    cout << "Welcome " << name << "! \n";

    return name;
}

//--- Error Check ---

bool isValidExit(string ex, char& confirm)
{
    if (ex.length()!= 1) return false; // Not a number

    confirm = toupper(ex[0]); //convert into upper case

    return (confirm == 'Y' || confirm == 'N'); // Valid range
}

bool isValidChoice4(string check, int& choice_lvl) {
    // Check if the entire string is a valid integer and no extra characters exist
    istringstream iss(check);
    if (!(iss >> choice_lvl)) return false; // Not a number
    string leftover;
    if (iss >> leftover) return false; // Extra junk after number
    return (choice_lvl == 1 || choice_lvl == 2 || choice_lvl == 3 || choice_lvl == 4);
}


// --- Quiz Logic ---
int score = 0;

bool PresstoEnter(int& level)
{
    cout << "\n\n\n" << endl;


    cout << "Press any key to continue the quiz or ESC to pause...";

    int key = getch();
    if (key == KEY_ESCAPE)
    {
        system("cls");
        cout << endl;
        DrawMenuTop();
        DrawMenu_Margin("    Resume Menu", 0);
        DrawMenu_Margin("1. Resume", 0);
        DrawMenu_Margin("2. Current Score", 0);
        DrawMenu_Margin("3. Quit to Main", 0);
        DrawMenuBottom();

        string input;
        int choice;

        cin.ignore();

        while (true)
        {
            cout << "Please enter a valid choice (1/ 2/ 3): ";
            getline (cin, input);

            if (!(isValidChoice3(input, choice)))
            {
                cout << "\nInvalid input! ";
                continue;
            }
            break;
        }

        if (choice == 1) return true;
        if (choice == 2)
        {
            system("cls");
            readFile("Quiz History.txt");
            return PresstoEnter(level);
        }
        if (choice == 3)
        {
            system("cls");
            MainMenu();
            return false;
        }

    }
    else //other keys
    {
        return true;
    }
}

int runQuizQuestions(int& level) {

    auto askQuestion = [](const string& question, char correctOption) -> bool
    {
        string input;
        while (true)
        {
            cout << question;
            cin >> input;

            // Only accept a single character A–D
            if (input.length() == 1)
            {
                char answer = toupper(input[0]);
                if (answer >= 'A' && answer <= 'D')
                {
                    return answer == correctOption;
                }
            }

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice! Please enter A, B, C, or D.\n";
        }
    };

    string levelNames[3] = {"Beginner", "Intermediate", "Advanced"};

    if (level == 1)
    {
        system ("cls");
        cout << "\n[Beginner Level]\n";
        DrawText_Margin("\nWhat is digital citizenship? ^A. Avoiding the internet to stay safe \nB. Using digital devices only for school \nC. Responsible and appropriate behavior when using technology\nD. Teaching children how to code", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Quiz History.txt");
        autocreateFile("Quiz History.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        system ("cls");
        cout << "\n[Beginner Level]\n";
        DrawText_Margin("\nQ2. What percentage of American teens (ages 12 to 17) use the internet? ^A. 44% \nB. 67% \nC. 88% \nD. 95%", 1);
        if (askQuestion("Answer: ", 'D')) score++;
        deleteFile("Quiz History.txt");
        autocreateFile("Quiz History.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        system ("cls");
        cout << "\n[Beginner Level]\n";
        DrawText_Margin("\nQ3. Digital etiquette means: ^A. Posting anything you want anonymously \nB. Using social media only with adult supervision \nC. Following respectful behavior online as you would in person \nD. Turning off devices completely", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Quiz History.txt");
        autocreateFile("Quiz History.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        system ("cls");
        cout << "\n[Beginner Level]\n";
        DrawText_Margin("\nQ4. Which animal barks? ^A. Cat \nB. Cow \nC. Dog \nD. Elephant", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Quiz History.txt");
        autocreateFile("Quiz History.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        system ("cls");
        cout << "\n[Beginner Level]\n";
        DrawText_Margin("\nQ5. 10 - 3 = ? ^A. 7 \nB. 6 \nC. 5 \nD. 8", 1);
        if (askQuestion("Answer: ", 'A')) score++;
        deleteFile("Quiz History.txt");
        autocreateFile("Quiz History.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        system("pause");

        cin.ignore();
        system("cls");

        string ex;
        char confirm;
        DrawText_Margin("\nCongrats! You have obtained a total of^" + string (39, ' ') + to_string(score) + "/5 ^Do you want to continue to next level? \nType [Y] to Continue, [N] to Quit to Main", 1);

        while (true)
        {
            cout << "Answer: ";
            getline (cin, ex);

            if (!isValidExit(ex, confirm))
            {
                cout << "Invalid input! Please enter Y or N only." << endl;
                continue;
            }
            if (confirm == 'Y')
            {
                level = 2;
                runQuizQuestions(level);
                break;
            }
            if (confirm == 'N')
            {
                system("cls");
                MainMenu();
                break;
            }
            break;
        }
    }
    else if (level == 2) {
        system ("cls");

        cout << "\n[Intermediate Level]\n";
        DrawText_Margin("\nQ1. Which of the following is part of digital safety? ^A. Posting every detail of your daily life online \nB. Giving a false age to sign up for websites \nC. Protecting personal information online \nD. Ignoring harmful content", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Quiz History.txt");
        autocreateFile("Quiz History.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        system ("cls");
        cout << "\n[Intermediate Level]\n";
        DrawText_Margin("\nQ2. Which of the following is NOT a recommended method to teach digital etiquette? ^A. Discussing the permanency of online posts \nB. Encouraging device use in bathrooms \nC. Role modeling respectful online behavior \nD. Setting limits on screen time", 1);
        if (askQuestion("Answer: ", 'B')) score++;
        deleteFile("Quiz History.txt");
        autocreateFile("Quiz History.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        system ("cls");
        cout << "\n[Intermediate Level]\n";
        DrawText_Margin("\nQ3. What is one strategy parents can use to help ensure their child's online safety? ^A. Let them use the internet freely at night \nB. Avoid checking their social media \nC. Set privacy controls on devices and accounts \nD. Allow unlimited screen time", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Quiz History.txt");
        autocreateFile("Quiz History.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        system ("cls");
        cout << "\n[Intermediate Level]\n";
        DrawText_Margin("\nQ4. Which symbol is used to comment in C++? ^A. //# \nB. */ \nC. // \nD. <!-- -->", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Quiz History.txt");
        autocreateFile("Quiz History.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        system ("cls");
        cout << "\n[Intermediate Level]\n";
        DrawText_Margin("\nQ5. What is the extension for a C++ source file? ^A. .txt \nB. .html \nC. .cpp \nD. .exe", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Quiz History.txt");
        autocreateFile("Quiz History.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        system("pause");

        cin.ignore();
        system("cls");

        string ex;
        char confirm;
        DrawText_Margin("\nCongrats! You have obtained a total of ^" + string (39, ' ') + to_string(score) + "/5 ^Do you want to continue to next level? \nType [Y] to Continue, [N] to Quit to Main", 1);

        while (true)
        {
            cout << "Answer: ";
            getline (cin, ex);

            if (!isValidExit(ex, confirm))
            {
                cout << "Invalid input! Please enter Y or N only." << endl;
                continue;
            }
            if (confirm == 'Y')
            {
                level = 3;
                runQuizQuestions(level);
                break;
            }
            if (confirm == 'N')
            {
                system("cls");
                MainMenu();
                break;
            }
            break;
        }

    }
    else if (level == 3) {
        system("cls");

        cout << "\n[Advanced Level]\n";

        DrawText_Margin("\nQ1. Why is it important to keep an open dialogue with teens about digital behavior? ^A. To punish them when they do wrong \nB. To track every message they send \nC. So they feel safe to share their experiences \nD. So they avoid making friends online", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Quiz History.txt");
        autocreateFile("Quiz History.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        system ("cls");
        cout << "\n[Advanced Level]\n";
        DrawText_Margin("\nQ2. Why might teens engage in inappropriate online behavior even if they didn't plan to? ^A. Because parents do not monitor them \nB. Because social media encourages good habits \nC. Because they are trying to impress others \nD. Because they access content using false ages", 1);
        if (askQuestion("Answer: ", 'D')) score++;
        deleteFile("Quiz History.txt");
        autocreateFile("Quiz History.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        system ("cls");
        cout << "\n[Advanced Level]\n";
        DrawText_Margin("\nQ3. How does parental involvement affect teen digital behavior? ^A. It encourages teens to hide things \nB. It increases risk-taking online \nC. It supports appropriate and responsible use \nD. It has no effect", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Quiz History.txt");
        autocreateFile("Quiz History.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        system ("cls");
        cout << "\n[Advanced Level]\n";
        DrawText_Margin("\nQ4. What does OOP stand for? ^A. Object Operating Program \nB. Ordered Operation Process \nC. Object-Oriented Programming \nD. Optimal Object Planning", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Quiz History.txt");
        autocreateFile("Quiz History.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        system ("cls");
        cout << "\n[Advanced Level]\n";
        DrawText_Margin("\nQ5. What is 3^2? ^A. 6 \nB. 7 \nC. 9 \nD. 12", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Quiz History.txt");
        autocreateFile("Quiz History.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        system("pause");

        cin.ignore();

        system("cls");

        string ex;
        char confirm;
        DrawText_Margin("\nCongrats! You have obtained a total of ^" + string (39, ' ') + to_string(score) + "/5 ^You have completed the hardest quiz from us. Exiting to main...", 1);
        system("pause");

        system("cls");
        MainMenu();
    }
    return score;
}

void DifficultyMenu()
{
    system("cls");

    DrawMenuTop();
    DrawMenu_Margin("  Difficulty Level", 0);
    DrawMenu_Margin(" ", 0);
    DrawMenu_Margin("1. Beginner", 0);
    DrawMenu_Margin("2. Intermediate", 0);
    DrawMenu_Margin("3. Advanced", 0);
    DrawMenu_Margin("4. Back to Main Menu", 0);
    DrawMenuBottom();

    int choice_lvl;
    string check;

    while (true)
    {
        cout << "\nEnter your preference (1/ 2/ 3/ 4): ";
        getline (cin, check);

        if (!isValidChoice4(check, choice_lvl))
        {
            cout << "Invalid input. Please enter a valid number (1, 2, 3 or 4).\n";
            continue;
        }

        if (choice_lvl >= 1 && choice_lvl <= 3)
        {
            deleteFile("Quiz History.txt");
            string levelNames[3] = {"Beginner", "Intermediate", "Advanced"};
            autocreateFile("Quiz History.txt","Marks obtained from " + levelNames[choice_lvl - 1] + " level: " + to_string(score) + "/5^");
            system("cls");

            int score = runQuizQuestions(choice_lvl);

            system("pause");
            system("cls");
            MainMenu();
        }
        else
        {
            system("cls");
            MainMenu();
        }
        break;
    }
}

void MainMenu()
{
    DrawMenuTop();
    DrawMenu_Margin("     Main Menu", 0);
    DrawMenu_Margin(" ", 0);
    DrawMenu_Margin(" 1. New Quiz", 0);
    DrawMenu_Margin(" 2. History ", 0);
    DrawMenu_Margin(" 3. Exit", 0);
    DrawMenuBottom();

    int choice;
    string check;

    while (true)
    {
        cout << "\nEnter your choice (1/ 2/ 3): ";
        getline (cin, check);

        if (!isValidChoice3(check, choice))
        {
            cout << "Invalid input. Please enter a valid number (1, 2, or 3).\n";
            continue;
        }
        if (choice == 1)
        {
            DifficultyMenu();
        }
        else if (choice == 2)
        {
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
                system("pause");
                system("cls");
                MainMenu();
            }
        }
        else //choice 3
        {
            exit(0);
        }

        break;
    }
}

int main()
{
    getName();

    MainMenu();
    return 0;
}
