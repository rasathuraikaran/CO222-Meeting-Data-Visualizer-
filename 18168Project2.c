

/****************************************************************************************************************
		CO222: Programming Methodology-	Project 2
The objective of this project is to analyze such
data files and visually represent the data as per the requirement of user.

E/18/168

KARAN R

Date of Submission: 11/10/2021


**************************************************************************************************************************/



//----------------------------------------Libraries----------------------------------------------------------------------//


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>




//--------------------------------------Constants-------------------------------------------------------------------//

# define WHITESPACES 3
#define MAXPRINTLETTERS 80


//----------------------------------------------User Defined DataTypes------------------------------------------------------------//
//Create a Node that contains NameOf Host and  No. of Meetings, No. of Participants and  their Duration.
typedef  struct node {
    char hostName[100];
    int numberOfmeeting;
    int participationOfMeeting;
    int durataion;
    struct node *next;
}node;


typedef enum __ {
    NOTSCALED, SCALED
} scaler;

//create a boolean datatype
typedef enum ___ {
    FALSE, TRUE
} bool;

//------------------------------------------------------------------------------------------------------------------------//







//-------------------------------------------------Functions Prototype-----------------------------------------------------------------------//



//check the validation  OF inputs
void inputValidation(int count, char **array);

//Take all the data from a file
node *collectingData(char filename[100]);

//Mergesort function to run  the Mergesort alogorithms
void MergeSort(node **headRef, char dataSelector[2]);

// In mergesort algorithsm first we have to separate  each node by dividing by using midpoint
void separation(node *source,
                node **frontRef, node **backRef);

//sortedMerge function for connect all links by comparing the data from each separation
node *SortedMerge(node *a, node *b, char dataSelector[2]);

//Plot the graph
void plotingGraph(int, char [2]);

//To find the Maxmium digits length
int findMaxLength(int n);


//------------------------------------------------------------------------------------------------------------------------//



//-------------------------------------------Gloabal-Variables-----------------------------------------------------------------------------//

node *headerRow = NULL;
int totalParticipants = 0;
int totalMeetings = 0;
int totalDuration = 0;
char dataselector[2] = "-m";
int lengthSelector = -99;//also user may input 0 as lengthselector
int totalHost = 0;
scaler isScaled=NOTSCALED;
bool isFirstFile = TRUE;
bool isNewData = TRUE;
bool isExistingData = FALSE;
char filenames[100][100];

//------------------------------------------------------------------------------------------------------------------------//





//---------------------------------------- Define the Main Function --------------------------------------------------------------------------------//

int main(int argc, char **argv) {


    inputValidation(argc, argv);
    /*
     *The program must take at least one file name to work
     * We are checking if user input correct userDatafile if its acceptable then collect the data from the
     * file after that store in linked List beacuse it is growable
     *In all of the above cases, you may use the following static pattern of the command line arguments to run the program.

    ./prog − m − −scaled − l 10 file1 file2 file3 ...
    where, -m can be changed to either -t or -p and 10 can be any positive integer
     *
     */



    MergeSort(&headerRow, dataselector);

    /* We use here merge sort to sort the Data
     * Merge sort is a sorting technique based on divide and conquer technique.
    * With worst-case time complexity being Ο(n log n), it is one of the most respected algorithms.
     * Merge sort first divides the array into equal halves and then combines them in a sorted manner.
     * Then, merge sort combines the smaller sorted lists keeping the new list sorted too.
     */



    plotingGraph(lengthSelector, dataselector);

    /*if user does not give lenth limit program automatticaly takes 10
     * if exceed userlength it takes max lenth of Data
     * The program  works in 80 character width screen.
     * If user gives over the lenth limit of the graph ,then it plot the graph to totalHost
     */



    return 0;


}


void inputValidation(int count, char **array) {


//The program must take at least one file name to work.so count should be greater than or equal to 2
    if (count < 2) {
        printf("No input files were given\nusage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",array[0]);
        exit(0);

        // return 0
    }
//checking for invalif flags
    for(int i=0;i<count;i++){
        if(array[i][0]=='-'){
            if(!((strcmp(array[i], "-t") == 0)||(strcmp(array[i], "-m") == 0)||(strcmp(array[i], "-p") == 0)||(strcmp(array[i], "--scaled") == 0)||(strcmp(array[i], "") == 0)||(strcmp(array[i], "-l") == 0)||(isdigit(array[i][1])))){
                printf("Invalid option [%s]\n""usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",array[i],array[0]);
                exit(0);
            }


        }




    }

    for (int i = 0; i < count; i++) {

        // Extract the Strings which contains ".csv"
        char *filename = strstr(array[i], ".csv");
        // printf("%s\n",array[i]);
      //  printf("%s\n",array[i]);
        if (filename) {
            // copy the file name in a array of filenames
            strcpy(filenames[i], array[i]);
            //printf("array[%d] is %s \n",i,array[i]);
          // printf("%s\n",array[i]);
            //create temporarypointer to handle  the linkedList

            node *temporaryPointer;
            if (isExistingData == FALSE) {
                //If it  is firstfile then connect with headerRow
                temporaryPointer = headerRow = collectingData(array[i]);
                isExistingData = TRUE;
            } else {
                while (temporaryPointer->next != NULL) {
                    //move the file to last node of a list .
                    temporaryPointer = temporaryPointer->next;
                }
                temporaryPointer->next = collectingData(array[i]);

            }


        }

        //other file name or fake files name uer inputs show this message

        else if((isdigit(array[i][0]))){
            bool lflag=FALSE;
            for(int i=0;i<count;i++){
                if(strcmp(array[i],"-l")==0){
                    lflag=TRUE;
                    break;

                }
            }
            if(lflag==FALSE){
                printf("Only .csv files should be given as inputs.\n");
                exit(0);
            }


        }

        else if(!(array[i][0]=='-'||array[i][0]=='.')){
            printf("Only .csv files should be given as inputs.\n");
            exit(0);

        }




    }


    char *endptrs;


    for (int i = 0; i < count; i++) {
        // printf("dataselector is karan=>\n");
        if (strcmp(array[i], "-t") == 0) {
            strcpy(dataselector, array[i]);
            //printf("dataselector is => %s kllm\n", dataselector);


        } else if (strcmp(array[i], "-m") == 0) {
            strcpy(dataselector, array[i]);

        } else if (strcmp(array[i], "-p") == 0) {
            strcpy(dataselector, array[i]);

        }


        if (strcmp(array[i], "-l") == 0) {
        //checking array i+1 to check user input number or not
            if (array[i + 1] == NULL) {
                printf("Not enough options for [-l]\nusage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",array[0]);
                exit(0);
            }

            /* Convert a string to an unsigned long integer.  */
            lengthSelector = strtol(array[i + 1], &endptrs, 10);
            if (*endptrs != '\0' || endptrs == array[i + 1]) {
               printf("Invalid option [%s]\n""usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",array[i],array[0]);
                exit(0);
            } else if (lengthSelector >=totalHost) {

                //if lenth is higth than total host then take length selector as sum of totalHost
                lengthSelector =totalHost ;
                //printf("totalHost%d\n",totalHost);


            }
            else if(lengthSelector<0) {
                printf("Invalid option(negative) for [-l]\nusage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",array[0]);
                exit(0);

            }
            else{
                //printf("lenth selector is %d\n",lengthSelector);

            }



        }


        if (strcmp(array[i], "--scaled") == 0) {
            isScaled = SCALED;
            //When − − scaled argument is given, the first row of the graph should fully occupy the max print
            //width. Any other row should scale to be matched with first row scale factor

        }


    }
    if (lengthSelector == -99) {
        lengthSelector = 10;
        //If user does not give the length selector then it triggered to 10
    }


}


node *collectingData(char filename[100]) {


    // creating some local variables to extract data from each row from file
    char hostName[100];
    int participantsOfMeeting;
    int hrs;
    int min;
    int sec;



    //Create a pointer to hold the node adrees of HeadNode.
    node *headerRows = NULL;

    //create file pointer
    FILE *file;

    // open a file to read.
    file = fopen(filename, "r");

    if (file == NULL) {
        //exit the program after printing the statement
        printf("Cannot open one or more given files\n");
        exit(0);


    }
    static node *row;



    //created  boolean from enum datatype to check NewData or Existing Data.


    // printf("Karan1\n");
    while (fscanf(file, "%[^,],%d,%d:%d:%d\n", hostName, &participantsOfMeeting, &hrs, &min,
                  &sec) != EOF) {


        if (isExistingData == TRUE) {
            //Find the node and update the node variables
            node *currentRow = row;
            while (currentRow != NULL) {
                if (strcmp(currentRow->hostName, hostName) == 0) {


                    currentRow->numberOfmeeting = currentRow->numberOfmeeting + 1;
                    currentRow->participationOfMeeting =
                            currentRow->participationOfMeeting + participantsOfMeeting;
                    currentRow->durataion = currentRow->durataion + hrs * 60 + min;

                    isNewData = FALSE;
                    break;
                }


                currentRow = currentRow->next;


            }
        }


        if (isNewData == TRUE) {
            // printf("Karan2\n");
            row = (node *) malloc(sizeof(node));
            strcpy(row->hostName, hostName);
            row->participationOfMeeting = participantsOfMeeting;
            row->durataion = hrs * 60 + min;
            row->numberOfmeeting++;
            row->next = headerRows;
            headerRows = row;
            totalHost++;
        } else {
            isNewData = TRUE;
        }
        isExistingData = TRUE;

    }
    //close the file
    fclose(file);





    return headerRows;


}










void MergeSort(node **headReference, char dataSelector[2]) {


    node *head = *headReference;
    node *front;
    node *end;




    if ((head == NULL) || (head->next == NULL)) {

        return;
    }

    /*  it is going to Split head into 'front' and 'end' */
    separation(head, &front, &end);


    /* Recursively    split  the sublists */
    MergeSort(&front, dataSelector);
    MergeSort(&end, dataSelector);

    /*merge the sort list */
    *headReference = SortedMerge(front, end, dataSelector);

}


node *SortedMerge(node *a, node *b, char dataSelector[2]) {

    node *result = NULL;


    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);


    /*Choose either an or b, and repeat.    */
    //First we compare Data selector
    if (strcmp(dataselector, "-t") == 0) {

        if (b->durataion < a->durataion) {

            result = a;
            result->next = SortedMerge(a->next, b, dataSelector);
           //using recursion method

        } else {
            result = b;
            result->next = SortedMerge(a, b->next, dataSelector);
        }


    } else if (strcmp(dataselector, "-p") == 0) {
        if (b->participationOfMeeting < a->participationOfMeeting) {
            result = a;
            result->next = SortedMerge(a->next, b, dataSelector);
		 //using recursion method

        } else {
            result = b;
            result->next = SortedMerge(a, b->next, dataSelector);
             //using recursion method
        }


    } else {
        if (b->numberOfmeeting < a->numberOfmeeting) {

            result = a;
            result->next = SortedMerge(a->next, b, dataSelector);


        } else {
            result = b;
            result->next = SortedMerge(a, b->next, dataSelector);
        }
    }


    return (result);
}


/* Divide the given list's nodes into front and back halves, and return the two lists using the reference parameters.
If the length is odd, the extra node should be placed at the top of the list.
The fast/slow pointer strategy is used.
 */

void separation(node *source,
                node **frontRefe, node **backRefe) {
    /*Here we have to find midpoint so there are we create two pointer
     * slow pointer indicates first node then second node indicates fast pointers
     * fast pointer  check the next node if its does not null it moves fast pointer by one node as well as slow pointer also moves
     * finally fast pointer find null then you can see slow pointer in midpoint
        */
    node *fast;
    node *slow;
    slow = source;
    fast = source->next;


    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }


    *frontRefe = source;
    *backRefe = slow->next;
    slow->next = NULL;// we cut of the link list by assigning  NUll


}












void plotingGraph(int limitOfGraph, char dataselector[2]) {



 for (node *currentRow = headerRow; currentRow != NULL; currentRow = currentRow->next) {

        //to find out the total values to calculations
        totalDuration = totalDuration + currentRow->durataion;
        totalParticipants =totalParticipants + currentRow->participationOfMeeting;
        totalMeetings = totalMeetings + currentRow->numberOfmeeting;

    }




    int limit = limitOfGraph;
    int breaker = 0;
    int plottingArea;
    int headData;
    int currentData;
    node *current = headerRow;
    char bigName[100];
    int lengthofBigname;
    int sumOfData;
    int bignumber;

    //Initialu set big name and bignumber to initial node claue

    strcpy(bigName, headerRow->hostName);
    if (strcmp(dataselector, "-t") == 0) {

        bignumber = headerRow->durataion;

    } else if (strcmp(dataselector, "-m") == 0) {

        bignumber = headerRow->numberOfmeeting;

    } else if (strcmp(dataselector, "-p") == 0) {
        bignumber = headerRow->participationOfMeeting;

    }
    printf("\n");
    // there is one line output in sample program

    for (node *current = headerRow; current != NULL; current = current->next) {
// limit means lengthselector
        if (breaker == limit) {

            breaker = 0;

            break;
        }

        if (strlen(current->hostName) > strlen(bigName)) {

            strcpy(bigName, current->hostName);

        }
        if (strcmp(dataselector, "-t") == 0) {
            if (current->durataion > bignumber) {

                bignumber = current->durataion;

            }
        } else if (strcmp(dataselector, "-m") == 0) {
            if (current->numberOfmeeting > bignumber) {

                bignumber = current->numberOfmeeting;

            }

        } else if (strcmp(dataselector, "-p") == 0) {
            if (current->participationOfMeeting > bignumber) {

                bignumber = current->participationOfMeeting;

            }
        }


        breaker++;
    }


    int lengthofBigNumber = findMaxLength(bignumber); // bignumber  is 43221  => length of bignumber is 5

    lengthofBigname = strlen(bigName);


    int MaxplottingCharacters = MAXPRINTLETTERS - lengthofBigname - WHITESPACES - lengthofBigNumber;// place of bar chart


    breaker = 0;// to break c when necesssary
    for (node *current = headerRow; current != NULL; current = current->next) {
        if (strcmp(dataselector, "-t") == 0) {
            currentData = current->durataion;
            headData = headerRow->durataion;
            sumOfData = totalDuration;

        } else if (strcmp(dataselector, "-m") == 0) {
            currentData = current->numberOfmeeting;
            headData = headerRow->numberOfmeeting;
            sumOfData = totalMeetings;

        } else if (strcmp(dataselector, "-p") == 0) {

            currentData = current->participationOfMeeting;
            headData = headerRow->participationOfMeeting;
            sumOfData = totalParticipants;

        }
        if (isScaled == SCALED) {
            plottingArea = (MaxplottingCharacters * currentData) / headData;
        } else {
            plottingArea = (MaxplottingCharacters * currentData) / sumOfData;

        }


        if (breaker == limit) {
            breaker = 0;
            break;
        }



        for (int i = 0; i < lengthofBigname + 2; i++) {
            printf(" ");
        }
        printf("│");
        for (int i = 0; i < plottingArea; i++) {
            printf("░");
        }

        printf("\n");
        for (int i = 0; i < lengthofBigname + 2; i++) {
            if (i == 0) {
                printf(" ");

            } else if (i < strlen(current->hostName) + 1) {
                printf("%c", current->hostName[i - 1]);
            } else {
                printf(" ");
            }


        }

        printf("│");
        for (int i = 0; i < plottingArea; i++) {
            printf("░");
        }
        printf("%d", currentData);
        printf("\n");
        for (int i = 0; i < lengthofBigname + 2; i++) {
            printf(" ");
        }
        printf("│");
        for (int i = 0; i < plottingArea; i++) {
            printf("░");
        }
        printf("\n");
        for (int i = 0; i < lengthofBigname + 2; i++) {
            printf(" ");
        }
        printf("│\n");


//Draw a last line or fill the x axis
        if (breaker == limit-1) {
            for (int i = 0; i < lengthofBigname+2; i++) {
                printf(" ");
            }
            printf("└");
            for (int i = 0; i < MAXPRINTLETTERS - lengthofBigname - WHITESPACES ; i++) {
                printf("─");
            }
            printf("\n");


        }


        breaker++;

    }


}
//find the maximum digits of the given  integer

int findMaxLength(int n) {
    static int counter = 0; // variable declaration
    if (n > 0) {
        counter = counter + 1;
        return findMaxLength(n / 10);
    } else
        return counter;
}



