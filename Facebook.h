#ifndef __FACEBOOK_H
#define __FACEBOOK_H

#include "Header.h"

class User;
class Page;

class Facebook
{
private:
	std::vector<User*> users;
	std::vector<Page*> pages;

public:
	Facebook() = default;
	Facebook(std::istream& inFile);
	Facebook(const Facebook& other) = delete; // It is not possible to copy the Facebook system
	~Facebook();
	void addFriend(const User& user) noexcept(false);
	void addPage(const Page& page) noexcept(false);
	void printUsers() const;
	void printPages() const;
	int getNumOfUsers() const { return users.size(); }
	int getNumOfPages() const { return pages.size(); }
	User* getUser(int i) const { return users[i]; } // not returning const because sometimes we need to change the User
	Page* getPage(int i) const { return pages[i]; } // same reason
	int searchUserByName(std::string name) const noexcept(false);
	int searchPageByName(std::string name) const noexcept(false);
	void saveDataToFile() const;
};


#endif


