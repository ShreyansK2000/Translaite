#ifndef GLOBAL_H__
#define GLOBAL_H__

//Language macros to access language array
#define ENGLISH 0
#define FRENCH 1
#define SPANISH 2
#define GERMAN 3
#define ITALIAN 4

// The user's target language when performing translations
extern int target_language;

// The user's native language when displaying the menu buttons and performing translations
extern int native_language;

// Flag to kill the loading screen. After long blocking function, set this flag to 0 to stop the loading screen thread.
extern int keep_loading;

// Buffer used to store the username
extern char username[1024];

// Buffer used to store the password
extern char password[1024];

// LINUX asssigned virtual address space base
extern void * virtual_base;

// User modes
enum user_modes {Test = 0, User = 1};

// User mode for different gui
extern enum user_modes user_mode;

#endif /* GLOBAL_H__ */
