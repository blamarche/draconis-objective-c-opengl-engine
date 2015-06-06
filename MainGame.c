
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <AudioToolbox/AudioToolbox.h>
#import "EAGLView.h"
#import "DraconisAppDelegate.h"
#import "RenderUtils.h"
#import "SoundUtils.h"
#import "CSprite.h"
#import "CAnimatedSprite.h"
#import "CFont.h"
#import "CButtonManager.h"
#import "GlobalEnums.h"
#import "TrigArrays.h"


#define RANDOM_SEED() srandom(time(NULL))
#define RANDOM_INT(__MIN__,__MAX__) ((__MIN__)+random() % ((__MAX__+1)-(__MIN__)))


/* FUNCTIONS */
void VibratePhone()
{
	AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}

CAnimatedSprite* SpawnAI(int aitype, NSString* animation, float x, float y, float vecx, float vecy)
{
	int hp=1;
	if (aitype==AI_SKEET||aitype==AI_SKEET_WAVE)
		hp=2;
	else if (aitype==AI_BIGENEMY)
		hp=7;
	else if (aitype==AI_GLOWSHIP)
		hp=2;
	else if (aitype==AI_BOSS1)
		hp=100;
	else if (aitype==AI_BOSS2)
		hp=125;
	else if (aitype==AI_BOSS3)
		hp=150;
	else if (aitype==AI_BOSS4)
		hp=200;
	else if (aitype==AI_BOSS5)
		hp=250;
	else
		hp=5;
	
	bool newai=true;
	for (int i=0; i<[gAIList count]; i++){	
		CAnimatedSprite* ai = (CAnimatedSprite*)[gAIList objectAtIndex:i];
		if (!ai.sprite.enabled)
		{
			//[ai.sprite.parameters setObject:[NSString stringWithFormat:@"%d",aitype] forKey:@"aitype"];
			//[ai.sprite.parameters setObject:[NSString stringWithFormat:@"%d",hp] forKey:@"hp"];
			[ai.sprite.parameters removeAllObjects];
			[ai.sprite setParamInt:@"aitype" :aitype];
			[ai.sprite setParamInt:@"hp" :hp];
			ai.sprite.enabled=true;
			ai.sprite.isdead=false;
			ai.sprite.x=x;
			ai.sprite.y=y;
			ai.sprite.vector_x=vecx;
			ai.sprite.vector_y=vecy;
			ai.sprite.angle=0;
			[ai setCurrentAnimation:animation :true];
			newai=false;
			[ai.sprite.parameters setObject:@"0" forKey:@"isboss"];
			return ai;
		}
	}
	if (newai)
	{
		CAnimatedSprite* ai = [[CAnimatedSprite alloc] initWithSprite:[[CSprite alloc] initWithFile:@"Sprite.png"]];
		if ([gAIList count]>0)
			[ai shareAnimationsWith:[gAIList objectAtIndex:0]];
		else
			[ai addAnimationsFromFile:@"Sprite.ani":true];
		
		[ai.sprite setParamInt:@"aitype" :aitype];
		[ai.sprite setParamInt:@"hp" :hp];
		ai.sprite.x=x;
		ai.sprite.y=y;
		ai.sprite.vector_x=vecx;
		ai.sprite.vector_y=vecy;
		ai.sprite.angle=0;
		ai.sprite.layer=2;
		ai.sprite.shadow_scale_x = 0.50;
		ai.sprite.shadow_scale_y = 0.50;
		ai.sprite.showShadow=true;
		ai.sprite.shadowAlpha = 255;
		[ai setCurrentAnimation:animation :true];
		[ai.sprite.parameters setObject:@"0" forKey:@"isboss"];
		
		[gRenderQueue addObject:ai];
		[gAIList addObject:ai];
		return ai;
	}
	return nil;
}

void SpawnAIType(int aitype, float x, float y)
{
	switch (aitype) {
		case AI_FIGHTERENEMY:
			SpawnAI(AI_FIGHTERENEMY, @"FighterEnemy", x, y, 0, RANDOM_INT(120,160));
			break;
		
		case AI_SKEET_WAVE:
			SpawnAI(AI_SKEET_WAVE, @"SkeetWave", RANDOM_INT(gScreenWidth/4,gScreenWidth-gScreenWidth/4), y, RANDOM_INT(0,50)-25, RANDOM_INT(50,100));
			break;
			
		case AI_SKEET:
			SpawnAI(AI_SKEET, @"Skeet", x, y, RANDOM_INT(0,50)-25, RANDOM_INT(50,100));
			break;
			
		case AI_BIGENEMY:
			SpawnAI(AI_BIGENEMY, @"BigEnemy", x, y, 0.0f, RANDOM_INT(25,50));
			break;
			
		case AI_GLOWSHIP:
			if (RANDOM_INT(0,1)==1) {
				CAnimatedSprite* ai = SpawnAI(AI_GLOWSHIP, @"GlowShip", 1, y, 50, 70);
				ai.frame = [((CAnimation*)[ai.animations objectForKey:[ai getCurrentAnimation]]).frames count]-1;
				ai.sprite.angle=-40;
			}
			else {
				CAnimatedSprite* ai = SpawnAI(AI_GLOWSHIP, @"GlowShip", gScreenWidth-1, y, -50, 70);
				ai.frame = [((CAnimation*)[ai.animations objectForKey:[ai getCurrentAnimation]]).frames count]-1;
				ai.sprite.angle=40;
			}
			break;
	}
}

void SpawnAIBoss()
{
	if (gPlayerLevel%5==1)
	{
		CAnimatedSprite* ai = SpawnAI(AI_BOSS1, @"Boss1", gScreenWidth/2, 0, 0, 25);
		[ai.sprite.parameters setObject:@"1" forKey:@"isboss"];
	}
	else if (gPlayerLevel%5==2)
	{
		CAnimatedSprite* ai = SpawnAI(AI_BOSS2, @"Boss2", gScreenWidth/2, 0, 0, 25);
		[ai.sprite.parameters setObject:@"1" forKey:@"isboss"];		
	}
	else if (gPlayerLevel%5==3)
	{
		CAnimatedSprite* ai = SpawnAI(AI_BOSS3, @"Boss3", gScreenWidth/2, 0, 0, 25);
		[ai.sprite.parameters setObject:@"1" forKey:@"isboss"];		
	}
	else if (gPlayerLevel%5==4)
	{
		CAnimatedSprite* ai = SpawnAI(AI_BOSS4, @"Boss4", gScreenWidth/2, 0, 0, 25);
		[ai.sprite.parameters setObject:@"1" forKey:@"isboss"];		
	}
	else if (gPlayerLevel%5==0)
	{
		CAnimatedSprite* ai = SpawnAI(AI_BOSS5, @"Boss5", gScreenWidth/2, 0, 0, 25);
		[ai.sprite.parameters setObject:@"1" forKey:@"isboss"];		
	}
	/*else if (gPlayerLevel%6==0)
	{
		CAnimatedSprite* ai = SpawnAI(AI_BOSS6, @"Boss2", gScreenWidth/2, 0, 0, 25);
		[ai.sprite.parameters setObject:@"1" forKey:@"isboss"];		
	}
	 */
}

int GetLevelAIType()
{
	switch (gPlayerLevel%5) 
	{
		case 1:
			switch (RANDOM_INT(0,1))
		{
			case 0:
				return AI_SKEET;
				break;
			case 1:
				return AI_SKEET_WAVE;
				break;
		}
			break;
		case 2:
			switch (RANDOM_INT(0,2))
		{
			case 0:
				return AI_SKEET;
				break;
			case 1:
				return AI_GLOWSHIP;
				break;
			case 2:
				return AI_SKEET_WAVE;
				break;
		}
			break;
		
		case 3:
			switch (RANDOM_INT(0,3))
		{
			case 0:
				return AI_SKEET;
				break;
			case 1:
				return AI_BIGENEMY;
				break;
			case 2:
				return AI_GLOWSHIP;
				break;
			case 3:
				return AI_SKEET_WAVE;
				break;
		}
			break;	
			
		default:
			switch (RANDOM_INT(0,4))
		{
			case 0:
				return AI_SKEET;
				break;
			case 1:
				return AI_BIGENEMY;
				break;
			case 2:
				return AI_GLOWSHIP;
				break;
			case 3:
				return AI_SKEET_WAVE;
				break;
			case 4:
				return AI_FIGHTERENEMY;
				break;
		}
			break;
	}
	return 0;
}

void ResetPlayer()
{
	player.sprite.x=gScreenWidth/2;
	player.sprite.y=gScreenHeight-100;
	player.sprite.showShadow=true;
	player.sprite.shadowAlpha=255;
	player.sprite.enabled=true;
	player.sprite.isdead=false;
	player.sprite.layer = 3;
	[player setCurrentAnimation:@"Player" :true];
	//[player.sprite.parameters setObject:[NSString stringWithFormat:@"%f",gLastTick] forKey:@"lastShotTime"];
	//[player.sprite.parameters setObject:[NSString stringWithFormat:@"%d",1] forKey:@"shotPower"];	
	[player.sprite setParamDouble:@"lastShotTime" :gLastTick];
	//[player.sprite setParamInt:@"shotPower" :1];
	gPlayerShotPower=1;
}

void ResetBackground()
{
	background.sprite.x=0;
	background.sprite.y=-1*gBackgroundHeight+gScreenHeight;
	background.sprite.z = 72.0f;
	background.sprite.showShadow=false;
	background.sprite.positionMode = 1; //topleft
	background.sprite.layer = 1;
}

void LoadGraphics() 
{
	//[[gButtonManager addButton:[[CSprite alloc] initWithFile:@"Sprite.png"] :@"test1" :100 :100 :50 :50 :1] setRenderArea:10 :10 :50 :50];
	//[[gButtonManager addButton:[[CSprite alloc] initWithFile:@"Sprite.png"] :@"test2" :200 :200 :50 :50 :2] setRenderArea:30 :30 :50 :50];
	
	gMenuSprite_Background = [[CSprite alloc] initWithFile:@"Menu.png"];
	gMenuSprite_Background.positionMode = 1; // topleft
	[gMenuSprite_Background setRenderArea:0 :0 :320 :480];
	
	gFont = [[CFont alloc] initWithFile:@"font-bold.png" : 32];
	[gFont setColorWithRed:255 :177 :0 :255];
	gFont.sprite.showShadow=false;
	//gFont.sprite.shadowOffsetX=2;
	//gFont.sprite.shadowOffsetY=1;
	//gFont.sprite.shadowAlpha=255;
	
	gFont2 = [[CFont alloc] initWithFile:@"font-normal.png" : 32];
	[gFont2 setColorWithRed:0 :0 :0 :255];
	
	background = [[CAnimatedSprite alloc] initWithSprite:[[CSprite alloc] initWithFile:@"Backgrounds.png"]];
	[background addAnimationsFromFile:@"Backgrounds.ani":true];
	ResetBackground();
	
	pauseSprite = [[CAnimatedSprite alloc] initWithSprite:[[CSprite alloc] initWithFile:@"Sprite.png"]];
	[pauseSprite addAnimationsFromFile:@"Sprite.ani":true];
	[pauseSprite setCurrentAnimation:@"Pause" :true];
	pauseSprite.sprite.x=gScreenWidth/2;
	pauseSprite.sprite.y=gScreenHeight/2;
	pauseSprite.sprite.showShadow=true;
	pauseSprite.sprite.layer=6;
	pauseSprite.sprite.enabled=false;
	
	gameoverSprite = [[CAnimatedSprite alloc] initWithSprite:[[CSprite alloc] initWithFile:@"Sprite.png"]];
	[gameoverSprite addAnimationsFromFile:@"Sprite.ani":true];
	[gameoverSprite setCurrentAnimation:@"Gameover" :false];
	gameoverSprite.sprite.x = gScreenWidth/2;
	gameoverSprite.sprite.y = gScreenHeight/2;
	
	player = [[CAnimatedSprite alloc] initWithSprite:[[CSprite alloc] initWithFile:@"Sprite.png"]];
	[player addAnimationsFromFile:@"Sprite.ani":true];
	player.sprite.shadow_scale_x = 0.50;
	player.sprite.shadow_scale_y = 0.50;
	ResetPlayer();
	
	[gRenderQueue addObject:background];
	[gRenderQueue addObject:player];
	[gRenderQueue addObject:pauseSprite];
}

void LoadSounds()
{	
	SoundEngine_Initialize(22100);
	SoundEngine_SetEffectsVolume(0.5f);
	SoundEngine_SetListenerPosition(0.0, 0.0, 0.0f);
	 
	gSound_PlayerShot = LoadSound(@"playershot.wav");
	gSound_EnemyShot = LoadSound(@"aishot.wav");
	gSound_EnemyDie = LoadSound(@"aideath.wav");
	gSound_PlayerDie = LoadSound(@"aideath.wav");
}

void LoadGameSettings()
{
	NSString *docDir = [(NSArray*)NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
	gHighScore=[[NSString stringWithContentsOfFile:[docDir stringByAppendingString:@"/draconis2d_highscore.txt"]] intValue];	
}

void SaveGameSettings()
{
	NSString *docDir = [(NSArray*)NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
	[[NSString stringWithFormat:@"%d",gHighScore] writeToFile:[docDir stringByAppendingString:@"/draconis2d_highscore.txt"] atomically:YES];
}

void InitGame(DraconisAppDelegate *application, EAGLView *view, bool firstTime)
{
	if (firstTime)
	{
		InitTrigArrays();
		gGameState = GS_LOADING;
	
		RANDOM_SEED();
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	
		glEnable(GL_TEXTURE_2D);	
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);	// Set a blending function to use
		glEnable(GL_BLEND);	
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			
		gApplication = application;
		gView = view;
		
		gPerspectiveOn=true;
	
		gRenderQueue = [[NSMutableArray alloc] initWithCapacity:70];	
		gPlayerShotList = [[NSMutableArray alloc] initWithCapacity:60];	 
		gAIShotList = [[NSMutableArray alloc] initWithCapacity:60];	 
		gAIList = [[NSMutableArray alloc] initWithCapacity:10];	 
		gPowerupFPList = [[NSMutableArray alloc] initWithCapacity:5];	 
		gPowerupSPList = [[NSMutableArray alloc] initWithCapacity:5];	 
		//gButtonManager = [[CButtonManager alloc] init];
	}
	
	gLastTick = CFAbsoluteTimeGetCurrent();
	gStatusString = @"";
	gStatusString2=@"";
	gPlayerScore=0;
	gPlayerLevel=1;
	gLevelAICount=3;
	gLastBossLevel=0;
	//gLevelMoveSpeed=2032.0f; 
	gLevelMoveSpeed=33.0f; 
	gEnemyKillPoints=100;
	gDifficultyShotFreq=1.0f;
	gDifficultyString=@"Normal";
	gPlayerLives=15;
	gPlayerShotPower=1;
	
	if (firstTime)
	{
		LoadGameSettings();
		
		LoadGraphics();
		LoadSounds();	
	}
	
	[background setCurrentAnimation:[NSString stringWithFormat:@"%d",gPlayerLevel] :true];
	[player.sprite setParamInt:@"specials" :1];
	player.sprite.isdead=false;
	player.sprite.enabled=true;
	player.sprite.y = 365; //menu pos
	player.sprite.x = 270;
	
	if (firstTime)
	{
		PreloadGameObjects();	
			
		//gl frustrum values
		float left,right,bot,top,near,far;
		near=0.01f;
		far=1000.0f;
		top = tan(3.1415f/2.0f * 0.5f)*near;
		bot = -top;
		left = 0.666667 * bot; //0.666666 == aspect ratio of iphone screen
		right = 0.666667 * top;
		gFrustLeft=left; gFrustRight=right;gFrustBot=bot;gFrustTop=top;gFrustNear=near;gFrustFar=far;	
	}	
	for (int i=0; i<gLevelAICount; i++)
		SpawnAIType(GetLevelAIType(),RANDOM_INT(20,gScreenWidth-20), 0);	
	
	gGameState = GS_MENU;	
}

void RenderQueue()
{
	//for (int l=gMinLayer; l<=gMaxLayer; l++)
		for (int i=0; i<[gRenderQueue count]; i++)
		{
			//if (l==[[gRenderQueue objectAtIndex:i] getLayer])
				[[gRenderQueue objectAtIndex:i] render:gPerspectiveOn];
		}
}

void DrawScreen()
{
	EAGLContext *context =  gView.context;
	//GLuint viewFramebuffer = gView.viewFramebuffer, viewRenderbuffer = gView.viewRenderbuffer;
	GLint backingWidth = gView.backingWidth, backingHeight = gView.backingHeight;
		
	//[EAGLContext setCurrentContext:context];
	
	//glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
	//glViewport(0, 0, backingWidth, backingHeight);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (gPerspectiveOn)
	{		
		glFrustumf(gFrustLeft, gFrustRight, gFrustBot, gFrustTop, gFrustNear, gFrustFar);
		glRotatef(70.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(-gScreenWidth/2,-215,-gScreenHeight*0.85);
	}
	else
		glOrthof(0.0f, backingWidth, backingHeight, 0.0f, -1.0f, 1.0f);	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	
	//RENDER NOW
	RenderQueue();
	
	//set ortho
	if (gPerspectiveOn)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();	
		glOrthof(0.0f, backingWidth, backingHeight, 0.0f, -1.0f, 1.0f);	
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	
	[gFont renderStringAt:5 :5 :gStatusString :10 : 0];
	if (gPlayerLives<=0){
		[gameoverSprite render:false];
		[gFont renderStringAt:15 :gScreenHeight-210 :@"  Tap the screen to try again" :9 :0];
	}
	[gFont renderStringAt:2 :gScreenHeight-20 :gStatusString2 :10 : 128];
	//END RENDER
	
	//glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	
	[context presentRenderbuffer:GL_RENDERBUFFER_OES];	
}

void AnimateQueue(double currentTime)
{	
	for (int i=0; i<[gRenderQueue count]; i++)		
		if ([[gRenderQueue objectAtIndex:i] animate:currentTime :false] && [[gRenderQueue objectAtIndex:i] getIsDead] )
			[[gRenderQueue objectAtIndex:i] setIsEnabled:false];
}

void AnimateGame(double currentTime)
{
	AnimateQueue(currentTime);
	[gameoverSprite animate:currentTime :false];
	
	//scroll background
	if (gGameState==GS_INGAME && background.sprite.y < 0){
		background.sprite.y += (float)gTimeDiff * gLevelMoveSpeed; //Y speed
		if (background.sprite.y>0)
			background.sprite.y=0;
	}
}

CAnimatedSprite* AddPlayerShot(float x,float y,float vector_x, float vector_y)
{
	CAnimatedSprite* shot = [[CAnimatedSprite alloc] initWithSprite:[[CSprite alloc] initWithFile:@"Sprite.png"]];
	if ([gPlayerShotList count]>0)
		[shot shareAnimationsWith:[gPlayerShotList objectAtIndex:0]];
	else
		[shot addAnimationsFromFile:@"Sprite.ani":true];
	
	shot.sprite.x=x;
	shot.sprite.y=y-10;
	shot.sprite.vector_x=vector_x;
	shot.sprite.vector_y=vector_y;
	shot.sprite.layer=2;
	[shot setCurrentAnimation:@"PlayerShot" :false];

	[gRenderQueue addObject:shot];
	[gPlayerShotList addObject:shot];
	return shot;
}

void PlayerShootSpecial(float vecx, float vecy, int angle)
{
	bool newshot=true;
	for (int i=0; i<[gPlayerShotList count]; i++){	
		CAnimatedSprite* shot = (CAnimatedSprite*)[gPlayerShotList objectAtIndex:i];
		if (!shot.sprite.enabled)
		{
			shot.sprite.enabled=true;
			shot.sprite.isdead=false;
			shot.sprite.x=player.sprite.x;
			shot.sprite.y=player.sprite.y;					
			shot.sprite.vector_x=vecx;
			shot.sprite.vector_y=vecy;
			shot.sprite.angle = angle; //angle*3.14f;
			[shot setCurrentAnimation:@"PlayerShot" :false];
			newshot=false;
			break;
		}
	}
	if (newshot)
		AddPlayerShot(player.sprite.x,player.sprite.y,vecx,vecy);
}

void PlayerShoot()
{
	if (gPlayerSpecialTimeLeft > 0.0f)
		return;
		
	int shotPower = gPlayerShotPower;//[player.sprite getParamInt:@"shotPower"];//[[player.sprite.parameters objectForKey:@"shotPower"] intValue];
	bool newshot=true;
	
	switch (shotPower)
	{
		case 1:
			newshot=true;
			for (int i=0; i<[gPlayerShotList count]; i++){	
				CAnimatedSprite* shot = (CAnimatedSprite*)[gPlayerShotList objectAtIndex:i];
				if (!shot.sprite.enabled)
				{
					shot.sprite.enabled=true;
					shot.sprite.isdead=false;
					shot.sprite.x=player.sprite.x;
					shot.sprite.y=player.sprite.y;					
					shot.sprite.vector_x=0.0f;
					shot.sprite.vector_y=gPlayerShot_Speed;
					shot.sprite.angle = 0;
					[shot.sprite setScale:1.0f :1.0f];
					[shot setCurrentAnimation:@"PlayerShot" :false];
					newshot=false;
					break;
				}
			}
			if (newshot)
				AddPlayerShot(player.sprite.x,player.sprite.y,0.0f,gPlayerShot_Speed);
			break;
			
		case 2:
			for (int shotIndex=0; shotIndex<2; shotIndex++)
			{
				float vecx, vecy;
				int angle;
				if (shotIndex==0){
					vecx=80.0f;
					vecy=gPlayerShot_Speed+65.0f;
					angle=9;
				}else{
					vecx=-80.0f;
					vecy=gPlayerShot_Speed+65.0f;	
					angle=-9;
				}					
				
				bool newshot=true;
				for (int i=0; i<[gPlayerShotList count]; i++){	
					CAnimatedSprite* shot = (CAnimatedSprite*)[gPlayerShotList objectAtIndex:i];
					if (!shot.sprite.enabled)
					{
						shot.sprite.isdead=false;
						shot.sprite.enabled=true;
						shot.sprite.x=player.sprite.x;
						shot.sprite.y=player.sprite.y;											
						shot.sprite.vector_x=vecx;
						shot.sprite.vector_y=vecy;
						shot.sprite.angle = angle;
						[shot.sprite setScale:1.0f :1.0f];
						[shot setCurrentAnimation:@"PlayerShot" :false];
						newshot=false;
						break;
					}
				}
				if (newshot)
					AddPlayerShot(player.sprite.x,player.sprite.y,vecx,vecy);
			}
			break;

		case 3:
			for (int shotIndex=0; shotIndex<3; shotIndex++)
			{
				float vecx, vecy;
				int angle;
				if (shotIndex==0){
					angle=13;
					vecx=110.0f;
					vecy=gPlayerShot_Speed+110.0f;
				}else if (shotIndex==1){
					angle=-13;
					vecx=-110.0f;
					vecy=gPlayerShot_Speed+110.0f;	
				}else{
					angle=0;
					vecx=0.0f;
					vecy=gPlayerShot_Speed;	
				}
				
				bool newshot=true;
				for (int i=0; i<[gPlayerShotList count]; i++){	
					CAnimatedSprite* shot = (CAnimatedSprite*)[gPlayerShotList objectAtIndex:i];
					if (!shot.sprite.enabled)
					{
						shot.sprite.isdead=false;
						shot.sprite.enabled=true;
						shot.sprite.x=player.sprite.x;
						shot.sprite.y=player.sprite.y;											
						shot.sprite.vector_x=vecx;
						shot.sprite.vector_y=vecy;
						shot.sprite.angle = angle;
						[shot.sprite setScale:1.0f :1.0f];
						[shot setCurrentAnimation:@"PlayerShot" :false];
						newshot=false;
						break;
					}
				}
				if (newshot)
					AddPlayerShot(player.sprite.x,player.sprite.y,vecx,vecy);
			}
			break;
	}
}

void DropFP(CAnimatedSprite* ai, float vecx, float vecy)
{
	bool newp=true;	
	for (int i=0; i<[gPowerupFPList count]; i++){	
		CAnimatedSprite* pow = (CAnimatedSprite*)[gPowerupFPList objectAtIndex:i];
		if (!pow.sprite.enabled)
		{
			pow.sprite.enabled=true;
			pow.sprite.x=ai.sprite.x;
			pow.sprite.y=ai.sprite.y;		
			pow.sprite.vector_x=vecx;
			pow.sprite.vector_y=vecy;
			pow.sprite.angle = 0;
			[pow setCurrentAnimation:@"FirePowerup" :true];
			newp=false;
			break;
		}
	}
	if (newp)
	{
		CAnimatedSprite* pow = [[CAnimatedSprite alloc] initWithSprite:[[CSprite alloc] initWithFile:@"Sprite.png"]];
		if ([gPowerupFPList count]>0)
			[pow shareAnimationsWith:[gPowerupFPList objectAtIndex:0]];
		else
			[pow addAnimationsFromFile:@"Sprite.ani":true];
		pow.sprite.x=ai.sprite.x;
		pow.sprite.y=ai.sprite.y;		
		pow.sprite.vector_x=vecx;
		pow.sprite.vector_y=vecy;
		pow.sprite.layer = 5;
		[pow setCurrentAnimation:@"FirePowerup" :true];
		
		[gRenderQueue addObject:pow];
		[gPowerupFPList addObject:pow];
	}
}			

void DropSP(CAnimatedSprite* ai, float vecx, float vecy)
{
	bool newp=true;	
	for (int i=0; i<[gPowerupSPList count]; i++){	
		CAnimatedSprite* pow = (CAnimatedSprite*)[gPowerupSPList objectAtIndex:i];
		if (!pow.sprite.enabled)
		{
			pow.sprite.enabled=true;
			pow.sprite.x=ai.sprite.x;
			pow.sprite.y=ai.sprite.y;		
			pow.sprite.vector_x=vecx;
			pow.sprite.vector_y=vecy;
			pow.sprite.angle = 0;
			[pow setCurrentAnimation:@"SpecialPowerup" :true];
			newp=false;
			break;
		}
	}
	if (newp)
	{
		CAnimatedSprite* pow = [[CAnimatedSprite alloc] initWithSprite:[[CSprite alloc] initWithFile:@"Sprite.png"]];
		if ([gPowerupSPList count]>0)
			[pow shareAnimationsWith:[gPowerupSPList objectAtIndex:0]];
		else
			[pow addAnimationsFromFile:@"Sprite.ani":true];
		pow.sprite.x=ai.sprite.x;
		pow.sprite.y=ai.sprite.y;		
		pow.sprite.vector_x=vecx;
		pow.sprite.vector_y=vecy;
		pow.sprite.layer = 5;
		[pow setCurrentAnimation:@"SpecialPowerup" :true];
		
		[gRenderQueue addObject:pow];
		[gPowerupSPList addObject:pow];
	}
}			

void DisableAllSprites()
{
	for (int i=0; i<[gAIList count]; i++){	
		CAnimatedSprite* ai = (CAnimatedSprite*)[gAIList objectAtIndex:i];
		if (ai.sprite.enabled)
		{	
			ai.sprite.enabled=false;
		}
	}
	
	for (int i=0; i<[gAIShotList count]; i++){			//ai shots to player
		CAnimatedSprite* shot = (CAnimatedSprite*)[gAIShotList objectAtIndex:i];
		if (shot.sprite.enabled)
		{
			shot.sprite.enabled=false;
		}
	}
	
	for (int i=0; i<[gPlayerShotList count]; i++){			//ai shots to player
		CAnimatedSprite* shot = (CAnimatedSprite*)[gPlayerShotList objectAtIndex:i];
		if (shot.sprite.enabled)
		{
			shot.sprite.enabled=false;
		}
	}
}

void KillAllAI()
{
	for (int i=0; i<[gAIList count]; i++){	
		CAnimatedSprite* ai = (CAnimatedSprite*)[gAIList objectAtIndex:i];
		if (ai.sprite.enabled)
		{	
			if (!ai.sprite.isdead)
			{
				ai.sprite.isdead=true;
				[ai setCurrentAnimation:@"Die" :false];	
				AIDied(ai,false);
			}
		}
	}
	
	for (int i=0; i<[gAIShotList count]; i++){			//ai shots to player
		CAnimatedSprite* shot = (CAnimatedSprite*)[gAIShotList objectAtIndex:i];
		if (shot.sprite.enabled)
		{
			shot.sprite.isdead=true;
			[shot setCurrentAnimation:@"ShotDie" :false];
		}
	}
	
	for (int i=0; i<[gPlayerShotList count]; i++){			//ai shots to player
		CAnimatedSprite* shot = (CAnimatedSprite*)[gPlayerShotList objectAtIndex:i];
		if (shot.sprite.enabled)
		{
			shot.sprite.isdead=true;
			[shot setCurrentAnimation:@"ShotDie" :false];
		}
	}
}

CAnimatedSprite* AIShoot(CAnimatedSprite* ai, NSString* shotAnim, float vecx, float vecy, bool playsfx)
{
	if (playsfx)
		PlaySound(gSound_EnemyShot,false);
	
	bool newshot=true;	
	newshot=true;
	for (int i=0; i<[gAIShotList count]; i++){	
		CAnimatedSprite* shot = (CAnimatedSprite*)[gAIShotList objectAtIndex:i];
		if (!shot.sprite.enabled)
		{
			shot.sprite.isdead=false;
			shot.sprite.enabled=true;
			shot.sprite.x=ai.sprite.x;
			shot.sprite.y=ai.sprite.y;		
			shot.sprite.vector_x=vecx;
			shot.sprite.vector_y=vecy;
			shot.sprite.angle = 0;
			[shot.sprite setScale:1.0f :1.0f];
			[shot setCurrentAnimation:shotAnim :false];
			newshot=false;
			return shot;
			break;
		}
	}
	if (newshot)
	{
		CAnimatedSprite* shot = [[CAnimatedSprite alloc] initWithSprite:[[CSprite alloc] initWithFile:@"Sprite.png"]];
		if ([gAIShotList count]>0)
			[shot shareAnimationsWith:[gAIShotList objectAtIndex:0]];
		else
			[shot addAnimationsFromFile:@"Sprite.ani":true];
		shot.sprite.x=ai.sprite.x;
		shot.sprite.y=ai.sprite.y;
		shot.sprite.vector_x=vecx;
		shot.sprite.vector_y=vecy;
		shot.sprite.layer=4;
		[shot setCurrentAnimation:shotAnim :false];
		
		[gRenderQueue addObject:shot];
		[gAIShotList addObject:shot];
		return shot;
	}
}			

void AIBoss5(CAnimatedSprite* ai)
{
	//init settings
	if ([ai.sprite getParamInt:@"ai_isset"]==0)
	{
		[ai.sprite setParamInt:@"ai_specialangle": 0];
		[ai.sprite setParamDouble:@"ai_specialstepleft" :0.15];
		[ai.sprite setParamInt:@"ai_isset": 1];
		[ai.sprite setParamInt:@"ai_specialdir": 1];
		[ai.sprite setParamInt:@"ai_direction": -1];
	}
	
	//shoot
	if (RANDOM_INT(0,(int)(20*gDifficultyShotFreq))==1)
		AIShoot(ai, @"AIShot1", RANDOM_INT(0,100)-50, RANDOM_INT(100,200),true);		
	
	//direction
	if (ai.sprite.y > 100)
	{
		ai.sprite.vector_y=0.0f;
		ai.sprite.vector_x=[ai.sprite getParamInt:@"ai_direction"]*25.0f;
		if (ai.sprite.x < 100){
			[ai.sprite setParamInt:@"ai_direction": 1];
			ai.sprite.x=101;
		}else if (ai.sprite.x > gScreenWidth-100){
			[ai.sprite setParamInt:@"ai_direction": -1];
			ai.sprite.x=gScreenWidth-101;
		}		
	}
	
	//now do special
	double stepleft = [ai.sprite getParamDouble:@"ai_specialstepleft"]-gEvenTickTimeDiff;
	if (stepleft<=0.0)
	{
		int angle = [ai.sprite getParamInt:@"ai_specialangle"];
		
		if (angle >= 50)
			[ai.sprite setParamInt:@"ai_specialdir": 0];
		else if (angle<=0)
			[ai.sprite setParamInt:@"ai_specialdir": 1];
		
		stepleft=0.15;
		
		float vx,vy;
		vx=__fsin[WrapDegrees(angle)]*200.0f;
		vy=__fcos[WrapDegrees(angle)]*200.0f;
		AIShoot(ai, @"AIShot2", vx,vy,true);	
		vx=__fsin[WrapDegrees(-angle)]*200.0f;
		vy=__fcos[WrapDegrees(-angle)]*200.0f;		
		AIShoot(ai, @"AIShot2", vx,vy,false);	
		if ([ai.sprite getParamInt:@"ai_specialdir"]==1)
			[ai.sprite setParamInt:@"ai_specialangle" :angle+6*gDifficultyShotFreq];
		else
			[ai.sprite setParamInt:@"ai_specialangle" :angle-6*gDifficultyShotFreq];
	}					
	[ai.sprite setParamDouble:@"ai_specialstepleft" :stepleft];
}

void AIBoss2(CAnimatedSprite* ai)
{
	//init settings
	if ([ai.sprite getParamInt:@"ai_isset"]==0)
	{
		[ai.sprite setParamInt:@"ai_specialangle": 0];
		[ai.sprite setParamDouble:@"ai_specialstepleft" :0.15];
		[ai.sprite setParamInt:@"ai_isset": 1];
		[ai.sprite setParamInt:@"ai_specialdir": 1];
		[ai.sprite setParamInt:@"ai_direction": -1];
	}
	
	//shoot
	if (RANDOM_INT(0,(int)(75*gDifficultyShotFreq))==1)
		AIShoot(ai, @"AIShot2", RANDOM_INT(0,100)-50, RANDOM_INT(100,200),true);		
	
	//direction
	if (ai.sprite.y > 100)
	{
		ai.sprite.vector_y=0.0f;
		ai.sprite.vector_x=[ai.sprite getParamInt:@"ai_direction"]*25.0f;
		if (ai.sprite.x < 100){
			[ai.sprite setParamInt:@"ai_direction": 1];
			ai.sprite.x=101;
		}else if (ai.sprite.x > gScreenWidth-100){
			[ai.sprite setParamInt:@"ai_direction": -1];
			ai.sprite.x=gScreenWidth-101;
		}		
	}
	
	//now do special
	double stepleft = [ai.sprite getParamDouble:@"ai_specialstepleft"]-gEvenTickTimeDiff;
	if (stepleft<=0.0)
	{
		int angle = [ai.sprite getParamInt:@"ai_specialangle"];
		
		if (angle >= 30)
			[ai.sprite setParamInt:@"ai_specialdir": 0];
		else if (angle<=-30)
			[ai.sprite setParamInt:@"ai_specialdir": 1];
			
		stepleft=0.15;
		
		float vx,vy;
		vx=__fsin[WrapDegrees(angle-15)]*200.0f;
		vy=__fcos[WrapDegrees(angle-15)]*200.0f;
		AIShoot(ai, @"AIShot1", vx,vy,true);	
		vx=__fsin[WrapDegrees(angle+15)]*200.0f;
		vy=__fcos[WrapDegrees(angle+15)]*200.0f;		
		AIShoot(ai, @"AIShot1", vx,vy,false);	
		if ([ai.sprite getParamInt:@"ai_specialdir"]==1)
			[ai.sprite setParamInt:@"ai_specialangle" :angle+3*gDifficultyShotFreq];
		else
			[ai.sprite setParamInt:@"ai_specialangle" :angle-3*gDifficultyShotFreq];
	}					
	[ai.sprite setParamDouble:@"ai_specialstepleft" :stepleft];
	[ai.sprite setParamDouble:@"ai_specialdur" :[ai.sprite getParamDouble:@"ai_specialdur"]-0.1];
}

void AIBoss4(CAnimatedSprite* ai)
{
	//init settings
	if ([ai.sprite getParamInt:@"ai_isset"]==0)
	{
		[ai.sprite setParamDouble:@"ai_specialdur" :2.0];
		[ai.sprite setParamInt:@"ai_specialdir" :1];
		[ai.sprite setParamInt:@"ai_specialangle": 0];
		[ai.sprite setParamDouble:@"ai_specialstepleft" :0.05];
		[ai.sprite setParamInt:@"ai_specialangle2": 0];
		[ai.sprite setParamDouble:@"ai_specialstepleft2" :0.2];
		[ai.sprite setParamInt:@"ai_isset": 1];					
		[ai.sprite setParamInt:@"ai_direction": 1];
	}
	
	//shoot
	if (RANDOM_INT(0,(int)(65*gDifficultyShotFreq))==1) {
		AIShoot(ai, @"AIShot2", RANDOM_INT(0,100)-50, RANDOM_INT(150,250),true);		
		AIShoot(ai, @"AIShot2", RANDOM_INT(0,100)-50, RANDOM_INT(150,250),false);		
	}
	
	//direction
	if (ai.sprite.y > 100)
	{
		ai.sprite.vector_y=0.0f;
		ai.sprite.vector_x=[ai.sprite getParamInt:@"ai_direction"]*25.0f;
		if (ai.sprite.x < 50){
			[ai.sprite setParamInt:@"ai_direction": 1];
			ai.sprite.x=51;
		}else if (ai.sprite.x > gScreenWidth-50){
			[ai.sprite setParamInt:@"ai_direction": -1];
			ai.sprite.x=gScreenWidth-51;
		}						
	}
	
	//now do special
	double stepleft2 = [ai.sprite getParamDouble:@"ai_specialstepleft2"]-gEvenTickTimeDiff;
	if (stepleft2<=0.0)
	{
		int angle2 = [ai.sprite getParamInt:@"ai_specialangle2"];
		
		if (angle2 >= 30)
			[ai.sprite setParamInt:@"ai_specialdir2": 0];
		else if (angle2<=-30)
			[ai.sprite setParamInt:@"ai_specialdir2": 1];
		
		stepleft2=0.15;
		
		float vx2,vy2;
		vx2=__fsin[WrapDegrees(angle2-15)]*200.0f;
		vy2=__fcos[WrapDegrees(angle2-15)]*200.0f;
		AIShoot(ai, @"AIShot1", vx2,vy2,true);	
		vx2=__fsin[WrapDegrees(angle2+15)]*200.0f;
		vy2=__fcos[WrapDegrees(angle2+15)]*200.0f;		
		AIShoot(ai, @"AIShot1", vx2,vy2,false);	
		if ([ai.sprite getParamInt:@"ai_specialdir2"]==1)
			[ai.sprite setParamInt:@"ai_specialangle2" :angle2+3*gDifficultyShotFreq];
		else
			[ai.sprite setParamInt:@"ai_specialangle2" :angle2-3*gDifficultyShotFreq];
	}					
	[ai.sprite setParamDouble:@"ai_specialstepleft2" :stepleft2];
	
	//now do special
	if ([ai.sprite getParamDouble:@"ai_specialdur"]>0.0)
	{					
		double stepleft = [ai.sprite getParamDouble:@"ai_specialstepleft"]-gEvenTickTimeDiff;
		if (stepleft<=0.0)
		{
			int angle = [ai.sprite getParamInt:@"ai_specialangle"];
			
			stepleft=0.05;
			
			float vx,vy;
			
			if (RANDOM_INT(0,1)==1)
			{
				vy = 1.0f - (__fsin[WrapDegrees(angle)]);
				vx = 1.0f-vy;
				vy = vy*150.0f;
				vx = vx * 150.0f;
			}
			else
			{
				vx=__fsin[WrapDegrees(angle)]*150.0f;
				vy=__fcos[WrapDegrees(angle)]*150.0f;
			}
			
			AIShoot(ai, @"AIShot1", vx,vy,true);	
			AIShoot(ai, @"AIShot1", -vx,vy,false);	
			[ai.sprite setParamInt:@"ai_specialangle" :angle+(14*gDifficultyShotFreq)];
		}					
		[ai.sprite setParamDouble:@"ai_specialstepleft" :stepleft];
		[ai.sprite setParamDouble:@"ai_specialdur" :[ai.sprite getParamDouble:@"ai_specialdur"]-0.05];
	}	
	else
	{
		[ai.sprite setParamDouble:@"ai_specialdur" :[ai.sprite getParamDouble:@"ai_specialdur"]-gEvenTickTimeDiff];
		if ([ai.sprite getParamDouble:@"ai_specialdur"] < -7.0*gDifficultyShotFreq)
		{
			[ai.sprite setParamDouble:@"ai_specialdur" :2.0];
			[ai.sprite setParamInt:@"ai_specialangle": 0];
		}
	}
}

void AIBoss3(CAnimatedSprite* ai)
{	
	//init settings
	if ([ai.sprite getParamInt:@"ai_isset"]==0)
	{
		[ai.sprite setParamDouble:@"ai_specialdur" :2.0];
		[ai.sprite setParamInt:@"ai_specialangle": 0];
		[ai.sprite setParamDouble:@"ai_specialstepleft" :0.05];
		[ai.sprite setParamInt:@"ai_isset": 1];					
		[ai.sprite setParamInt:@"ai_direction": 1];
	}
	
	//shoot
	if (RANDOM_INT(0,(int)(65*gDifficultyShotFreq))==1)
		AIShoot(ai, @"AIShot2", RANDOM_INT(0,100)-50, RANDOM_INT(100,200),true);		
	
	//direction
	if (ai.sprite.y > 100)
	{
		ai.sprite.vector_y=0.0f;
		ai.sprite.vector_x=[ai.sprite getParamInt:@"ai_direction"]*25.0f;
		if (ai.sprite.x < 50){
			[ai.sprite setParamInt:@"ai_direction": 1];
			ai.sprite.x=51;
		}else if (ai.sprite.x > gScreenWidth-50){
			[ai.sprite setParamInt:@"ai_direction": -1];
			ai.sprite.x=gScreenWidth-51;
		}						
	}
	
	//now do special
	if ([ai.sprite getParamDouble:@"ai_specialdur"]>0.0)
	{					
		double stepleft = [ai.sprite getParamDouble:@"ai_specialstepleft"]-gEvenTickTimeDiff;
		if (stepleft<=0.0)
		{
			int angle = [ai.sprite getParamInt:@"ai_specialangle"];
			
			stepleft=0.05;
			
			float vx,vy;
			vy = 1.0f - (__fsin[WrapDegrees(angle)]);
			vx = 1.0f-vy;
			vy = vy*150.0f;
			vx = vx * 150.0f;
			AIShoot(ai, @"AIShot1", vx,vy,true);	
			AIShoot(ai, @"AIShot1", -vx,vy,false);	
			[ai.sprite setParamInt:@"ai_specialangle" :angle+(14*gDifficultyShotFreq)];
		}					
		[ai.sprite setParamDouble:@"ai_specialstepleft" :stepleft];
		[ai.sprite setParamDouble:@"ai_specialdur" :[ai.sprite getParamDouble:@"ai_specialdur"]-0.05];
	}	
	else
	{
		[ai.sprite setParamDouble:@"ai_specialdur" :[ai.sprite getParamDouble:@"ai_specialdur"]-gEvenTickTimeDiff];
		if ([ai.sprite getParamDouble:@"ai_specialdur"] < -7.0*gDifficultyShotFreq)
		{
			[ai.sprite setParamDouble:@"ai_specialdur" :2.0];
			[ai.sprite setParamInt:@"ai_specialangle": 0];
		}
	}	
}

void AIBoss1(CAnimatedSprite* ai)
{
	
	//init settings
	if ([ai.sprite getParamInt:@"ai_isset"]==0)
	{
		[ai.sprite setParamDouble:@"ai_specialdur" :2.0];
		[ai.sprite setParamInt:@"ai_specialangle": 0];
		[ai.sprite setParamDouble:@"ai_specialstepleft" :0.1];
		[ai.sprite setParamInt:@"ai_isset": 1];					
		[ai.sprite setParamInt:@"ai_direction": 1];
	}
	
	//shoot
	if (RANDOM_INT(0,(int)(75*gDifficultyShotFreq))==1)
		AIShoot(ai, @"AIShot2", RANDOM_INT(0,100)-50, RANDOM_INT(100,200),true);		
	
	//direction
	if (ai.sprite.y > 100)
	{
		ai.sprite.vector_y=0.0f;
		ai.sprite.vector_x=[ai.sprite getParamInt:@"ai_direction"]*25.0f;
		if (ai.sprite.x < 50){
			[ai.sprite setParamInt:@"ai_direction": 1];
			ai.sprite.x=51;
		}else if (ai.sprite.x > gScreenWidth-50){
			[ai.sprite setParamInt:@"ai_direction": -1];
			ai.sprite.x=gScreenWidth-51;
		}						
	}
	
	//now do special
	if ([ai.sprite getParamDouble:@"ai_specialdur"]>0.0)
	{					
		double stepleft = [ai.sprite getParamDouble:@"ai_specialstepleft"]-gEvenTickTimeDiff;
		if (stepleft<=0.0)
		{
			int angle = [ai.sprite getParamInt:@"ai_specialangle"];
			
			stepleft=0.1;
			
			float vx,vy;
			vx=__fsin[WrapDegrees(angle)]*200.0f;
			vy=__fcos[WrapDegrees(angle)]*200.0f;
			AIShoot(ai, @"AIShot1", vx,vy,true);	
			AIShoot(ai, @"AIShot1", -vx,vy,false);	
			[ai.sprite setParamInt:@"ai_specialangle" :angle+(7*gDifficultyShotFreq)];
		}					
		[ai.sprite setParamDouble:@"ai_specialstepleft" :stepleft];
		[ai.sprite setParamDouble:@"ai_specialdur" :[ai.sprite getParamDouble:@"ai_specialdur"]-0.1];
	}	
	else
	{
		[ai.sprite setParamDouble:@"ai_specialdur" :[ai.sprite getParamDouble:@"ai_specialdur"]-gEvenTickTimeDiff];
		if ([ai.sprite getParamDouble:@"ai_specialdur"] < -10.0*gDifficultyShotFreq)
		{
			[ai.sprite setParamDouble:@"ai_specialdur" :2.0];
			[ai.sprite setParamInt:@"ai_specialangle": 0];
		}
	}	
}

void AIThink(CAnimatedSprite* ai)
{	
	if (!ai.sprite.isdead)	//i'm alive, kill kill KILL!
	{
		int aitype = [ai.sprite getParamInt:@"aitype"];//[[ai.sprite.parameters objectForKey:@"aitype"] intValue];
		
		switch (aitype) {
			case AI_SKEET_WAVE:
				if ([ai.sprite getParamInt:@"ai_isset"]==0)
				{
					[ai.sprite setParamInt:@"ai_isset" :1];
					[ai.sprite setParamInt:@"ai_angle" :RANDOM_INT(0,359)];
				}
				
				if (ai.sprite.y > 20)
				{
					int angle = [ai.sprite getParamInt:@"ai_angle"];				
					angle += gEvenTickTimeDiff*180.0f;
					[ai.sprite setParamInt:@"ai_angle" :angle];
					
					ai.sprite.vector_x = __fsin[WrapDegrees(angle)] * 80.0f;
					ai.sprite.vector_y = abs(ai.sprite.vector_x/2);
				}
				
				if (RANDOM_INT(0,(int)(30*gDifficultyShotFreq))==1)
					AIShoot(ai, @"AIShot1", RANDOM_INT(0,100)-50, RANDOM_INT(100,200),true);	
				break;
				
			case AI_SKEET:
				if (RANDOM_INT(0,(int)(30*gDifficultyShotFreq))==1)
					AIShoot(ai, @"AIShot1", RANDOM_INT(0,100)-50, RANDOM_INT(100,200),true);	
				break;
						
			case AI_BIGENEMY:
				if ([ai.sprite getParamInt:@"ai_isset"]==0)
				{
					[ai.sprite setParamDouble:@"ai_specialdur" :2.0];
					[ai.sprite setParamInt:@"ai_specialangle": 0];
					[ai.sprite setParamDouble:@"ai_specialstepleft" :0.2];
					[ai.sprite setParamInt:@"ai_isset": 1];					
				}
				
				if ([ai.sprite getParamDouble:@"ai_specialdur"]>0.0)
				{					
					double stepleft = [ai.sprite getParamDouble:@"ai_specialstepleft"]-gEvenTickTimeDiff;
					if (stepleft<=0.0)
					{
						int angle = [ai.sprite getParamInt:@"ai_specialangle"];
						
						stepleft=0.2;
						
						float vx,vy;
						vx=__fsin[WrapDegrees(angle)]*200.0f;
						vy=__fcos[WrapDegrees(angle)]*200.0f;
						AIShoot(ai, @"AIShot1", vx,vy,true);	
						AIShoot(ai, @"AIShot1", -vx,vy,false);	
						[ai.sprite setParamInt:@"ai_specialangle" :angle+10];
					}					
					[ai.sprite setParamDouble:@"ai_specialstepleft" :stepleft];
					[ai.sprite setParamDouble:@"ai_specialdur" :[ai.sprite getParamDouble:@"ai_specialdur"]-0.2];
				}
				else
				{
					if (RANDOM_INT(0,(int)(36*gDifficultyShotFreq))==1)
						AIShoot(ai, @"AIShot2", RANDOM_INT(0,50)-25, RANDOM_INT(250,350),true);						
				}
				
				break;
			
			case AI_GLOWSHIP:
				if (RANDOM_INT(0,(int)(30*gDifficultyShotFreq))==1)
					AIShoot(ai, @"AIShot1", ai.sprite.vector_x*2,ai.sprite.vector_y*2,true);	
				break;
				
			case AI_FIGHTERENEMY:
				if (RANDOM_INT(0,(int)(35*gDifficultyShotFreq))==1)
					AIShoot(ai, @"AIShot1", 0,ai.sprite.vector_y*1.5,true);	
				break;
			
			case AI_BOSS1:
				AIBoss1(ai);
				break;
				
			case AI_BOSS2:
				AIBoss2(ai);
				break;
				
			case AI_BOSS3:
				AIBoss3(ai);
				break;
				
			case AI_BOSS4:
				AIBoss4(ai);
				break;
				
			case AI_BOSS5:
				AIBoss5(ai);
				break;
										
			default:
				if (RANDOM_INT(0,(int)(30*gDifficultyShotFreq))==1)
					AIShoot(ai, @"AIShot1", RANDOM_INT(0,100)-50, RANDOM_INT(100,200),true);	
				break;
		}
	}
}

void AIDied(CAnimatedSprite* ai, bool outOfBounds)
{
	if (!outOfBounds)
	{
		PlaySound(gSound_EnemyDie,false);
		if (RANDOM_INT(0,10)==1)
			DropFP(ai, 0, 200);
		else if (RANDOM_INT(0,10)==1)
			DropSP(ai, 0, 200);
	}
	
	if ([[ai.sprite.parameters objectForKey:@"isboss"] compare:@"1"]==NSOrderedSame)
	{
		gPlayerLevel+=1;
		gLevelAICount+=1;
		//if (gPlayerLevel>6)
		//	gPlayerLevel=1;
		
		KillAllAI();
		ResetBackground();
		[background setCurrentAnimation:[NSString stringWithFormat:@"%d",gPlayerLevel%5] :true];
		for (int i=0;i<gLevelAICount;i++)
			SpawnAIType(GetLevelAIType(),RANDOM_INT(20,gScreenWidth-20), RANDOM_INT(1,100));
	}
	else
	{
		if (background.sprite.y < 0)
		{
			int aitype = GetLevelAIType();	
			SpawnAIType(aitype,RANDOM_INT(20,gScreenWidth-20), 1);
		}
	}
}

void PreloadGameObjects()
{
	//player shots
	for (int i=0; i<10; i++)
		AddPlayerShot(0.0f, 0.0f, 0.0f, 0.0f);
	
	//ai 	
	CAnimatedSprite* ai;
	for (int i=0; i<5; i++)
		ai= SpawnAI(AI_SKEET, @"Skeet", 0, 0, 0, 0);
	
	//ai shots
	for (int i=0; i<10; i++)
		AIShoot(ai, @"AIShot1", 0, 0,false);
	
	//powerups
	//for (int i=0; i<5; i++)
	//{
	//}
	
	//disable all
	for (int i=0; i<[gAIList count]; i++)	
		((CAnimatedSprite*)[gAIList objectAtIndex:i]).sprite.enabled=false;
	for (int i=0; i<[gAIShotList count]; i++)	
		((CAnimatedSprite*)[gAIShotList objectAtIndex:i]).sprite.enabled=false;
	for (int i=0; i<[gPlayerShotList count]; i++)	
		((CAnimatedSprite*)[gPlayerShotList objectAtIndex:i]).sprite.enabled=false;
	
	//ShowAlertBox(@"", [NSString stringWithFormat:@"pls: %d - ais: %d - ai: %d",[gPlayerShotList count],[gAIShotList count],[gAIList count]], gView);
}

void CheckPlayerBounds()
{
	if (player.sprite.x<1)
		player.sprite.x=1;
	if (player.sprite.x>=gScreenWidth)
		player.sprite.x=gScreenWidth-1;
	if (player.sprite.y<100)
		player.sprite.y=100;
	if (player.sprite.y>=gScreenHeight-player.sprite.scaleHeight/2)
		player.sprite.y=gScreenHeight-player.sprite.scaleHeight/2-1;	
}

void DoPlayerSpecial()
{
	//fire player special
	if (!player.sprite.isdead && gPlayerSpecialTimeLeft>0.0f)
	{
		gPlayerSpecialStepLeft -= gTimeDiff;
		if (gPlayerSpecialStepLeft <= 0.0f)
		{
			if (gPlayerSpecialAngle % 21 == 0)
				PlaySound(gSound_PlayerShot);
			
			gPlayerSpecialAngle+=7;
			float vx=__fsin[WrapDegrees(gPlayerSpecialAngle)]*gPlayerShot_Speed;
			float vy=__fcos[WrapDegrees(gPlayerSpecialAngle)]*gPlayerShot_Speed;
			int shotPower = gPlayerShotPower;//[player.sprite getParamInt:@"shotPower"];//[[player.sprite.parameters objectForKey:@"shotPower"] intValue];			
			switch (shotPower)
			{
				case 1:
					PlayerShootSpecial(vx, vy, -gPlayerSpecialAngle);
					break;
				case 2:
					PlayerShootSpecial(vx, vy,-gPlayerSpecialAngle);
					PlayerShootSpecial(vx*-1, vy*-1,-gPlayerSpecialAngle);
					break;
				case 3:
					PlayerShootSpecial(vx, vy, -gPlayerSpecialAngle);
					float vx2=vx*0.5f;
					float vy2=vy*-0.5f;
					PlayerShootSpecial(vx2, vy2, gPlayerSpecialAngle);
					if (gPlayerSpecialAngle % 21 == 0)
						PlayerShootSpecial(0.0f, gPlayerShot_Speed, 0);
					break;
			}				
			gPlayerSpecialTimeLeft -= 0.03f;
			gPlayerSpecialStepLeft = 0.03f;
		}
	}
}

void DoAI()
{	
	//enemy ai
	for (int i=0; i<[gAIList count]; i++){	
		CAnimatedSprite* ai = (CAnimatedSprite*)[gAIList objectAtIndex:i];
		if (ai.sprite.enabled)
		{	
			if (!ai.sprite.isdead && (ai.sprite.y < 0 || ai.sprite.y > gScreenHeight+100 || ai.sprite.x < 0 || ai.sprite.x > gScreenWidth))
			{
				ai.sprite.enabled=false;
				AIDied(ai,true);
			}
			else
				AIThink(ai);
		}
	}
}

void DoMovement()
{
	//move ai
	for (int i=0; i<[gAIList count]; i++){	
		CAnimatedSprite* ai = (CAnimatedSprite*)[gAIList objectAtIndex:i];
		if (ai.sprite.enabled)
		{	
			ai.sprite.y += (float)gTimeDiff * ai.sprite.vector_y;		
			ai.sprite.x += (float)gTimeDiff * ai.sprite.vector_x;	
		}
	}
	
	//move player	
	if (!player.sprite.isdead)
	{
		float moveVamt = (float)gTimeDiff * 160.0f; //Y speed
		float moveHamt = (float)gTimeDiff * 195.0f; //X speed
		if (gAccelX>0.1f || gAccelX < -0.1f || gAccelY>0.1f || gAccelY < -0.1f )
		{
			moveVamt = moveVamt*gAccelY*-2.0f;
			moveHamt = moveHamt*gAccelX*2.0f;
			player.sprite.x += moveHamt;
			player.sprite.y += moveVamt;
			CheckPlayerBounds();
		}
	}
	
	//move projectiles
	for (int i=0; i<[gPlayerShotList count]; i++){	
		CAnimatedSprite* shot = (CAnimatedSprite*)[gPlayerShotList objectAtIndex:i];
		if (shot.sprite.enabled && !shot.sprite.isdead)
		{
			shot.sprite.y += (float)gTimeDiff * shot.sprite.vector_y;		
			shot.sprite.x += (float)gTimeDiff * shot.sprite.vector_x;		
			if (shot.sprite.y < 0 || shot.sprite.y > gScreenHeight || shot.sprite.x < 0 || shot.sprite.x > gScreenWidth)
				shot.sprite.enabled=false;
		}
	}
	for (int i=0; i<[gAIShotList count]; i++){	
		CAnimatedSprite* shot = (CAnimatedSprite*)[gAIShotList objectAtIndex:i];
		if (shot.sprite.enabled && !shot.sprite.isdead)
		{
			shot.sprite.y += (float)gTimeDiff * shot.sprite.vector_y;		
			shot.sprite.x += (float)gTimeDiff * shot.sprite.vector_x;		
			if (shot.sprite.y < 0 || shot.sprite.y > gScreenHeight+100 || shot.sprite.x < 0 || shot.sprite.x > gScreenWidth)
				shot.sprite.enabled=false;
		}
	}
	
	//move powerups
	for (int i=0; i<[gPowerupFPList count]; i++){	
		CAnimatedSprite* pow = (CAnimatedSprite*)[gPowerupFPList objectAtIndex:i];
		if (pow.sprite.enabled)
		{
			pow.sprite.y += (float)gTimeDiff * pow.sprite.vector_y;		
			pow.sprite.x += (float)gTimeDiff * pow.sprite.vector_x;		
			if (pow.sprite.y < 0 || pow.sprite.y > gScreenHeight || pow.sprite.x < 0 || pow.sprite.x > gScreenWidth)
				pow.sprite.enabled=false;
		}
	}
	for (int i=0; i<[gPowerupSPList count]; i++){	
		CAnimatedSprite* pow = (CAnimatedSprite*)[gPowerupSPList objectAtIndex:i];
		if (pow.sprite.enabled)
		{
			pow.sprite.y += (float)gTimeDiff * pow.sprite.vector_y;		
			pow.sprite.x += (float)gTimeDiff * pow.sprite.vector_x;		
			if (pow.sprite.y < 0 || pow.sprite.y > gScreenHeight || pow.sprite.x < 0 || pow.sprite.x > gScreenWidth)
				pow.sprite.enabled=false;
		}
	}
}

void DoCollision()
{
	//check collision
	for (int i=0; i<[gPlayerShotList count]; i++){		//player shots to AI
		CAnimatedSprite* shot = (CAnimatedSprite*)[gPlayerShotList objectAtIndex:i];
		if (shot.sprite.enabled && !shot.sprite.isdead)
		{
			for (int j=0; j<[gAIList count]; j++){	
				CAnimatedSprite* ai = (CAnimatedSprite*)[gAIList objectAtIndex:j];
				if (!ai.sprite.isdead && ai.sprite.enabled )
				{								
					if( [ai.sprite collidesWith:shot.sprite])
					{
						int hp = [ai.sprite getParamInt:@"hp"];//[[ai.sprite.parameters objectForKey:@"hp"] intValue];
						hp--;					
						shot.sprite.isdead=true;
						[shot setCurrentAnimation:@"ShotDie" :false];
						gPlayerScore+=20;
						
						if (hp<=0)
						{
							ai.sprite.isdead=true;
							[ai setCurrentAnimation:@"Die" :false];	
							AIDied(ai,false);		//in case it needs to do something on-death
							gPlayerScore+=gEnemyKillPoints;
						}
						else
							[ai.sprite setParamInt:@"hp" :hp];//[ai.sprite.parameters setObject:[NSString stringWithFormat:@"%d",hp] forKey:@"hp"];
					}
				}
			}
		}
	}	
	 
	for (int i=0; i<[gAIShotList count]; i++){			//ai shots to player
		CAnimatedSprite* shot = (CAnimatedSprite*)[gAIShotList objectAtIndex:i];
		if (shot.sprite.enabled && !player.sprite.isdead)
		{
			float shleft,shright,shtop,shbottom;
			shleft = player.sprite.x-player.sprite.scaleWidth/2+5;
			shright = shleft+player.sprite.scaleWidth-10;
			shtop = player.sprite.y-player.sprite.scaleHeight/2-6;
			shbottom = shtop+12;
			if ([shot.sprite collidesWithRect:shleft :shtop :shright :shbottom])	//player shield
			{
				[gPlayerShotList addObject:shot];
				[gAIShotList removeObject:shot];
				shot.sprite.vector_y = shot.sprite.vector_y*-1.0f;
				gPlayerScore+=10;
			}
			else if ([shot.sprite collidesWith:player.sprite])			//player hit
			{
				PlaySound(gSound_PlayerDie,false);
				gLastShotTime = gLastTick;
				gPlayerScore-=1000;
				gPlayerLives--;
				gPlayerSpecialTimeLeft = 0.0f;
				player.sprite.isdead=true;
				[player setCurrentAnimation:@"Die" :true];
				
				shot.sprite.isdead=true;
				[shot setCurrentAnimation:@"ShotDie" :false];
			}			
		}
	}
	for (int i=0; i<[gAIList count]; i++){			//ai to player
		CAnimatedSprite* ai = (CAnimatedSprite*)[gAIList objectAtIndex:i];
		if (!ai.sprite.isdead && ai.sprite.enabled && !player.sprite.isdead)
		{
			if ([ai.sprite collidesWith:player.sprite])				//player hit
			{
				PlaySound(gSound_PlayerDie,false);
				gPlayerSpecialTimeLeft = 0.0f;
				gPlayerScore-=1000*gPlayerLevel;
				gPlayerLives--;
				player.sprite.isdead=true;
				gLastShotTime = gLastTick;
				[player setCurrentAnimation:@"Die" :true];
				
				if ([[ai.sprite.parameters objectForKey:@"isboss"] compare:@"1"]!=NSOrderedSame)
				{
					ai.sprite.isdead=true;
					[ai setCurrentAnimation:@"Die" :false];					
					AIDied(ai,false);		//in case it needs to do something on-death
				}
			}			
		}
	}
	for (int i=0; i<[gPowerupFPList count]; i++){	//powerups to player
		CAnimatedSprite* pow = (CAnimatedSprite*)[gPowerupFPList objectAtIndex:i];
		if (!player.sprite.isdead && pow.sprite.enabled )
			if ([pow.sprite collidesWith:player.sprite])
		{	//grab powerup
			int shotPower = gPlayerShotPower+1;//[player.sprite getParamInt:@"shotPower"]+1;//[[player.sprite.parameters objectForKey:@"shotPower"] intValue] + 1;
			if (shotPower>3)
				shotPower=3;
			//[player.sprite.parameters setObject:[NSString stringWithFormat:@"%d",shotPower] forKey:@"shotPower"];	
			gPlayerShotPower=shotPower; //[player.sprite setParamInt:@"shotPower" :shotPower];
			pow.sprite.enabled=false;
		}
	}
	for (int i=0; i<[gPowerupSPList count]; i++){	//powerups to player
		CAnimatedSprite* pow = (CAnimatedSprite*)[gPowerupSPList objectAtIndex:i];
		if (!player.sprite.isdead && pow.sprite.enabled )
			if ([pow.sprite collidesWith:player.sprite])
		{	//grab powerup
			int specials = [[player.sprite.parameters objectForKey:@"specials"] intValue] + 1;
			if (specials>9)
				specials=9;		
			[player.sprite setParamInt:@"specials" :specials];
			pow.sprite.enabled=false;
		}
	}
}

void DoPlayerInput()
{
	//fire projectiles + grab pwoerups (player)
	if (gMouseDown)
	{
		//double lastShotTime = [player.sprite getParamDouble:@"lastShotTime"];//[[player.sprite.parameters objectForKey:@"lastShotTime"] doubleValue];
		
		if (!player.sprite.isdead) {
			/*
			for (int i=0; i<[gPowerupFPList count]; i++){	
				CAnimatedSprite* pow = (CAnimatedSprite*)[gPowerupFPList objectAtIndex:i];				
				if (pow.sprite.enabled )
					if ([pow.sprite collidesWithRect:gMouseX-10 :gMouseY-10 :gMouseX+10 :gMouseY+10])
				{	//grab powerup
					int shotPower = [player.sprite getParamInt:@"shotPower"]+1;//[[player.sprite.parameters objectForKey:@"shotPower"] intValue] + 1;
					if (shotPower>3)
						shotPower=3;
					[player.sprite setParamInt:@"shotPower" :shotPower];
					pow.sprite.enabled=false;
				}
			}
			for (int i=0; i<[gPowerupSPList count]; i++){	
				CAnimatedSprite* pow = (CAnimatedSprite*)[gPowerupSPList objectAtIndex:i];
				if (pow.sprite.enabled )
					if ([pow.sprite collidesWithRect:gMouseX-10 :gMouseY-10 :gMouseX+10 :gMouseY+10])
				{	//grab powerup
					int specials = [[player.sprite.parameters objectForKey:@"specials"] intValue] + 1;
					if (specials>9)
						specials=9;	
					[player.sprite setParamInt:@"specials" :specials];
					pow.sprite.enabled=false;
				}
			}
			 */
			
			if (gLastTick-gLastShotTime >= 0.2f)
			{
				PlaySound(gSound_PlayerShot);
				PlayerShoot();
				gPlayerScore -= 1;
				gLastShotTime=gLastTick;
			}
		}
		else if (!player.sprite.enabled && gLastTick-gLastShotTime >= 2.0f)
		{
			ResetPlayer();
			player.sprite.x=gMouseX;
			player.sprite.y=gMouseY;
			CheckPlayerBounds();
		}
	}	
}

void DoGame()
{ 	
	DoPlayerSpecial();
	if (gTickIsEven)
		DoAI();
	DoMovement();
	if (gTickIsEven)
	{
		DoCollision();
		DoPlayerInput();
		
		//player force respawn
		if (gPlayerLives>0 && !player.sprite.enabled && gLastTick-gLastShotTime >= 8.0f)
		{
			ResetPlayer();
			CheckPlayerBounds();
		}
	}
}

void MenuDraw(double current)
{
	EAGLContext *context =  gView.context;
	GLuint viewFramebuffer = gView.viewFramebuffer, viewRenderbuffer = gView.viewRenderbuffer;
	GLint backingWidth = gView.backingWidth, backingHeight = gView.backingHeight;
	
	[EAGLContext setCurrentContext:context];
	
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
	glViewport(0, 0, backingWidth, backingHeight);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0.0f, backingWidth, backingHeight, 0.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	//RENDER NOW	
	[gMenuSprite_Background render:false];
	[player animate:current :false];
	[player render:false];
		
	[gFont.sprite setOverlayWithRed:0 :255 :0 :255];
	[gFont renderStringAt:236 :319 :gDifficultyString :10 :0];
	[gFont.sprite setOverlayWithRed:255 :255 :0 :255];
	[gFont renderStringAt:60 :274 :[NSString stringWithFormat:@"High Score: %07d",gHighScore] :10 :0];
	//END RENDER
	
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	
	[context presentRenderbuffer:GL_RENDERBUFFER_OES];	
}

void GameTick()
{		
	double current = CFAbsoluteTimeGetCurrent();
	gTimeDiff = current - gLastTick;
	gTickIsEven = !gTickIsEven;
	if (gTickIsEven) //disable eventick
		gEvenTickTimeDiff += gTimeDiff;
	else
		gEvenTickTimeDiff = gTimeDiff;
	
	if (gGameState==GS_INGAME || gGameState==GS_PAUSE)
	{
		if (gPlayerScore<0)
			gPlayerScore=0;
		else if (gPlayerScore>gHighScore)
			gHighScore=gPlayerScore;
		
		gStatusString = [NSString stringWithFormat:@"Score: %07d   Best: %07d",gPlayerScore,gHighScore];
		gStatusString2 = [NSString stringWithFormat:@"SP-%@   1UP-%02d   LEVEL-%02d   FP-%d", [player.sprite.parameters objectForKey:@"specials"], gPlayerLives, gPlayerLevel, gPlayerShotPower];
		//gStatusString2 = [NSString stringWithFormat:@"TimeDiff: %f - Shots: %d", gTimeDiff, [gPlayerShotList count]];
		if (gLastBossLevel < gPlayerLevel && background.sprite.y >=0)
		{
			gLastBossLevel=gPlayerLevel;
			KillAllAI();
			SpawnAIBoss();
		}
		
		if (gGameState==GS_INGAME)
			DoGame();
		
		if (!gTickIsEven)
			AnimateGame(current);					
		
		//if (gTimeDiff < 0.031f)
			DrawScreen();
	}
	else if (gGameState==GS_MENU)
	{
		MenuDraw(current);
	}
	
	gLastTick = current;	
}

void AccelerationChanged(UIAccelerationValue x, UIAccelerationValue y, UIAccelerationValue z)
{
	if (gGameState==GS_INGAME && gAccelX_Base==-100.0f)
	{
		gAccelX_Base=x;
		gAccelY_Base=y;
		
		if (gAccelX_Base>0.0f)
			gAccelX_Offset=1.0f-gAccelX_Base;
		else if (gAccelX_Base<=0.0f)
			gAccelX_Offset=1.0f-(-1*gAccelX_Base);
		
		if (gAccelY_Base>0.0f)
			gAccelY_Offset=1.0f-gAccelY_Base;
		else if (gAccelY_Base<=0.0f)
			gAccelY_Offset=1.0f-(-1*gAccelY_Base);
		
		if (gAccelX_Offset<gAccelY_Offset)
			gAccelY_Offset=gAccelX_Offset;
		else
			gAccelX_Offset=gAccelY_Offset;
	}
	else if (gGameState==GS_INGAME)
	{	//***scaling***
		gAccelX=x-gAccelX_Base;
		gAccelY=y-gAccelY_Base;
		
		if (gAccelX>gAccelX_Offset)
			gAccelX=gAccelX_Offset;
		if (gAccelX<-gAccelX_Offset)
			gAccelX=-gAccelX_Offset;
		if (gAccelY>gAccelY_Offset)
			gAccelY=gAccelY_Offset;
		if (gAccelY<-gAccelY_Offset)
			gAccelY=-gAccelY_Offset;
		
		
		gAccelX=gAccelX*(1.0f/gAccelX_Offset);
		gAccelY=gAccelY*(1.0f/gAccelY_Offset);
		
		
		/*/wrapping
		gAccelX=x-gAccelX_Offset;
		gAccelY=y-gAccelY_Offset;
		if (gAccelX>1.0f)
			gAccelX = gAccelX - 2.0f;
		else if (gAccelX < -1.0f)
			gAccelX = gAccelX+2.0f;
		if (gAccelY>1.0f)
			gAccelY=gAccelY-2.0f;
		else if (gAccelY<1.0f)
			gAccelY=gAccelY+2.0f;
		 */
	}
}

void TouchStarted(NSSet* touches, UIEvent* event)
{	
	UITouch* touch = [touches anyObject];
	CGPoint point = [touch locationInView:gView];
	
	if (gGameState==GS_MENU)
	{
		//92, 314 == difficulty
		//92, 350 == 3d
		//92, 406 == 2d
		if (point.x>=92 && point.x<=92+126)
		{
			if (point.y>=314 && point.y<=314+43)	//difficulty
			{
				if (gDifficultyShotFreq<=0.5f)	//change to easy
				{
					gDifficultyShotFreq=1.99f;
					gDifficultyString=@"Simple";
					gEnemyKillPoints=50;
					gPlayerLives=20;
				}
				else if (gDifficultyShotFreq<=1.0f	)   //change to hard
				{
					gDifficultyShotFreq=0.49f;
					gDifficultyString=@"Xtreme";
					gEnemyKillPoints=150;
					gPlayerLives=10;
				}
				else //change to normal
				{
					gDifficultyShotFreq=0.99f;
					gDifficultyString=@"Normal";
					gEnemyKillPoints=100;
					gPlayerLives=15;
				}
			}
			else if (point.y>=340 && point.y<=340+43)//3d
			{
				gPerspectiveOn=true;
				gGameState=GS_INGAME;
				ResetPlayer();
				player.sprite.y = gScreenHeight-50;
			}
			else if (point.y>=396 && point.y<=396+43)//2d
			{
				gPerspectiveOn=false;
				gGameState=GS_INGAME;
				ResetPlayer();
			}
		}		
	}
	
	if (gGameState==GS_PAUSE)
	{
		gGameState=GS_INGAME;
		pauseSprite.sprite.enabled=false;
		return;
	}
	
	if (!player.sprite.isdead && [touches count]>=2 || [[event allTouches] count]>=2 || gMouseDown)
	{		
		CGPoint p1,p2;
		UITouch* t1 = [[[event allTouches] allObjects] objectAtIndex:0];
		UITouch* t2 = [[[event allTouches] allObjects] objectAtIndex:1];
		p1=[t1 locationInView:gView];
		p2=[t2 locationInView:gView];
		if ((p1.y < 50 && p2.y > gScreenHeight-50) || (p1.y > gScreenHeight-50 && p2.y < 50))
		{			
			if (gGameState==GS_INGAME)
			{
				gGameState=GS_PAUSE;
				pauseSprite.sprite.enabled=true;
			}
		}
		else if (gGameState==GS_INGAME)
		{
			int specials = [[player.sprite.parameters objectForKey:@"specials"] intValue] ;
			if (specials > 0 && gPlayerSpecialTimeLeft<=0.0f)
			{
				gPlayerSpecialTimeLeft = 5.0f;
				gPlayerSpecialAngle=91;
				[player.sprite setParamInt:@"specials" :specials-1];
			}
		}
	}
	
	if (gGameState==GS_INGAME)
	{
		/*
		CSprite* buttonClicked= [gButtonManager handleClick:point.x :point.y :1];
		if (buttonClicked!=nil)
		{
			ShowAlertBox(@"Button clicked", [buttonClicked.parameters objectForKey:@"buttonName"], gView);
		}
		else
		{
		 */
			gMouseDown=true;
			gMouseX = point.x;
			gMouseY = point.y;
			if (gPerspectiveOn)
				gMouseY = gMouseY*1.2f;
		
		if (gPlayerLives<=0 && player.sprite.isdead)
		{
			DisableAllSprites();
			ResetPlayer();
			ResetBackground();
			InitGame(nil, nil, false);
		}
		//}
	}
}

void TouchMoved(NSSet* touches, UIEvent* event)
{
	UITouch* touch = [touches anyObject];
	CGPoint point = [touch locationInView:gView];
	gMouseX = point.x;
	gMouseY = point.y;
	if (gPerspectiveOn)
		gMouseY = gMouseY*1.2f;
}

void TouchEnded(NSSet* touches, UIEvent* event)
{
	UITouch* touch = [touches anyObject];
	CGPoint point = [touch locationInView:gView];
	gMouseX=point.x;
	gMouseY=point.y;
	if (gPerspectiveOn)
		gMouseY = gMouseY*1.2f;
	gMouseDown=false;
}

void QuitGame()
{
	SaveGameSettings();
}
