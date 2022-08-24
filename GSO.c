/*Written by Manas Sharma. BS2137 BStat 1st Year
Please forgive any bad practices as he has learnt coding just last semester.
He is also terribly sorry for writing in C but he is still in the process of learning R.*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

// Basic functionalities...

double dot(double *a, double *b, int length)
{
    double dot_product = 0;
    for (int i = 0; i < length; i++)
    {
        dot_product += ((*(a + i)) * (*(b + i)));
    }
    return dot_product;
}

double norm(double *a, int length)
{
    double norm_square = dot(a, a, length);
    return sqrt(norm_square);
}

void unit_vec(double *a, int length)
{
    double s = norm(a, length);
    for (int i = 0; i < length; i++)
    {
        a[i] /= s;
    }
}

// An indexing function since code implements an 1D array as a matrix (C11 removed Variable Length Arrays)...

double *index(double *a, int i, int j, int length)
{
    return (a + i * length + j);
}

// I work with rows throughout the code, its in presentation that I use columns...

void row_extract(double *matrix, double *b, int i, int length)
{
    for (int j = 0; j < length; j++)
    {
        b[j] = *index(matrix, i, j, length);
        printf("%lf\n", b[j]);
    }
}

// The GSO method for orthogonalisation without the normalisation...

void orth_vec(double *vec_list, double *b, int length, int num_row)
{

    double dot_arr[num_row];

    for (int k = 0; k < num_row; k++)
    {
        dot_arr[k] = dot(b, index(vec_list, k, 0, length), length);  
    }

    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < num_row; j++)
        {
            b[i] -= dot_arr[j] * (*index(vec_list, j, i, length));
        }
    }
}

void print_matrix(double *a, int length, int num_col, bool print_zerovec)
{
    for (int j = 0; j < length; j++)
    {

        for (int i = 0; i < num_col; i++)
        {
            if (((norm(index(a, i, 0, length), length) > pow(10,-6)) || print_zerovec))
            {
                printf("%+4lf\t", *index(a, i, j, length));
            }

            else
            {
                continue;
            }
            
        }
        printf("\n");
    }
    printf("\n");
}

void create_matrix(double *matrix, int length, int num_row)
{

    for (int i = 0; i < num_row; i++)
    {
        for (int j = 0; j < length; j++)
        {
            printf("Please enter the %dth entry of the %dth vector: ", j + 1, i + 1);
            scanf("%lf", index(matrix, i, j, length));
        }
        printf("The %dth vector has been stored!\n", i + 1);
    }
    printf("\n");
}

// Rudimentary array copying functionality...

void row_eq(double *matrix, double *row, int i, int length)
{
    double temp[length];
    memcpy(temp, row, length * sizeof(double));
    unit_vec(temp, length);
    for (int j = 0; j < length; j++)
    {
        *index(matrix, i, j, length) = temp[j];
        printf("%lf\n", temp[j]);
    }
}

int main()
{
    int num_vec;
    int length;
    int nullity = 0;

    printf("Please input the length of your vectors: ");
    scanf("%d", &length);
    printf("Please input size of starting set of vectors: ");
    scanf("%d", &num_vec);

    // Dynamic Allocation of required arrays...

    double *temp_vec = calloc(length, sizeof(double));
    double *start_matrix = calloc(num_vec * length, sizeof(double));
    double *final_matrix = calloc(num_vec * length, sizeof(double));

    // Initialization of required arrays...

    create_matrix(start_matrix, length, num_vec);
    row_eq(final_matrix, start_matrix, 0, length);

    // GSO...

    for (int i = 1; i < num_vec; i++)
    {
        row_extract(start_matrix, temp_vec, i, length);
        orth_vec(final_matrix, temp_vec, length, num_vec);

        if (norm(temp_vec, length) <= pow(10, -6)) // Checking for Linear Dependence...
        {
            nullity += 1;
            continue;
        }
        else
        {
            row_eq(final_matrix, temp_vec, i, length);
        }
    }

    printf("Your starting list of vectors in column vector form is \n");
    print_matrix(start_matrix, length, num_vec, true);
    printf("Your orthonormal list of vectors in column vector form is \n");
    print_matrix(final_matrix, length, num_vec, false);
    printf("Number of linearly dependent vectors are %d\n\n", nullity);

    // Freeing of allocated memory...

    free(final_matrix);
    free(start_matrix);
    free(temp_vec);
    return 0;
}
