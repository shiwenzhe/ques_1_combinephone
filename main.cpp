#include <iostream>
#include <fstream>
#include <set>
#include <string>
//#include <ctime>

using namespace std;

struct node
{
    set<string> fset; //���ڴ洢��Ӧ�绰����������
    node * next[10];  //�绰�����ÿһλ����0-9�����ֱ�ʾ
    node()            //��ʼ��
    {
        for(int i=0; i<10; i++)
        {
            next[i] = NULL;
        }
    }
};

//�����ֵ���
void construct(node *head, string line)
{
    int num;
    node * temp = head;
    //ǰ11λ�϶��ǵ绰���룬���Թ����ֵ���
    for(int i=0; i<11; i++)
    {
        num = line[i] - '0';
        if(temp->next[num] == NULL)
        {
            temp->next[num] = new node();
        }
        temp = temp->next[num];
    }

    //���һλ��Ӧ�Ľڵ��У������Ӧ������ֵ��set�Ὣ�ظ����������ǣ������ע
    //line�ӵ�12λ��ʼ����������ֵ
    int start = 12;
    int last = line.length();
    int index = line.find(','); //��Ƕ��ŵ�λ��
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

//���������ֵ���
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

    //-----------new.txt�ļ������ֵ���--------
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
        //���ھ�д�벢ɾ������������ֱ��д���ļ�
        if(myset.size() == 0){
            phoneallout << line << endl;
        }
        else{

            int start = 12;
            int last = line.length();
            int index = line.find(','); //��Ƕ��ŵ�λ��
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

    //------------�����ֵ���------------------
    Traversal(head, "", 0, phoneallout);

    //eend = clock();
    //cout << "һ����ʱ��" <<(double)(eend - beg)/CLK_TCK << " ��" << endl;
    phoneallout.close();

    return 0;
}
