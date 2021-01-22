#include <stdio.h> 
#include <math.h> 
#include <stdbool.h>

#define NUM_OF_POINTS 4
#define set_index(i, num) \
    ((i+num) >= (NUM_OF_POINTS) ? ((i - NUM_OF_POINTS) + num) : (i+num))

struct point {

    char name;
    float x;
    float y;
};

struct triangle {

    float side1;
    float side2;
    float side3;
    float area;
};

typedef struct point point;
typedef struct triangle triangle;


void enter_points(point *current_point) 
{
    printf("Enter points coordinate for %c:\nx = ", current_point->name);
    scanf("%f", &current_point->x);
    printf("y = ");
    scanf("%f", &current_point->y);

    printf("%c: (x y) = (%.2f %.2f)\n", current_point->name, 
        current_point->x, current_point->y);

}


float set_side(point *p1, point *p2) 
{
    float length;
    float p1p2_x = p2->x - p1->x;
    float p1p2_y = p2->y - p1->y;

    length = sqrt(pow(p1p2_x, 2) + pow(p1p2_y, 2));

    return length;
}


void set_area(triangle *current_triangle) 
{
    float a = current_triangle->side1;
    float b = current_triangle->side2;
    float c = current_triangle->side3;
    
    // Semiperimeter
    float p = (a + b + c) * 0.5;

    current_triangle->area = sqrt(p * (p - a) * (p - b) * (p - c));
}


triangle new_triangle(point *p1, point *p2, point *p3) 
{
    triangle current_triangle;

    current_triangle.side1 = set_side(p1, p2);
    current_triangle.side2 = set_side(p2, p3);
    current_triangle.side3 = set_side(p3, p1);
    set_area(&current_triangle);

    return current_triangle;
}


bool check_point(point *tr_p1, point *tr_p2, point *tr_p3, point *ex_p)  
{
    triangle main_tr = new_triangle(tr_p1, tr_p2, tr_p3);
    triangle ex_tr1 = new_triangle(tr_p1, tr_p2, ex_p);
    triangle ex_tr2 = new_triangle(tr_p1, ex_p, tr_p3);
    triangle ex_tr3 = new_triangle(ex_p, tr_p2, tr_p3);

    float sum_of_areas = ex_tr1.area + ex_tr2.area + ex_tr3.area; 

    printf("Sum of areas: %f\nmain area: %f\n", sum_of_areas, main_tr.area);
    if (sum_of_areas <= main_tr.area) {
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

    point_a.name = 'A';
    point_b.name = 'B';
    point_c.name = 'C';
    point_d.name = 'D';
    
    enter_points(&point_a);
    enter_points(&point_b);
    enter_points(&point_c);
    enter_points(&point_d);

    point *ptr_plist[] = {&point_a, &point_b, &point_c, &point_d};

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
            printf("Point %c:(%.2f %.2f) out %c%c%c:\n", 
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



