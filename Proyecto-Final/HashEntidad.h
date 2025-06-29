#pragma once

#include <string>

class HashEntidadString {
private:
	std::string key;
	producto* value;
public:
	HashEntidadString(std::string key, producto* prod) {
		this->key = key;
		this->value = prod;
	}
	std::string getKey() { return key; }
	producto* getValue() { return value; }
};

