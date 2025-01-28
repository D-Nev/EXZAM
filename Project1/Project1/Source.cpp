#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

//Функция для де/шифрования пароля
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
        string password; //В зашифрованном виде
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
            cout << "Добро пожаловать в систему тестирования!\n";
            cout << "1. Вход как тестируемый\n";
            cout << "2. Вход как администратор\n";
            cout << "3. Регистрация нового пользователя\n";
            cout << "0. Выход\n";
            cout << "Выберите опцию: ";
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
                cout << "Выход из системы...\n";
                break;
            default:
                cout << "Неверный выбор. Попробуйте снова.\n";
            }
        } while (choice != 0);
    }

private:
    string adminLogin;
    string adminPassword;
    vector<User> users; // список пользователей
    vector<Test> tests; // все тесты
    vector<pair<string, vector<int>>> userResults; // логин -> результаты тестов

    void registerUser() {
        User newUser;

        cout << "Регистрация нового пользователя:\n";
        cout << "Введите имя: ";
        cin.ignore();
        getline(cin, newUser.name);

        cout << "Введите адрес: ";
        getline(cin, newUser.address);

        cout << "Введите номер дома: ";
        getline(cin, newUser.houseNum);

        cout << "Введите номер телефона: ";
        getline(cin, newUser.phoneNum);

        cout << "Придумайте логин: ";
        cin >> newUser.login;

        if (find_if(users.begin(), users.end(), [&newUser](const User& u) { return u.login == newUser.login; }) != users.end()) {
            cout << "Пользователь с таким логином уже существует. Попробуйте другой логин.\n";
            return;
        }

        string password;
        cout << "Придумайте пароль: ";
        cin >> password;
        newUser.password = encryptDecrypt(password); // Шифруем пароль

        users.push_back(newUser);

        cout << "Регистрация успешна! Теперь вы можете войти в систему.\n";
    }

    void userMenu() {
        string login, password;
        cout << "Введите логин: ";
        cin >> login;
        cout << "Введите пароль: ";
        cin >> password;

        auto it = find_if(users.begin(), users.end(), [&login](const User& u) { return u.login == login; });
        if (it != users.end() && encryptDecrypt(it->password) == password) {
            cout << "Успешный вход. Добро пожаловать, " << it->name << "!\n";
            userActions(login);
        }
        else {
            cout << "Неверный логин или пароль.\n";
        }
    }

    void userActions(const string& login) {
        int choice;
        do {
            cout << "1. Пройти тест\n";
            cout << "2. Просмотреть результаты\n";
            cout << "0. Выйти\n";
            cout << "Выберите опцию: ";
            cin >> choice;

            switch (choice) {
            case 1:
                takeTest(login);
                break;
            case 2:
                viewResults(login);
                break;
            case 0:
                cout << "Выход в главное меню...\n";
                break;
            default:
                cout << "Неверный выбор. Попробуйте снова.\n";
            }
        } while (choice != 0);
    }

    void takeTest(const string& login) {
        if (tests.empty()) {
            cout << "Нет доступных тестов.\n";
            return;
        }

        cout << "Доступные категории: \n";
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

        cout << "Выберите категорию: ";
        int catChoice;
        cin >> catChoice;

        if (catChoice < 1 || catChoice > categories.size()) {
            cout << "Неверный выбор.\n";
            return;
        }

        string selectedCat = categories[catChoice - 1];
        vector<Test> selectedTests;
        copy_if(tests.begin(), tests.end(), back_inserter(selectedTests), [&selectedCat](const Test& t) { return t.category == selectedCat; });

        i = 1;
        for (const auto& test : selectedTests) {
            cout << i++ << ". " << test.subcat << "\n";
        }

        cout << "Выберите подкатегорию: ";
        int subCatChoice;
        cin >> subCatChoice;

        if (subCatChoice < 1 || subCatChoice > selectedTests.size()) {
            cout << "Неверный выбор.\n";
            return;
        }

        const auto& selectedTest = selectedTests[subCatChoice - 1];
        cout << "Начало теста: " << selectedTest.subcat << "\n";

        int correctAns = 0;
        cout << selectedTest.question << "\n";
        for (size_t j = 0; j < selectedTest.answers.size(); ++j) {
            cout << j + 1 << ". " << selectedTest.answers[j] << "\n";
        }

        cout << "Ваш ответ: ";
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

        cout << "Результаты теста: \n";
        cout << "Правильных ответов: " << correctAns << "\n";
        cout << "Процент: " << percentage << "%\n";
        cout << "Оценка: " << (percentage / 10 + 2) << "\n";
    }

    void viewResults(const string& login) {
        auto it = find_if(userResults.begin(), userResults.end(), [&login](const pair<string, vector<int>>& p) { return p.first == login; });
        if (it == userResults.end() || it->second.empty()) {
            cout << "Результатов не найдено.\n";
            return;
        }

        cout << "Результаты: \n";
        for (size_t i = 0; i < it->second.size(); ++i) {
            cout << i + 1 << ". " << it->second[i] << "%\n";
        }
    }

    void adminMenu() {
        string login, password;
        cout << "Введите логин администратора: ";
        cin >> login;
        cout << "Введите пароль: ";
        cin >> password;

        if (login == adminLogin && encryptDecrypt(password) == adminPassword) {
            cout << "Успешный вход. Добро пожаловать, администратор!\n";
            adminActions();
        }
        else {
            cout << "Неверный логин или пароль администратора.\n";
        }
    }

    void adminActions() {
        int choice;
        do {
            cout << "1. Управление пользователями\n";
            cout << "2. Управление тестами\n";
            cout << "3. Просмотр статистики\n";
            cout << "0. Выйти\n";
            cout << "Выберите опцию: ";
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
                cout << "Выход в главное меню..\n";
                break;
            default:
                cout << "Неверный выбор. Попробуйте снова.\n";
            }
        } while (choice != 0);
    }

    void manageUsers() {
        cout << "Управление пользователями \n";
    }

    void manageTests() {
        int choice;
        do {
            cout << "1. Добавить тест\n";
            cout << "2. Удалить тест\n";
            cout << "3. Просмотреть тесты\n";
            cout << "0. Назад\n";
            cout << "Выберите опцию: ";
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
                cout << "Возврат в меню администратора...\n";
                break;
            default:
                cout << "Неверный выбор. Попробуйте снова.\n";
            }
        } while (choice != 0);
    }

    void addTest() {
        Test newTest;

        cout << "Добавление нового теста:\n";
        cout << "Введите категорию: ";
        cin.ignore();
        getline(cin, newTest.category);

        cout << "Введите подкатегорию: ";
        getline(cin, newTest.subcat);

        cout << "Введите вопрос: ";
        getline(cin, newTest.question);

        int numAnswers;
        cout << "Введите количество ответов: ";
        cin >> numAnswers;
        cin.ignore();

        for (int i = 0; i < numAnswers; ++i) {
            string answer;
            cout << "Ответ " << i + 1 << ": ";
            getline(cin, answer);
            newTest.answers.push_back(answer);
        }

        cout << "Введите номер правильного ответа (1-" << numAnswers << "): ";
        cin >> newTest.correctAnsIdx;
        newTest.correctAnsIdx -= 1;

        tests.push_back(newTest);
        cout << "Тест успешно добавлен.\n";
    }

    void deleteTest() {
        if (tests.empty()) {
            cout << "Нет доступных тестов для удаления.\n";
            return;
        }

        cout << "Список тестов:\n";
        for (size_t i = 0; i < tests.size(); ++i) {
            cout << i + 1 << ". " << tests[i].category << " - " << tests[i].subcat << "\n";
        }

        cout << "Введите номер теста для удаления: ";
        int index;
        cin >> index;

        if (index < 1 || index > tests.size()) {
            cout << "Неверный номер.\n";
            return;
        }

        tests.erase(tests.begin() + index - 1);
        cout << "Тест успешно удален.\n";
    }

    void viewTests() {
        if (tests.empty()) {
            cout << "Нет доступных тестов.\n";
            return;
        }

        cout << "Список тестов:\n";
        for (const auto& test : tests) {
            cout << "Категория: " << test.category << ", Подкатегория: " << test.subcat << ", Вопрос: " << test.question << "\n";
        }
    }

    void viewStats() {
        cout << "Статистика\n";
    }
};

int main() {
    setlocale(LC_ALL, "");
    TestSystem system;
    system.mainMenu();
    return 0;
}
