#ifndef DB_STRUCTS_H
#define DB_STRUCTS_H

typedef struct {
    char *name; // Name of field
    char *type; // Data type of field
} Field;

typedef struct {
    char *table_name; // Name of the table
    int field_count; // Number of fields in the table
    Field *fields; // Pointer to an array of Field structs
} TableSchema;

#endif // DB_STRUCTS_H