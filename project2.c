#include <stdio.h>
#include <float.h>

#define MBITTOBIT 1000000 // Multiply to convert MBits in Bits (or devide for the reverse)
#define MBYTETOBIT 8000000 // Multiply to convert MBytes in Bits (or devide for the reverse)
#define KBYTETOBIT 8000 // Multiply to convert KBytes in Bits (or devide for the reverse)
#define SECTOMILISEC 1000 // Multiply to convert secs in msecs (or devide for the reverse)
#define UPPERLIMIT DBL_MAX // Will represent "positive infinity" for practical purposes
#define YES 1
#define NO 0

float checkNumber(float number, float low, float high);
void printReceivedBits(float propDelay, float totalDelay, float startTime, float endTime, float bitRate, float extraDelayPercentage);

/*====================================================================================================================================*/
/*=========================================================== MAIN ===================================================================*/
/*====================================================================================================================================*/

int main(){
    float propDelay; // Propagation Delay, in secs
    float bitRate; // Bit rate of the link, in bits/sec
    float packetSize; // Average packet size, in bits
    float extraDelayPercentage; // Percentage of total delay that corresponds to queueing and processing delay
    float extraDelay; // Sum of queueing and processing delay in secs
    float totalDelay;
    int choice = 1; // If 0, program stops

    while (choice != 0){

/*------------------------------------------------------Getting input from user-------------------------------------------------------*/

        printf("Enter the propagation delay in msecs: ");
        scanf("%f", &propDelay);
        propDelay = checkNumber(propDelay, 0, UPPERLIMIT);
        propDelay = propDelay/SECTOMILISEC;

        printf("Enter the bit rate of the link between the nodes (in Mbps): ");
        scanf("%f", &bitRate);
        bitRate = checkNumber(bitRate, 0, UPPERLIMIT);
        bitRate = bitRate*MBITTOBIT;

        printf("Enter the average packet size in KBytes: ");
        scanf("%f", &packetSize);
        packetSize = checkNumber(packetSize, 0, UPPERLIMIT);
        packetSize = packetSize*KBYTETOBIT;

        printf("Enter the sum of processing and queueing delay in the form of a percentage of the total delay: ");
        scanf("%f", &extraDelayPercentage);
        extraDelayPercentage = checkNumber(extraDelayPercentage, 0, 100);

/*---------------------------------Total delay and extra delay (processing+queueing) calculation--------------------------------------*/

        totalDelay = propDelay + (packetSize/bitRate); // In secs. Processing and queueing delay not yet calculated
        totalDelay = totalDelay/(1-extraDelayPercentage/100);
        extraDelay = totalDelay*extraDelayPercentage;

        printf("The total delay for the reception of a size %0.0fKB packet will be %0.4f msecs.",
                                                    packetSize/KBYTETOBIT, totalDelay*SECTOMILISEC);

/*-----------------------------------Calculate amount of packets sent between two timestamps------------------------------------------*/

        printf("\nDo you want to calculate amount of bits of the above packet that will be sent in a given time period?");
        printf("\nIf yes press \"1\", else press another integer: ");
        scanf("%d", &choice);
        if (choice == 1){
            float startTime, endTime;
            printf("\nConsider timestamp=0 to be the time when the sending node starts sending bits.");
            printf("\nEnter the starting timestamp (in msecs) from where you want to calculate the received bits: ");
            scanf("%f", &startTime);
            while (startTime<0){
                printf("You cannot enter a negative timestamp. Please enter again: ");
                scanf("%f", &startTime);
            }// while
            printf("\nEnter the ending timestamp (in msecs): ");
            scanf("%f", &endTime);
            while (endTime<0 || endTime<=startTime){
                printf("Ending timestamp must be non negative and higher than starting timestamp. Please enter again: ");
                scanf("%f", &endTime);
            }// while
            printf("\n");
            endTime = endTime/SECTOMILISEC;
            startTime = startTime/SECTOMILISEC;
            printReceivedBits(propDelay, totalDelay, startTime, endTime, bitRate, extraDelayPercentage);
        }// if

        printf("\nPress \"0\" to exit the program or another integer to restart: ");
        scanf("%d", &choice);
    }// while
}// main

/*====================================================================================================================================*/
/*==========================================================FUNCTIONS=================================================================*/
/*====================================================================================================================================*/

/*------------------------------------------Checks if a variable holds an appropriate value-------------------------------------------*/

// If lowerlimit is 0 and upperlimit is "infinity" then user is prompted to enter a non negative number
// Else, the user is prompted to enter a number between the lower and upper limit
// Repeat until a right value is given
float checkNumber(float number, float low, float high){
    while (number<low || number>high){
        if (high >= UPPERLIMIT && low == 0) printf("The value cannot be negative. ");
        else printf("The value must be between %0.0f and %0.0f. ", low, high);
        printf("Please enter again: ");
        scanf("%f", &number);
    }// while
    printf("\n");
    return number;
}// checkNumber

/*-----------------------------Calculate and print the amount of received bits between two timestamps--------------------------------*/

void printReceivedBits(float propDelay, float totalDelay, float startTime, float endTime, float bitRate, float extraDelayPercentage){
    int receivedBits;
    float beneficialPeriod;

    if(endTime<propDelay){
        printf("\nYou chose an ending timestamp that is less than the propagation delay. No bits will be received.");
        return;
    }// if

    if(startTime>=totalDelay){
        printf("\nYou chose a starting timestamp that is higher than the total delay. All bits are already received since then.");
        printf(" So no bits will be received inside the period you entered.");
        return;
    }// if

    if(endTime>totalDelay){
        endTime = totalDelay; // No bits are received after the total delay (whole packet already received).
    }// if

    if(startTime<propDelay){
        startTime = propDelay; // No bits are received before the propagation dealy is passed.
    }// if

    beneficialPeriod = endTime - startTime; // We care only for the period where bits are actually received by the receiving node
    if (beneficialPeriod >= totalDelay-propDelay){  // No need to calculate, because
                                                    // user chose a period which contains the whole actual period
                                                    // where receiving node receives bits (whole packet is received)
        printf("\nThe period you entered contains the whole receiving packet.");
        return;
    }// if
    receivedBits = (beneficialPeriod - (extraDelayPercentage/100)*beneficialPeriod) * bitRate;
    printf("\nThe amount of bits that will be received in the given period is %d.", receivedBits);
}// printReceivedBits