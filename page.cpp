#include "page.h"
#include "status.h"
using namespace std;

Page::Page(const Page& other)
{
	for (int i = 0; i < other.page_statuses.size(); i++)
	{
		addStatusForPage(*(other.page_statuses[i]));
	}
	name = other.name;
	followers = other.followers;
}

Page::Page(istream& in)
{
	size_t nameLen, numOfStatuses; int len;
	in.read((char*)&nameLen, sizeof(size_t));
	char* tempName = new char[nameLen + 1];
	in.read((char*)tempName, nameLen);
	tempName[nameLen] = '\0';
	name = string(tempName);
	delete[] tempName;
	in.read((char*)&numOfStatuses, sizeof(size_t));
	for (int i = 0; i < numOfStatuses; i++)
	{
		in.read((char*)&len, sizeof(int));
		tempName = new char[len];
		in.read(tempName, len);
		if (!strcmp(tempName, typeid(Status).name() + SIX))
			addStatusForPage(Status(in));
		else if (!strcmp(tempName, typeid(StatusImage).name() + SIX))
			addStatusForPage(StatusImage(in));
		else
			addStatusForPage(StatusVideo(in));
		delete[]tempName;
	}
}

Page::~Page()
{
	vector<Status*>::iterator itr = page_statuses.begin();
	vector<Status*>::iterator endItr = page_statuses.end();
	for (; itr != endItr; ++itr)
		delete* itr;
}

void Page::addFollower(User& new_follower)
{
	bool founded = false;
	vector<User*>::iterator itr = followers.begin();
	vector<User*>::iterator endItr = followers.end();
	for (; itr != endItr && !founded; ++itr) // Check if the new follower is already following the fan page
	{
		if ((*itr)->getName() == new_follower.getName())
			founded = true;
	}
	if (!founded) // The new follower isn't in the list of followers
	{
		followers.push_back(&new_follower);
		new_follower.addPage(*this); // Mutual action, the new follower will add the fan page to his list of pages
	}
}

void Page::deleteFollower(User& old_follower)
{
	vector<User*>::iterator itr = followers.begin();
	vector<User*>::iterator endItr = followers.end();
	for (; itr != endItr; ++itr) // Check if the new follower is in the list of followers
	{
		if ((*itr)->getName() == old_follower.getName())
		{
			followers.erase(itr);
			old_follower.deletePage(*this); // Mutual action, the old follower delete the fan page from his list of pages
			break;
		}
	}
}

void Page::addStatusForPage(const Status& new_status)
{
	const Status* text = dynamic_cast<const Status*>(&new_status);
	const StatusImage* image = dynamic_cast<const StatusImage*>(&new_status);
	const StatusVideo* video = dynamic_cast<const StatusVideo*>(&new_status);
	if (image)
	{
		Status* status = new StatusImage(*image);
		page_statuses.push_back(status);
	}
	else if (video)
	{
		Status* status = new StatusVideo(*video);
		page_statuses.push_back(status);
	}
	else if (text)
	{
		Status* status = new Status(new_status);
		page_statuses.push_back(status);
	}
	else
		throw StatusObjectType();
}

void Page::printFollowers() const
{
	int i = 0;
	vector<User*>::const_iterator itr = followers.begin();
	vector<User*>::const_iterator endItr = followers.end();
	for (; itr != endItr; ++itr)
		cout << ++i << ". " << (*itr)->getName() << endl;
}

void Page::printStatusesOfPage() const
{
	int i = 0;
	vector<Status*>::const_iterator itr = page_statuses.begin();
	vector<Status*>::const_iterator endItr = page_statuses.end();
	if (page_statuses.size() != 0)
		cout << "" << name << "'s statuses:" << endl;
	for (; itr != endItr; ++itr)
	{
		cout << ++i << ". ";
		(*itr)->printStatus();
	}
}

void Page::saveDataToFile(std::ostream& file) const
{
	string followerName;
	size_t nameLen = name.size(), numOfFollowers = followers.size(), statusesSize = page_statuses.size(), followerNameLen;
	file.write((const char*)&nameLen, sizeof(size_t));
	file.write((const char*)name.c_str(), nameLen);
	file.write((const char*)&statusesSize, sizeof(size_t));
	for (int i = 0; i < statusesSize; i++)
	{
		page_statuses[i]->saveDataToFile(file);
		page_statuses[i]->saveExtra(file);
	}
	file.write((const char*)&numOfFollowers, sizeof(size_t));
	for (int i = 0; i < numOfFollowers; i++)
	{
		followerName = followers[i]->getName();
		followerNameLen = followerName.size();
		file.write((const char*)&followerNameLen, sizeof(size_t));
		file.write((const char*)followerName.c_str(), followerNameLen);
	}
}

const Page& Page::operator+=(User& follower)
{
	addFollower(follower);
	return *this;
}

bool Page::operator>(const Page& other) const
{
	return (this->followers.size() > other.followers.size());
}

bool Page::operator>(const User& user)
{
	return (this->followers.size() > user.getNumOfFriends());
}