#include "user.h"
#include "status.h"
using namespace std;

User::User(const User& other)
{
	for (int i = 0; i < other.my_statuses.size(); i++)
	{
		addStatusForUser(*(other.my_statuses[i]));
	}
	name = other.name;
	date_of_birth = other.date_of_birth;
	friends = other.friends;
	fan_pages = other.fan_pages;
}

User::User(const std::string& name, const Date& date_of_birth) : name(name)
{
	if (date_of_birth.day > MAX_DAYS_IN_MONTH || date_of_birth.day < ONE ||
		date_of_birth.month > MAX_MONTHS_IN_YEAR || date_of_birth.month < ONE) //Check if the date input is correct
		throw DateException();
	this->date_of_birth = date_of_birth;
}

User::User(istream& in)
{
	size_t nameLen, numOfStatuses; int len;
	const type_info& typeID = typeid(*this);
	in.read((char*)&nameLen, sizeof(size_t));
	char* tempName = new char[nameLen + 1];
	in.read((char*)tempName, nameLen);
	tempName[nameLen] = '\0';
	name = string(tempName);
	delete[] tempName;
	in.read((char*)&date_of_birth, sizeof(Date));
	in.read((char*)&numOfStatuses, sizeof(size_t));
	for (int i = 0; i < numOfStatuses; i++)
	{
		in.read((char*)&len, sizeof(int));
		tempName = new char[len];
		in.read(tempName, len);
		if (!strcmp(tempName, typeid(Status).name() + SIX))
			addStatusForUser(Status(in));
		else if (!strcmp(tempName, typeid(StatusImage).name() + SIX))
			addStatusForUser(StatusImage(in));
		else
			addStatusForUser(StatusVideo(in));
		delete[]tempName;
	}
}

User::~User()
{
	vector<Status*>::iterator itr = my_statuses.begin();
	vector<Status*>::iterator endItr = my_statuses.end();
	for (; itr != endItr; ++itr)
		delete* itr;
}

void User::addFriend(User& new_friend) noexcept(false)
{
	if (this->name == new_friend.getName())
		throw SelfFriend();
	bool founded = false;
	vector<User*>::iterator itr = friends.begin();
	vector<User*>::iterator itrEnd = friends.end();
	for (; !founded && itr != itrEnd; ++itr) // Check if the new friend is already following the friend
	{
		if ((*itr)->getName() == new_friend.getName())
			founded = true;
	}
	if (!founded) // The new friend isn't in the list of friends of the current friend
	{
		friends.push_back(&new_friend); // Adding the new friend to the array of friends
		new_friend.addFriend(*this); // Mutual action, the new friend will add the current friend to his list of friends
	}
}

void User::deleteFriend(User& old_friend)
{
	bool founded = false;
	vector<User*>::iterator itr = friends.begin();
	vector<User*>::iterator itrEnd = friends.end();
	for (; itr != itrEnd && !founded; ++itr) // Check if the old friend is already deleted from the list of friends of the current friend
	{
		if ((*itr)->getName() == old_friend.getName()) // The old friend wasn't deleted
		{
			friends.erase(itr);
			old_friend.deleteFriend(*this); // Mutual action, the old friend will delete the current friend from his list of friends
			break;
		}
	}
}

void User::addStatusForUser(const Status& new_status)
{
	const Status* text = dynamic_cast<const Status*>(&new_status);
	const StatusImage* image = dynamic_cast<const StatusImage*>(&new_status);
	const StatusVideo* video = dynamic_cast<const StatusVideo*>(&new_status);
	if (image)
	{
		Status* status = new StatusImage(*image);
		my_statuses.push_back(status);
	}
	else if (video)
	{
		Status* status = new StatusVideo(*video);
		my_statuses.push_back(status);
	}
	else if (text)
	{
		Status* status = new Status(new_status);
		my_statuses.push_back(status);
	}
	else
		throw StatusObjectType();
}

void User::addPage(Page& new_page)
{
	bool founded = false;
	vector<Page*>::iterator itr = fan_pages.begin();
	vector<Page*>::iterator itrEnd = fan_pages.end();

	for (; itr != itrEnd && !founded; ++itr) // Check if the new page is already in the friend's list of pages
	{
		if ((*itr)->getName() == new_page.getName())
			founded = true;
	}
	if (!founded) // The new page isn't in the friend's list of pages
	{
		fan_pages.push_back(&new_page);
		new_page.addFollower(*this); // Mutual action,the new page will add the current friend to his list of followers
	}
}

void User::deletePage(Page& old_page)
{
	vector<Page*>::iterator itr = fan_pages.begin();
	vector<Page*>::iterator itrEnd = fan_pages.end() - 1;
	for (; itr != itrEnd; --itrEnd) // Check if the old page is already deleted from the list of pages of the current friend
	{
		if ((*itrEnd)->getName() == old_page.getName()) // The old page wasn't deleted
		{
			fan_pages.erase(itrEnd);
			old_page.deleteFollower(*this); // Mutual action, the old page will delete the current friend from his list of followers
			break;
		}
	}
}

void User::printFriends() const
{
	vector<User*>::const_iterator itr = friends.begin();
	vector<User*>::const_iterator itrEnd = friends.end();
	int count = 0;
	if (friends.size() != 0)
		cout << "" << name << "'s friends list:" << endl;
	for (; itr != itrEnd; ++itr)
	{
		count++;
		cout << "" << count << ". " << (*itr)->name << endl;
	}
}

void User::printStatusesOfUser() const
{
	vector<Status*>::const_iterator itr = my_statuses.begin();
	vector<Status*>::const_iterator itrEnd = my_statuses.end();
	int count = 0;
	if (this->my_statuses.size() != 0)
		cout << "" << name << "'s statuses:" << endl;
	for (; itr != itrEnd; ++itr)
	{
		count++;
		cout << "" << count << ". ";
		(*itr)->printStatus();
	}
}

void User::printFriendsStatuses() const
{
	vector<User*>::const_iterator itr = friends.begin();
	vector<User*>::const_iterator itrEnd = friends.end();

	cout << "" << name << "'s 10 last friends statuses:" << endl;
	for (; itr != itrEnd; ++itr)
	{
		cout << (*itr)->getName() << "'s statuses: " << endl;
		vector<Status*>::iterator itr1 = (*itr)->my_statuses.begin();
		vector<Status*>::iterator itrEnd1 = (*itr)->my_statuses.end() - ONE;
		int count = 0;
		for (; ++count <= TEN; --itrEnd1) // Prints the last 10 statuses or less if there are less than 10
		{
			(*itrEnd1)->printStatus();
			if (itr1 == itrEnd1)
				break;
		}
	}
}

void User::printFanPages() const
{
	vector<Page*>::const_iterator itr = fan_pages.begin();
	vector<Page*>::const_iterator itrEnd = fan_pages.end();
	int count = 0;
	if (this->fan_pages.size() != 0)
		cout << "" << name << "'s fan pages:" << endl;
	for (; itr != itrEnd; ++itr)
	{
		count++;
		cout << "" << count << ". " << (*itr)->getName() << endl;
	}
}

void User::saveDataToFile(ofstream& file) const
{
	size_t nameLen = name.size(), numOfStatuses = my_statuses.size();
	file.write((const char*)&nameLen, sizeof(size_t));
	file.write((const char*)name.c_str(), nameLen);
	file.write((const char*)&date_of_birth, sizeof(Date));
	file.write((const char*)&numOfStatuses, sizeof(size_t));
	for (int i = 0; i < numOfStatuses; i++)
	{
		my_statuses[i]->saveDataToFile(file);
		my_statuses[i]->saveExtra(file);
	}
}

bool User::operator>(const Page& page) const
{
	return (friends.size() > page.getNumOfFollowers());
}

const User& User::operator+=(User& user)
{
	addFriend(user);
	return *this;
}

bool User::operator>(const User& user) const
{
	return (friends.size() > user.getNumOfFriends());
}