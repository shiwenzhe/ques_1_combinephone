#include <iostream>
#include <fstream>
#include <set>
#include <string>
//#include <ctime>

using namespace std;

struct node
{
    set<string> fset; //用于存储对应电话的属性特征
    node * next[10];  //电话号码的每一位都是0-9的数字表示
    node()            //初始化
    {
        for(int i=0; i<10; i++)
        {
            next[i] = NULL;
        }
    }
};

//构造字典树
void construct(node *head, string line)
{
    int num;
    node * temp = head;
    //前11位肯定是电话号码，所以构造字典树
    for(int i=0; i<11; i++)
    {
        num = line[i] - '0';
        if(temp->next[num] == NULL)
        {
            temp->next[num] = new node();
        }
        temp = temp->next[num];
    }

    //最后一位对应的节点中，存入对应的特征值，set会将重复的特征覆盖，无需关注
    //line从第12位开始，就是特征值
    int start = 12;
    int last = line.length();
    int index = line.find(','); //标记逗号的位置
    while(index <= last && index >= 0)
    {
        temp->fset.insert(line.substr(start, index-start));
        start = index+1;
        index = line.find_first_of(',', start);
    }
    if(start != last)
    {
        temp->fset.insert(line.substr(start, last-start));
    }
}

//遍历整个字典树
void Traversal(node * head,string res, int deep, ofstream &out)
{
    if(deep == 11)
    {
        //cout << res << '|';
        out << res << '|';
        set<string>::iterator it = head->fset.begin();
        //cout << *it;
        out << *it;
        it ++;
        for(; it != head->fset.end(); it++)
        {
            //cout << ',' << *it;
            out << ',' << *it;
        }
        //cout << endl;
        out << endl;
        return ;
    }

    char c;
    string str;
    for(int i=0; i < 10; i++)
    {
        c = '0';
        str = res;
        if(head->next[i] != NULL)
        {
            c += i;
            str += c;
            Traversal(head->next[i], str, deep+1, out);
        }
    }
}

set<string> ifexist(string line, node * head)
{
    int         num;
    node *      temp = head;
    node *      last = temp;
    set<string> tempset;
    for(int i=0; i < 11; i++)
    {
        num = line[i] - '0';
        if(temp->next[num] == NULL)
        {
            return tempset;
        }
        else{
            last = temp;
            temp = temp->next[num];
        }
    }
    tempset = temp->fset;
    last->next[num] = NULL;
    return tempset;
}

int main()
{
    string line;
    node * head = new node();
    //clock_t eend, beg;

    //beg = clock();

    //-----------new.txt文件构造字典树--------
    fstream newin("new.txt");
    while(newin >> line)
    {
        construct(head, line);
    }
    newin.close();


    ofstream phoneallout("phone_all.txt");

    fstream phonein("phone.txt");

    while(phonein >> line)
    {
        set<string> myset = ifexist(line, head);
        //存在就写入并删除，不存在则直接写入文件
        if(myset.size() == 0){
            phoneallout << line << endl;
        }
        else{

            int start = 12;
            int last = line.length();
            int index = line.find(','); //标记逗号的位置
            while(index <= last && index >= 0)
            {
                myset.insert(line.substr(start, index-start));
                start = index+1;
                index = line.find_first_of(',', start);
            }
            if(start != last)
            {
                myset.insert(line.substr(start, last-start));
            }

            phoneallout << line.substr(0, 12);
            set<string>::iterator sit = myset.begin();
            phoneallout << *sit;
            sit ++;
            while(sit != myset.end())
            {
                phoneallout << ',' << *sit;
                sit ++;
            }
            phoneallout << endl;
        }
    }

    //------------遍历字典树------------------
    Traversal(head, "", 0, phoneallout);

    //eend = clock();
    //cout << "一共用时：" <<(double)(eend - beg)/CLK_TCK << " 秒" << endl;
    phoneallout.close();

    return 0;
}
