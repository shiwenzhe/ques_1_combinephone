#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <set>

using namespace std;

set<string> phoneflag(string str)
{
    int start = 12;
    int last  = str.length();
    int index = str.find(',');
    set<string> res;
    while(index <= last && index > 0)
    {
        res.insert(str.substr(start, index-start));
        start = index+1;
        index = str.find_first_of(',', start);
    }
    if(start != last)
    {
        res.insert(str.substr(start, last-start));
    }
    return res;
}

int main()
{
    fstream     newin("new.txt");
    string      line;
    unordered_map<string, set<string> >    hmap;

    string      ssbuffer = "";//�ַ�������

    //����new.txt�ļ����������has_map
    while(newin >> line)
    {
        string      phone = line.substr(0, 11);
        set<string> flag  = phoneflag(line);
        if(hmap.find(phone) == hmap.end())
            hmap.insert(pair<string, set<string> >(phone, flag));
        else{
            //new.txt�ļ������������ظ��ĵ绰���룬���ظ��ĵ绰�����flag�ϲ�
            for(set<string>::iterator setit=flag.begin(); setit != flag.end(); setit++)
            {
                hmap[phone].insert(*setit);
            }
        }
    }
    newin.close();

    //����phone.txt�ļ�,��hash_map�е��ļ��Աȣ���������²�ɾ����������ֱ��д������ļ�
    fstream     phonein("phone.txt");
    ofstream    phoneout("phone_all.txt");
    while(phonein >> line)
    {
        string      phone = line.substr(0, 11);
        unordered_map<string, set<string> >::iterator it = hmap.find(phone);
        if(it == hmap.end())
        {
            //phoneout << line << endl;
            ssbuffer += line;
            ssbuffer += "\n";
        }
        else{
            set<string> flag = phoneflag(line);
            for(set<string>::iterator setit=flag.begin(); setit != flag.end(); setit++)
            {
                hmap[phone].insert(*setit);
            }
            //phoneout << phone << '|';
            ssbuffer += phone;
            ssbuffer += "|";
            //����Ӧ��set�еı�ǩ�����
            set<string>::iterator setit=hmap[phone].begin();
            //phoneout << *setit;
            ssbuffer += phone;
            setit++;
            for(; setit != hmap[phone].end(); setit ++)
            {
                //phoneout << ',' << *setit;
                ssbuffer += ",";
                ssbuffer += *setit;
            }
            //phoneout << endl;
            ssbuffer += "\n";

            //���Ѿ�����ɾ��
            hmap.erase(hmap.find(phone));
        }

        if(ssbuffer.length() >= 1024)
        {
            phoneout << ssbuffer;
            ssbuffer = "";
        }
    }
    if(ssbuffer.length() != 0)
    {
        phoneout << ssbuffer;
        ssbuffer = "";
    }
    phonein.close();

    //����hash_map����ʣ������д��phone_all.txt�ļ���
    unordered_map<string, set<string> >::iterator unmapit = hmap.begin();
    while(unmapit != hmap.end())
    {
        //phoneout << unmapit->first << "|";
        ssbuffer += unmapit->fisrt;
        ssbuffer += "|";

        set<string>::iterator sit = unmapit->second.begin();
        if(unmapit->second.size() != 0)
        {
            //phoneout << *sit ;
            ssbuffer += *sit;
            sit ++;
        }
        for(;sit != unmapit->second.end(); sit ++)
        {
            //phoneout << ',' << *sit ;
            ssbuffer += ",";
            ssbuffer += *sit;
        }
        //phoneout << endl;
        ssbuffer += "\n";
        unmapit ++;

        if(ssbuffer.length() >= 1024)
        {
            phoneout << ssbuffer;
            ssbuffer = "";
        }
    }

    if(ssbuffer.length() != 0)
    {
        phoneout << ssbuffer;
        ssbuffer = "";
    }

    phoneout.close();

    //cout << "Hello world!" << endl;
    return 0;
}
