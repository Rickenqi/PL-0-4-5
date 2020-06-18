#include <bits/stdc++.h>
using namespace std;

class Computer
{
private:
	enum compare
	{
		LESS,
		EQUAL,
		GREAT,
		ERROR
	};
	map<pair<char, char>, compare> compareTable;

	void SetCompareTable()
	{
		compareTable[make_pair('+', '+')] = GREAT;
		compareTable[make_pair('+', '-')] = GREAT;
		compareTable[make_pair('+', ')')] = GREAT;
		compareTable[make_pair('+', '#')] = GREAT;

		compareTable[make_pair('-', '+')] = GREAT;
		compareTable[make_pair('-', '-')] = GREAT;
		compareTable[make_pair('-', ')')] = GREAT;
		compareTable[make_pair('-', '#')] = GREAT;

		compareTable[make_pair('*', '+')] = GREAT;
		compareTable[make_pair('*', '-')] = GREAT;
		compareTable[make_pair('*', '*')] = GREAT;
		compareTable[make_pair('*', '/')] = GREAT;
		compareTable[make_pair('*', ')')] = GREAT;
		compareTable[make_pair('*', '#')] = GREAT;

		compareTable[make_pair('/', '+')] = GREAT;
		compareTable[make_pair('/', '-')] = GREAT;
		compareTable[make_pair('/', '*')] = GREAT;
		compareTable[make_pair('/', '/')] = GREAT;
		compareTable[make_pair('/', ')')] = GREAT;
		compareTable[make_pair('/', '#')] = GREAT;

		compareTable[make_pair('(', ')')] = EQUAL;
		compareTable[make_pair('(', '#')] = ERROR;

		compareTable[make_pair(')', '+')] = GREAT;
		compareTable[make_pair(')', '-')] = GREAT;
		compareTable[make_pair(')', '*')] = GREAT;
		compareTable[make_pair(')', '/')] = GREAT;
		compareTable[make_pair(')', '(')] = ERROR;
		compareTable[make_pair(')', ')')] = GREAT;
		compareTable[make_pair(')', '#')] = GREAT;

		compareTable[make_pair('#', ')')] = ERROR;
		compareTable[make_pair('#', '#')] = EQUAL;
	}
	int cal(int left, char op, int right)
	{
		switch (op)
		{
		case '+':
			return left + right;
		case '-':
			return left - right;
		case '*':
			return left * right;
		case '/':
			return left / right;
		}
		return -1;
	}

public:
	void work(string exp)
	{
		if (compareTable.empty())
			SetCompareTable();
		exp += '#';
		stack<char> op;
		stack<int> val;
		op.push('#');
		stringstream ss(exp);
		char cur;

		while (ss >> cur)
		{
			if (isdigit(cur))
			{
				int num = cur - '0';
				while (ss >> cur && isdigit(cur))
				{
					num *= 10;
					num += cur - '0';
				}
				val.push(num);
			}

			int result;
			do
			{
				result = compareTable[make_pair(op.top(), cur)];
				switch (result)
				{
				case GREAT:
				{
					int right = val.top();
					val.pop();
					int left = val.top();
					val.pop();
					char oper = op.top();
					op.pop();
					val.push(cal(left, oper, right));
					break;
				}
				case LESS:
				{
					op.push(cur);
					break;
				}
				case EQUAL:
				{
					op.pop();
					break;
				}
				case ERROR:
				{
					cout << "expression error!" << endl;
					return;
				}
				}
			} while (result == GREAT);
		}

		cout << val.top() << endl;
	}
};

string readin(string filename)
{
	ifstream in;
	in.open(filename);
	string s;
	in >> s;
	return s;
}

int main(int argc, char **argv)
{
	Computer computer;
	string exp = readin(string(argv[1]));
	computer.work(exp);
}