//============================================================================
// Name        : LinkedList.cpp
// Author      : James Richmond
// Course      : CS-260-J5337 Data Structures and Algorithms 20EW5
// Date        : 5/16/2020
// Project Num : 3-2
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Lab 3-3 Lists and Searching
//============================================================================

//TODO: Check for test code
//TODO: Cleanup comments
//TODO: fix div/0 in HT?
//TODO: Check switches/ifs
//TODO: Security
//TODO: Enable custom search


#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
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

		// default constructor
		Node() {
			next = nullptr;
		}

		// initialize a node with a bid
		Node(Bid aBid) {
			bid = aBid;
			next = nullptr;
		}
	};

	Node* head;
	Node* tail;
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

/**
 * Default constructor
 */
LinkedList::LinkedList() {
	// FIXME (2): Initialize housekeeping variables
	/*
	 * JR: Initializes head and tail as null pointers until list is populated with nodes
	 */
	head = tail = nullptr;
}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
}

/**
 * Append a new bid to the end of the list
 */
void LinkedList::Append(Bid bid) {
	// FIXME (3): Implement append logic
	/*
	 * JR: Creates pointer variable node of type Node, initialized as a new Node (in the heap)
	 * The first if statement determines if the node being appended is the first node, and sets the node being added as the head if so
	 * The if statement nested in the else statement checks to see if a tail node exists, and if so, assigns the next pointer of what was previously the tail to the new node
	 */
	Node* node = new Node(bid);

	if (head == nullptr) {
		head = node;
	} else {
		if (tail != nullptr) {
			tail->next = node;
		}
	}

	// new node is  now the tail
	tail = node;

	++size;
}

/**
 * Prepend a new bid to the start of the list
 */
void LinkedList::Prepend(Bid bid) {
	// FIXME (4): Implement prepend logic
	/*
	 * JR: Creates pointer variable node of type Node, initialized as a new Node (in the heap)
	 * If the head pointer is not null (ie if a first node exists), what was previously the head pointer is assigned to the next pointer of the new node
	 * The head pointer then is assigned to point to the new node.
	 */
	Node* node = new Node(bid);

	if (head != nullptr) {
		node->next = head;
	}

	head = node;

	++size;
}

/**
 * Simple output of all bids in the list
 */
void LinkedList::PrintList() {
	// FIXME (5): Implement print logic
	/*
	 * JR: Creates pointer variable curNode, initialized to head
	 * This sets the node pointed to by head as the first to be operated on
	 * The while loop iterates through the nodes until the pointer to the next node is nullptr (indicating there is no next node)
	 * The loop prints the bid variables to the terminal
	 */
	Node* curNode = head;

	while (curNode != nullptr) {
		cout << curNode->bid.bidId << ": " << curNode->bid.title << " | "
				<< curNode->bid.amount << " | " << curNode->bid.fund << endl;
		curNode = curNode->next;
	}
}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void LinkedList::Remove(string bidId) {
	// FIXME (6): Implement remove logic
	/*
	 * JR: If node to be removed is pointed to by the list head (ie the first node), creates tempNode to hold a pointer to the second node
	 * First node is then deleted, and the head pointer is set to the second node.
	 * If node to be removed is not the first node, a while loop iterates through nodes until the node containing the bid with the matching ID is found (in the node after the current node)
	 * When the matching node is found, tempNode is set to the matching node, and then the next pointer for tempNode is assigned to the next pointer for the currently iterated node.
	 */
	if (head != nullptr) {
		if (head->bid.bidId.compare(bidId) == 0) {

			Node* tempNode = head->next;
			delete head;
			head = tempNode;

		}
	}

	Node* curNode = head;

	while (curNode->next != nullptr) {
		if (curNode->next->bid.bidId.compare(bidId) == 0) {
			Node* tempNode = curNode->next;

			curNode->next = tempNode->next;

			delete tempNode;

			--size;

			return;
		}
		curNode = curNode->next;
	}
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid LinkedList::Search(string bidId) {
	// FIXME (7): Implement search logic
	/*
	 * JR: Iterates over nodes similarly to print, append, prepend methods
	 * Compares bidId given as argument to bidId of each bid in each node
	 * When a match is found, returns bid containing matching bidId
	 * Returns an empty bid object if the matching key is not found.
	 */
	Node* curNode = head;

	while (curNode != nullptr) {
		if (curNode->bid.bidId.compare(bidId) == 0) {
			return curNode->bid;
		}
		curNode = curNode->next;
	}
	Bid emptyBid;
	return emptyBid;
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
	return size;
}

//============================================================================
// Static methods used for testing
//============================================================================

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
		// loop to read rows of a CSV file
		for (unsigned int i = 0; i < file.rowCount(); i++) {

			// initialize a bid using data from current row (i)
			Bid bid;
			bid.bidId = file[i][1];
			bid.title = file[i][0];
			bid.fund = file[i][8];
			bid.amount = strToDouble(file[i][4], '$');

			//cout << bid.bidId << ": " << bid.title << " | " << bid.fund << " | " << bid.amount << endl;

			// add this bid to the end
			list->Append(bid);
		}
	} catch (csv::Error &e) {
		std::cerr << e.what() << std::endl;
	}
}

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

/**
 * The one and only main() method
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
 */
int main(int argc, char *argv[]) {

	// process command line arguments
	string csvPath, bidKey;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		bidKey = "98109";
		break;
	case 3:
		csvPath = argv[1];
		bidKey = argv[2];
		break;
	default:
		csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
		bidKey = "98109";
	}

	clock_t ticks;

	LinkedList bidList;

	Bid bid;

	int choice = 0;
	while (choice != 9) {
		cout << "Menu:" << endl;
		cout << "  1. Enter a Bid (Append)" << endl;
		cout << "  2. Load Bids" << endl;
		cout << "  3. Display All Bids" << endl;
		cout << "  4. Find Bid" << endl;
		cout << "  5. Remove Bid" << endl;
		cout << "  6. Enter a Bid (Prepend)" << endl;
		cout << "  9. Exit" << endl;
		cout << "Enter choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			bid = getBid();
			bidList.Append(bid);
			displayBid(bid);

			break;

		case 2:
			ticks = clock();

			loadBids(csvPath, &bidList);

			cout << bidList.Size() << " bids read" << endl;

			ticks = clock() - ticks; // current clock ticks minus starting clock ticks
			cout << "time: " << ticks << " milliseconds" << endl;
			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds"
					<< endl;

			break;

		case 3:
			bidList.PrintList();

			break;

		case 4:
			ticks = clock();

			bid = bidList.Search(bidKey);

			ticks = clock() - ticks; // current clock ticks minus starting clock ticks

			if (!bid.bidId.empty()) {
				displayBid(bid);
			} else {
				cout << "Bid Id " << bidKey << " not found." << endl;
			}

			cout << "time: " << ticks << " clock ticks" << endl;
			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds"
					<< endl;

			break;

		case 5:
			bidList.Remove(bidKey);

			break;

		case 6:
			bid = getBid();
			bidList.Prepend(bid);
			displayBid(bid);

			break;

		}
	}

	cout << "Good bye." << endl;

	return 0;
}
