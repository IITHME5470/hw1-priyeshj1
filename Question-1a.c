#include <stdio.h>
#include <stdlib.h>

void print_to_file(int n, double **A, int format_flag) {
    char filename[50];
    snprintf(filename, sizeof(filename), "array_%06d_%s.out", n, format_flag == 0 ? "asc" : "bin");

    FILE *file = fopen(filename, format_flag == 0 ? "w" : "wb");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    if (format_flag == 0) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fprintf(file, "%.15f ", A[i][j]);
            }
            fprintf(file, "\n");
        }
    } else {
        for (int i = 0; i < n; i++) {
            fwrite(A[i], sizeof(double), n, file);
        }
    }

    fclose(file);
}

int main() {
    FILE *input_file = fopen("input.in", "r");
    if (!input_file) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    int n = 4000;
    int format_flag = 1;

    fscanf(input_file, "%d", &n);
    fclose(input_file);

    double **A = (double **)malloc(n * sizeof(double *));
    if (!A) {
        perror("Memory allocation failed");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n; i++) {
        A[i] = (double *)malloc(n * sizeof(double));
        if (!A[i]) {
            perror("Memory allocation failed");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = i + j;
        }
    }

    printf("Enter format flag (0 for ASCII, 1 for binary): ");
    scanf("%d", &format_flag);

    if (format_flag != 0 && format_flag != 1) {
        fprintf(stderr, "Invalid format flag. Must be 0 or 1.\n");
        return EXIT_FAILURE;
    }

    print_to_file(n, A, format_flag);

    for (int i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);

    return 0;
}
