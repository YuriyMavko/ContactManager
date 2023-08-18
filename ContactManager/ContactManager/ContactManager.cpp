#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
using namespace std;

class Contact {
private:
    string emailAddress, address, note;
    int phoneNumber;

public:
    string firstName, lastName;

    void addContact();
    void writeContactToFile();
    void viewContact(const vector<string>& nameList);
    void delContact(vector<string>& nameList);
    void findContactInformation();
    void loadContactsFromFile(vector<string>& nameList);
    void saveContactsToFile(const vector<string>& nameList);
};

void Contact::addContact() {
    int phoneNumberDigits;

    cout << "Enter the name of the contact" << endl;
    getline(cin, firstName);

    cout << "Enter the contact's last name" << endl;
    getline(cin, lastName);

    // Validate and input phone number
    while (true) {
        phoneNumberDigits = 0;
        cout << "Enter the phone number after +380: ";
        cin >> phoneNumber;
        cin.ignore();
        cin.clear();
        int tempPhoneNumber = phoneNumber;

        // Check the number of digits in the number
        while (tempPhoneNumber != 0) {
            phoneNumberDigits++;
            tempPhoneNumber /= 10;
        }

        if (phoneNumberDigits != 9) {
            cout << "Input error, the number of digits must be 9" << endl;
        }
        else {
            break;
        }
    }

    cout << "Enter the contact's e-mail address" << endl;
    getline(cin, emailAddress);

    cout << "Enter the contact's address" << endl;
    getline(cin, address);

    cout << "Enter notes about the contact" << endl;
    getline(cin, note);

}

void Contact::writeContactToFile() {
    ofstream fout;
    fout.open(firstName + "_" + lastName + ".txt");
    fout << firstName << " " << lastName << endl;
    fout << "380" << phoneNumber << endl;
    fout << emailAddress << endl;
    fout << note << endl;
    fout << endl;
    fout.close();
}

void Contact::viewContact(const vector<string>& nameList) {
    for (const string& fullName : nameList) {
        string name, surname;
        stringstream ss(fullName);
        ss >> name >> surname;

        ifstream fin(name + "_" + surname + ".txt");
        if (fin.is_open()) {
            string line;

            while (getline(fin, line)) {
                cout << line << endl;
            }

            fin.close();
        }
        else {
            cout << "There is no contact with this name and surname." << endl;
        }
    }
}

void Contact::delContact(vector<string>& nameList) {
    cout << "Enter the first name and last name of the contact you want to delete" << endl;
    string name, surname, fullName;
    getline(cin, fullName);
    stringstream ss(fullName);
    ss >> name >> surname;


    string filename = name + "_" + surname + ".txt";
    if (remove(filename.c_str()) != 0) {
        perror("Error deleting file");
    }
    else {
        cout << "File deleted successfully" << endl;
        nameList.erase(remove(nameList.begin(), nameList.end(), (name + " " + surname)), nameList.end());
        saveContactsToFile(nameList); // Save the updated list
    }
}

void Contact::findContactInformation() {
    string fname, lname, name;
    cout << "Please enter the first name and last name" << endl;
    getline(cin, name);
    stringstream ss(name);
    ss >> fname >> lname;
    cout << endl;

    ifstream fin(fname + "_" + lname + ".txt");
    if (fin.is_open()) {
        string line;
        while (getline(fin, line)) {
            cout << line << endl;
        }
        fin.close();
    }
    else {
        cout << "There is no contact with this name and surname." << endl;
    }
}

void Contact::loadContactsFromFile(vector<string>& nameList) {
    ifstream fin("contact_list.txt");
    if (fin.is_open()) {
        nameList.clear();

        string fullName;
        while (getline(fin, fullName)) {
            nameList.push_back(fullName);
        }

        fin.close();
    }
}

void Contact::saveContactsToFile(const vector<string>& nameList) {
    ofstream fout("contact_list.txt");
    if (fout.is_open()) {
        for (const string& fullName : nameList) {
            fout << fullName << endl;
        }

        fout.close();
    }
}

// Lambda function for sorting contact names
auto compareNames = [](const string& a, const string& b) {
    return a < b;
    };

int main() {
    Contact object;
    vector<string> contactList;
    object.loadContactsFromFile(contactList); // Load contacts from file

    int choice = 0;
    while (choice != 5) {
        // Display menu and get user's choice
        cout << "Enter 1 to add a new contact,\n"
            << "2 to delete a contact,\n"
            << "3 to see the list of contacts,\n"
            << "4 to search for a specific contact,\n"
            << "5 to exit.\n";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: {
            object.addContact();
            contactList.push_back(object.firstName + " " + object.lastName); // Add the new contact to the list
            object.writeContactToFile();
            break;
        }

        case 2:
            object.delContact(contactList);
            break;

        case 3: {
            sort(contactList.begin(), contactList.end(), compareNames);
            object.viewContact(contactList);
            break;
        }

        case 4: {
            object.findContactInformation();
            break;
        }

        case 5:
            cout << "The end" << endl;
            break;

        default:
            cout << "Input error" << endl;
        }
        // Save updated contact list
        object.saveContactsToFile(contactList);
    }

    return 0;
}
