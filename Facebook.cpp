using namespace std;
#include "Facebook.h"
#include "Exceptions.h"

Facebook::Facebook(istream& inFile)
{
	size_t usersSize, numOfFriends, nameLen, pagesSize, numOfFollowers, followerNameLen;
	char* temp;
	int index_friend, index_follower;
	inFile.read((char*)&usersSize, sizeof(size_t));
	for (int i = 0; i < usersSize; i++)
		addFriend(User(inFile));
	for (int i = 0; i < usersSize; i++)
	{
		inFile.read((char*)&numOfFriends, sizeof(size_t));
		for (int j = 0; j < numOfFriends; j++)
		{
			inFile.read((char*)&nameLen, sizeof(size_t));
			temp = new char[nameLen + ONE];
			inFile.read(temp, nameLen);
			temp[nameLen] = '\0';
			index_friend = searchUserByName(string(temp));
			delete[]temp;
			users[i]->addFriend(*(users[index_friend]));
		}
	}
	inFile.read((char*)&pagesSize, sizeof(size_t));
	for (int i = 0; i < pagesSize; i++)
	{
		addPage(Page(inFile));
		inFile.read((char*)&numOfFollowers, sizeof(size_t));
		for (int j = 0; j < numOfFollowers; j++)
		{
			inFile.read((char*)&followerNameLen, sizeof(size_t));
			temp = new char[followerNameLen + ONE];
			inFile.read(temp, followerNameLen);
			temp[followerNameLen] = '\0';
			index_follower = searchUserByName(string(temp));
			delete[]temp;
			pages[i]->addFollower(*(users[index_follower]));
		}
	}
}


Facebook::~Facebook()
{
	this->saveDataToFile();
	vector<User*>::iterator usersItr = users.begin();
	vector<User*>::iterator UsersEndItr = users.end();
	for (; usersItr != UsersEndItr; ++usersItr)
		delete* usersItr;
	vector<Page*>::iterator pagesItr = pages.begin();
	vector<Page*>::iterator PagesEndItr = pages.end();
	for (; pagesItr != PagesEndItr; ++pagesItr)
		delete* pagesItr;
}

void Facebook::addFriend(const User& user) noexcept(false)
{
	vector<User*>::iterator usersItr = users.begin();
	vector<User*>::iterator UsersEndItr = users.end();
	for (; usersItr != UsersEndItr; ++usersItr)
	{
		if ((*usersItr)->getName() == user.getName())
			throw UserOrPageExist(user.getName());
	}
	User* newUser = new User(user);
	users.push_back(newUser);
}

void Facebook::addPage(const Page& page) noexcept(false)
{
	vector<Page*>::iterator pagesItr = pages.begin();
	vector<Page*>::iterator PagesEndItr = pages.end();
	for (; pagesItr != PagesEndItr; ++pagesItr)
	{
		if ((*pagesItr)->getName() == page.getName())
			throw UserOrPageExist(page.getName());
	}
	Page* newPage = new Page(page);
	pages.push_back(newPage);
}

void Facebook::printUsers() const
{
	int i = 0;
	vector<User*>::const_iterator usersItr = users.begin();
	vector<User*>::const_iterator UsersEndItr = users.end();
	for (; usersItr != UsersEndItr; ++usersItr)
	{
		cout << ++i << ". " << (*usersItr)->getName() << endl;
		(*usersItr)->printStatusesOfUser();
	}
}

void Facebook::printPages() const
{
	int i = 0;
	vector<Page*>::const_iterator pagesItr = pages.begin();
	vector<Page*>::const_iterator PagesEndItr = pages.end();
	for (; pagesItr != PagesEndItr; ++pagesItr)
	{
		cout << ++i << ". " << (*pagesItr)->getName() << endl;
		(*pagesItr)->printStatusesOfPage();
	}
}

int Facebook::searchUserByName(string name) const noexcept(false)
{
	vector<User*>::const_iterator usersItr = users.begin();
	vector<User*>::const_iterator UsersEndItr = users.end();
	int count = 0;
	for (; usersItr != UsersEndItr; ++usersItr)
	{
		if ((*usersItr)->getName() == name)
			return count;
		count++;
	}
	throw UserOrPageNotExist(name);
}

int Facebook::searchPageByName(string name) const noexcept(false)
{
	int count = 0;
	vector<Page*>::const_iterator pagesItr = pages.begin();
	vector<Page*>::const_iterator PagesEndItr = pages.end();
	for (; pagesItr != PagesEndItr; ++pagesItr)
	{
		if ((*pagesItr)->getName() == name)
			return count;
		count++;
	}
	throw UserOrPageNotExist(name);
}

void Facebook::saveDataToFile() const
{
	ofstream file(FILE_NAME, ios::binary | ios::trunc);
	size_t usersSize = users.size(), pagesSize = pages.size(), numOfFriends, nameLen, fanPagesOfUserLen;
	file.write((const char*)&usersSize, sizeof(size_t));
	for (int i = 0; i < usersSize; i++)
		users[i]->saveDataToFile(file);
	for (int i = 0; i < usersSize; i++)
	{
		numOfFriends = users[i]->friends.size();
		file.write((const char*)&numOfFriends, sizeof(size_t));
		for (int j = 0; j < numOfFriends; j++)
		{
			nameLen = users[i]->friends[j]->name.size();
			file.write((const char*)&nameLen, sizeof(size_t));
			file.write((const char*)users[i]->friends[j]->name.c_str(), nameLen);
		}
	}
	file.write((const char*)&pagesSize, sizeof(size_t));
	for (int i = 0; i < pagesSize; i++)
		pages[i]->saveDataToFile(file);
	file.close();
}

