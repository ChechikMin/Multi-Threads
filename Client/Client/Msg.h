
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
	std::vector<long long> data;

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
	removeCharsFromString(s, "-");
	return all_of(s.begin(), s.end(), ::isdigit);
	
}
bool Msg::check_size() {
	return this->str.size() <= 64;
}

void Msg::display() { std::cout << this->str; }
void Msg::fromStringToDigits() {
	data.clear();

	std::string tmp = str;
	std::regex r(R"([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?)");
	for (std::sregex_iterator i = std::sregex_iterator(tmp.begin(), tmp.end(), r);
		i != std::sregex_iterator();
		++i)
	{
		std::smatch m = *i;
		if (m.str().size() >= 10)
			std::cout << "Too large number."<< m.str() <<"Skiped\n";
		else
			data.push_back(std::stoi(m.str()));
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
	std::regex r("[^a-z^\s]*");
	std::string tmp = str;
	std::smatch m;
	
	std::regex_search(tmp, m, r);
	return std::atoi(m.str().c_str());
		
}
