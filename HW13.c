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
int WriteBinary(int nChan, int nPt, double Data[][nPt], char * pfname, int nSize);
int ReadBinary(int nChan, int nPt, double Data[][nPt], const char * pfname);
int Results(int nChan, double Avg1[], double Avg2[]);


int main(void) {
    printf("\n\n|#####################|\n|HW #13, Gabriel Cotua|\n|#####################|\n\n");

    srand((unsigned int) time(0));

    int nPt = 0, nChan = 0;
    char filename[50] = "binaryDataFile.bin";


    while (Setup(&nChan, &nPt) != -1) {
        int totalSize = nChan * nPt * sizeof(double);
        void * pData = malloc(totalSize); // allocating memory
        if (pData == NULL) {
            printf("Memory allocation failed. Try fewer channels or points.\n");
        }
        else {
            CollectData(nChan, nPt, (double(*)[nPt]) pData);

            printf("\n--- Data Summary ---\n");
            printf("Channels: %d\n", nChan);
            printf("Data points per channel: %d\n", nPt);
            printf("Total memory allocated: %d bytes\n", totalSize);
            printf("Memory address: %p\n", pData);

            int writestatus = WriteBinary(nChan, nPt, (double(*)[nPt]) pData, filename, totalSize);
            if (writestatus != 0) {
                printf("WriteBinary failed with error code: %d\n", writestatus);
                free(pData);
            }
            else {
                double avg1[nChan], avg2[nChan];
                AvgData(nChan, nPt, (double (*)[nPt]) pData, avg1);
                free(pData);
                void *pData2 = malloc(nChan * nPt * sizeof(double));

                if (pData2 == NULL)
                {
                    printf("malloc failed. \n");
                } else {
                    int readStatus = ReadBinary(nChan, nPt, (double(*)[nPt]) pData2, filename);
                    if (readStatus != 0) {
                        printf("ReadBinary failed with error code: %d\n", readStatus);
                        free(pData2);
                    }
                    else {
                        AvgData(nChan, nPt, (double (*)[nPt]) pData2, avg2);

                        Results(nChan, avg1, avg2);
                        printf("\n\nSuccess!\n");
                        free(pData2);
                    }
                }
                free(pData);
            }

        }
    }
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
            printf("Quiting...\n");
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
            printf("Array was empty/not assigned\n");
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
    int NchNpt[2] = {nChan, nPt}; // NchNpt[0] == nChan, NchNpt[1] == nPt

    while (tolower(ans) != 'y') {
        pfile = fopen(pfname, "r");

        if (pfile == NULL) {
            printf("Error opening the file. File does not exist. \n");
            printf("Would you like to create it? (y\\n)\n");
            ans = getchar();
            FLUSH;
            if (tolower(ans) == 'y') {
                pfile = fopen(pfname, "w");

                if (pfile == NULL) {
                    printf("error creating the file\n");
                    ans = 'y';
                    i = -1;
                }
            } else {
                ans = 'y'; // setting up condition to exit this loop
                i = -1; // settings as a flag to exit function later
            }
        } else {
            printf("File already exist, Would you like to write to the file? %s (y\\n): ", pfname);
            ans = getchar();
            FLUSH;

            if (tolower(ans) == 'n') {
                printf("\nWhat file would you like to write to?\n");
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
        printf("\nfile didn't load correctly or doesn't exit.\n");
        return -4; // exits the loop if the file didn't load correctly nor exist;
    }

    printf("Using file %s\n", pfname);
    pfile = fopen(pfname, "wb+");
    fwrite(NchNpt , 2 * sizeof(int), 1, pfile); // Writing nChan and nPt

    for (i = 0; i < nChan; i++) {
        if (fwrite(Data[i], sizeof(double), nPt, pfile) != nPt) { // Writing data values and checking for process done
            printf("Something went wrong writing to the file. Exiting...\n");
            i = nChan + 2; // setting up flag
        }
    }

    if ( i == nChan + 2) { // data didn't write properly
        printf("\nData didnt write properly.\n");
        return -5;
    }

    fclose(pfile); // closing file
    printf("Transfer completed! data transferred to %s\n", pfname);

    return 0;
}


int ReadBinary(int nChan, int nPt, double Data[][nPt], const char * pfname) {

    FILE * pfile = fopen(pfname, "rb");
    int NchNpt[2];

    if (pfile == NULL) {
        printf("Error opening the file. File does not exist. Exiting...\n");
        return -6;
    }

    if (fread(NchNpt, sizeof(int), 2, pfile) != 2) {
        printf("Something went wrong reading from file. Exiting...\n");
        return -7;
    }

    if (NchNpt[0] != nChan || NchNpt[1] != nPt) {
        printf("Error: Data dimensions in file '%s' do not match expected dimensions.\n", pfname);
        printf("  File has: %d channels, %d points\n", NchNpt[0], NchNpt[1]);
        printf("  Expected: %d channels, %d points\n", nChan, nPt);
        fclose(pfile);
        return -8;
    }

    for (int i = 0; i < nChan; i++) {
        if (fread(Data[i], sizeof(double), nPt, pfile) != nPt) {
            printf("Error: Failed to read data for channel %d from '%s'.\n", i, pfname);
            fclose(pfile);
            return -9;
        }
    }

    fclose(pfile);
    printf("File successfully read.\n");
    return 0;
}

int Results(int nChan, double Avg1[], double Avg2[]){
    //Avg1 should contain the information that was sent to the binary file
    //Avg2 should contain the information read from the binary file
    int i = 0;

    for (i = 0; i < nChan; i++) {
        printf("Sent: %.4lf Received: %.4lf match? - %c\n", Avg1[i], Avg2[i], (Avg1[i] == Avg2[i]) ? 'y' : 'n');
        if (Avg1[i] != Avg2[i]) {
            i = nChan + 2;
        }
    }

    if (i == nChan + 2) {
        printf("Averages didn't match completely.\n");
        return -10;
    }



    return 0;
}

/*

|#####################|
|HW #13, Gabriel Cotua|
|#####################|

input number of channels to use: [2...10]:3

Amount of data points:
[0] - quit
[1] - 2048
[2] - 4096
[3] - 8192
(invalid input - quit)
3


--- Data Summary ---
Channels: 3
Data points per channel: 8192
Total memory allocated: 196608 bytes
Memory address: 000001689ec50080
File already exist, Would you like to write to the file? binaryDataFile.bin (y\n):y

Using file binaryDataFile.bin
Transfer completed! data transferred to binaryDataFile.bin
File successfully read.
Sent: 4.9703 Received: 4.9703 match? - y
Sent: 4.9622 Received: 4.9622 match? - y
Sent: 5.0334 Received: 5.0334 match? - y

------------------------------------------------------------------------------------------------------------------------

|#####################|
|HW #13, Gabriel Cotua|
|#####################|

input number of channels to use: [2...10]:10

Amount of data points:
[0] - quit
[1] - 2048
[2] - 4096
[3] - 8192
(invalid input - quit)
3


--- Data Summary ---
Channels: 10
Data points per channel: 8192
Total memory allocated: 655360 bytes
Memory address: 000001ab0d120080
File already exist, Would you like to write to the file? binaryDataFile.bin (y\n):n


What file would you like to write to?
informationData.bin

Error opening the file. File does not exist.
Would you like to create it? (y\n)
y

Using file informationData.bin
Transfer completed! data transferred to informationData.bin
File successfully read.
Sent: 4.9696 Received: 4.9696 match? - y
Sent: 5.0694 Received: 5.0694 match? - y
Sent: 4.9933 Received: 4.9933 match? - y
Sent: 4.9246 Received: 4.9246 match? - y
Sent: 4.9707 Received: 4.9707 match? - y
Sent: 4.9886 Received: 4.9886 match? - y
Sent: 5.0139 Received: 5.0139 match? - y
Sent: 5.0331 Received: 5.0331 match? - y
Sent: 5.0340 Received: 5.0340 match? - y
Sent: 5.0239 Received: 5.0239 match? - y

 */