#include "numbers.hpp"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <regex>

void Numbers::operator()(std::csub_match match) {
    int number = atoi(match.str().c_str());

    if (number >= 0 && number <= 9) {
        sum += number;

        numbers.push_back(number);

        if (number > max) {
            max = number;
        }

        if (number < min) {
            min = number;
        }
    }
}

Numbers Numbers::find_numbers(const char* str, int len) {
    std::regex number("[+-]?\\d+");
    std::cregex_token_iterator it(str, str + len, number);
    std::cregex_token_iterator it_end;

    return std::for_each(it, it_end, Numbers());
}

void Numbers::sort(bool asc) {
    if (asc) {
        numbers.sort();
    }
    else {
        numbers.sort(std::greater<int>());
    }
}

void Numbers::print() {
    if (size()) {
        std::cout << "Numbers: ";

        std::for_each(
            numbers.begin(), 
            numbers.end(), 
            [](const int& number){std::cout << number << " ";}
        );
        
        std::cout << std::endl 
        << "quantity: " << size() << " sum: " << sum << " max: " << max << " min: " << min 
        << std::endl;
    }
    else {
        std::cout << "Numbers [0-9] not found" << std::endl;
    }
}

int Numbers::get_max() const {
    return max;
}

int Numbers::get_min() const {
    return min;
}

int Numbers::get_sum() const {
    return sum;
}

int Numbers::size() const {
    return numbers.size();
}

const std::list<int>& Numbers::get_numbers() const {
    return numbers;
}