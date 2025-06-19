#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// Forward declaration for MainMenu to avoid scope issues
void MainMenu();

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

        if (c == '\n' || c == '\0' || c == '\N') //handle newline and end of line
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
            if (c == '\N') lines.push_back("");
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

// --- Quiz Logic ---
int runQuizQuestions(int level) {
    int score = 0;

    auto askQuestion = [](const string& question, char correctOption) -> bool {
        char answer;
        bool valid = false;
        while (!valid) {
            cout << question;
            cin >> answer;
            answer = toupper(answer);

            if (answer >= 'A' && answer <= 'D') {
                valid = true;
                return answer == correctOption;
            } else {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid choice! Please enter A, B, C, or D.\n";
            }
        }
        return false;
    };

    if (level == 1) {
        cout << "\n[Beginner Level]\n";
        DrawText_Margin("\nQ1. 2 + 2 = ? \NA. 3 \nB. 4 \nC. 5\nD. 6", 0);
        if (askQuestion("Answer: ", 'B')) score++;

        DrawText_Margin("\nQ2. What color is the sky on a clear day? \NA. Blue \nB. Green \nC. Red \nD. Yellow", 0);
        if (askQuestion("Answer: ", 'A')) score++;

        DrawText_Margin("\nQ3. What is the capital of Malaysia? \NA. Bangkok \nB. Manila \nC. Kuala Lumpur \nD. Jakarta", 0);
        if (askQuestion("Answer: ", 'C')) score++;

        DrawText_Margin("\nQ4. Which animal barks? \NA. Cat \nB. Cow \nC. Dog \nD. Elephant", 0);
        if (askQuestion("Answer: ", 'C')) score++;

        DrawText_Margin("\nQ5. 10 - 3 = ? \NA. 7 \nB. 6 \nC. 5 \nD. 8", 0);
        if (askQuestion("Answer: ", 'A')) score++;
    }
    else if (level == 2) {
        cout << "\n[Intermediate Level]\n";
        if (askQuestion("Q1. What is 6 * 7?\nA. 36\nB. 42\nC. 48\nD. 49\nAnswer: ", 'B')) score++;
        if (askQuestion("Q2. What language is this code?\nA. Java\nB. C++\nC. Python\nD. PHP\nAnswer: ", 'B')) score++;
        if (askQuestion("Q3. What does RAM stand for?\nA. Read All Memory\nB. Run Access Mode\nC. Real-time Access Method\nD. Random Access Memory\nAnswer: ", 'D')) score++;
        if (askQuestion("Q4. Which symbol is used to comment in C++?\nA. //#\nB. */\nC. //\nD. <!-- -->\nAnswer: ", 'C')) score++;
        if (askQuestion("Q5. What is the extension for a C++ source file?\nA. .txt\nB. .html\nC. .cpp\nD. .exe\nAnswer: ", 'C')) score++;
    }
    else if (level == 3) {
        cout << "\n[Advanced Level]\n";
        if (askQuestion("Q1. What is the binary of 5?\nA. 100\nB. 101\nC. 110\nD. 111\nAnswer: ", 'B')) score++;
        if (askQuestion("Q2. Who developed C++?\nA. Dennis Ritchie\nB. James Gosling\nC. Guido van Rossum\nD. Bjarne Stroustrup\nAnswer: ", 'D')) score++;
        if (askQuestion("Q3. What is 15 % 4?\nA. 3\nB. 2\nC. 4\nD. 1\nAnswer: ", 'A')) score++;
        if (askQuestion("Q4. What does OOP stand for?\nA. Object Operating Program\nB. Ordered Operation Process\nC. Object-Oriented Programming\nD. Optimal Object Planning\nAnswer: ", 'C')) score++;
        if (askQuestion("Q5. What is 3^2?\nA. 6\nB. 7\nC. 9\nD. 12\nAnswer: ", 'C')) score++;
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
    cout << "\nPlease enter your preference (1, 2, 3 or 4): ";
    cin >> choice_lvl;

    while (true)
    {
        if (cin.fail())// If non-integer entered
        {
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "\nInvalid input. Please enter a number (1, 2, 3 or 4): ";
            cin >> choice_lvl;
            continue;  //loop
        }

        if (choice_lvl < 1 || choice_lvl > 4)
        {
            cin.ignore();
            cout << "\nInvalid number. Please enter either 1, 2, 3 or 4 only: ";
            cin >> choice_lvl;
            continue;  //loop
        }

        break;
    }

    if (choice_lvl >= 1 && choice_lvl <= 3)
    {
        deleteFile("Quiz History.txt");
        string levelNames[3] = {"Beginner", "Intermediate", "Advanced"};
        autocreateFile("Quiz History.txt","Marks obtained from " + levelNames[choice_lvl - 1] + " level:");
        system("cls");

        int score = runQuizQuestions(choice_lvl);
        autoappendFile("Quiz History.txt", to_string(score) + "/5");

        system("pause");
        system("cls");
        MainMenu();
    }
    else
    {
        system("cls");
        MainMenu();
    }
}

void MainMenu()
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

    while (true)
    {
        if (cin.fail())// If non-integer entered
        {
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "\nInvalid input. Please enter a number (1, 2, or 3): ";
            cin >> choice;
            continue;  //loop
        }

        if (choice != 1 && choice !=2 && choice !=3)
        {
            cin.ignore();
            cout << "\nInvalid number. Please enter either 1, 2 or 3: ";
            cin >> choice;
            continue;  //loop
        }

        break;
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
    else
    {
        exit(0);
    }
}

int main()
{
    getName();

    MainMenu();
    return 0;
}
