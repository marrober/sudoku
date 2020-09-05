#include <stdio.h>
#include "definitions.h"
#include "cell.h"

int main(void) {

    cell cells[GRID_SIZE][GRID_SIZE];

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
    cells[5][0].setValue(1);
    cells[5][5].setValue(5);

    printf("values remaining to set ....\n");
    for (int rowCounter = 0; rowCounter < GRID_SIZE; rowCounter++) {
        for (int columnCounter = 0; columnCounter < GRID_SIZE; columnCounter++) {
            printf("%d ", cells[rowCounter][columnCounter].getNumRemainingValues());
        }
        printf("\n");
    }
    printf("\n");
    printf("Assigned values ....\n");
    for (int rowCounter = 0; rowCounter < GRID_SIZE; rowCounter++) {
        for (int columnCounter = 0; columnCounter < GRID_SIZE; columnCounter++) {
            printf("%d ", cells[rowCounter][columnCounter].getValue());
        }
        printf("\n");
    }

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
    printf("\n");

    printf("Grid group ....\n");
    for (int rowCounter = 0; rowCounter < GRID_SIZE; rowCounter++) {
        for (int columnCounter = 0; columnCounter < GRID_SIZE; columnCounter++) {
            printf("%d ", cells[rowCounter][columnCounter].getGridGroup());
        }
        printf("\n");
    }

    printf("Rule out possible values\n");

    for (int rowCounter = 0; rowCounter < GRID_SIZE; rowCounter++) {
        for (int columnCounter = 0; columnCounter < GRID_SIZE; columnCounter++) {
            if (!cells[rowCounter][columnCounter].isFixedValue()) {
                printf("starting to guess for cell %d %d\n", rowCounter, columnCounter);
                while((possibleValue = cells[rowCounter][columnCounter].getNextPossibleValue()) > 0) {

                    // compare to fixed row values

                    for (int compareColumnCounter = 0; compareColumnCounter < GRID_SIZE; compareColumnCounter++) {
                        if (compareColumnCounter != columnCounter) {
                            if (cells[rowCounter][compareColumnCounter].isFixedValue()) {
                                if (possibleValue == cells[rowCounter][compareColumnCounter].getValue()) {
                                    // possible value rulled out
                                    cells[rowCounter][columnCounter].removePossibleValue(possibleValue);
                                }
                            }
                        }
                    }

                    // compare to fixed column values

                    for (int compareRowCounter = 0; compareRowCounter < GRID_SIZE; compareRowCounter++) {
                        if (compareRowCounter != rowCounter) {
                            if (cells[compareRowCounter][columnCounter].isFixedValue()) {
                                if (possibleValue == cells[compareRowCounter][columnCounter].getValue()) {
                                    // possible value rulled out
                                    cells[rowCounter][columnCounter].removePossibleValue(possibleValue);
                                }
                            }
                        }
                    }

                    // compare to grid values

                    for (int compareColumnCounter = 0; compareColumnCounter < GRID_SIZE; compareColumnCounter++) {
                        for (int compareRowCounter = 0; compareRowCounter < GRID_SIZE; compareRowCounter++) {
                            if ((compareRowCounter != rowCounter) && (compareColumnCounter != columnCounter)) {
                                if (cells[rowCounter][columnCounter].getGridGroup() == cells[compareRowCounter][compareColumnCounter].getGridGroup()) {
                                    if (cells[compareRowCounter][compareColumnCounter].isFixedValue()) {
                                        if (possibleValue == cells[compareRowCounter][compareColumnCounter].getValue()) {
                                            // possible value rulled out
                                            cells[rowCounter][columnCounter].removePossibleValue(possibleValue);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

            }
        }
    }
    // Fix cells with only 1 remaining value.

    for (int rowCounter = 0; rowCounter < GRID_SIZE; rowCounter++) {
        for (int columnCounter = 0; columnCounter < GRID_SIZE; columnCounter++) {
            if ((!cells[rowCounter][columnCounter].isFixedValue()) && cells[rowCounter][columnCounter].getNumRemainingValues()) {
                printf("Fixing cell %d %d ", rowCounter, columnCounter);
                cells[rowCounter][columnCounter].setValue(cells[rowCounter][columnCounter].getCurrentPossibleValue());
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

}