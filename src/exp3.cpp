#include "exp3.h"
#include <iostream>

exp3::exp3(const vector<pair<string, string>>& vec)
        :ret(true), cur_pos(0), vec(vec), tot(0) {
    string num;
    ret = expression(0,num);
}

exp3::~exp3() {}

void exp3::print_ret() const {
    cout << "------------------------------" << endl;
    cout << cur_pos << "/" << vec.size() << endl;
    if (ret) {
        cout << "Correct!" << endl;
        cout << "\nTuples: " << endl;
        for(auto v:tuples)
            cout<< "(" << v[0] << "," << v[1] << "," << v[2] << "," << v[3] << ")" << endl;
    }
    else {
        for (auto& it : vec) cout << it.second << " ";
        cout << endl;
        int blank_cnt = 0;
        for (int i = 0; i < cur_pos; i++)
            blank_cnt += 1 + vec[i].second.size();
        for (int i = 0; i < blank_cnt; i++) cout << ' ';
        cout << "^ ";
        //  << "Error!" << endl;
        for (auto& it : error_information) cout << it << endl;
    }
    cout << "------------------------------" << endl;
}

bool exp3::expression(int w,string &num) {
    bool flag = 1;
    int op = 0;
    // 找到最左边的term
    if (cur_pos < vec.size()
        && (vec[cur_pos].first == "plus" || vec[cur_pos].first == "minus"))
        {
            if(vec[cur_pos].first == "plus")
                op=1;
            else
                op=2;
            cur_pos++;
        }
    flag &= term(num);
    if(op)
    {
        vector<string> temp;
        temp.push_back(op==1?"+":"-");
        temp.push_back(num);
        temp.push_back(" ");
        num="t"+to_string(++tot);
        temp.push_back(num);
        tuples.push_back(temp);
    }
    // 从第一个term开始不断地寻找下一个term
    // 注意这里while循环的条件，在判断term结束之后如果下一个不是表达式，就代表表达非法
    while (flag && cur_pos < vec.size()
           && (vec[cur_pos].first == "plus" || vec[cur_pos].first == "minus")) {
        if(vec[cur_pos].first == "plus")
            op=1;
        else
            op=2;
        cur_pos++;
        vector<string> temp;
        temp.push_back(op==1?"+":"-");
        temp.push_back(num);
        flag &= term(num);
        temp.push_back(num);
        num="t"+to_string(++tot);
        temp.push_back(num);
        tuples.push_back(temp);
    }

    // 判断表达式非法的条件就是上面的循环是否读取完了所有的字符，并且发生错误
    if (flag && w == 0 && cur_pos != vec.size()) {
        flag = 0;
        error_information.push_back("the link symbol of the expression is invalid");
    }

    return flag;
}

bool exp3::term(string &num) {
    bool flag = 1;
    int op;
    // 第一个字符一定是因子，或者没有出错的情况下一定是因子
    // 关于+,-并不会在这里出现，因为已经被表达式判断所排除
    flag &= factor(num);
    // 下面则是对于项的判断原理与表达式一致
    while (flag && cur_pos < vec.size()
           && (vec[cur_pos].first == "times" || vec[cur_pos].first == "slash")) {
        if(vec[cur_pos].first == "times")
            op=1;
        else
            op=2;
        cur_pos++;
        vector<string> temp;
        temp.push_back(op==1?"*":"/");
        temp.push_back(num);
        flag &= factor(num);
        temp.push_back(num);
        num="t"+to_string(++tot);
        temp.push_back(num);
        tuples.push_back(temp);
    }
    return flag;
}

bool exp3::factor(string &num) {
    bool flag = 1;
    // 已经位于表达式的最右端，但是项任然需要一个因子
    if (cur_pos == vec.size()) {
        flag = 0;
        error_information.push_back("the expression has an invalid ending or uncomplete !");
    }
    // 注意，因子已经是最小单位，因此无需循环
    // 直接判断当前的项是什么
    if (flag && (vec[cur_pos].first == "number" || vec[cur_pos].first == "ident")) 
    {
        num=vec[cur_pos].second;
        cur_pos++;
    }
    else if (flag && vec[cur_pos].first == "lparen") {
        cur_pos++;
        // 括号中可以出现表达式
        flag &= expression(1,num);
        // 注意，之前的判断中都没有右括号的判断，因此在遇到右括号时会退出递归，此时的指针，指向的是右括号
        if (flag && cur_pos < vec.size() && vec[cur_pos].first == "rparen") cur_pos++;
        else if (flag){
            flag = 0;
            error_information.push_back("the rparen is missing !");
        }
    }
    else if (flag){
        flag = 0;
        // 这里经过上面几个判断之后，除了右括号以外就是其他乱七八糟的东西，因此，这里的情况是"()"括号中没有任何表达式
        // 并且，括号闭合已经在上面进行过了判断
        if (cur_pos - 1 >= 0 && vec[cur_pos - 1].first == "lparen")
            error_information.push_back("a expression is needed after the lparen!");
        // 这个是乱七八糟的东西
        else
            error_information.push_back("symbol'" + vec[cur_pos].second + "'need a previous identifier or number!");
    }

    return flag;
}