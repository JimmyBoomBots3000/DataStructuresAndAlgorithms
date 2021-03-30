/*
 * Utilizes a CSV Parser library to import records into a Hash Table data structure
 * Demonstrates Insert, Remove, Search functionality
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <string>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

const unsigned int DEFAULT_SIZE = 179;

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

class HashTable {

private:
	// Node for hash table/linked list for collisions
	struct Node {
		Bid bid;
		unsigned key;
		Node *next;

		// default constructor
		Node() {
			key = UINT_MAX;
			next = nullptr;
		}

		// initialize with bid
		Node(Bid aBid) :
				Node() {
			bid = aBid;
		}

		// initialize with a bid and key
		Node(Bid aBid, unsigned aKey) :
				Node(aBid) {
			key = aKey;
		}
	};

	vector<Node> nodes;

	unsigned tableSize = DEFAULT_SIZE;

	unsigned int Hash(int key);

public:
	HashTable();
	HashTable(unsigned size);
	virtual ~HashTable();
	void Insert(Bid bid);
	void PrintAll();
	void Remove(string bidId);
	Bid Search(string bidId);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
	nodes.resize(tableSize);
}

HashTable::HashTable(unsigned size) {
	this->tableSize = size;
	nodes.resize(tableSize);
}

/**
 * Destructor
 */
HashTable::~HashTable() {
	// Free storage when class is destroyed
	nodes.erase(nodes.begin());
}

/**
 * Calculate the hash value of a given key.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::Hash(int key) {

	return key % tableSize;
}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {

	// calculate the key for this bid
	unsigned key = Hash(stoi(bid.bidId));

	// try to retrieve node using the key
	Node *oldNode = &(nodes.at(key));

	//if no entry found for this key
	if (oldNode == nullptr) {
		Node *newNode = new Node(bid, key);
		nodes.insert(nodes.begin() + key, (*newNode));

	} else {
		//node found
		if (oldNode->key == UINT_MAX) {
			oldNode->key = key;
			oldNode->bid = bid;
			oldNode->next = nullptr;
		} else {
			// find the next open node (last one)
			while (oldNode->next != nullptr) {
				oldNode = oldNode->next;
			}
			oldNode->next = new Node(bid, key);
		}
	}
}

/**
 * Display the bid information to the console (std::out)
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
	getline(cin, bid.fund);

	cout << "Enter amount: ";
	cin.ignore();
	string strAmount;
	getline(cin, strAmount);
	bid.amount = strToDouble(strAmount, '$');

	return bid;
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {

	cout << "Displaying bids" << endl;

	// initialize loop variables
	Node *node;
	Bid bid;
	unsigned displayCounter = 0;
	bool newKey;

	// iterate through all keys of hash table
	for (unsigned key = 0; key < tableSize; key++) {

		newKey = true;
		node = &(nodes.at(key));
		bid = node->bid;

		// walk linked list to display entries of each key (if any)
		while (node != nullptr && node->key != UINT_MAX) {

			// display 'Key' only for first bid of key, otherwise padding spaces
			if (newKey == true) {
				cout << "Key " << key << " ";
			} else {
				cout << "\t";
			}

			// display bid information
			bid = node->bid;
			displayBid(bid);

			// increment counter
			displayCounter++;

			// steps to next node in current key
			node = node->next;
			newKey = false;
		}
	}

	//display count
	cout << displayCounter << " bids displayed" << endl;
}

// FIXME: Deleting a bid shifts table keys
// FIXME: Deleting a bid throws out-of-range on a subsequent PrintAll()
/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {

	// calculate the key for this bid
	unsigned key = Hash(stoi(bidId));

	//
	nodes.erase(nodes.begin() + key);
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
	Bid bid;

	// calculate the key for this bid
	unsigned key = Hash(atoi(bidId.c_str()));

	// try to retrieve node using the key
	Node *node = &(nodes.at(key));

	// if no entry found
	if (node == nullptr || node->key == UINT_MAX) {
		return bid;
	}

	// if node found that matches key
	if (node != nullptr && node->key != UINT_MAX
			&& node->bid.bidId.compare(bidId) == 0) {
		return node->bid;
	}

	// walk linked list to find match
	while (node != nullptr) {
		if (node->key != UINT_MAX && node->bid.bidId.compare(bidId) == 0) {
			return node->bid;
		}
		node = node->next;
	}

	return bid;
}


/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, HashTable *hashTable) {
	cout << "Loading CSV file " << csvPath << endl;

	// initialize the CSV Parser using the given path
	csv::Parser file = csv::Parser(csvPath);

	// read and display header row - optional
	vector<string> header = file.getHeader();
	for (auto const &c : header) {
		cout << c << " | ";
	}
	cout << "" << endl;

	try {
		// loop to read rows of a CSV file
		for (unsigned int i = 0; i < file.rowCount(); i++) {

			// Create a data structure and add to the collection of bids
			Bid bid;
			bid.bidId = file[i][1];
			bid.title = file[i][0];
			bid.fund = file[i][8];
			bid.amount = strToDouble(file[i][4], '$');

			// push this bid to the end
			hashTable->Insert(bid);
		}
	} catch (csv::Error &e) {
		std::cerr << e.what() << std::endl;
	}
}

void displayTime(clock_t ticks) {
	ticks = clock() - ticks; // current clock ticks minus starting clock ticks
	cout << "time: " << ticks << " ticks" << endl;
	cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
}

string getBidId() {

	string bidKey;
	cout << "Enter a bid ID" << endl;
	cin.ignore();
	getline(cin, bidKey);

	return bidKey;
}

/**
 * The one and only main() method
 */
int main(int argc, char *argv[]) {

	string csvPath, searchValue;

	// process command line arguments
	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;
	default:
		csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
	}

	// Define a timer variable
	clock_t ticks;

	// Define a hash table to hold all the bids
	HashTable *bidTable;

	Bid bid;

	bidTable = new HashTable();

	ticks = clock();
	loadBids(csvPath, bidTable);
	ticks = clock() - ticks; // current clock ticks minus starting clock ticks
	displayTime(ticks);

	int choice = 0;
	while (choice != 9) {
		cout << "Menu:" << endl;
		cout << "  1. Display All Bids" << endl;
		cout << "  2. Enter a Bid" << endl;
		cout << "  3. Find Bid" << endl;
		cout << "  4. Remove Bid" << endl;
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
			ticks = clock() - ticks; // current clock ticks minus starting clock ticks
			bidTable->PrintAll();
			displayTime(ticks);

			break;

		case 2:
			bid = getBid();
			bidTable->Insert(bid);
			displayBid(bid);

			break;


		case 3:
			ticks = clock();
			// Prompt user for a bid ID and search
			searchValue = getBidId();
			bid = bidTable->Search(searchValue);
			ticks = clock() - ticks; // current clock ticks minus starting clock ticks

			// Display bid info if found
			if (!bid.bidId.empty()) {
				displayBid(bid);
			} else {
				cout << "Bid Id " << searchValue << " not found." << endl;
			}

			displayTime(ticks);

			break;

		case 4:
			// Prompt user for a bid ID and remove
			searchValue = getBidId();
			bidTable->Remove(searchValue);

			break;

		case 9:
			break;

		default:
			cout << "Not a valid selection." << endl;

			break;

		}
	}

	cout << "Good bye." << endl;

	return 0;
}
