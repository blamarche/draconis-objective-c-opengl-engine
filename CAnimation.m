
#import "CAnimation.h"


@implementation CAnimation

//@synthesize frames;

- (id) init
{
	self=[super init];	
	self->frames = [[NSMutableArray alloc] initWithCapacity:5];	
	return self;
}

- (CAnimationFrame*) addFrame: (NSString*)type:(double)duration
{
	CAnimationFrame* thisframe = [[CAnimationFrame alloc] initWithType:type :duration];
	[frames addObject:thisframe];
	return thisframe;
}

- (CAnimationFrame*) frameAt:(int)index
{
	return (CAnimationFrame*)[frames objectAtIndex:index];
}

@end
