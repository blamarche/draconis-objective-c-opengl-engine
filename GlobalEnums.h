

#define bool boolean
#define true YES
#define false NO

/* ENUMS */
enum AI_TYPE {
	AI_SKEET,
	AI_BIGENEMY,
	AI_GLOWSHIP,
	AI_FIGHTERENEMY,
	AI_SKEET_WAVE,
	AI_BOSS1,
	AI_BOSS2,
	AI_BOSS3,
	AI_BOSS4,
	AI_BOSS5,
	AI_BOSS6,
	AI_MAX
};

enum GAME_STATE  {
	GS_INIT,
	GS_MENU,
	GS_INGAME,
	GS_LOADING,
	GS_PAUSE
};

/* GLOBALS */
SDL_Surface			*screen;
NSAutoreleasePool	*autoPool;
unsigned long		keys[SDLK_LAST];

bool				gQuitGame = false;
int					gScreenWidth = 320;
int					gScreenHeight = 480;
int					gWindowWidth = 480;
int					gWindowHeight = 720;
int					gBackgroundHeight = 2048;
float				gPlayerShot_Speed=-600.0f;
double				gLastShotTime;
bool				gPerspectiveOn;
float				gFrustLeft,gFrustRight,gFrustBot,gFrustTop,gFrustNear,gFrustFar;

int					gMouseX, gMouseY;
bool				gMouseDown;
double				gAccelX, gAccelY, gAccelZ;

double				gTimeDiff;
double				gLastTick;
bool				gTickIsEven;
double				gEvenTickTimeDiff;
enum GAME_STATE		gGameState = GS_INIT;

CFont				*gFont;
CFont				*gFont2;
CAnimatedSprite		*player;
CAnimatedSprite		*background;
CAnimatedSprite		*pauseSprite;
CAnimatedSprite		*gameoverSprite;
CSprite				*gMenuSprite_Background;
CSprite				*gCreditSprite_Background;

CButtonManager		*gButtonManager;
NSMutableArray		*gRenderQueue;
NSMutableArray		*gPlayerShotList;
NSMutableArray		*gAIShotList;
NSMutableArray		*gAIList;
NSMutableArray		*gPowerupSPList;
NSMutableArray		*gPowerupFPList;

int					gMaxLayer=6;
int					gMinLayer=-1;


Mix_Chunk*			gSound_PlayerShot;
Mix_Chunk*			gSound_PlayerDie;
Mix_Chunk*			gSound_EnemyDie;
Mix_Chunk*			gSound_EnemyShot;


NSString*			gStatusString;
NSString*			gStatusString2;
float				gPlayerSpecialTimeLeft;
int					gPlayerSpecialAngle;
float				gPlayerSpecialStepLeft;
int					gPlayerScore;
int					gPlayerShotPower;
int					gPlayerLevel;
int					gPlayerLives;
int					gLevelAICount;
int					gLastBossLevel;
int					gHighScore;
float				gLevelMoveSpeed;
int					gEnemyKillPoints;
float				gDifficultyShotFreq;
NSString*			gDifficultyString;
