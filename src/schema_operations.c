#include <stdio.h>
#include <stdlib.h>
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
    schema->fields[i].name = field_name; // adding name to field[i]
    schema->fields[i].type = field_type; // adding type to field[i]
}

int main() {
    //printf("Hello World\n");
    TableSchema *table = create_table_schema("test");
    add_field(table, "field1", "int");
    add_field(table, "field2", "char");
    printf("%d\n",table->field_count);
    printf("name: %s type: %s\n", table->fields[1].name, table->fields[1].type);
}