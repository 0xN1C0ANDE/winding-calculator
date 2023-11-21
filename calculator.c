/*
    * @brief Simple winding calculator.
    * @file calculator.c
    * @author Nicolas Andersson
    * @date 21.11.23
 */

#define COPPER_RESISTIVITY 1.68E-8 // roh: ohm-meter

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void printHelpScreen();

int main(int argc, char* argv[]) {
    if(argc == 2 && !strncmp(argv[1], "-h", 10))
        printHelpScreen();
    
    printf("Need for resistance ? [Y/N]\n");
    return 0;
}

 void printHelpScreen() {}
/*
           _           _ _                                 _            _       _             
 __      _(_)_ __   __| (_)_ __   __ _            ___ __ _| | ___ _   _| | __ _| |_ ___  _ __ 
 \ \ /\ / / | '_ \ / _` | | '_ \ / _` |  _____   / __/ _` | |/ __| | | | |/ _` | __/ _ \| '__|
  \ V  V /| | | | | (_| | | | | | (_| | |_____| | (_| (_| | | (__| |_| | | (_| | || (_) | |   
   \_/\_/ |_|_| |_|\__,_|_|_| |_|\__, |          \___\__,_|_|\___|\__,_|_|\__,_|\__\___/|_|   
                                 |___/                                                        

    ,----..              ,-----.. ────────────│
   /   /    \            /   /   \            │
  /   .      :          /   .     :           │
 .   /    ;------------.   /   ..  \─────│    │
.   ;    / / / / / / /.   ;   /  \ ;     │    │
;   |   ; ; ; ; ; ; ; ;   |  ;   ; |     │    │
|   :   | | | | | | | |   :  |   | '     d    D
.   |   ' ' ' ' ' ' ' .   |  '   ' :     │    │
'   ;   \ \ \ \ \ \ \ '   ;  \  /  |     │    │
 \   \   ',____________\   \  ',  / ─────┤    │
  ;   :     /           ;   :    /            │
   \   \  .'             \   \ .'             │
    `---`                `----`  ─────────────┤
        │                │
        ├────── w ───────┤


*/