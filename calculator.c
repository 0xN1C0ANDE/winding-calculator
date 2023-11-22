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

typedef struct Variables {
    char needForRes;
    double widthMm, innerDiamMm, outerDiamMm, wireDiamMm, wireSectionMm2, wireLengthMm, wireResistance, resistivity;
    char providedFields;
} variables;

int getVariables(variables *var);
int dataChecker(variables *var);
void solver(variables *var);
void printHelpScreen(void);

int main(int argc, char* argv[]) {

    variables var = {0};

    if(argc == 2 && !strncmp(argv[1], "-h", 10)) printHelpScreen();
    
    getVariables(&var);

    dataChecker(&var);

    solver(&var);

    return 0;
}

int getVariables(variables *var) {
    printf("Need for resistance ? [Y/N]\n");
    if(!scanf("%c[YN]", &var->needForRes)) return 1;

    printf("Please provide winding dimensions [mm] as follow: \"width-innerDiameter-outerDiameter\"\n");
    printf("(Use 0 for unknowns)\n");
    if(scanf("%lf-%lf-%lf", &var->widthMm, &var->innerDiamMm, &var->outerDiamMm) != 3) return 1;

    if(var->needForRes == 'Y') {
        printf("Please provide wire diameter[mm] OR section area [mm2] as follow: \"wireDiameter-wireSection\"\n");
        printf("(Use 0 for unknown)\n");
        if(scanf("%lf-%lf", &var->wireDiamMm, &var->wireSectionMm2) != 2) return 1;

        printf("Please provide wire length[mm], resistance[ohm] and optionaly material resistivity[ohm*m] as follow: \"wireLength-wireResistance-()\"\n");
        printf("(Default resistivity %lf[ohm*m] (Copper @ 20ºC))\n", COPPER_RESISTIVITY);
        printf("(Use 0 for unknowns)\n");
        if(scanf("%lf-%lf-%lf", &var->wireLengthMm, &var->wireResistance, &var->resistivity) < 2) return 1;

    } else {
        printf("Please provide wire length in [mm]:\n");
        if(scanf("%lf", &var->wireLengthMm) != 1) return 1;
    }
    return 0;
}

int dataChecker(variables *var) {
    //Check for inconsistencies in the variables
    printf("Check...\n");
    if(var->widthMm < 0 || var->innerDiamMm < 0 || var->outerDiamMm < 0 || var->wireDiamMm < 0 || var->wireSectionMm2 < 0 || var->wireLengthMm < 0 || var->wireResistance < 0 || var->resistivity < 0) {
        printf("invalid value");
        return 1;
    }

    //saved what we have
    if(var->widthMm > 0)         var->providedFields |= 0x80;
    if(var->innerDiamMm > 0)     var->providedFields |= 0x40;
    if(var->outerDiamMm > 0)     var->providedFields |= 0x20;
    if(var->wireDiamMm > 0)      var->providedFields |= 0x10;
    if(var->wireSectionMm2 > 0)  var->providedFields |= 0x08;
    if(var->wireLengthMm > 0)    var->providedFields |= 0x04;
    if(var->wireResistance > 0)  var->providedFields |= 0x02;
    if(var->resistivity > 0)     var->providedFields |= 0x01;

    return 0;
}

void solver(variables *var) {
    if(var->needForRes == 'Y') {
        while(!(~var->providedFields)) {
            printf("Solving with resistance...");
        }
    } else {
        while((~var->providedFields)) {
            printf("Solving... %x\n", ~var->providedFields);
        }
    }
}

void printHelpScreen(void) {}
/*
    printf("
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
"\n)
*/

