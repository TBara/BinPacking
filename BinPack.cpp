/**********************************************************************
 * *   Author: Tom Barabasz
 * *   Class:  CS 325 W2020
 * *   Date: 03/08/2020 
 * *   Description: Homework8; Bin packing 
**********************************************************************/
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <iterator>
#include <sstream>
#include <bits/stdc++.h>
#include <ctime>

using namespace std;
using std::cout;
using std::endl;
using std::sort;

struct bin
{
    int capacity;
    int items;
    vector<int> weights;
};

void startTests(string fileName);
void startTests2(string fileName);
void createTestCases();
bool inputValidInt(std::string question, int& userInp);
vector<bin> getBins(string inFile);
int firstFit(bin bins);
int bestFit(bin bins);
int firstFitDec(bin bins);

int main()
{
    int userInput = 0;
    do
    {
        string question = "\n1.	Run test cases from bin.txt file\n2.	Create and run 25 random test cases\n3.	Exit the program";

        if (inputValidInt(question, userInput))
        {
            switch (userInput)
            {
            case 1:
                startTests("bin.txt");
                break;
            case 2:
                createTestCases();
                startTests2("Test_Cases.txt");
                break;
            case 3:
                break;
            default:
                break;
            }
        }
    } while (userInput != 3);

    return 0;
}

// Summarize test cases from bin.txt file
void startTests(string fileName)
{
    vector<bin> bins = getBins(fileName);
    int caseNo = 1;
    for (auto var : bins)
    {
        cout << "Test Case " << caseNo << " First Fit: " << firstFit(var) 
             <<", First Fit Decreasing: " << firstFitDec(var) << ", Best Fit: " << bestFit(var) << endl;
        caseNo ++;
    }
}

// Put each item as you come to it into the first (earliest opened)
// bin into which it fits. If there is no available bin then open a new bin.
int firstFit(bin bin)
{
    int result = 0;
    int binSpace[bin.items]; //Store remaining space in each bin

    //Insert items into bins
    for (size_t i = 0; i < bin.items; i++)
    {
        int j;
        for (j = 0; j < result; j++)
        {
            if (binSpace[j] >= bin.weights[i])
            {
                binSpace[j] = binSpace[j] - bin.weights[i];
                break;
            }
        }

        //Item does not fit into any existing bin. Create new bin
        if (j == result)
        {
            binSpace[result] = bin.capacity - bin.weights[i];
            result++;
        }
    }
    return result;
}

// Place the items in the order in which they arrive.
// Place the next item into the bin which will leave the least room left over
// after the item is placed in the bin. If it does not fit in any bin, start a new bin.
int bestFit(bin bin)
{
    //Count bins
    int result = 0;

    //Array to keep track of remaining space
    int binSpace[bin.items];

    for (int i = 0; i < bin.items; i++)
    {
        //best bin for weight[i]
        int j;
        int minCap = bin.capacity + 1;
        int binIdx = 0;
        for (j = 0; j < result; j++)
        {
            if (binSpace[j] >= bin.weights[i] && binSpace[j] - bin.weights[i] < minCap)
            {
                binIdx = j;
                minCap = binSpace[j] - bin.weights[i];
            }
        }

        // If no bin could accommodate weight[i],
        // create a new bin
        if (minCap == bin.capacity + 1)
        {
            binSpace[result] = bin.capacity - bin.weights[i];
            result++;
        }
        else // Assign the item to best bin
        {
            binSpace[binIdx] = binSpace[binIdx] - bin.weights[i];
        }
    }
    return result;
}

// Get number of bins: first-it-decreasing
int firstFitDec(bin bin)
{
    // First sort all weights in decreasing order
    sort(bin.weights.begin(), bin.weights.end(), greater<int>());

    //Use first-fit with sorted vector of ints
    return firstFit(bin);
}

// Read data.txt file and create problems to solve
vector<bin> getBins(string inFile)
{
    string line;
    vector<bin> bins;
    ifstream binFile(inFile);
    if (binFile.is_open())
    {
        int loaded = 0;
        int lineCnt = 0;
        int structCnt, cases, cap, n;
        structCnt = cases = cap = n = 0;
        while (getline(binFile, line))
        {
            stringstream ss;
            ss << line;
            string tempStr;
            int number;

            while (ss)
            {
                ss >> tempStr;
                if ((stringstream(tempStr) >> number) && (lineCnt == 0))
                {
                    cases = number;
                    if (!loaded)
                    {
                        for (size_t i = 0; i < cases; i++)
                        {
                            bins.push_back(bin());
                        }
                        loaded = true;
                    }
                }
                else if ((stringstream(tempStr) >> number) && (lineCnt == 1))
                {
                    bins[structCnt].capacity = number;
                }
                else if ((stringstream(tempStr) >> number) && (lineCnt == 2))
                {
                    bins[structCnt].items = number;
                }
                else if ((stringstream(tempStr) >> number) && (lineCnt == 3))
                {
                    string tmpNr;
                    stringstream ss2;
                    ss2 << line;
                    int wgt;
                    while (ss2)
                    {
                        ss2 >> tmpNr;
                        if (stringstream(tmpNr) >> wgt)
                        {
                            bins[structCnt].weights.push_back(wgt);
                        }
                    }
                    bins[structCnt].weights.pop_back();
                }

                if ((lineCnt % 3 == 0) && (lineCnt != 0))
                {
                    structCnt++;
                    lineCnt = 0;
                }
            }
            lineCnt++;
        }
        binFile.close();
    }
    return bins;
}

/*
					inputValidInt
This function takes a string to prompt the user and a reference to an integer to be set. It validates
user input for an integer. Once an integer is entered, the function returns true.
*/
bool inputValidInt(std::string question, int& userInp)
{
	bool validInt = false;
	float integer;
	do
	{
		cin.clear();
		cout << question << endl;
		cin >> integer;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Invalid entry. Try again. " << endl;
		}
		else
		{
			if ((floor(integer) == ceil(integer)) && (integer >= 0))
			{
				validInt = true;
				userInp = integer;
			}
		}
	} while (!validInt);
	return validInt;
}

//Summarize randomly generated test cases from Test_Cases.txt file
//Test cases are generated by  createTestCases()
void startTests2(string fileName)
{
    vector<bin> bins = getBins(fileName);
    int caseNo = 1;
    
    int fitFirstScore = 0;
    int fitFirstDecrScore = 0;
    int fitBestScore = 0;

    for (auto var : bins)
    {
        int fitFirst = firstFit(var);
        int fitFirstDecr = firstFitDec(var);
        int fitBest = bestFit(var);

        if(fitFirst <= fitFirstDecr && fitFirst <= fitBest)
        {
            fitFirstScore++;
        }
        if (fitFirstDecr <= fitFirst && fitFirstDecr <= fitBest)
        {
            fitFirstDecrScore++;
        }
        if (fitBest <= fitFirst && fitBest <= fitFirstDecr)
        {
            fitBestScore++;
        }
        cout <<  "Test Case " << caseNo << " First Fit: " << fitFirst
             <<", First Fit Decreasing: " << fitFirstDecr << ", Best Fit: " << fitBest << endl;
        caseNo ++;
    }
    cout << "\nResults: \n"<< "First Fit: " << fitFirstScore 
         << "\nFit First Decreasing: "<< fitFirstDecrScore 
         << "\nBest Fit: " << fitBestScore << endl; 
}

//Cretes 25 randomly generatd bin packing test cases
void createTestCases()
{
    int nCases = 25;
    
    ofstream testCases;
    testCases.open("Test_Cases.txt", std::ofstream::out | std::ofstream::trunc);
    srand((unsigned) time(0));

    //Assign number of test cases
    testCases << nCases << endl;

    for (size_t i = 0; i < nCases; i++)
    {
        int cap = 10 + (rand() % 25);
        testCases << cap <<endl;

        int items = 7 + (rand() % 25);
        testCases << items << endl;
        for (size_t j = 0; j < items; j++)
        {
            testCases << (rand() % cap + 1) << " ";
        }
        testCases << endl;
    }
    testCases.close();
}