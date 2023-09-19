#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <algorithm>
#include <vector>

using namespace std;

struct Node {
    char data;
    Node* next;

    Node(char c) : data(c), next(nullptr) {}
};

class LinkedList {
public:
    Node* head;
    LinkedList() : head(nullptr) {}

    void append(char c) {
        Node* newNode = new Node(c);
        if (!head) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    void display() {
        Node* current = head;
        while (current) {
            cout << current->data;
            current = current->next;
        }
        cout << endl;
    }
};

class TextEditor {
private:
    LinkedList text;
    stack<pair<char, int> > undoStack; // Pair for (operation, data_length)
    string filename;

public:
    TextEditor() : filename("") {}

    void insert(const string& data) {
        for (char c : data) {
            text.append(c);
            undoStack.push(make_pair('D', 1));
        }
    }

    void deleteText() {
        if (text.head) {
            char deletedChar = text.head->data;
            text.head = text.head->next;
            undoStack.push(make_pair('I', 1));
        }
    }

    void undo() {
        if (!undoStack.empty()) {
            pair<char, int> operation = undoStack.top();
            undoStack.pop();
            if (operation.first == 'I') {
                text.append(' '); // Placeholder for deleted characters
            } else if (operation.first == 'D') {
                for (int i = 0; i < operation.second; i++) {
                    if (text.head) {
                        text.head = text.head->next;
                    }
                }
            }
        }
    }

    void display() {
        text.display();
    }

    void saveToFile() {
        if (filename.empty()) {
            cout << "Enter a filename to save to: ";
            cin >> filename;
        }

        ofstream outFile(filename);
        Node* current = text.head;
        while (current) {
            outFile << current->data;
            current = current->next;
        }
        outFile.close();
        cout << "File saved as " << filename << endl;
    }

    void loadFromFile() {
        cout << "Enter a filename to load from: ";
        cin >> filename;

        ifstream inFile(filename);
        if (!inFile) {
            cout << "File not found." << endl;
            return;
        }

        char c;
        while (inFile.get(c)) {
            text.append(c);
        }
        inFile.close();
        cout << "File loaded from " << filename << endl;
    }

    void searchAndReplace(const string& searchTerm, const string& replaceTerm) {
        string document;
        Node* current = text.head;
        while (current) {
            document += current->data;
            current = current->next;
        }

        size_t found = document.find(searchTerm);
        while (found != string::npos) {
            document.replace(found, searchTerm.length(), replaceTerm);
            found = document.find(searchTerm, found + replaceTerm.length());
        }

        text.head = nullptr;
        for (char c : document) {
            text.append(c);
        }
    }

    void printStatistics() {
        string document;
        Node* current = text.head;
        while (current) {
            document += current->data;
            current = current->next;
        }

        int wordCount = 0;
        int charCount = 0;
        int lineCount = 0;
        int paragraphCount = 0;

        bool inWord = false;
        bool inParagraph = false;

        for (char c : document) {
            charCount++;

            if (c == ' ' || c == '\t' || c == '\n') {
                if (inWord) {
                    wordCount++;
                    inWord = false;
                }

                if (c == '\n') {
                    lineCount++;
                }

                if (c == '\n' && inParagraph) {
                    paragraphCount++;
                    inParagraph = false;
                }
            } else {
                inWord = true;
                if (c == '\n') {
                    inParagraph = true;
                }
            }
        }

        // Check if the last word/paragraph is not counted
        if (inWord) {
            wordCount++;
        }

        if (inParagraph) {
            paragraphCount++;
        }

        cout << "Word count: " << wordCount << endl;
        cout << "Character count: " << charCount << endl;
        cout << "Line count: " << lineCount << endl;
        cout << "Paragraph count: " << paragraphCount << endl;
    }
};

int main() {
    TextEditor editor;

    while (true) {
        cout << "\nOptions:" << endl;
        cout << "1. Insert text" << endl;
        cout << "2. Delete text" << endl;
        cout << "3. Undo" << endl;
        cout << "4. Display text" << endl;
        cout << "5. Save to file" << endl;
        cout << "6. Load from file" << endl;
        cout << "7. Search and Replace" << endl;
        cout << "8. Print statistics" << endl;
        cout << "9. Exit" << endl;

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                string data;
                cout << "Enter text to insert: ";
                cin.ignore(); // Clear the newline from previous input
                getline(cin, data);
                editor.insert(data);
                break;
            }
            case 2:
                editor.deleteText();
                break;
            case 3:
                editor.undo();
                break;
            case 4:
                editor.display();
                break;
            case 5:
                editor.saveToFile();
                break;
            case 6:
                editor.loadFromFile();
                break;
            case 7: {
                string searchTerm, replaceTerm;
                cout << "Enter search term: ";
                cin.ignore();
                getline(cin, searchTerm);
                cout << "Enter replace term: ";
                getline(cin, replaceTerm);
                editor.searchAndReplace(searchTerm, replaceTerm);
                break;
            }
            case 8:
                editor.printStatistics();
                break;
            case 9:
                return 0;
            default:
                cout << "Invalid choice. Try again." << endl;
        }
    }

    return 0;
}
