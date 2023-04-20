#ifndef __PAGE_H
#define __PAGE_H

#include "Header.h"
#include "status.h"

class Status;
class User;

class Page
{
private:
	std::string name;
	std::vector<User*> followers;
	std::vector<Status*> page_statuses;

	Page(const Page& other);

public:
	Page() = delete;
	Page(std::istream& in);
	Page(const std::string name) : name(name) {}
	~Page();
	void addFollower(User& new_follower);
	void deleteFollower(User& old_follower);
	void addStatusForPage(const Status& new_status);
	const std::string& getName() const { return name; }
	const int getNumOfFollowers() const { return followers.size(); }
	const int getNumOfStatuses() const { return page_statuses.size(); }
	const Status* getStatus(int i) const { return page_statuses[i]; }
	void printFollowers() const;
	void printStatusesOfPage() const;
	void saveDataToFile(std::ostream& file) const;
	const Page& operator+=(User& follower);
	bool operator>(const Page& other) const;
	bool operator<(const Page& other) const { return !(*this > other); }
	bool operator>(const User& user);
	bool operator<(const User& user) { return !(*this > user); }
	const Page& operator=(const Page& page) = delete;

	friend class Facebook;
};

#endif
