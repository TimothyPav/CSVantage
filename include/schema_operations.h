#ifndef SCHEMA_OPERATIONS_H
#define SCHEMA_OPERATIONS_H
#include "../include/db_structs.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// (C)REATE

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

// (R)EAD

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
 * @brief returns true if a value is in the csv file
 * 
 * Parses the entire csv file looking for a loose match in the table. If an entry INCLUDES the input, this function will return true
 * 
 * @param schema The table/csv loaded into memory with struct TableSchema
 * @param file_pointer pointer to write the different values into the file
 * @param input value the function will look for inside the csv
 * 
 * @return bool returns true if value is found
*/
bool is_in_table(TableSchema *schema, FILE *file_pointer, char *input);

/**
 * @brief prints rows that include the input
 * 
 * Parses entire table looking for a loose match in each row. Will print entire row if input is found anywhere in a row. Will print message if nothing is found
 * 
 * @param file_pointer pointer to write the different values into the file
 * @param input value the function will look for inside the csv
 * 
 * @return void
*/
void print_rows_including_input(FILE* file_pointer, char* input);

/**
 * @brief prints entire csv
 * 
 * Prints all the contents of the table include the fields in the console
 * 
 * @param file_pointer pointer to write the different values into the file
 * 
 * @return void
*/
void print_entire_table(FILE* file_pointer);

// (U)PDATE

/**
 * @brief if a match is found in the csv, it will be replaced by the input
 * 
 * Parses table looking for a strict match and if one is found, it will be replaced by the user input
 * 
 * @param schema The table/csv loaded into memory with struct TableSchema
 * @param file_pointer pointer to write the different values into the file
 * @param find_input The value the parser searches for to then replace
 * @param replace_input The value that will replace find_input if a match is found
 * 
 * @return void
*/
void update_find_and_replace(TableSchema* schema, FILE* file_pointer, char* find_input, char* replace_input);

/**
 * @brief replaces 1 entry based off another entry if a match is found
 * 
 * Takes an input and if that input is found, in a strict match, anywhere in the table, the selected user field will be changed to the replace value
 * 
 * @param schema The table/csv loaded into memory with struct TableSchema
 * @param file_pointer pointer to write the different values into the file
 * @param input The value that will be searched for with a strict match
 * @param field The field/column that will be replaced in the row that the input is found in
 * @param replace The value that will take up the row of the match and the column of the field parameter
 * 
 * @return void
*/
void update_record_based_on_another_record(TableSchema* schema, FILE* file_pointer, char* input, char* field, char* replace);

// (D)ELETE

/**
 * @brief deletes the last row of the csv
 * 
 * Deletes a user selected amount of lines starting from the bottom
 * 
 * @param schema The table/csv loaded into memory with struct TableSchema
 * @param file_pointer pointer to write the different values into the file
 * @param num_of_lines The number of lines that will be deleted
 * 
 * @return void
*/
void delete_last_line(TableSchema *schema, FILE *file_pointer, int num_of_lines);

/**
 * @brief Deletes entire row if the input is found
 * 
 * If the input is found in the row with a loosey compare, the entire row will be deleted
 * 
 * @param schema The table/csv loaded into memory with struct TableSchema
 * @param file_pointer pointer to write the different values into the file
 * @param input The value that will be searched for with a loose match
 * 
 * @return void
*/
void delete_row_including_input(TableSchema *schema, FILE *file_pointer, char *input);

/**
 * @brief deletes an entire column based on the user input
 * 
 * Searches for the field that matches the user input and if a match is found, the entire column + the field at row 0 will be deleted
 * 
 * @param schema The table/csv loaded into memory with struct TableSchema
 * @param file_pointer pointer to write the different values into the file
 * @param field The user inputted field that will be searched for in the csv row 0
 * 
 * @return void
*/
void delete_entire_column_based_on_field_input(TableSchema* schema, FILE* file_pointer, char* field);

/**
 * @brief deletes entire csv file
 * 
 * DELETES csv does NOT clear it. The file is lost forever
 * 
 * @param schema The table/csv loaded into memory with struct TableSchema
 * 
 * @return void
*/
void delete_entire_table(TableSchema* schema);

#endif