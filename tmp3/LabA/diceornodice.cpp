#include <iostream>
#include <vector>

using namespace std;

double roll(int sides, int times, int last) {
    if (times == 1 && last == -1) {
        return 1;
    }
    if (times == 1) {
        if (last == 0 || last == sides - 1) {
            return double(sides - 2) / sides;
        } else {
            return double(sides - 3) / sides;
        }
    }
    if (times > 1) {
        double res = 0;
        for (int i = 0; i < sides; i++) {
            if (i == last - 1 || i == last || i == last + 1) {
                continue;
            }
            res += roll(sides, times - 1, i);
        }
        return res / (double)sides;
    }
    return 68;
}

int main(int argc, char** argv) {
    int sides = stoi(argv[1]);
    int times = stoi(argv[2]);
    int last =
        stoi(argv[3]); // -1 indicated times down, otherwise cur + times

    double res = 0;
    if (last == -1) {
        for (int i = 0; i < sides; i++) {
            res += roll(sides, times - 1, i);
        }
        res *= double(1) / sides;

        printf("%g\n", res);
        return 0;
    }

    printf("%g\n", roll(sides, times, last));
}
