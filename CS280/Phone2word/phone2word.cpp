#include <string>
#include <vector>
#include <map>
#include "phone2word.h"

std::vector<std::string> all_combinations(std::string const &phone_number, std::map<char, std::string> &keypad)
{
	std::vector<std::string> combs = {""};
	for (char digit : phone_number)
	{
		if (keypad.count(digit) > 0)
		{
			std::vector<std::string> new_combs;
			for (std::vector<std::string>::iterator comb = combs.begin(); comb != combs.end(); ++comb)
			{
				for (char c : keypad[digit])
				{
					new_combs.push_back(*comb + c);
				}
			}
			combs = new_combs;
		}
		else
		{
			for (std::vector<std::string>::iterator comb = combs.begin(); comb != combs.end(); ++comb)
			{
				*comb += digit;
			}
		}
	}
	return combs;
}
