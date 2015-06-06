
#import "CSprite.h"


@interface CButtonManager : NSObject {
	@public
	NSMutableDictionary* buttonSprites;
}

- (id) init;
- (CSprite*) addButton:(CSprite*)buttonsprite:(NSString*)buttonName:(int)x:(int)y:(int)clickableWidth:(int)clickableHeight:(int)buttonGroup;
- (void) renderButtons:(int)buttonGroup;
- (CSprite*) handleClick:(int)x:(int)y:(int)buttonGroup;

@end
