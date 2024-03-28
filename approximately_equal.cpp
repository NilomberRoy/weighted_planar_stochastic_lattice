#include <iostream>
#include <cmath>

bool approximatelyEqual(double a, double b, double epsilon = 1e-9) {
    // Compare the absolute difference to a fraction of the larger number
    double absDiff = std::abs(a - b);
    double maxVal = std::max(std::abs(a), std::abs(b));

    return absDiff <= epsilon * maxVal;
}

int main() {
    // Example 1: Comparing numbers with similar magnitudes
    double num1 = 0.12345679;
    double num2 = 0.12345679;

    if (approximatelyEqual(num1, num2)) {
        std::cout << "Numbers are approximately equal." << std::endl;
    } else {
        std::cout << "Numbers are not approximately equal." << std::endl;
    }

    // Example 2: Comparing numbers with vastly different magnitudes
    double largeNum = 1e-30;
    double smallNum = 1e-31;

    if (approximatelyEqual(largeNum, smallNum)) {
        std::cout << "Numbers are approximately equal." << std::endl;
    } else {
        std::cout << "Numbers are not approximately equal." << std::endl;
    }

    return 0;
}

