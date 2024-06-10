#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

// Samples and levels that each samples will be quantized to will be stored in the N-dimensional array samples[N][X],
// where the dimension is the amount of sample types to store (2),
// with the first dimension (N=0) being the user's analog values and the second dimension (N=1) being the quantized value of each sample
#define ANALOG 0
#define QUANTIZED 1
#define SAMPLE_TYPES 2
// Values that indicate the number of quantization levels allowed
#define VALID_LEVEL_1 16
#define VALID_LEVEL_2 32

// Function behaviour is explained in FUNCTIONS section after MAIN 
float getLastLevel(float firstLevel);
int getTotalLevels();
void levelsQuantization(float firstLevel, float lastLevel, int totalLevels, float *levels);
void getSamples(float firstLevel, float lastLevel, int totalSamples, float *samples);
void valueToBinary(int value, int numOfBits, char *binaryValue);
int sampleToLevel(int totalLevels, float analog, float *quantized, float *levels);

/*====================================================================================================================================*/
/*========================================================= MAIN =====================================================================*/
/*====================================================================================================================================*/

int main()
{
    /*-------------------Getting start level, end level and total quantization levels amount from user-------------------*/

    int totalLevels;
    float firstLevel, lastLevel; 

    printf("Enter the starting value of the range of the analog signal: ");
    scanf("%f", &firstLevel);

    printf("\nNow enter the ending value of the range of the analog signal.");
    lastLevel = getLastLevel(firstLevel);
    
    printf("\n\nNow enter the amount of quantization levels for the conversion to digital signal.\n");
    totalLevels = getTotalLevels();
    
    /*---------Creating the quantized levels according to previous input, at storing them in the declared array----------*/

    // index 0 -----> last index
    // level 1 -----> last level
	float levels[totalLevels];
    levelsQuantization(firstLevel, lastLevel, totalLevels, levels);

    /*---------------------------------Creating a digital value for each level------------------------------------------*/

    // using the least bits that are needed for every level representation
    int numOfBits = ceil(log2f(totalLevels));
    // levelsInBinary[0][0] -----> levelsInBinary[0][last] for the first level binary representation
    // levelsInBinary[1][0] -----> levelsInBinary[0][last] for the second level binary representation
    // ......
    // levelsInBinary[k][0] -----> levelsInBinary[k][last] for the k level binary representation
    char levelsInBinary[totalLevels][numOfBits+1];
    for (int i=0; i<totalLevels; i++){
        valueToBinary(i, numOfBits, levelsInBinary[i]);
    }// for i

    /*-------------------------Getting amount of samples and sample values from user-----------------------------------*/

    // Amount of samples must be more than 0
    int totalSamples;
    printf("\nEnter the amount of samples you want to quantize: ");
    scanf("%d", &totalSamples);
    while (totalSamples <= 0){
        printf("\nAmount of samples must be more than 0. Please try again: ");
        scanf("%d", &totalSamples); 
    }// while
    float samples[SAMPLE_TYPES][totalSamples];
    getSamples(firstLevel, lastLevel, totalSamples, samples[ANALOG]);

    /*----------------------Sample matching to quantized levels and convertion to binary------------------------------*/

    char samplesInBinary[totalSamples][numOfBits+1]; // +1 for the '\0' as the last index's value
    int sampleLevel;
    for (int i=0; i<totalSamples; i++){
        sampleLevel = sampleToLevel(totalLevels, samples[ANALOG][i], &samples[QUANTIZED][i], levels); // get the sample's corresponding level
        strcpy(samplesInBinary[i], levelsInBinary[sampleLevel]);    // get the sample's binary representation using its quantization level 
    }// for i

    /*-------------------------------------------Observe the results---------------------------------------------------*/

    printf("\nQuantized samples are: ");
    for (int i=0; i<totalSamples; i++){
        printf("\nsample-%d\tvalue %0.2f\t quantized value %0.2f\t binary value %s", i+1, samples[ANALOG][i], samples[QUANTIZED][i], samplesInBinary[i]);
    }// for i

    int choice;
    printf("\n\nEnter enter 0 to exit or another integer if you also want to see the sequence of bits for the signal you gave: ");
    scanf("%d", &choice);
    if (choice){
        printf("\n");
        for (int i=0; i<totalSamples; i++){
            printf("%s", samplesInBinary[i]);
        }// for i
    }// if
    printf("\n\n");
    system("pause");// Take your time watching the results :) :)
}// main

/*====================================================================================================================================*/
/*==========================================================FUNCTIONS=================================================================*/
/*====================================================================================================================================*/

/*-------------------------------------------Determining the last quantization level---------------------------------------------*/

// Takes as argument the first level that the user chose.
// Asks as input from user the last level and returns the value.
// Loop will continue to ask for input until last level is higher than first level.
float getLastLevel(float firstLevel){
    float lastLevel;
    do{
        printf("\nEnding value must be higher than the starting value: ");
        scanf("%f", &lastLevel);
    }while(firstLevel>=lastLevel);
    return lastLevel;
}// getLastLevel

/*----------------------------------------Determining the amount of quantization levels--------------------------------------------*/

// Gets from user the amount of quantization levels.
// Loop will continue to excecute until user enters a valid amount.
// Returns the amount given.
int getTotalLevels(){
    int totalLevels;
    do{
        printf("\nAmount of levels must be 16 or 32: ");
        scanf("%d", &totalLevels);
    }while(totalLevels!=VALID_LEVEL_1 && totalLevels!=VALID_LEVEL_2);
    return totalLevels;
}// getTotalLevels

/*---------------------Create the quantization levels' values and store them in the levels-array address space----------------------*/

// Initialize a step according to first and last levels, and value to the first level
// The level's address space will hold the levels' values
// Iterate through each level, assign the value to the level and increment the value by step-size to be ready for the next level
void levelsQuantization(float firstLevel, float lastLevel, int totalLevels, float *levels){
    float step = (lastLevel-firstLevel)/(totalLevels-1);// levels-1 because I want both first level and last level to be part of the level values
    float value = firstLevel;
    for (int i=0; i<totalLevels; i++){
        levels[i] = value;
        value += step;
        printf("%d level value is %0.2f\n", i+1, levels[i]);
    }// for
}// getQuantizedLevels

/*-----------------------------Sample initialization and storing in the samples-array's address space-------------------------------*/

// Takes as arguments the first and last samples, the amount of samples and a memory address where the samples will be stored
// Getting input from user for each sample
// Before moving to the next sample, verify if sample value is in the range of acceptable values
// If not prompt the user to reenter the sample value
void getSamples(float firstLevel, float lastLevel, int totalSamples, float *samples){
    for (int i=0; i<totalSamples; i++){
        printf("Enter sample number %d: ", i+1);
        scanf("%f", &samples[i]);
        while (samples[i]<firstLevel || samples[i]>lastLevel){
            printf("\nSamples must be in the range of [%0.2f, %0.2f]. Try again: ", firstLevel, lastLevel);
            scanf("%f", &samples[i]);
        }// while
    }// for
}// getSamples

/*------------------------------Creating binary values for levels according to the number of bits used-------------------------------*/

// Takes as arguments a value, a bit range and a memory address.
// First char(byte) of memory address (binaryValue[0])  -----> bit with 2^n value (where n is numOfBits)
// Second char(byte) of memory address (binaryValue[1]) -----> bit with 2^(n-1) value
// .....
// Next to last char of memory address  ---------------------> bit with 2^0 value
// Last char has to be '\0'
// Start from first char, compare remaining value with bit value,
// if remaining is higher then char=1 and subtract the bit value, if not then char=0 and go to next comparison 
void valueToBinary(int value, int numOfBits, char *binaryValue){
	binaryValue[numOfBits] = '\0';
	int curBitPlace = numOfBits-1;
	int curBitValue = pow(2.0, (float)curBitPlace);
	for (int i=0; i<numOfBits; i++){
		if (value >= curBitValue){
			binaryValue[i] = '1';
			value -= curBitValue;
		} else {
			binaryValue[i] = '0';
		}// if-else
		curBitPlace--;
		curBitValue = pow(2.0, (float)curBitPlace);
	}// for
}// valueToBinary

/*-----------------------------------------Sample quantization and binary representation----------------------------------------------*/

// Need the amount of quantization levels (totalLevels), their values (*levels),
// the address space where the quantized value will be stored (*quantized) and of course a sample value.
// Do the same for all quantization levels starting from the first one (levels[0]).
// If an exact match of quantization level to sample value is found, then we have the quantized sample value ready. Stop the loop and return the level index
// If not, then we try to find between which two levels will our sample fall. We use the lowLimit and upLimit indexes to hold onto the upper and lower level at each loop
// At first loop the sample is not between any of the levels, thats why lowLimit is -1 and upLimit is 0.
// Everytime sample is higher than the level value we continue. We try to hit a level which is higher than sample. up-lowLimits follow as well.
// If a level with higher value is found then the sample falls between the lowLimit (previous) and upLimit (current) level.
// The absolute distance between the sample and those two levels tells us at which level is the sample more near.
// Quantized sample take the corresponding value and we return the level. 
int sampleToLevel(int totalLevels, float analog, float *quantized, float *levels){
        int level;
        int lowLimit = -1;
        int upLimit = 0;
        for (int i=0; i<totalLevels; i++){ 
            if (levels[i] == analog){ 
                *quantized = levels[i];
                level = i;
                break;
            }// if
            if (levels[i] > analog){
                float lowDistance = fabs(levels[lowLimit] - analog);
                float upDistance = fabs(levels[upLimit] - analog);
                if (lowDistance > upDistance){
                    *quantized=levels[upLimit];
                    level = i;
                }else {
                    *quantized=levels[lowLimit];
                    level = i-1;
                }// if-else
                break;
            }// if
            lowLimit++;
            upLimit++;
        }// for i
        return level;
}// sampleToLevel