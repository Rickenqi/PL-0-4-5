#include "variables.h"
#include "exp3.h"
#include <iostream>
using namespace std;

string decode[31] = { "begin", "call", "const", "do", "end", "if", "odd", "procedure", "read", "then", "var", "while",
                      "write", "identifier", "number", "+", "-", "*", "/", "=", "#", "<", "<=", ">", ">=", ":=",
                      "(", ")", ",", ";", "." };
string encode[31] = { "beginsym", "callsym", "constsym", "dosym", "endsym", "ifsym", "oddsym", "proceduresym", "readsym",
                      "thensym", "varsym", "whilesym", "writesym", "ident", "number", "plus", "minus", "times", "slash",
                      "eql", "neq", "lss", "leq", "gtr", "geq", "becomes", "lparen", "rparen", "comma", "semicolon", "period" };
map<string, string> encoder;
set<char> ender;

void init() {
    for (int i = 0; i < 31; ++i) {
        encoder.insert(pair<string,string>(decode[i], encode[i]));
        // cout << decode[i] << " " << encoder[decode[i]] << endl;
    }
    ender.insert('+'); ender.insert('-'); ender.insert('*'); ender.insert('/'); ender.insert(':');
    ender.insert('='); ender.insert('#'); ender.insert('>'); ender.insert('<'); ender.insert('(');
    ender.insert(')'); ender.insert(','); ender.insert(';'); ender.insert('.');
}

void deal_encode(string &line)
{
    vector<pair<string, string>> math_exp;
    // 正则符，分别匹配数字和标识符
    regex number("[0-9]+"),identifier("[a-z]([a-z]|[0-9])*");
    string cur;
    // 每次记录终结符之间的字符，在第二个终结符出现时进行处理
    for(int i=0;i<=line.length();++i)
    {
        if(i==line.length()||line[i]==' '||ender.count(line[i])) // 在终结符处进行处理
        {
            // 分别处理关键字，标识符，数字
            if(cur.length() && encoder.count(cur))
            {
                math_exp.emplace_back(encoder[cur],cur);
                total.emplace_back(encoder[cur], cur);
            }
            else if(cur.length() && regex_match(cur,identifier))
            {
                math_exp.emplace_back("ident",cur);
                total.emplace_back("ident",cur);
            }
            else if(cur.length() && regex_match(cur,number))
            {
                math_exp.emplace_back("number",cur);
                total.emplace_back("number",cur);
            }
            else if(cur.length())
                cout << "error occur: " << cur;
            if(i==line.length())
                break;
            // 处理完毕，清空cur
            cur="";
            // 处理运算符（终结符）
            if(line[i]!=' ')
            {
                string Ender;
                Ender+=line[i];
                if(i+1<line.length()&&encoder.count(Ender+line[i+1]))
                {
                    Ender+=line[i+1];
                    i++;
                }
                total.emplace_back(encoder[Ender],Ender);
                math_exp.emplace_back(encoder[Ender],Ender);
            }
            continue;
        }
        // 非终结符记录当前字符
        cur+=line[i];
    }
    // 换行符

    cout << line << endl;
    for(int i = 0; i < math_exp.size(); i++)
        cout << "(" << math_exp[i].first << "," << math_exp[i].second << ")" << endl;
    exp3 exp(math_exp);
    exp.print_ret();

    total.emplace_back("endline","endl");
}
