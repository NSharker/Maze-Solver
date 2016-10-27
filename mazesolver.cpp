/*****************************************************************************
Title:          mazesolver.cpp
Author:     Nishad Sharker
Created on:     April 17, 2015
Description:    Will recursively find each path in a given maze and print it out
******************************************************************************/
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>
using namespace std;


/**This will contain the information of the cell in a pathway
   pathNumb is the number that this current cell is in the pathway
      So the first cell in the path will be number 0, the next is 1 and so on until
      it reaches the exit.*/
struct Cell
{
    int row;
    int col;
    int pathNumb = 0;


};

// Function Declarations
/**Finds all the paths in a maze and output them to a vector of vectors of cells
@pre  pathwayList should be an empty vector
@pre  This function will search for an end that is at the bottom right corner of the maze
@post If pathways are found they will be placed into the pathwayList vector

@param maze is the maze from the input file
@param row is the current row that the function is being called on
@param column is the current column that the function is being called on
@param maxRow is the maximum number of rows in our maze
@param maxColumn is the maximum number of columns in our maze
@param pathway is the current pathway that we are transversing
@param pathCount is the current number of our cell in the path
@param pathwayList will contain all the different paths the function finds

*/
bool mazeSolver(char** maze, int row, int column, int maxRow, int maxColumn,vector<Cell> pathway,int pathCount,vector<vector<Cell>>& pathwayList);

int main(int argc,char *argv[])
{

    if (argc != 3)
    {
        cerr << "Improper number of command line arguments provided" << endl;
        exit(1);
    }
    //Opening Maze input file
    ifstream inStream;
    inStream.open(argv[2]);
    if (inStream.fail())
    {
        cerr << "File open failed.\n";
        exit(1);
    }

    int rows, columns;
    inStream >> rows;
    inStream >> columns;

    //Creating Dynamically allocated 2D array
    char ** mazeArray = new char * [rows];
    for(int i=0; i < rows; i++)
    {
        mazeArray[i] = new char[columns];
    }


    //Create maze from INPUT file
    for(int i=0;i < rows; i++)
    {
        for(int j=0; j < columns; j++)
        {
            inStream >> mazeArray[i][j];
        }
    }

    vector<Cell> pathway;

    vector<vector<Cell>> pathwayList;

    //Calling our recusive function to find the pathways
    mazeSolver(mazeArray,0,0,rows,columns,pathway,0,pathwayList);


    int task = atoi(argv[1]);


    //pathwayList size will be 0 if no paths were found
    if(pathwayList.size() == 0)
    {
        cout << "No paths were found for this maze" << endl;
    }
    //Printing out the first pathway in our list of Task 1 is picked
    else if(task == 1)
    {
        cout << "Displaying one possible path: " << endl;
        for(int i=0;i < rows; i++)
        {
            for(int j=0; j < columns; j++)
            {
                bool elementFound = false;
                int pathwayListSize = pathwayList[0].size();

                for(int v = 0;elementFound == false && v < pathwayListSize; v++)
                {
                    if (pathwayList[0][v].row == i && pathwayList[0][v].col == j)
                    {
                        elementFound = true;
                        //if our path contains this cell, print this cell instead of the maze
                        cout << setw(3) << pathwayList[0][v].pathNumb << " ";
                    }
                }
                //
                if(elementFound == false)
                {
                    //If the current cell is not part of the pathway then we print the maze
                    cout << setw(3) << mazeArray[i][j] << " ";
                }

            }
            cout << endl;
        }
    }
    //Printing all the paths found if Task 2 was selected
    else if (task == 2)
    {
        int numberOfPaths = pathwayList.size();

        //We need to print out each pathway found in our PathwayList
        for(int pathwayIndex = 0; pathwayIndex < numberOfPaths; pathwayIndex++)
        {
            cout << "Displaying path number: " << pathwayIndex + 1 << endl;
            for(int i=0; i < rows; i++)
            {
                for(int j=0; j < columns; j++)
                    {
                        bool elementFound = false;
                        int pathwayListSize = pathwayList[pathwayIndex].size();

                        for(int v = 0;elementFound == false && v < pathwayListSize; v++)
                        {
                            if (pathwayList[pathwayIndex][v].row == i && pathwayList[pathwayIndex][v].col == j)
                            {
                                elementFound = true;
                                //Print out the current cell if it matches with the current row and column being printed
                                cout << setw(3) << pathwayList[pathwayIndex][v].pathNumb << " ";
                            }
                        }
                        if(elementFound == false)
                        {
                            //If the current cell is not in the pathway then print out what we normally find in our maze
                            cout << setw(3) << mazeArray[i][j] << " ";
                        }

                    }
                    cout << endl;
            }
        }

    }
    else
    {
        cout << "Invalid Task arguements!";
    }


    //Cleaning up our Dynamically allocated arrays.
    for (int i = 0;i<rows;i++)
    {
        delete [] mazeArray[i];
    }
    delete[] mazeArray;
    mazeArray = nullptr;


    return 0;
}



//Function Definition
bool mazeSolver(char** maze, int row, int column, int maxRow, int maxColumn,vector<Cell> pathway,int pathCount,vector<vector<Cell>>& pathwayList)
{
    Cell currentCell;
    currentCell.row = row;
    currentCell.col = column;
    currentCell.pathNumb = pathCount;

    // instantiate a new path, copying the previous path
    vector<Cell> currentPath = pathway;

    bool inPath = false;
    int pathSize = currentPath.size();

    //Check to see if this cell is already in our pathway
    for(int index = 0;inPath == false && index < pathSize; index++)
    {
        if(currentPath[index].row == currentCell.row && currentPath[index].col == currentCell.col)
        {
            inPath = true;
        }
    }
    //Return false if the current cell is already in our path
    if(inPath == true)
    {
        //cell already in path
        return false;
    }
    //Check to see if we are outside the bounds of our maze
    else if(row < 0 || column < 0 || row >= maxRow || column >= maxColumn)
    {
        // invalid position
        return false;

    }
    //Check to see if the current cell is at a wall
    else if(maze[row][column] == '-')
    {
        //wall haveto go back
        return false;

    }
    //Check to see if we are at the exit point of our maze
    else if(row == maxRow - 1 && column == maxColumn - 1)
    {
        //destination, return path
        currentPath.push_back(currentCell);
        pathway = currentPath;
        //int pathlength = currentPath.size();
        pathwayList.push_back(currentPath);

        return true;
    }
    else
    {
        //This is a valid path so far and we need to place it in our pathway
        // then recall our function on the adjacent cellls
        currentPath.push_back(currentCell);
        int nextPathnum = currentCell.pathNumb + 1;
        //Going D U R L
        mazeSolver(maze,row+1,column,maxRow,maxColumn,currentPath,nextPathnum,pathwayList);
        mazeSolver(maze,row-1,column,maxRow,maxColumn,currentPath,nextPathnum,pathwayList);
        mazeSolver(maze,row,column+1,maxRow,maxColumn,currentPath,nextPathnum,pathwayList);
        mazeSolver(maze,row,column-1,maxRow,maxColumn,currentPath,nextPathnum,pathwayList);

    }
    return true;
}
