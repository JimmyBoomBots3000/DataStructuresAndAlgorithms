/*
 * Utilizes a CSV Parser library to import records into a Vector data structure
 * Demonstrates SelectionSort, QuickSort algorithms
 */

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//TODO: Security
//TODO: Enable custom search

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
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
vector<Bid> loadBids(string csvPath) {
	cout << "Loading CSV file " << csvPath << endl;

	vector<Bid> bids;

	// initialize the CSV Parser
	csv::Parser file = csv::Parser(csvPath);

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
			bids.push_back(bid);
		}

	} catch (csv::Error &e) {
		std::cerr << e.what() << std::endl;
	}

	return bids;
}

/**
 * To facilitate QuickSort
 * Partition the vector of bids into two parts, low and high
 *
 * @param bids Address of the vector<Bid> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 */
int partition(vector<Bid> &bids, int begin, int end) {
	int low = begin;
	int high = end;

	// Relative center of partition
	int pivot = begin + (end - begin) / 2;

	bool done = false;
	while (!done) {
		while (bids.at(low).title.compare(bids.at(pivot).title) < 0) {
			++low;
		}

		while (bids.at(pivot).title.compare(bids.at(high).title) < 0) {
			--high;
		}

		if (low >= high) {
			done = true;
		} else {
			swap(bids.at(low), bids.at(high)); // @suppress("Invalid arguments")

			++low;
			--high;
		}
	}

	return high;

}

/**
 * Perform a quick sort on bid title
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 *
 * @param bids address of the vector<Bid> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 */
void quickSort(vector<Bid> &bids, int begin, int end) {
	int mid = 0;

	if (begin >= end) {
		return;
	}

	mid = partition(bids, begin, end);

	// Recursive calls to each partition
	quickSort(bids, begin, mid);
	quickSort(bids, mid + 1, end);
}

/**
 * Perform a selection sort on bid title
 * Average performance: O(n^2))
 * Worst case performance O(n^2))
 *
 * @param bid address of the vector<Bid>
 *            instance to be sorted
 */
void selectionSort(vector<Bid> &bids) {

	unsigned int min;

	for (unsigned int i = 0; i < bids.size(); ++i) {
		min = i;

		for (unsigned int j = i + 1; j < bids.size(); ++j) {
			if (bids.at(j).title.compare(bids.at(min).title) < 0) {
				min = j;
			}

		}

		if (min != i) {
			swap(bids.at(i), bids.at(min)); // @suppress("Invalid arguments")
		}
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

	// Define a vector to hold all the bids
	vector<Bid> bids;

	clock_t ticks;

	Bid bid;

	bool bidFound;


	ticks = clock();

	// Define a vector to hold all the bids
	bids = loadBids(csvPath);
	cout << bids.size() << " bids read" << endl;
	ticks = clock() - ticks; // current clock ticks minus starting clock ticks
	displayTime(ticks);

	int choice = 0;
	while (choice != 9) {
		cout << "Menu:" << endl;
		cout << "  1. Reload Bids" << endl;
		cout << "  2. Display All Bids" << endl;
		cout << "  3. Selection Sort All Bids" << endl;
		cout << "  4. Quick Sort All Bids" << endl;
		cout << "  5. Find Bid" << endl;
		cout << "  9. Exit" << endl;
		cout << "Enter choice: ";

		// Prompt for selection - check input
		while (!(cin >> choice)) {
			cout << "Invalid choice, please try again" << endl;
			cin.clear();
			cin.ignore();
		}

		bidFound = false;

		switch (choice) {

		case 1:
			ticks = clock();
			bids = loadBids(csvPath); // Load the bids
			cout << bids.size() << " bids read" << endl;
			ticks = clock() - ticks; // current clock ticks minus starting clock ticks
			displayTime(ticks);

			break;

		case 2:
			// Print all bid objects to console
			for (unsigned int i = 0; i < bids.size(); ++i) {
				displayBid(bids[i]);
			}
			cout << endl;

			break;

		case 3:
			ticks = clock();
			selectionSort(bids); // sort the bids
			cout << bids.size() << " bids read" << endl;
			ticks = clock() - ticks; // current clock ticks minus starting clock ticks
			displayTime(ticks);

			break;

		case 4:
			ticks = clock();
			quickSort(bids, 0, bids.size() - 1); // Sort the bids
			cout << bids.size() << " bids read" << endl;
			ticks = clock() - ticks; // current clock ticks minus starting clock ticks
			displayTime(ticks);

			break;

		case 5:
			ticks = clock();

			// Prompt user for a bid ID
			bidKey = getBidId();

			ticks = clock() - ticks; // current clock ticks minus starting clock ticks

			// Search for entered bid id
			for (auto searchBid : bids) {
				if (searchBid.bidId.compare(bidKey) == 0) {
					displayBid(searchBid);
					bidFound = true;
				}
			}
			if (!bidFound) {
				cout << "Bid Id " << bidKey << " not found." << endl;
			}

			ticks = clock() - ticks; // current clock ticks minus starting clock ticks
			displayTime(ticks);

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
