#pragma once

#include <list>
#include <regex>

class Numbers {

public:

    static Numbers find_numbers(const char* str, int len);

    void operator()(std::csub_match match);
    
    void sort(bool asc);
    void print();
    
    int get_max() const;
    int get_min() const;
    int get_sum() const;
    int size() const;
    const std::list<int>& get_numbers() const;

private:

    std::list<int> numbers;
    int sum = 0;
    int max = 0;
    int min = 9;
};