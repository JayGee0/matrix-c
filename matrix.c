#include "matrix.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include <errno.h> /* for ENOSYS */

void print_matrix(matrix_t *m) {
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->columns; j++) {
            printf("%d ", m->content[i][j]);
        }
        printf("\n");
    }
}

int matrix_allocate(matrix_t *m, int rows, int columns) {
    /* implement the function here ... */
    if(rows < 1 || columns < 1) return -1;
    if(!m) return -1;

    m->columns = columns;
    m->rows = rows;
    m->content = malloc(rows * sizeof(int *));
    if (m->content == NULL) {
        return -1;
    }
    
    for(int i = 0; i < rows; i++) {
        m->content[i] = malloc(columns * sizeof(int));
        if (m->content[i] == NULL) {
            for(int j = 0; j < i; j++) {
                free(m->content[j]);
            }
            free(m->content);
            return -1;
        }
    }
    
    return 0; /* ENOSYS is a  standard error code that means 'function
                       not implemented'. We use it to force the failure of tests
                       while the functions are not written. Make sure to remove
                       it when you start to work on a function */
}

void matrix_free(matrix_t *m) {
    if(!m) return;
    /* implement the function here ... */
    for(int i = 0; i < m->rows; i ++) free(m->content[i]);
    free(m->content);
    m->content = NULL;
}

void matrix_init_n(matrix_t *m, int n) {
    if(!m) return;
    /* implement the function here ... */
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->columns; j++) {
            m->content[i][j] = n;
        }
    } 
}

void matrix_init_zeros(matrix_t *m) {
    if(!m) return;
    /* implement the function here ... */
    matrix_init_n(m, 0);
}

int matrix_init_identity(matrix_t *m){
    /* implement the function here ... */
    if(!m) return -1;
    matrix_init_zeros(m);
    if (m->rows != m-> columns) return -1;

    for(int i = 0; i < m->rows; i++) {
        m->content[i][i] = 1;
    }
    return 0;
}

void matrix_init_rand(matrix_t *m, int val_min, int val_max) {
    if(!m) return;
    /* implement the function here ... */
    int interval = val_max - val_min;
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->columns; j++) {
            m->content[i][j] = val_min + (rand() % (interval));
        }
    }
}

int matrix_equal(matrix_t *m1, matrix_t *m2) {
    /* implement the function here ... */
    if(!m1 || !m2) return 0;
    if(m1->rows != m2->rows) return 0;
    if(m1->columns != m2->columns) return 0;
    for(int i = 0; i < m1->rows; i++) {
        for(int j = 0; j < m1->columns; j++) {
            if(m1->content[i][j] != m2->content[i][j]) return 0;
        }
    }
    return 1;
}

int matrix_sum(matrix_t *m1, matrix_t *m2, matrix_t *result) {
    /* implement the function here ... */
    if(!m1 || !m2) return -1;
    if(m1->rows != m2->rows) return -1;
    if(m1->columns != m2->columns) return -1;

    if(matrix_allocate(result, m1->rows, m1->columns) != 0) return -1;

    for(int i = 0; i < result->rows; i++) {
        for(int j = 0; j < result->columns; j++) {
            result->content[i][j] = m1->content[i][j] + m2->content[i][j];
        }
    }

    return 0;
}

int matrix_scalar_product(matrix_t *m, int scalar, matrix_t *result) {
    /* implement the function here ... */
    if(!m) return -1;
    if(matrix_allocate(result, m->rows, m->columns) != 0) return -1;
    
    for(int i = 0; i < result->rows; i++) {
        for(int j = 0; j < result->columns; j++) {
            result->content[i][j] = m->content[i][j] * scalar;
        }
    }

    return 0;
}

int matrix_transposition(matrix_t *m, matrix_t *result) {
    /* implement the function here ... */
    if(!m) return -1;
    if(matrix_allocate(result, m->columns, m->rows) != 0) return -1;
    for(int i = 0; i < result->rows; i++) {
        for(int j = 0; j < result->columns; j++) {
            result->content[i][j] = m->content[j][i];
        }
    }
    
    return 0;
}

int dot_product(int *v1, int *v2, int size) {
    int ans = 0;
    for(int i = 0; i < size; i++) {
        ans += v1[i] * v2[i];
    }

    return ans;
}

int matrix_product(matrix_t *m1, matrix_t *m2, matrix_t *result) {
    /* implement the function here ... */
    if(!m1 || !m2) return -1;
    if(m1->columns != m2->rows) return -1;
    if(matrix_allocate(result, m1->rows, m2->columns) != 0) return -1;
    matrix_t m2Transpose;
    if(matrix_transposition(m2, &m2Transpose) == -1) {
        matrix_free(result);
        return -1;
    }

    for(int i = 0; i < result->rows; i++) {
        for(int j = 0; j < result->columns; j++) {
            result->content[i][j] = dot_product(m1->content[i], m2Transpose.content[j], m1->columns);
        }
    }
    
    matrix_free(&m2Transpose);
    return 0;
}

int matrix_dump_file(matrix_t *m, char *output_file) {
    /* implement the function here ... */
    if(!m || !output_file) return -1;
    FILE *file = fopen(output_file, "w");
    if (file == NULL) return -1;

    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->columns; j++) {
             fprintf(file, "%d", m->content[i][j]);
             if ( j < m->columns - 1) fprintf(file, " ");
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return 0;
}

int matrix_allocate_and_init_file(matrix_t *m, char *input_file) {
    if (!m || !input_file) return -1;

    FILE *file = fopen(input_file, "r");
    if (file == NULL) return -1;

    // Determine the dimensions of the matrix
    int total_rows = 0;
    int total_cols = -1;
    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, file) != -1) {
        if (strlen(line) == 0) continue;
        char *token = strtok(line, " ");
        if (!strcmp(token, "\n")) continue;

        total_rows++;
        int cols = 0;

        while (token != NULL) {
            if (strcmp(token, "\n")) cols++; // Count the number of integers in the line
            token = strtok(NULL, " ");
        }

        if (total_cols != -1 && cols != total_cols) { // If number of columns is inconsistent
            free(line);
            fclose(file);
            return -1;
        } else {
            total_cols = cols;
        }
    }

    rewind(file);

    if (matrix_allocate(m, total_rows, total_cols) != 0) {
        matrix_free(m);
        free(line);
        fclose(file);
        return -1;
    }

    int row = 0;
    while (getline(&line, &len, file) != -1) {
        if (strlen(line) == 0) continue;
        char *token = strtok(line, " \n");
        if (!token) continue;

        int col = 0;

        while (token != NULL) {
            char *endptr;
            long value = strtol(token, &endptr, 10);

            if (*endptr != '\0' && *endptr != '\n') {
                // Conversion failed
                free(line);
                fclose(file);
                matrix_free(m);
                return -1;
            }

            m->content[row][col] = (int)value;
            col++; 
            token = strtok(NULL, " \n");
        }

        row++;
    }

    free(line);
    fclose(file);
    return 0;
}


// int matrix_allocate_and_init_file(matrix_t *m, char *input_file) {
//     /* implement the function here ... */
//     if(!m || !input_file) return -1;
//     FILE *file = fopen(input_file, "r");
//     if (file == NULL) return -1;

//     // Determine the dimensions of the matrix
//     int total_rows = 0;
//     int total_cols = -1;
//     char line[20000];
    
//     while (fgets(line, sizeof(line), file) != NULL) {
//         if (strlen(line) == 0) continue;
//         char *token = strtok(line, " ");
//         if(!strcmp(token, "\n")) continue;
        
//         total_rows++;
//         int cols = 0;

//         while (token != NULL) {
//             if(strcmp(token, "\n")){
//                 cols++; // Count the number of integers in the line
//             } 
            
//             token = strtok(NULL, " ");
//         }
//         printf("%d %d %d\n", total_rows, total_cols, cols);

//         if(total_cols != -1 && cols != total_cols) return -1; 
//         else total_cols = cols;

//     }
//     printf("%d %d \n", total_rows, total_cols);


//     if(matrix_allocate(m, total_rows, total_cols) != 0) return -1;
    
//     rewind(file);

//     int row = 0;
//     while (fgets(line, sizeof(line), file) != NULL) {
//         if (strlen(line) == 0) continue;
//         char *token = strtok(line, " ");
//         if(!strcmp(token, "\n")) continue;
        
//         int col = 0;

//         while (token != NULL) {
//             if(strcmp(token, "\n")) {
//                 m->content[row][col] = atoi(token);
//                 col++; // Count the number of integers in the line
//             }
//             token = strtok(NULL, " ");
//         }
        
//         row++;
//     }

//     // Close the file
//     fclose(file);

//     return 0;
// }

