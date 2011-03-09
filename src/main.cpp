#include <iostream>
#include <boost/spirit/include/qi.hpp>
#include <string>
#include <boost/bind.hpp>
#include <vector>


namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

using boost::ref;
using boost::bind;

using std::string;
using std::cout;
using std::endl;
using std::vector;

template <typename Iterator>
bool parse_numbers(Iterator first, Iterator lasti,double &number);

double parse(vector<char> &opers, vector<double> &nums);
void takeChar(char , vector<char> &);
void takeDouble(double , vector<double> & );

int main(int argv, char **argc)
{
   if (argv == 1)
   {
      cout<<"Wrong number of arguments"<<endl;
      return -2;
   }

   string mySrc = "";   

   for (int i = 1; i< argv;i++)
   {
      mySrc += ' ';
      mySrc += argc[i];
   }

   cout<<mySrc<<endl;

   double number;
   if (parse_numbers(mySrc.begin(),mySrc.end(),number))
   {
      cout<<"Yay it worked, with a result of "<<number<<endl;
   }
   
   else
   {
      cout<<"No it did not work."<<endl;
   }
}

template <typename Iterator>
bool parse_numbers(Iterator first, Iterator last,double &number)
{
    vector<double> nums;
    vector<char> opers;

    using qi::double_;
    using qi::char_;
    using qi::phrase_parse;
    using ascii::space;

    bool r = phrase_parse(
        first,                          
        last,                           
        *char_('(')[bind(takeChar,_1,ref(opers))] >> double_[bind(takeDouble,_1,ref(nums))] >> *(char_[bind(takeChar,_1,ref(opers))] >> *char_("(")[bind(takeChar,_1,ref(opers))] >> double_[bind(takeDouble,_1,ref(nums))] >> *char_(')')[bind(takeChar,_1,ref(opers))] ),
        space                           
    );

    if (first != last) // fail if we did not get a full match
        return false;
   
   number = parse(opers,nums);

    return r;
}

void takeChar(char lol,vector<char> &opers)
{
   //cout<<"I got a char "<<lol<<endl;
   opers.push_back(lol);
}

void parseAdd(vector<char> &opers, vector<double> &nums,int start, int end, int &mod)
{
   int i = start;
   while ( i< end)
   {
            
      char last = opers[i]; 
      double & num = nums[i];
      const double & lol = nums[i + 1];

      if (last != '-' && last != '+')
      {  
         //cout<<"Not addition or subtraction, so skipping"<<endl;
         i++;
         continue;
      } 

      //cout<<"Doing "<<last<<" to "<<num<<" and "<<lol<<endl;
      switch (last)
      {
         case '-':
            num-=lol;
            break;

         case '+':
            num+=lol;
            break;

         default:
            cout<<"Match did not work"<<endl;
      }
   
      nums.erase(nums.begin() + i + 1);
      opers.erase(opers.begin() + i);
      end--;
      mod--;
   }
}

void parseMult(vector<char> &opers, vector<double> &nums,int start,int end,int &mod)
{
   int i =start;
   while ( i< end)
   {
            
      char last = opers[i]; 
      double & num = nums[i];
      const double & lol = nums[i + 1];

      if (last != '/' && last != '*')
      {  
         //cout<<"Not a division, so skipping"<<endl;
         i++;
         continue;
      } 

      //cout<<"Doing "<<last<<" to "<<num<<" and "<<lol<<endl;
      switch (last)
      {
         case '*':
            num*=lol;
            break;
      
         case '/':
            num/=lol;
            break;
      
         default:
            cout<<"Match did not work"<<endl;
      }
   
      nums.erase(nums.begin() + i + 1);
      opers.erase(opers.begin() + i);
      end--;
      mod--;
   }
}

void parseParen(vector<char> &opers, vector<double> &nums, int start,int &end)
{
   int i =start;
   while ( i< end)
   {
      char last = opers[i]; 
      double & num = nums[i];
      const double & lol = nums[i + 1];

      if (last != '(' && last != ')')
      {  
         cout<<"This "<<last<<" at "<<i<<" is not a parenthesis, so skipping"<<endl;
         i++;
         continue;
      } 
      
      opers.erase(opers.begin() + i);
      end--;

      cout<<"Doing "<<last<<" to "<<num<<" and "<<lol<<endl;
      switch (last)
      {
         case '(':
            parseParen(opers,nums,i,end);
            break;
      
         case ')':
            parseMult(opers,nums,start,i,end);
            parseAdd(opers,nums,start,i,end);
            cout<<"Parenthesis ended"<<endl;
            return;
            break;
      
         default:
            cout<<"Match did not work"<<endl;
      }
   
   }
} 

double parse(vector<char> &opers, vector<double> &nums)
{
   int lol = 0;
   int end = opers.size();

   parseParen(opers,nums,0,end);
   //cout<<"Finished parsing parentheses"<<endl;

   end = opers.size();
   parseMult(opers,nums,0,end,lol);

   end = opers.size();
   parseAdd(opers,nums,0,end,lol);
   
   return nums[0];
} 

void takeDouble(double lol,vector<double> &nums)
{
   //cout<<"I got a double "<<lol<<endl;
   nums.push_back(lol);
}
