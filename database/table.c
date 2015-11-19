#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table.h"
#include "../type/type.h"

struct table_ {
   	FILE *pf;
	int col_no;
	type_t *types;
};

void table_create(char* path, int ncols, type_t* types) {
	if(!path || !types)
		return;
	FILE *pf;
	int i;

	pf=fopen(path, "w");
	if(!pf)
		return;

	fwrite(&ncols, 1, sizeof(int), pf);
	for(i=0; i<ncols; i++)
		fwrite(types+i, 1, sizeof(type_t), pf);

	fclose(pf);
	return;
}

table_t* table_open(char* path) {	/*we keep the file open until the end, so the rest of the functions know where to act*/
	int i;	
	table_t *  t;
	if(!path)
		return NULL;    	

	FILE *pf = fopen(path, "r+");
	if(!pf)
		return NULL;
	t= (table_t *)malloc(sizeof(table_t));
	fread(&(t->col_no), 1, sizeof(int), t->pf);
	t->types = (type_t *)malloc(t->col_no*sizeof(type_t));
	if(!t->types)
		return NULL;

	for(i=0;i<t->col_no;i++){
		fread(t->types+i, 1, sizeof(type_t), pf);		
	}
	return t;
}

void table_close(table_t* table) {
	if (!table)
		return;
	fclose(table->pf);
}

int table_ncols(table_t* table) {
	if(!table)
		return -1;
	return table->col_no;
}

type_t* table_types(table_t* table) {
	if(!table->types)
		return NULL;
	return table->types;
}

long table_first_pos(table_t* table) {
	if(!table)
		return -1;
	return sizeof(int) + sizeof(type_t)*table->col_no;
}

long table_last_pos(table_t* table) {
	if(!table)
		return -1;	/*hacer contador y devolverlo*/
	fseek(table->pf, 0L, SEEK_END);
    	return ftell(table->pf);
}

record_t* table_read_record(table_t* table, long pos) {
	int len, i;
	char * buf;
	void ** values;
	if(!fseek(table->pf, pos, SEEK_SET))
		return NULL;
	if(fread(&len, sizeof(int), 1, table->pf) < 1)
		return NULL;
	buf=(char*)malloc(len*sizeof(char));
	fread(buf, len, sizeof(char), table->pf);
	
	values= (void**) malloc (table->col_no*sizeof(void *));
	for(i=0; i<table->col_no ;i++){
		values[i]= (void*) buf;
		buf += value_length(table->types[i], values[i]);
	}
	return record_create(values, table->col_no, ftell(table->pf));
}

void table_insert_record(table_t* table, void** values) {
   	int i, len;
printf("1\n");
	for(i=0, len=0; i<table->col_no; i++)
		len+=value_length(table->types[i], values[i]);

	fseek(table->pf, 0L, SEEK_END);
	fwrite(&len, 1, sizeof(int), table->pf);
	
	for(i=0;i<table->col_no;i++)
		fwrite(values[i], 1, value_length(table->types[i], values[i]), table->pf);
printf("1\n");
	return;
}









