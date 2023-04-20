#include "status.h"
#include <Windows.h>
#define END_FILE ".png"

using namespace std;

StatusImage::StatusImage(const std::string& content, const std::string& file_name) : Status(content)
{
	this->file_name = file_name;
	std::string temp = file_name + END_FILE;
	std::ofstream file(temp, ios::app);
	file.close();
}

void StatusImage::printStatus() const
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
	cout << "Printing Image status by color: \nStatus content : " << this->text_status.content <<
		"\nStatus creation date : " << this->text_status.date_of_release;
	string temp = START + file_name + END_FILE;
	system(temp.c_str());
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}
void StatusImage::saveExtra(std::ostream& file) const
{
	size_t len = file_name.size();
	file.write((const char*)&len, sizeof(size_t));
	file.write((const char*)file_name.c_str(), len);
}

StatusImage::StatusImage(istream& in) : Status(in)
{
	size_t len;
	in.read((char*)&len, sizeof(size_t));
	char* str = new char[len+1];
	in.read((char*)str, len);
	str[len] = '\0';
	file_name = string(str);
	delete[]str;
}