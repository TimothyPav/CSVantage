#include "../include/db_structs.h"
// #include <cstddef>
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
void select_column_by_field(TableSchema *schema, char *field, FILE *file_pointer, char **arr);
bool is_in_table(TableSchema *schema, FILE *file_pointer, char *input);
void print_rows_including_input(FILE* file_pointer, char* input);

//UPDATE

//DELETE
void delete_last_line(TableSchema *schema, FILE *file_pointer, int num_of_lines);
void delete_row_including_input(TableSchema *schema, FILE *file_pointer, char *input);


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

void create_entry(TableSchema *schema, char *arr[schema->field_count],
                  FILE *file_pointer)
{
  for (int i = 0; i < schema->field_count; i++)
  {
    if (i == schema->field_count - 1)
      fprintf(file_pointer, "%s\n", arr[i]);
    else
      fprintf(file_pointer, "%s,", arr[i]);
  }
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
  do
  {
    ch = fgetc(file_pointer);
    if (ch == ',')
    {
      s[i] = '\0';
      add_field(schema, s, "placeholder");
      field_num++;
      i = 0;
      s[0] = '\0';
    }
    else if (ch == '\n')
    {
      s[i] = '\0';
      add_field(schema, s, "placeholder");
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

void select_column_by_field(TableSchema *schema, char *field, FILE *file_pointer, char **arr)
{
  if (schema == NULL || field == NULL || file_pointer == NULL || arr == NULL)
  {
    fprintf(stderr, "Invalid input to function\n");
    return; // Optionally handle the error more gracefully or exit if critical
  }

  read_fields_csv(schema, file_pointer);

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
        // printf("s: %s\n", s);
        // printf("row: %d\n", row-2);
        // arr[row-2] = malloc(strlen(s) + 99);
        arr[row - 2] = strdup(s);
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
      // printf("%c", ch);
      s[i++] = ch;
    }

  } while (ch != EOF);
  rewind(file_pointer);
}

int get_rows(FILE *file_pointer)
{
  char buf[BUF_SIZE];
  int counter = 0;
  for (;;)
  {
    size_t res = fread(buf, 1, BUF_SIZE, file_pointer);
    if (ferror(file_pointer))
      return -1;

    int i;
    for (i = 0; i < res; i++)
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
        return true;
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
        return true;
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
  return false;
}

void delete_last_line(TableSchema *schema, FILE *file_pointer, int num_of_lines)
{
  FILE *temp_file_pointer = fopen("temp.csv", "w+");
  char ch = '?';
  rewind(file_pointer);
  int rows = get_rows(file_pointer);
  printf("get_rows:%d\n", rows);
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

void print_rows_including_input(FILE* file_pointer, char* input){
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
      if(lossey_str_cmp(input, line)) printf("%s", line);
      line = NULL;
    }

  }
  rewind(file_pointer);
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

int main()
{
  char name_of_table[100] = "customers-100";
  char *file_type = ".csv";
  strcat(name_of_table, file_type);
  TableSchema *table = create_table_schema(name_of_table);

  if (file_exists(table->table_name))
  {
    // printf("File with name '%s' already exists.\n", table->table_name);
    // exit(EXIT_FAILURE);

    FILE *file_parser = fopen(table->table_name, "r");

    // add_field(table, "field7", "int");
    int num_rows = get_rows(file_parser); // Call get_rows once and store the
                                          // result to avoid multiple calls
    if (num_rows <= 0)
    {
      fprintf(stderr, "Failed to get the number of rows\n");
      exit(EXIT_FAILURE);
    }

    char **column_data = (char **)malloc(num_rows * sizeof(char *));
    if (column_data == NULL)
    {
      perror("Failed to allocate memory for column_data");
      exit(EXIT_FAILURE);
    }

    // for (int i = 0; i < num_rows; ++i) {
    //     column_data[i] = malloc(sizeof(char)+1);  // Initialize as NULL,
    //     allocate later as needed
    // }

    // select_column_by_field(table, "Job Title", file_parser, column_data);

    //bool x = is_in_table(table, file_parser, "IT sales professional");
    //printf("Your boolean variable is: %s\n", x ? "true" : "false");
    printf("num_of_rows:%d\n", get_rows(file_parser));
    print_entire_table(file_parser);
    printf("num_of_rows:%d\n", get_rows(file_parser));
    /*
     for (int i = 0; i < num_rows - 1; i++) {
      // printf("column_data[%d] = %s\n", i, column_data[i]);
      free(column_data[i]);
    }
    free(column_data);
    */
    fclose(file_parser);
  }
  else
  {
    // create file and add fields
    FILE *file_pointer = fopen(table->table_name, "w+");
    add_field(table, "field1", "int");
    add_field(table, "field2", "char");
    add_field(table, "field3", "int");
    add_field(table, "field4", "char");
    add_field(table, "field5", "int");
    add_field(table, "field6", "char");

    create_csv(table, file_pointer);

    char *entry1[6] = {"1", "a", "2", "b", "3", "c"};
    create_entry(table, entry1, file_pointer);
    char *entry2[6] = {"d", "4", "e", "5", "f", "6"};
    create_entry(table, entry2, file_pointer);

    for (int i = 0; i < table->field_count; i++)
    {
      // printf("name: %s index: %d\n", table->fields[i].name,
      // table->fields[i].field_index);
    }
    fclose(file_pointer);
  }
  freeTableSchema(table);
}
