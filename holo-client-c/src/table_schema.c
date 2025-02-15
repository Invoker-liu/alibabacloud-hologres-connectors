#include "table_schema.h"
#include "table_schema_private.h"
#include "logger.h"
#include "utils.h"

void holo_client_destroy_tablename(TableName* tableName) {
    FREE(tableName->fullName);
    tableName->fullName = NULL;
    FREE(tableName->schemaName);
    tableName->schemaName = NULL;
    FREE(tableName->tableName);
    tableName->tableName = NULL;
}

void holo_client_destroy_columns(Column* columns, int n) {
    if (columns == NULL) {
        return;
    }
    for (int i = 0; i < n; i++) {
        FREE(columns[i].name);
        FREE(columns[i].quoted);
        FREE(columns[i].defaultValue);
    }
    FREE(columns);
    columns = NULL;
}

Column* holo_client_new_columns(int n) {
    Column* columns = MALLOC(n, Column);
    for (int i = 0; i < n; i++) {
        columns[i].name = NULL;
        columns[i].quoted = NULL;
        columns[i].defaultValue = NULL;
    }
    return columns;
}

TableSchema* holo_client_new_tableschema() {
    TableSchema* schema = MALLOC(1, TableSchema);
    schema->tableName = MALLOC(1, TableName);
    schema->tableName->fullName = NULL;
    schema->tableName->schemaName = NULL;
    schema->tableName->tableName = NULL;
    schema->columns = NULL;
    schema->nColumns = 0;
    schema->nDistributionKeys = 0;
    schema->distributionKeys = NULL;
    //schema->dictionaryEncoding = NULL;
    //schema->bitmapIndexKey = NULL;
    //schema->clusteringKey = NULL;
    //schema->segmentKey = NULL;
    schema->primaryKeys = NULL;
    schema->nPrimaryKeys = 0;
    schema->partitionColumn = -1;
    schema->estimatedRecordByteSize = 0;
    return schema;
}

void holo_client_destroy_tableschema(TableSchema* schema) {
    holo_client_destroy_tablename(schema->tableName);
    FREE(schema->tableName);
    holo_client_destroy_columns(schema->columns, schema->nColumns);
    FREE(schema->distributionKeys);
    //FREE(schema->dictionaryEncoding);
    //FREE(schema->bitmapIndexKey);
    //FREE(schema->clusteringKey);
    //FREE(schema->segmentKey);
    FREE(schema->primaryKeys);
    FREE(schema);
    schema = NULL;
}

int get_colindex_by_colname(TableSchema* schema, char* colName) {
    for (int i = 0; i < schema->nColumns; i++) {
        if (strcmp(colName, schema->columns[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

bool table_has_pk(TableSchema* schema) {
    return (schema->nPrimaryKeys > 0);
}