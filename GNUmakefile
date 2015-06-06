include $(GNUSTEP_MAKEFILES)/common.make
   
APP_NAME = Draconis
Draconis_HEADERS = GlobalEnums.h CAnimatedSprite.h CAnimationFrame.h CFont.h GlobalEnums.h RenderUtils.h CAnimation.h CButtonManager.h CSprite.h TrigArrays.h
Draconis_OBJC_FILES = CAnimatedSprite.m CAnimationFrame.m CFont.m TrigArrays.m CAnimation.m CButtonManager.m CSprite.m RenderUtils.m main.m
Draconis_RESOURCE_FILES =   

ADDITIONAL_OBJC_LIBS = -lglu32 -lopengl32 -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_mixer -lgnustep-base
ADDITIONAL_CFLAGS = -DWIN32 -DDOCKING_CLEARANCE_ENABLED -DNEED_STRLCPY `sdl-config --cflags` -std=c99 
ADDITIONAL_OBJCFLAGS = -DLOADSAVEGUI -DWIN32 -DXP_WIN -DDOCKING_CLEARANCE_ENABLED -Wno-import `sdl-config --cflags` -std=c99
oolite_LIB_DIRS += -L/usr/local/lib -L$(GNUSTEP_LOCAL_ROOT)/lib -Ldeps/Windows-x86-deps/lib


include $(GNUSTEP_MAKEFILES)/application.make
