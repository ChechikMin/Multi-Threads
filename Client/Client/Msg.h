
#pragma once
#include<string>
#include<algorithm>
#include<cctype>
#include<numeric>
#include<regex>



class Msg
{
public:

	Msg(std::string str);
	void display();
	int sum();
	bool is_digits();
	bool check_size();
	void removeCharsFromString(std::string &str, const char* charsToRemove);
	void sorting_str();
	void fromStringToDigits();
	std::string prepareForSend();

protected:
	std::string str;
	std::vector<int> data;

};

Msg::Msg(std::string str) : str(str) {}
void Msg::removeCharsFromString(std::string &str, const char* charsToRemove) {
	for (unsigned int i = 0; i < strlen(charsToRemove); ++i) {
		str.erase(remove(str.begin(), str.end(), charsToRemove[i]), str.end());
	}
}

int Msg::sum() {

	return std::accumulate(data.begin(), data.end(), 0);
}

bool Msg::is_digits()
{
	std::string s = str;
	removeCharsFromString(s, " ");
	return all_of(s.begin(), s.end(), ::isdigit);
	
}
bool Msg::check_size() {
	return this->str.size() <= 64;
}

void Msg::display() { std::cout << this->str; }
void Msg::fromStringToDigits() {
	data.clear();
	std::regex r("\d");
	std::string tmp = str;
	
	while (!tmp.empty())
	{
		std::smatch m;
		std::regex_search(tmp, m, r);
		
		for (auto v : m) data.push_back(std::atoi(v.str().c_str()));
		
		tmp.erase(tmp.begin());
		while (!std::isdigit(tmp[0]) && !tmp.empty())
			tmp.erase(tmp.begin());
	}
}


void Msg::sorting_str() {
	std::sort(data.begin(), data.end(),
		[](int const & a, int const & b)
		{ return  a > b; });
}
std::string Msg::prepareForSend() {
	int i = 1;
	return std::accumulate(begin(data), end(data), std::string(),
		[&](std::string r, int x) {
			if (i % 2 == 0) {
				i++;
				return r += "KB";
			}
			i++;
			return r += std::to_string(x); });

}

class GettenMsg :protected Msg
{
public:
	int fromStringToDigits();
	GettenMsg(std::string str) : Msg(str)  {}
private:

};

int GettenMsg::fromStringToDigits() {
	std::regex r("[0-9]*");
	std::string tmp = str;
	std::smatch m;
	
	std::regex_search(tmp, m, r);
	return std::atoi(m.str().c_str());
		
}
