// Gabriel Cotua HW-13

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//#define nPt 9

double dRand(void);
int Setup(int *pnChan, int*pnPt);
int CollectData(int nChan, int nPt, double Data[][nPt]);
int AvgData(int nChan, int nPt, double Data[][nPt], double Avg[]);
int WriteBinary(int nCHan, int nPt, double Data[][nPt], char * pfname, int nSize);
int ReadBinary(int nChan, int nPt, double Data[][nPt], const char * pfname);
int Results(int nChan, double Avg1[], double Avg2[]);


int main(void) {
    printf("\n\n|#####################|\n|HW #13, Gabriel Cotua|\n|#####################|\n\n");

    srand((unsigned int) time(0));

    return 0;
}

double dRand(void) {
    return rand()*(10.0/RAND_MAX);
}

int Setup(int *pnChan, int *pnPt) {
    int chan;

    printf("input number of channels to use: [2...10]");
    scanf(" %d", &chan);

    printf("Amount of data points: \n[0] - quit\n[1] - 2048\n[2] - 4096\n[3] - 8192\n0 or unvalid to exit");

    switch(getchar()){
        case '1':
        break;

        case '2':
        break;

        case '3':
        break;

        default: 
        printf("Exiting...\n");
        return -1;
    }
    return 0;
}

int CollectData(int nChan, int nPt, double Data[][nPt]) {

    for (int i = 0; i < nChan; i++) { 
        for (int j = 0; j < nPt; j++) {
            Data[i][j] = dRand();
        }
    }

    return 0;
}

int AvgData(int nChan, int nPt, double Data[][nPt], double Avg[]) {
    long long int sum = 0;

    for ( int i = 0; i < nChan; i++ ) {
        for ( int j = 0; j < nPt; j++ ) {
            sum += Data[i][j];
        }
        Avg[i] = sum;
        sum = 0; // reset for next loop
        if ( Avg[i] == 0 ) {
            printf("Array was empty/not assigned");
            return -3;
        }
    }
    return 0;
}

int WriteBinary(int nCHan, int nPt, double Data[][nPt], char * pfname, int nSize) {
    return 0;
}


int ReadBinary(int nChan, int nPt, double Data[][nPt], const char * pfname) {
    return 0;
}

int Results(int nChan, double Avg1[], double Avg2[]){
    return 0;
}
