#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <limits>
#include <cctype>
#include <sstream>
#include <cstdlib>
using namespace std;

#ifndef EDITOR_H
#define EDITOR_H

#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#else
#define CLEAR_COMMAND "clear"
#endif

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

// ---------------------------
// Node for each line
// ---------------------------
struct LineNode
{
    string text;
    LineNode *next;
    LineNode *prev;
};

// ---------------------------
// Node for stack (editor state)
// ---------------------------
struct StackNode
{
    LineNode *editorHead;
    LineNode *cursorLinePtr;
    int cursorPos;
    int totalLines;
    StackNode *next;
};

// ---------------------------
// Stack using linked list
// ---------------------------
struct LinkedListStack
{
    StackNode *top;
    int size;
};

// ---------------------------
// Text Editor structure
// ---------------------------
struct TextEditor
{
    LineNode *head;
    LineNode *tail;
    LineNode *cursorLinePtr;
    int cursorPos;
    int totalLines;
    LinkedListStack undoStack;
    LinkedListStack redoStack;
};

// ---------------------------
// Function declarations
// ---------------------------
void deleteAllLines(LineNode *head);

LineNode* copyLines(LineNode *sourceHead, LineNode *sourceCursor, LineNode **destCursorPtr);

StackNode* createStackNode(LineNode* head, LineNode* cursor, int pos, int totalLines);

void pushStack(LinkedListStack* stack, LineNode* head, LineNode* cursor, int pos, int totalLines);

bool popStack(LinkedListStack* stack);

StackNode* peekStack(LinkedListStack* stack);

bool isStackEmpty(LinkedListStack* stack);

void clearStack(LinkedListStack* stack);

void initStack(LinkedListStack* stack);

void initEditor(TextEditor* editor);

void saveState(TextEditor* editor);

void restoreState(TextEditor* editor, StackNode* state);

void menu();
void displayContent(TextEditor* editor, string file);

void insertText(TextEditor* editor, const string& text);

void insertNewLine(TextEditor* editor);

void deleteChar(TextEditor* editor);

void deleteLine(TextEditor* editor);

void undo(TextEditor* editor);

void redo(TextEditor* editor);

void saveToFile(TextEditor* editor, const string& filename);

void loadFromFile(TextEditor* editor, const string& filename);

void moveCursor(TextEditor* editor, short direction);

void searchText(TextEditor* editor, const string& pattern);

void replaceText(TextEditor* editor, const string& oldText, const string& newText, bool replaceAll=false);

int countWords(TextEditor* editor);

int countChars(TextEditor* editor);

void showStatistics(TextEditor* editor);


#endif
// ---------------------------
// Varible globle declarations
// ---------------------------
extern string massage;
string massage = "";
int chose();
int chose_2();


// ---------------------------
// Main
// ---------------------------
int main()
{
    TextEditor editor;
    initEditor(&editor);
    int command, commend_2;
    string input;

    do
    {
        bool found = false;
        bool file_open = false;
        string file = "";
        menu();
        commend_2 = chose_2();
        if (commend_2 == 2)
        {
            cout << MAGENTA << "\n\tEnter filename: " << RESET;
            cin.ignore();
            getline(cin, file);
            loadFromFile(&editor, file);
            file_open = true;
        }
        if (commend_2 == 3)
        {
            cout << "Goodbye!\n";
            break;
        }
        do
        {
            displayContent(&editor, file);
            cout << setw(20) << massage << RESET << endl;
            command = chose();
            cin.ignore(); // clear newline from input buffer
            massage = "";
            switch (command)
            {
            case 1: // Insert text
                cout << "Enter text: ";
                getline(cin, input);
                insertText(&editor, input);
                break;

            case 4: // New line
                insertNewLine(&editor);
                break;

            case 2: // Delete character
                deleteChar(&editor);
                break;

            case 3: // Delete line
                deleteLine(&editor);
                break;

            case 5:
            case 6:
            case 7:
            case 8:
            case 11:
            case 12:
                moveCursor(&editor, command);
                break;

            case 10: // Undo
                undo(&editor);
                break;

            case 9: // Redo
                redo(&editor);
                break;

            case 16: // Save to file
            {
                if(!file_open && !found){
                cout << MAGENTA << "\n\tEnter filename: " << RESET;
                getline(cin, input);
                file = input;
                saveToFile(&editor, input);
                found = true;
                //cin.get();
                }
                if(file_open || found){
                saveToFile(&editor, file);
                found = true;
                }
            }
                break;

             // Load from file
            case 17:
            {
                if(!file_open && !found){
                cout << MAGENTA << "\n\tEnter filename: " << RESET;
                getline(cin, input);
                loadFromFile(&editor, input);
                }
                if(file_open){
                    cout<< "File is open "<<endl;
                }
                //cin.get();
            }
            break;

            case 14: // Find text
                cout << MAGENTA << "\n\tEnter text to search: " << RESET;
                getline(cin, input);
                searchText(&editor, input);
                //cin.get();
                break;

            case 13: // Replace
            {
                string oldText, newText;
                cout << MAGENTA << "\n\tEnter text to replace: " << RESET;
                getline(cin, oldText);
                cout << MAGENTA << "\n\tEnter new text: " << RESET;
                getline(cin, newText);

                char choice;
                cout << "Replace all? (y/n): ";
                cin >> choice;
                cin.ignore();
                replaceText(&editor, oldText, newText, (choice == 'y' || choice == 'Y'));
                cout << "Press Enter to continue...";
                cin.get();
            }
            break;

            case 15: // Statistics
                showStatistics(&editor);
                cout << "Press Enter to continue...";
                cin.get();
                break;

            case 18: // Exit
            {
                if (!found && !file_open)
                {
                    char saveChoice;
                    cout << "Do you want to save changes? (y/n): ";
                    cin >> saveChoice;
                    cin.ignore();

                    if (saveChoice == 'y' || saveChoice == 'Y')
                    {
                        cout << "Enter filename: ";
                        getline(cin, input);
                        saveToFile(&editor, input);
                    }
                }
                cout << "Goodbye!\n";
                deleteAllLines(editor.head);
                clearStack(&editor.undoStack);
                clearStack(&editor.redoStack);
                cout << "Press Enter to continue...";
                cin.get();
            }
            break;

            default:
                cout << "Unknown command!\n";
                cout << "Press Enter to continue...";
                cin.get();
                break;
            }
        } while (command != 18);

    } while (commend_2 != 3);

    return 0;
}


// ---------------------------
// Functions implementation
// ---------------------------


// Delete all lines
void deleteAllLines(LineNode *head)
{
    LineNode *current = head;
    while (current)
    {
        LineNode *next = current->next;
        delete current;
        current = next;
    }
}

// Copy lines
LineNode *copyLines(LineNode *sourceHead, LineNode *sourceCursor, LineNode **destCursorPtr)
{
    if (!sourceHead)
    {
        *destCursorPtr = nullptr;
        return nullptr;
    }

    LineNode *newHead = nullptr;
    LineNode *newTail = nullptr;
    LineNode *current = sourceHead;

    while (current)
    {
        LineNode *newNode = new LineNode;
        newNode->text = current->text;
        newNode->next = nullptr;
        newNode->prev = nullptr;

        if (!newHead)
        {
            newHead = newNode;
            newTail = newNode;
        }
        else
        {
            newTail->next = newNode;
            newNode->prev = newTail;
            newTail = newNode;
        }

        if (current == sourceCursor)
        {
            *destCursorPtr = newNode;
        }

        current = current->next;
    }

    if (!(*destCursorPtr) && newHead)
        *destCursorPtr = newHead;

    return newHead;
}

// ---------------------------
// Stack functions
// ---------------------------
StackNode *createStackNode(LineNode *head, LineNode *cursor, int pos, int totalLines)
{
    StackNode *node = new StackNode;
    node->cursorPos = pos;
    node->totalLines = totalLines;
    node->next = nullptr;

    if (head)
        node->editorHead = copyLines(head, cursor, &node->cursorLinePtr);
    else
    {
        node->editorHead = nullptr;
        node->cursorLinePtr = nullptr;
    }

    return node;
}

void pushStack(LinkedListStack *stack, LineNode *head, LineNode *cursor, int pos, int totalLines)
{
    StackNode *node = createStackNode(head, cursor, pos, totalLines);
    node->next = stack->top;
    stack->top = node;
    stack->size++;
}

bool popStack(LinkedListStack *stack)
{
    if (!stack->top)
        return false;

    StackNode *temp = stack->top;
    stack->top = temp->next;
    delete temp;
    stack->size--;
    return true;
}

StackNode *peekStack(LinkedListStack *stack)
{
    return stack->top;
}

bool isStackEmpty(LinkedListStack *stack)
{
    return stack->top == nullptr;
}

void clearStack(LinkedListStack *stack)
{
    while (!isStackEmpty(stack))
        popStack(stack);
}

void initStack(LinkedListStack *stack)
{
    stack->top = nullptr;
    stack->size = 0;
}

// ---------------------------
// Editor functions
// ---------------------------
void initEditor(TextEditor *editor)
{
    editor->head = new LineNode;
    editor->head->text = "";
    editor->head->next = nullptr;
    editor->head->prev = nullptr;
    editor->tail = editor->head;
    editor->cursorLinePtr = editor->head;
    editor->cursorPos = 0;
    editor->totalLines = 1;

    initStack(&editor->undoStack);
    initStack(&editor->redoStack);

    pushStack(&editor->undoStack, editor->head, editor->cursorLinePtr, editor->cursorPos, editor->totalLines);
}

void saveState(TextEditor *editor)
{
    pushStack(&editor->undoStack, editor->head, editor->cursorLinePtr, editor->cursorPos, editor->totalLines);
    clearStack(&editor->redoStack);
}

void restoreState(TextEditor *editor, StackNode *state)
{
    if (!state || !state->editorHead)
        return;

    deleteAllLines(editor->head);

    editor->head = copyLines(state->editorHead, state->cursorLinePtr, &editor->cursorLinePtr);
    editor->cursorPos = state->cursorPos;
    editor->totalLines = state->totalLines;

    editor->tail = editor->head;
    if (editor->tail)
    {
        while (editor->tail->next)
            editor->tail = editor->tail->next;
    }
}
// Count words
int countWords(TextEditor *editor)
{
    int wordCount = 0;
    bool inWord = false;

    LineNode *current = editor->head;
    while (current)
    {
        for (char c : current->text)
        {
            if (isalnum(c) || c == '_')
            {
                if (!inWord)
                {
                    wordCount++;
                    inWord = true;
                }
            }
            else
                inWord = false;
        }
        inWord = false;
        current = current->next;
    }
    return wordCount;
}

// Count characters
int countChars(TextEditor *editor)
{
    int charCount = 0;
    LineNode *current = editor->head;
    while (current)
    {
        charCount += current->text.length();
        current = current->next;
    }
    return charCount;
}

void content(string arr[], int end)
{
    for (int i = 0; i < end; i++)
        cout << YELLOW << i + 1 << ((i >= 9) ? "" : " ") << "> " << RESET << arr[i] << endl;
}
void menu()
{
    system(CLEAR_COMMAND);
    cout << "\n\t\t";
    cout << CYAN << " WELLCOM TO" << YELLOW << " TEXT EDITOR " << RESET;

    cout << GREEN << "\n ----- >>> Commands <<< -----\n"
         << RESET;
    string arr[] = {" New File ", " Open File ", " Exit "};
    content(arr, 3);
    cout << GREEN << "\n ----- >>> Commands <<< -----\n"
         << RESET;
}

void displayContent(TextEditor *editor ,string file)
{
    system(CLEAR_COMMAND);
    cout << GREEN << "\n\t File : "<<file<<"\n"
         << RESET;
    cout << YELLOW << "\n ----- >>> Commands <<< -----\n"
         << RESET;

    string arr[] = {"Insert text", "Delete character", "Delete line", "New line", "Up", "Down", "Left", "Right", "Redo", "Undo", "Begin line", "End line", "Replace", "Find word", "Statistics", "Save to file", "Open file", "Exit"};
    content(arr, 18);
    cout << YELLOW << " <<--------------------------->>\n\n"
         << RESET;
    cout << "\n\n";
    LineNode *current = editor->head;
    int lineNum = 1;
    int line = 0;
    cout << setw(15) << "  =================================" << endl;
    while (current)
    {
        cout << "||" << GREEN << lineNum << ": " << RESET << current->text; //<<setw(40)<<"||";
        if (current->text.empty())
            cout << "  ";
        cout << endl;

        if (current == editor->cursorLinePtr)
        {
            line = lineNum;
            cout << "||  ";
            for (int j = 0; j < editor->cursorPos; j++)
                cout << " ";
            cout << MAGENTA << " ^\n" //<<setw(30)
                 << RESET;            //<<"||"<<endl;
        }

        current = current->next;
        lineNum++;
    }
    cout << setw(15) << "  =================================" << endl;
    cout << setw(15) << "  ------------------------------" << endl;
    cout << "|" << CYAN << setw(10) << "   COL[ " << editor->cursorPos + 1 << " ]"
         << setw(10) << "    ROW[ " << line << " ]"
         << setw(10) << "    Charectars[ " << countChars(editor) << " ]"
         << setw(10) << "    Words[ " << countWords(editor) << " ]" << RESET;
    cout << setw(15) << "\n  ------------------------------" << endl;
}

// Insert text
void insertText(TextEditor *editor, const string &text)
{
    saveState(editor);

    if (editor->cursorPos > (int)editor->cursorLinePtr->text.length())
        editor->cursorPos = editor->cursorLinePtr->text.length();

    editor->cursorLinePtr->text.insert(editor->cursorPos, text);
    editor->cursorPos += text.length();
}

// New line
void insertNewLine(TextEditor *editor)
{
    saveState(editor);

    string currentLine = editor->cursorLinePtr->text;
    string firstPart = currentLine.substr(0, editor->cursorPos);
    string secondPart = currentLine.substr(editor->cursorPos);

    editor->cursorLinePtr->text = firstPart;

    LineNode *newNode = new LineNode;
    newNode->text = secondPart;
    newNode->next = editor->cursorLinePtr->next;
    newNode->prev = editor->cursorLinePtr;

    if (editor->cursorLinePtr->next)
        editor->cursorLinePtr->next->prev = newNode;

    editor->cursorLinePtr->next = newNode;
    if (editor->cursorLinePtr == editor->tail)
        editor->tail = newNode;

    editor->cursorLinePtr = newNode;
    editor->cursorPos = 0;
    editor->totalLines++;
}

// Delete character
void deleteChar(TextEditor *editor)
{
    if (!editor->head)
        return;
    saveState(editor);

    string &line = editor->cursorLinePtr->text;
    if (editor->cursorPos < (int)line.length())
    {
        line.erase(editor->cursorPos, 1);
    }
    else if (editor->cursorPos == (int)line.length() && editor->cursorLinePtr != editor->tail)
    {
        line += editor->cursorLinePtr->next->text;

        LineNode *toDelete = editor->cursorLinePtr->next;
        editor->cursorLinePtr->next = toDelete->next;
        if (toDelete->next)
            toDelete->next->prev = editor->cursorLinePtr;
        if (toDelete == editor->tail)
            editor->tail = editor->cursorLinePtr;
        delete toDelete;
        editor->totalLines--;
    }

    if (editor->cursorPos > 0)
        editor->cursorPos--;
}

// Delete line
void deleteLine(TextEditor *editor)
{
    if (editor->totalLines <= 1)
        return;
    saveState(editor);

    if (editor->cursorLinePtr == editor->head)
    {
        LineNode *newHead = editor->head->next;
        delete editor->head;
        editor->head = newHead;
        if (editor->head)
        {
            editor->head->prev = nullptr;
            editor->cursorLinePtr = editor->head;
        }
    }
    else
    {
        LineNode *toDelete = editor->cursorLinePtr;
        toDelete->prev->next = toDelete->next;
        if (toDelete->next)
            toDelete->next->prev = toDelete->prev;
        else
            editor->tail = toDelete->prev;

        editor->cursorLinePtr = toDelete->prev;
        delete toDelete;
    }

    editor->totalLines--;
    if (editor->cursorPos > (int)editor->cursorLinePtr->text.length())
        editor->cursorPos = editor->cursorLinePtr->text.length();
}

// Undo
void undo(TextEditor *editor)
{
    if (editor->undoStack.size <= 1)
    {
        massage = "\033[41m Cannot undo!";
        return;
    }

    pushStack(&editor->redoStack, editor->head, editor->cursorLinePtr, editor->cursorPos, editor->totalLines);
    popStack(&editor->undoStack);

    StackNode *prevState = peekStack(&editor->undoStack);
    if (prevState)
        restoreState(editor, prevState);
}

// Redo
void redo(TextEditor *editor)
{
    if (isStackEmpty(&editor->redoStack))
    {
        massage = "\033[41m Cannot redo!";
        return;
    }

    pushStack(&editor->undoStack, editor->head, editor->cursorLinePtr, editor->cursorPos, editor->totalLines);
    StackNode *redoState = peekStack(&editor->redoStack);
    if (redoState)
    {
        restoreState(editor, redoState);
        popStack(&editor->redoStack);
    }
}

// Save to file
void saveToFile(TextEditor *editor, const string &filename)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        massage = "\033[41m Error opening file!";
        return;
    }

    LineNode *current = editor->head;
    while (current)
    {
        file << current->text << endl;
        current = current->next;
    }
    file.close();
    massage = "\033[42m Saved: ";
    massage.append(filename);
}

// Load from file
void loadFromFile(TextEditor *editor, const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        massage = "\033[41m Error opening file!";
        massage.append(filename);
        return;
    }

    saveState(editor);
    deleteAllLines(editor->head);

    string line;
    LineNode *prev = nullptr;
    editor->head = nullptr;
    editor->totalLines = 0;

    while (getline(file, line))
    {
        LineNode *newNode = new LineNode;
        newNode->text = line;
        newNode->next = nullptr;
        newNode->prev = prev;

        if (!editor->head)
            editor->head = newNode;
        if (prev)
            prev->next = newNode;
        prev = newNode;
        editor->totalLines++;
    }

    if (!editor->head)
    {
        editor->head = new LineNode;
        editor->head->text = "";
        editor->tail = editor->head;
        editor->totalLines = 1;
    }
    else
        editor->tail = prev;

    editor->cursorLinePtr = editor->head;
    editor->cursorPos = 0;

    file.close();
    massage = "\033[42m Loaded: ";
    massage.append(filename);
}

// Move cursor
void moveCursor(TextEditor *editor, short direction)
{
    switch (direction)
    {
    case 5:
        if (editor->cursorLinePtr->prev)
        {
            editor->cursorLinePtr = editor->cursorLinePtr->prev;
            if (editor->cursorPos > (int)editor->cursorLinePtr->text.length())
                editor->cursorPos = editor->cursorLinePtr->text.length();
        }
        break;
    case 6:
        if (editor->cursorLinePtr->next)
        {
            editor->cursorLinePtr = editor->cursorLinePtr->next;
            if (editor->cursorPos > (int)editor->cursorLinePtr->text.length())
                editor->cursorPos = editor->cursorLinePtr->text.length();
        }
        break;
    case 7:
        if (editor->cursorPos > 0)
            editor->cursorPos--;
        break;
    case 8:
        if (editor->cursorPos < (int)editor->cursorLinePtr->text.length())
            editor->cursorPos++;
        break;
    case 11:
        editor->cursorPos = 0;
        break;
    case 12:
        editor->cursorPos = editor->cursorLinePtr->text.length();
        break;
    }
}

// Search text
void searchText(TextEditor *editor, const string &pattern)
{
    ostringstream mass;
    LineNode *current = editor->head;
    int lineNum = 0;
    int foundCount = 0;

    mass << YELLOW << "Search results for \"" << pattern << "\":" << RESET << endl;

    while (current)
    {
        size_t pos = current->text.find(pattern);
        while (pos != string::npos)
        {
            foundCount++;
            mass << GREEN << "\tLine " << lineNum + 1 << ", Position " << pos + 1 << RESET;
            pos = current->text.find(pattern, pos + 1);
        }
        current = current->next;
        lineNum++;
    }

    if (foundCount == 0)
        mass << "\033[41m\tNot found: \"" << pattern << "\"" << RESET;
    else
        mass << "\t\033[42m\033[30m Found " << foundCount << " items";
    massage = mass.str();
}

// Replace text
void replaceText(TextEditor *editor, const string &oldText, const string &newText, bool replaceAll)
{
    ostringstream mass;
    saveState(editor);

    int replacements = 0;
    LineNode *current = editor->head;

    while (current)
    {
        size_t pos = 0;
        while ((pos = current->text.find(oldText, pos)) != string::npos)
        {
            current->text.replace(pos, oldText.length(), newText);
            pos += newText.length();
            replacements++;
            if (!replaceAll)
            {
                mass << "\033[43m\033[30m Replaced " << replacements << " occurrence";
                massage = mass.str();
                return;
            }
        }
        current = current->next;
    }

    mass << "\033[42m\033[30m  Replaced " << replacements << " occurrence(s)";
    massage = mass.str();
}

// Show statistics
void showStatistics(TextEditor *editor)
{
    ostringstream mass;

    int words = countWords(editor);
    int chars = countChars(editor);

    mass << YELLOW << "\n \t>>>>>>>  Text Statistics  >>>>>>>" << RESET;
    mass << GREEN << "\n\t\tLines: " << editor->totalLines << "\n\t\t";
    mass << "Words: " << words << "\n\t\t";
    mass << "Characters: " << chars;
    mass << YELLOW << "\n \t>>----------------------------<<";
    massage = mass.str();
}

// Choose command
int chose()
{
    int Choice;
    do
    {
        cout << "\n >>> ";
        cin >> Choice;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << " \033[41\tmInvalid input!" << RESET << endl;
        }
        else if (Choice < 1 || Choice > 18)
        {
            cout << "\033[41\tmInvalid choice!" << RESET << endl;
            cin.clear();
        }
    } while (Choice < 1 || Choice > 18);
    return Choice;
}

int chose_2()
{
    int Choice;
    do
    {
        cout << "\n >>> ";
        cin >> Choice;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << " \033[41\tmInvalid input!" << RESET << endl;
        }
        else if (Choice < 1 || Choice > 3)
        {
            cout << "\033[41\tmInvalid choice!" << RESET << endl;
            cin.clear();
        }
    } while (Choice < 1 || Choice > 3);
    return Choice;
}
