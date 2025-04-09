#pragma once
#include <iostream>
#include <string>


class Welfare
{
public:
	int id;
	std::string name;
	std::string description;
	int type;//deposit_offers = 0,	loan_discounts = 1,		credit_card_benefits = 2
	~Welfare() = default;
	Welfare(std::string name, std::string description, int type)
		: name(name), description(description), type(type) {
	}
	Welfare() {}
	Welfare(int id) {
		this->id = id;
	}
};