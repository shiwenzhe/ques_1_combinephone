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

    string      ssbuffer = "";//字符缓冲区

    //遍历new.txt文件，将其存入has_map
    while(newin >> line)
    {
        string      phone = line.substr(0, 11);
        set<string> flag  = phoneflag(line);
        if(hmap.find(phone) == hmap.end())
            hmap.insert(pair<string, set<string> >(phone, flag));
        else{
            //new.txt文件中若果存在重复的电话号码，将重复的电话号码的flag合并
            for(set<string>::iterator setit=flag.begin(); setit != flag.end(); setit++)
            {
                hmap[phone].insert(*setit);
            }
        }
    }
    newin.close();

    //遍历phone.txt文件,和hash_map中的文件对比，存在则更新并删除，不存在直接写入输出文件
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
            //将对应的set中的标签填到后面
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

            //将已经存在删除
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

    //遍历hash_map，将剩余数据写入phone_all.txt文件中
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
