/**
 * @file    test2.cpp
 * @author  Jinwook Jung (jinwookjungs@gmail.com)
 * @date    2017-03-06 22:22:25
 *
 * Created on Mon Mar  6 22:22:25 2017.
 */

#include <vector>
#include <iostream>

using namespace std;

int main(void)
{
    vector<int> int_list;
    int_list.emplace_back(0);
    int_list.emplace_back(1);
    int_list.emplace_back(2);

    cout << int_list[-1] << endl;
    cout << int_list[0] << endl;
    cout << int_list[1] << endl;
    cout << int_list[2] << endl;
}
