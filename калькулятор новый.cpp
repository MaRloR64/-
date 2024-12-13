#include <iostream>
#include <stack>
#include <cmath>
#include <string>
using namespace std;

// Функция для определения приоритета оператора
int Priority(char ch) {
    if (ch == '+' || ch == '-') return 1;
    if (ch == '*' || ch == '/') return 2;
    if (ch == '^') return 3;
    if (ch == 's' || ch == 'c' || ch == 't' || ch == 'l') return 4; // Функции
    return 0;
}

// Функция для выполнения операций
double ApplyOperation(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (b == 0) {
            cout << "Error: division by zero" << endl;
            exit(1);
        }
        return a / b;
    case '^': return pow(a, b);
    default: return 0;
    }
}

// Функция для выполнения функций (sin, cos, tan, log)
double ApplyFunction(double x, char func) {
    switch (func) {
    case 's': return sin(x);
    case 'c': return cos(x);
    case 't': return tan(x);
    case 'l':
        if (x <= 0) {
            cout << "Error: invalid argument for log" << endl;
            exit(1);
        }
        return log(x);
    default: return 0;
    }
}

// Основная функция для вычисления выражения
double EvaluateExpression(const string& expression) {
    stack<double> values; // Стек для чисел
    stack<char> ops;      // Стек для операторов и функций

    for (size_t i = 0; i < expression.length(); i++) {
        char ch = expression[i];

        // Пропускаем пробелы
        if (ch == ' ') continue;

        // Если текущий символ - число, считываем все число
        if (isdigit(ch) || ch == '.') {
            string numStr;
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                numStr += expression[i++];
            }
            i--; // Возвращаемся на один символ назад
            double num = stod(numStr);
            values.push(num);
        }
        // Если текущий символ - открывающая скобка
        else if (ch == '(') {
            ops.push(ch);
        }
        // Если текущий символ - закрывающая скобка
        else if (ch == ')') {
            while (!ops.empty() && ops.top() != '(') {
                char op = ops.top();
                ops.pop();

                // Если оператор - функция
                if (op == 's' || op == 'c' || op == 't' || op == 'l') {
                    double val = values.top();
                    values.pop();
                    values.push(ApplyFunction(val, op));
                }
                // Если оператор - бинарная операция
                else {
                    double val2 = values.top();
                    values.pop();
                    double val1 = values.top();
                    values.pop();
                    values.push(ApplyOperation(val1, val2, op));
                }
            }
            if (!ops.empty()) ops.pop(); // Удаляем '('
        }
        // Если текущий символ - оператор или функция
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' ||
            ch == 's' || ch == 'c' || ch == 't' || ch == 'l') {
            // Если текущий символ - функция, считываем её аргумент
            if (ch == 's' || ch == 'c' || ch == 't' || ch == 'l') {
                ops.push(ch);
                i++; // Пропускаем '(' после функции
            }
            // Если текущий символ - оператор
            else {
                while (!ops.empty() && Priority(ops.top()) >= Priority(ch)) {
                    char op = ops.top();
                    ops.pop();

                    // Если оператор - функция
                    if (op == 's' || op == 'c' || op == 't' || op == 'l') {
                        double val = values.top();
                        values.pop();
                        values.push(ApplyFunction(val, op));
                    }
                    // Если оператор - бинарная операция
                    else {
                        double val2 = values.top();
                        values.pop();
                        double val1 = values.top();
                        values.pop();
                        values.push(ApplyOperation(val1, val2, op));
                    }
                }
                ops.push(ch);
            }
        }
        // Если ввод некорректен
        else {
            cout << "Error: invalid character '" << ch << "'" << endl;
            exit(1);
        }
    }

    // Выполняем оставшиеся операции
    while (!ops.empty()) {
        char op = ops.top();
        ops.pop();

        // Если оператор - функция
        if (op == 's' || op == 'c' || op == 't' || op == 'l') {
            double val = values.top();
            values.pop();
            values.push(ApplyFunction(val, op));
        }
        // Если оператор - бинарная операция
        else {
            double val2 = values.top();
            values.pop();
            double val1 = values.top();
            values.pop();
            values.push(ApplyOperation(val1, val2, op));
        }
    }

    // Результат - верхний элемент стека значений
    return values.top();
}

int main() {
    string expression;
    cout << "Write Expression: ";
    getline(cin, expression);

    double result = EvaluateExpression(expression);
    cout << "Answer = " << result << endl;

    return 0;
}