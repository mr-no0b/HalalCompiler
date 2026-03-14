#include <stdio.h>

double multiply(int x, int y) {
    return (x * (y * 1.589));
}

int main() {
    int a = 6;
    int b = 7;
    int result = (a * b);
    if ((result > 40)) {
        printf("%s", "Result is greater than 40");
        printf("\n");
    }
    else {
        printf("%s", "Result is not greater than 40");
        printf("\n");
    }
    double ans = (multiply(a,b) + 7.0);
    printf("%g", (double)(ans));
    printf("\n");
    for (int i = 1; (i <= 5); i = (i + 1)) {
        printf("%d", (int)(i));
        printf("\n");
    }
    return 0;
}

