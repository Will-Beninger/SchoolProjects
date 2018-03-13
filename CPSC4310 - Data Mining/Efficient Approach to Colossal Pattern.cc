//********************************************************************
// CPSC 4310 - Fall 2013
// Prof: Wendy Osborn
// Project: An Efficient Approach to Colossal Pattern Mining
// Written By: Will Beninger, Krysti Bouttell-Bonnar
// Date: November 21, 2013
//********************************************************************

#include<iostream>       // cout
#include<map>            // map
#include<vector>         // vector
#include<algorithm>      // sort
#include<stdlib.h>       // rand, srand
#include<time.h>         // time
#include<ctime>          // time

using namespace std;

const clock_t begin_time = clock();

const int DATA_X_COORDINATE = 26;//60;
const int DATA_Y_COORDINATE = 12;//39;
const double CORE_RATIO = 0.5;
const int MIN_SUPPORT = 13;//20;
const int MAX_PATTERNS = 2;

struct itemIDStruct
{
  int patternNumber;
  vector<int> itemset;
  vector<int> transactions;
};

struct customComparer
{
  bool operator()(const vector<int>& leftKey, const vector<int>& rightKey) const
  {
    if(leftKey.size() > rightKey.size())
      return true;
    else if(leftKey.size() == rightKey.size())
      return true;
    else
      return false;
  }
};

struct alphaBeta
{
  int alphaPatternIDNumber;
  vector<int> betaPatternIDNumber;
};

int data[DATA_X_COORDINATE][DATA_Y_COORDINATE];

typedef map< vector<int>, vector<int> > verticalItemsetMap;
typedef map<int, vector<int> > clusterMap;
typedef map< vector<int>, int, customComparer > sortClusterMap;

verticalItemsetMap verticalDataItems;
verticalItemsetMap initialPool;

vector<itemIDStruct> patternID;
vector<itemIDStruct> frequentLongPattern;





// FUNCTION PROTOTYPES

void createDiag40();
void createVerticalData();
verticalItemsetMap apriori();
bool contains(vector<int> temp, int value);
clusterMap frequencyClusters(verticalItemsetMap tempMap);
verticalItemsetMap colossalPatternMining(verticalItemsetMap pool, 
					 int max_patterns, double ratio);
sortClusterMap sortClusters(clusterMap clusters); 
double distanceSet(vector<int> alphaTransactions, vector<int> betaTransactions);
verticalItemsetMap patternMerge(alphaBeta alphaBetaStruct);


int main()
{
  cout << "Data size: " << DATA_X_COORDINATE << " by " << DATA_Y_COORDINATE 
       << endl;
  cout << "With a minimum support of: " << MIN_SUPPORT << endl;
  cout << "With maximumu number of patterns to find: " << MAX_PATTERNS << endl;
  srand(time(NULL));

  // creates the Diag40 data we will be working with
  createDiag40();

  // takes the Diag40 data and stroes it in vertical format
  createVerticalData();

  // create initial pool
  initialPool = apriori();

  verticalItemsetMap initialPool2;
  verticalItemsetMap initialPool3;

  do{
    
    initialPool = colossalPatternMining(initialPool, MAX_PATTERNS, CORE_RATIO);
     }while(initialPool.size() > MAX_PATTERNS);

  verticalItemsetMap::iterator itBegin = initialPool.begin();
  verticalItemsetMap::iterator itEnd = initialPool.end();


  for(itBegin; itBegin != itEnd; itBegin++)
    {
      for(int i = 0; i < itBegin->first.size(); i++)
	{
	  cout << itBegin->first[i] << ", ";
	}
      cout << " = ";
      for(int i = 0; i < itBegin->second.size(); i++)
	{
	  cout << itBegin->second[i] << ", ";
	}
      cout << endl;
    }
  
  cout << "Algorithm takes " << float(clock() - begin_time) / CLOCKS_PER_SEC 
     << " seconds to complete." << endl;

  return 0;
}


//**********************************************************************
// Function: colossalPatternMining
// Purpose: 
// Return: a verticalItemsetMap, which is a map<vector<int>, vector<int> >
//**********************************************************************
verticalItemsetMap colossalPatternMining(verticalItemsetMap pool, 
					 int max_patterns, double ratio) 
{
  //pool is a map<vector<int>, vector<int> >

  verticalItemsetMap tempMap;          // map<vector<int>, vector<int> >
  verticalItemsetMap tempReturn;       // map<vector<int>, vector<int> >

  clusterMap clusters;                 // map<int, vector<int> >
  sortClusterMap sortedClusters;         // map< vector<int>, int>
  
  double coreRatio;
  int randomNumber;
  int patternIDNum;
  int largestCluster = 0;
  int clusterMean;

  vector<alphaBeta> coreList;

  vector<int> tempPatternVector;
  vector<int> tempTransactionVector;

  vector<int> tempBetaPatternVector;
  vector<int> tempBetaTransactionVector;
  int betaPatternNumber;

  coreRatio = 1 - (1 / ((2/ratio) - 1));

  clusters = frequencyClusters(pool);

  sortedClusters = sortClusters(clusters);
  
  clusterMap::iterator itBeginClusters;
  clusterMap::iterator itEndClusters = clusters.end();

  sortClusterMap::iterator itBeginSort;
  sortClusterMap::iterator itEndSort = sortedClusters.end();

  sortClusterMap::iterator itPointerToLargestCluster = sortedClusters.begin();

  for(int i = 0; i < MAX_PATTERNS; i++)
    {
      vector<int> tempPatternVector;
      vector<int> tempTransactionVector;
      
      int betaPatternIDNum;
      vector<int> betaTempPatternVector;
      vector<int> betaTempTransactionVector;
      
      alphaBeta temp;
      
      // always choose random alpha to be from largest cluster
      randomNumber = rand() % (itPointerToLargestCluster->first.size() + 1);
      
      // a random alpha's pattern ID Number is now stored in 
      //    patternIDNum
      patternIDNum = randomNumber;
      
      // using alpha'a patternIDNum access alpha'a pattern and transaction 
      //    list
      for(int k = 0; k < patternID.size(); k++)
	{
	  if(patternIDNum == patternID[k].patternNumber)
	    {
	      // alpha's pattern is now stored in tempPatternVector
	      tempPatternVector = patternID[k].itemset;
	      // alpha's transaction list is now stored in 
	      //    tempTransactionVector
	      tempTransactionVector = patternID[k].transactions;
	    }
	}
      // if alpha passes minimum support
      if(tempTransactionVector.size() >= MIN_SUPPORT)
	{
	  itBeginClusters = clusters.begin();
	  // find beta patterns by going through clusters
	  for(itBeginClusters; itBeginClusters != itEndClusters; 
	      itBeginClusters++)
	    {
	      for(int k = 0; k < itBeginClusters->second.size(); k++)
		{
		  // check to make sure alpha and beta are not the same
		  if(itBeginClusters->second[k] != patternIDNum)
		    {
		      // beta'a pattern ID Number is now stored in 
		      //    betaPatternIDNum
		      betaPatternIDNum = itBeginClusters->second[k];
		    
		  
		      // get beta's pattern and transaction list
		      for(int a = 0; a < patternID.size(); a++)
			{
			  if(betaPatternIDNum == patternID[a].patternNumber)
			    {
			      // beta's pattern is now stored in 
			      //    betaTempPatternVector
			      betaTempPatternVector = patternID[a].itemset;
			      // beta's transaction list is now stored in 
			      //    betaTempTransactionVector
			      betaTempTransactionVector = patternID[a].
				transactions;
			    }
			}
		      
		    
		  bool contains2 = false;
		  // if alpha and beta are core patterns
		  if(distanceSet(tempTransactionVector, 
				 betaTempTransactionVector) <= coreRatio)
		    {
		      
		      // check to see if alpha's coreList is empty
		      if(coreList.empty())
			{
			  // if it is put first values into it
			  temp.alphaPatternIDNumber = patternIDNum;
			  temp.betaPatternIDNumber.push_back(betaPatternIDNum);
			  coreList.push_back(temp);
			  continue;
			}
		    
		      // if it is not empty check to see if alpha is 
		      //    already in coreList
		      if(!(coreList.empty()))
			{
			  for(int w = 0; w < coreList.size(); w++)
			    {
			      if(coreList[w].alphaPatternIDNumber == 
				 patternIDNum)
				{
				  contains2 = true;
				  coreList[w].betaPatternIDNumber.
				    push_back(betaPatternIDNum);
				}
			    }
			  if(!(contains2))
			    {
			      temp.alphaPatternIDNumber = patternIDNum;
			      temp.betaPatternIDNumber.
				push_back(betaPatternIDNum);
			      coreList.push_back(temp);
			    }
			}
		    }
		    }
		}
	    }
	}
    }

  


  verticalItemsetMap tempMap2;

  for(int i = 0; i < coreList.size(); i++)
    {
      tempMap = patternMerge(coreList[i]);

      if(tempReturn.empty())
	{
	  tempReturn = tempMap;
	}
      else
	{
	  int count = 0;
	  verticalItemsetMap::iterator itBeginTemp = tempMap.begin();
	  verticalItemsetMap::iterator itEndTemp = tempMap.end();

	  for(itBeginTemp; itBeginTemp != itEndTemp; itBeginTemp++)
	    {
	      if(!(tempReturn.count(itBeginTemp->first) > 0))
		tempReturn.insert(make_pair(itBeginTemp->first,
					    itBeginTemp->second));
	    }
	}
    }

  verticalItemsetMap::iterator itBeginReturn = tempReturn.begin();
  verticalItemsetMap::iterator itEndReturn = tempReturn.end();

  vector<verticalItemsetMap::iterator> itErase;

  for(itBeginReturn; itBeginReturn != itEndReturn; itBeginReturn++)
    {
      if(itBeginReturn->second.size() < MIN_SUPPORT)
	{
	  itErase.push_back(itBeginReturn);
	}
    }

  for(int i = 0; i < itErase.size(); i++)
    {
      tempReturn.erase(itErase[i]);
    }
  

  return tempReturn;
  
}


//**********************************************************************
// Function: patternMerge
// Purpose: to merge pattern alpha with it's core patterns
// Return: a verticalItemsetMap which is a map<vector<int>, vector<int> >
//**********************************************************************
verticalItemsetMap patternMerge(alphaBeta alphaBetaStruct)
{
  // alphaBetaStruct is a struct with members int alphaPatternIDNumber and 
  //    vector<int> betaPatternIDNumber

  vector<int> coreList;

  verticalItemsetMap colossal;
  verticalItemsetMap tempMap;

  int alphaIDNumber;
  vector<int> alphaItemset;
  vector<int> alphaTransactionList;

  int betaIDNumber;
  vector<int> betaItemset;
  vector<int> betaTransactionList;

  

  int count = 0;				
  int countUnion = 0;

  coreList.push_back(alphaBetaStruct.alphaPatternIDNumber);
  for(int i = 0; i < alphaBetaStruct.betaPatternIDNumber.size(); i++)
    {
      coreList.push_back(alphaBetaStruct.betaPatternIDNumber[i]);
    }
 
  for(int i = 0; i < coreList.size(); i++)
    {
      alphaIDNumber = coreList[i];

       // get alpha's itemset and transaction list
      for(int m = 0; m < patternID.size(); m++)
	{
	  if(alphaIDNumber == patternID[m].patternNumber)
	    {
	      // now have alpha's itemset and transasction list
	      alphaItemset = patternID[m].itemset;
	      alphaTransactionList = patternID[m].transactions;
	    }
	}
	  
      for(int j = 0; j < coreList.size(); j++)
	{
	  betaIDNumber = coreList[j];

	  // get beta's itemset and transaction list
	  for(int i = 0; i < patternID.size(); i++)
	    {
	      if(betaIDNumber == patternID[i].patternNumber)
		{
		  // now have beta's itemset and transasction list
		  betaItemset = patternID[i].itemset;
		  betaTransactionList = patternID[i].transactions;
		}
	    }

	  // checking to see if beta is a subset of alpha
	  for(int j = 0; j < alphaItemset.size(); j++)
	    {
	      for(int k = 0; k < betaItemset.size(); k++)
		{
		  if(betaItemset[k] == alphaItemset[j])
		    count++;
		}
	    }
	  
	  // if beta is not a subset of alpha then union alpha's 
	  //    itemset and beta's itemset and store that new itemset 
	  //    in tempItemset
	  if(!(count == betaItemset.size() || count == alphaItemset.size()))
	    {
	      itemIDStruct temp;
	      count = 0;
	      // unioning alpha's itemset and beta's itemset
	      temp.itemset = alphaItemset;
	      for(int j = 0; j < betaItemset.size(); j++)
		{
		  for(int k = 0; k < temp.itemset.size(); k++)
		    {
		      if(!(betaItemset[j] == temp.itemset[k]))
			{
			  countUnion++;
			}
		    }
		  if(countUnion == temp.itemset.size())
		    temp.itemset.push_back(betaItemset[j]);
		  countUnion = 0;
		}
	      // intersecting alpha's transaction list and beta's 
	      //    transaction list
	      for(int j = 0; j < betaTransactionList.size(); j++)
		{
		  for(int k = 0; k < alphaTransactionList.size(); k++)
		    {
		      if(betaTransactionList[j] == alphaTransactionList[k])
			temp.transactions.push_back(betaTransactionList[j]);
		    }
		}
	      // does intersection of alpha and beta have minimum support
	      if(temp.transactions.size() < MIN_SUPPORT)
		{
		  continue;
		}
	      // does intersection of alpha and beta satisfy distance change
	      else if(distanceSet(temp.transactions, 
				  alphaTransactionList) > (1 - CORE_RATIO))
		{
		  temp.patternNumber = betaIDNumber;
		  temp.itemset = betaItemset;
		  temp.transactions = betaTransactionList;
		  frequentLongPattern.push_back(temp);
		  continue;
		}
	      else
		{
		  //cout << "in else" << endl;
		  sort(temp.itemset.begin(), temp.itemset.end());
		  sort(temp.transactions.begin(), 
		       temp.transactions.end());
		  
		  for(int z = 0; z < patternID.size(); z++)
		    {
		      if(alphaIDNumber == patternID[z].patternNumber)
			{
			  patternID[z].itemset = temp.itemset;
			  patternID[z].transactions = temp.transactions;
			}
		    }
		  
		  
		  if(tempMap.empty())
		    tempMap.insert(make_pair(temp.itemset, temp.transactions));
		  else
		    {
		      if(!(tempMap.count(temp.itemset) > 0))
			tempMap.insert(make_pair(temp.itemset, 
						 temp.transactions));
		    }
		} 
	      
	      
	    }
	  else
	  count = 0;
	}
      
    }
  
 
  colossal = tempMap;
 
  return colossal;
}
 

//********************************************************************
// Function: distanceSet
// Purpose: determining the distance between alpha and beta
// Return: a double storing the distance
//********************************************************************
double distanceSet(vector<int> alphaTransactions, vector<int> betaTransactions)
{
  double temp;
  double intersect = 0;
  double unionSet = 0;

  for(int i = 0; i < alphaTransactions.size(); i++)
    {
      for(int j = 0; j < betaTransactions.size(); j++)
	{
	  if(alphaTransactions[i] == betaTransactions[j])
	    intersect++;
	}
    }

  unionSet = (alphaTransactions.size() + betaTransactions.size()) - intersect;

  temp = 1 - (intersect / unionSet);
  return temp;
}

//*********************************************************************
// Function: sortClusters
// Purpose: sort the clusters on descending order based on their frequency
// Return: a sortClusterMap, which is a map< vector<int>, int>
//********************************************************************
sortClusterMap sortClusters(clusterMap clusters)
{
  // clusterMap is map<int, vector<int> >
  // sortClusterMap is map< vector<int>, int>

  sortClusterMap sortClusters;
  vector<int> tempVector;
  int temp;
  
  clusterMap::iterator itBegin = clusters.begin();
  clusterMap::iterator itEnd = clusters.end();

  for(itBegin; itBegin != itEnd; itBegin++)
    {
      temp = itBegin->first;
      tempVector = itBegin->second;
      sortClusters.insert(make_pair(tempVector, temp));
    }
  return sortClusters;
  
}



//**********************************************************************
// Function: createVerticalData
// Purpose: creates a vertical representation of the data
// Return: not applicable
//**********************************************************************
void createVerticalData()
{
  verticalItemsetMap::iterator itEnd;
  verticalItemsetMap::iterator itPosition;

  for(int i = 0; i < DATA_X_COORDINATE; i++)
    {
      for(int j = 0; j < DATA_Y_COORDINATE; j++)
	{
	  itEnd = verticalDataItems.end();
	  vector<int> tempVectorItem;
	  tempVectorItem.push_back(data[i][j]);
	  itPosition = verticalDataItems.find(tempVectorItem);
	  if(itPosition != itEnd)
	    {
	      itPosition->second.push_back(i);
	    }
	  else
	    {
	      vector<int> tempVector;
	      tempVector.push_back(i);
	      verticalDataItems.insert(make_pair(tempVectorItem, tempVector));
	    }
	}
    }
}


//****************************************************************
// Function: contains
// Purpose: return whether a specific value is in a vector
// Return: boolean
//****************************************************************
bool contains(vector<int> tempVector, int value)
{
  for(int i = 0; i < tempVector.size(); i++)
    {
      if(tempVector[i] == value)
	return true;
    }
  return false;
}



//*******************************************************************
// Function: createDiag40
// Purpose: creates the transaction dataset
// Return: not applicable
//*******************************************************************
void createDiag40()
{
  int dataItem = 41;
  bool diagonal = false;

  int tempItem;

  for(int i = 0; i < DATA_X_COORDINATE; i++)
    {
      for(int j = 0; j < DATA_Y_COORDINATE; j++)
	{
	  if(i < (DATA_Y_COORDINATE + 1))
	    {
	      if(i == j)
		{
		  data[i][j] = j + 2;
		  diagonal = true;
		  
		}
	      else if(diagonal)
		data[i][j] = j + 2;
	      else
		data[i][j] = j + 1;
	    }
	  else
	      data[i][j] = dataItem++;
	}
      dataItem = 41;
      diagonal = false;
    }
}


//********************************************************************
// Function: frequencyClusters
// Purpose: to group the patterns together based on their frequency
// Return: a clusterMap, which is a map<int, vector<int>>
//********************************************************************
clusterMap frequencyClusters(verticalItemsetMap tempMap)
{
  int patternCounter = 0;

  verticalItemsetMap::iterator itBegin = tempMap.begin();
  verticalItemsetMap::iterator itEnd = tempMap.end();

  for(itBegin; itBegin != itEnd; itBegin++)
    {
      itemIDStruct tempStruct;

      tempStruct.patternNumber = patternCounter;

      for(int i = 0; i < itBegin->first.size(); i++)
	{
	  tempStruct.itemset.push_back(itBegin->first[i]);
	}
      for(int i = 0; i <itBegin->second.size(); i++)
	{
	  tempStruct.transactions.push_back(itBegin->second[i]);
	}
      
      patternID.push_back(tempStruct);
      ++patternCounter;
    }
 
  clusterMap clusterFrequency;           // map<int, vector<int>>

  clusterMap::iterator itPosition;
  clusterMap::iterator itEndClustFreq;

  vector<itemIDStruct>::iterator itBeginID = patternID.begin();
  vector<itemIDStruct>::iterator itEndID = patternID.end();

  int count = 0;


  // loop through patternID which is a vector of itemIDStructs
  for(itBeginID; itBeginID != itEndID; itBeginID++)
    {
      vector<int> tempVector;
      // if clusterFrequency is empty put first values into it
      if(clusterFrequency.empty())
	{
	  tempVector.push_back(itBeginID->patternNumber);
	  clusterFrequency.insert(make_pair(itBeginID->transactions.size(), 
					    tempVector));
	}
      else
	{
	  // check to see if frequency is already in clusterFrequency
	  itPosition = clusterFrequency.find(itBeginID->transactions.size());
	  itEndClustFreq = clusterFrequency.end();
	  // if true itPosition is now storing a pointer to the location of 
	  //    that specific frequency in clusterFrequency
	  if(itPosition != itEndClustFreq)
	    {
	      for(int i = 0; i < itPosition->second.size(); i++)
		{
		  // check to see if pattern with specific frequency is 
		  //    already in clusterFrequency
		  if(itPosition->second[i] != itBeginID->patternNumber)
		    count++;
		  if(count == itPosition->second.size())
		    itPosition->second.push_back(itBeginID->patternNumber);
		}
	      count = 0;
	    }
	  else
	    {
	      tempVector.push_back(itBeginID->patternNumber);
	      clusterFrequency.insert(make_pair(itBeginID->transactions.size(), 
						tempVector));
	      }
	}
    }
  
  return clusterFrequency;
  
}

//*************************************************************
// Function: apriori
// Purpose: finds 2-itemsets using apriori
// Return: a verticalItemsetMap containing the 2-itemsets
//*************************************************************
verticalItemsetMap apriori()
{

  verticalItemsetMap oneItemset;
  verticalItemsetMap twoItemset;

  verticalItemsetMap::iterator itBeginData = verticalDataItems.begin();
  verticalItemsetMap::iterator itEndData = verticalDataItems.end();

  verticalItemsetMap::iterator itBeginOneFirst;
  verticalItemsetMap::iterator itBeginOneSecond;
  verticalItemsetMap::iterator itEndOne;

  // creating 1-itemsets
  for(itBeginData; itBeginData != itEndData; itBeginData++)
    {
      if(itBeginData->second.size() >= MIN_SUPPORT)
	{
	  vector<int> tempItem;
	  vector<int> tempTransaction;
	  for(int i = 0; i < itBeginData->first.size(); i++)
	    {
	      tempItem.push_back(itBeginData->first[i]);
	    }
	  for(int j = 0; j < itBeginData->second.size(); j++)
	    {
	      tempTransaction.push_back(itBeginData->second[j]);
	    }
	  sort(tempItem.begin(), tempItem.end());

	  if(tempTransaction.size() >= MIN_SUPPORT)
	    {
	      sort(tempTransaction.begin(), tempTransaction.end());
	      oneItemset.insert(make_pair(tempItem, tempTransaction));
	    }
	}
    }

  itBeginOneFirst = oneItemset.begin();
  itBeginOneSecond = oneItemset.begin();
  itEndOne = oneItemset.end();

  // creating 2-itemsets
 
  // go through every item in the oneItemset comparing it to the other items 
  //    in the oneItemset
  for(itBeginOneFirst; itBeginOneFirst != itEndOne; itBeginOneFirst++)
    {
      itBeginOneSecond = oneItemset.begin();
      for(itBeginOneSecond; itBeginOneSecond != itEndOne; itBeginOneSecond++)
	{
	  // check to see if the 2 sets of items are different
	  if(itBeginOneFirst->first != itBeginOneSecond->first)
	    {
	      vector<int> tempItem;

	      // if the 2 sets of items are different union them
	      for(int i = 0; i < itBeginOneFirst->first.size(); i++)
		{
		  tempItem.push_back(itBeginOneFirst->first[i]);
		}
	      for(int j = 0; j < itBeginOneSecond->first.size(); j++)
		{
		  if(!(contains(tempItem, itBeginOneSecond->first[j])))
		    tempItem.push_back(itBeginOneSecond->first[j]);
		}
	      sort(tempItem.begin(), tempItem.end());

	      // check to see if the union of the 2 sets is already in 
	      //    twoItemset
	      if(twoItemset.count(tempItem) == 0)
		{
		  vector<int> tempTransaction;

		  // intersect the set of transactions for the 2 sets of items
		  for(int i = 0; i < itBeginOneFirst->second.size(); i++)
		    {
		      for(int j = 0; j < itBeginOneSecond->second.size(); j++)
			{
			  if(itBeginOneFirst->second[i] == 
			     itBeginOneSecond->second[j])
			    {
			      tempTransaction.push_back(itBeginOneFirst->
						      second[i]);
			    }
			}
		    }

		  // remove any itemsets that don't meet minimum support
		  if(tempTransaction.size() >= MIN_SUPPORT)
		    {
		      sort(tempTransaction.begin(), tempTransaction.end());
		      twoItemset.insert(make_pair(tempItem, tempTransaction));
		    }
		}
	    }
	}
    }
 
  return twoItemset;

}
