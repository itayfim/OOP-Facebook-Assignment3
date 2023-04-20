#include "status.h"
using namespace std;

bool Status::operator==(const Status& status) const
{
	if (typeid(status) != typeid(*this))
		return false;
	else
		return text_status.content == status.text_status.content;
}

Status::Status(const string& content)
{
	text_status.content = content;
	time_t now = time(0);
	text_status.date_of_release = ctime(&now); // Put in the release date the time now
}

Status::Status(istream& in)
{
	size_t statusLen, creationDateLen;
	in.read((char*)&statusLen, sizeof(size_t));
	char* temp = new char[statusLen + 1];
	in.read((char*)temp, statusLen);
	temp[statusLen] = '\0';
	text_status.content = string(temp);
	delete[]temp;
	in.read((char*)&creationDateLen, sizeof(size_t));
	temp = new char[creationDateLen + 1];
	in.read((char*)temp, creationDateLen);
	temp[creationDateLen] = '\0';
	text_status.date_of_release = string(temp);
	delete[]temp;
}

void Status::printStatus() const
{
	cout << "Printing text status by Black & White:\n" << text_status.content << endl;
	cout << "Status creation date: " << text_status.date_of_release << endl;
}

void Status::saveDataToFile(ostream& file) const
{
	int len = strlen(typeid(*this).name()) - FIVE;
	char* typeID = new char[len];
	strcpy(typeID, typeid(*this).name() + 6);
	typeID[len - ONE] = '\0';
	size_t statusLen = text_status.content.size(), creationDateLen = text_status.date_of_release.size();
	file.write((const char*)&len, sizeof(int));
	file.write((const char*)typeID, len);
	file.write((const char*)&statusLen, sizeof(size_t));
	file.write((const char*)text_status.content.c_str(), statusLen);
	file.write((const char*)&creationDateLen, sizeof(size_t));
	file.write((const char*)text_status.date_of_release.c_str(), creationDateLen);
}
