#include "../include/schema_operations.h"
#include "../include/db_structs.h"
#include <stdio.h>
#include <string.h>

void printLogo();
void printMainMenu();
void printHelp();
void create_csv_command(char* name);
void print_creat_csv_command_menu(TableSchema* schema, FILE* file_pointer);
void print_load_csv_command_menu(char* name);
void print_read_menu(TableSchema* schema, FILE* file_reader);
void print_update_menu(TableSchema* schema, FILE* file_reader);
void print_delete_menu(TableSchema* schema, FILE* file_reader);
void clear_screen();
void pressAnyKeyToContinue();
void free_table_and_close_file(TableSchema* schema, FILE* file_pointer);


int main(){
    printMainMenu();
}


void printMainMenu(){
    char menu_option;

    char command;
    bool quit = false;
    while(!quit){
        clear_screen();
        printf("\n");
        printf("-------Main Menu-------\n");
        printf("\n");
        printf("[L]oad existing CSV file\n");
        printf("[C]reate new CSV file\n");
        printf("[H]elp\n");
        printf("[Q]uit\n");
        printf("\n");
        printf("Enter Command: ");
        scanf(" %c", &command);

        switch(command){
            case 'l':
            case 'L': {
                char load_filename[127];
                printf("*note must end in .csv!\n");
                printf("Enter the CSV you want to load: ");
                scanf(" %s", load_filename);
                if(file_exists(load_filename)) print_load_csv_command_menu(load_filename);
                else {
                    printf("No file with name '%s' found\n", load_filename);
                    pressAnyKeyToContinue();
                }
                break;
            }

            case 'c':
            case 'C':{
                char filename[127];
                printf("*note: must end in .csv!\n");
                printf("Enter a filename: ");
                scanf(" %s", filename);
                create_csv_command(filename);
                pressAnyKeyToContinue();
                break;
            }

            case 'h':
            case 'H':
                printHelp();
                break;

            case 'q':
            case 'Q':
                printf("quitting...\n");
                quit = true;
                break;

            default:
                printf("Command '%c' is not recognized\n", command);
                pressAnyKeyToContinue();
                break;
        }
    }

}

void print_load_csv_command_menu(char* name){
    TableSchema* table = create_table_schema(name);
    FILE* file_reader = fopen(name, "r");
    read_fields_csv(table, file_reader);

    char command;
    bool quit = false;
    while(!quit){
        clear_screen();
        printf("\n");
        printf("----csv CRUD menu----\n");
        printf("\n");
        printf("'%s' successfully loaded!\n", name);
        printf("[C]reate\n");
        printf("[R]ead\n");
        printf("[U]pdate\n");
        printf("[D]elete\n");
        printf("[M]ain Menu\n");
        printf("[Q]uit\n");
        printf("\n");
        FILE* file_reader = fopen(name, "r");
        printf("Enter a command: ");
        scanf(" %c", &command);

        switch(command){
            case 'c':
            case 'C':{
                char create;
                printf("Create new row. press y to continue: ");
                scanf(" %c", &create);
                if(create == 'y' || create == 'Y'){
                    int field_count_var = table->field_count;
                    char* pointers_to_fields_names[field_count_var];
                    for(int i=0; i<field_count_var; i++){ pointers_to_fields_names[i] = NULL; }


                    printf("Enter a value or record for each corresponding field!\n");
                    for(int i=0; i<field_count_var; i++){
                        char input[127] = {0};
                        printf("%s: ", table->fields[i].name);
                        scanf(" %[^\n]",input);
                        pointers_to_fields_names[i] = malloc(strlen(input) + 1);
                        strcpy(pointers_to_fields_names[i], input);
                    }
                    create_entry(table, pointers_to_fields_names, file_reader);
                    for(int i=0; i<field_count_var; i++) { free(pointers_to_fields_names[i]); }
                }
                printf("Records successfully inserted!\n");
                printf("\n");
                break;
            }

            case 'r':
            case 'R':
                print_read_menu(table, file_reader);
                break;

            case 'u':
            case 'U':
                print_update_menu(table, file_reader);
                break;

            case 'd':
            case 'D':
                print_delete_menu(table, file_reader);
                break;

            case 'm':
            case 'M':
                quit = true;
                break;

            case 'q':
            case 'Q':
                printf("quitting...\n");
                free_table_and_close_file(table, file_reader);
                exit(EXIT_SUCCESS);
                break;
            default:
                printf("Command '%c' is not recognized\n", command);
                pressAnyKeyToContinue();
                break;
        }
    }
}
//RUD
void print_read_menu(TableSchema* schema, FILE* file_reader){
    char command;
    bool quit = false;
    while(!quit){
        clear_screen();
        printf("\n");
        printf("----csv READ menu----\n");
        printf("\n");
        printf("[S]elect columns by a field\n");
        printf("[C]heck if a value is in the csv file\n");
        printf("[T]arget rows including an input\n");
        printf("[P]rint entire table\n");
        printf("[B]ack\n");
        printf("[Q]uit\n");
        printf("\n");
        char input[127] = {0};
        printf("Enter a command: ");
        scanf(" %c", &command);

        switch(command){
            case 's':
            case 'S':
                input[127];
                printf("Enter a field: ");
                scanf(" %[^\n]",input);
                select_column_by_field(schema, input, file_reader);
                rewind(file_reader);
                pressAnyKeyToContinue();
                break;
            
            case 'c':
            case 'C':
                input[127];
                printf("Enter a value to check: ");
                scanf(" %[^\n]",input);
                is_in_table(schema, file_reader, input);
                rewind(file_reader);
                pressAnyKeyToContinue();
                break;

            case 't':
            case 'T':
                input[127];
                printf("Enter a value to check: ");
                scanf(" %[^\n]",input);
                print_rows_including_input(file_reader, input);
                rewind(file_reader);
                pressAnyKeyToContinue();
                break;

            case 'p':
            case 'P':
                print_entire_table(file_reader);
                rewind(file_reader);
                pressAnyKeyToContinue();
                break;

            case 'b':
            case 'B':
                quit = true;
                break;

            case 'q':
            case 'Q':
                printf("quitting...\n");
                free_table_and_close_file(schema, file_reader);
                exit(EXIT_SUCCESS);
                break;

            default:
                printf("Command '%c' is not recognized\n", command);
                pressAnyKeyToContinue();
                break;
        }

    }
}
void print_update_menu(TableSchema* schema, FILE* file_reader){
    char command;
    bool quit = false;
    while(!quit){
        clear_screen();
        printf("\n");
        printf("----csv UPDATE menu----\n");
        printf("\n");
        printf("[F]ind and Replace\n");
        printf("[U]pdate record in specific column based on another record in that row\n");
        printf("[B]ack\n");
        printf("[Q]uit\n");
        printf("\n");
        char input[127] = {0};
        char replace[127] = {0};
        printf("Enter a command: ");
        scanf(" %c", &command);

        switch(command){
            case 'f':
            case 'F':
                input[127];
                replace[127];
                printf("Enter a value to find: ");
                scanf(" %[^\n]",input);
                printf("Enter a value to replace: ");
                scanf(" %[^\n]",replace);
                update_find_and_replace(schema, file_reader, input, replace);
                rewind(file_reader);
                pressAnyKeyToContinue();
                break;

            case 'u':
            case 'U':
                input[127];
                replace[127];
                printf("Enter a value to find: ");
                scanf(" %[^\n]",input);
                printf("Enter a value to replace: ");
                scanf(" %[^\n]",replace);
                char field[127] = {0};
                printf("Enter the field you want to insert your replacement: ");
                scanf(" %[^\n]",field);
                update_record_based_on_another_record(schema, file_reader, input, field, replace);
                rewind(file_reader);
                pressAnyKeyToContinue();
                break;

            case 'b':
            case 'B':
                quit = true;
                break;

            case 'q':
            case 'Q':
                printf("quitting...\n");
                free_table_and_close_file(schema, file_reader);
                exit(EXIT_SUCCESS);
                break; 

            default:
                printf("Command '%c' is not recognized\n", command);
                pressAnyKeyToContinue();
                break;
        }
    }
}
void print_delete_menu(TableSchema* schema, FILE* file_reader){
    char command;
    bool quit = false;
    while(!quit){
        clear_screen();
        printf("\n");
        printf("----csv DELETE menu----\n");
        printf("\n");
        printf("Delete [L]ast lines\n");
        printf("Delete [R]ow including an input\n");
        printf("Delete [C]olumn based on field input\n");
        printf("Delete [F]ile\n");
        printf("[B]ack\n");
        printf("[Q]uit\n");
        printf("\n");
        char input[127] = {0};
        printf("Enter a command: ");
        scanf(" %c", &command);

        switch(command){
            case 'l':
            case 'L':                pressAnyKeyToContinue();
                int input_num = 0;
                printf("Enter number of lines to delete: ");
                scanf(" %d",&input_num);
                delete_last_line(schema, file_reader, input_num);
                rewind(file_reader);
                pressAnyKeyToContinue();
                break;

            case 'r':
            case 'R':
                input[127];
                printf("Enter a value to delete the row the value is in: ");
                scanf(" %[^\n]",input);
                delete_row_including_input(schema, file_reader, input);
                rewind(file_reader);
                pressAnyKeyToContinue();
                break;

            case 'c':
            case 'C':{
                input[127];
                printf("Enter a field to delete with all its contents: ");
                scanf(" %[^\n]",input);
                delete_entire_column_based_on_field_input(schema, file_reader, input);
                rewind(file_reader);
                pressAnyKeyToContinue();
                break;
            }

            case 'f':
            case 'F':{
                char sure;
                printf("Are you sure? press y to continue...  ");
                scanf(" %c",&sure);
                if(sure == 'y' || sure == 'Y') delete_entire_table(schema);
                pressAnyKeyToContinue();
                break;
            }
            
            case 'b':
            case 'B':
                quit = true;
                break;

            case 'q':
            case 'Q':
                printf("quitting...\n");
                free_table_and_close_file(schema, file_reader);
                exit(EXIT_SUCCESS);
                break;
            
            default:
                printf("Command '%c' is not recognized\n", command);
                pressAnyKeyToContinue();
                break;
        }
    }
}
void create_csv_command(char* name){
    int i = strlen(name);
    if(!(name[i-4] == '.' && name[i-3] == 'c' && name[i-2] == 's' && name[i-1] == 'v')){
        printf("Invalid file tpye. Must end with '.csv'\n");
        return;
    }
    FILE* file_pointer = fopen(name, "w+");
    TableSchema* table = create_table_schema(name);
    print_creat_csv_command_menu(table, file_pointer);
    fclose(file_pointer);


}
void print_creat_csv_command_menu(TableSchema* schema, FILE* file_pointer){
    char command;
    bool quit = false;
    while(!quit){
        clear_screen();
        printf("\n");
        printf("----Create new CSV file menu----\n");
        printf("\n");
        printf("[A]dd field\n");
        printf("[M]ain Menu\n");
        printf("[Q]uit\n");
        printf("\n");
        printf("Enter a command: ");
        scanf(" %c", &command);

        switch(command){
            case 'a':
            case 'A':{
                char field[127];
                printf("Enter a field: ");
                scanf( "%s", field);
                add_field(schema, field, "N/A");
                printf("field '%s' successfully added!\n", field);
                printf("CSV will update on [M]ain Menu or [Q]uit\n");
                pressAnyKeyToContinue();
                break;
            }

            case 'm':
            case 'M':
                create_csv(schema, file_pointer);
                quit = true;
                break;
            
            case 'q':
            case 'Q':
                create_csv(schema, file_pointer);
                printf("quitting...\n");
                freeTableSchema(schema);
                exit(EXIT_SUCCESS);
                break;

            default:
                printf("Command '%c' is not recognized\n", command);
                pressAnyKeyToContinue();
                break;
        }

    }
}
void printHelp(){
    printf("-------Help Menu-------\n");
    printf("\n");
    printf("[L] Load existing CSV file:\n");
    printf("    Use this command to load and work with an existing CSV file.\n");
    printf("    Syntax: L <filename>\n");
    printf("    Example: L data.csv\n");
    printf("\n");
    printf("[C] Create new CSV file:\n");
    printf("    Allows you to create a new CSV file. You will be prompted to enter data.\n");
    printf("    Syntax: <filename>\n");
    printf("    Example: newfile.csv\n");
    printf("\n");
    printf("[H] Help:\n");
    printf("    Displays this help information.\n");
    printf("\n");
    printf("[Q] Quit:\n");
    printf("    Exits the application.\n");
    printf("\n");
    printf("General Tips:\n");
    printf("    - Commands are case-insensitive.\n");
    printf("    - Ensure file names do not contain spaces or special characters.\n");
    printf("    - If you encounter errors, check the file name and data format.\n");
    printf("\n");
    pressAnyKeyToContinue();
}
void printLogo(){
    printf("\n");
    printf("  ______    ______   __     __                       __                                   \n");
    printf(" /      \\  /      \\ /  |   /  |                     /  |                                  \n");
    printf("/$$$$$$  |/$$$$$$  |$$ |   $$ | ______   _______   _$$ |_     ______    ______    ______  \n");
    printf("$$ |  $$/ $$ \\__$$/ $$ |   $$ |/      \\ /       \\ / $$   |   /      \\  /      \\  /      \\ \n");
    printf("$$ |      $$      \\ $$  \\ /$$/ $$$$$$  |$$$$$$$  |$$$$$$/    $$$$$$  |/$$$$$$  |/$$$$$$  |\n");
    printf("$$ |   __  $$$$$$  | $$  /$$/  /    $$ |$$ |  $$ |  $$ | __  /    $$ |$$ |  $$ |$$    $$ |\n");
    printf("$$ \\__/  |/  \\__$$ |  $$ $$/  /$$$$$$$ |$$ |  $$ |  $$ |/  |/$$$$$$$ |$$ \\__$$ |$$$$$$$$/ \n");
    printf("$$    $$/ $$    $$/    $$$/   $$    $$ |$$ |  $$ |  $$  $$/ $$    $$ |$$    $$ |$$       |\n");
    printf(" $$$$$$/   $$$$$$/      $/     $$$$$$$/ $$/   $$/    $$$$/   $$$$$$$/  $$$$$$$ | $$$$$$$/ \n");
    printf("                                                                      /  \\__$$ |          \n");
    printf("                                                                      $$    $$/           \n");
    printf("                                                                       $$$$$$/            \n");
}
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    printLogo();
}
void free_table_and_close_file(TableSchema* schema, FILE* file_pointer){
    freeTableSchema(schema);
    fclose(file_pointer);
}

#ifdef _WIN32
#include <conio.h> // For Windows getch()
void pressAnyKeyToContinue() {
    printf("Press any key to continue...\n");
    getch(); // Use getch() on Windows
}
#else
#include <termios.h>
#include <unistd.h> // For Unix-like systems read()
void pressAnyKeyToContinue() {
    struct termios old_attr, new_attr;
    int ch;
    printf("Press any key to continue...\n");
    tcgetattr(STDIN_FILENO, &old_attr);
    new_attr = old_attr;
    new_attr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);
    read(STDIN_FILENO, &ch, 1);
    tcsetattr(STDIN_FILENO, TCSANOW, &old_attr);
}
#endif