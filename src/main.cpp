#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <set>
#include <utility>
#include <map>

using namespace std;

map<pair<unsigned int, unsigned int>, unsigned int > cache;

int findMax(unsigned int row, unsigned int column, vector< vector<int> > &array,int side)
{
   if (row >= array.size())
   {
      return 0;
   }

   if (cache.count(make_pair(row,column)))
   {
      return cache[make_pair(row,column)];
   }

   int left;
   int right;

   if (side == 0)
   {  
      left = findMax(row + 1,column,array,0);
      right = findMax(row + 1,column +1,array,1);
   }

   else
   {
      right = findMax(row + 1,column +1,array,1);
      left = findMax(row + 1,column,array,0);
   }

   int max = (left > right ? left: right) + array.at(row).at(column);

   cache.insert(make_pair(make_pair(row,column),max));
   return max;
}


int main()
{
   vector< vector<int> > array;
   vector<int> empty;

   ifstream file("lol");
   
   stringstream lol;
   int i =0;
   while (file.get(*lol.rdbuf()))
   {
      array.push_back(empty);

      int test;
      while (lol>>test)
      {   
         array[i].push_back(test);
      }

      lol.clear();
      file.get();
      i++;
   }

   for (int unsigned i = 0;i<array.size();i++)
   {   
      for (unsigned int l =0;l<array[i].size();l++)
      {
         cout<<array[i][l]<<' ';
      }
      cout<<endl;
   }


   int max = findMax(0,0,array,0);

   cout<<max<<endl;

   
}
