#include <stdio.h>

#define MBITTOBIT 1000000 // Multiply to convert MBits in Bits (or devide for the reverse)
#define NO 0

int main(){
    int packetSize;
    int headerSize;
    int nodesNum;
    int fragmentSize;
    int again = 1;
    float delayWithFragmentation;
    float delayWithoutFragmentation;

    printf("This is a program which calculates the delay between two terminals in a network.\n");
    printf("Before we calculate the delay you must please enter the appropriate data.\n");

    while (!(again == NO)){
        printf("\nEnter average packet size in bits: ");
        scanf("%d", &packetSize);
        while(packetSize<1){
            printf("Packet size must be higher than zero.\nPlease enter again: ");
            scanf("%d", &packetSize);
        }// while

        printf("\nEnter header size of each packet/fragment in bits: ");
        scanf("%d", &headerSize);
        while(headerSize<1){
            printf("Header size must be higher than zero.\nPlease enter again: ");
            scanf("%d", &headerSize);
        }// while

        printf("\nEnter number of nodes that exists between the terminals: ");
        scanf("%d", &nodesNum);
        while(nodesNum<1){
            printf("At least one node should exist between the terminals.\nPlease enter again: ");
            scanf("%d", &nodesNum);
        }// while

        float bitRate[nodesNum+1];

        printf("\nNow you must bit rate for each link in Mbps ");

        for (int i=0; i<nodesNum+1; i++){
            printf("\nEnter bit rate for the link number %d: ", i+1);
            scanf("%f", &bitRate[i]);
            while(bitRate[i]<=0){
                printf("Bit rate must be higher than zero.\nPlease enter again: ");
                scanf("%f", &bitRate[i]);
            }// while
            bitRate[i] *= MBITTOBIT; //Mbps conversion to bps
        }// for
        
        printf("\nEnter fragment size in bits (in case of fragmentation): ");
        scanf("%d", &fragmentSize);
        while(fragmentSize<1){
            printf("Fragment size must be higher than zero.\nPlease enter again: ");
            scanf("%d", &fragmentSize);
        }// while

        // Calculating the time it takes for packet/fragments to leave first terminal and reach first node
        float totalFragments = packetSize/fragmentSize;

        delayWithFragmentation = totalFragments * (fragmentSize+headerSize) / bitRate[0];
        delayWithoutFragmentation = (packetSize+headerSize) / bitRate[0];

        // Calculating the time it takes for packet/last fragment to travel from the first node to the second terminal
        float linkDelayWithFragmentation;
        float linkDelayWithoutFragmentation;
        for (int i=1; i<nodesNum+1; i++){
            linkDelayWithFragmentation = (fragmentSize+headerSize) / bitRate[i];
            linkDelayWithoutFragmentation = (packetSize+headerSize) / bitRate[i];

            delayWithFragmentation += linkDelayWithFragmentation;
            delayWithoutFragmentation += linkDelayWithoutFragmentation;
        }// for

        printf("\n======================================================================================================");
        printf("\nDelay in seconds WITH fragmentation: %0.2f", delayWithFragmentation);
        printf("\n======================================================================================================");
        printf("\n======================================================================================================");
        printf("\nDelay in seconds WITHOUT fragmentation: %0.2f", delayWithoutFragmentation);
        printf("\n======================================================================================================");
        
        printf("\n\nWould you like to again?\nEnter 1 for Yes or 0 for No: ");
        scanf("%d", &again);
    }// while
}// main