#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void readMatrix(const char *filename, int n, double matrix[n][n]) {
    char location[100];
    snprintf(location, sizeof(location), "inputfiles/%s", filename);
    FILE *file = fopen(location, "r");
    if (!file) {
        perror("Error opening matrix file");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(file, "%lf", &matrix[i][j]);
            if (j < n - 1) {
                fgetc(file);
            }
        }
    }
    fclose(file);
}

void readVector(const char *filename, int n, double vector[n]) {
    char location[100];
    snprintf(location, sizeof(location), "inputfiles/%s", filename);
    FILE *file = fopen(location, "r");
    if (!file) {
        perror("Error opening vector file");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        fscanf(file, "%lf", &vector[i]) != 1;
        if (i < n - 1) {
            fgetc(file);
        }
    }
    fclose(file);
}

bool isEigenvector(int n, double matrix[n][n], double vector[n], double *eigenvalue) {
    double result[n];
    int firstNonZeroIndex = -1;

    for (int i = 0; i < n; i++) {
        result[i] = 0.0;
        for (int j = 0; j < n; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }

    for (int i = 0; i < n; i++) {
        if (vector[i] != 0.0) {
            firstNonZeroIndex = i;
            break;
        }
    }

    if (firstNonZeroIndex == -1) {
        return false;
    }

    *eigenvalue = result[firstNonZeroIndex] / vector[firstNonZeroIndex];

    for (int i = 0; i < n; i++) {
        if (result[i] - *eigenvalue * vector[i] > 1e-14) {
            return false;
        }
    }

    return true;
}

void appendEigenvalueToFile(const char *filename, double eigenvalue) {
    char location[100];
    snprintf(location, sizeof(location), "inputfiles/%s", filename);
    FILE *file = fopen(location, "a");
    if (!file) {
        perror("Error opening vector file for appending");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "\nEigenvalue: %.15f\n", eigenvalue);
    fclose(file);
}

int main() {
    FILE *input_file = fopen("input.in", "r");
    if (!input_file) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    int n;
    fscanf(input_file, "%d", &n);
    fclose(input_file);

    double matrix[n][n];
    char matrix_filename[50];
    snprintf(matrix_filename, sizeof(matrix_filename), "mat_%06d.in", n);
    readMatrix(matrix_filename, n, matrix);

    for (int vecnum = 1; vecnum <= 4; vecnum++) {
        char vector_filename[50];
        snprintf(vector_filename, sizeof(vector_filename), "vec_%06d_%06d.in", n, vecnum);

        double vector[n];
        readVector(vector_filename, n, vector);

        double eigenvalue;
        if (isEigenvector(n, matrix, vector, &eigenvalue)) {
            printf("%s : Yes : %.15f\n", vector_filename, eigenvalue);
            appendEigenvalueToFile(vector_filename, eigenvalue);
        } else {
            printf("%s : Not an eigenvector\n", vector_filename);
        }
    }

    return 0;
}
