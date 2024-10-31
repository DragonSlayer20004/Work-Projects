/*Function Declaration */
int add(Movie* movies[]);
int readToNew(FILE* file, char* word);
void clear(Movie movies[]);
void deleteMovie(Movie* movies[]);
void find(Movie* movies[]);
void help();
void highest(Movie* movie[]);
void load(Movie* movies[]);
void quit(Movie* movieGenres[]);
void revenue(Movie* movieGenres[]);
void save(Movie* movieGenres[]);
void statistics(Movie* movieGenres[]);
int comparison(Movie* root, char* name, int year);
int genreNumber(char* genreTemp);
int loadMovie(FILE* file, Movie* movies[]);
int printMatches(Movie* root, char* name);
char toShorten(char* input);
int readToTab(FILE* file, char* word);
char* inputToTab();

/*External Declaration*/
extern const char* GENRE_NAMES[GENRES];
