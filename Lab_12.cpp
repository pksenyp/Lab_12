#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

/* Create a new Trains data type with the fields specified in the variant */
typedef struct
{
    char station[30];
    int number[20];
    char time[10];

} Trains;

void error_open(FILE* origin) // Function to process the opening of the file, pass in the arguments a pointer to the file we want to check
{
    if (origin == NULL) // fopen returns NULL if the file was not opened
    {
        printf("\n-----------------Failed to open file--------------------\n");
    }
}

void error_close(FILE* origin) // The function to close the fclose file returns 0 if closing EOF successfully if there was an error
{

    if (fclose(origin))
    {
        printf("\n-------------------Failed to close file-------------------\n");
    }
}

void WriteF(Trains* mas, int* n) // Read function from binary file
{

    FILE* pf = fopen("structures.bin", "wb"); // open wb file, i.e. if there is no file, it will be created, if there is one, it will be overwritten, the file itself will be opened in binary format

    if (pf != NULL) // check for errors opening the file
    {
		/* the fwrite function accepts the pointer to the file from where to write, the size of one record, how many times to repeat the record and the pointer to the file to write to as arguments. First, let's record the number of records in the file, thus shifting the flow to the size *n */
        fwrite(n, sizeof(*n), 1, pf); 
        fwrite(mas, sizeof(Trains), *n, pf); // the pointer in the flow has moved from *n to the record itself. The fwrite function returns the number of successfully read characters
        fclose(pf); // close the file
    }
    else
    {
        printf("Error opening binary file\n"); // print an error if the file was not opened
    }
}

Trains* ReadF(int* n) // read function from the file
{
    int k;

    FILE* pf = fopen("structures.bin", "rb"); // open the file for binary reading
    if (pf != NULL && fread(&k, sizeof(k), 1, pf) != 0) // check the opening of the file for errors and also check the number of successfully read characters, if it is 0, then the file is empty
    {
        /* the function to read from the fread file takes all the same arguments as fwrite, only n in this case is where to write and pf is where to write */
        *n = k; // because we did not use a pointer to the number of records in the database when checking if the file is empty, then if the file is not empty we should change it
        Trains* New = (Trains*)calloc((k), sizeof(Trains)); // allocate memory for n structures of the Trains array
        fread(New, sizeof(Trains), k, pf);  // read the structures in New

        fclose(pf); // close the file
        puts("\nReading from a binary file was successful \n");
        return New;
    }
    else
    {
        printf("\nThe binary is empty or an error occurred while opening it\n");
        printf("\nDatabase reset\n");
        *n = 0;
        return NULL; // return a blank if the file hasn't opened
    }
}

Trains enter() // Data entry function of the new structure
{
    FILE* f; // create a pointer to a file
    f = fopen("structures.txt", "ab+"); // open it for reading only (no longer binary but ordinary)
    error_open(f); // check for opening errors

    Trains tmp; // create a new structure
    printf("Enter name of station: "); // enter the data on the keyboard
    scanf("%s", &tmp.station);
    fwrite(&tmp.station, sizeof(char), 1, f);

    printf("Enter number of train: ");
    scanf("%s", &tmp.number);
    fwrite(&tmp.station, sizeof(char), 1, f);

    printf("Enter departure time: ");
    scanf("%s", &tmp.time);
    fwrite(&tmp.station, sizeof(char), 1, f);

    return tmp; // bringing back the new structure
}

Trains* Add(Trains* Old, int* n, Trains tmp) // The function of adding a structure
{

    Old = (Trains*)realloc(Old, sizeof(Trains) * (*n + 1)); // allocate memory for another structure

    Old[*n] = tmp; // to the last element of the array (i.e. this will be the memory allocated above), we assign the structure that we pass in the arguments

    (*n) = (*n) + 1; // change n databases through a pointer

    return Old; // return the augmented structure array
}

void show_item(Trains tmp) // single-structure print function
{
    printf("%s\t%s\t%s\n", tmp.station, tmp.number, tmp.time);
}

Trains* Add_2(Trains* Old, int* np) // Function to search in the binary file of the structure with the entered from the keyboard destination
{
    FILE* f;                                  // create a pointer to the file
    f = fopen("structures.bin", "rb");        // open the file in binary reading mode
    char* str = (char*)malloc(sizeof(char)); // allocate memory to enter the destination
    printf("Enter name of station \n");
    scanf("%s", str); // enter a destination
    Trains tmp; // create a structure

    int k, sign = 0;

    fread(&k, sizeof(k), 1, f); // read the number of structures in k in the binary file

    for (int i = 0; i < k; i++) // go through a loop of k times
    {

        fread(&tmp, sizeof(tmp), 1, f); // consecutively read the structures from the binary file

        if (strcmp(tmp.station, str) == 0) // compare the entered item with the item of the current structure in the file
        {
            show_item(tmp); // if the conditions are true, we put this structure into the console

            Old = Add(Old, np, tmp); // add structure to the array
            sign = 1;                // change the sign that the structure was found
            return Old;              // bringing back the new structure
        }
    }
    if (sign == 0) // if the sign remained equal to 0, then no matches were found and we print the corresponding message and return the original array
    {
        puts("No matches found!\n");
        return Old;
    }

    free(str);      // free up the memory for str
    error_close(f); // close the file
}

Trains* Del(Trains* Old, int* n, char* Nazv) // function of removing the structure from the array by the entered destination
{

    int k = 0;
    for (int i = 0; i < *n; i++) // read the number of matches of the entered point with points of all structures
    {
        if (strcmp(Old[i].station, Nazv) == 0)
        {
            k++;
        }
    }

    Trains* New = (Trains*)calloc(((*n) - k), sizeof(Trains)); // allocate memory for an array

    for (int i = 0, j = 0; i < (*n); i++) // write all structures into the new array except for those whose destination coincides with the one entered by the user
    {
        if (strcmp(Old[i].station, Nazv) != 0)
        {
            New[j] = Old[i];
            j++;
        }
    }
    if (k == 0) // if there is no point on which the deletion takes place, we will display the corresponding message
    {
        printf("No matches found!\n");
    }
    else
    {
        printf("Removal occurred\n"); // If the deletion has occurred, we print out the message
    }

    *n = (*n) - k; // change the number of structures
    return New; // returning the new pointer
}

void print_all(Trains* t, int n) // the print function of all structures
{
    for (int i = 0; i < n; i++)
    {
        printf("\t%d. Name of station: %s", (i + 1), t[i].station);
        printf("\tNumber of train: %s", t[i].number);
        printf("\tDeparture time:  %s", t[i].time);
        printf("\n\n");
    }
}

void find_by_name(Trains* mas, int n, char* NAME) // feature to search for structures by item and display them on the user's screen
{
    int f = 0;
    for (int i = 0; i < n; i++) // go through an array of structures and compare items, strcmp takes 2 strings in its arguments and returns 0 if they match
    {
        if (strcmp(mas[i].station, NAME) == 0)
        {
            f = 1; // change f if there are any matches
            show_item(mas[i]);
        }
    }
    if (f == 0) // if there are no matches, we print the message
    {
        printf("No matches found!\n");
    }
}

void sort_by_name(Trains *mas, int n) // sorting the structures in alphabetical order by item
{
    for (int i = 0; i < n; i++) // compare each item with all the next ones and switch places
    {
        for (int j = 0; j < n - 1; j++)
        {
            if (strcmp(mas[j].station, mas[j + 1].station) > 0) //the strcmp function returns a positive number if the lines are different and the code of the first character in the line is larger than the code of the character at the same position in another line
            {
                Trains tmp = mas[j]; // swapping structures
                mas[j] = mas[j + 1];
                mas[j + 1] = tmp;
            }
        }
    }
}

Trains* load_from_txt(int* np, Trains* Old) // uploading data to the database from a text file
{
    FILE* f; // create a pointer to the file

    f = fopen("structures.txt", "r"); // open it for reading only (no longer binary but ordinary)

    error_open(f); // check for opening errors
    int k;

    if (fscanf(f, "%d", &k) != EOF)
    { // fscanf works in the same way as the scanf function, except that it reads information from the thread specified by the first argument. Here we take the number of structures from the file and assign it to np
        *np = k;
        Trains* New = (Trains*)calloc(k, sizeof(Trains)); // allocate memory for a new array with data from txt

        for (int i = 0; i < k; i++) // read the structures until the end of the file, i.e. fscanf, when it reaches the end of the file, will return EOF, in other cases this function returns the number of arguments to which values were actually assigned
        {
            if (fscanf(f, "%s%s%s", New[i].station, &(New[i].number), New[i].time) == EOF)
            {
                break;
            }
        }
        error_close(f); // close the file

        free(Old);
        puts("The data from the text file is extracted to the database\n");
        return New; // return an array with data from a file
    }
    else
    {
        puts("Text file is empty\n"); // if the text file is empty, the base will remain the same
        return Old;
    }
}

void save_txt(Trains* arr, int n) // saving data from the array into txt works the same way as the load_from_txt function, but fpintf is used instead of fscanf. This function accepts the same arguments but does not read but writes the data from the array to the file
{
    FILE* f;
    f = fopen("structures.txt", "w"); // открываем файл для перезаписи
    error_open(f);
    fprintf(f, "%d\n", n);

    for (int i = 0; i < n; i++)
    {
        fprintf(f, "%s %s %s\n", arr[i].station, arr[i].number, arr[i].time);
    }

    error_close(f); // close the file
    puts("\nWriting to txt file is completed.\n");
}

char* search_for_item() // structure search in the binary file by number in the binary file
{
    FILE* f = fopen("structures.bin", "rb"); // open the file for binary reading
    Trains tmp; // create a structure that we'll find
    int choice = 0, k;

    //  rewind(f);
    fread(&k, sizeof(k), 1, f); // read the number of structures in the binary file and write that number in k
    printf("Enter the number of the record you want to extract: \n");
    scanf("%d", &choice); // enter structure number
    printf("\n");
    if (f != NULL && choice > 0 && choice <= k) // check for errors in opening the file and correctness of the number of the entered structure (the number must be within the range from 1 to k )
    {

        for (int i = 0; i < k; i++) // go through a file k times the size of tmp
        {
            if (i == choice - 1) // if i is the same as the user's choice (1 is removed because the numbering of user structures starts with 1 and in the array with 0 )
            {
                fread(&tmp, sizeof(tmp), 1, f); // then read this structure
                show_item(tmp);                 // and show it

                break; // выходим из цикла
            }
            fseek(f, (sizeof(tmp)), SEEK_CUR); // the function of moving through the binary file flow accepts in the arguments a pointer to the file the size to be moved to relative to the position specified in the next argument. SEEK_CUR is the current position, i.e., we pass to the number of structures that the user has entered and read into tmp the last one
        }
        error_close(f); // close the file
        return "true";  // returns true if the reading was successful
    }
    else
    {
        error_close(f); // close the file
        return "false"; // returns false if the data was entered incorrectly or the file did not open
    }
}

void show_menu() // Menu
{
    system("cls");
    printf("\n\n");
    printf("MENU:\n");
    printf("\tDatabase\n");
    printf("\t1 - Adding Trains \n");
    printf("\t2 - Destination deletion \n");
    printf("\t3 - List of all trains \n");
    printf("\t4 - Search for a name of station \n");
    printf("\t5 - Sort by name of station \n");
    printf("\n\n");
    printf("\tBinary file\n");
    printf("\tw - Writing to a binary file \n");
    printf("\tr - Reading from a binary file \n");
    printf("\t6 - Search for train by number in binary file\n");
    printf("\t7 - Search for train by destination in a binary file and add it to the database\n");
    printf("\n\n");
    printf("\tText file\n");
    printf("\tR - Reading from a text file \n");
    printf("\tW - Writing to text file \n");
    printf("\n\n");
    printf("\tProgram\n");
    printf("\tM - On the menu\n");
    printf("\t# - Exit from the program \n");
}

int main()
{
    char* ch = (char*)calloc(1, sizeof(char)); // user selection variable

    int n = 0;    // the number of structures in the database
    int* np = &n; // pointer to the number of structures

    Trains* Trains = NULL; // a pointer to the base array

    char str[20];

    show_menu();

    while (1) // decision cycle
    {

        scanf("%c", ch); // read out the selection

        switch (*ch) // choose by ch
        {
        case '1':
        {
            // system("cls");
            printf("\n");
            Trains = Add(Trains, np, enter()); // add a file and the base is initialized by a new array returned by Add
            printf("Train added\n");

            break;
        }
        case '2':
        {
            // system("cls");
            printf("\n");
            if (n != 0) // check n, you can't remove the structure from an empty base
            {
                printf("Enter name of removable station: ");
                scanf("%s", str); // enter the name of the point and pass it to a function that will return an array without structures with such points
                Trains = Del(Trains, np, str);
            }
            else
            {
                printf("Database is empty\n");
            }

            break;
        }
        case '3':
        {
            // system("cls");
            printf("\n");
            if (n != 0) // print out all the structures from the base if it's not empty
            {
                print_all(Trains, n);
            }
            else
            {
                printf("Database is empty\n");
            }
            break;
        }
        case '4':
        {
            // system("cls");
            printf("\n");
            if (n != 0)
            {
                printf("Enter name of station: ");
                scanf("%s", str);
                find_by_name(Trains, n, str);
            }
            else
            {
                printf("Database is empty\n");
            }

            break;
        }
        case '5':
        {
            // system("cls");
            printf("\n\n");
            if (n != 0)
            {
                sort_by_name(Trains, n);
                printf("Sorted\n");
            }
            else
            {
                printf("Database is empty\n");
            }
            break;
        }
        case 'w':
        {
            // system("cls");
            printf("\n");
            if (n != 0)
            {
                WriteF(Trains, np);
                printf("Database saved\n");
            }
            else
            {
                printf("Database is empty\n");
            }
            break;
        }
        case 'r':
        {
            // system("cls");
            printf("\n");

            Trains = ReadF(np);

            break;
        }
        case 'M':
        {
            show_menu();
            break;
        }
        case '#':
        {
            exit(1);
        }
        case 'R':
        {

            Trains = load_from_txt(np, Trains);

            break;
        }
        case 'W':
        {

            save_txt(Trains, n);

            break;
        }
        case '6':
        {
            Trains = Add_2(Trains, np);
            break;
        }
        case '7': // as this function should return true or false in the job, then depending on what the function returned, we print out on the screen whether the reading was successful or not
        {
            printf("\n");
            if (strcmp(search_for_item(), "true") == 0)
            {
                printf("\n");
                puts("\nReading was successful\n");
            }
            else
            {
                puts("\nError opening file or entry with entered number does not exist!\n");
            }
            break;
        }
        }
    }
}
