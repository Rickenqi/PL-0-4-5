#pragma once
#include<string>
#include<vector>
using namespace std;

/************************************************************************/
/*
<Expr>   ->  [+|-] <Term> { (+|-) <Term> }
<Term>   ->  <Factor> { (*|/) <Factor> }
<Factor> ->  (<Expr>) | num
+ plus
- minus
* times
/ slash
( lparen
) rparen
a ident
/************************************************************************/

class exp3 {
public:
    exp3(const vector<pair<string, string>> &vec);
    ~exp3();
    void print_ret() const;
private:
    bool ret;
    int cur_pos;
    vector<string> error_information;
    vector<pair<string, string>> vec;
    vector<vector<string>> tuples;
    int tot;
    bool expression(int w,string &num);
    bool term(string &num);
    bool factor(string &num);
};