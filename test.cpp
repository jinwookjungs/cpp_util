/**
 * @file    test.cpp
 * @author  Jinwook Jung (jinwookjungs@gmail.com)
 * @date    2017-03-06 16:32:48
 *
 * Created on Mon Mar  6 16:32:48 2017.
 */

#include <vector>
#include <iostream>
#include <memory>

using namespace std;

int copy_count;

struct A 
{
    int a_;
    A(void) : a_(0) {}
    A(int a) : a_(a) {
        cout << "CONSTRUCT" << endl;
    };
    A(const A& other) : a_(other.a_) {
        copy_count++;
        cout << "COPY(" << copy_count << "): " << *this << " <- " << other << endl;
    };
    ~A() {
        cout << "DELETE" << endl;
    }
    friend ostream& operator<< (ostream& os, const A& a);
};

ostream& operator<< (ostream& os, const A& a)
{
    os << a.a_;
    return os;
}

void func_a(vector<shared_ptr<A>>& a_list) {
    shared_ptr<A> a = make_shared<A>(100);
    auto b = make_shared<A>(200);
    a_list.push_back(a);
    a_list.push_back(b);
    cout << "TEST" << endl;
}

int main (void)
{
//    // vector<A> a_list(10);
    vector<shared_ptr<A>> a_list;
    func_a(a_list);
//    cout << *a_list[0] << endl;
    // a_list.clear();

    vector<A> b_list;
    b_list.emplace_back(A(10));
    b_list.emplace_back(A(10));
    b_list.emplace_back(A(10));
    b_list.emplace_back(A(10));
    b_list.emplace_back(A(10));
    b_list.emplace_back(A(10));
    b_list.clear();

//    a_list.push_back(make_shared<A>(10));
//    a_list.push_back(make_shared<A>(20));
//    a_list.push_back(make_shared<A>(30));
//    a_list.push_back(make_shared<A>(40));
//    a_list.push_back(make_shared<A>(50));
//    a_list.push_back(make_shared<A>(60));
//    a_list.push_back(make_shared<A>(70));
//    a_list.push_back(make_shared<A>(80));
//    a_list.push_back(make_shared<A>(90));
//    a_list.push_back(make_shared<A>(100));
}
