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
    bool expression(int w);
    bool term();
    bool factor();
};