#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>

using namespace std;
using namespace boost::algorithm;


class Student {
        string name;
        string id;
        string course;
        char classroom;
    public:
        string primkey();
};

string Student::primkey()
{
    string uppername = to_upper_copy(name);
    erase_all(uppername, " ");
    return id + uppername;
}
