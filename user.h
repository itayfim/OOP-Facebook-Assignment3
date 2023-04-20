#ifndef __USER_H
#define __USER_H

#include "Header.h"
#include "Exceptions.h"
#include "status.h"

class Page;
class Status;

class User  // class of a friend in the Facebook system
{
private:
	std::string name;
	Date date_of_birth;
	std::vector<User*> friends;
	std::vector<Status*> my_statuses;
	std::vector<Page*> fan_pages;
	User(const User& other);

public:
	User() = delete;
	User(const std::string& name, const Date& date_of_birth);
	User(std::istream& in);
	~User();
	void addFriend(User& new_friend) noexcept(false);
	void deleteFriend(User& old_friend);
	void addStatusForUser(const Status& new_status);
	void addPage(Page& new_page);
	void deletePage(Page& old_page);
	const std::string& getName() const { return name; }
	const Date& getDateOfBirth() const { return date_of_birth; }
	void printFriends() const;
	void printStatusesOfUser() const;
	void printFriendsStatuses() const;
	void printFanPages() const;
	const int getNumOfFriends() const { return friends.size(); }
	void saveDataToFile(std::ofstream& file) const;
	bool operator>(const Page& page) const;
	bool operator<(const Page& page) const { return !(*this > page); }
	bool operator>(const User& user) const;
	bool operator<(const User& user) const { return !(*this > user); }
	const User& operator+=(User& user);

	friend class Facebook;
};

#endif