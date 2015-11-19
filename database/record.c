#include "record.h"

struct record_ {
	long next;
	int ncols;
	void ** values;
};

record_t* record_create(void** values, int ncols, long next) {
	record_t * record = (record_t *) malloc(sizeof(record_t)) ;
	record->next= next;
	record->ncols=ncols;
	record->values=values;
}

void* record_get(record_t* record, int n) {
	/*Las columnas se cuentan desde 0, es puto importante*/    
	return record->values[n];
}

long record_next(record_t* record) {
	return record->next;
}

void record_free(record_t* record) {
	int i;
	for (i=0;i<record->ncols;i++) free(record->values[i]);
	free(record->values);
	free(record);
	return;
}
