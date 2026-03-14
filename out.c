#include <stdio.h>

int main() {
    int b = 7;
    int c = 5;
    int a = (b + c);
    if ((a > 10)) {
        printf("%g\n", (double)(a));
    }
    else {
        printf("%g\n", (double)(c));
    }
    return 0;
}

