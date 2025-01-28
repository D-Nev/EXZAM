#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

//������� ��� ��/���������� ������
string encryptDecrypt(const string& str, char key = 'K') {
    string result = str;
    for (char& c : result) {
        c ^= key;
    }
    return result;
}

class TestSystem {
public:
    struct User {
        string name;
        string address;
        string houseNum;
        string phoneNum;
        string login;
        string password; //� ������������� ����
        vector<pair<string, int>> testProg; 
    };

    struct Test {
        string category;
        string subcat;
        string question;
        vector<string> answers;
        int correctAnsIdx;
    };

    TestSystem() {
        adminLogin = "admin";
        adminPassword = encryptDecrypt("admin123455678911");
    }

    void mainMenu() {
        int choice;
        do {
            cout << "����� ���������� � ������� ������������!\n";
            cout << "1. ���� ��� �����������\n";
            cout << "2. ���� ��� �������������\n";
            cout << "3. ����������� ������ ������������\n";
            cout << "0. �����\n";
            cout << "�������� �����: ";
            cin >> choice;

            switch (choice) {
            case 1:
                userMenu();
                break;
            case 2:
                adminMenu();
                break;
            case 3:
                registerUser();
                break;
            case 0:
                cout << "����� �� �������...\n";
                break;
            default:
                cout << "�������� �����. ���������� �����.\n";
            }
        } while (choice != 0);
    }

private:
    string adminLogin;
    string adminPassword;
    vector<User> users; // ������ �������������
    vector<Test> tests; // ��� �����
    vector<pair<string, vector<int>>> userResults; // ����� -> ���������� ������

    void registerUser() {
        User newUser;

        cout << "����������� ������ ������������:\n";
        cout << "������� ���: ";
        cin.ignore();
        getline(cin, newUser.name);

        cout << "������� �����: ";
        getline(cin, newUser.address);

        cout << "������� ����� ����: ";
        getline(cin, newUser.houseNum);

        cout << "������� ����� ��������: ";
        getline(cin, newUser.phoneNum);

        cout << "���������� �����: ";
        cin >> newUser.login;

        if (find_if(users.begin(), users.end(), [&newUser](const User& u) { return u.login == newUser.login; }) != users.end()) {
            cout << "������������ � ����� ������� ��� ����������. ���������� ������ �����.\n";
            return;
        }

        string password;
        cout << "���������� ������: ";
        cin >> password;
        newUser.password = encryptDecrypt(password); // ������� ������

        users.push_back(newUser);

        cout << "����������� �������! ������ �� ������ ����� � �������.\n";
    }

    void userMenu() {
        string login, password;
        cout << "������� �����: ";
        cin >> login;
        cout << "������� ������: ";
        cin >> password;

        auto it = find_if(users.begin(), users.end(), [&login](const User& u) { return u.login == login; });
        if (it != users.end() && encryptDecrypt(it->password) == password) {
            cout << "�������� ����. ����� ����������, " << it->name << "!\n";
            userActions(login);
        }
        else {
            cout << "�������� ����� ��� ������.\n";
        }
    }

    void userActions(const string& login) {
        int choice;
        do {
            cout << "1. ������ ����\n";
            cout << "2. ����������� ����������\n";
            cout << "0. �����\n";
            cout << "�������� �����: ";
            cin >> choice;

            switch (choice) {
            case 1:
                takeTest(login);
                break;
            case 2:
                viewResults(login);
                break;
            case 0:
                cout << "����� � ������� ����...\n";
                break;
            default:
                cout << "�������� �����. ���������� �����.\n";
            }
        } while (choice != 0);
    }

    void takeTest(const string& login) {
        if (tests.empty()) {
            cout << "��� ��������� ������.\n";
            return;
        }

        cout << "��������� ���������: \n";
        vector<string> categories;
        for (const auto& test : tests) {
            if (find(categories.begin(), categories.end(), test.category) == categories.end()) {
                categories.push_back(test.category);
            }
        }

        int i = 1;
        for (const auto& category : categories) {
            cout << i++ << ". " << category << "\n";
        }

        cout << "�������� ���������: ";
        int catChoice;
        cin >> catChoice;

        if (catChoice < 1 || catChoice > categories.size()) {
            cout << "�������� �����.\n";
            return;
        }

        string selectedCat = categories[catChoice - 1];
        vector<Test> selectedTests;
        copy_if(tests.begin(), tests.end(), back_inserter(selectedTests), [&selectedCat](const Test& t) { return t.category == selectedCat; });

        i = 1;
        for (const auto& test : selectedTests) {
            cout << i++ << ". " << test.subcat << "\n";
        }

        cout << "�������� ������������: ";
        int subCatChoice;
        cin >> subCatChoice;

        if (subCatChoice < 1 || subCatChoice > selectedTests.size()) {
            cout << "�������� �����.\n";
            return;
        }

        const auto& selectedTest = selectedTests[subCatChoice - 1];
        cout << "������ �����: " << selectedTest.subcat << "\n";

        int correctAns = 0;
        cout << selectedTest.question << "\n";
        for (size_t j = 0; j < selectedTest.answers.size(); ++j) {
            cout << j + 1 << ". " << selectedTest.answers[j] << "\n";
        }

        cout << "��� �����: ";
        int answer;
        cin >> answer;

        if (answer - 1 == selectedTest.correctAnsIdx) {
            ++correctAns;
        }

        int percentage = (correctAns * 100) / 1;

        auto it = find_if(userResults.begin(), userResults.end(), [&login](const pair<string, vector<int>>& p) { return p.first == login; });
        if (it != userResults.end()) {
            it->second.push_back(percentage);
        }
        else {
            userResults.push_back({ login, {percentage} });
        }

        cout << "���������� �����: \n";
        cout << "���������� �������: " << correctAns << "\n";
        cout << "�������: " << percentage << "%\n";
        cout << "������: " << (percentage / 10 + 2) << "\n";
    }

    void viewResults(const string& login) {
        auto it = find_if(userResults.begin(), userResults.end(), [&login](const pair<string, vector<int>>& p) { return p.first == login; });
        if (it == userResults.end() || it->second.empty()) {
            cout << "����������� �� �������.\n";
            return;
        }

        cout << "����������: \n";
        for (size_t i = 0; i < it->second.size(); ++i) {
            cout << i + 1 << ". " << it->second[i] << "%\n";
        }
    }

    void adminMenu() {
        string login, password;
        cout << "������� ����� ��������������: ";
        cin >> login;
        cout << "������� ������: ";
        cin >> password;

        if (login == adminLogin && encryptDecrypt(password) == adminPassword) {
            cout << "�������� ����. ����� ����������, �������������!\n";
            adminActions();
        }
        else {
            cout << "�������� ����� ��� ������ ��������������.\n";
        }
    }

    void adminActions() {
        int choice;
        do {
            cout << "1. ���������� ��������������\n";
            cout << "2. ���������� �������\n";
            cout << "3. �������� ����������\n";
            cout << "0. �����\n";
            cout << "�������� �����: ";
            cin >> choice;

            switch (choice) {
            case 1:
                manageUsers();
                break;
            case 2:
                manageTests();
                break;
            case 3:
                viewStats();
                break;
            case 0:
                cout << "����� � ������� ����..\n";
                break;
            default:
                cout << "�������� �����. ���������� �����.\n";
            }
        } while (choice != 0);
    }

    void manageUsers() {
        cout << "���������� �������������� \n";
    }

    void manageTests() {
        int choice;
        do {
            cout << "1. �������� ����\n";
            cout << "2. ������� ����\n";
            cout << "3. ����������� �����\n";
            cout << "0. �����\n";
            cout << "�������� �����: ";
            cin >> choice;

            switch (choice) {
            case 1:
                addTest();
                break;
            case 2:
                deleteTest();
                break;
            case 3:
                viewTests();
                break;
            case 0:
                cout << "������� � ���� ��������������...\n";
                break;
            default:
                cout << "�������� �����. ���������� �����.\n";
            }
        } while (choice != 0);
    }

    void addTest() {
        Test newTest;

        cout << "���������� ������ �����:\n";
        cout << "������� ���������: ";
        cin.ignore();
        getline(cin, newTest.category);

        cout << "������� ������������: ";
        getline(cin, newTest.subcat);

        cout << "������� ������: ";
        getline(cin, newTest.question);

        int numAnswers;
        cout << "������� ���������� �������: ";
        cin >> numAnswers;
        cin.ignore();

        for (int i = 0; i < numAnswers; ++i) {
            string answer;
            cout << "����� " << i + 1 << ": ";
            getline(cin, answer);
            newTest.answers.push_back(answer);
        }

        cout << "������� ����� ����������� ������ (1-" << numAnswers << "): ";
        cin >> newTest.correctAnsIdx;
        newTest.correctAnsIdx -= 1;

        tests.push_back(newTest);
        cout << "���� ������� ��������.\n";
    }

    void deleteTest() {
        if (tests.empty()) {
            cout << "��� ��������� ������ ��� ��������.\n";
            return;
        }

        cout << "������ ������:\n";
        for (size_t i = 0; i < tests.size(); ++i) {
            cout << i + 1 << ". " << tests[i].category << " - " << tests[i].subcat << "\n";
        }

        cout << "������� ����� ����� ��� ��������: ";
        int index;
        cin >> index;

        if (index < 1 || index > tests.size()) {
            cout << "�������� �����.\n";
            return;
        }

        tests.erase(tests.begin() + index - 1);
        cout << "���� ������� ������.\n";
    }

    void viewTests() {
        if (tests.empty()) {
            cout << "��� ��������� ������.\n";
            return;
        }

        cout << "������ ������:\n";
        for (const auto& test : tests) {
            cout << "���������: " << test.category << ", ������������: " << test.subcat << ", ������: " << test.question << "\n";
        }
    }

    void viewStats() {
        cout << "����������\n";
    }
};

int main() {
    setlocale(LC_ALL, "");
    TestSystem system;
    system.mainMenu();
    return 0;
}
