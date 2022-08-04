//
//  main.cpp
//  maxMachineProfit
//
//  Created by TracyChang on 2022/8/3.
//

#include <iostream>
//#include <bits/stdc++.h>
#include <vector>
using namespace std;

#define INPUT_FILE (const char*)"input.txt"

// The Structure
struct AdvanceManchime
{
     std::string name;
     int saleDay;        // machine sale day
     int buyPrice;      // machine bought price
     int resalePrice;   // machine resell price
     int profit;        // machine generate profit per day
     AdvanceManchime(){}
};

struct less_than_key
{
     inline bool operator() (const AdvanceManchime& mach1,
                             const AdvanceManchime& mach2)
     {
         return (mach1.saleDay < mach2.saleDay);
     }
};


//---------------
// calMaxProfit
//---------------
int calMaxProfit(
    vector<AdvanceManchime>const      &machines,
    vector<AdvanceManchime>::iterator ix,
    int                               currenProfit,
    int                               endDate)
{
     //---------------
     //Calculate the final profit amount without buy any other
     //machine and also include resale price
     //---------------
     int profit_t0 = currenProfit + ((endDate)-(ix->saleDay)) * ix->profit + ix->resalePrice;
     //cout << "Name=" << ix->name<< " profit_t0=" <<profit_t0 << "currenProfit=" << currenProfit << endl;

     // if reach end
     if (ix == machines.end())
     {
         return profit_t0;
     }
     //---------------
     //Find next available machine to buy
     //---------------
     vector<AdvanceManchime>::iterator iy2;
     for (iy2 = ix+1; iy2 != machines.end(); iy2++)
     {
         //---------------
         //Need to consider how much money CVCM owns on the day of next machine on sale
         //---------------
         int profit_t1 = currenProfit + ((iy2->saleDay-1) - (ix->saleDay)) * ix->profit + ix->resalePrice;
        
         //---------------
         //If enough money to buy
         //---------------
         if( profit_t1 >= iy2->buyPrice)
         {
             //cout << "[o]" << iy2->name << endl;
             int temp_pro = calMaxProfit(machines, iy2, profit_t1 - iy2->buyPrice , endDate);
             //---------------
             //Update the max profit
             //---------------
             return max( profit_t0,  temp_pro);
         }
         //cout << "[x]" << iy2->name << endl;
     }
     return profit_t0;
}


//---------------
// Main
//---------------
int main()
{
     //---------------
     // Read File
     //---------------
     FILE *fp = NULL;
     if ( (fp = fopen(INPUT_FILE, "r")) == NULL )
     {
         cout << "File is not exist\n";
         return 1;
     }

     int case_no = 0;
     while(1)
     {
         char line[64];
         int f1 = 0, f2 = 0, f3 = 0, f4 = 0;

         memset(line, '\0', sizeof(line));
         if ( fgets(line, sizeof(line), fp) == NULL ) { cout << "Not Found 0 0 0 for end!!\n"; fclose(fp);return 2; }
         int cnt = sscanf(line, "%d %d %d", &f1, &f2, &f3);
         if ( cnt != 3 ) { cout << "File Format Is Not Correct!!\n"; fclose(fp); return 3; }
         if ( f1 == 0 && f2 == 0 && f3 == 0 ) { cout << "End of File!!\n"; fclose(fp); return 0; }

         int numMachine = f1;
         int initAmount = f2;
         int endDate    = f3;

         vector<AdvanceManchime> gMachinesVec;
         vector<AdvanceManchime>::iterator it;

         //---------------
         // Initial Data
         //---------------
         for (int m = 0; m < numMachine; m++)
         {
             memset(line, '\0', sizeof(line));
             fgets(line, sizeof(line), fp);
             cnt = sscanf(line, "%d %d %d %d", &f1, &f2, &f3, &f4);
             if ( cnt != 4 ) { cout << "File Format Is Not Correct!!\n"; fclose(fp); return 2; }
             if ( f1 == 0 ) { continue; }

             AdvanceManchime aMachine = AdvanceManchime();
             aMachine.name        = "M_" + std::to_string(m);
             aMachine.saleDay     = f1;
             aMachine.buyPrice    = f2;
             aMachine.resalePrice = f3;
             aMachine.profit      = f4;
             gMachinesVec.push_back(aMachine);
         }
         //---------------
         //Sortig machine by its sale day
         //---------------
         std::sort(gMachinesVec.begin(), gMachinesVec.end(), less_than_key());
         
         int overallMax = initAmount;
         //---------------
         //Find next available machine to buy
         //---------------
         for (it = gMachinesVec.begin(); it != gMachinesVec.end(); ++it)
         {
             int maxProfit = initAmount;
             //---------------
             //If enough money to buy
             //---------------
             if ( maxProfit >= it->buyPrice )
             {
                 //---------------
                 //Update the max profit
                 //---------------
                 maxProfit= max( maxProfit , calMaxProfit(gMachinesVec, it, maxProfit - it->buyPrice, endDate) );
             }
             overallMax = max( overallMax ,maxProfit );
         }
         cout << "Case " << ++case_no << ": "  << overallMax << endl;
     }

     return 0;

}
