/*
 /*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Author : Mirka Kolarikova <xkolar76@stud.fit.vutbr.cz>
 * some parts taken from:
 * https://mcuoneclipse.com/2017/01/30/cycle-counting-on-arm-cortex-m-with-dwt/
 * https://www.geeksforgeeks.org/selection-sort/
 * https://www.geeksforgeeks.org/insertion-sort/
 */


#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include "MK60D10.h"

#define DWT_CONTROL                   (*((volatile uint32_t*)0xE0001000))    // control register
#define DWT_CYCCNT                    (*((volatile uint32_t*)0xE0001004))    // cycle count register
#define DWT_SLEEPCNT                  (*((volatile uint32_t*)0xE0001010))    // sleep count register
#define DWT_EXCCNT                    (*((volatile uint32_t*)0xE000100C))    // exception counter
#define DEMCR                         (*((volatile uint32_t*)0xE000EDFC))    // Debug Exception and Monitor Control Register


// 1UL is an unsigned long int with a value of 1
#define DWT_CYCCNTENA_BIT       (1UL<<0)  //CYCCNTENA bit in DWT_CONTROL register
#define DWT_EXCEVTENA_BIT       (1UL<<18) //EXCEVTENA bit enables exception counter
#define DWT_SLEEPEVTENA_BIT     (1UL<<19) //SLEEPEVTENA bit enables sleep counter

#define DEMCR_TRCENA_BIT        (1UL<<24) //Trace enable bit in DEMCR register


#define ITERATIONS_COUNT        10



//////////////// Init DWT  //////////////////

// enable trace end debug
#define InitCycleCounter() \
  DEMCR |= DEMCR_TRCENA_BIT


//////////////// Cycle counter //////////////////

// reset cycle counter
#define ResetCycleCounter() \
  DWT_CYCCNT = 0

// enable cycle counter
//OR operation to set last bit of DWT_CONTROL to 1
#define EnableCycleCounter() \
  DWT_CONTROL |= DWT_CYCCNTENA_BIT

// disable cycle counter
#define DisableCycleCounter() \
  DWT_CONTROL &= ~DWT_CYCCNTENA_BIT //NAND

// read cycle counter register
#define GetCycleCounter() \
  DWT_CYCCNT


//////////////// Sleep counter //////////////////

// enable counting cycles that the processor is sleeping
//set to 1 to enable overflow event and initialize to 0
#define EnableSleepCycleCounter() \
  DWT_CONTROL |= DWT_SLEEPEVTENA_BIT //set to 1

// read sleep cycle counter register
#define GetSleepCycleCounter() \
  DWT_SLEEPCNT

//////////////// Exception counter //////////////////
  //set to 1 to enable overflow event and initialize to 0
 #define EnableExceptionCounter() \
  DWT_CONTROL |= DWT_EXCEVTENA_BIT //set to 1

  // read exception counter register
#define GetSleepCycleCounter() \
  DWT_EXCCNT



//////////// Global variables  /////////////

uint32_t cycles;      //number of cycles
uint32_t sleepCycles; //number of sleep cycles
uint32_t exceptionCycles; //number of exeption

uint32_t cyclesTotal;
uint32_t cyclesMin;
uint32_t cyclesMax;
uint32_t cyclesMean;

uint32_t sleepCyclesTotal;
uint32_t sleepCyclesMin;
uint32_t sleepCyclesMax;
uint32_t sleepCyclesMean;

uint32_t exceptionCyclesTotal;
uint32_t exceptionCyclesMin;
uint32_t exceptionCyclesMax;
uint32_t exceptionCyclesMean;




//////////// Parts of code to be measured  /////////////

//sequence
void sequence(){
	int a = 20;
	int b = 100;
	int result = b - a * 2;

}

//selection
void selection(){
	int a = 2;
	a = a + 40;
	if(a == 30){
		return;
	}else if(a < 18){
		return;
	}else if(a > 18 && a <= 40){
		return;
	}else if( a == 42){
		return;
	}else{
		return;
	}
}

//iteration
void iteration(){
	int a = 0;
	int b = 0;
	for(a;a < 42;a++){
		b += 4;
	}
	return;
}

////// Selection sort ///////
// taken from: https://www.geeksforgeeks.org/selection-sort/
void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void selectionSort(int arr[], int n)
{
    int i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n-1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < n; j++)
        if (arr[j] < arr[min_idx])
            min_idx = j;

        // Swap the found minimum element with the first element
        swap(&arr[min_idx], &arr[i]);
    }
}

////// Insertion sort ///////
// taken from https://www.geeksforgeeks.org/insertion-sort/
void insertionSort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// start counting cycles and sleep cycles
void startCounting(){

	InitCycleCounter();            // init DWT
	ResetCycleCounter();           // set counter to 0
	EnableCycleCounter();          // start counting cycles
	EnableSleepCycleCounter();     // start counting cycles in sleep
	EnableExceptionCounter();
	return;
}


// start counting cycles and sleep cycles
// update cycles MIN and MAX
void finishCounting(){
	cycles = GetCycleCounter();           // get number of cycles
	sleepCycles = GetSleepCycleCounter(); // get number of cycles in sleep
	exceptionCycles = GetSleepCycleCounter(); //get exception counter

	//update min cycles
	if(cycles < cyclesMin){
		cyclesMin = cycles;
	}
	//update min sleep cycles
	if(sleepCycles < sleepCyclesMin){
		sleepCyclesMin = sleepCycles;
	}
	if(exceptionCycles < exceptionCyclesMin){
		exceptionCyclesMin = exceptionCycles;
	}
	//update max cycles
	if(cycles > cyclesMax){
		cyclesMax = cycles;
	}
	//update max sleep cycles
	if(sleepCycles > sleepCyclesMax){
		sleepCyclesMax = sleepCycles;
	}
	if(exceptionCycles > exceptionCyclesMax){
		exceptionCyclesMax = exceptionCycles;
	}

	cyclesTotal += cycles;
	sleepCyclesTotal += sleepCycles;
	DisableCycleCounter();                // disable counting cycles
	return;

}

// init stats variables before counting
void initStatsVar(){
	cyclesTotal = 0;
	cyclesMin = UINT32_MAX;
	cyclesMax = 0;
	cyclesMean = 0;

	sleepCyclesTotal = 0;
	sleepCyclesMin = UINT32_MAX;
	sleepCyclesMax = 0;
	sleepCyclesMean = 0;

	exceptionCyclesTotal = 0;
	exceptionCyclesMin = UINT32_MAX;
	exceptionCyclesMax = 0;
	exceptionCyclesMean = 0;
	return;
}

//print statistics
void printStats(char* codeName){
	printf("---------Stats for %s in %d runs ----------\n", codeName, ITERATIONS_COUNT);
	printf("Total cycles: %d \n", cyclesTotal);
	printf("Minimum cycles: %d \n", cyclesMin);
	printf("Maximum cycles: %d \n", cyclesMax);
	printf("Average cycles: %d \n", cyclesMean);
	printf("\n");
	printf("Total sleep cycles: %d \n", sleepCyclesTotal);
	printf("Minimum sleep cycles: %d \n", sleepCyclesMin);
	printf("Maximum sleep cycles: %d \n", sleepCyclesMax);
	printf("Average sleep cycles: %d \n", sleepCyclesMean);
	printf("\n");
	printf("Total exeption cycles: %d \n", exceptionCyclesTotal);
	printf("Minimum exeption cycles: %d \n", exceptionCyclesMin);
	printf("Maximum exeption cycles: %d \n", exceptionCyclesMax);
	printf("Average exeption cycles: %d \n", exceptionCyclesMean);
}

//init MCU
void MCUInit(){
	MCG->C4 |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );
	SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);
	WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}


int main(void)
{

	MCUInit();

	//  stats for sequence
	initStatsVar();
	for(int i = 0; i < ITERATIONS_COUNT; i++){
		startCounting();
		sequence();
		finishCounting();
	}
	cyclesMean = cyclesTotal / ITERATIONS_COUNT;
	sleepCyclesMean = sleepCyclesTotal / ITERATIONS_COUNT;
	exceptionCyclesMean = exceptionCyclesTotal /ITERATIONS_COUNT;
	printStats("SEQUENCE");


	//  stats for selection
	initStatsVar();
	for(int i = 0; i < ITERATIONS_COUNT; i++){
		startCounting();
		selection();
		finishCounting();
	}
	cyclesMean = cyclesTotal / ITERATIONS_COUNT;
	sleepCyclesMean = sleepCyclesTotal / ITERATIONS_COUNT;
	exceptionCyclesMean = exceptionCyclesTotal /ITERATIONS_COUNT;
	printStats("SELECTION");

	//  stats for iteration
	initStatsVar();
	for(int i = 0; i < ITERATIONS_COUNT; i++){
		startCounting();
		iteration();
		finishCounting();
	}
	cyclesMean = cyclesTotal / ITERATIONS_COUNT;
	sleepCyclesMean = sleepCyclesTotal / ITERATIONS_COUNT;
	exceptionCyclesMean = exceptionCyclesTotal /ITERATIONS_COUNT;
	printStats("ITERATION");

	//  stats for selection sort
	initStatsVar();
	for(int i = 0; i < ITERATIONS_COUNT; i++){
		startCounting();
		int arr[] = {64, 25, 12, 22, 11};
    	int n = sizeof(arr)/sizeof(arr[0]);
    	selectionSort(arr, n);
		finishCounting();
	}
	cyclesMean = cyclesTotal / ITERATIONS_COUNT;
	sleepCyclesMean = sleepCyclesTotal / ITERATIONS_COUNT;
	exceptionCyclesMean = exceptionCyclesTotal /ITERATIONS_COUNT;
	printStats("SELECTION SORT");

	//  stats for insertion sort
	initStatsVar();
	for(int i = 0; i < ITERATIONS_COUNT; i++){
		startCounting();
		int arr[] = { 12, 11, 13, 5, 6 };
    	int n = sizeof(arr) / sizeof(arr[0]);
    	insertionSort(arr, n);
		finishCounting();
	}
	cyclesMean = cyclesTotal / ITERATIONS_COUNT;
	sleepCyclesMean = sleepCyclesTotal / ITERATIONS_COUNT;
	exceptionCyclesMean = exceptionCyclesTotal /ITERATIONS_COUNT;
	printStats("INSERTION SORT");

    //never leave main
    while (1);

    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

