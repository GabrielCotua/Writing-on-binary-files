// Gabriel Cotua HW-13

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

#define FLUSH while(getchar() != '\n');

double dRand(void);
int Setup(int *pnChan, int*pnPt);
int CollectData(int nChan, int nPt, double Data[][nPt]);
int AvgData(int nChan, int nPt, double Data[][nPt], double Avg[]);
int WriteBinary(int nCHan, int nPt, double Data[][nPt], char * pfname, int nSize);
int ReadBinary(int nChan, int nPt, double Data[][nPt], const char * pfname);
int Results(int nChan, double Avg1[], double Avg2[]);


int main(void) {
    printf("\n\n|#####################|\n|HW #13, Gabriel Cotua|\n|#####################|\n\n");
    int nPt = 0, nChan = 0;

    srand((unsigned int) time(0));
    Setup(&nChan, &nPt);
    return 0;
}

double dRand(void) {
    return rand()*(10.0/RAND_MAX);
}

int Setup(int *pnChan, int *pnPt) {
    char ans;
    printf("input number of channels to use: [2...10]: ");
    scanf(" %d", pnChan);
    FLUSH;

    printf("Amount of data points: \n[0] - quit\n[1] - 2048\n[2] - 4096\n[3] - 8192\n(invalid input - quit)\n");
    ans = getchar();
    FLUSH;
    switch(ans){
        case '1':
            *pnPt = 2048;
            break;

        case '2':
            *pnPt = 4096;
            break;

        case '3':
            *pnPt = 8192;
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
    double sum = 0;

    for ( int i = 0; i < nChan; i++ ) {
        for ( int j = 0; j < nPt; j++ ) {
            sum += Data[i][j];
        }
        Avg[i] = sum / (double) nPt;
        sum = 0; // reset for next loop
        if ( Avg[i] == 0 ) {
            printf("Array was empty/not assigned");
            return -3;
        }
    }
    return 0;
}
// check
int WriteBinary(int nChan, int nPt, double Data[][nPt], char * pfname, int nSize) {
    char ans = 'n';
    FILE * pfile;
    int i = 0;
    int NchNpt[2] = {nChan, nPt}; // NchNpt[0] == nCha, NchNpt[1] == nPt

    while (tolower(ans) != 'y') {
        pfile = fopen(pfname, "r");

        if (pfile == NULL) {
            printf("Error opening the file. File does not exist. Exiting...");
            ans = 'y'; // setting up condition to exit this loop
            i = -1; // settings as a flag to exit function later
        }
        else {
            printf("Would you like to write to the file? %s (y\\n): ", pfname);
            ans = getchar();
            FLUSH;

            if (tolower(ans) == 'n') {
                printf("\nWhat file would you like to write on?\n");
                scanf(" %49s", pfname);
                if (strchr(pfname, '\n') == NULL)
                    FLUSH;
            }

            if (tolower(ans) != 'y' && tolower(ans) !='n') {
                printf("please insert a valid choice:\n");
                ans = getchar();
                FLUSH;
            }
        }
    }

    if (i == -1) {
        printf("\nfile didn't load correctly or doesn't exit.");
        return -4; // exits the loop if the file didn't load correctly nor exist;
    }

    printf("Using file %s", pfname);
    pfile = fopen(pfname, "rb+");
    fwrite(NchNpt , 2 * sizeof(int), 1, pfile); // Writing nChan and nPt

    for (i = 0; i < nChan; i++) {
        if (fwrite(Data[i], nPt * sizeof(double), 1, pfile) != 1) { // Writing data values and checking for process done
            printf("Something went wrong writing to the file. Exiting...");
            i = nChan + 2; // setting up flag
        }
    }

    if ( i == nChan + 2) { // data didn't write properly
        printf("\nData didnt write properly.");
        return -5;
    }

    fclose(pfile); // closing file
    printf("Transfer completed! data transferred to %s", pfname);

    return 0;
}


int ReadBinary(int nChan, int nPt, double Data[][nPt], const char * pfname) {

    FILE * pfile = fopen(pfname, "rb");
    int NchNpt[2];

    if (pfile == NULL) {
        printf("Error opening the file. File does not exist. Exiting...");
        return -6;
    }

    if (fread(NchNpt, sizeof(int), 2, pfile) != 2) {
        printf("Something went wrong reading from file. Exiting...");
        return -7;
    }

    if (meta[0] != nChan || meta[1] != nPt) {
        printf("Error: Data dimensions in file '%s' do not match expected dimensions.\n", pfname);
        printf("  File has: %d channels, %d points\n", meta[0], meta[1]);
        printf("  Expected: %d channels, %d points\n", nChan, nPt);
        fclose(pfile);
        return -3;
    }

    fclose(pfile);
    return 0;
}

int Results(int nChan, double Avg1[], double Avg2[]){
    return 0;
}
