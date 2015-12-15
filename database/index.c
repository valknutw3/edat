#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "index.h"

typedef struct {
	int key;
	int npos;
	long *pos;
}entry;

struct index_ {
	entry ** idx;
	int nidx;
};

int index_create(char* path) {
	if(!path){
		fputs("Invalid arguments for index_create",stderr);
		abort();	
	}
	FILE *fp = fopen (path, "r");
	if(!fp){
		fputs("Error opening file", stderr);
		abort();
	}
	int q =0;

	
	fwrite(&q,sizeof(int),1,fp);
	fclose(fp);
}

int index_save(index_t *index, char* path) {
	if(!index || !path){
		fputs("\nInvalid argumentes for index_save", stderr);
		abort();
	}
	FILE *fp = fopen (path, "w");
	if(!fp){
		fputs("Error opening file", stderr);
		abort();
	}
	fwrite(&index->nidx,1,sizeof(int),fp);
	for(int i=0; i< index->nidx;i++){
		fwrite(&(index->idx[i]->key),sizeof(int),1,fp);
		fwrite(&(index->idx[i]->npos),sizeof(int),1,fp);
		fwrite(index->idx[i]->pos,sizeof(long),index->idx[i]->npos,fp);

	
	}
	fcolse(fp);
}

index_t* index_open(char* path) {
	if(!path){
		fputs("Invalid arguments for index_open",stderr);
		abort();	
	}
	FILE *fp = fopen (path, "r");
	if(!fp){
		fputs("Error opening file", stderr);
		abort();
	}
	int nidx;
	entry  * ent;
	fread(&nidx,1,sizeof(int),fp);	
	index_t * index = malloc(sizeof(index_t));/**/
	index->nidx = nidx;
	index->idx=malloc(nidx * sizeof( entry *));/**/
	for(int i=0; i<nidx; i++){
		ent=malloc(sizeof(entry));/**/
		fread(&(ent->key),sizeof(int),1,fp);
		fread(&(ent->npos),sizeof(int),1,fp);
		ent->pos=malloc(ent->npos *sizeof(long));/**/
		fread(ent->pos,sizeof(long),ent->npos,fp);
		index->idx[i]=ent;
	
	}

	 
}

int index_put(index_t *index, int key, long pos) {
	if(!index) {
		fputs("Invalid arguments for index_put", fputs);
		abort();
	}
	int found=0;

/*BINARY SEARCH CODE*/
	int m,P,U;
	P=0;
	U=index->nidx;

	while(P<=U){
		m=(P+U)/2;
		if(index->idx[m]->key==key){
			found=1;
			break;
		}
		else if(key<index->idx[m]->key) U=m-1;
		else P=m+1;
	}
	 
/*************************/
	int i=m;

	if (!found) {

		index->nidx++;
		index->idx = realloc(index->idx,index->nidx*sizeof(entry *));
		for (i=index->nidx-1; i>0 && index->idx[i-1]->key > key; i--)
			index->idx[i] = index->idx[i-1];
		entry * ent= malloc(sizeof(entry));
		ent->key=key;
		ent->npos=1;
		ent->pos=malloc(sizeof(long));
		ent->pos[0]= pos;
		index->idx[i]=ent;
		
		return 0;
	}else{
		index->idx[i]->npos++;
		index->idx[i]->pos=realloc(index->idx[i]->pos,index->idx[i]->npos * sizeof(long));
		index->idx[i]->pos[index->idx[i]->npos]=pos;

		return 1;

	}
}

void index_get(index_t *index, int key, long** offsets, int* noffsets) {
	/*ERROR CONTROL*/

/*BINARY SEARCH CODE*/
	int m,P,U;
	P=0;
	U=index->nidx;

	while(P<=U){
		m=(P+U)/2;
		if(index->idx[m]->key==key){
			found=1;
			break;
		}
		else if(key<index->idx[m]->key) U=m-1;
		else P=m+1;
	}
	 
/*************************/
	* noffsets= index->idx[m]->npos;
	* offsets= index->idx[m]->pos;
	
	
}

void index_close(index_t *index) {
	if(!index) {
		fputs("Invalid arguments for index_close", fputs);
		abort();
	}
	for (int i =0; i<index->nidx; i ++){
		if(index->idx[i]) {
			for (int j=0; j<index->idx[i]->npos; j++)
				free(index->idx[i]->pos);
			free(index->idx[i]);
		}	
	}
	if (index->idx) free(index->idx);
	free(index);
	return;
}


/******************/


