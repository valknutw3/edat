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

	fwrite(&ncols,sizeof(int), 1, pf);
	for(i=0; i<ncols; i++)
		fwrite(types+i, sizeof(type_t),1, pf);

	fclose(pf);

	return;
}

table_t* table_open(char* path) {	/*we keep the file open until the end, so the rest of the functions know where to act*/
	int i;	
	table_t *  t;
	if(!path)
		return NULL;    	
										printf("%d\n",221);
	FILE *pf = fopen(path, "r+");						printf("%s\n", path);
	if(!pf)
		return NULL;
	
	t= (table_t *)malloc(sizeof(table_t));
	t->pf=pf;
	pf=NULL;									printf("%d\n",2211);
										printf("1: %d\n",t->col_no);
	fread(&(t->col_no), sizeof(int), 1, t->pf);
										printf("2: %d\n",t->col_no);
										fflush(stdout);
	t->types = (type_t *)malloc(t->col_no*sizeof(type_t));
	if(!t->types)
		return NULL;
										printf("%d\n",222);
	for(i=0;i<t->col_no;i++){
		fread(t->types+i, sizeof(type_t),1, t->pf);			
	}
										printf("%d\n",222);
										fflush(stdout);
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
	fread(buf,  sizeof(char),len, table->pf);
	
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
	fwrite(&len, sizeof(int), 1, table->pf);				/*May need len as first parameter*/
	
	for(i=0;i<table->col_no;i++)
		fwrite(values[i], value_length(table->types[i], values[i]), 1, table->pf);   /*May need &values[i] as first parameter*/
										printf("11\n");
	return;
}









