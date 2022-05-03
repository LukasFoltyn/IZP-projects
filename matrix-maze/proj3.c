			//*****************************************************//
			//    		      Lukas Foltyn		       //
			//   IZP - 3.projekt - Prace s datovymi strukturami    //
			//		      9.12.2019                        //
			//                                                     //
			//*****************************************************//




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//macros for boarders
#define LEFT_B 2
#define RIGHT_B 1
#define LOWUP_B 0

//macros for command line arguments
#define HELP_ARG 2
#define HELP_STR "--help"
#define TEST_ARG 3
#define	TEST_STR "--test"
#define PATH_ARG 5
#define RPATH_STR "--rpath"
#define LPATH_STR "--lpath"
#define FUNCTION_IDX 1
#define FILE_TEST 2
#define	FILE_PATH 4
#define ROW 2
#define COL 3 

#define RPATH 0
#define LPATH 1
#define WALL 3
#define VALID 2

#define MAKE_NUM '0'
#define FIRST_ROWCOL 1
#define CLOSED_B '1'

//macros for changing directions
#define GO_UP 1
#define GO_DOWN 1
#define GO_LEFT 1
#define GO_RIGHT 1

//even row/odd column or odd row/even column
#define EO_OE ((!(c&1) && r&1) || (!(r&1) && (c&1)))
//even row/even column or odd row/odd column
#define EE_OO ((c&1 && r&1) || (!(c&1) && !(r&1)))
#define DIRECTIONS 4
#define LAST_DIR 3

#define BORDERS 3
#define TURN_TWO 2
#define	TURN_ONE 1
#define NUM_LENGHT 5 
typedef struct{
	int rows;
	int cols;
	unsigned char *cells;
	}map;

const char *const binary[]={"000", "001", "010", "011", "100", "101", "110", "111"};

//** Used functions **//
void print_instructions();
int maze_constractor(map *maze, char *filename);
void maze_deconstractor(map *maze);
bool border_match(map *maze);
bool isborder(map *maze, int r, int c, int border);
int start_border(map *maze, int r, int c, int leftright);
int check_start_border(map *maze, int r, int c);
int first_row(map *maze, int r, int c, int leftright);
int last_row(map *maze, int r, int c, int leftright);
int first_col(map *maze, int r, int c, int leftright);
int last_col_odd(map *maze, int r, int c, int leftright);
int last_col_even(map *maze, int r, int c, int leftright);
void move(map *maze, int r, int c, int starting_border, int leftright);
int get_new_border(int r, int c, int starting_border, int leftright);
int file_testing(char *filename, char *r, char *c);
int check_arg(char *string);

int main(int argc, char **argv)
{	
	int test;
	//printing instruction if command line has only two arguments
	if(argc == HELP_ARG && !(strcmp(argv[FUNCTION_IDX], HELP_STR)))
		print_instructions();
	//testing the file if command line has three arguments
	else if(argc == TEST_ARG && !(strcmp(argv[FUNCTION_IDX], TEST_STR)))
	{	
		test = file_testing(argv[FILE_TEST], NULL, NULL);
		if(!test)
			printf("Invalid\n");
		else if(test == VALID)
			printf("Valid\n");
		else
		{	
			fprintf(stderr, "Constractor or file opening failed!\n");
			return 1;
		}
	}
	//testing the file and also finding the path in the maze if possible
	else if(argc == PATH_ARG && ((!strcmp(argv[FUNCTION_IDX], RPATH_STR)) || !(strcmp(argv[FUNCTION_IDX], LPATH_STR))))
	{	 
		if(file_testing(argv[FILE_PATH], argv[ROW], argv[COL]) != VALID)
		{	
			fprintf(stderr, "Invalid\n");
			return 1;
		}	
		map maze;
		maze_constractor(&maze, argv[FILE_PATH]);
		//choosing which rule will be followed
		if(!strcmp(argv[FUNCTION_IDX], RPATH_STR))
		{
			//right hand rule
			int border = start_border(&maze, atoi(argv[ROW]),atoi(argv[COL]), RPATH);
			if(border == WALL || !check_start_border(&maze, atoi(argv[ROW]),atoi(argv[COL])))
			{	
				fprintf(stderr, "You can not enter at this position!\n");	
				maze_deconstractor(&maze);
				return 1;
			}
			move(&maze, atoi(argv[ROW]), atoi(argv[COL]), border, RPATH);
		}
		else
		{
			//lefthand rule
			int border = start_border(&maze, atoi(argv[ROW]),atoi(argv[COL]), LPATH);
			if(border == WALL)
			{
				fprintf(stderr,"You can not enter at this position!\n");	
				maze_deconstractor(&maze);
				return 1;
			}
			move(&maze, atoi(argv[ROW]), atoi(argv[COL]), border, LPATH);
		}
		maze_deconstractor(&maze);

	}
	//prtining to stderr if command line has wrong amount of arguments
	else
	{	fprintf(stderr, "Invalid arguments\n");
		return 1;
	}
	return 0;
}
//allocation of memory for the pointer in the sruct and filling it with data from file
int maze_constractor(map *maze, char *filename)
{
	FILE*filep;
	filep = fopen(filename, "r");
	if(filep != NULL)
	{
		fscanf(filep, "%d %d", &maze->rows, &maze->cols);
		maze->cells = malloc(maze->rows*maze->cols*sizeof(unsigned char));
		if(maze->cells != NULL)
		{
			int char_load;
			while((char_load = fgetc(filep)) != '\n'){}
			int i = 0;
			while((char_load = fgetc(filep)) != EOF)
			{
				if(!(i&1))
					maze->cells[i>>1] = char_load;
				i++;
			}
			fclose(filep);
			return 1;
		}
	}
	fclose(filep);
	return 0;	
}
//deallocation of memory
void maze_deconstractor(map *maze)
{
	free(maze->cells);
	maze->cells = NULL;
}
void print_instructions()
{
	printf("For testing your file if it's valid or not enter: --test name_of_your_file\n");
	printf("For finding the path in the matrix maze enter: --rpath/--lpath R C name_of_your_file\n");
	printf("R stands for a row and C for a column, where you wanna start at.\n");
}
//checking if the borders of single elements are matching
bool border_match(map *maze)
{
	
	if(maze->rows < 3 && maze->cols == 1)
		return true;
	//elements that are next to each other
	for(int i=0; i < maze->rows; i++)
	{
		for(int j=0; j < maze->cols-1; j++)
		{
			if(binary[maze->cells[i*maze->cols+j]-MAKE_NUM][RIGHT_B] != binary[maze->cells[i*maze->cols+j+1]-MAKE_NUM][LEFT_B])
				return false;
		}
	}
	//elements that are above each other
	if(maze->cols&1)
	{
		for(int i=1; i < (maze->rows-1)*maze->cols; i+=2)
		{
			if(binary[maze->cells[i]-MAKE_NUM][LOWUP_B] != binary[maze->cells[i+maze->cols]-MAKE_NUM][LOWUP_B])
				return false;
		}
	}
	else
	{
		for(int i=0; i < maze->rows-1; i++)
		{
			if((i&1))
			{
				for(int j=0; j < maze->cols; j+=2)
				{
					if(binary[maze->cells[i*maze->cols+j]-MAKE_NUM][LOWUP_B] != binary[maze->cells[i*maze->cols+j+maze->cols]-MAKE_NUM][LOWUP_B])
						return false;
				}
			}
			else
			{
				for(int l=1; l < maze->cols; l+=2)
				{	
					if(binary[maze->cells[i*maze->cols+l]-MAKE_NUM][LOWUP_B] != binary[maze->cells[i*maze->cols+l+maze->cols]-MAKE_NUM][LOWUP_B])
						return false;
				}
			}
		}
	}

	return true;
}
//deciding if the the border in maze is wall or not
bool isborder(map *maze, int r, int c, int border)
{
	if(binary[maze->cells[(r-1)*maze->cols+c-1]-MAKE_NUM][border] == CLOSED_B)
		return true;
	else
		return false;
}
//checking if it's possible to start at the position given by user
int check_start_border(map *maze, int r, int c)
{
	if(r != FIRST_ROWCOL && c !=  FIRST_ROWCOL && r != maze->rows && c != maze->cols)
		return 0;
	else if(r == FIRST_ROWCOL && !(c&1) && c > FIRST_ROWCOL && c < maze->cols)
		return 0;
	else if(r == maze->rows && (maze->rows&1) && c&1 && c > FIRST_ROWCOL && c < maze->cols)
		return 0;
	else if(r == maze->rows && !(maze->rows&1) && !(c&1) && c > FIRST_ROWCOL && c < maze->cols)
		return 0;
	else
		return 1;
}
//deciding at which border is the beggining of path
int start_border(map *maze, int r, int c, int leftright)
{	
	int st_bord;
	int sec_bord;
	//calling different fucntions depending on the satrting postion in the maze
	if(r == FIRST_ROWCOL && c != maze->cols)
	{	
		sec_bord = first_col(maze, r, c, leftright);
		st_bord = first_row(maze, r, c, leftright);
		if(st_bord != WALL)
			return st_bord;	
		else if(c == FIRST_ROWCOL && sec_bord != WALL)
			return sec_bord;
		else
			return WALL;
	}
	else if(r == maze->rows  && c != maze->cols && c != FIRST_ROWCOL)
	{
		return last_row(maze, r, c, leftright);
	}
	else if(c == FIRST_ROWCOL)
	{	
		sec_bord = last_row(maze, r, c, leftright);
		st_bord = first_col(maze, r, c, leftright);
		if(st_bord != WALL)
			return st_bord;
		else if(r == maze->rows && sec_bord != WALL)
			return sec_bord;
		else
			return WALL;
	}			
	else if(c == maze->cols && (c&1))
	{
		st_bord = last_col_odd(maze, r, c, leftright);
		if(st_bord != WALL)
			return st_bord;
		else if(r == FIRST_ROWCOL)
			return first_row(maze, r, c, leftright);
		else if(r == maze->rows)
			return last_row(maze, r, c, leftright);
		else
			return WALL;	
	}			
	else
	{	
		st_bord = last_col_even(maze, r,c ,leftright);
		if(st_bord != WALL)
			return st_bord;
		else if(r == maze->rows && (r&1))
			return last_row(maze, r, c, leftright);
		else
			return WALL;	
				
	}
}
//finding the border in first column
int first_col(map *maze, int r, int c, int leftright)
{
	if(isborder(maze, r,c, LEFT_B))
		return WALL;
	else if((leftright == RPATH && !(r&1)) || (leftright == LPATH && (r&1)))
		return LOWUP_B;
	else
		return RIGHT_B;

}
//finding the border in last column if columns are even
int last_col_even(map *maze, int r, int c, int leftright)
{
	if(isborder(maze, r,c, RIGHT_B))
		return WALL;
	else if((leftright == RPATH && !(r&1)) || (leftright == LPATH && (r&1)))
		return LOWUP_B;
	else
		return LEFT_B;
}
//finding the border in last column if columns are odd
int last_col_odd(map *maze, int r, int c, int leftright)
{	
	if(isborder(maze, r,c, RIGHT_B))
		return WALL;
	else if((leftright == RPATH && (r&1)) || (leftright == LPATH && !(r&1)))
		return LOWUP_B;
	else
		return LEFT_B;	
}
//finding the border in last row
int last_row(map *maze, int r, int c, int leftright)
{
	if(isborder(maze, r, c, LOWUP_B))
		return WALL;
	else if(leftright == RPATH)
		return RIGHT_B;
	else
		return LEFT_B;
}
//finding the border in first row
int first_row(map *maze, int r, int c, int leftright)
{	
	if(isborder(maze, r,c, LOWUP_B))
		return WALL;
	else if(leftright == RPATH)
		return LEFT_B;
	else
		return RIGHT_B;
}
//finding the path out of the maze
void move(map *maze, int r, int c, int starting_border, int leftright)
{
	struct directions{
		int r;
		int c;
	};
	struct directions dir[DIRECTIONS] = {{1,0}, {0,1}, {0,-1}, {-1,0}};
	//looping until we're out of the maze
	do{
		printf("%d, %d\n", r, c);
		//looping until one of the borders is open
		while(isborder(maze, r, c, starting_border))
		{
			if((EE_OO && leftright == RPATH) || (EO_OE && leftright == LPATH ))
				starting_border = (starting_border + TURN_TWO)%BORDERS;
			else
				starting_border = (starting_border + TURN_ONE)%BORDERS;
		}
		//moving left/right or up/down 
		if(EE_OO && starting_border == LOWUP_B)

			r += dir[starting_border+LAST_DIR].r;
		else
			r += dir[starting_border].r;
		
		c += dir[starting_border].c;

			starting_border = get_new_border(r, c, leftright, starting_border);

	

	}while(c <= maze->cols && c > 0 && r <= maze->rows && r > 0);
}
//fingind a new border depending on the taken direction and the rule that is followed
int get_new_border(int r, int c, int leftright, int starting_border)
{
	if(EE_OO)
	{
		if((starting_border == LOWUP_B && leftright == LPATH) || (starting_border == RIGHT_B && leftright == RPATH))
			return RIGHT_B;
		else if((starting_border == LOWUP_B && leftright == RPATH) || (starting_border == LEFT_B && leftright == LPATH))
			return LEFT_B;
		else
			return LOWUP_B;
	}
	else
	{	
		if((starting_border == LOWUP_B && leftright == RPATH) || (starting_border == RIGHT_B && leftright == LPATH))
			return RIGHT_B;
		else if((starting_border == LOWUP_B && leftright == LPATH) || (starting_border == LEFT_B && leftright == RPATH))
			return LEFT_B;
		else
			return LOWUP_B;
	}
}
//testing the file for invalid data
int file_testing(char *filename, char *r, char *c)
{
	int load_char;
	char strrow[NUM_LENGHT];
	char strcol[NUM_LENGHT];
	int i=0, rows=0, cols=0;
	FILE*filep;
	filep = fopen(filename, "r");
	if(filep)
	{	
		if((fscanf(filep,"%4s %4s", strrow, strcol)) != 2)
		{
			fclose(filep);
			return 0;
		}
		//checking the validation of arguments
		if(((r && c)  && (!check_arg(r) || !check_arg(c))) || !check_arg(strrow) || !check_arg(strcol)) 
		{	
			fclose(filep);
			return 0;
		}
		while((load_char = fgetc(filep)) != '\n'){}
		//counting the rows and columns with white spaces and newline character
		while((load_char = fgetc(filep)) != EOF)
		{
			if((!(i&1) && (load_char < '0'|| load_char > '9')) || ((i&1) && load_char != ' ' && load_char != '\n'))
			{
				fclose(filep);
				return 0;
			}
			else if(i&1 && load_char == ' ')
				cols++;
			else if(i&1)
				rows++;
			i++;
		}
	}
	else
		return 1;
	//checking the rows and the columns are matching with the ones given by user
	if(atoi(strrow) != rows || atoi(strcol) != cols/rows+1 || ((r &&c) && (atoi(r) < 1 || atoi(r) > rows || atoi(c) < 1 || atoi(c) > cols/rows+1)))
	{	
		fclose(filep);
		return 0;
	}
	//creating maze for checking the borders	
	map maze;
	if(maze_constractor(&maze, filename))
	{
		if(!border_match(&maze))
		{	
			maze_deconstractor(&maze);
			fclose(filep);
			return 0;
		}
	}		
	else	
	{	
		fclose(filep);
		return 1;
	}
	//returning 2 if the file's data are valid
	maze_deconstractor(&maze);
	fclose(filep);
	return 2;
}
//checking the characters different from nums
int check_arg(char *string)
{
	for(int i=0; string[i] != '\0'; i++)
	{
		if(i==0 && (string[i] < '1' || string[i] > '9'))
			return 0;
		else if(string[i] < '0' || string[i] > '9')
			return 0;
	}
	return 1;
}






























