#pragma once

#include <list>
#include <regex>

class Numbers {

public:

    static Numbers find_numbers(const char* str, int len); // find numbers in str, len - length of str

    void operator()(std::csub_match match);
    
    void sort(bool asc); // asc - if true sort ascending else descending
    void print(); // print quantity, sum, max, min of found numbers
    
    int get_max() const;
    int get_min() const;
    int get_sum() const;
    int size() const;
    const std::list<int>& get_numbers() const;

private:

    std::list<int> numbers; // list of found numbers
    int sum = 0; // sum of found numbers
    int max = 0; // max of found numbers
    int min = 9; // min of found numbers
};