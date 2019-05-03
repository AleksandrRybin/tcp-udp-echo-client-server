#include "digits.hpp"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>

void Digits::operator()(char c) {
    if (isdigit(c)) {
        int digit = atoi(&c);

        sum += digit;

        digits.push_back(digit);

        if (digit > max) {
            max = digit;
        }

        if (digit < min) {
            min = digit;
        }
    }
}

Digits Digits::find_digits(char* str, int len) {
    return std::for_each(str, str + len, Digits());
}

void Digits::sort(bool asc) {
    if (asc) {
        digits.sort();
    }
    else {
        digits.sort(std::greater<int>());
    }
}

void Digits::print() {
    std::for_each(digits.begin(), digits.end(), [](const int& digit){std::cout << digit << " ";});
    std::cout << "sum: " << sum << " max: " << max << " min: " << min << std::endl;
}

int Digits::get_max() const {
    return max;
}

int Digits::get_min() const {
    return min;
}

int Digits::get_sum() const {
    return sum;
}

const std::list<int>& Digits::get_digits() const {
    return digits;
}