
#import "CAnimationFrame.h"

@interface CAnimation : NSObject {
	@public
	NSMutableArray* frames;
}

//@property (retain) NSMutableArray* frames;

- (CAnimationFrame*) frameAt:(int)index;
- (id) init;
- (CAnimationFrame*) addFrame: (NSString*)type:(double)duration;

@end
