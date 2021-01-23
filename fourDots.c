#include <stdio.h> 
#include <math.h> 
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define NUM_OF_POINTS 4
/** Set index in the loop that the index was moving in a circle. 
* Example:
*   1 2 3 4
*   2 3 4 1
*   3 4 1 2
*   4 3 2 1 
*/
#define set_index(i, num) \
    ((i+num) >= (NUM_OF_POINTS) ? ((i - NUM_OF_POINTS) + num) : (i+num))

/**
 * struct point - Is used for saving parameters of point .
 * @name: Name of the point.
 * @x: x coordinate.
 * @y: y coordinate.
 *
 * This structure saves parameters of point for comfortable using.  
 */
struct point {
    char name;
    float x;
    float y;
};

/**
 * struct triangle - Is used for saving the parameters of triangle.
 * @side1: size of side1.
 * @side2: size of side2.
 * @side3: size of side3.
 * @area: area of triangle.
 * 
 *
 * This structure saves parameters of point for comfortable using.  
 */
struct triangle {

    float side1;
    float side2;
    float side3;
    float area;
};

typedef struct point point;
typedef struct triangle triangle;

/**
 * enter_points() - sets parameters of point.
 * @current_point: Pointer to the structure point.
 *
 * Sets x and y coordinates in the structure poit.
 * 
 * Context: Sets x and y coordinates using scanf().
 */
void enter_points(point *current_point) 
{
    printf("Enter points coordinate for %c:\nx = ", current_point->name);
    scanf("%f", &current_point->x);
    printf("y = ");
    scanf("%f", &current_point->y);

    printf("%c: (x y) = (%.2f %.2f)\n", current_point->name, 
        current_point->x, current_point->y);

}

/**
 * enter_points() - reads file.
 * @fileName: Pointer to the name of file wich 
 *            contains coordinates of points.
 * 
 * Context: Reads file and writes content to buffer.
 *
 * Return: buffer which contains content of file
 */
char *read_file(char *fileName)
{
    FILE *fp = fopen(fileName, "r");

    if(NULL == fp) {
        fprintf(stderr, "File not found!\n");
        return NULL;
    }

    // Size of file
    fseek(fp, 0L, SEEK_END); 
    long size = ftell(fp);
    fseek(fp, 0L, SEEK_SET); 

    // Create buffer
    char *buffer = (char *)malloc(size);
    if (NULL == buffer) {
        fprintf(stderr, "Failed!\n");
        return NULL;
    }

    memset(buffer, 0, size);
    fread(buffer, 1, size, fp);

    fclose(fp);
    return buffer;
}

/**
 * set_side() - calc side of triangle.
 * @p1: Pointer to the first structure point.
 * @p2: Pointer to the second structure point.
 * 
 * Context: Calc length of side.
 *
 * Return: Length of side
 */
float set_side(point *p1, point *p2) 
{
    float length;
    float p1p2_x = p2->x - p1->x;
    float p1p2_y = p2->y - p1->y;

    length = sqrt(pow(p1p2_x, 2) + pow(p1p2_y, 2));

    return length;
}

/**
 * set_area() - set area in structure triangle.
 * @current_triangle: Pointer to the structure triangle.
 * 
 * Context: Calc area of triangle with known three sides.
 */
void set_area(triangle *current_triangle) 
{
    float a = current_triangle->side1;
    float b = current_triangle->side2;
    float c = current_triangle->side3;
    
    // Semiperimeter
    float p = (float)(a + b + c) / 2;

    current_triangle->area = sqrt(p * (p - a) * (p - b) * (p - c));
}

/**
 * new_triangle() - creates struct triangle.
 * @p1: Pointer to the first structure point.
 * @p2: Pointer to the second structure point.
 * @p3: Pointer to the third structure point.
 * 
 * Context: Creates structure triangle using structures points.
 *
 * Return: struct triangle
 */
triangle new_triangle(point *p1, point *p2, point *p3) 
{
    triangle current_triangle;

    current_triangle.side1 = set_side(p1, p2);
    current_triangle.side2 = set_side(p2, p3);
    current_triangle.side3 = set_side(p3, p1);
    set_area(&current_triangle);

    return current_triangle;
}

/**
 * check_point() - checks, is the point outside or in the triangle.
 * @tr_p1: Pointer to the first structure point.
 * @tr_p2: Pointer to the second structure point.
 * @tr_p3: Pointer to the third structure point.
 * @ex_p: Pointer to the fourth structure point which is checked.
 * 
 * Context: The first three points are points of triangle 
            and the fourth is point to check.
 *
 * Return: true - if point in and false if outside.
 */
bool check_point(point *tr_p1, point *tr_p2, point *tr_p3, point *ex_p)  
{
    triangle main_tr = new_triangle(tr_p1, tr_p2, tr_p3);
    triangle ex_tr1 = new_triangle(tr_p1, tr_p2, ex_p);
    triangle ex_tr2 = new_triangle(tr_p1, ex_p, tr_p3);
    triangle ex_tr3 = new_triangle(ex_p, tr_p2, tr_p3);

    float sum_of_areas = ex_tr1.area + ex_tr2.area + ex_tr3.area; 
    // The difference must be equal to 0 if the point is in the triangle
    float diff = main_tr.area - sum_of_areas;
    // Checking difference, taking into measurement error in calculations
    if (diff <= 1e-3 && diff >= -1e-3) {
        return true;
    }
    
    return false;    
}


int main(int argc, char *argv[]) 
{    
    point point_a;
    point point_b;
    point point_c; 
    point point_d;

    // For comfortable using
    point *ptr_plist[] = {&point_a, &point_b, &point_c, &point_d};

    for (int i = 0; i < NUM_OF_POINTS; i++) {
        ptr_plist[i]->name = 'A' + i;
    }  

    char *buf = NULL; 
    
    // If we use reading from file
    if (argc > 1) {
        int size = strlen(argv[1]);
        char *fileName = malloc(size + 7);
        if (NULL == fileName) {
            fprintf(stderr, "Failed!\n");
            return 1;
        }
        strncpy(fileName, "./", 3);
        strncat(fileName, argv[1], size);

        buf = read_file(fileName);
        if (NULL == buf) {
            fprintf(stderr, "File %s is not found!\n", fileName);
        }
        free(fileName);        
    } 
    
    // If the file was opened
    if (buf != NULL) {
        char *nums = strtok(buf," \n"); 
        float f[4][2];
        for (int i = 0; i < 4; i++) {
            for (int k = 0; k < 2; k++) {
                f[i][k] = atof(nums);
                nums = strtok(NULL, " \n");
            }            
        }

        for (int i = 0; i < NUM_OF_POINTS; i++) {
            ptr_plist[i]->x = f[i][0];
            ptr_plist[i]->y = f[i][1]; 
        }
        
        free(buf);
    } else {
        // If we dont use reading from file or file was not opened
        for (int i = 0; i < NUM_OF_POINTS; i++) {
            enter_points(ptr_plist[i]);
        }
    }

    for (int i = 0; i < NUM_OF_POINTS; i++) {
        
        bool res = check_point(ptr_plist[i], ptr_plist[set_index(i, 1)], 
            ptr_plist[set_index(i, 2)], ptr_plist[set_index(i, 3)]);

        if (res) {
                printf("Point %c:(%.2f %.2f) in %c%c%c:\n", 
                ptr_plist[set_index(i, 3)]->name, 
                ptr_plist[set_index(i, 3)]->x, 
                ptr_plist[set_index(i, 3)]->y, 
                ptr_plist[i]->name, 
                ptr_plist[set_index(i, 1)]->name, 
                ptr_plist[set_index(i, 2)]->name);
        } else {
            printf("Point %c:(%.2f %.2f) outside %c%c%c:\n", 
                ptr_plist[set_index(i, 3)]->name, 
                ptr_plist[set_index(i, 3)]->x, 
                ptr_plist[set_index(i, 3)]->y, 
                ptr_plist[i]->name, 
                ptr_plist[set_index(i, 1)]->name, 
                ptr_plist[set_index(i, 2)]->name);
        }
    }

    return 0;
}