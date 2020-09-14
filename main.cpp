#include <stdio.h>
#include <locale.h>
#include <stdint.h>
#include "definitions.h"
#include "cell.h"
#include "analyse.h"


int main(void) {

    setlocale(LC_NUMERIC, "");

    cell cells[GRID_SIZE][GRID_SIZE];
    cell *pCells = &cells[GRID_SIZE][GRID_SIZE];

    cells[0][0].setValue(4);
    cells[0][5].setValue(1);
    cells[1][2].setValue(2);
    cells[1][3].setValue(3);
    cells[2][1].setValue(5);
    cells[2][4].setValue(3);
    cells[3][1].setValue(6);
    cells[3][4].setValue(4);
    cells[4][2].setValue(5);
    cells[4][3].setValue(4);
    // cells[5][0].setValue(1);
    // cells[5][5].setValue(5);

    printf("values remaining to set ....\n");
    for (int rowCounter = 0; rowCounter < GRID_SIZE; rowCounter++) {
        for (int columnCounter = 0; columnCounter < GRID_SIZE; columnCounter++) {
            printf("%d ", cells[rowCounter][columnCounter].getNumRemainingValues());
        }
        printf("\n");
    }
    printf("Assigned values ....\n");
    for (int rowCounter = 0; rowCounter < GRID_SIZE; rowCounter++) {
        for (int columnCounter = 0; columnCounter < GRID_SIZE; columnCounter++) {
            printf("%d ", cells[rowCounter][columnCounter].getValue());
        }
        printf("\n");
    }

    uint64_t combinations = 1;
    for (int rowCounter = 0; rowCounter < GRID_SIZE; rowCounter++) {
        for (int columnCounter = 0; columnCounter < GRID_SIZE; columnCounter++) {
            if (!cells[rowCounter][columnCounter].isFixedValue()) {
                combinations = combinations * cells[rowCounter][columnCounter].getNumRemainingValues();
            }
        }
    }
    printf("Combination of options before first pass ... %'lld\n", combinations);

    int columnNumItems = GRID_SIZE / BLOCK_SIZE_COL;
    int rowNumItems = GRID_SIZE / BLOCK_SIZE_ROW;
    int blockIndex = 1;
    int colBlockIndex = 1;
    int rowBlockIndex = 1;
    int possibleValue = 0;

    for (int rowCounter = 0; rowCounter < GRID_SIZE; rowCounter++) {
        for (int columnCounter = 0; columnCounter < GRID_SIZE; columnCounter++) {
            cells[rowCounter][columnCounter].setGridGroup(blockIndex);
            if ((columnCounter > 0) && ((columnCounter + 1) % columnNumItems) == 0) {
               blockIndex++; 
            }
        }
        if ((rowCounter == 0) || (((rowCounter + 1) % rowNumItems) != 0)) {
            blockIndex = blockIndex - BLOCK_SIZE_COL;
        }

    }
    for (int rowCounter = 0; rowCounter < GRID_SIZE; rowCounter++) {
        for (int columnCounter = 0; columnCounter < GRID_SIZE; columnCounter++) {
            //printf("%d ", cells[rowCounter][columnCounter].getGridGroup());
        }
    }

    int passCounter = 1;
    int lastCombination = 0;

    while(lastCombination != combinations) {

        printf("PASS --- %d\n", passCounter++);

        printf("Rule out possible values\n");

        for (int rowCounter = 0; rowCounter < GRID_SIZE; rowCounter++) {
            for (int columnCounter = 0; columnCounter < GRID_SIZE; columnCounter++) {
                if (!cells[rowCounter][columnCounter].isFixedValue()) {
                    // printf("starting to guess for cell %d %d\n", rowCounter, columnCounter);
                    while((possibleValue = cells[rowCounter][columnCounter].getNextPossibleValue()) > 0) {

                        // compare to fixed row values

                        if (compareToFixedRowValues(rowCounter, columnCounter, cells, possibleValue)) {
                            cells[rowCounter][columnCounter].removePossibleValue(possibleValue);
                            fixCellIfPossible(rowCounter, columnCounter, cells);

                        }

                        // compare to fixed column values

                        if(compareToFixedColumnValues(rowCounter, columnCounter, cells, possibleValue)) {
                            cells[rowCounter][columnCounter].removePossibleValue(possibleValue);
                            fixCellIfPossible(rowCounter, columnCounter, cells);
                        }

                        // compare to grid values

                        if(compareToGridValues(rowCounter, columnCounter, cells, possibleValue)) {
                            cells[rowCounter][columnCounter].removePossibleValue(possibleValue);
                            fixCellIfPossible(rowCounter, columnCounter, cells);
                        }
                    }
                }
            }
        }

        printf("values remaining to set ....\n");
        for (int rowCounter = 0; rowCounter < GRID_SIZE; rowCounter++) {
            for (int columnCounter = 0; columnCounter < GRID_SIZE; columnCounter++) {
                printf("%d ", cells[rowCounter][columnCounter].getNumRemainingValues());
            }
            printf("\n");
        }
        printf("Assigned values ....\n");
        for (int rowCounter = 0; rowCounter < GRID_SIZE; rowCounter++) {
            for (int columnCounter = 0; columnCounter < GRID_SIZE; columnCounter++) {
                printf("%d ", cells[rowCounter][columnCounter].getValue());
            }
            printf("\n");
        }

        lastCombination = combinations;
        combinations = 1;

        for (int rowCounter = 0; rowCounter < GRID_SIZE; rowCounter++) {
            for (int columnCounter = 0; columnCounter < GRID_SIZE; columnCounter++) {
                if (!cells[rowCounter][columnCounter].isFixedValue()) {
                    combinations = combinations * cells[rowCounter][columnCounter].getNumRemainingValues();
                }
            }
        }
        printf("Combination of options remaining ... %'lld\n", combinations);

        printf("Assigned and trial values ....\n");
        for (int rowCounter = 0; rowCounter < GRID_SIZE; rowCounter++) {
            for (int columnCounter = 0; columnCounter < GRID_SIZE; columnCounter++) {
                if (cells[rowCounter][columnCounter].isFixedValue()) {
                    printf("*%d ", cells[rowCounter][columnCounter].getValue());
                } else {
                    printf("-%d ", cells[rowCounter][columnCounter].getTrialValue()); 
                }
            }
            printf("\n");
        }
    }

    int numberVariableCells = 0;
    printf("Set initial trial cells ....\n");
    for (int rowCounter = 0; rowCounter < GRID_SIZE; rowCounter++) {
        for (int columnCounter = 0; columnCounter < GRID_SIZE; columnCounter++) {
            if (!cells[rowCounter][columnCounter].isFixedValue()) {
                cells[rowCounter][columnCounter].setTrialValue(cells[rowCounter][columnCounter].getNextPossibleValue());
                numberVariableCells++;
                printf("cells to iterate : ");
                printf("%d %d %d %d\n", rowCounter, columnCounter, cells[rowCounter][columnCounter].getNumRemainingValues(), cells[rowCounter][columnCounter].getTrialValue());  
            }
        }
        printf("\n");
    }
    printf("Number of variable cells : %d\n", numberVariableCells);



}