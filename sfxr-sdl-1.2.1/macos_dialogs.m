#import <Cocoa/Cocoa.h>
#include <string.h>

// Native macOS file open dialog
int macos_open_file_dialog(char *outPath, size_t maxLen) {
    @autoreleasepool {
        NSOpenPanel *panel = [NSOpenPanel openPanel];
        [panel setCanChooseFiles:YES];
        [panel setCanChooseDirectories:NO];
        [panel setAllowsMultipleSelection:NO];

        if ([panel runModal] == NSModalResponseOK) {
            NSURL *url = [[panel URLs] objectAtIndex:0];
            const char *path = [[url path] UTF8String];
            strncpy(outPath, path, maxLen - 1);
            outPath[maxLen - 1] = '\0';
            return 1;
        }
    }
    return 0;
}

// Native macOS file save dialog
int macos_save_file_dialog(char *outPath, size_t maxLen) {
    @autoreleasepool {
        NSSavePanel *panel = [NSSavePanel savePanel];

        if ([panel runModal] == NSModalResponseOK) {
            NSURL *url = [panel URL];
            const char *path = [[url path] UTF8String];
            strncpy(outPath, path, maxLen - 1);
            outPath[maxLen - 1] = '\0';
            return 1;
        }
    }
    return 0;
}
