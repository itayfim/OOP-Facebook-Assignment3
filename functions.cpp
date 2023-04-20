#include "functions.h"
#include "Facebook.h"
#include "status.h"
#include "page.h"
#include "Exceptions.h"

#define TEXT 1
#define IMAGE 2
#define VIDEO 3
using namespace std;

void printMenu()
{
	cout << "What would you like to do? " << endl;
	cout << "1. add a friend to the system" << endl;
	cout << "2. add a fan page to the system" << endl;
	cout << "3. add status to a friend / fan page" << endl;
	cout << "4. print all statuses off a friend / fan page" << endl;
	cout << "5. print 10 last statuses of each friend of a specific user" << endl;
	cout << "6. connect 2 friends" << endl;
	cout << "7. cancel connection of 2 friends" << endl;
	cout << "8. add a friend to a fan page" << endl;
	cout << "9. remove a friend from a fan page" << endl;
	cout << "10. print all entities in the system" << endl;
	cout << "11. print all frineds of a specific user / all the followers of a specific fan page" << endl;
	cout << "12. exit" << endl;
}

void executeChoice(int choice, Facebook& system)
{
	switch (choice)
	{
	case ONE:
		addFriendToTheSystem(system);
		break;
	case TWO:
		addFanPageToTheSystem(system);
		break;
	case THREE:
		addStatus(system);
		break;
	case FOUR:
		printStatus(system);
		break;
	case FIVE:
		print10LastStatuses(system);
		break;
	case SIX:
		connectTwoFriends(system);
		break;
	case SEVEN:
		disconnectTwoFriends(system);
		break;
	case EIGHT:
		addFanToFanPage(system);
		break;
	case NINE:
		deleteFanFromFanPage(system);
		break;
	case TEN:
		printEntities(system);
		break;
	case ELEVEN:
		printFollowers(system);
		break;
	case TWELVE:
		break;
	default:
		throw invalid_argument("Wrong input, please enter a valid number between 1-12.\n");
		break;
	}
}

void initSystem(Facebook& system) // hard-coded entities:
{
	User u1("Shani", { 3,1,2001 });
	User u2("Itay", { 4,1,1994 });
	User u3("Liav", { 3,7,1999 });
	system.addFriend(u1); system.addFriend(u2); system.addFriend(u3);
	system.getUser(0)->addFriend(*(system.getUser(1)));
	system.getUser(0)->addFriend(*(system.getUser(2)));
	system.getUser(1)->addFriend(*(system.getUser(2)));
	Page p1("MTA");
	Page p2("Tel Aviv");
	Page p3("Even Yehuda");
	system.addPage(p1); system.addPage(p2); system.addPage(p3);
	system.getPage(0)->addFollower(*(system.getUser(0)));
	system.getPage(1)->addFollower(*(system.getUser(1)));
	system.getPage(2)->addFollower(*(system.getUser(0)));
	Status s1("Hi, my name is Shani and I live in Even Yehuda");
	Status s2("I like to eat salads");
	system.getUser(0)->addStatusForUser(s1); system.getUser(0)->addStatusForUser(s2);
	Status s3("Hi, my name is Itay and I live in Tel Aviv");
	StatusVideo s4("I like to eat Happy-Hippo's", "videosStatus");
	system.getUser(1)->addStatusForUser(s3); system.getUser(1)->addStatusForUser(s4);
	Status s5("Hi, my name is Liav and I live in Oranit");
	StatusImage s6("I like to eat a lot of scarambled eggs", "imageStatus");
	system.getUser(2)->addStatusForUser(s5); system.getUser(2)->addStatusForUser(s6);
	Status s7("Important notice: there will be no classes in light of the beginning of the year happening");
	Status s8("Reminder: Please fill out the teaching survey");
	system.getPage(0)->addStatusForPage(s7); system.getPage(0)->addStatusForPage(s8);
	Status s9("Ayalon routes will be closed in light of the White Night events");
	StatusVideo s10("The sale has opened for discounted tours in the Carmel Market for the residents of the city", "videosStatus");
	system.getPage(1)->addStatusForPage(s9); system.getPage(1)->addStatusForPage(s10);
	Status s11("A cat named Mitzi has been lost, anyone who comes across him - please contact Liora from the council");
	StatusImage s12("Agricultural land in Evan Yehuda, please respond if you are interested","imageStatus");
	system.getPage(2)->addStatusForPage(s11); system.getPage(0)->addStatusForPage(s12);
}

void printFollowers(Facebook& system)
{
	int input;
	cout << "Select one of the followings:" << endl << "1. print all friends of a specific friend" << endl <<
		"2. print all followers of a fan page" << endl;
	cin >> input;
	cin.ignore();
	bool goodInput = false;
	do
	{
		if (input == ONE)
		{
			printFriendFollowers(system);
			goodInput = true;
		}
		else if (input == TWO)
		{
			printPageFollowers(system);
			goodInput = true;
		}
		else
		{
			cout << "Please enter 1 or 2" << endl;
			cin >> input;
			cin.ignore();
		}
	} while (!goodInput);
}

void printFriendFollowers(Facebook& system)
{
	int index;
	cout << "Please enter the user's name whose friends you would like to see: " << endl;
	string name;
	getline(cin, name);
	index = system.searchUserByName(name);
	system.getUser(index)->printFriends();
}

void printPageFollowers(Facebook& system)
{
	int index;
	cout << "Please enter the fan page's name whose followers you would like to see: " << endl;
	string name;
	getline(cin, name);
	index = system.searchPageByName(name);
	system.getPage(index)->printFollowers();
}

void addFanToFanPage(Facebook& system)
{
	int indexUser, indexPage;
	if (system.getNumOfUsers() < ONE)  // Cannot connect user to fan page if there are no users or fan pages 
		throw ZeroUsers();
	else if (system.getNumOfPages() < ONE)
		throw ZeroPages();
	indexUser = takeInput(system, ONE, true); // The user selects a fan to add to the fan page
	indexPage = takePageInput(system, true); // The user chooses which fan page the fan will be added to
	system.getPage(indexPage)->addFollower(*(system.getUser(indexUser)));
}

void deleteFanFromFanPage(Facebook& system)
{
	int indexUser, indexPage;
	if (system.getNumOfUsers() < ONE)  // Cannot connect user to fan page if there are no users or fan pages 
		throw ZeroUsers();
	else if (system.getNumOfPages() < ONE)
		throw ZeroPages();
	indexUser = takeInput(system, ONE, false); // The user selects a fan to delete from the fan page
	indexPage = takePageInput(system, false); //The user chooses which fan page the fan will be deleted from
	system.getPage(indexPage)->deleteFollower(*(system.getUser(indexUser)));
}

int takePageInput(Facebook& system, bool add)
{
	int res;
	string name;
	cout << "Please enter the fan pages's name that the fan will be " << (add ? "added to" : "removed from") << endl;
	getline(cin, name);
	res = system.searchPageByName(name);
	return res; // The index in the array of the selected fan page
}

void disconnectTwoFriends(Facebook& system)
{
	int index1, index2;
	if (system.getNumOfUsers() < TWO) // Cannot disconnect 1 or 0 users 
		throw LessThan2Users();
	index1 = takeInput(system, ONE, false); // The user selects the first friend to disconnect
	index2 = takeInput(system, TWO, false); // The user selects the second friend to disconnect
	system.getUser(index1)->deleteFriend(*(system.getUser(index2)));
}

void connectTwoFriends(Facebook& system)
{
	int index1, index2;
	if (system.getNumOfUsers() < TWO) // Cannot connect 1 or 0 users 
		throw LessThan2Users();
	index1 = takeInput(system, ONE, true); // The user selects the first friend to connect
	index2 = takeInput(system, TWO, true); // The user selects the second friend to connect
	system.getUser(index1)->addFriend(*(system.getUser(index2)));
}

int takeInput(Facebook& system, int num, bool connect)
{
	int res;
	string name;
	cout << "Please enter the " << (num == ONE ? "1st" : "2nd") << " member's name that you want to " << (connect ? "connect" : "disconnect") << endl;
	getline(cin, name);
	res = system.searchUserByName(name);
	return res; // The index in the array of the selected User
}

void printEntities(Facebook& system)
{
	cout << "Printing all the entities in the system: " << endl;
	cout << "Printing all users: " << endl;
	system.printUsers();
	cout << "Printing all pages: " << endl;
	system.printPages();
}

void print10LastStatuses(Facebook& system)
{
	int index;
	cout << "Please enter the user's name you would like to see the 10 most recent statuses of each of its friends: " << endl;
	string name;
	getline(cin, name);
	index = system.searchUserByName(name);
	system.getUser(index)->printFriendsStatuses();
}

void printStatus(Facebook& system)
{
	int input;
	cout << "Select one of the followings:" << endl << "1. print statuses of a friend" << endl << "2. print statuses of a fan page" << endl;
	cin >> input;
	cin.ignore();
	bool goodInput = false;
	do {
		if (input == ONE)
		{
			printFriendStatuses(system);
			goodInput = true;
		}
		else if (input == TWO)
		{
			printFanPageStatuses(system);
			goodInput = true;
		}
		else
		{
			cout << "Please enter 1 or 2" << endl;
			cin >> input;
			cin.ignore();
		}
	} while (!goodInput);
}

void printFriendStatuses(Facebook& system)
{
	int index;
	cout << "Please enter the user's name whose statuses you would like to see: " << endl;
	string name;
	getline(cin, name);
	index = system.searchUserByName(name);
	system.getUser(index)->printStatusesOfUser();
}

void printFanPageStatuses(Facebook& system)
{
	int index;
	cout << "Please enter the fan page's name whose statuses you would like to see: " << endl;
	string name;
	getline(cin, name);
	index = system.searchPageByName(name);
	system.getPage(index)->printStatusesOfPage();
}

void addStatus(Facebook& system)
{
	int input;
	cout << "Select one of the followings:" << endl << "1. add status to a friend" << endl << "2. add status to a fan page" << endl;
	cin >> input;
	cin.ignore();
	bool goodInput = false;
	do
	{
		if (input == ONE)
		{
			addStatusToFriend(system);
			goodInput = true;
		}
		else if (input == TWO)
		{
			addStatusToFanPage(system);
			goodInput = true;
		}
		else
		{
			cout << "Please enter 1 or 2" << endl;
			cin >> input;
			cin.ignore();
		}

	} while (!goodInput);
}

void addStatusToFanPage(Facebook& system)
{
	string newTextStatus, name, file_name;
	int index, input;
	cout << "Please enter the fan page's name to which you would like to add a status: " << endl;
	getline(cin, name);
	index = system.searchPageByName(name);
	cout << "Which status do you want to add?\n1. Text status\n2. Image status\n3. Video status\n";
	cin >> input;
	cin.ignore();
	cout << "Write your status here: " << endl;
	getline(cin, newTextStatus);
	switch (input)
	{
	case TEXT: system.getPage(index)->addStatusForPage(Status(newTextStatus)); break;
	case IMAGE:
	{
		cout << "Write name of image file" << endl;
		getline(cin, file_name);
		system.getPage(index)->addStatusForPage(StatusImage(newTextStatus,file_name));
	}
	break;
	case VIDEO:
	{
		cout << "Write name of video file" << endl;
		getline(cin, file_name);
		system.getPage(index)->addStatusForPage(StatusVideo(newTextStatus,file_name));
	}
	break;
	default: throw invalid_argument("incorrect input, please try again");
	}
}

void addStatusToFriend(Facebook& system)
{
	string newTextStatus, name, file_name;
	int index, input;
	cout << "Please enter the user's name to which you would like to add a status: " << endl;
	getline(cin, name);
	index = system.searchUserByName(name);
	cout << "Which status do you want to add?\n1. Text status\n2. Image status\n3. Video status\n";
	cin >> input;
	if (input > VIDEO || input < TEXT)
		throw invalid_argument("incorrect input, please enter a number between 1-3\n");
	cin.ignore();
	cout << "Write your status here: " << endl;
	getline(cin, newTextStatus);
	switch (input)
	{
	case TEXT:
		system.getUser(index)->addStatusForUser(Status(newTextStatus));
		break;
	case IMAGE:
	{
		cout << "Write name of image file" << endl;
		getline(cin, file_name);
		system.getUser(index)->addStatusForUser(StatusImage(newTextStatus, file_name));
	}
	break;
	case VIDEO:
	{
		cout << "Write name of video file" << endl;
		getline(cin, file_name);
		system.getUser(index)->addStatusForUser(StatusVideo(newTextStatus,file_name));
	}
	break;
	default: throw invalid_argument("incorrect input, please try again\n"); break;
	}
}

void addFriendToTheSystem(Facebook& system)
{
	int index;
	string name;
	char birthday[MAX_NAME_LEN];
	char seps[] = "\/."; // Separators of the date input
	Date date_of_birth;
	index = VALID;
	cout << "Please enter the friend's name: " << endl;
	getline(cin, name);
	cout << "Please enter the friend's birthday (by the format of dd.mm.yyyy): " << endl;
	cin.ignore();
	cin.getline(birthday, MAX_NAME_LEN);
	date_of_birth.day = atoi(strtok(birthday, seps));
	date_of_birth.month = atoi(strtok(NULL, seps));
	date_of_birth.year = atoi(strtok(NULL, seps));
	User newUser(name, date_of_birth);
	system.addFriend(newUser);
}

void addFanPageToTheSystem(Facebook& system)
{
	int index;
	string name;
	index = VALID;
	cout << "Please enter the page's name: " << endl;
	getline(cin, name);
	Page newPage(name); // Create a new fan page
	system.addPage(newPage);
}

void newTerminate()
{
	cout << "A general error has been occurred, exiting the program." << endl;
	exit(1);
}