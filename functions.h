#ifndef __FUNCTIONS_H
#define	__FUNCTIONS_H

#include "Header.h"

class Facebook;

void printMenu(); // Prints the list of actions that can be performed in the Facebook system
void executeChoice(int choice, Facebook& system); // Matches between the selected choice in the menu and the function
void addFriendToTheSystem(Facebook& system); // Adding a new friend to the Facebook system
void addFanPageToTheSystem(Facebook& system); // Adding a fan page to the Facebook system
void addStatus(Facebook& system); // Adding a status to a friend or fan page
void addStatusToFriend(Facebook& system); // Adding a status to a friend 
void addStatusToFanPage(Facebook& system); // Adding a status to a fan page
void printStatus(Facebook& system); // Prints statues of a user or a fan page
void printFriendStatuses(Facebook& system); // Prints all the statuses of the selected friend
void printFanPageStatuses(Facebook& system); // Prints all the statuses of the selected fan page
void print10LastStatuses(Facebook& system); // Prints the last ten statuses of all friends of a specific friend
void printEntities(Facebook& system); // Prints all entities of the Facebook system, friends and fan pages
void connectTwoFriends(Facebook& system); // Connect between two friends, where each will be part of the other's friends list
int takeInput(Facebook& system, int num, bool connect); // Asks the user to select a friend to perform an action 
void disconnectTwoFriends(Facebook& system); // Disconnect between two friends, where each will be deleted from the other's friends list
void addFanToFanPage(Facebook& system); // Adding a friend to a fan page while the fan page is added to the friend's list of pages
void deleteFanFromFanPage(Facebook& system); // Deleting a friend from a fan page while the fan page is deleted from the friend's list of pages
int takePageInput(Facebook& system, bool add); // Asks the user to select a fan page to perform an action 
void initSystem(Facebook& system); // Initializing the Facebook system with friends , pages and statuses
void printFollowers(Facebook& system); // Printing all the friends of a certain friend or fan page
void printFriendFollowers(Facebook& system); // Printing all the friends of a certain friend 
void printPageFollowers(Facebook& system); // Printing all the friends of a certain fan page
void newTerminate(); // implement of our terminate function

#endif
