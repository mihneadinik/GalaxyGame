#ifndef __GALAXY_FUNCTIONS_H_
#define __GALAXY_FUNCTIONS_H_

//generic element (planet/scout)
typedef struct element element;
struct element
{
    element *next, *prev;
    void *data;
};

//the main list
typedef struct genlist genlist;
struct genlist
{
    int data_size;
    int size;
    element *head;
};

//the data for each planet
typedef struct planet_info planet_info;
struct planet_info
{
    char name[30];
    int kills;
    genlist *scouts;
};

void
add_scouts(genlist* list, int num_scouts);

genlist* //apelat cu sizeof(planet_info)
planets_create(int data_size);

genlist* //apelat cu sizeof(int)
scouts_create(int data_size, int num_scouts);

element*
get_planet(genlist *list, int ind);

void
add_planet(genlist* list, char *name, int ind, int num_scouts);

void
print_scouts(genlist *list);

void
print_planet_info(planet_info *info, element *curr);

void
print_planets(genlist *list, int ind);

void
rot_planets(genlist *list, int ind, int units);

element*
remove_node(genlist* list, int n);

void
rmv_scout(genlist *list, int ind, int ind_scout);

void
exp_scout(genlist *list, int ind, int scout_value);

void
upg_scout(genlist *list, int ind, int ind_scout, int scout_value);

void
blh_planet(genlist *list, int ind);

void
col_planet(genlist *list, int ind, int ind2);

void
get_add(char *line, int *ind, int *scouts_number, char **name);

void
get_shw(char *line, int *ind);

void
get_rot(char *line, int *ind, int *units);

void
get_rmv(char *line, int *ind, int *ind_scout);

void
get_exp(char *line, int *ind, int *scout_value);

void
get_upg(char *line, int *ind, int *ind_scout, int *scout_value);

void
get_blh(char *line, int *ind);

void
get_col(char *line, int *ind, int *ind2);

void
big_free(genlist **list, int ctrl);
#endif /* __GALAXY_FUNCTIONS_H_ */