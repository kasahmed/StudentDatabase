/*
       Group 8
	   Written by: Kashan Ahmed, Surya Mukherjee, Rohan Pai
	   Date: 12/6/2013
*/
#include "bstADT.h"
#include "stackADT.h"
#include "hashADT.h"
#include "string.h"


//	Prototype Declarations
typedef struct
{
    char studentId[7];
    char *studentName;
    double studentGpa;
    int unitsEarned;
}DATA;


int readFile(char *fileName);
int compareId (void* num1, void* num2);
int compareString (void* string1, void* string2);
int encryption(void *key, int maxSize);
int generateNextPrime(int starting);


void printData   (void* num1);
void buildData(char *fileName, BST_TREE* BSTRoot, BST_TREE* BSTRootName, HASH* hashId);
void stringSize(DATA *pData, char *temp);
void insertNewData(BST_TREE **BSTRoot, BST_TREE **BSTRootName, HASH **HashId);
void deleteData(BST_TREE *BSTRootId, BST_TREE* BSTRootName, HASH* hash, STACK *stack);
void undoStack(BST_TREE* BSTRootId,  BST_TREE* BSTRootName, HASH* hash, STACK *stack);
void searchId(HASH* hashId);
void searchName(BST_TREE *treeName);
void printBST   (void* num1);
void printHash  (HASH* hash);
void printPrettyBST(NODE* node, int level);
void printTree(void* stu, int indent);

void freeMemory(void *dataPtr);
void doNothing(void *dataPtr);
void fileWritePTR(void* data);
void writeToFile(HASH* hash);
void efficiency (HASH *hash);

bool getData (FILE* fpData, DATA* pData);

char menuPage(void);

int main (void)
{
//	Local Definitions
	BST_TREE* BSTRootId;
	BST_TREE* BSTRootName;
	HASH* hashId;
	STACK *stack;

    int maxSize;

	char userInput;
	char *fileId;

	fileId = "gpa.txt";


//	Statements

	BSTRootId = BST_Create (compareId);
	BSTRootName = BST_Create (compareString);

	if((maxSize = 2* readFile(fileId)) <= 0)
        printf("There is no data in the file %s", fileId), exit(1);
    maxSize = generateNextPrime(maxSize);

	hashId = hashCreate(maxSize,encryption, compareId);

	buildData(fileId, BSTRootId, BSTRootName, hashId);


	stack = createStack();

    do
    {

        userInput = menuPage();

        if(userInput == 'a' || userInput == 'A')
            BST_Traverse (BSTRootId, printData);
        if(userInput == 'b' || userInput == 'B')
            BST_Traverse(BSTRootName, printData);
        if(userInput == 'c' || userInput == 'C')
            searchId(hashId);
        if(userInput == 'd' || userInput == 'D')
           searchName(BSTRootName);
        if(userInput == 'e' || userInput == 'E')
            insertNewData(&BSTRootId, &BSTRootName, &hashId);
        if(userInput == 'f' || userInput == 'F')
            BST_BFS(BSTRootId->root, printData);
        if(userInput == 'm' || userInput == 'M')
            deleteData(BSTRootId, BSTRootName, hashId, stack);
        if(userInput == 'n' || userInput == 'N')
            hashTraverse(hashId, printData);
        if(userInput == 's' || userInput == 'S')
            writeToFile(hashId);
        if(userInput == 'o' || userInput == 'O')
            printHash(hashId);
        if(userInput == 'p' || userInput == 'P')
            BST_Graphic(BSTRootId, printTree);

        if(userInput == 'u' || userInput == 'U')
            undoStack(BSTRootId, BSTRootName, hashId, stack);
        if(userInput == 'h' || userInput == 'H')
            efficiency(hashId);

    }while(userInput != 'q');

    stack = destroyStack(stack, freeMemory);
    BST_Destroy(BSTRootName, freeMemory);
    BST_Destroy(BSTRootId, NULL);
    hashDestroy(hashId, NULL);

	return 0;
}	// main

/*  -readFile-
    This program will read the file and count
    the number of lines that are in the input file that
    contains data

    returns the number of lines that contain data

*/
int readFile(char *fileName)
{
    FILE *fpData;

    char temp[50];

    int i;

    i = 0;

    fpData = fopen(fileName, "r");
    if(!fpData)
	{
	    printf("Error opening file %s\a\n", fileName);
	    exit (99);
	} // if open fail

    while(fscanf(fpData, "%s %[^;]; %s %s", temp,temp,temp,temp) != EOF)
        i++;

    fclose(fpData);
    return i;
}
/*  BuildData
This funciton call getData which reads the text file and will retrieve
the data from getData and insert the data into both of the tree and hash table.
(All will have same information )
*/
void buildData(char *fileName, BST_TREE *BSTRoot, BST_TREE *BSTRootName, HASH* hash)
{
    FILE *fpData;
    DATA data;
    DATA *dataPtr;

    fpData = fopen(fileName, "r");
    if (!fpData)
	{
	    printf("Error opening file %s\a\n", fileName);
	    exit (101);
	} // if open fail

	while (getData (fpData, &data))
	{
	    dataPtr = (DATA*) malloc(sizeof(DATA));
        *dataPtr = data;
        BST_Insert (BSTRoot, dataPtr);
        BST_Insert (BSTRootName, dataPtr);
        hashInsert(hash, dataPtr);
	} // while

    fclose(fpData);
}

/*  getData
This funciton will read the input file and pass
back the data in the file to the calling function.

*/

bool getData (FILE* fpData, DATA* pData)
{
//	Local Definitions
	int ioResult;
    char temp[50];


//	Statements

    ioResult = fscanf(fpData, "%s %[^;]; %lf %d", (pData->studentId), temp, &(pData->studentGpa), &pData->unitsEarned);
	stringSize(pData,temp);

	return ioResult == 4;
}	// getData


/*	==================== compareInt ====================
	Compare two integers and return low, equal, high.
	    Pre  num1 and num2 are valid pointers to DATA
	    Post return low (-1), equal (0), or high (+1)
*/
int compareId (void* num1, void* num2)
{
//	Local Definitions

    int key1;
    int key2;

    key1 = atoi((char*)num1);
    key2 = atoi((char*)num2);

//	Statements
	if (key1 < key2)
        return -1;
	if (key1 == key2)
	    return 0;
	return +1;

}	// compareInt

/*	==================== compareString ====================
	Compare two strings and return -1, 0, 1.
	    Pre  string1 and string2 are valid pointers to DATA
	    Post return low (-1), equal (0), or high (+1)
*/
int compareString (void* string1, void* string2)
{
//	Local Definitions
    DATA key1 = *((DATA*)string1);
	DATA key2 = *((DATA*)string2);


    return strcmpi(key1.studentName, key2.studentName);
//	Statements

}	// compareInt

/*	==================== encryption ====================
    This is the hashing function that hashes the key
    by modulusing with maxSize of the table

    returns the key index
*/
int encryption(void *key, int maxSize)
{
    char *name = (char*)key;
    int size = strlen(name);

    int i = 0;
    int hash = 0;
    for(i = 0; i < size; i++)
            hash += name[i];


    hash = hash % maxSize;
    return hash;
    //return hash;
    //return (encriptKey = atoi((char*)key));

}
/*	====================== printData ======================
	Prints data
	the pointer passed is a void pointer and will get data
	from it to print
*/
void printData (void* data)
{
//	Statements
    DATA studentInfo = *((DATA*)data);

	printf("%30s %s %2.2f %d\n", studentInfo.studentName, studentInfo.studentId, studentInfo.studentGpa, studentInfo.unitsEarned);
	return;
}	// printBST

/*  -printHash-
    This function will print the the hash table in seq depending
    on the index from smallest to largest

*/
void printHash(HASH *hash)
{
    int i;

    for(i = 0; i < hash->maxSize; i++)
    {
        if(hash->HashArray[i] != NULL)
        {
            printf("%5d.", i);
            printData(hash->HashArray[i]);
        }
    }

}
/*  -printTree-
    This function will print data in tree format

*/
void printTree(void* stu, int indent)
{
	int i;


	if(indent == -1){
        DATA stuPT = *((DATA*)stu);
		printf("%s \n", stuPT.studentId);
	} else {
		for(i = 0; i < indent; i++){
			printf("        ");
		}
	}

}

/*  InsertNewData

    This function will get user Input to add a new student
    asking for student id whiches is checked to make sure
    the ID is unique, gets student Name from user and the gpa
    and inserts the new data into two trees and hash table  returns the newly
    sorted tree containing the new student along with the hash table to the main function

*/
void insertNewData(BST_TREE **BSTRootId, BST_TREE **BSTRootName, HASH **hashId)
{
    DATA newData;
    DATA *dataPtr;
    char temp[500];
    NODE *holder;

    printf("Enter the student ID: ");
    scanf("%s", newData.studentId);
    while((holder = BST_Retrieve(*BSTRootId, &newData)))
    {
        printf("The student %s id has already been assigned to an other student\n", newData.studentId);
        printf("please enter another Id #: ");
        scanf("%s", newData.studentId);
    }
    printf("Please enter the name of the student divided by a cama (last, first)\n-> ");
    do
    {
        fgets(temp,500,stdin);
    }while(temp[0] == '\n');

    stringSize(&newData,temp);
    printf("Enter the student's GPA: ");
    scanf("%lf", &newData.studentGpa);
    printf("Enter the units the Student has earned: ");
    scanf("%d", &newData.unitsEarned);


    if(!(dataPtr = (DATA*) malloc(sizeof(DATA))))
    {
        printf("Memory overflow");
        exit(103);
    }

    *dataPtr = newData;
    BST_Insert (*BSTRootId, dataPtr);
    BST_Insert (*BSTRootName, dataPtr);
    hashInsert(*hashId, dataPtr);

    free(holder);
}

/*  - deleteData
This function will ask the user to delete an ID by asking for the Id.
It will search for the data through the BST Tree and if found it will delete
from the 2 trees and hash list but the stack will hold the deleted student data
*/
void deleteData(BST_TREE* BSTRootId, BST_TREE* BSTRootName, HASH* hash, STACK* stack)
{
    NODE* holder;
    char deleteKey[8];

    printf("Enter a student ID to delete: ");
    scanf("%s", deleteKey);

    if(!(holder = (BST_Retrieve(BSTRootId, deleteKey))))
        printf("\nStudent Id %s was not found\nDelete Failed\n\n", deleteKey);
    else
    {
        pushStack(stack,holder);
        BST_Delete(BSTRootId, &deleteKey);
        BST_Delete(BSTRootName, holder);
        hashDelete(hash, &deleteKey);
    }
}

/*  -undoStack-
This function will look in the stack to see if any data has been recently deleted
then insert that data if found back in the trees and hash table

*/
void undoStack(BST_TREE* BSTRootId, BST_TREE* BSTRootName, HASH* hash, STACK* stack)
{
    NODE* holder;

    if((holder = popStack(stack)))
    {
        BST_Insert(BSTRootId, holder);
        BST_Insert(BSTRootName, holder);
        hashInsert(hash, holder);
        printf("\nA student has been restored\n");
    }
    else
        printf("No student has been deleted\n\n");

}
/*  stringSize

This function allocates appropiate memory of the student data
studentsName.

*/
void stringSize(DATA * pData, char *temp)
{
    int stringLengh;

    stringLengh = strlen(temp);
    if(temp[stringLengh - 1] == '\n')
        temp[stringLengh - 1] = '\0';

    if(!(pData->studentName = (char*) calloc(stringLengh,sizeof(char))))
    {
        printf("Memory overflow");
        exit(102);
    }

    strcpy(pData->studentName,temp);
}

/*  searchId
    This function gets the id the user wants
    to search and then tries to find it. If the data is
    found it will print the data related to that id. If that
    Id does not exist it will print out a message saying
    the Id was not found.
*/
void searchId(HASH* hashId)
{
   // DATA data;
   char target[7];
   DATA* data;

    data = (DATA*)malloc(sizeof(DATA));
    printf("Enter id to search: ");
    scanf("%s", target);

    if(!RetriveHashData(hashId, &target, printData))
        printf("Id: %s was not found", target);

}

/*  searchName
    This function gets the name the user wants
    to search, finds the name and prints all information
    associated with the name. if the name was not found
    it will print a message saying the name was not found

*/
void searchName(BST_TREE* treeName)
{
    bool found;
    DATA data;
    char name[50];


    printf("Enter the students name, make sure to add a coma between frist and last name\n-> ");
    do
    {
        fgets(name,50,stdin);
    }while(name[0] == '\n');
    stringSize(&data, name);

    found = BST_RetrieveMultiple(treeName, &data, treeName->root, printData);
    if(found == false)
        printf("Student name %s was not found!", data.studentName);

    free(data.studentName);
}

/*  menuPage
    This function will print all the menu options the user can
    chose froms and returns the char the user entered to the
    calling function
*/
 char menuPage()
 {
    char userInput;

    printf("\n\n");

        printf("\t----Menu options----\n");
        printf("\tA. Print List Sorted by PIN\n");
        printf("\tB. Print List sorted by name\n");
        printf("\tC. Search by PIN\n");
        printf("\tD. Search by name\n");
        printf("\tE. Add a new student\n");
        printf("\tF. Print by level\n");
        printf("\tM. delete student\n");
        printf("\tN. Print in hash order\n");
        printf("\tO. Print data with hash key\n");
        printf("\tP. Print like a tree\n");
        printf("\tS. Save data to file\n");
        printf("\tU. Undo Delete\n");
        printf("\tQ. Quit.\n");
        printf("Enter the key corresponing to the action: ");
        scanf("%s", &userInput);

    printf("\n\n");

     return userInput;
 }

/*
    This function will free memory in the data struct
*/
 void freeMemory(void *dataPtr)
 {
    DATA* studentInfo = ((DATA*)dataPtr);

    free(studentInfo->studentName);
    printf("studentName freed!\n");
 }

/*  -efficiency-
    This function will so the efficiency of the hash
    and also the hash statistics

*/
void efficiency(HASH *hash)
{
    float loadFactor;

    loadFactor = ((float)hash->size / hash->maxSize) *100;

    printf("\nEfficiency of the program: \n");
    printf("Number of collisions: %d", hash->collisions);
    printf("\nLongest collision path: %d", hash->lcp);
    printf("\nLoad factor:  %3.2f", loadFactor);
    printf("\nAbout Hash:\n");
    printf("Amount of data in hash: %d", hash->size);
    printf("\nMax size of hash:       %d\n\n", hash->maxSize);

}

/*
    This function does nothing
*/
 void doNothing(void *dataPtr)
 {

 }

/*  -fileWritePTR-
    This function writes the hash seq in key order
    in a output file
*/
 void fileWritePTR(void* data)
 {
    DATA* stud = (DATA*)data;
    FILE* pFile;

	pFile = fopen ("output.txt","a");

    fprintf(pFile, "%s %s %lf %d\n", stud->studentId, stud->studentName, stud->studentGpa, stud->unitsEarned);
    fclose(pFile);
}

/*  writeToFile
    This function writes to file by creating the
    output file and going through the hashfunction
    it sends a pointer to a write
*/
void writeToFile(HASH* hash)
{
    FILE* pFile;

    pFile = fopen ("output.txt","w");
    fclose(pFile);
	hashTraverse(hash, fileWritePTR);

}

int generateNextPrime(int starting)
{
    int i;
    int j;

    for (i=starting; i<100; i++)
    {
        int prime=1;
        for (j=2; j*j<=i; j++)
        {
            if (i % j == 0)
            {
                prime=0;
                break;
            }
        }
        if(prime) return i;
    }
    return 0;
}

