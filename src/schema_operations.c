#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../include/db_structs.h"

TableSchema *create_table_schema(char *name) {
    TableSchema *schema = malloc(sizeof(TableSchema));
    if (schema == NULL) return NULL;
    schema->table_name = name;
    schema->field_count = 0;
    schema->fields = NULL;
    return schema;
}

void add_field(TableSchema *schema, char *field_name, char *field_type){
    schema->field_count++;
    Field *field = realloc(schema->fields, (schema->field_count*sizeof(Field))); // making room for new field
    if (field == NULL){
        printf("ERROR IN 'add_field'");
        exit(1);
    }
    schema->fields = field;
    int i = schema->field_count - 1; // index of field
    schema->fields[i].name = strdup(field_name); // Copying name to field[i]
    schema->fields[i].type = strdup(field_type); // Copying type to field[i]
    schema->fields[i].field_index = schema->field_count; 

    //printf("name: %s\n", schema->fields[i].name);
}

void create_csv(TableSchema *schema, FILE *file_pointer){
    for(int i=0; i<schema->field_count; i++){
        // append ',' to the end of the name if not the last field in schema
        if(i == schema->field_count-1) fprintf(file_pointer, "%s\n", schema->fields[i].name);
        else fprintf(file_pointer, "%s,", schema->fields[i].name);
    }
}

void create_entry(TableSchema *schema, char *arr[schema->field_count], FILE *file_pointer){
    for(int i=0; i<schema->field_count; i++){
        if(i == schema->field_count-1) fprintf(file_pointer, "%s\n", arr[i]);
        else fprintf(file_pointer, "%s,", arr[i]);
    }
}

void appendChar(char* str, char c, size_t max_len) {
    size_t len = strlen(str);
    if (len + 1 < max_len) { // Check if there's room for one more character and the null terminator
        str[len] = c;       // Place the character at the end of the current string
        str[len + 1] = '\0';// Null-terminate the string
    } else {
        // Handle the error in case there's no room
        fprintf(stderr, "Not enough space to append character.\n");
    }
}

bool file_exists(const char *filename){
    FILE *fp = fopen(filename, "r");
    bool is_exist = false;
    if (fp != NULL)
    {
        is_exist = true;
        fclose(fp); // close the file
    }
    return is_exist;
}

void read_fields_csv(TableSchema *schema, FILE *file_pointer){
    char ch;
    char s[256] = {0};
    int i=0;
    int field_num = 1;
    do {
        ch = fgetc(file_pointer);
        if(ch == ','){
            s[i] = '\0';
            add_field(schema, s, "placeholder");
            field_num++;
            i = 0;
            s[0] = '\0';
        }
        else if(ch == '\n'){
            s[i] = '\0';
            add_field(schema, s, "placeholder");
            printf("last field: %s\n", s);
            field_num++;
            i = 0;
            s[0] = '\0';
            break;
        }
        else {
            s[i] = ch;
            i++;
        }
    } while (ch != EOF);

    rewind(file_pointer); // rewind file pointer to the top of the file
}

bool lossey_str_cmp(char *input, char *existing_field){
    int i = 0;
    int j = 0;
    while(existing_field[i] != '\0'){
        if(existing_field[i] == input[j]){
            j++;
            if(input[j] == '\0') return true;
        }
        else {
            j = 0;
        }
        i++;
    }
    return false;
}

void select_column_by_field(TableSchema *schema, char *field, FILE *file_pointer){
    read_fields_csv(schema, file_pointer);

    int index = -1;
    for(int i=0; i<schema->field_count; i++){
        if(lossey_str_cmp(field, schema->fields[i].name)) index = schema->fields[i].field_index-1;
    } if (index == -1){
        printf("No field matching '%s' found\n", field);
        exit(EXIT_FAILURE);
    }

    char ch;
    int column = 0, row = 0, quotes = 0;
    do{
        ch = fgetc(file_pointer);
        if (ch == '\n'){
            column = 0;
            row++;
            printf("\n");
            continue;
        }
        else if(ch == ',' && quotes % 2 == 0){
            column++;
            continue;
        }
        else if(ch == '"'){
            quotes++;
        }

        if(column == index && row > 0){
            printf("%c", ch);
        }

    } while (ch != EOF);
}

int main() {
    char name_of_table[100] = "hurricanes";
    char *file_type = ".csv";
    strcat(name_of_table, file_type);
    TableSchema *table = create_table_schema(name_of_table);

    if(file_exists(table->table_name)) {
        //printf("File with name '%s' already exists.\n", table->table_name);
        //exit(EXIT_FAILURE);

        FILE *file_parser = fopen(table->table_name, "r");

        //add_field(table, "field7", "int");

        select_column_by_field(table, "2015", file_parser);
        //read_fields_csv(table, file_parser);
        for(int i=0; i<table->field_count; i++){
            printf("name: %s index: %d\n", table->fields[i].name, table->fields[i].field_index);
        }

        fclose(file_parser);

    } else {
        // create file and add fields
        FILE *file_pointer = fopen(table->table_name, "w+");
        add_field(table, "field1", "int");
        add_field(table, "field2", "char");
        add_field(table, "field3", "int");
        add_field(table, "field4", "char");
        add_field(table, "field5", "int");
        add_field(table, "field6", "char");

        
        create_csv(table, file_pointer);

        char *entry1[6] = { "1", "a", "2", "b", "3", "c"};
        create_entry(table, entry1, file_pointer);
        char *entry2[6] = { "d", "4", "e", "5", "f", "6"};
        create_entry(table, entry2, file_pointer);

        for(int i=0; i<table->field_count; i++){
            //printf("name: %s index: %d\n", table->fields[i].name, table->fields[i].field_index);
        }

        fclose(file_pointer);
    }   
}