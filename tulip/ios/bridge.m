// bridge.m
#include <SDL.h>
#include <SDL_syswm.h>
#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import "SDL_uikitviewcontroller.h"
#import <UIKit/UIKit.h>

extern SDL_Window *window;

// Stuff here that needs to bridge iOS/SDL

uint8_t is_ipad() {
    if([UIDevice currentDevice].userInterfaceIdiom == UIUserInterfaceIdiomPad) {
        return 1;
    } 
    return 0;
}
uint8_t is_iphone() {
    if([UIDevice currentDevice].userInterfaceIdiom == UIUserInterfaceIdiomPhone) {
        return 1;
    } 
    return 0;
}
void ios_copy_fs() {
    fprintf(stderr, "copying fs..\n");
    NSURL *fileFromBundle,*destinationURL;
    NSArray *folders = @[@"ex", @"images", @"sounds", @"test"];
    for (NSString *folder in folders){
        fileFromBundle = [[NSBundle mainBundle]URLForResource:folder withExtension:nil];
        destinationURL = [[[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject] URLByAppendingPathComponent:folder];
        if ([[NSFileManager defaultManager] fileExistsAtPath:[destinationURL path]]) {
            [[NSFileManager defaultManager] removeItemAtURL:destinationURL error:nil];
        }
        [[NSFileManager defaultManager]copyItemAtURL:fileFromBundle toURL:destinationURL error:nil];
    }
}

// Returns the y position of the keyboard on the screen -- will change with rotations etc 
int get_keyboard_y()
{
    SDL_SysWMinfo systemWindowInfo;
    SDL_VERSION(&systemWindowInfo.version);
    SDL_GetWindowWMInfo(window, &systemWindowInfo);
    UIWindow * appWindow = systemWindowInfo.info.uikit.window;
    SDL_uikitviewcontroller * vc = (SDL_uikitviewcontroller * )appWindow.rootViewController;
    int keybottom = vc.view.bounds.size.height - vc.keyboardHeight;

    return keybottom;
}

void ios_draw_text(float x, float y, float w, float h, char *text) {
    UILabel *lbl1 = [[UILabel alloc] init];
    [lbl1 setFont:[UIFont systemFontOfSize:30]];
    lbl1.textColor = [UIColor blackColor];
    [lbl1 setFrame:CGRectMake(x,y,w,h)];
    lbl1.backgroundColor=[UIColor clearColor];
    lbl1.textColor=[UIColor blackColor];
    lbl1.userInteractionEnabled=NO;
    lbl1.text= [NSString stringWithCString:text encoding:NSUTF8StringEncoding];

    SDL_SysWMinfo systemWindowInfo;
    SDL_VERSION(&systemWindowInfo.version);
    SDL_GetWindowWMInfo(window, &systemWindowInfo);
    UIWindow * appWindow = systemWindowInfo.info.uikit.window;
    SDL_uikitviewcontroller * vc = (SDL_uikitviewcontroller * )appWindow.rootViewController;

    [vc.view addSubview:lbl1];
}

