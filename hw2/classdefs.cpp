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

        void print()
        {
            // printf("Primary Key: %s\n", primkey().c_str());
            printf("Name: %s\n", name.c_str());
            printf("ID: %s\n", id.c_str());
            printf("Course: %s\n", course.c_str());
            printf("Classroom: %c\n", classroom);
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

        string getline(int RRN)
        {
            string line;
            srcfile.seekg(RRN*55);
            if(srcfile.is_open())
            {
                std::getline(srcfile, line);
            } else {
                cout << "ERROR: File not open" << endl;
            }
            // cout << line << endl;
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
        bool isleaf;
        int pageRRN;
        int *children;
        char **keys;
        int *regRRN;
        unsigned int n_keys;
        

        BTreeNode(unsigned int m, bool leaf)
        {
            order = m;
            isleaf = leaf;
            pageRRN = -1;
            n_keys = 0;
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

        size_t nodesize()
        {
            size_t size = 0;
            size += sizeof(unsigned int);
            size += sizeof(bool);
            size += order*sizeof(int);
            size += (order-1)*9*sizeof(char);
            size += (order-1)*sizeof(int);
            size += sizeof(unsigned int);
            return size;
        }

        unsigned int next_pageRRN()
        {
            fstream file;
            file.open("btree.ind");
            file.seekg(0, file.end);
            unsigned int size = file.tellg();
            file.close();
            return size/nodesize();
        }

        BTreeNode(unsigned int tree_order, unsigned int RRN, string fname)
        {
            fstream file;
            int i;
            file.open("btree.ind");
            order = tree_order;
            children = new int[order];
            keys = new char*[order-1];
            regRRN = new int[order-1];
            pageRRN = RRN;
            if(RRN > 10)
            {
                printf("DEBUG: too many nodes\n");
                exit(EXIT_FAILURE);
            }
            // printf("Trying to read node %d with byte address %ld.\n", RRN, RRN*nodesize());
            file.seekg(RRN*nodesize());
            if(!file.eof())
            {
                unsigned int tmp = order;
                file.read((char*) &order, sizeof(unsigned int));
                if(tmp != order)
                {
                    printf("ERROR: tried to read node with wrong order\n");
                    exit(EXIT_FAILURE);
                }
                file.read((char*) &isleaf, sizeof(bool));
                file.read((char*) children, order*sizeof(int));
                tmp = 0;
                for(i = 0; i < order - 1; ++i)
                {
                    keys[i] = new char[9];
                    file.read((char*) keys[i], 9*sizeof(char));
                    if(keys[i][0] != '\0') ++tmp;
                }
                file.read((char*) regRRN, (order-1)*sizeof(int));
                file.read((char*) &n_keys, sizeof(unsigned int));
                if(n_keys != tmp)
                {
                    printf("ERROR: n_keys does not match valid keys");
                    exit(EXIT_FAILURE);
                }
            }
            // printf("Order: %d\n", order);
            // printf("IsLeaf: %d\n", isleaf);
            // for(i = 0; i < order; ++i) printf("Child %d: %d\n", i, children[i]);
            // for(i = 0; i < order-1; ++i) printf("Key %d: %s\n", i, keys[i]);
            // for(i = 0; i < order-1; ++i) printf("regRRN %d: %d\n", i, regRRN[i]);
            // printf("n_keys: %d\n", n_keys);            
            
            // printf("end of constructor\n");
        }

        void write_page()
        {
            fstream file;
            file.open("btree.ind");
            file.seekg(pageRRN*nodesize());
            file.write((char*)&order, sizeof(unsigned int));
            file.write((char*)&isleaf, sizeof(bool));
            file.write((char*)children, order*sizeof(int));
            int i;
            for(i = 0; i < order - 1; ++i)
            {
                // printf("Writing key %s to page %d.\n", keys[i], pageRRN);
                file.write((char*)keys[i], 9*sizeof(char));
            }
            file.write((char*)regRRN, (order-1)*sizeof(int));
            file.write((char*)&n_keys, sizeof(unsigned int));
            file.close();
            // printf("- Node written:\n");
            // printf("Byte Address: %ld\n", pageRRN*nodesize());
            // printf("Order: %d\n", order);
            // printf("IsLeaf: %d\n", isleaf);
            // for(i = 0; i < order; ++i)
            // {
            //     printf("Child %d: %d\n", i, children[i]);
            // }
            // for(i = 0; i < order - 1; ++i)
            // {
            //     printf("Key %d: %s\n", i, keys[i]);
            //     printf("regRRN %d: %d\n", i, regRRN[i]);
            // }
            // printf("Number of keys: %d\n", n_keys);
        }

        void insert_key(string key, int regRRN)
        {
            if(n_keys == order-1)
            {
                printf("ERROR: tried to insert key in a full root page");
                exit(EXIT_FAILURE);
            } else {
                int i = n_keys - 1;
                if(isleaf)
                {
                    while(i >= 0 && strcmp(keys[i], key.c_str()) > 0)
                    {
                        strcpy(keys[i+1], keys[i]);
                        this->regRRN[i+1] = this->regRRN[i];
                        --i;
                    }
                    strcpy(keys[i+1], key.c_str());
                    this->regRRN[i+1] = regRRN;
                    ++n_keys;
                } else {
                    while(i >= 0 && strcmp(keys[i], key.c_str()) > 0) --i;

                    BTreeNode *child = new BTreeNode(order, children[i+1], "btree.ind");

                    if(child->n_keys == order-1)
                    {
                        split(i+1, children[i+1]);
                        reload();
                        if (strcmp(keys[i+1], key.c_str()) < 0)
                        {
                            ++i;
                        }
                    }
                    child = new BTreeNode(order, children[i+1], "btree.ind");
                    child->insert_key(key, regRRN);
                }
                write_page();
            }
        }

        void split(int childidx, int childRRN)
        {
            BTreeNode *y = new BTreeNode(order, childRRN, "btree.ind");
            BTreeNode *z = new BTreeNode(y->order, y->isleaf);
            z->n_keys = order/2;

            int j;
            for (j = 0; j < order/2; ++j)
            {
                strcpy(z->keys[j], y->keys[j+order/2]);
                z->regRRN[j] = y->regRRN[j+order/2];
                strcpy(y->keys[j+order/2], "\0");
                y->regRRN[j+order/2] = -1;
            }

            if(!(y->isleaf))
            {
                for(int j = 0; j < order/2; ++j)
                {
                    z->children[j] = y->children[j+order/2];
                }
            }

            y->n_keys = order/2-1;
            
            for (j = n_keys; j >= childidx+1; --j)
            {
                children[j+1] = children[j];
            }
                 
            z->pageRRN = next_pageRRN();
            children[childidx+1] = z->pageRRN;

            for(j = n_keys-1; j >= childidx; --j)
            {
                strcpy(keys[j+1], keys[j]);
                regRRN[j+1] = regRRN[j];
            }

            strcpy(keys[childidx], y->keys[order/2-1]);
            regRRN[childidx] = y->regRRN[order/2-1];
            strcpy(y->keys[order/2-1], "\0");
            y->regRRN[order/2-1] = -1;

            ++n_keys;

            // print();
            // cout << endl;
            // y->print();
            // printf(" ");
            // z->print();
            // cout << endl;

            write_page();
            y->write_page();
            z->write_page();
        }

        void reload()
        {
            fstream file;
            int i;
            file.open("btree.ind");
            file.seekg(pageRRN*nodesize());
            if(!file.eof())
            {
                unsigned int tmp = order;
                file.read((char*) &order, sizeof(unsigned int));
                if(tmp != order)
                {
                    printf("ERROR: tried to read node with wrong order\n");
                    exit(EXIT_FAILURE);
                }
                file.read((char*) &isleaf, sizeof(bool));
                file.read((char*) children, order*sizeof(int));
                
                for(i = 0; i < order - 1; ++i)
                {
                    keys[i] = new char[9];
                    file.read((char*) keys[i], 9*sizeof(char));
                }
                file.read((char*) regRRN, (order-1)*sizeof(int));
                file.read((char*) &n_keys, sizeof(unsigned int));
            }
            file.close();
        }

        void print()
        {
            char ptype;
            if(isleaf)
            {
                ptype = 'l';
            } else {
                ptype = 'p';
            }
            // printf("|%c%d(%d)", ptype, pageRRN, n_keys);
            printf("|%c%d", ptype, pageRRN);
            int i;
            for(i = 0; i < order; ++i)
            {
                if(i != 0)
                {
                    // printf("%8s@(% d)|% d|", keys[i - 1], regRRN[i - 1], children[i]);
                    printf("%8s|% d|", keys[i - 1], children[i]);
                } else {
                    printf("|% d|", children[i]);
                }
            }
        }

        int search(string key)
        {
            int i = 0;
            // printf("Looking for key %s in page %d\n", key.c_str(), pageRRN);
            while(i < n_keys) 
            {
                if(strcmp(key.c_str(), keys[i]) <= 0) break;
                ++i;
            }
            if(i < n_keys && strcmp(key.c_str(), keys[i]) == 0)
            {
                return regRRN[i];
            } else {
                // printf("Key not found in page %d.\n", pageRRN);
                if(!isleaf)
                {
                    // printf("Accessing page %d.\n", children[i]);
                    BTreeNode *page = new BTreeNode(order, children[i], "btree.ind");
                    return page->search(key);
                } else {
                    return -1;
                }
            }
        }
};

class BTree
{
        unsigned int order;
        fstream file;
        BTreeNode *root;
    public:
        BTree(unsigned int m)
        {
            order = m;
            root = NULL;
            file.open("btree.ind", fstream::out);
        }
        BTree()
        {
            file.open("btree.ind");
            if(!file.eof())
            {
                file.read((char*) &order, sizeof(unsigned int));
                file.seekg(0, file.beg);
                root = new BTreeNode(order, 0, "btree.ind");
            }
        }
        unsigned int get_order()
        {
            return order;
        }
        void show()
        {
            unsigned int total_keys = 0;
            if(root != NULL)
            {
                total_keys += root->n_keys;
                root->print();
                cout << endl;
                int i;
                for(i = 0; i < order; ++i)
                {
                    if(root->children[i] != -1)
                    {
                        BTreeNode *page = new BTreeNode(order, root->children[i], "btree.ind");
                        total_keys += page->n_keys;
                        page->print();
                        cout << endl;
                    }
                }
                cout << endl;
            }
            printf("Total number of keys: %d\n", total_keys);
        }
        void insert(string key, int regRRN)
        {
            if (root == NULL)
            {
                root = new BTreeNode(order, true);
                root->pageRRN = 0;
                strcpy(root->keys[0], key.c_str());
                root->regRRN[0] = regRRN;
                root->n_keys = 1;
                root->write_page();
            } else {
                if (root->n_keys == order-1)
                {
                    BTreeNode *newroot = new BTreeNode(order, false);
                    newroot->pageRRN = 0;
                    root->pageRRN = 1;
                    root->isleaf = true;
                    root->write_page();

                    newroot->children[0] = root->pageRRN;
        
                    newroot->split(0, newroot->children[0]);
                    newroot->reload();

                    int i = 0;
                    if(strcmp(newroot->keys[0], key.c_str()) < 0)
                    {
                        ++i;
                    }
                    BTreeNode *node = new BTreeNode(order, newroot->children[i], "btree.ind");
                    node->insert_key(key, regRRN);

                    root = newroot;
                    newroot->write_page();
                } else {
                    root->insert_key(key, regRRN);
                }
            }
        }
        int search(string key)
        {
            int ret;
            if (root != NULL)
            {
                ret = root->search(key);
            } else {
                ret = -1;
            }
            return ret;
        }
};
