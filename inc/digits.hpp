#pragma once

#include <list>

class Digits {

public:

    static Digits find_digits(char* str, int len);

    void operator()(char c);
    
    void sort(bool asc);
    void print();
    
    int get_max() const;
    int get_min() const;
    int get_sum() const;
    const std::list<int>& get_digits() const;

private:

    std::list<int> digits;
    int sum = 0;
    int max = 0;
    int min = 9;
};