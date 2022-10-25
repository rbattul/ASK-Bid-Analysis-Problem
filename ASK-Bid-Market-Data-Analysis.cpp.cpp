// ConsoleApplication3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <tuple>
#include <sstream> 
#include <string.h>
#include <vector>


using namespace std;

int main() {
	// Open file:
	std::ifstream fin("D:\\Rajesh\\Daily-Work\\18-Apr-2019\\marketdata_for_interview.csv"); // Open in text-mode.

																							// Opening may fail, always check.
	if (!fin) {
		std::cout << "Error, could not open file." << std::endl;
		return -1;
	}

	std::cout << endl << "Input File is opened successfully";


	// Constructing map.
	std::map<std::string, tuple<float, float, long long, long, long double>> outputMap;
	std::multimap<std::string, tuple<long, float, int, float, int, long>> inputMultiMap;
	std::map<std::string, short> inputFields;
	
	/* Declare Constants for Input File Fields*/
	const string TICKER = "Ticker";
	const string TIMESTAMP = "TimeStamp";
	const string BID = "Bid";
	const string BIDSIZE = "BidSize";
	const string ASK = "Ask";
	const string ASKSIZE = "AskSize";
	const string VOLUME = "Volume";

	/* Set the user provided index for the input fields*/
	inputFields.insert(make_pair(TIMESTAMP, 0));
	inputFields.insert(make_pair(TICKER, 1));
	inputFields.insert(make_pair(BID, 2));
	inputFields.insert(make_pair(BIDSIZE, 3));
	inputFields.insert(make_pair(ASK, 4));
	inputFields.insert(make_pair(ASKSIZE, 5));
	inputFields.insert(make_pair(VOLUME, 6));


	long timeStamp = 232323, volume = 100, oMaxTimStaDiff = 0.0, prevTism = 0.0, prevMaxTismDiff = 0.0, maxTismDiff = 0.0;
	long long oSumVol = 0;
	long double oTotalSum = 0;
	std::string ticker = "test", temp = "test", oTicker = "test";
	float bid = 1.1, ask = 1.0, oMaxAskBid = 0.0, oMinAskBid = 500.00, prevAsk = 0.0, prevBid = 0.0, maxAskBid = 0.0, prevMaxAskBid = 0.0, minAskBid = 500.00, sumBid = 0.0, sumAsk = 0.0;
	int bidSize = 10, askSize = 20, sumBidSize = 0, sumAskSize = 0;

	std::string prevTicker = "";
	std::vector<string> values;
	std::string line,strTokesn;

	std::cout << endl;

	while (fin.good())
	{
		getline(fin, temp);
		stringstream ss(temp);
		std::string ticker;
		while (std::getline(ss, strTokesn, ','))
		{
			values.push_back(strTokesn);
		}

		short index = 0;

		for (auto &vec : values)
		{
			if (inputFields[TICKER] == index)
			{
				ticker = (string)vec;
			}
			else if (inputFields[TIMESTAMP] == index)
			{
				timeStamp = std::stol(vec);
			}
			else if (inputFields[BID] == index)
			{
				bid = std::stof(vec);
			}
			else if (inputFields[BIDSIZE] == index)
			{
				bidSize = std::stoi(vec);
			}
			else if (inputFields[ASK] == index)
			{
				ask = std::stof(vec);
			}
			else if (inputFields[ASKSIZE] == index)
			{
				askSize = std::stoi(vec);
			}
			else if (inputFields[VOLUME] == index)
			{
				volume = std::stol(vec);
			}
			else
			{
				cout << endl << "Failed to read input fields ..." << endl;
				return -1;
			}
			++index;
		}

		inputMultiMap.insert(make_pair(ticker, make_tuple(timeStamp, bid, bidSize, ask, askSize, volume)));
		values.clear();
	}


	// Perform record processing
	cout << endl << "Performing Records Processing ..." << endl;
	for (auto & columnMap : inputMultiMap)  // Iterate over input multimap
	{
		ask = get<3>(columnMap.second);
		bid = get<1>(columnMap.second);
		volume = get<5>(columnMap.second);
		timeStamp = get<0>(columnMap.second);
		bidSize = get<2>(columnMap.second);
		askSize = get<4>(columnMap.second);
		ticker = columnMap.first;

		if (0 == prevTicker.compare(ticker) || (0 == prevTicker.compare("")))
		{
			maxAskBid = std::fmax(ask, bid);
			oMaxAskBid = std::fmax(oMaxAskBid, maxAskBid);

			minAskBid = std::fmin(ask, bid);
			oMinAskBid = std::fmin(oMinAskBid, minAskBid);

			oSumVol += volume;

			maxTismDiff = timeStamp - prevTism;
			oMaxTimStaDiff = std::fmax(maxTismDiff, prevMaxTismDiff);

			sumBid += bid;
			sumAsk += ask;
			sumBidSize += bidSize;
			sumAskSize += askSize;
		}
		else
		{

			outputMap[prevTicker] = make_tuple(oMaxAskBid, oMinAskBid, oSumVol, oMaxTimStaDiff, (((sumBid * sumAskSize) + (sumAsk * sumBidSize)) / (sumBidSize + sumAskSize)));

			// re-intialize all required variables 
			prevTicker = "";
			oMaxAskBid = 0.0;
			oMinAskBid = 500.0;
			oSumVol = 0;
			oMaxTimStaDiff = 0;
			prevTism = 0;
			prevMaxTismDiff = 0;
			sumBid = 0;
			sumAsk = 0;
			sumBidSize = 0;
			sumAskSize = 0;

			// and perform logic for new ticker

			maxAskBid = std::fmax(ask, bid);
			oMaxAskBid = std::fmax(oMaxAskBid, maxAskBid);

			minAskBid = std::fmin(ask, bid);
			oMinAskBid = std::fmin(oMinAskBid, minAskBid);

			oSumVol += volume;

			maxTismDiff = timeStamp - prevTism;
			oMaxTimStaDiff = std::fmax(maxTismDiff, prevMaxTismDiff);

			sumBid += bid;
			sumAsk += ask;
			sumBidSize += bidSize;
			sumAskSize += askSize;
		}

		// *** if the record is last one then need to add that in outputMap
		prevTicker = ticker;
		prevTism = timeStamp;
		prevMaxTismDiff = maxTismDiff;
	} // end of loop

	std::cout << endl << " --------------------------------------------------------- " << endl;
	fin.close();
	std::cout << endl;

	// Printing out the output data
	cout << endl << " Printing Output Data on Console" << endl;
	for (auto & columnMap : outputMap) {
		std::cout << columnMap.first << " : " << get<0>(columnMap.second) << " : " << get<1>(columnMap.second) << " : " << get<2>(columnMap.second) << " : " << get<3>(columnMap.second) << " : " << get<4>(columnMap.second) << endl;
	}

	/* Write output to file*/

	ofstream outputFile("D:\\Rajesh\\Daily-Work\\18-Apr-2019\\Output.csv");

	for (auto & columnMap : outputMap) {
		outputFile << columnMap.first << "," << get<0>(columnMap.second) << "," << get<1>(columnMap.second) << "," << get<2>(columnMap.second) << "," << get<3>(columnMap.second) << "," << get<4>(columnMap.second) << endl;
	}

	outputFile.close();

	int te;
	std::cin >> te;

	return 0;
}

