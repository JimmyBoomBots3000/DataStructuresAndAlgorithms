//============================================================================
// Name        : Lab2-2.cpp
// Author      : James Richmond
// Course      : CS-260-J5337 Data Structures and Algorithms 20EW5
// Date        : 5/16/2020
// Project Num : 2-1
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Lab 2-1 Up to Speed in C++, Part 2
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

// FIXME (1): Reference the CSVParser library
/*
 * JR: This includes the user-defined CSVparser
 */
#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

struct Bid {
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.title << " | " << bid.amount << " | " << bid.fund << endl;
    return;
}

/**
 * Prompt user for bid information
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    cout << "Enter title: ";
    cin.ignore();
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
	// FIXME (2): Define a vector data structure to hold a collection of bids.
	/*
	 * JR: The vector is defined, with the data type Bid and the identifier bids
	 */
    vector<Bid> bids;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

	// loop to read rows of a CSV file
	for (unsigned int i = 0; i < file.rowCount(); i++) {
        // FIXME (3): create a data structure to hold data from each row and add to vector
		/*
		 * JR: The variable bid of type Bid is created, using the structure defined above in the struct block (line 28)
		 * Each bid is an element of the vector bids
		 * The for loop iterates over rows of the parsed CSV file and assigns data from the specified columns to
		 * respective data members of the bid variable
		 * A debug message can optionally be enabled by un-commenting the cout statement
		 * The push_back() call is used to expand the size of the bids vector for each bid
		 */
		Bid bid;
		bid.title = file[i][0];
		bid.fund = file[i][8];
		bid.amount = strToDouble(file[i][4], '$');

//		cout << "Record " << i + 1 << ": Title: "<< bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

		bids.push_back(bid);

    }
    return bids;
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
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
    }

    // FIXME (4): Define a vector to hold all the bids
    /*
     * JR: Defines a vector of type Bid; each element of the vector will have a Bid-type item as defined in the struct
     * The vector is named theBids
     */
    vector<Bid> theBids;

    // FIXME (7a): Define a timer variable
    /*
     * JR: From the c++ primer; defines the variable ticks as type clock_t
     */
    clock_t ticks;


    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Enter a Bid" << endl;
        cout << "  2. Load Bids" << endl;
        cout << "  3. Display All Bids" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
        	cout << "Not currently implemented." << endl;

            break;
        case 2:
            // FIXME (7b): Initialize a timer variable before loading bids
        	/*
        	 * JR: From the c++ primer; calls clock() and assigns the return value (the current CPU time) to ticks
        	 */
        	ticks = clock();

            // FIXME (5): Complete the method call to load the bids
        	/*
        	 * JR: Calls loadBids method, using csvPath as the argument
        	 * The return value of loadBids, which is a vector of Bid items, is assigned to theBids
        	 */
        	theBids = loadBids(csvPath);

            // FIXME (7c): Calculate elapsed time and display result
        	/*
        	 * JR: From the c++ primer; calls clock() again to get a new current CPU time, subtracts ticks (as assigned in 7b)
        	 * and assigns this as the new value of ticks (which equates to the elapsed time)
        	 * cout statements display messages including this elapsed time in milliseconds and seconds
        	 */
        	ticks = clock() - ticks; // current clock ticks minus starting clock ticks
        	cout << "time: " << ticks << " milliseconds" << endl;
        	cout << "time: " << ticks*1.0/CLOCKS_PER_SEC << " seconds" << endl;

            break;
        case 3:
            // FIXME (6): Loop and display the bids read
        	/*
        	 * JR: Iterates over the elements of the theBids vector
        	 * In each iteration, calls displayBid(), passing the vector element itself as an argument
        	 * displayBid() prints a message for the vector element, which includes its data members
        	 */
            for (int i = 0; i < theBids.size(); ++i) {
            	displayBid(theBids[i]);
            }
            cout << endl;

            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
