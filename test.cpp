#include <iostream>
#include <string>
using namespace std;

int readint() {
    int x;
    cin >> x;
    return x;
}
float readfloat() {
    float x;
    cin >> x;
    return x;
}

int readchar() {
    int c = cin.get();
    return c;
}

string readstring() {
    string s;
    cin >> s;
    return s;
}

string readline() {
    string line;
    getline(cin, line);
    return line;
}

void printint(int i) {
    cout << i;
}

void printfloat(float f) {
    cout << f;
}

void printstr(const std::string& s) {
    cout << s;
}

void printline(const std::string& s) {
    cout << s << endl;
}

int main() {

int _program_i;
float _v0;
float _program_j;
struct _program_Exemplo {
	int x;
};
_program_Exemplo _v1;
int _v2;
int _v3;
int _v4;
int _program_ex;
int _main_i;

_v0 = 5.5 + 1;
_program_j = _v0;
_v1 = _program_Exemplo{};
_v2 = _v1.x + 5;
_v3 = -7;
_v4 = _v2 + _v3;
_program_ex = _v4;
_main_i = readint();
printint(_main_i);

return 0;
}
