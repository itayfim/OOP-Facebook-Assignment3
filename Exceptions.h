#ifndef __EXCEPTIONS_H
#define __EXCEPTIONS_H

#include "Header.h"

class FacebookException : public std::exception
{
public:
	virtual const char* what() const override { return "An error has been occurred in the facebook system, please try again.\n"; }
};

class UserOrPageExist : public FacebookException
{
private:
	std::string name;
public:
	UserOrPageExist(std::string name) : name(name) {}
	virtual std::string myWhat() const
	{
		return (name + " allready exsist in the system, please try again.\n");
	}
};

class UserOrPageNotExist : public FacebookException
{
private:
	std::string name;
public:
	UserOrPageNotExist(std::string name) : name(name) {}
	virtual std::string myWhat() const
	{
		return (name + " does not exsist in the system, please try again.\n");
	}
};

class LessThan2Users : public FacebookException
{
public:
	virtual const char* what() const override { return "There are less than 2 users in the system, cannot excecute this order, please try again.\n"; }
};

class ZeroUsers : public FacebookException
{
public:
	virtual const char* what() const override { return "There are no users in the system, cannot excecute this order, please try again.\n"; }
};

class ZeroPages : public FacebookException
{
public:
	virtual const char* what() const override { return "There are no Pages in the system, cannot excecute this order, please try again.\n"; }
};

class UserException : public std::exception
{
public:
	virtual const char* what() const override { return "An error has been occurred in User class, please try again.\n"; }
};

class SelfFriend : public UserException
{
public:
	virtual const char* what() const override { return "A user cannot be his own friend, please try again.\n"; }
};

class DateException : public UserException
{
public:
	virtual const char* what() const override { return "Wrong date format input, please try again.\n"; }
};

class StatusObjectType : public std::exception
{
public:
	virtual const char* what() const override { return "Cannot outhorize status object type, please try again.\n"; }
};

#endif





