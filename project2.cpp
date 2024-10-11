// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6
#include "project2.hpp"
#include "P2random.h"
#include <getopt.h>
#include <iostream>
#include <sstream>



int main(int argc, char* argv[]){
    int option_index = 0;
    int verbose_flag = 0, median_flag = 0, trader_info_flag = 0, time_travelers_flag = 0;

   
    static struct option long_options[] = {
        {"verbose", no_argument, &verbose_flag, 1},
        {"median", no_argument, &median_flag, 1},
        {"trader_info", no_argument, &trader_info_flag, 1},
        {"time_travelers", no_argument, &time_travelers_flag, 1},
        {0, 0, 0, 0} 
    };

   
    int c;
    while ((c = getopt_long(argc, argv, "vmit", long_options, &option_index)) != -1) {
        switch (c) {
            case 0:
                break;
            case 'v':
                verbose_flag = 1;
                break;
            case 'm':
                median_flag = 1;
                break;
            case 'i':
                trader_info_flag = 1;
                break;
            case 't':
                time_travelers_flag = 1;
                break;
            case '?':
                cout << "Error: Unknown command line option" << "\n";
                return EXIT_FAILURE;
            default:
                break;
        }
    }

    bool v = false; 
    bool m = false; 
    bool t = false;
    bool tt = false;


    // After parsing, you can check the flags and run the corresponding logic
    if (verbose_flag) {
       
        v = true;
    }
    if (median_flag) {
        
       m = true; 
    }
    if (trader_info_flag) {
    
        t = true;
    }
    if (time_travelers_flag) {
        
        tt = true; 
    }




    string comment; 
    string mode_label;
    string mode; 
    string num_traders_label;
    uint32_t num_traders;
    string num_stockers_label;
    uint32_t num_stocks; 
    string seed_label; 
    uint32_t seed;
    string order_label;
    uint32_t order;
    string arrival_label;
    uint32_t arrival; 


    

    getline(cin, comment);
    cin >> mode_label;
    cin >> mode;
    cin >> num_traders_label;
    cin >> num_traders;
    cin >> num_stockers_label;
    cin >> num_stocks;
    stringstream ss2;
   
    if(mode == "PR"){
        cin >> seed_label;
        cin >> seed;
        cin >> order_label;
        cin >> order;
        cin >> arrival_label;
        cin >> arrival;
        P2random::PR_init(ss2, seed, num_traders, num_stocks, order, arrival);
    }

    
    StockTrading classi(mode, num_stocks, num_traders, v, m, tt, t);
    classi.reading_input(ss2);
    
    





}