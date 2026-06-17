#include <stdio.h>
#include <stdlib.h>

int  main(int argc, char* argv[]) {
    int num_inputs;
    int* ticks;
    int i;
    
    if( scanf("%d",&num_inputs) != 1){
        return __LINE__;
    }
    

    ticks = malloc((unsigned long)num_inputs * sizeof(int));
    
    // get the input 
    for(i=0; i < num_inputs; i++ ){
        if (scanf("%d",&ticks[i]) != 1){
            return __LINE__;
        }
    }    
    
    int max_profit = 0;
    // brute force calulate 
    for(i=0; i < num_inputs;i++ ){
        int profit = 0;
        for(int buy=0; buy<num_inputs-1;buy++){
            for(int sell=buy+1;sell<num_inputs;sell++){
                profit = ticks[sell] - ticks[buy];
                if(profit > max_profit){
                   max_profit = profit;
                }                
            }
        }    
    }
    
    
    
    
    
    
    
    // convert array into 
    printf("%d\n",max_profit);
    
    
    return 0;
}
