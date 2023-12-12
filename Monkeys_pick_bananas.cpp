#include <iostream>
#include <string>
using namespace std;

struct State {
    int monkey; /*-1:Monkey at A;0: Monkey at B;1:Monkey at C;*/
    int box;    /*-1:box at A;0:box at B;1:box at C;*/
    int banana; /*Banana at B,Banana=0*/
    int on;     /*-1: monkey on the box;1: monkey  the box;*/
};
struct State states[100];
string routesave[100];
/*function monkeygoto,it makes the monkey goto the other place*/
void monkeygoto(int b, int i) {
    int a;
    a = b;
    if (a == -1) {
        routesave[i] = "猴子前往 A";
        states[i + 1] = states[i];
        states[i + 1].monkey = -1;
    } else if (a == 0) {
        routesave[i] = "猴子前往 B";
        states[i + 1] = states[i];
        states[i + 1].monkey = 0;
    } else if (a == 1) {
        routesave[i] = "猴子前往 C";
        states[i + 1] = states[i];
        states[i + 1].monkey = 1;
    } else {
        printf("参数错误");
    }
}
void movebox(int a, int i) {
    int B;
    B = a;
    if (B == -1) {
        routesave[i] = "猴子移动箱子到 A";
        states[i + 1] = states[i];
        states[i + 1].monkey = -1;
        states[i + 1].box = -1;
    } else if (B == 0) {
        routesave[i] = "猴子移动箱子到 B";
        states[i + 1] = states[i];
        states[i + 1].monkey = 0;
        states[i + 1].box = 0;
    } else if (B == 1) {
        routesave[i] = "猴子移动箱子到 C";
        states[i + 1] = states[i];
        states[i + 1].monkey = 1;
        states[i + 1].box = 1;
    } else {
        printf("参数错误");
    }
}

void climbonto(int i) {
    routesave[i] = "猴子爬上箱子";
    states[i + 1] = states[i];
    states[i + 1].on = 1;
}
void reach(int i) { routesave[i] = "猴子拿到香蕉"; }
/*output the solution to the problem*/
void showSolution(int i) {
    int c;
    cout << "Result to problem:" << endl;
    for (c = 0; c < i + 1; c++) {
        cout << "Step" << c + 1 << ":" << routesave[c] << endl;
    }
    cout << endl;
}
/*perform next step*/
void nextStep(int i) {
    int c;
    int j;
    if (i >= 100) {
        cout << "步长过长 " << endl;
        return;
    }
    for (c = 0; c < i; c++) {
        if (states[c].monkey == states[i].monkey && states[c].box == states[i].box &&
            states[c].banana == states[i].banana && states[c].on == states[i].on) {
            return;
        }
    }
    if (states[i].on == 1 && states[i].monkey == states[0].banana && states[i].banana == states[0].banana &&
        states[i].box == states[0].banana) {
        showSolution(i);
        cout << "按下任意按键继续 " << endl;
        // getchar();/*to save screen for user,press any key to continue*/
        return;
    }
    j = i + 1;
    if (states[i].box == states[i].monkey && states[i].box == states[i].banana) {
        if (states[i].on == -1) {
            climbonto(i);
            reach(i + 1);
            nextStep(j);

        } else {
            reach(i + 1);
            nextStep(j);
        }
    } else if (states[i].box == states[i].monkey && states[i].box != states[i].banana) {
        if (states[i].on == -1) {
            movebox(states[i].banana, i);
            nextStep(j);
        }
    } else if (states[i].box != states[i].monkey && states[i].box == states[i].banana) {
        monkeygoto(states[i].box, i);
        nextStep(j);

    } else if (states[i].box != states[i].monkey && states[i].box != states[i].banana) {
        monkeygoto(states[i].box, i);
        nextStep(j);
    }
}
int main() {
    cout << "初始位置：" << endl;
    cout << "monkey(-1 or 0 or 1):";
    cin >> states[0].monkey;
    cout << "box(-1 or 0 or 1):";
    cin >> states[0].box;
    cout << "banana(-1 or 0 or 1):";
    cin >> states[0].banana;
    states[0].on = -1;
    nextStep(0);
}
