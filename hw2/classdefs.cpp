#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
using namespace boost::algorithm;

class Student {
    public:
        string name;
        string id;
        string course;
        char classroom;
        Student(string a, string b, string c, char d)
        {
            name = a;
            id = b;
            course = c;
            classroom = d;
        }

        Student(string regstr)
        {
            name = regstr.substr(0, 40);
            id = regstr.substr(41, 5);
            course = regstr.substr(48, 2);
            classroom = regstr.at(52);
        }

        string primkey()
        {
            string uppername = string(name);
            erase_all(uppername, " ");
            uppername = to_upper_copy(uppername.erase(3));
            return uppername + id;
        }
};