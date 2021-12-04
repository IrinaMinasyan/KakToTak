#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

struct variables
{
    std ::string type;
    std ::string name;
    void *value;
};
std ::vector<std ::string> keyword = {"cell", "drob", "logic", "strak", "kolco", "esli", "print"};
std ::vector<variables> var;

void translate(std ::ifstream &fin);
void do_decleration(std::vector<std::string> &);
void do_operation(std::vector<std::string> &);
int operation_int(std ::string, std ::string, std ::string);
double operation_double(std ::string, std ::string, std ::string);
bool operation_bool(std ::string, std ::string, std ::string);
std ::string operation_string(std ::string, std ::string, std ::string);
void do_print(std ::vector<std ::string> &);
void do_condition(std ::vector<std ::vector<std ::string>> &);
void translate(std::vector<std::string> &);
bool operation_bool(std::vector<std::string> &);

int main()
{
    std ::string path;
    std ::ifstream fin;
    fin.open("len.txt");
    if (fin.is_open())
    {
        translate(fin);
    }
    else
    {
        std ::cout << "Error" << std ::endl;
    }
       
}

void translate(std ::ifstream &fin)
{
    while (!fin.eof())
    {
        std::string line;
        getline(fin, line);
        std::vector<std::string> tokens;
        std::string tmp;
        for (int i = 0; i <= line.length(); ++i)
        {
            if (line[i] != ' ' && line[i] != '\0')
            {
                tmp += line[i];
            }
            else
            {
                tokens.push_back(tmp);
                tmp = "";
            }
        }
        if (tokens[0] == keyword[0] || tokens[0] == keyword[1] || tokens[0] == keyword[2] || tokens[0] == keyword[3])
        {
            do_decleration(tokens);
        }
        else if (tokens[0] == keyword[5] || tokens[0] == keyword[4])
        {
            std ::string key = tokens[0];
            std ::vector<std ::vector<std ::string>> tox;
            std ::vector<std ::string> payman;

            for (int i = 1; i < tokens.size(); ++i)
            {
                if (tokens[i] == "[")
                {
                    ++i;
                    while (tokens[i] != "]")
                    {
                        payman.push_back(tokens[i]);
                        ++i;
                    }
                }
                break;
            }

            while (tokens[0] != "}")
            {
                tokens.clear();
                getline(fin, line);
                for (int i = 0; i <= line.length(); ++i)
                {
                    if (line[i] != ' ' && line[i] != '\0')
                    {
                        tmp += line[i];
                    }
                    else
                    {
                        tokens.push_back(tmp);
                        tmp = "";
                    }
                }
                tox.push_back(tokens);
            }

            bool ete = operation_bool(payman);
            if (ete != 0)
            {
                if (key == keyword[5])
                {
                    do_condition(tox);
                    tox.clear();
                }
                else if (key == keyword[4])
                {
                    while (ete != 0)
                    {
                        do_condition(tox);
                        ete = operation_bool(payman);
                    }
                }
            }
        }
        else if (tokens[0] == keyword[6])
        {
            do_print(tokens);
        }
        else
        {
            for (int j = 0; j < var.size(); j++)
            {
                if (tokens[0] == var[j].name)
                {
                    do_operation(tokens);
                    continue;
                }
            }
        }
    }
}

void translate(std::vector<std::string> &vec)
{
    if (vec[0] == keyword[0] || vec[0] == keyword[1] || vec[0] == keyword[2] || vec[0] == keyword[3])
    {
        do_decleration(vec);
    }
    else if (vec[0] == keyword[6])
    {
        do_print(vec);
    }
    else
    {
        for (int j = 0; j < var.size(); j++)
        {
            if (vec[0] == var[j].name)
            {
                do_operation(vec);
                continue;
            }
        }
    }
}

void do_decleration(std::vector<std::string> &vec)
{
    variables v1;
    void *ptr = nullptr;
    for (int i = 0; i < vec.size(); ++i)
    {
        v1.type = vec[0];
        v1.name = vec[1];
        if (vec[2] == "=")
        {
            if (vec[0] == keyword[0])
            {
                ptr = static_cast<void *>(new int);
                *(static_cast<int *>(ptr)) = std::stoi(vec[3]);
            }
            else if (vec[0] == keyword[1])
            {
                ptr = static_cast<void *>(new double);
                *(static_cast<double *>(ptr)) = std::stod(vec[3]);
            }
            else if (vec[0] == keyword[3])
            {
                ptr = static_cast<void *>(new std::string);
                *(static_cast<std::string *>(ptr)) = vec[3];
            }
            else if (vec[0] == keyword[2])
            {
                ptr = static_cast<void *>(new bool);
                if (vec[3] == "false" || vec[3] == "0")
                {
                    *(static_cast<bool *>(ptr)) = false;
                }
                else
                {
                    *(static_cast<bool *>(ptr)) = true;
                }
            }
        }
    }
    v1.value = ptr;
    var.push_back(v1);
}

void do_operation(std::vector<std::string> &vec)
{
    if (vec[1] == "=")
    {
        for (int i = 0; i < var.size(); ++i)
        {
            if (vec[i] == ";")
            {
                break;
            }
            if (vec[0] == var[i].name)
            {
                if (var[i].type == keyword[0])
                {
                    if (vec.size() > 3 && vec[3] == "+" || vec[3] == "-" || vec[3] == "*" || vec[3] == "/")
                    {
                        *(static_cast<int *>(var[i].value)) = operation_int(vec[2], vec[3], vec[4]);
                    }

                    else
                    {
                        bool b = true;
                        for (int j = 0; j < var.size(); ++j)
                        {
                            if (var[j].name == vec[2])
                            {
                                *(static_cast<int *>(var[i].value)) = *(static_cast<int *>(var[j].value));
                                b = false;
                                break;
                            }
                        }
                        if (b)
                        {
                            *(static_cast<int *>(var[i].value)) = std ::stoi(vec[2]);
                        }
                    }
                }

                    else if (var[i].type == keyword[1])
                    {
                        if (vec.size() > 3 && vec[3] == "+" || vec[3] == "-" || vec[3] == "*" || vec[3] == "/")
                            *(static_cast<double *>(var[i].value)) = operation_double(vec[2], vec[3], vec[4]);

                        else
                        {
                            bool b = true;
                            for (int j = 0; j < var.size(); ++j)
                            {
                                if (var[j].name == vec[2])
                                {
                                    *(static_cast<double *>(var[i].value)) = *(static_cast<double *>(var[j].value));
                                    b = false;
                                    break;
                                }
                            }
                            if (b)
                            {
                                *(static_cast<double *>(var[i].value)) = std ::stod(vec[2]);
                            }
                        }
                    }

                    else if (var[i].type == keyword[2])
                    {
                        if (vec.size() > 3 && vec[3] == "&&" || vec[3] == "||" || vec[3] == ">" || vec[3] == "<" || vec[3] == "==" || vec[3] == ">=" || vec[3] == "<=" || vec[3] == "!=")
                        {
                            *(static_cast<bool *>(var[i].value)) = operation_bool(vec[2], vec[3], vec[4]);
                        }
                        else
                        {
                            bool b = true;
                            for (int j = 0; j < var.size(); ++j)
                            {
                                if (var[j].name == vec[2])
                                {
                                    *(static_cast<bool*>(var[i].value)) = *(static_cast<bool *>(var[j].value));
                                    b = false;
                                    break;
                                }
                            }
                            if (b)
                            {
                                if(vec[2] == "false" || vec[2] == "0")
                                {
                                *(static_cast<bool*>(var[i].value)) = false;
                                }
                                else
                                {
                                *(static_cast<bool*>(var[i].value)) = true;
                                }
                            }
                        }
                    }

                    else if (var[i].type == keyword[3])
                    {
                        if (vec.size() > 3 && vec[3] == "+")
                            *(static_cast<std ::string *>(var[i].value)) = operation_string(vec[2], vec[3], vec[4]);

                        else 
                        {
                            *(static_cast<std ::string *>(var[i].value)) = vec[2];
                        }
                    }
                }            
                }
        }
    }

    int operation_int(std ::string arg1, std ::string nshan, std ::string arg2)
    {
        int size = var.size();
        int x = 0, y = 0;
        if (nshan == "+")
        {
            for (int i = 0; i < size; ++i)
            {
                for (int j = 0; j < size; ++j)
                {
                    if (var[i].name == arg1 && var[j].name == arg2)
                    {
                        x = *(static_cast<int *>(var[i].value));
                        y = *(static_cast<int *>(var[j].value));
                        return x + y;
                    }
                    else if (var[j].name == arg2)
                    {
                        x = std ::stoi(arg1);
                        y = *(static_cast<int *>(var[j].value));
                        return x + y;
                    }
                    else if (var[i].name == arg1)
                    {
                        x = *(static_cast<int *>(var[i].value));
                        y = std ::stoi(arg2);
                        return x + y;
                    }

                    else
                    {
                        x = std ::stoi(arg1);
                        y = std ::stoi(arg2);
                        return x + y;
                    }
                }
            }
        }

        if (nshan == "-")
        {
            for (int i = 0; i < size; ++i)
            {
                for (int j = 0; j < size; ++j)
                {
                    if (var[i].name == arg1 && var[j].name == arg2)
                    {
                        x = *(static_cast<int *>(var[i].value));
                        y = *(static_cast<int *>(var[j].value));
                        return x - y;
                    }
                    else if (var[j].name == arg2)
                    {
                        x = std ::stoi(arg1);
                        y = *(static_cast<int *>(var[j].value));
                        return x - y;
                    }
                    else if (var[i].name == arg1)
                    {
                        x = *(static_cast<int *>(var[i].value));
                        y = std ::stoi(arg2);
                        return x - y;
                    }
                }
            }
        }

        if (nshan == "*")
        {
            for (int i = 0; i < size; ++i)
            {
                for (int j = 0; j < size; ++j)
                {
                    if (var[i].name == arg1 && var[j].name == arg2)
                    {
                        x = *(static_cast<int *>(var[i].value));
                        y = *(static_cast<int *>(var[j].value));
                        return x * y;
                    }
                    else if (var[j].name == arg2)
                    {
                        x = std ::stoi(arg1);
                        y = *(static_cast<int *>(var[j].value));
                        return x * y;
                    }
                    else if (var[i].name == arg1)
                    {
                        x = *(static_cast<int *>(var[i].value));
                        y = std ::stoi(arg2);
                        return x * y;
                    }
                }
            }
        }

        if (nshan == "/")
        {
            for (int i = 0; i < size; ++i)
            {
                for (int j = 0; j < size; ++j)
                {
                    if (var[i].name == arg1 && var[j].name == arg2)
                    {
                        x = *(static_cast<int *>(var[i].value));
                        y = *(static_cast<int *>(var[j].value));
                        return x / y;
                    }
                    else if (var[j].name == arg2)
                    {
                        x = std ::stoi(arg1);
                        y = *(static_cast<int *>(var[j].value));
                        return x / y;
                    }
                    else if (var[i].name == arg1)
                    {
                        x = *(static_cast<int *>(var[i].value));
                        y = std ::stoi(arg2);
                        return x / y;
                    }
                }
            }
        }
    }
    double operation_double(std ::string arg1, std ::string nshan, std ::string arg2)
    {
        int size = var.size();
        double x = 0, y = 0;
        if (nshan == "+")
        {
            for (int i = 0; i < size; ++i)
            {
                for (int j = 0; j < size; ++j)
                {
                    if (var[i].name == arg1 && var[j].name == arg2)
                    {
                        x = *(static_cast<double *>(var[i].value));
                        y = *(static_cast<double *>(var[j].value));
                        return x + y;
                    }
                    else if (var[j].name == arg2)
                    {
                        x = std ::stod(arg1);
                        y = *(static_cast<double *>(var[j].value));
                        return x + y;
                    }
                    else if (var[i].name == arg1)
                    {
                        x = *(static_cast<double *>(var[i].value));
                        y = std ::stod(arg2);
                        return x + y;
                    }
                }
            }
        }

        if (nshan == "-")
        {
            for (int i = 0; i < size; ++i)
            {
                for (int j = 0; j < size; ++j)
                {
                    if (var[i].name == arg1 && var[j].name == arg2)
                    {
                        x = *(static_cast<double *>(var[i].value));
                        y = *(static_cast<double *>(var[j].value));
                        return x - y;
                    }
                    else if (var[j].name == arg2)
                    {
                        x = std ::stod(arg1);
                        y = *(static_cast<double *>(var[j].value));
                        return x - y;
                    }
                    else if (var[i].name == arg1)
                    {
                        x = *(static_cast<double *>(var[i].value));
                        y = std ::stod(arg2);
                        return x - y;
                    }
                }
            }
        }

        if (nshan == "*")
        {
            for (int i = 0; i < size; ++i)
            {
                for (int j = 0; j < size; ++j)
                {
                    if (var[i].name == arg1 && var[j].name == arg2)
                    {
                        x = *(static_cast<double *>(var[i].value));
                        y = *(static_cast<double *>(var[j].value));
                        return x * y;
                    }
                    else if (var[j].name == arg2)
                    {
                        x = std ::stod(arg1);
                        y = *(static_cast<double *>(var[j].value));
                        return x * y;
                    }
                    else if (var[i].name == arg1)
                    {
                        x = *(static_cast<double *>(var[i].value));
                        y = std ::stod(arg2);
                        return x * y;
                    }
                }
            }
        }

        if (nshan == "/")
        {
            for (int i = 0; i < size; ++i)
            {
                for (int j = 0; j < size; ++j)
                {
                    if (var[i].name == arg1 && var[j].name == arg2)
                    {
                        x = *(static_cast<double *>(var[i].value));
                        y = *(static_cast<double *>(var[j].value));
                        return x / y;
                    }
                    else if (var[j].name == arg2)
                    {
                        x = std ::stod(arg1);
                        y = *(static_cast<double *>(var[j].value));
                        return x / y;
                    }
                    else if (var[i].name == arg1)
                    {
                        x = *(static_cast<double *>(var[i].value));
                        y = std ::stod(arg2);
                        return x / y;
                    }
                }
            }
        }
    }
    bool operation_bool(std ::string arg1, std ::string nshan, std ::string arg2)
    {
        int size = var.size();
        bool x = 0, y = 0;
        int i1 = 0, i2 = 0;
        double d1 = 0, d2 = 0;
        if (nshan == "&&")
        {
            for (int i = 0; i < size; ++i)
            {
                for (int j = 0; j < size; ++j)
                {
                    if (var[i].name == arg1 && var[j].name == arg2)
                    {
                        x = *(static_cast<bool *>(var[i].value));
                        y = *(static_cast<bool *>(var[j].value));
                        return x && y;
                    }
                }
            }
        }

        else if (nshan == "||")
        {
            for (int i = 0; i < size; ++i)
            {
                for (int j = 0; j < size; ++j)
                {
                    if (var[i].name == arg1 && var[j].name == arg2)
                    {
                        x = *(static_cast<bool *>(var[i].value));
                        y = *(static_cast<bool *>(var[j].value));
                        return x || y;
                    }
                }
            }
        }

        else if (nshan == ">")
        {
            for (int i = 0; i < size; ++i)
            {
                for (int j = 0; j < size; ++j)
                {
                    if (var[i].name == arg1 && var[j].name == arg2)
                    {
                        if (var[i].type == keyword[0])
                        {
                            i1 = *(static_cast<int *>(var[i].value));

                            if(var[j].type == keyword[0])
                            {
                                i2 = *(static_cast<int *>(var[j].value));
                                return i1 > i2;
                            }
                            else
                            {
                                d2 = *(static_cast<double *>(var[j].value));
                                return i1 > d2;
                            }
                        }
                        else if (var[i].type == keyword[1])
                        {
                           d1 = *(static_cast<double *>(var[i].value));

                            if(var[j].type == keyword[0])
                            {
                                i2 = *(static_cast<int *>(var[j].value));
                                return d1 > i2;
                            }
                            else
                            {
                                d2 = *(static_cast<double *>(var[j].value));
                                return d1 > d2;
                            } 
                        }
                    }
                }
            }
        }

        else if (nshan == "<")
        {
            for (int i = 0; i < size; ++i)
            {
                for (int j = 0; j < size; ++j)
                {
                    if (var[i].name == arg1 && var[j].name == arg2)
                    {
                        if (var[i].type == keyword[0])
                        {
                            i1 = *(static_cast<int *>(var[i].value));
                            if(var[j].type == keyword[0])
                            {
                                i2 = *(static_cast<int *>(var[j].value));
                                return i1 < i2;
                            }
                            else
                            {
                                d2 = *(static_cast<double *>(var[j].value));
                                return i1 < d2;
                            }
                        }
                        else if (var[i].type == keyword[1])
                        {
                           d1 = *(static_cast<double *>(var[i].value));
                            if(var[j].type == keyword[0])
                            {
                                i2 = *(static_cast<int *>(var[j].value));
                                return d1 < i2;
                            }
                            else
                            {
                                d2 = *(static_cast<double *>(var[j].value));
                                return d1 < d2;
                            } 
                        }
                    }
                }
            }
        }

        else if (nshan == "==")
        {
            for (int i = 0; i < size; ++i)
            {
                for (int j = 0; j < size; ++j)
                {
                    if (var[i].name == arg1 && var[j].name == arg2)
                    {
                        if (var[i].type == keyword[0])
                        {
                            i1 = *(static_cast<int *>(var[i].value));
                            if(var[j].type == keyword[0])
                            {
                                i2 = *(static_cast<int *>(var[j].value));
                                return i1 == i2;
                            }
                            else
                            {
                                d2 = *(static_cast<double *>(var[j].value));
                                return i1 == d2;
                            }
                        }
                        else if (var[i].type == keyword[1])
                        {
                           d1 = *(static_cast<double *>(var[i].value));
                            if(var[j].type == keyword[0])
                            {
                                i2 = *(static_cast<int *>(var[j].value));
                                return d1 == i2;
                            }
                            else
                            {
                                d2 = *(static_cast<double *>(var[j].value));
                                return d1 == d2;
                            } 
                        }
                    }
                }
            }
        }

        else if (nshan == ">=")
        {
            for (int i = 0; i < size; ++i)
            {
                for (int j = 0; j < size; ++j)
                {
                    if (var[i].name == arg1 && var[j].name == arg2)
                    {
                        if (var[i].type == keyword[0])
                        {
                            i1 = *(static_cast<int *>(var[i].value));
                            if(var[j].type == keyword[0])
                            {
                                i2 = *(static_cast<int *>(var[j].value));
                                return i1 >= i2;
                            }
                            else
                            {
                                d2 = *(static_cast<double *>(var[j].value));
                                return i1 >= d2;
                            }
                        }
                        else if (var[i].type == keyword[1])
                        {
                           d1 = *(static_cast<double *>(var[i].value));
                            if(var[j].type == keyword[0])
                            {
                                i2 = *(static_cast<int *>(var[j].value));
                                return d1 >= i2;
                            }
                            else
                            {
                                d2 = *(static_cast<double *>(var[j].value));
                                return d1 >= d2;
                            } 
                        }
                    }
                }
            }
        }

        else if (nshan == "<=")
        {
            for (int i = 0; i < size; ++i)
            {
                for (int j = 0; j < size; ++j)
                {
                    if (var[i].name == arg1 && var[j].name == arg2)
                    {
                        if (var[i].type == keyword[0])
                        {
                            i1 = *(static_cast<int *>(var[i].value));
                            if(var[j].type == keyword[0])
                            {
                                i2 = *(static_cast<int *>(var[j].value));
                                return i1 <= i2;
                            }
                            else
                            {
                                d2 = *(static_cast<double *>(var[j].value));
                                return i1 <= d2;
                            }
                        }
                        else if (var[i].type == keyword[1])
                        {
                           d1 = *(static_cast<double *>(var[i].value));
                            if(var[j].type == keyword[0])
                            {
                                i2 = *(static_cast<int *>(var[j].value));
                                return d1 <= i2;
                            }
                            else
                            {
                                d2 = *(static_cast<double *>(var[j].value));
                                return d1 <= d2;
                            } 
                        }
                    }
                }
            }
        }

        else if (nshan == "!=")
        {
            for (int i = 0; i < size; ++i)
            {
                for (int j = 0; j < size; ++j)
                {
                    if (var[i].name == arg1 && var[j].name == arg2)
                    {
                        if (var[i].type == keyword[0])
                        {
                            i1 = *(static_cast<int *>(var[i].value));
                            if(var[j].type == keyword[0])
                            {
                                i2 = *(static_cast<int *>(var[j].value));
                                return i1 != i2;
                            }
                            else
                            {
                                d2 = *(static_cast<double *>(var[j].value));
                                return i1 != d2;
                            }
                        }
                        else if (var[i].type == keyword[1])
                        {
                           d1 = *(static_cast<double *>(var[i].value));
                            if(var[j].type == keyword[0])
                            {
                                i2 = *(static_cast<int *>(var[j].value));
                                return d1 != i2;
                            }
                            else
                            {
                                d2 = *(static_cast<double *>(var[j].value));
                                return d1 != d2;
                            } 
                        }
                    }
                }
            }
        }
    }

    std ::string operation_string(std ::string arg1, std ::string nshan, std ::string arg2)
    {
        int size = var.size();
        std ::string x = "", y = "";
        if (nshan == "+")
        {
            for (int i = 0; i < size; ++i)
            {
                for (int j = 0; j < size; ++j)
                {
                    if (var[i].name == arg1 && var[j].name == arg2)
                    {
                        x = *(static_cast<std ::string *>(var[i].value));
                        y = *(static_cast<std ::string *>(var[j].value));
                        return x + y;
                    }
                }
            }
        }
    }
    void do_print(std ::vector<std ::string> & vec)
    {
        for (int i = 0; i < vec.size(); ++i)
        {
            if (vec[i] == ";")
            {
                break;
            }
            if (vec[i] == "~")
            {
                ++i;
            }
            for (int j = 0; j < var.size(); ++j)
            {
                if (vec[i] == var[j].name)
                {
                    if (var[j].type == keyword[0])
                    {
                        std::cout << *(static_cast<int *>(var[j].value)) << std ::endl;
                    }
                    else if (var[j].type == keyword[1])
                    {
                        std::cout << *(static_cast<double *>(var[j].value)) << std ::endl;
                    }
                    else if (var[j].type == keyword[2])
                    {
                        std::cout << *(static_cast<bool *>(var[j].value)) << std ::endl;
                    }
                    else if (var[j].type == keyword[3])
                    {
                        std::cout << *(static_cast<std ::string *>(var[j].value)) << std ::endl;
                    }
                    break;
                }
                else if (vec[i] == "(")
                {
                    ++i;
                    while (vec[i] != ")")
                    {
                        if (vec[i] == var[j].name)
                        {
                        if (var[j].type == keyword[0])
                        {
                            std::cout << operation_int(vec[3],vec[4],vec[5]) << std :: endl;
                        }
                        else if (var[j].type == keyword[1])
                        {
                            std::cout << operation_double(vec[3],vec[4],vec[5]) << std :: endl;
                        }
                        else if (var[j].type == keyword[2])
                        {
                            std::cout << operation_bool(vec[3],vec[4],vec[5]) << std :: endl;
                        }
                        else if (var[j].type == keyword[3])
                        {
                            std::cout << operation_string(vec[3],vec[4],vec[5]) << std :: endl;
                        }

                        }
                        break ;
                    }
                }
            }
        }
    }

    bool operation_bool(std::vector<std::string> & payman)
    {
        for (int j = 0; j < var.size(); ++j)
        {
            if (payman.size() == 1 && payman[0] == var[j].name)
            {
                return *(static_cast<bool*>(var[j].value));
            }
            else if (payman.size() > 1 && payman[1] == "&&" || payman[1] == "||" || payman[1] == ">" || payman[1] == "<" || payman[1] == "==" || payman[1] == ">=" || payman[1] == "<=" || payman[1] == "!=")
            {  
                *(static_cast<bool *>(var[j].value)) = operation_bool(payman[0], payman[1], payman[2]);
                return *(static_cast<bool*>(var[j].value));
            }
            else if (payman.size() > 1 && payman[0] == "!")
            {
                *(static_cast<bool *>(var[j].value)) =  !(*(static_cast<bool *>(var[j].value)));
                return *(static_cast<bool*>(var[j].value));
            }
            else 
            return false;
        }
    }

    void do_condition(std ::vector<std ::vector<std ::string>> & vec)
    {

        for (int k = 0; k < vec.size(); ++k)
        {
            translate(vec[k]);
        }
    }