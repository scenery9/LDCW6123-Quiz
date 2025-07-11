#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include <vector>
#include <variant>

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
    DrawText_Margin("Greetings. ^This is a program which is specifically developed as a short quiz themed Social Media Etiquette and Digital Citizenship. ^Before we start the quiz, may I know how would you like to be addressed?", 0);
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

    cout << "\nWelcome " << name << "! \n";

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
    cout << "\n\n" << endl;

    cout << "\nEnter any key to continue the quiz or [P] to pause: ";

    char key = cin.get();

    if (key == 'p' || key == 'P')
    {
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
        {;
            readFile("Current Score.txt");
            return PresstoEnter(level);
        }
        if (choice == 3)
        {
            MainMenu();
            return false;
        }

    }
    if (key != 'p' || key != 'P') //other keys
    {
        return true;
    }
    return true;
}

int runQuizQuestions(int& level) {

    auto askQuestion = [](const string& question, char correctOption) -> bool
    {
        string input;
        while (true)
        {
            cout << question;
            cin >> input;

            // Only accept a single character A�D
            if (input.length() == 1)
            {
                char answer = toupper(input[0]);
                if (answer >= 'A' && answer <= 'D')
                {
                    if (answer == correctOption)
                    {
                        cout << "\nCorrect!\n";
                        return true;
                    }
                    else
                    {
                        cout << "\nIncorrect. The correct answer is " << correctOption << ".\n";
                        return false;
                    }
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
        score = 0;

        cout << "\n[Beginner Level]\n";
        DrawText_Margin("\nQ1. What is digital citizenship? ^A. Avoiding the internet to stay safe \nB. Using digital devices only for school \nC. Responsible and appropriate behavior when using technology\nD. Teaching children how to code", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Current Score.txt");
        cin.ignore();
        autocreateFile("Current Score.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        cout << "\n[Beginner Level]\n";
        DrawText_Margin("\nQ2. What should a board member do when posting district information on social media? ^A. Make up new information to sound engaging \nB. Check accuracy and link to official sources \nC. Only post during school hours \nD. Use a fake name to post content", 1);
        if (askQuestion("Answer: ", 'B')) score++;
        deleteFile("Current Score.txt");
        cin.ignore();
        autocreateFile("Current Score.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        cout << "\n[Beginner Level]\n";
        DrawText_Margin("\nQ3. Digital etiquette means: ^A. Posting anything you want anonymously \nB. Using social media only with adult supervision \nC. Following respectful behavior online as you would in person \nD. Turning off devices completely", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Current Score.txt");
        cin.ignore();
        autocreateFile("Current Score.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        cout << "\n[Beginner Level]\n";
        DrawText_Margin("\nQ4. What is a major behavioral red flag that a teen may be experiencing cyberbullying? ^A. Reading books more often \nB. Becoming more talkative \nC. Suddenly avoiding technology \nD. Eating more fruits and vegetables", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Current Score.txt");
        cin.ignore();
        autocreateFile("Current Score.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");

        cout << "\n[Beginner Level]\n";
        DrawText_Margin("\nQ5. Which of the following is a possible academic sign that a teen may be cyberbullied? ^A. Joining more school clubs \nB. Getting into trouble at school \nC. Getting all A's in exams \nD. Attending extra classes", 1);
        if (askQuestion("Answer: ", 'B')) score++;
        deleteFile("Current Score.txt");
        cin.ignore();
        autocreateFile("Current Score.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        cin.ignore();
        if (FileExists("Quiz History.txt"))
        {
            autoappendFile("Quiz History.txt", "Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        }
        else
        {
            autocreateFile("Quiz History.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        }

        string ex;
        char confirm;
        DrawText_Margin("\nCongrats! You have obtained a total of ^ \n" + string (39, ' ') + to_string(score) + "/5 ^Do you want to continue to next level? \nType [Y] to Continue, [N] to Quit to Main", 1);

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
                MainMenu();
                break;
            }
            break;
        }
    }
    else if (level == 2) {

        score = 0;

        cout << "\n[Intermediate Level]\n";
        DrawText_Margin("\nQ1. Which of the following is part of digital safety? ^A. Posting every detail of your daily life online \nB. Giving a false age to sign up for websites \nC. Protecting personal information online \nD. Ignoring harmful content", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Current Score.txt");
        cin.ignore();
        autocreateFile("Current Score.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        cout << "\n[Intermediate Level]\n";
        DrawText_Margin("\nQ2. Which of the following is NOT a recommended method to teach digital etiquette? ^A. Discussing the permanency of online posts \nB. Encouraging device use in bathrooms \nC. Role modeling respectful online behavior \nD. Setting limits on screen time", 1);
        if (askQuestion("Answer: ", 'B')) score++;
        deleteFile("Current Score.txt");
        cin.ignore();
        autocreateFile("Current Score.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        cout << "\n[Intermediate Level]\n";
        DrawText_Margin("\nQ3. What is one strategy parents can use to help ensure their child's online safety? ^A. Let them use the internet freely at night \nB. Avoid checking their social media \nC. Set privacy controls on devices and accounts \nD. Allow unlimited screen time", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Current Score.txt");
        cin.ignore();
        autocreateFile("Current Score.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        cout << "\n[Intermediate Level]\n";
        DrawText_Margin("\nQ4. What is one legal reason board members should avoid discussing school business with each other on social media? ^A. It can increase their social media traffic too much \nB. It can be considered a violation of open meeting laws \nC. It will confuse students and teachers \nD. It prevents them from attending board meetings", 1);
        if (askQuestion("Answer: ", 'B')) score++;
        deleteFile("Current Score.txt");
        cin.ignore();
        autocreateFile("Current Score.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        cout << "\n[Intermediate Level]\n";
        DrawText_Margin("\nQ5. What does 'guilty by association' mean in social media context? ^A. You get blamed for your friend's hacking \nB. Your friend deletes your account \nC. You are judged based on photos of your friends \nD. You are guilty of breaking laws", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Current Score.txt");
        cin.ignore();
        autocreateFile("Current Score.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");

        if (FileExists("Quiz History.txt"))
        {
            autoappendFile("Quiz History.txt", "Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        }
        else
        {
            autocreateFile("Quiz History.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        }

        string ex;
        char confirm;
        DrawText_Margin("\nCongrats! You have obtained a total of ^ \n" + string (39, ' ') + to_string(score) + "/5 ^Do you want to continue to next level? \nType [Y] to Continue, [N] to Quit to Main", 1);

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
                MainMenu();
                break;
            }
            break;
        }

    }
    else if (level == 3) {

        score = 0;
        cout << "\n[Advanced Level]\n";

        DrawText_Margin("\nQ1. Why is it important to keep an open dialogue with teens about digital behavior? ^A. To punish them when they do wrong \nB. To track every message they send \nC. So they feel safe to share their experiences \nD. So they avoid making friends online", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Current Score.txt");
        cin.ignore();
        autocreateFile("Current Score.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        cout << "\n[Advanced Level]\n";
        DrawText_Margin("\nQ2. Why might teens engage in inappropriate online behavior even if they didn't plan to? ^A. Because parents do not monitor them \nB. Because social media encourages good habits \nC. Because they are trying to impress others \nD. Because they access content using false ages", 1);
        if (askQuestion("Answer: ", 'D')) score++;
        deleteFile("Current Score.txt");
        cin.ignore();
        autocreateFile("Current Score.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        cout << "\n[Advanced Level]\n";
        DrawText_Margin("\nQ3. How does parental involvement affect teen digital behavior? ^A. It encourages teens to hide things \nB. It increases risk-taking online \nC. It supports appropriate and responsible use \nD. It has no effect", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Current Score.txt");
        cin.ignore();
        autocreateFile("Current Score.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        cout << "\n[Advanced Level]\n";
        DrawText_Margin("\nQ4. What does the SIFT Method help students with? ^A. Memorizing facts for exams \nB. Managing physical fitness \nC. Evaluating questionable content and misinformation online \nD. Tracking screen time", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Current Score.txt");
        cin.ignore();
        autocreateFile("Current Score.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        PresstoEnter(level);

        cout << "\n[Advanced Level]\n";
        DrawText_Margin("\nQ5. What is a major risk of using inappropriate photos online? ^A. They take up space \nB. They could cause your device to slow down \nC. They may be the top result on search engines and hurt your career \nD. They don't get enough likes", 1);
        if (askQuestion("Answer: ", 'C')) score++;
        deleteFile("Current Score.txt");
        cin.ignore();
        autocreateFile("Current Score.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");

        if (FileExists("Quiz History.txt"))
        {
            autoappendFile("Quiz History.txt", "Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        }
        else
        {
            autocreateFile("Quiz History.txt","Marks obtained from " + levelNames[level - 1] + " level: " + to_string(score) + "/5\n");
        }

        string ex;
        char confirm;
        DrawText_Margin("\nCongrats! You have obtained a total of ^ \n" + string (39, ' ') + to_string(score) + "/5 ^You have completed the hardest quiz from us. Exiting to main...", 1);

        MainMenu();
    }
    return score;
}

void DifficultyMenu()
{

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
            string levelNames[3] = {"Beginner", "Intermediate", "Advanced"};

            int score = runQuizQuestions(choice_lvl);

            MainMenu();
        }
        else
        {
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
                readFile("Quiz History.txt");
                MainMenu();
            }
            else
            {
                cout << "Error. File not found. Please try a quiz first. " << endl;
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
