// Project Identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6
#include <getopt.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <queue>
#include <sstream>  

using namespace std;

struct Stock {
    char buyer_or_seller;
    int32_t small_dollars = 0;
    int32_t small_dollars2 = 0;
    int32_t dollars = 0;
    uint32_t shares;
    uint32_t stock_name;
    uint32_t trader_name; 
    int32_t id = 0;
    int32_t timestamp; 
    int32_t timestamp2; 
    int32_t timestamp3; 
};


struct BuyerComparator {
    bool operator()(const Stock &lhs, const Stock &rhs) {
        if (lhs.dollars == rhs.dollars) {
            return lhs.id < rhs.id;
        }
        return lhs.dollars < rhs.dollars;  
    }
};

struct SellerComparator {
    bool operator()(const Stock &lhs, const Stock &rhs) {
        if (lhs.dollars == rhs.dollars) {
            return lhs.id < rhs.id;
        }
        return lhs.dollars > rhs.dollars;  
    }
};

int32_t median_find(vector<int32_t> v) {
    sort(v.begin(), v.end());
    if (v.size() % 2 == 0){
        return (v[(v.size()/2 - 1)] + v[(v.size()/2)])/2;
    } else {
        return v[v.size()/2];  
    }
}



struct Stock_Sold {
    uint32_t who_bought;
    uint32_t seller; 
    uint32_t shares_sold;
    uint32_t stock_got_sold;
    int32_t net_transfer;
    int32_t net_median;
    uint32_t shares_bought;
    int32_t timestamp; 
};

class StockTrading {
private:
    string mode_for_stocks; 
    stringstream os; 
    vector<priority_queue<Stock, vector<Stock>, BuyerComparator>> buyers;
    vector<priority_queue<Stock, vector<Stock>, SellerComparator>> sellers;
    vector<vector<Stock>> time_traveling; 
    vector<Stock_Sold> output;
    int32_t trades_completed; 
    uint32_t num_traders;
    uint32_t num_stocks;
    int32_t current_timestamp; 
    bool print_median; 
    vector<vector<int32_t>> median_allocator;
    int id_number;
    vector<Stock> time_traveling_output;
    vector<int> current_max_output;
    vector<Stock_Sold> trader_info_output;
    bool VerboseOutput;
    bool MedianOutput;
    bool TimeTravelingOutput;
    bool TravelersInfoOutput;
     
    

public:
    StockTrading(string modeinput, uint32_t numberofstock, uint32_t numoftrad, bool v, bool m, bool tt, bool i) : mode_for_stocks(modeinput), trades_completed(0), num_traders(numoftrad), num_stocks(numberofstock), current_timestamp(0),
    VerboseOutput(v), MedianOutput(m), TimeTravelingOutput(tt), TravelersInfoOutput(i){
        Stock temp;
        Stock_Sold output2;
        output2.net_transfer = 0;
        output2.shares_bought = 0;
        output2.shares_sold = 0;
        output2.seller = 0; 
        temp.dollars = -1; 
        temp.small_dollars = -2;
        temp.small_dollars2 = -2;
        temp.timestamp = 0;
        temp.timestamp2 = 0; 
        buyers.resize(num_stocks);  
        sellers.resize(num_stocks);
        median_allocator.resize(num_stocks);
        time_traveling_output.resize(num_stocks, temp);
        current_max_output.resize(num_stocks, -1);
        trader_info_output.resize(num_traders, output2);
        id_number = 0;
        print_median = false;
    }
  

    void assortment() {
        for (uint32_t i = 0; i < sellers.size(); i++) {
            for (uint32_t j = 0; j < buyers.size(); j++) {
                if (i == j && !sellers[i].empty() && !buyers[j].empty()) {
                    if(sellers[i].top().dollars <= buyers[i].top().dollars){
                        if (sellers[i].top().shares > buyers[i].top().shares) {
                            Stock_Sold sold;
                            if(sellers[i].top().timestamp > buyers[i].top().timestamp){
                                sold.net_transfer = buyers[i].top().dollars;
                                sold.net_median = buyers[i].top().dollars;
                                sold.shares_bought = buyers[i].top().shares;
                                sold.stock_got_sold = sellers[i].top().stock_name;
                                sold.who_bought = buyers[i].top().trader_name;
                                sold.seller = sellers[i].top().trader_name;
                                trader_info_output[sold.who_bought].shares_bought += sold.shares_bought;
                                trader_info_output[sold.seller].shares_sold += sold.shares_bought;
                                trader_info_output[sold.seller].net_transfer += (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                trader_info_output[sold.who_bought].net_transfer -= (sold.net_transfer  * static_cast<int32_t>(sold.shares_bought));
                                output.push_back(sold);
                                median_allocator[i].push_back(sold.net_median);
                                trades_completed++;
                                Stock replacement = sellers[i].top();
                                replacement.shares -= buyers[i].top().shares;
                                buyers[i].pop();
                                sellers[i].pop();
                                sellers[i].push(replacement);
                                

                            } else if(sellers[i].top().timestamp < buyers[i].top().timestamp){
                                sold.net_transfer = sellers[i].top().dollars;
                                sold.net_median = sellers[i].top().dollars;
                                sold.shares_bought = buyers[i].top().shares;
                                sold.stock_got_sold = sellers[i].top().stock_name;
                                sold.who_bought = buyers[i].top().trader_name;
                                sold.seller = sellers[i].top().trader_name;
                                output.push_back(sold);
                                trades_completed++;
                                trader_info_output[sold.who_bought].shares_bought += sold.shares_bought;
                                trader_info_output[sold.seller].shares_sold += sold.shares_bought ;
                                trader_info_output[sold.seller].net_transfer += (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                trader_info_output[sold.who_bought].net_transfer -= (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                
                                median_allocator[i].push_back(sold.net_median);
                                Stock replacement = sellers[i].top();
                                replacement.shares -= buyers[i].top().shares;
                                buyers[i].pop();
                                sellers[i].pop();
                                sellers[i].push(replacement);


                            } else if(sellers[i].top().timestamp == buyers[i].top().timestamp){
                                if(sellers[i].top().id > buyers[i].top().id){
                                    sold.net_transfer = buyers[i].top().dollars;
                                    sold.net_median = buyers[i].top().dollars;
                                    sold.shares_bought = buyers[i].top().shares;
                                    sold.stock_got_sold = sellers[i].top().stock_name;
                                    sold.who_bought = buyers[i].top().trader_name;
                                    sold.seller = sellers[i].top().trader_name;
                                    output.push_back(sold);
                                    trades_completed++;
                                    trader_info_output[sold.who_bought].shares_bought += sold.shares_bought;
                                    trader_info_output[sold.seller].shares_sold += sold.shares_bought;
                                    trader_info_output[sold.seller].net_transfer += (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                    trader_info_output[sold.who_bought].net_transfer -= (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                  
                                    median_allocator[i].push_back(sold.net_median);
                                    Stock replacement = sellers[i].top();
                                    replacement.shares -= buyers[i].top().shares;
                                    buyers[i].pop();
                                    sellers[i].pop();
                                    sellers[i].push(replacement); 
                                } else {
                                    sold.net_transfer = sellers[i].top().dollars;
                                    sold.net_median = sellers[i].top().dollars;
                                    sold.shares_bought = buyers[i].top().shares;
                                    sold.stock_got_sold = sellers[i].top().stock_name;
                                    sold.who_bought = buyers[i].top().trader_name;
                                    sold.seller = sellers[i].top().trader_name;
                                    output.push_back(sold);
                                    trades_completed++;
                                    trader_info_output[sold.who_bought].shares_bought += sold.shares_bought;
                                    trader_info_output[sold.seller].shares_sold += sold.shares_bought;
                                    trader_info_output[sold.seller].net_transfer += (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                    trader_info_output[sold.who_bought].net_transfer -= (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                    median_allocator[i].push_back(sold.net_median);
                                    Stock replacement = sellers[i].top();
                                    replacement.shares -= buyers[i].top().shares;
                                    buyers[i].pop();
                                    sellers[i].pop();
                                    sellers[i].push(replacement); 
                                    
  
                                       
                                }
                            }

                        } else if (sellers[i].top().shares == buyers[i].top().shares){
                            Stock_Sold sold;
                            if(sellers[i].top().timestamp < buyers[i].top().timestamp){
                                sold.net_transfer = sellers[i].top().dollars;
                                sold.net_median = sellers[i].top().dollars;
                                sold.shares_bought = sellers[i].top().shares;
                                sold.stock_got_sold = sellers[i].top().stock_name;
                                sold.who_bought = buyers[i].top().trader_name;
                                sold.seller = sellers[i].top().trader_name;
                                output.push_back(sold);
                                trader_info_output[sold.who_bought].shares_bought += sold.shares_bought;
                                trader_info_output[sold.seller].shares_sold += sold.shares_bought;
                                trader_info_output[sold.seller].net_transfer += (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                trader_info_output[sold.who_bought].net_transfer -= (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                               
                                median_allocator[i].push_back(sold.net_median);
                                buyers[i].pop();
                                sellers[i].pop();
                                trades_completed++;


                            } 
                            else if(sellers[i].top().timestamp > buyers[i].top().timestamp){
                                sold.net_transfer = buyers[i].top().dollars;
                                sold.net_median = buyers[i].top().dollars;
                                sold.shares_bought = sellers[i].top().shares;
                                sold.stock_got_sold = sellers[i].top().stock_name;
                                sold.who_bought = buyers[i].top().trader_name;
                                sold.seller = sellers[i].top().trader_name;
                                trader_info_output[sold.who_bought].shares_bought += sold.shares_bought;
                                trader_info_output[sold.seller].shares_sold += sold.shares_bought;
                                trader_info_output[sold.seller].net_transfer += (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                trader_info_output[sold.who_bought].net_transfer -= (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                output.push_back(sold);
                                median_allocator[i].push_back(sold.net_median);
                                buyers[i].pop();
                                sellers[i].pop();
                        
                                trades_completed++;    
                            }
                            else if(sellers[i].top().timestamp == buyers[i].top().timestamp){
                                if(sellers[i].top().id > buyers[i].top().id){
                                    sold.net_transfer = buyers[i].top().dollars;
                                    sold.net_median = buyers[i].top().dollars;
                                    sold.shares_bought = sellers[i].top().shares;
                                    sold.stock_got_sold = sellers[i].top().stock_name;
                                    sold.who_bought = buyers[i].top().trader_name;
                                    sold.seller = sellers[i].top().trader_name;
                                    output.push_back(sold);
                                    median_allocator[i].push_back(sold.net_median);
                                    buyers[i].pop();
                                trader_info_output[sold.who_bought].shares_bought += sold.shares_bought;
                                trader_info_output[sold.seller].shares_sold += sold.shares_bought;
                                trader_info_output[sold.seller].net_transfer += (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                trader_info_output[sold.who_bought].net_transfer -= (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                            
                                    sellers[i].pop();
                                    trades_completed++;  
                                    
                                
                                } else {
                                    sold.net_transfer = sellers[i].top().dollars;
                                    sold.net_median = sellers[i].top().dollars;
                                    sold.shares_bought = sellers[i].top().shares;
                                    sold.stock_got_sold = sellers[i].top().stock_name;
                                    sold.who_bought = buyers[i].top().trader_name;
                                    sold.seller = sellers[i].top().trader_name;
                                trader_info_output[sold.who_bought].shares_bought += sold.shares_bought;
                                trader_info_output[sold.seller].shares_sold += sold.shares_bought;
                                trader_info_output[sold.seller].net_transfer += (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                trader_info_output[sold.who_bought].net_transfer -= (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                    output.push_back(sold);
                                    median_allocator[i].push_back(sold.net_median);
                                    buyers[i].pop();
                                    sellers[i].pop();
                                    trades_completed++;
                                    
                                }
                            }
                        } else if(sellers[i].top().shares < buyers[i].top().shares){
                            Stock_Sold sold;
                            if(sellers[i].top().timestamp < buyers[i].top().timestamp){
                                sold.net_transfer = sellers[i].top().dollars;
                                sold.net_median = sellers[i].top().dollars;
                                sold.shares_bought = sellers[i].top().shares;
                                sold.stock_got_sold = sellers[i].top().stock_name;
                                sold.who_bought = buyers[i].top().trader_name;
                                sold.seller = sellers[i].top().trader_name;
                                 output.push_back(sold);
                                trader_info_output[sold.who_bought].shares_bought += sold.shares_bought;
                                trader_info_output[sold.seller].shares_sold += sold.shares_bought;
                                trader_info_output[sold.seller].net_transfer += (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                trader_info_output[sold.who_bought].net_transfer -= (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                trades_completed++;
                                median_allocator[i].push_back(sold.net_median);
                                Stock replacement = buyers[i].top();
                                replacement.shares -= sellers[i].top().shares;
                                buyers[i].pop();
                                sellers[i].pop();
                                buyers[i].push(replacement);   
                                
 
                            }
                            else if(sellers[i].top().timestamp > buyers[i].top().timestamp){
                                sold.net_transfer = buyers[i].top().dollars;
                                sold.net_median = buyers[i].top().dollars;
                                sold.shares_bought = sellers[i].top().shares;
                                sold.stock_got_sold = sellers[i].top().stock_name;
                                sold.who_bought = buyers[i].top().trader_name;
                                sold.seller = sellers[i].top().trader_name;
                                trader_info_output[sold.who_bought].shares_bought += sold.shares_bought;
                                trader_info_output[sold.seller].shares_sold += sold.shares_bought;
                                trader_info_output[sold.seller].net_transfer += (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                trader_info_output[sold.who_bought].net_transfer -= (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                output.push_back(sold);
                                trades_completed++;
                                median_allocator[i].push_back(sold.net_median);
                                Stock replacement = buyers[i].top();
                                replacement.shares -= sellers[i].top().shares;
                                buyers[i].pop();
                                sellers[i].pop();
                                buyers[i].push(replacement);       
                            }
                            else if(sellers[i].top().timestamp == buyers[i].top().timestamp){
                                if(sellers[i].top().id > buyers[i].top().id){
                                    sold.net_transfer = buyers[i].top().dollars;
                                    sold.net_median = buyers[i].top().dollars;
                                    sold.shares_bought = sellers[i].top().shares;
                                    sold.stock_got_sold = sellers[i].top().stock_name;
                                    sold.who_bought = buyers[i].top().trader_name;
                                    sold.seller = sellers[i].top().trader_name;
                                    trader_info_output[sold.who_bought].shares_bought += sold.shares_bought;
                                    trader_info_output[sold.seller].shares_sold += sold.shares_bought;
                                    trader_info_output[sold.seller].net_transfer += (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                    trader_info_output[sold.who_bought].net_transfer -= (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                    output.push_back(sold);
                                    median_allocator[i].push_back(sold.net_median);
                                    trades_completed++;
                                    Stock replacement = buyers[i].top();
                                    replacement.shares -= sellers[i].top().shares;
                                    buyers[i].pop();
                                    sellers[i].pop();
                                    buyers[i].push(replacement);
                                } else {
                                    sold.net_transfer = sellers[i].top().dollars;
                                    sold.net_median = sellers[i].top().dollars;
                                    sold.shares_bought = sellers[i].top().shares;
                                    sold.stock_got_sold = sellers[i].top().stock_name;
                                    sold.who_bought = buyers[i].top().trader_name;
                                    sold.seller = sellers[i].top().trader_name;
                                    trader_info_output[sold.who_bought].shares_bought += sold.shares_bought;
                                    trader_info_output[sold.seller].shares_sold += sold.shares_bought;
                                    trader_info_output[sold.seller].net_transfer += (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                    trader_info_output[sold.who_bought].net_transfer -= (sold.net_transfer * static_cast<int32_t>(sold.shares_bought));
                                    output.push_back(sold);
                                    median_allocator[i].push_back(sold.net_median);
                                    trades_completed++;
                                    Stock replacement = buyers[i].top();
                                    replacement.shares -= sellers[i].top().shares;
                                    buyers[i].pop();
                                    sellers[i].pop();
                                    buyers[i].push(replacement);

                                }
                            }

                        }

                    }
                    }
                }
            
        }

        if((output.size() > 0 && VerboseOutput)){
            for(uint32_t i = 0; i < output.size(); i++){
                cout << "Trader " << output[i].who_bought << " purchased " << output[i].shares_bought
                << " shares of Stock " << output[i].stock_got_sold << " from Trader " << output[i].seller <<
                 " for $"<<  output[i].net_transfer << "/share" << '\n';
            }
  
            output.clear();
        }

    }

    void statements() {
        cout << "---End of Day---" << "\n";
        cout << "Trades Completed: " << trades_completed << "\n";
        if (TravelersInfoOutput) {
            cout << "---Trader Info---" << '\n';
            for(uint32_t i = 0; i < trader_info_output.size(); i++){
                cout << "Trader " << i << " bought " << trader_info_output[i].shares_bought << 
                " and sold " << trader_info_output[i].shares_sold << " for a net transfer of $" << 
                trader_info_output[i].net_transfer << "\n";
            }
        }

        if(TimeTravelingOutput){
            cout << "---Time Travelers---" << "\n";
            for(uint32_t i = 0; i < time_traveling_output.size(); i++){
                if(time_traveling_output[i].dollars != -1){
                    cout << "A time traveler would buy Stock " << i << " at time " << 
                    time_traveling_output[i].timestamp2 << " for $" << time_traveling_output[i].small_dollars << " and sell it at "
                    << "time " << time_traveling_output[i].timestamp << " for $" << time_traveling_output[i].dollars << "\n";

                } else {
                    cout << "A time traveler could not make a profit on Stock " << i << "\n"; 
                }
            }
        }


    }

    void reading_input(stringstream &s) {
    int32_t time;
    string sorb;
    char junk; // For consuming extra characters
    uint32_t tradernum;
    uint32_t stocknum;
    int32_t price; 
    uint32_t amtshares;
    

    cout << "Processing orders..." << '\n';
    string line;
    if(mode_for_stocks == "PR"){
        while (getline(s, line)) { 
        istringstream iss(line);
        if (!(iss >> time >> sorb >> junk >> tradernum >> junk >> stocknum >> junk >> price >> junk >> amtshares)) {
            continue; 
        }

        
        
        // Print to debug what has been read
       // cout << time << " " << sorb << " " << tradernum << " " << stocknum << " " << price << " " << amtshares << endl;
      
        assortment();
        
                        
        Stock inputted;
        
        
        inputted.timestamp = time;
        inputted.trader_name = tradernum;
        inputted.stock_name = static_cast<uint32_t>(stocknum);  
        inputted.dollars = price;
        
        inputted.shares = amtshares;
        inputted.id = id_number;
        mode_for_stocks = "tl";

        if(time < 0){
            cerr << "Error: Negative timestamp" << "\n";
        }

        if(price < 0){
            cerr << "Error: Invalid price" << "\n";
        }


        if(id_number == 0 && sorb == "BUY" && time_traveling_output[stocknum].small_dollars != -2){
            time_traveling_output[stocknum].dollars = inputted.dollars;
            time_traveling_output[stocknum].timestamp = inputted.timestamp;

        }

        if(id_number !=0){
            if(sorb == "BUY"){
                if(time_traveling_output[stocknum].small_dollars != -2 && inputted.dollars > time_traveling_output[stocknum].small_dollars2){
                    if(time_traveling_output[stocknum].dollars < inputted.dollars){
                        time_traveling_output[stocknum].dollars = inputted.dollars;
                        time_traveling_output[stocknum].timestamp = inputted.timestamp;
                    }
                }
            }
        }

        if(sorb == "SELL"){
        if(time_traveling_output[stocknum].dollars > inputted.dollars || time_traveling_output[stocknum].dollars == -1){
            if(time_traveling_output[stocknum].small_dollars == -2){
                time_traveling_output[stocknum].small_dollars = inputted.dollars;
                time_traveling_output[stocknum].small_dollars2 = inputted.dollars;
                time_traveling_output[stocknum].timestamp2 = inputted.timestamp;
                time_traveling_output[stocknum].timestamp3 = inputted.timestamp; 
            }
            if(time_traveling_output[stocknum].small_dollars > inputted.dollars){

                if(inputted.timestamp >= time_traveling_output[stocknum].timestamp){
             
                    if(time_traveling_output[stocknum].small_dollars2 > inputted.dollars){
                   
                        time_traveling_output[stocknum].small_dollars2 = inputted.dollars;
                        time_traveling_output[stocknum].timestamp3 = inputted.timestamp;
                        
                    }
                } else {
                    time_traveling_output[stocknum].small_dollars = inputted.dollars;
                    time_traveling_output[stocknum].timestamp2 = inputted.timestamp;
                }
            }
            }
        }

        if(time_traveling_output[stocknum].small_dollars != -2){
            if(time_traveling_output[stocknum].timestamp3 < time_traveling_output[stocknum].timestamp){
                time_traveling_output[stocknum].timestamp2 = time_traveling_output[stocknum].timestamp3;
                time_traveling_output[stocknum].small_dollars = time_traveling_output[stocknum].small_dollars2;
            }
        }


        

       
        if (sorb == "SELL") {
            inputted.buyer_or_seller = 's';
            
            sellers[stocknum].push(inputted);
        } else if (sorb == "BUY") {
            inputted.buyer_or_seller = 'b';
            buyers[stocknum].push(inputted);
        } else {
            cerr << "Error: Invalid stock ID" << "\n";
        }

        if(current_timestamp > inputted.timestamp){
            cerr << "Error: Decreasing timestamp" << "\n";
        }

        if (current_timestamp != inputted.timestamp) {
            print_median = true;
            if(print_median && MedianOutput){
                for(uint32_t i = 0; i < num_stocks; i++){
                    if(median_allocator[i].size() > 0){
                        int32_t median = median_find(median_allocator[i]);
                        cout << "Median match price of Stock " << i << " at time " << current_timestamp <<
                        " is $" << median << "\n";
                    }
                }
                
            }
            current_timestamp = inputted.timestamp;
        }

        //cout << "inputted " << endl;
        assortment();
        //cout << print_median;
        id_number++;
    }
    }
    while (getline(cin, line)) { 
        istringstream iss(line);
        if (!(iss >> time >> sorb >> junk >> tradernum >> junk >> stocknum >> junk >> price >> junk >> amtshares)) {
            continue; 
        }

        
        
        // Print to debug what has been read
       // cout << time << " " << sorb << " " << tradernum << " " << stocknum << " " << price << " " << amtshares << endl;
      
        assortment();
        
                        
        Stock inputted;
        
        
        inputted.timestamp = time;
        inputted.trader_name = tradernum;
        inputted.stock_name = static_cast<uint32_t>(stocknum);  
        inputted.dollars = price;
        
        inputted.shares = amtshares;
        inputted.id = id_number;
        mode_for_stocks = "tl";

        if(time < 0){
            cerr << "Error: Negative timestamp" << "\n";
        }

        if(price < 0){
            cerr << "Error: Invalid price" << "\n";
        }


        if(id_number == 0 && sorb == "BUY" && time_traveling_output[stocknum].small_dollars != -2){
            time_traveling_output[stocknum].dollars = inputted.dollars;
            time_traveling_output[stocknum].timestamp = inputted.timestamp;

        }

        if(id_number !=0){
            if(sorb == "BUY"){
                if(time_traveling_output[stocknum].small_dollars != -2 && inputted.dollars > time_traveling_output[stocknum].small_dollars2){
                    if(time_traveling_output[stocknum].dollars < inputted.dollars){
                        time_traveling_output[stocknum].dollars = inputted.dollars;
                        time_traveling_output[stocknum].timestamp = inputted.timestamp;
                    }
                }
            }
        }

        if(sorb == "SELL"){
        if(time_traveling_output[stocknum].dollars > inputted.dollars || time_traveling_output[stocknum].dollars == -1){
            if(time_traveling_output[stocknum].small_dollars == -2){
                time_traveling_output[stocknum].small_dollars = inputted.dollars;
                time_traveling_output[stocknum].small_dollars2 = inputted.dollars;
                time_traveling_output[stocknum].timestamp2 = inputted.timestamp;
                time_traveling_output[stocknum].timestamp3 = inputted.timestamp; 
            }
            if(time_traveling_output[stocknum].small_dollars > inputted.dollars){

                if(inputted.timestamp >= time_traveling_output[stocknum].timestamp){
             
                    if(time_traveling_output[stocknum].small_dollars2 > inputted.dollars){
                   
                        time_traveling_output[stocknum].small_dollars2 = inputted.dollars;
                        time_traveling_output[stocknum].timestamp3 = inputted.timestamp;
                        
                    }
                } else {
                    time_traveling_output[stocknum].small_dollars = inputted.dollars;
                    time_traveling_output[stocknum].timestamp2 = inputted.timestamp;
                }
            }
            }
        }

        if(time_traveling_output[stocknum].small_dollars != -2){
            if(time_traveling_output[stocknum].timestamp3 < time_traveling_output[stocknum].timestamp){
                time_traveling_output[stocknum].timestamp2 = time_traveling_output[stocknum].timestamp3;
                time_traveling_output[stocknum].small_dollars = time_traveling_output[stocknum].small_dollars2;
            }
        }


        

       
        if (sorb == "SELL") {
            inputted.buyer_or_seller = 's';
            
            sellers[stocknum].push(inputted);
        } else if (sorb == "BUY") {
            inputted.buyer_or_seller = 'b';
            buyers[stocknum].push(inputted);
        } else {
            cerr << "Error: Invalid stock ID" << "\n";
        }

        if(current_timestamp > inputted.timestamp){
            cerr << "Error: Decreasing timestamp" << "\n";
        }

        if (current_timestamp != inputted.timestamp) {
            print_median = true;
            if(print_median && MedianOutput){
                for(uint32_t i = 0; i < num_stocks; i++){
                    if(median_allocator[i].size() > 0){
                        int32_t median = median_find(median_allocator[i]);
                        cout << "Median match price of Stock " << i << " at time " << current_timestamp <<
                        " is $" << median << "\n";
                    }
                }
                
            }
            current_timestamp = inputted.timestamp;
        }

        //cout << "inputted " << endl;
        assortment();
        //cout << print_median;
        id_number++;
    }
    if(MedianOutput){
                        for(uint32_t i = 0; i < num_stocks; i++){
                    if(median_allocator[i].size() > 0){
                        int32_t median = median_find(median_allocator[i]);
                        cout << "Median match price of Stock " << i << " at time " << current_timestamp <<
                        " is $" << median << "\n";
                    }
                }
    }


        statements();
    }
};
