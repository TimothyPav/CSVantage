#ifndef SCHEMA_OPERATIONS_H
#define SCHEMA_OPERATIONS_H
#include "../include/db_structs.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CREATE

/**
 * @brief Allocates memory for a new or existing csv
 * 
 * Allocates and sets all the default values to a new or existing csv file inside the directory
 * 
 * @param name name of the table you want to create OR import that is already in the directory
 * 
 * @return void

*/
TableSchema *create_table_schema(char *name);

/**
 * @brief Adds a field to a csv
 * 
 * Adds a field to a csv that must already be loaded with a user inputted name
 * 
 * @param schema The table/csv loaded into memory with struct TableSchema
 * @param field_name Name of custom field you want to be added
 * @param field_type *DEPRECATED Type of entries that will be in the field
 * 
 * @return void
*/
void add_field(TableSchema *schema, char *field_name, char *field_type);

/**
 * @brief Create new csv file
 * 
 * Create new csv file with all the different fields and metadata from the TableSchema struct
 * 
 * @param schema The table/csv loaded into memory with struct TableSchema
 * @param file_pointer pointer to write the different values into the file
 * 
 * @return void
*/
void create_csv(TableSchema *schema, FILE *file_pointer);

/**
 * @brief Adds a new row to the csv
 * 
 * Takes an array filled with values that will populate 1 row in the csv
 * 
 * @param schema The table/csv loaded into memory with struct TableSchema
 * @param file_pointer pointer to write the different values into the file
 * 
 * @return void
*/
void create_entry(TableSchema *schema, char *arr[schema->field_count],FILE *file_pointer);

//READ

/**
 * @brief Checks if a file exists 
 * 
 * Checks if a file is loaded and can be used to create a new file with the new name if one is not found
 * 
 * @param filename The name of a file the might exist
 * 
 * @return bool true if file exists/can be found in the directory
*/
bool file_exists(const char *filename);

/**
 * @brief stores the fields of a csv in the TableSchema struct
 * 
 * Used for gettings the fields of a csv in a file that is prexisting and storing those fields in the struct
 * 
 * @param schema The table/csv loaded into memory with struct TableSchema
 * @param file_pointer pointer to write the different values into the file
 * 
 * @return void
*/
void read_fields_csv(TableSchema *schema, FILE *file_pointer);

/**
 * @brief prints 1 column of a field
 * 
 * Takes a field and prints all associated entries with the same column number
 * 
 * @param schema The table/csv loaded into memory with struct TableSchema
 * @param field Field you want to see the column contents of
 * @param file_pointer pointer to write the different values into the file
 * 
 * @return void
*/
void select_column_by_field(TableSchema *schema, char *field, FILE *file_pointer);

/**
 * @brief 
 * 
 * xyz
 * 
 * @param
 * 
 * @return void
*/
bool is_in_table(TableSchema *schema, FILE *file_pointer, char *input);
void print_rows_including_input(FILE* file_pointer, char* input);
void print_entire_table(FILE* file_pointer);

//UPDATE
void update_find_and_replace(TableSchema* schema, FILE* file_pointer, char* find_input, char* replace_input);
void update_record_based_on_another_record(TableSchema* schema, FILE* file_pointer, char* input, char* field, char* replace);

//DELETE
void delete_last_line(TableSchema *schema, FILE *file_pointer, int num_of_lines);
void delete_row_including_input(TableSchema *schema, FILE *file_pointer, char *input);
void delete_entire_column_based_on_field_input(TableSchema* schema, FILE* file_pointer, char* field);


#endif