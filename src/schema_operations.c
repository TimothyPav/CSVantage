#include "../include/schema_operations.h"
#include "../include/db_structs.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 65536

//CREATE
TableSchema *create_table_schema(char *name);
void add_field(TableSchema *schema, char *field_name, char *field_type);
void create_csv(TableSchema *schema, FILE *file_pointer);
void create_entry(TableSchema *schema, char *arr[schema->field_count],FILE *file_pointer);

//READ
void read_fields_csv(TableSchema *schema, FILE *file_pointer);
void select_column_by_field(TableSchema *schema, char *field, FILE *file_pointer);
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
void delete_entire_table(TableSchema* schema);


TableSchema *create_table_schema(char *name)
{
  TableSchema *schema = malloc(sizeof(TableSchema));
  if (schema == NULL)
    return NULL;
  schema->table_name = name;
  schema->field_count = 0;
  schema->fields = NULL;
  return schema;
}

void add_field(TableSchema *schema, char *field_name, char *field_type)
{
  schema->field_count++;
  Field *field =
      realloc(schema->fields, (schema->field_count *
                               sizeof(Field))); // making room for new field
  if (field == NULL)
  {
    printf("ERROR IN 'add_field'");
    exit(1);
  }
  schema->fields = field;
  int i = schema->field_count - 1;             // index of field
  schema->fields[i].name = strdup(field_name); // Copying name to field[i]
  schema->fields[i].type = strdup(field_type); // Copying type to field[i]
  schema->fields[i].field_index = schema->field_count;

  // printf("name: %s  field_count: %d\n", schema->fields[i].name,
  // schema->field_count);
}

void create_csv(TableSchema *schema, FILE *file_pointer)
{
  for (int i = 0; i < schema->field_count; i++)
  {
    // append ',' to the end of the name if not the last field in schema
    if (i == schema->field_count - 1)
      fprintf(file_pointer, "%s\n", schema->fields[i].name);
    else
      fprintf(file_pointer, "%s,", schema->fields[i].name);
  }
}

void create_entry(TableSchema *schema, char *arr[schema->field_count], FILE *file_pointer)
{
  FILE *temp_file_pointer = fopen("temp.csv", "w+");
  rewind(file_pointer);
  char ch;
  while(1){
    ch = fgetc(file_pointer);
    if(ch == EOF){
      break;
    }
    fputc(ch, temp_file_pointer);
  }
  fputc('\n', temp_file_pointer);
  for (int i = 0; i < schema->field_count; i++)
  {
    if (i == schema->field_count - 1){
      fputs(arr[i], temp_file_pointer);
      //fputc('\n', temp_file_pointer);
    }
    else{
      fputs(arr[i], temp_file_pointer);
      fputc(',', temp_file_pointer);
    }
  }

  char *name_of_table = schema->table_name;
  int status = remove(name_of_table);
  if (status == 0)
  {
    //printf("File deleted successfully.\n");
  }
  else
  {
    perror("Failed to delete the file");
    exit(EXIT_FAILURE);
  }
  rename("temp.csv", name_of_table);
  fclose(temp_file_pointer);
  rewind(file_pointer);
}

void append_char_to_string(char **str, char c)
{
  // Determine the new length
  int len = *str ? strlen(*str) : 0;

  // Allocate memory for the new string (old length + char + '\0')
  char *new_str = malloc(len + 2);
  if (new_str == NULL)
  {
    perror("Failed to allocate memory");
    exit(EXIT_FAILURE);
  }

  // Copy the old string to the new string
  if (*str)
  {
    strcpy(new_str, *str);
  }

  // Append the new character and null terminator
  new_str[len] = c;
  new_str[len + 1] = '\0';

  // Free the old string if it was dynamically allocated
  free(*str);

  // Update the original string pointer
  *str = new_str;
}

bool file_exists(const char *filename)
{
  FILE *fp = fopen(filename, "r");
  bool is_exist = false;
  if (fp != NULL)
  {
    is_exist = true;
    fclose(fp); // close the file
  }
  return is_exist;
}

void read_fields_csv(TableSchema *schema, FILE *file_pointer)
{
  char ch;
  char s[256] = {0};
  int i = 0;
  int field_num = 1;
  rewind(file_pointer);
  do
  {
    ch = fgetc(file_pointer);
    if (ch == ',')
    {
      s[i] = '\0';
      add_field(schema, s, "N/A");
      field_num++;
      i = 0;
      s[0] = '\0';
    }
    else if (ch == '\n')
    {
      s[i-1] = '\0';
      add_field(schema, s, "N/A");
      field_num++;
      i = 0;
      s[0] = '\0';
      break;
    }
    else
    {
      s[i] = ch;
      i++;
    }
  } while (ch != EOF);

  rewind(file_pointer); // rewind file pointer to the top of the file
}

bool lossey_str_cmp(char *input, char *existing_field)
{
  int i = 0;
  int j = 0;
  while (existing_field[i] != '\0')
  {
    if (existing_field[i] == input[j])
    {
      j++;
      if (input[j] == '\0')
        return true;
    }
    else
    {
      j = 0;
    }
    i++;
  }
  return false;
}

void select_column_by_field(TableSchema *schema, char *field, FILE *file_pointer)
{
  if (schema == NULL || field == NULL || file_pointer == NULL)
  {
    fprintf(stderr, "Invalid input to function\n");
    exit(EXIT_FAILURE); // Optionally handle the error more gracefully or exit if critical
  }

  //read_fields_csv(schema, file_pointer);

  int index = -1;
  for (int i = 0; i < schema->field_count; i++)
  {
    // printf("Attempting to match field '%s' with '%s'\n", field,
    // schema->fields[i].name);
    if (lossey_str_cmp(field, schema->fields[i].name))
    {
      index = schema->fields[i].field_index - 1;
      break;
    }
  }
  if (index == -1)
  {
    printf("No field matching '%s' found\n", field);
    exit(EXIT_FAILURE);
  }

  char s[255] = {0};
  char ch;
  int column = 0, row = 0, quotes = 0, i = 0;
  do
  {
    ch = fgetc(file_pointer);
    if (ch == '\n')
    {
      column = 0;
      row++;

      if (row > 1)
      {
        s[i] = '\0';
        printf("\n");
        i = 0;
        s[0] = '\0';
      }
      continue;
    }
    else if (ch == ',' && quotes % 2 == 0)
    {
      column++;
      continue;
    }
    else if (ch == '"')
    {
      quotes++;
    }

    if (column == index && row > 0)
    {
      printf("%c", ch);
      s[i++] = ch;
    }

  } while (ch != EOF);
  rewind(file_pointer);
}

int get_rows(FILE *file_pointer){
    char buf[BUF_SIZE];
    int counter = 0;
    for(;;)
    {
        size_t res = fread(buf, 1, BUF_SIZE, file_pointer);
        if (ferror(file_pointer))
            return -1;

        int i;
        for(i = 0; i < res; i++)
            if (buf[i] == '\n')
                counter++;

        if (feof(file_pointer))
            break;
    }
    rewind(file_pointer);
    return counter;
}

void freeTableSchema(TableSchema *schema)
{
  if (schema == NULL)
    return;

  // Free each Field struct within the fields array
  for (int i = 0; i < schema->field_count; i++)
  {
    free(schema->fields[i].name); // Free the name string of the Field
    free(schema->fields[i].type); // Free the type string of the Field
  }

  free(schema->fields);
  free(schema);
}

bool is_in_table(TableSchema *schema, FILE *file_pointer, char *input)
{
  char ch;
  int column = 0, row = 0, quotes = 0, i = 0;
  char s[255] = {0};
  bool found_something = false;
  while (ch != EOF)
  {
    // printf("Hello\n");
    ch = fgetc(file_pointer);
    if (ch == '\n')
    {
      s[i] = '\0';
      if (lossey_str_cmp(input, s))
      {
        printf("Match found at [row][column]: [%d][%d]\n", row + 1, column + 1);
        found_something = true;
      }
      column = 0;
      row++;
      i = 0;
      s[0] = '\0';

      continue;
    }
    else if (ch == ',' && quotes % 2 == 0)
    {
      s[i] = '\0';
      if (lossey_str_cmp(input, s))
      {
        printf("Match found at [row][column]: [%d][%d]\n", row + 1, column + 1);
        found_something = true;
      }
      column++;
      i = 0;
      s[0] = '\0';
      continue;
    }
    else if (ch == '"')
    {
      quotes++;
    }
    else
    {
      s[i++] = ch;
    }
  }
  if(!found_something){
    printf("No record matching '%s' found\n", input);
    return false;
  } else {
    return true;
  }
}

void delete_last_line(TableSchema *schema, FILE *file_pointer, int num_of_lines)
{
  FILE *temp_file_pointer = fopen("temp.csv", "w+");
  char ch = '?';
  rewind(file_pointer);
  int rows = get_rows(file_pointer);
  int row_num = 0;
  int to_delete_rows = 0;
  while (1)
  {
    ch = fgetc(file_pointer);
    if (ch == EOF)
      break;
    if ((rows - num_of_lines) > row_num)
    {
      fputc(ch, temp_file_pointer);
    }
    if (ch == '\n')
      row_num++;
  }
  char *name_of_table = schema->table_name;
  int status = remove(name_of_table);
  if (status == 0)
  {
    //printf("File deleted successfully.\n");
  }
  else
  {
    perror("Failed to delete the file");
    exit(EXIT_FAILURE);
  }
  printf("Successfully deleted %d lines\n", num_of_lines);
  rename("temp.csv", name_of_table);
  fclose(temp_file_pointer);
  rewind(file_pointer);
}

void delete_row_including_input(TableSchema *schema, FILE *file_pointer, char *input)
{
  FILE *temp_file_pointer = fopen("temp.csv", "w+");
  char* line = NULL;
  char ch = '?';
  rewind(file_pointer);
  int row = 0;
  bool something_was_deleted = false;

  while(1){
    ch = fgetc(file_pointer);
    if(ch == EOF) break;
    append_char_to_string(&line, ch);
    if(ch == '\n'){
      row++;
      if(!lossey_str_cmp(input, line)) fputs(line, temp_file_pointer);
      else{
        printf("row:%d deleted\n", row);
        something_was_deleted = true;
      }
      line = NULL;
    }

  }
  if(!something_was_deleted) printf("No text matching '%s' was found\n", input);

  char *name_of_table = schema->table_name;
  int status = remove(name_of_table);
  if (!(status == 0))
  {
    perror("Failed to delete the file");
    exit(EXIT_FAILURE);
  }
  rename("temp.csv", name_of_table);
  fclose(temp_file_pointer);
  rewind(file_pointer);
  free(line);
  line = NULL;
}

void print_rows_including_input(FILE* file_pointer, char* input){
  char* line = NULL;
  char ch = '?';
  rewind(file_pointer);
  int row = 0;
  bool found = false;

  while(1){
    ch = fgetc(file_pointer);
    if(ch == EOF) break;
    append_char_to_string(&line, ch);
    if(ch == '\n'){
      row++;
      if(lossey_str_cmp(input, line)){
        printf("%s", line);
        found = true;
      }
      line = NULL;
    }
  }
  if(!found) printf("No entry containing '%s' was found\n", input);
  rewind(file_pointer);
  free(line);
  line = NULL;
}

void print_entire_table(FILE* file_pointer){
  rewind(file_pointer);
  char ch = '?';
  while(1){
    ch = fgetc(file_pointer);
    if(ch == EOF) break;
    printf("%c", ch);
  }
  rewind(file_pointer);
  printf("\n");
}

void delete_entire_column_based_on_field_input(TableSchema* schema, FILE* file_pointer, char* field){
  int index = -1;
  for (int i = 0; i < schema->field_count; i++)
  {
    // printf("Attempting to match field '%s' with '%s'\n", field,
    // schema->fields[i].name);
    if (lossey_str_cmp(field, schema->fields[i].name))
    {
      index = schema->fields[i].field_index - 1;
      break;
    }
  }
  if (index == -1)
  {
    printf("No field matching '%s' found\n", field);
    exit(EXIT_FAILURE);
  }
  FILE* temp_file_pointer = fopen("temp.csv", "w");
  int column = 0, row = 0, quotes = 0;
  rewind(file_pointer);
  char ch = '?';
  while(1){
    ch = fgetc(file_pointer);
    if(ch == EOF) break;
    
    if (ch == ',' && quotes % 2 == 0)
    {
      column++;
      //continue;
    }
    else if (ch == '"')
    {
      quotes++;
    }

    if (!(column == index && ch != '\n') && !(index == 0 && column-1 == 0 && ch == ','))
    {
      //printf("%c", ch);
      fputc(ch, temp_file_pointer);
    } else{
      //printf("%c", ch);
    }
    
    if (ch == '\n')
    {
      column = 0;
      row++;
      //continue;
    }
  }
  printf("\n");

  char *name_of_table = schema->table_name;
  int status = remove(name_of_table);
  if (!(status == 0))
  {
    perror("Failed to delete the file");
    exit(EXIT_FAILURE);
  } else {
    //printf("Deleted file successfully\n");
  }
  rename("temp.csv", name_of_table);
  fclose(temp_file_pointer);
  rewind(file_pointer);

}

void clearString(char *str) {
    if (str != NULL) {
        memset(str, 0, strlen(str));
    }
}

void update_find_and_replace(TableSchema* schema, FILE* file_pointer, char* find_input, char* replace_input){
  char* s = NULL;
  char ch = '?';
  int column = 0, row = 0, quotes = 0, number_of_replacements = 0;
  int length_of_replace_input = strlen(find_input);
  FILE* temp_file_pointer = fopen("temp.csv", "w");
  rewind(file_pointer);

  clearString(s);
  while(1)
  {
    ch = fgetc(file_pointer);
    if(ch == EOF) break;
    if (ch == '\n')
    {
      column = 0;
      if((row > 0) && (length_of_replace_input == strlen(s)) && (lossey_str_cmp(s, find_input))){
        printf("Found match @ [row][column]: [%d][%d]. Update Successful!\n", row, column);
        number_of_replacements++;
        fputs(replace_input, temp_file_pointer);
      } else {
        fputs(s, temp_file_pointer);
      }
      row++;
      clearString(s);
      fputc(ch, temp_file_pointer);
    }
    else if (ch == ',' && quotes % 2 == 0)
    {
      column++;
      if((row > 0) && (length_of_replace_input == strlen(s)) && (lossey_str_cmp(s, find_input))){
        printf("Found match @ [row][column]: [%d][%d]. Update Successful!\n", row, column);
        number_of_replacements++;
        fputs(replace_input, temp_file_pointer);
      } else {
        fputs(s, temp_file_pointer);
      }
      clearString(s);
      fputc(ch, temp_file_pointer);
    }
    else if (ch == '"')
    {
      quotes++;
      append_char_to_string(&s, ch);
    }
    else{
      append_char_to_string(&s, ch);
    }

  }
  printf("Number of replacements: %d\n", number_of_replacements);
  char *name_of_table = schema->table_name;
  int status = remove(name_of_table);
  if (!(status == 0))
  {
    perror("Failed to delete the file");
    exit(EXIT_FAILURE);
  } else {
    //printf("Deleted file successfully\n");
  }
  rename("temp.csv", name_of_table);
  fclose(temp_file_pointer);
  rewind(file_pointer);
  free(s);
  s = NULL;
}

void update_record_based_on_another_record(TableSchema* schema, FILE* file_pointer, char* input, char* field, char* replace){
  int index = -1;
  for (int i = 0; i < schema->field_count; i++)
  {
    printf("Attempting to match '%s' with '%s'\n", field, schema->fields[i].name);
    if (lossey_str_cmp(field, schema->fields[i].name))
    {
      index = schema->fields[i].field_index - 1;
      break;
    }
  }
  if (index == -1)
  {
    printf("No field matching '%s' found\n", field);
    exit(EXIT_FAILURE);
  }
  char* no_match_s = NULL;
  char* match_s = NULL;
  char* current_record = NULL;
  char ch = '?';
  int column = 0, row = 0, quotes = 0, number_of_replacements = 0;
  int length_of_replace_input = strlen(input);
  bool insterted = false;
  bool match_found = false;
  FILE* temp_file_pointer = fopen("temp.csv", "w");
  rewind(file_pointer);

  while(1)
  {
    ch = fgetc(file_pointer);
    if (ch == '\n' || ch == EOF)
    {
      column = 0;
      row++;

      if((row > 0) && (length_of_replace_input == strlen(current_record)) && (lossey_str_cmp(current_record, input))){
        match_found = true;
      }

      if (ch == '\n') append_char_to_string(&no_match_s, ch);
      if (ch == '\n') append_char_to_string(&match_s, ch);

      (match_found) ? fputs(match_s, temp_file_pointer) : fputs(no_match_s, temp_file_pointer);
      if(match_found) number_of_replacements++;
      match_found = false;
      insterted = false;
      clearString(match_s);
      clearString(no_match_s);
      clearString(current_record);
      if(ch == EOF) break;
    }
    else if (ch == ',' && quotes % 2 == 0)
    {
      column++;
      if((row > 0) && (length_of_replace_input == strlen(current_record)) && (lossey_str_cmp(current_record, input))){
        match_found = true;
      }
      insterted = false;
      append_char_to_string(&no_match_s, ch);
      append_char_to_string(&match_s, ch);
      clearString(current_record);
    }
    else if (ch == '"')
    {
      quotes++;
      append_char_to_string(&current_record, ch);
      append_char_to_string(&no_match_s, ch);

      if(column == index && !insterted){
        for(int i=0; i<strlen(replace); i++){
          append_char_to_string(&match_s, replace[i]);
        }
        insterted = true;
      }
      else if(column != index){
        append_char_to_string(&match_s, ch);
      }
    }
    else{
      append_char_to_string(&current_record, ch);
      append_char_to_string(&no_match_s, ch);

      if(column == index && !insterted){
        for(int i=0; i<strlen(replace); i++){
          append_char_to_string(&match_s, replace[i]);
        }
        insterted = true;
      }
      else if(column != index){
        append_char_to_string(&match_s, ch);
      }
    }

  }
  printf("Number of replacements: %d\n", number_of_replacements);
  char *name_of_table = schema->table_name;
  int status = remove(name_of_table);
  if (!(status == 0))
  {
    perror("Failed to delete the file");
    exit(EXIT_FAILURE);
  }
  rename("temp.csv", name_of_table);
  fclose(temp_file_pointer);
  rewind(file_pointer);
  free(no_match_s); no_match_s = NULL;
  free(match_s); match_s = NULL;
  free(current_record); current_record = NULL;
}

void delete_entire_table(TableSchema* schema){
  char* name_of_table = schema->table_name;
  int status = remove(name_of_table);
  if (!(status == 0))
  {
    perror("Failed to delete the file");
    exit(EXIT_FAILURE);
  } else {
    printf("'%s' successfully deleted\n", name_of_table);
  }
}

// int main()
// {
//   char name_of_table[100] = "customers-100";
//   char *file_type = ".csv";
//   strcat(name_of_table, file_type);
//   TableSchema *table = create_table_schema(name_of_table);

//   if (file_exists(table->table_name))
//   {
//     // printf("File with name '%s' already exists.\n", table->table_name);
//     // exit(EXIT_FAILURE);

//     FILE *file_parser = fopen(table->table_name, "r");
//     read_fields_csv(table, file_parser);

//     // add_field(table, "field7", "int");
//     int num_rows = get_rows(file_parser); // Call get_rows once and store the
//                                           // result to avoid multiple calls
//     if (num_rows <= 0)
//     {
//       fprintf(stderr, "Failed to get the number of rows\n");
//       exit(EXIT_FAILURE);
//     }

//     //char **column_data = (char **)malloc(num_rows * sizeof(char *));
//     //if (column_data == NULL)
//     //{
//       //perror("Failed to allocate memory for column_data");
//       //exit(EXIT_FAILURE);
//     //}

//     // for (int i = 0; i < num_rows; ++i) {
//     //     column_data[i] = malloc(sizeof(char)+1);  // Initialize as NULL,
//     //     allocate later as needed
//     // }

//     //bool x = is_in_table(table, file_parser, "IT sales professional");
//     //printf("Your boolean variable is: %s\n", x ? "true" : "false");
//     //printf("num_of_rows:%d\n", get_rows(file_parser));
//     //update_record_based_on_another_record(table, file_parser, "6", "field1", "BBBBB");
//     //printf("num_of_rows:%d\n", get_rows(file_parser));
//     //for (int i = 0; i < num_rows - 1; i++) {
//       // printf("column_data[%d] = %s\n", i, column_data[i]);
//       //free(column_data[i]);
//     //}
//     //free(column_data);

//     fclose(file_parser);
//   }
//   else
//   {
//     // create file and add fields
//     FILE *file_pointer = fopen(table->table_name, "w+");
//     add_field(table, "field1", "int");
//     add_field(table, "field2", "char");
//     add_field(table, "field3", "int");
//     add_field(table, "field4", "char");
//     add_field(table, "field5", "int");
//     add_field(table, "field6", "char");

//     create_csv(table, file_pointer);

//     char *entry1[6] = {"1", "a", "2", "b", "3", "c"};
//     create_entry(table, entry1, file_pointer);
//     char *entry2[6] = {"d", "4", "e", "5", "f", "6"};
//     create_entry(table, entry2, file_pointer);

//     for (int i = 0; i < table->field_count; i++)
//     {
//       // printf("name: %s index: %d\n", table->fields[i].name,
//       // table->fields[i].field_index);
//     }
//     fclose(file_pointer);
//   }
//   freeTableSchema(table);
// }
