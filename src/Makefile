default: main
	./main

main: main.c schema_operations.c
	gcc main.c schema_operations.c -o main

runschema_operations: schema_operations
	./schema_operations

schema_operations: schema_operations.c
	gcc schema_operations.c -o schema_operations

runTest: 
	gcc test.c -o test
	./test

