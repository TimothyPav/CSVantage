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


int main(){
    printMainMenu();
}


void printMainMenu(){
    char menu_option;
    printLogo();

    char command;
    bool quit = false;
    while(!quit){
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
            case 'L':
                char load_filename[127];
                printf("*note must end in .csv!\n");
                printf("Enter the CSV you want to load: ");
                scanf(" %s", load_filename);
                if(file_exists(load_filename)) print_load_csv_command_menu(load_filename);
                else printf("No file with name '%s' found\n", load_filename);
                break;

            case 'c':
            case 'C':
                char filename[127];
                printf("*note: must end in .csv!\n");
                printf("Enter a filename: ");
                scanf(" %s", filename);
                create_csv_command(filename);
                break;

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
            
            
        }
    }

}

void print_load_csv_command_menu(char* name){
    TableSchema* table = create_table_schema(name);
    FILE* file_reader = fopen(name, "r");
    read_fields_csv(table, file_reader);

    char command;
    bool quit = false;
    printf("'%s' successfully loaded!\n", name);
    while(!quit){
        printf("\n");
        printf("----csv CRUD menu----\n");
        printf("\n");
        printf("[C]reate\n");
        printf("[R]ead\n");
        printf("[U]pdate\n");
        printf("[D]elete\n");
        printf("[M]ain Menu\n");
        printf("[Q]uit\n");
        printf("\n");
        printf("Enter a command: ");
        scanf(" %c", &command);

        switch(command){
            case 'c':
            case 'C':
                FILE* file_reader = fopen(name, "r");
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
                fclose(file_reader);
                break;

            case 'r':
            case 'R':
                break;

            case 'u':
            case 'U':
                break;

            case 'd':
            case 'D':
                break;

            case 'm':
            case 'M':
                quit = true;
                break;

            case 'q':
            case 'Q':
                printf("quitting...\n");
                exit(EXIT_SUCCESS);
                break;
            default:
                    printf("Command '%c' is not recognized\n", command);
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
        printf("\n");
        printf("----Create new CSV file menu----\n");
        printf("\n");
        printf("[A]dd field\n");
        printf("[M]ain Menu\n");
        printf("\n");
        printf("Enter a command: ");
        scanf(" %c", &command);

        switch(command){
            case 'a':
            case 'A':
                char field[127];
                printf("Enter a field: ");
                scanf( "%s", field);
                add_field(schema, field, "N/A");
                printf("field '%s' successfully added!\n", field);
                printf("CSV will update on [M]ain Menu or [Q]uit\n");
                break;

            case 'm':
            case 'M':
                create_csv(schema, file_pointer);
                quit = true;
                break;
            
            case 'q':
            case 'Q':
                create_csv(schema, file_pointer);
                printf("quitting...\n");
                exit(EXIT_SUCCESS);
                break;
            default:
                printf("Command '%c' is not recognized\n", command);
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
    printf("    Syntax: C <filename>\n");
    printf("    Example: C newfile.csv\n");
    printf("\n");
    printf("[H] Help:\n");
    printf("    Displays this help information.\n");
    printf("\n");
    printf("[Q] Quit:\n");
    printf("    Exits the application. Make sure to save your work.\n");
    printf("\n");
    printf("General Tips:\n");
    printf("    - Commands are case-insensitive.\n");
    printf("    - Ensure file names do not contain spaces or special characters.\n");
    printf("    - If you encounter errors, check the file name and data format.\n");
    printf("\n");
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