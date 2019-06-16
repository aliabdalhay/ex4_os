
#include<stdio.h>

#include<string>
#include<iostream>

void mysubstr(std::string str, int low, int high)
{
    std::string new_str =  "";
    while(low<=high)
    {
        new_str = new_str + str[low];
        low += 1;
    }

    std::cout<<new_str;
}

int main ()
{
    std::string str = "geeksforgeeks";
    mysubstr(str, 1, 3);
    return 0;
}