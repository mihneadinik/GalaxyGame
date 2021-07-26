#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Galaxy_functions.h"
#include "utils.h"

void
add_scouts(genlist* list, int num_scouts)
{
    DIE(list == NULL, "Error alocating list of scouts");

    for (int i = 0; i < num_scouts; i++) {
        element *scout = malloc(sizeof(element));
        DIE(scout == NULL, "Error alocating scout");

        scout->data = malloc(list->data_size);
        DIE(scout->data == NULL, "Error alocating memory");
        int temp_val = 1;
        memcpy(scout->data, &temp_val, list->data_size);

        if (list->size == 0) {

            list->head = scout;
            scout->next = scout;
            scout->prev = scout;
            list->size ++;

        } else {

            scout->next = list->head;
            scout->prev = list->head->prev;
            list->head->prev->next = scout;
            list->head->prev = scout;
            list->size ++;

        }
    }
}

genlist* //apelat cu sizeof(planetsinfo)
planets_create(int data_size)
{
    genlist *list = malloc(sizeof(genlist));
    DIE(list == NULL, "Error alocating memory for planets");

    list->size = 0;
    list->head = NULL;
    list->data_size = data_size;
    return list;
}

genlist* //apelat cu sizeof(int)
scouts_create(int data_size, int num_scouts)
{
    genlist *list = malloc(sizeof(genlist));
    DIE(list == NULL, "Error alocating memory for scouts");

    list->size = 0;
    list->head = NULL;
    list->data_size = data_size;
    add_scouts(list, num_scouts);
    return list;
}

element*
get_planet(genlist *list, int ind)
{
    DIE(list == NULL, "Error");
    DIE(list->size == 0, "Error - no planets");

    element *curr = list->head;
    for (int i = 0; i < ind; i++)
        curr = curr->next;
    return curr;
}

void
add_planet(genlist* list, char *name, int ind, int num_scouts)
{
    DIE(list == NULL, "Error alocating list of planets");
    if (ind > list->size) {
        printf("Planet out of bounds!\n");
        return;
    }

    element *planet = malloc(sizeof(element));
    DIE(planet == NULL, "Error alocating planet");

    planet->data = malloc(list->data_size);
    DIE(planet->data == NULL, "Error alocating memory");
    genlist *list_of_scouts = scouts_create(sizeof(int), num_scouts);
    planet_info info;
    strcpy(info.name, name);
    info.kills = 0;
    info.scouts = list_of_scouts;
    memcpy(planet->data, &info, list->data_size);

    if (list->head == NULL) {

        list->head = planet;
        list->size ++;
        planet->next = planet;
        planet->prev = planet;
        printf("The planet %s has joined the galaxy.\n", name);
        return;

    }
    if (ind == 0) {

        planet->next = list->head;
        planet->prev = list->head->prev;
        list->head->prev->next = planet;
        list->head->prev = planet;
        list->head = planet;

    } else {

        element *curr = get_planet(list, ind);
        curr = curr->prev;
        curr->next->prev = planet;
        planet->next = curr->next;
        curr->next = planet;
        planet->prev = curr;

    }
    list->size ++;
    printf("The planet %s has joined the galaxy.\n", name);
}

void
print_scouts(genlist *list)
{
    DIE(list == NULL, "Error no list");
    element *curr = list->head;
    printf("SHIELDS: ");
    for (int i = 0; i < list->size; i++) {
        printf("%d ", *(int*)(curr->data));
        curr = curr->next;
    }
    printf("\n");
}

void
print_planet_info(planet_info *info, element *curr)
{
    printf("NAME: %s\n", info->name);
    printf("CLOSEST: ");

    if (curr->next == curr)
        printf("none\n");
    else
        if (curr->next == curr->prev)
            printf("%s\n", ((planet_info*)(curr->next)->data)->name);
        else
            printf("%s and %s\n", ((planet_info*)(curr->prev)->data)->name, ((planet_info*)(curr->next)->data)->name);

    print_scouts(info->scouts);
    printf("KILLED: %d\n", info->kills);
}

void
print_planets(genlist *list, int ind)
{
    DIE(list == NULL, "Error no list");
    if (ind >= list->size || list->size == 0) {
        printf("Planet out of bounds!\n");
        return;
    }
    element *curr = get_planet(list, ind);
    print_planet_info((planet_info*)(curr->data), curr);
}

void
rot_planets(genlist *list, int ind, int units)
{
    int way;
    DIE(list == NULL, "Error no list");
    if (ind >= list->size) {
        printf("Planet out of bounds!\n");
        return;
    }
    element *curr = get_planet(list, ind);
    if (units < 0) {
        way = -1;
        units = -units;
    } else {
        way = 1;
    }
    if (units >= ((planet_info*)(curr->data))->scouts->size)
        units %= ((planet_info*)(curr->data))->scouts->size;
    element *scout = ((planet_info*)(curr->data))->scouts->head;
    for (int i = 0; i < units; i++) {
        if (way == 1) {
            scout = scout->prev;
        } else {
            scout = scout->next;
        }
    }
    ((planet_info*)(curr->data))->scouts->head = scout;
}

element*
remove_node(genlist* list, int n)
{
    DIE(list == NULL, "Error no list");
    DIE(list->head == NULL, "Error no elements");

    element *out;

    if (list->size == 1) {
        out = list->head;
        list->head = NULL;
        list->size --;
        return out;
    }
    
    if (n == 0){
        out = list->head;
        list->head->prev->next = out->next;
        list->head->next->prev = out->prev;
        list->head = out->next;
    } else {
        if (n >= list->size / 2) {
            n = list->size - 1 - n;
            out = list->head->prev;

            for (int i = 0; i < n; i++) {
                out = out->prev;
            }
        } else {
            out = list->head;

            for (int i = 0; i < n; i++) {
                out = out->next;
            }
        }
        out->prev->next = out->next;
        out->next->prev = out->prev;
    }
    list->size --;
    return out;
}

void
rmv_scout(genlist *list, int ind, int ind_scout)
{
    DIE(list == NULL, "Error no list");
    if (ind >= list->size) {
        printf("Planet out of bounds!\n");
        return;
    }
    element *curr_planet = get_planet(list, ind);
    if (ind_scout >= ((planet_info*)(curr_planet->data))->scouts->size) {
        printf("Shield out of bounds!\n");
        return;
    }
    if (((planet_info*)(curr_planet->data))->scouts->size == 4) {
        printf("A planet cannot have less than 4 shields!\n");
        return;
    }
    element *out = remove_node(((planet_info*)(curr_planet->data))->scouts, ind_scout);
    free(out->data);
    free(out);
}

void
exp_scout(genlist *list, int ind, int scout_value)
{
    DIE(list == NULL, "Error no list");
    if (ind >= list->size) {
        printf("Planet out of bounds!\n");
        return;
    }

    element *curr_planet = get_planet(list, ind);
    genlist *scouts_list = ((planet_info*)(curr_planet->data))->scouts;
    element *new_scout = malloc(sizeof(element));
    DIE(new_scout == NULL, "Error alocating scout");

    new_scout->data = malloc(scouts_list->data_size);
    DIE(new_scout->data == NULL, "Error alocating memory");
    memcpy(new_scout->data, &scout_value, scouts_list->data_size);

    new_scout->next = scouts_list->head;
    new_scout->prev = scouts_list->head->prev;
    scouts_list->head->prev->next = new_scout;
    scouts_list->head->prev = new_scout;
    scouts_list->size ++;
}

void
upg_scout(genlist *list, int ind, int ind_scout, int scout_value)
{
    DIE(list == NULL, "Error no list");
    if (ind >= list->size) {
        printf("Planet out of bounds!\n");
        return;
    }

    element *curr_planet = get_planet(list, ind);
    genlist *scouts_list = ((planet_info*)(curr_planet->data))->scouts;
    if (ind_scout >= scouts_list->size) {
        printf("Shield out of bounds!\n");
        return;
    }

    element *curr_scout = get_planet(scouts_list, ind_scout);
    *(int *)(curr_scout)->data += scout_value;
}

void
blh_planet(genlist *list, int ind)
{
    DIE(list == NULL, "Error no list");
    if (ind >= list->size) {
        printf("Planet out of bounds!\n");
        return;
    }

    element *curr_planet = get_planet(list, ind);
    printf("The planet %s has been eaten by the vortex.\n", ((planet_info *)(curr_planet->data))->name);

    element *out = remove_node(list, ind);
    big_free(&((planet_info*)(out->data))->scouts, 0);
    free(out->data);
    free(out);
}

void
col_planet(genlist *list, int ind, int ind2)
{
    int ok = 0;

    if (ind >= list->size || ind2 >= list->size) {
        printf("Planet out of bounds!\n");
        return;
    }

    element *planet1, *planet2;
    planet1 = get_planet(list, ind);
    planet2 = planet1->next;

    int ind_scout1, ind_scout2;
    ind_scout1 = ((planet_info *)(planet1->data))->scouts->size / 4;
    ind_scout2 = (((planet_info *)(planet2->data))->scouts->size / 4) * 3;

    element *scout1, *scout2;
    scout1 = get_planet(((planet_info *)(planet1->data))->scouts, ind_scout1);
    scout2 = get_planet(((planet_info *)(planet2->data))->scouts, ind_scout2);

    *(int *)(scout1->data) = *(int *)(scout1->data) - 1;
    *(int *)(scout2->data) = *(int *)(scout2->data) - 1;

    if (*(int *)(scout1->data) < 0) {
        ((planet_info *)(planet2->data))->kills ++;
        printf("The planet %s has imploded.\n", ((planet_info *)(planet1->data))->name);

        element *out = remove_node(list, ind);
        big_free(&((planet_info*)(out->data))->scouts, 0);
        free(out->data);
        free(out);

        ok = 1;
    }

    if (*(int *)(scout2->data) < 0) {
        if (ok != 1)
            ((planet_info *)(planet1->data))->kills ++;
        else
            ind2 --;

        printf("The planet %s has imploded.\n", ((planet_info *)(planet2->data))->name);
        element *out2 = remove_node(list, ind2);
        big_free(&((planet_info*)(out2->data))->scouts, 0);
        free(out2->data);
        free(out2);
    }
}

void
get_add(char *line, int *ind, int *scouts_number, char **name)
{
    char *ind_string, *scouts_number_string;
    *name = strtok(line, " "); //parse the command
    *name = strtok(NULL, " "); //parse the name
    ind_string = strtok(NULL, " "); //parse the index
    scouts_number_string = strtok(NULL, " "); //parse the scouts number
    *ind = atoi(ind_string); //transform index to number
    *scouts_number = atoi(scouts_number_string); //transform scouts number to number
}

void
get_shw(char *line, int *ind)
{
    char *ind_string;
    ind_string = strtok(line, " "); //parse the command
    ind_string = strtok(NULL, " "); //parse the index
    *ind = atoi(ind_string); //transform index to number
}

void
get_rot(char *line, int *ind, int *units)
{
    char *p;
    int way;
    p = strtok(line, " "); //parse the command
    p = strtok(NULL, " "); //parse the index
    *ind = atoi(p); //transform index to number
    p = strtok(NULL, " "); //parse the way

    if (strncmp(p, "c", 1) == 0) //convention: clockwise = +units
        way = 1;  // counter clockwise = -units
    else 
        if (strncmp(p, "t", 1) == 0)
            way = -1;
        else {
            printf("Not a valid direction!\n");
            *units = 0;
            return;
        }

    p = strtok(NULL, " "); //parse the units
    *units = atoi(p); //transform units to number
    *units *= way;
}

void
get_rmv(char *line, int *ind, int *ind_scout)
{
    char *p;
    p = strtok(line, " "); //parse the command
    p = strtok(NULL, " "); //parse the index
    *ind = atoi(p); //transform index to number
    p = strtok(NULL, " "); //parse the scout's index
    *ind_scout = atoi(p); //transform scout's index to number
}

void
get_exp(char *line, int *ind, int *scout_value)
{
    char *p;
    p = strtok(line, " "); //parse the command
    p = strtok(NULL, " "); //parse the index
    *ind = atoi(p); //transform index to number
    p = strtok(NULL, " "); //parse the scout's value
    *scout_value = atoi(p); //transform scout's value to number
}

void
get_upg(char *line, int *ind, int *ind_scout, int *scout_value)
{
    char *p;
    p = strtok(line, " "); //parse the command
    p = strtok(NULL, " "); //parse the index
    *ind = atoi(p); //transform index to number
    p = strtok(NULL, " "); //parse the scout's index
    *ind_scout = atoi(p); //transform scout's index to number
    p = strtok(NULL, " "); //parse the scout's value
    *scout_value = atoi(p); //transform scout's value to number
}

void
get_blh(char *line, int *ind)
{
    char *p;
    p = strtok(line, " "); //parse the command
    p = strtok(NULL, " "); //parse the index
    *ind = atoi(p); //transform index to number
}

void
get_col(char *line, int *ind, int *ind2)
{
    char *p;
    p = strtok(line, " "); //parse the command
    p = strtok(NULL, " "); //parse the 1st index
    *ind = atoi(p); //transform 1st index to number
    p = strtok(NULL, " "); //parse the 2nd index
    *ind2 = atoi(p); //transform 2nd index to number
}

void
big_free(genlist **list, int ctrl)
{
    DIE(*list == NULL, "Error no list");

    if ((*list)->head == NULL) {
        free(*list);
        return;
    }

    element *curr = (*list)->head;
    curr->prev->next = NULL;
    if (curr->next != NULL) {
        while (curr != NULL) {
            element *next = curr->next;
            if (ctrl == 1)
                big_free(&((planet_info*)(curr->data))->scouts, 0);
            free(curr->data);
            free(curr);

            curr = next;
        }
    } else {
        if (ctrl == 1)
            big_free(&((planet_info*)(curr->data))->scouts, 0);
        free(curr->data);
        free(curr);
    }
    free(*list);
}