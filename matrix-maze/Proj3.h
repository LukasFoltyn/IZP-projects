// hlavickovy soubor pro praci s bludistem
// ak. rok 2018/19

#ifndef PROJ3_H
#define PROJ3_H
/*!
\author Lukas Foltyn
\date 16.12.2019
*/
typedef struct {
    int rows;
    int cols;
    unsigned char *cells;
} Map;

/* bitove vahy jednotlivych hranic */
enum borders { BLEFT=0x1, BRIGHT=0x2, BTOP=0x4, BBOTTOM=0x4 };
/*! \param map The memory area that will be freed*/
void free_map(Map *map);

// vraci 0, kdyz ok
/*!
\param filename Name of the file to read from
\param map The memory area to write to
\return 0 if success
*/
int load_map(const char *filename, Map *map);

/*!
\param map The memory area to read from
\param r The number of row in matrix
\param c The number of column in matrix
\param border The number of the asked edge
\return true if the edge is wall
*/
bool isborder(Map *map, int r, int c, int border);

// zjistuje, jestli policko na dane souradnici ma spodni hranici
bool hasbottom(int r, int c);

int start_border(Map *map, int r, int c, int leftright);

int check_map(Map *map);

int load_and_check_map(const char *filename, Map *map);

// jsme za hranicemi?
bool is_out(Map *map, int r, int c);

void print_path(Map *map, int r, int c, int leftright);

#endif
