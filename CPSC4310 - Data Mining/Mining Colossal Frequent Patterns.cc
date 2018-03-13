//********************************
// CPSC 4310 - Fall 2013
// Prof: Wendy Osborn
// Project: Mining Colossal Frequent Patterns by Core Pattern Fusion
// Written By: Krysti Bouttell-Bonnar, Will Beninger
// Date: November 17, 2013
//********************************
 
#include<iostream>      // cout
#include<map>           // map
#include<vector>        // vector
#include<algorithm>     // sort
#include<stdlib.h>      // rand, srand
#include<time.h>        // time
#include<ctime>         // time

using namespace std;

const clock_t begin_time = clock();

const int DATA_X_COORDINATE = 60;
const int DATA_Y_COORDINATE = 39;
const double CORE_RATIO = 0.5;
const int minSupportCount = 20;
const int maxPatterns = 2;

int data[DATA_X_COORDINATE][DATA_Y_COORDINATE];

typedef map<pair<int,int>, int> twoItemsetMap;
typedef map<vector<int>, int> threeItemsetMap;

threeItemsetMap pool;

struct alphaPattern
{
  vector<int> pattern;
  vector<int> core;

  bool operator< (const alphaPattern& alpha) const
  {
    if(pattern < alpha.pattern)
      return true;
    if(pattern > alpha.pattern)
      return false;
    return (core < alpha.core);
  }

};



// Function Prototypes

threeItemsetMap apriori();
bool contains(vector<int> tempVector, int value);
void createDiag40();
double distanceSet(vector<int> alpha, int alphaCount, vector<int> beta, 
		   int betaCount);
threeItemsetMap fusionSet(alphaPattern alpha, int& count);
threeItemsetMap patternFusion(threeItemsetMap pool, int size, double ratio);


int main()
{

  cout << "Data size: " << DATA_X_COORDINATE << " by " << DATA_Y_COORDINATE 
       << endl;
  cout << "With a minimum support of: " << minSupportCount << endl;
  cout << "With maximumu number of patterns to find: " << maxPatterns << endl;

  //int count = 0;
  srand(time(NULL));
  
  // creating transaction dataset
  createDiag40();
 
  // create 3-itemsets using Apriori
  pool = apriori();


  do
    {
      pool = patternFusion(pool, maxPatterns, CORE_RATIO);
    }while(pool.size() > maxPatterns);

  threeItemsetMap::iterator itBegin = pool.begin();
  threeItemsetMap::iterator itEnd = pool.end();
    
  for(itBegin; itBegin != itEnd; itBegin++)
    {
      if(itBegin->second >= minSupportCount)
	{
	  for(int i = 0; i < itBegin->first.size(); i++)
	    {
	      cout << itBegin->first[i] << ", ";
	    }
	  cout << " = " << itBegin->second << endl;
	}
      
    }

  cout << "Algorithm takes " << float(clock() - begin_time) / CLOCKS_PER_SEC 
       << " seconds to complete." << endl;

  return 0;
}




//********************************************************************
// Function: patternFusion
// Purpose: to fuse patterns into colossal patterns
// Return: a threeItemsetMap
//********************************************************************
threeItemsetMap patternFusion(threeItemsetMap pool, int size, double ratio)
{
  int value;
  int count;
  int randomNumber;

  alphaPattern temp;
  vector<alphaPattern> corePattern;

  threeItemsetMap tempReturn;
  threeItemsetMap tempFused;

  
  vector<int> tempVector;
  vector<int> tempVectorBeta;

  threeItemsetMap::iterator itTemp;

  threeItemsetMap::iterator itBegin = pool.begin();
  threeItemsetMap::iterator itEnd = pool.end();

  // looping through the maximum number of patterns to find and randomly
  //    choosing a pattern alpha
  for(int i = 0; i < size; i++)
    {
      itBegin = pool.begin();
      randomNumber = rand() % pool.size();

      for(int j = 0; j < randomNumber; j++)
	itBegin++;

      itTemp = itBegin;

      // tempVector now contains alpha's pattern
      tempVector = itTemp->first;

      itBegin = pool.begin();
      
      vector<int> coreList;

      for(itBegin; itBegin != itEnd; itBegin++)
	{
	  tempVectorBeta = itBegin->first;
	  
	  if(tempVector != tempVectorBeta)
	    if(distanceSet(tempVector, itTemp->second, tempVectorBeta, 
			   itBegin->second) <= CORE_RATIO)
	      {
		// if make it here tempVectorBeta is a core pattern to alpha
		for(int i = 0; i < tempVectorBeta.size(); i++)
		  {
		    if(!(contains(coreList, tempVectorBeta[i])))
		      coreList.push_back(tempVectorBeta[i]);
		  }
	      }
	  // storing alpha and alpha's corresponding core patterns

	  sort(tempVector.begin(), tempVector.end());
	  sort(coreList.begin(), coreList.end());
	  
	  temp.pattern = tempVector;
	  temp.core = coreList;
	}
      
      corePattern.push_back(temp);
    }

  sort(corePattern.begin(), corePattern.end());
 
  // fusing alpha and it's core patterns
  for(int i = 0; i < corePattern.size(); i++)
    {
      // alpha and it's core patterns are now fused together and stored in 
      //    tempFused
      tempFused = fusionSet(corePattern[i], count);
      
      threeItemsetMap::iterator itBeginTempFused = tempFused.begin();
      threeItemsetMap::iterator itEndTempFused = tempFused.end();

      if(tempReturn.empty())
	{
	  tempReturn = tempFused;
	}
      else
	{
	  for(itBeginTempFused; itBeginTempFused != itEndTempFused; 
	      itBeginTempFused++)
	    {
	      if(tempReturn.count(itBeginTempFused->first) > 0)
		{

		}
	      else
		tempReturn.insert(make_pair(itBeginTempFused->first, 
					    itBeginTempFused->second));
	    }
	}
    }
  
  threeItemsetMap::iterator itBeginTempReturn = tempReturn.begin();
  threeItemsetMap::iterator itEndTempReturn = tempReturn.end();
  
  vector<threeItemsetMap::iterator> itErase;

  for(itBeginTempReturn; itBeginTempReturn != itEndTempReturn; 
      itBeginTempReturn++)
    {
      if(itBeginTempReturn->second < minSupportCount)
	{
	  itErase.push_back(itBeginTempReturn);
	}
    }

  for(int i = 0; i < itErase.size(); i++)
    {
      tempReturn.erase(itErase[i]);
    }
   
   return tempReturn;
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


//*************************************************************
// Function: apriori
// Purpose: finds 3-itemsets using apriori
// Return: a threeItemsetMap containing the 3-itemsets
//*************************************************************
threeItemsetMap apriori()
{
  map<int, int> oneItemset;
  twoItemsetMap twoItemset;
  threeItemsetMap threeItemset;

  int position;
  int value;
  
  map<int, int>::iterator itBegin = oneItemset.begin();
  map<int, int>::iterator itEnd = oneItemset.end();

  twoItemsetMap::iterator itBeginTwo = twoItemset.begin();
  twoItemsetMap::iterator itEndTwo = twoItemset.end();

  threeItemsetMap::iterator itBeginThree = threeItemset.begin();
  threeItemsetMap::iterator itEndThree = threeItemset.end();
  
  pair<int, int> tempPair;

  // create 1-itemsets
  for(int i = 0; i < DATA_X_COORDINATE; i++)
    for(int j = 0; j < DATA_Y_COORDINATE; j++)
      {
	if(oneItemset.count(data[i][j]) > 0)
	  {
	    position = oneItemset[data[i][j]];
	    oneItemset[data[i][j]] = ++position;
	  }
	else
	  oneItemset.insert(map<int, int>::value_type(data[i][j], 1));
      }


  // remove any 1-itemsets that don't have minSupport
  for(itBegin; itBegin != itEnd; itBegin++)
      if(itBegin->second < minSupportCount)
	oneItemset.erase(itBegin->first);

  
  // create 2-itemsets
  for(int i = 0; i < DATA_X_COORDINATE; i++)
    for(int j = 0; j < DATA_Y_COORDINATE; j++)
      for(int k = 0; k < DATA_Y_COORDINATE; k++)
	if(data[i][j] != data[i][k])
	  {
	    tempPair = make_pair(data[i][j], data[i][k]);
	    if(twoItemset.count(tempPair) > 0)
	      {
		value = twoItemset[tempPair];
		twoItemset[tempPair] = ++value;
	      }
	    else
	      twoItemset.insert(make_pair(tempPair, 1));
	  }
  
  // remove any 2-itemsets that don't have minSupport		  
  for(itBeginTwo; itBeginTwo != itEndTwo; itEndTwo++)
      if(itBeginTwo->second < minSupportCount)
	twoItemset.erase(itBeginTwo->first);

  itBeginTwo = twoItemset.begin();
  
  // creating 3-itemsets
  
   for(int i = 0; i < DATA_X_COORDINATE; i++)
     for(int j = 0; j < DATA_Y_COORDINATE; j++)
       for(int k = j + 1; k < DATA_Y_COORDINATE; k++)
	 for(int h = k + 1; h < DATA_Y_COORDINATE; h++)
	   if((data[i][j] != data[i][k]) && (data[i][j] != data[i][h]) 
	      && (data[i][k] != data[i][h]))
	     {
	       vector<int> tempVector;
	       
	       tempVector.push_back(data[i][j]);
	       tempVector.push_back(data[i][k]);
	       tempVector.push_back(data[i][h]);
	       sort(tempVector.begin(), tempVector.end());
	       
	       if(threeItemset.count(tempVector) > 0)
		 {
		   value = threeItemset[tempVector];
		   threeItemset[tempVector] = ++value;
		 }
	       else
		 threeItemset.insert(make_pair(tempVector, 1));
	     }
     
   itBeginThree = threeItemset.begin();
   
   // remove any 3-itemsets that don't have minSupport		  
   for(itBeginThree; itBeginThree != itEndThree; itBeginThree++)
       if(itBeginThree->second < minSupportCount)
	 threeItemset.erase(itBeginThree->first);

   return threeItemset;
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

//*************************************************************************
// Function: distanceSet
// Purpose: determines the distance between two patterns alpha and beta
// Return: a double that is the calculated distance
//*************************************************************************
double distanceSet(vector<int> alpha, int alphaCount, vector<int> beta, 
		   int betaCount)
{
  double temp;
  double intersectSet = 0;
  double unionSet = 0;

  int countAlpha = 0;
  int countBeta = 0;

  vector<int> alphaTransactions;
  vector<int> betaTransactions;

  for(int i = 0; i < DATA_X_COORDINATE; i++)
    {
      for(int j = 0; j < DATA_Y_COORDINATE; j++)
	{
	  for(int k = 0; k < alpha.size(); k++)
	    if(alpha[k] == data[i][j])
	      countAlpha++;
	  for(int k = 0; k < beta.size(); k++)
	    if(beta[k] == data[i][j])
	      countBeta++;
	}
      // determining which transactions contain alpha
      if(alpha.size() == countAlpha)
	  alphaTransactions.push_back(i);

      // determining which transactions contain beta
      if(beta.size() == countBeta)
	  betaTransactions.push_back(i);
      
      countAlpha = 0;
      countBeta = 0;
    }

  // determining the cardinality of the intersection of the transactions 
  //    containing alpha and the transactions containing beta
  for(int i = 0; i < alphaTransactions.size(); i++)
    {
      for(int j = 0; j < betaTransactions.size(); j++)
	{
	  if(alphaTransactions[i] == betaTransactions[j])
	      intersectSet = intersectSet + 1;
	}
    }

  // determining the cardinality of the union of the transactions containing 
  //    alpha and the transactions containing beta
  unionSet = (alphaCount + betaCount) - intersectSet;

  temp = 1 - (intersectSet / unionSet);

  return temp;
}



//************************************************************************
// Function: fusionSet
// Purpose: fuses all of alpha's core patterns with alpha
// Return: returns a threeItemsetMap containing the fused pattern and the 
//         count of the fused pattern
//************************************************************************

threeItemsetMap fusionSet(alphaPattern alpha, int& count)
{
  threeItemsetMap fusedPattern;
  vector<int> tempPattern;
  int countPattern = 0;

  count = 0;
  
  // put alpha's pattern into tempPattern
  for(int i = 0; i < alpha.pattern.size(); i++)
    {
      tempPattern.push_back(alpha.pattern[i]);
    }

  // check to see if each integer stored in alpha's core is already in 
  //    tempPattern
  for(int i = 0; i < alpha.core.size(); i++)
    {
      if(!(contains(tempPattern, alpha.core[i])))
	tempPattern.push_back(alpha.core[i]);
    }

  sort(tempPattern.begin(), tempPattern.end());
  
  // check transaction dataset to see how many times tempPattern is found in 
  //    the transactions
  for(int i = 0; i < DATA_X_COORDINATE; i++)
    {
      for(int j = 0; j < DATA_Y_COORDINATE; j++)
	{
	  for(int k = 0; k < tempPattern.size(); k++)
	    {
	      if(tempPattern[k] == data[i][j])
		{
		  countPattern++;
		}

	    }
	}
      
      if(countPattern == tempPattern.size())
	count++;
      countPattern = 0;
    }

  fusedPattern.insert(make_pair(tempPattern, count));
  
  return fusedPattern;

}

