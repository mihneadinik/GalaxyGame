#include "Galaxy_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NMAX 100

int main()
{
    //data_planets *planets = planets_create();
    genlist *planets = planets_create(sizeof(planet_info));
    int m, ind, ind2, scouts_number, units, ind_scout, scout_value;
    char line[NMAX], *name;
    scanf("%d\n", &m);
    for (int i = 0; i < m; i++) {
        fgets(line, NMAX, stdin);
        line [strlen(line) - 1] = '\0';
        if (strncmp(line, "ADD", 3) == 0) {
            
            get_add(line, &ind, &scouts_number, &name);
            add_planet(planets, name, ind, scouts_number);
        }
        if (strncmp(line, "BLH", 3) == 0) {
            get_blh(line, &ind);
            blh_planet(planets, ind);
        }
        if (strncmp(line, "UPG", 3) == 0) {
            get_upg(line, &ind, &ind_scout, &scout_value);
            upg_scout(planets, ind, ind_scout, scout_value);
        }
        if (strncmp(line, "EXP", 3) == 0) {
            get_exp(line, &ind, &scout_value);
            exp_scout(planets, ind, scout_value);
        }
        if (strncmp(line, "RMV", 3) == 0) {
            get_rmv(line, &ind, &ind_scout);
            rmv_scout(planets, ind, ind_scout);
        }
        if (strncmp(line, "COL", 3) == 0) {
            get_col(line, &ind, &ind2);
            col_planet(planets, ind, ind2);
        }
        if (strncmp(line, "ROT", 3) == 0) {
            get_rot(line, &ind, &units);
            rot_planets(planets, ind, units);
        }
        if (strncmp(line, "SHW", 3) == 0) {
            get_shw(line, &ind);
            print_planets(planets, ind);
        }
    }
    big_free(&planets, 1);
    return 0;
}