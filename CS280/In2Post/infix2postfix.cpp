#include "infix2postfix.h"
#include <stack>
#include <cctype>
#include <iostream>
int get_priority(char c);

int Evaluate(std::string const &postfix)
{
	std::cout <<postfix<< std::endl;
	std::stack<int> stack;
	for (char c : postfix)
	{
		if (isdigit(c))
		{
			stack.push(c - '0');
		}
		else
		{
			int b = stack.top();
			stack.pop();
			int a = stack.top();
			stack.pop();
			switch (c)
			{
			case '+':
				stack.push(a + b);
				break;
			case '-':
				stack.push(a - b);
				break;
			case '*':
				stack.push(a * b);
				break;
			case '/':
				stack.push(a / b);
				break;
			}
		}
	}
	return stack.top();
}

std::string Infix2postfix(std::string const &infix)
{
	std::stack<char> stack;
	std::string postfix;
	for (char c : infix)
	{
		if (isdigit(c))
		{
			postfix += c;
		}
		else if (c == '(')
		{
			stack.push(c);
		}
		else if (c == ')')
		{
			while (stack.top() != '(')
			{
				postfix += stack.top();
				stack.pop();
			}
			stack.pop();
		}
		else
		{
			while (!stack.empty() && get_priority(stack.top()) >= get_priority(c) && stack.top() != '(')
			{
				postfix += stack.top();
				stack.pop();
			}
			stack.push(c);
		}
	}
	while (!stack.empty())
	{
		postfix += stack.top();
		stack.pop();
	}

	return postfix;
}

int get_priority(char c)
{
	if (c == '+' || c == '-')
	{
		return 1;
	}
	else if (c == '*' || c == '/')
	{
		return 2;
	}
	else
	{
		return 0;
	}
}
