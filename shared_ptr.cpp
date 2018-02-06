/**
 * @file    shared_ptr.cpp
 * @author  Jinwook Jung (jinwookjungs@gmail.com)
 * @date    2017-03-06 17:35:21
 *
 * Created on Mon Mar  6 17:35:21 2017.
 */

#include <memory>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Blob
{
    public:
        typedef vector<string>::size_type size_type;
        Blob();
        size_type size() const { return data->size(); }
        bool empty() const { return data->empty(); }

        // Add and remove elements
        void push_back(const string& t) { data->push_back(t); }
        void pop_back();

        string& front();
        string& back();

    private:
        shared_ptr<vector<string>> data;
        void check (size_type i, const string &msg) const;
};

Blob::Blob() : data(make_shared<vector<string>>()) {}
void Blob::check (size_type i, const string& msg) const
{
    if (i >= data->size()) {
        throw out_of_range(msg);
    }
}

string& Blob::front()
{
    check(0, "front on empty StrBlob");
    return data->front();
}

string& Blob::back()
{
    check(0, "back on empty StrBlob");
    return data->back();
}

void Blob::pop_back()
{
    check(0, "pop_back on empty StrBlob");
    return data->pop_back();
}
