/*
 * Utilizes a CSV Parser library to import records into a Linked List data structure
 * Demonstrates Append, Prepend, Remove, Search functionality
 */

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
	str.erase(remove(str.begin(), str.end(), ch), str.end());
	return atof(str.c_str());
}

// A structure to hold bid information
struct Bid {
	string bidId; // unique identifier
	string title;
	string fund;
	double amount;
	Bid() {
		amount = 0.0;
	}
};

class LinkedList {

private:
	// Defines node in singly-linked list, consisting of bid object and next pointer
	struct Node {
		Bid bid;
		Node *next;

		Node() {
			next = nullptr;
		}

		// initialize a node with a bid
		Node(Bid aBid) {
			bid = aBid;
			next = nullptr;
		}
	};

	Node *head;
	Node *tail;
	int size = 0;

public:
	LinkedList();
	virtual ~LinkedList();
	void Append(Bid bid);
	void Prepend(Bid bid);
	void PrintList();
	void Remove(string bidId);
	Bid Search(string bidId);
	int Size();
};

LinkedList::LinkedList() {
	// Initialize head and tail as null pointers until list is populated with nodes
	head = tail = nullptr;
}

LinkedList::~LinkedList() {
}

// Append a new bid to the end of the list
void LinkedList::Append(Bid bid) {
	Node *newNode = new Node(bid);
	// If first node, set as head node
	if (head == nullptr) {
		head = newNode;

	} else {
		//check to see if a tail node exists, and if so, assigns the next pointer of what was previously the tail to the new node
		if (tail != nullptr) {
			tail->next = newNode;
		}
	}

	// new node is  now the tail
	tail = newNode;

	++size;
}

// Prepend a new bid to the start of the list
void LinkedList::Prepend(Bid bid) {
	Node *newNode = new Node(bid);

	// if a head node exists, set new node next ptr to current head
	if (head != nullptr) {
		newNode->next = head;
	}

	head = newNode;

	++size;
}

// Print all bid objects to console
void LinkedList::PrintList() {

	// Forward declaration
	void displayBid(Bid bid);

	// Start at head node
	Node *curNode = head;

	// Iterate until next ptr is nullptr (ie through whole linked list)
	while (curNode != nullptr) {

		displayBid(curNode->bid);

		// Continue iterating
		curNode = curNode->next;
	}
}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void LinkedList::Remove(string bidId) {
	// Node to be deleted is head node
	if (head != nullptr) {
		if (head->bid.bidId.compare(bidId) == 0) {

			// transfer pointers
			Node *tempNode = head->next;
			delete head;
			head = tempNode;
		}
	}

	Node *curNode = head;

	// Iterate the whole linked list
	while (curNode->next != nullptr) {

		// Sought bid is found in next ptr
		if (curNode->next->bid.bidId.compare(bidId) == 0) {

			// transfer pointers to skip node
			Node *tempNode = curNode->next;
			curNode->next = tempNode->next;

			delete tempNode;

			--size;
			return;
		}

		// Continue iterating
		curNode = curNode->next;
	}
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid LinkedList::Search(string bidId) {

	Node *curNode = head;

	// Iterate the whole linked list
	while (curNode != nullptr) {

		// Sought bid found
		if (curNode->bid.bidId.compare(bidId) == 0) {
			return curNode->bid;
		}

		// Continue iterating
		curNode = curNode->next;
	}

	// Return empty object if sought bid not found
	Bid emptyBid;
	return emptyBid;
}

// Returns the current size (number of elements) in the list
int LinkedList::Size() {
	return size;
}

/**
 * Display the bid information
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
	cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
			<< bid.fund << endl;
	return;
}

/**
 * Prompt user for bid information
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
	Bid bid;

	cout << "Enter Id: ";
	cin.ignore();
	getline(cin, bid.bidId);

	cout << "Enter title: ";
	getline(cin, bid.title);

	cout << "Enter fund: ";
	cin >> bid.fund;

	cout << "Enter amount: ";
	cin.ignore();
	string strAmount;
	getline(cin, strAmount);
	bid.amount = strToDouble(strAmount, '$');

	return bid;
}

/**
 * Load a CSV file containing bids into a LinkedList
 *
 * @return a LinkedList containing all the bids read
 */
void loadBids(string csvPath, LinkedList *list) {
	cout << "Loading CSV file " << csvPath << endl;

	// initialize the CSV Parser
	csv::Parser file = csv::Parser(csvPath);

	try {
		// Read rows of a CSV file
		for (unsigned int i = 0; i < file.rowCount(); i++) {

			// initialize a bid using data from current row (i)
			Bid bid;
			bid.bidId = file[i][1];
			bid.title = file[i][0];
			bid.fund = file[i][8];
			bid.amount = strToDouble(file[i][4], '$');

			// add this bid to the end
			list->Append(bid);
		}

	} catch (csv::Error &e) {
		std::cerr << e.what() << std::endl;
	}
}

string getBidId() {

	string bidKey;
	cout << "Enter a bid ID" << endl;
	cin.ignore();
	getline(cin, bidKey);

	return bidKey;
}

void displayTime(clock_t ticks) {
	ticks = clock() - ticks; // current clock ticks minus starting clock ticks
	cout << "time: " << ticks << " ticks" << endl;
	cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
}

/**
 * Main
 *
 * @param arg[1] path to CSV file to load from (optional)
 */
int main(int argc, char *argv[]) {

	// process command line arguments
	string csvPath, bidKey;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;
	default:
		csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
	}

	clock_t ticks;

	LinkedList bidList;

	Bid bid;

	// Load bid objects from CSV file
	ticks = clock();
	loadBids(csvPath, &bidList);
	cout << bidList.Size() << " bids read" << endl;
	ticks = clock() - ticks; // current clock ticks minus starting clock ticks
	displayTime(ticks);

	int choice = 0;
	while (choice != 9) {
		cout << "Menu:" << endl;
		cout << "  1. Display All Bids" << endl;
		cout << "  2. Enter a Bid (Append)" << endl;
		cout << "  3. Enter a Bid (Prepend)" << endl;
		cout << "  4. Find Bid" << endl;
		cout << "  5. Remove Bid" << endl;
		cout << "  9. Exit" << endl;
		cout << "Enter choice: ";

		// Prompt for selection - check input
		while (!(cin >> choice)) {
			cout << "Invalid choice, please try again" << endl;
			cin.clear();
			cin.ignore();
		}

		switch (choice) {

		case 1:
			bidList.PrintList();

			break;

		case 2:
			bid = getBid();
			bidList.Append(bid);
			displayBid(bid);

			break;

		case 3:
			bid = getBid();
			bidList.Prepend(bid);
			displayBid(bid);

			break;

		case 4:
			ticks = clock();

			// Prompt user for a bid ID and search
			bidKey = getBidId();
			bid = bidList.Search(bidKey);

			ticks = clock() - ticks; // current clock ticks minus starting clock ticks

			if (!bid.bidId.empty()) {
				displayBid(bid);
			} else {
				cout << "Bid Id " << bidKey << " not found." << endl;
			}

			displayTime(ticks);

			break;

		case 5:
			// Prompt user for a bid ID and remove
			bidKey = getBidId();
			bidList.Remove(bidKey);

			break;

		case 6:
			bid = getBid();
			bidList.Prepend(bid);
			displayBid(bid);

			break;

		case 9:
			break;

		default:
			cout << "Not a valid selection.";

			break;
		}
	}

	cout << "Good bye." << endl;

	return 0;
}
