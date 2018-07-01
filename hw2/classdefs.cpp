#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
using namespace boost::algorithm;

class Student
{
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

class SourceFile
{
        ifstream srcfile;
    public:
        void open()
        {
            if(srcfile.is_open())
            {
                cout << "ERROR: file already open";
            } else {
                srcfile.open("lista.txt", ifstream::in);
            }
        }

        string getline()
        {
            string line;
            if(srcfile.is_open())
            {
                std::getline(srcfile, line);
            } else {
                cout << "ERROR: File not open" << endl;
            }
            return line;
        }

        bool eof()
        {
            return srcfile.eof();
        }

        void close()
        {
            srcfile.close();
        }
};

class BTreeNode
{
        unsigned int order;
        BTreeNode* children;
        string* key;
        unsigned int* RRN;
    public:
        BTreeNode()
        {
            order = 5;
            children = (BTreeNode*)malloc(order*sizeof(BTreeNode));
            key = (string*)malloc((order-1)*sizeof(string));
            RRN = (unsigned int*)malloc((order-1)*sizeof(unsigned int));
        }

        BTreeNode(unsigned int m)
        {
            order = m;
            children = (BTreeNode*)malloc(order*sizeof(BTreeNode));
            key = (string*)malloc((order-1)*sizeof(string));
            RRN = (unsigned int*)malloc((order-1)*sizeof(unsigned int));
        }

        void insert(unsigned int pos, string key, unsigned int RRN)
        {
            
        }
};

class BTree
{
        unsigned int order;
        BTreeNode *root;
    public:
        BTree()
        {
            order = 5;
            *root = BTreeNode(order);
        }
        
        BTree(unsigned int m)
        {
            order = m;
            *root = BTreeNode(order);
        }
};