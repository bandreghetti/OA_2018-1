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
    public:
        unsigned int order;
        int pageRRN;
        int *children;
        char **keys;
        int *regRRN;
        
        BTreeNode(unsigned int m)
        {
            order = m;
            pageRRN = -1;
            children = new int[order];
            int i, j;
            for(i = 0; i < m; ++i)
            {
                children[i] = -1;
            }
            keys = new char*[order-1];
            regRRN = new int[order-1];
            for(i = 0; i < m-1; ++i)
            {
                keys[i] = new char[9];
                for(j = 0; j < 9; ++j)
                {
                    keys[i][j] = '\0';
                }
                regRRN[i] = -1;
            }
        }

        void print()
        {
            printf("|pageRRN: %d", pageRRN);
            int i;
            for(i = 0; i < order; ++i)
            {
                if(i != 0)
                {
                    printf("%9s|%d|", keys[i - 1], children[i]);
                } else {
                    printf("|%d|", children[i]);
                }
            }
        }
};

class BTree
{
        unsigned int order;
        fstream file;
        BTreeNode *root;
        BTreeNode *page;
    public:
        BTree(unsigned int m)
        {
            order = m;
            *root = BTreeNode(order);
            root->pageRRN = 0;
        }
        BTree(string fname)
        {
            file.open(fname);
            file.read((char*) &order, sizeof(unsigned int));
            *root = BTreeNode(order);
            root->pageRRN = 0;
            file.read((char*) root->children, order*sizeof(int));
            int i;
            for(i = 0; i < order - 1; ++i)
            {
                file.read((char*) root->keys[i], 9*sizeof(char));
            }
            file.read((char*) root->regRRN, order*sizeof(int));
        }
        unsigned int get_order()
        {
            return order;
        }
        void show()
        {
            root->print();
        }
};