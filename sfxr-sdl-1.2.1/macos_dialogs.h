#ifndef MACOS_DIALOGS_H
#define MACOS_DIALOGS_H

#ifdef __cplusplus
extern "C" {
#endif

// Returns 1 if a file was selected, 0 if cancelled
int macos_open_file_dialog(char *outPath, size_t maxLen);
int macos_save_file_dialog(char *outPath, size_t maxLen);

#ifdef __cplusplus
}
#endif

#endif
