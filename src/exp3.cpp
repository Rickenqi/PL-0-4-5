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
    // �ҵ�����ߵ�term
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
    // �ӵ�һ��term��ʼ���ϵ�Ѱ����һ��term
    // ע������whileѭ�������������ж�term����֮�������һ�����Ǳ��ʽ���ʹ�����Ƿ�
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

    // �жϱ��ʽ�Ƿ����������������ѭ���Ƿ��ȡ�������е��ַ������ҷ�������
    if (flag && w == 0 && cur_pos != vec.size()) {
        flag = 0;
        error_information.push_back("the link symbol of the expression is invalid");
    }

    return flag;
}

bool exp3::term(string &num) {
    bool flag = 1;
    int op;
    // ��һ���ַ�һ�������ӣ�����û�г���������һ��������
    // ����+,-��������������֣���Ϊ�Ѿ������ʽ�ж����ų�
    flag &= factor(num);
    // �������Ƕ�������ж�ԭ������ʽһ��
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
    // �Ѿ�λ�ڱ��ʽ�����Ҷˣ���������Ȼ��Ҫһ������
    if (cur_pos == vec.size()) {
        flag = 0;
        error_information.push_back("the expression has an invalid ending or uncomplete !");
    }
    // ע�⣬�����Ѿ�����С��λ���������ѭ��
    // ֱ���жϵ�ǰ������ʲô
    if (flag && (vec[cur_pos].first == "number" || vec[cur_pos].first == "ident")) 
    {
        num=vec[cur_pos].second;
        cur_pos++;
    }
    else if (flag && vec[cur_pos].first == "lparen") {
        cur_pos++;
        // �����п��Գ��ֱ��ʽ
        flag &= expression(1,num);
        // ע�⣬֮ǰ���ж��ж�û�������ŵ��жϣ����������������ʱ���˳��ݹ飬��ʱ��ָ�룬ָ�����������
        if (flag && cur_pos < vec.size() && vec[cur_pos].first == "rparen") cur_pos++;
        else if (flag){
            flag = 0;
            error_information.push_back("the rparen is missing !");
        }
    }
    else if (flag){
        flag = 0;
        // ���ﾭ�����漸���ж�֮�󣬳�����������������������߰���Ķ�������ˣ�����������"()"������û���κα��ʽ
        // ���ң����űպ��Ѿ���������й����ж�
        if (cur_pos - 1 >= 0 && vec[cur_pos - 1].first == "lparen")
            error_information.push_back("a expression is needed after the lparen!");
        // ��������߰���Ķ���
        else
            error_information.push_back("symbol'" + vec[cur_pos].second + "'need a previous identifier or number!");
    }

    return flag;
}