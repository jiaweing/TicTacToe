int mainMenu();
int pvpMenu();
int difficultyMenu();
int drawMainMenu(SDL_Rect buttonRects[3]);
int drawPvPMenu(SDL_Rect buttonRects[3]);
int drawDifficultyMenu(SDL_Rect buttonRects[5]);
int askForHostPort(int *port);
int askForHostIP(char *ip);

#define PVP_GAME 1
#define PVAI_GAME 2
#define EXIT 0

#define ONLINE_GAME 1
#define OFFLINE_GAME 2
#define BACK 0

#define EASY_DIFFICULTY 1
#define MEDIUM_DIFFICULTY 2
#define HARD_DIFFICULTY 3
#define IMPOSSIBLE_DIFFICULTY 4
#define BACK 0