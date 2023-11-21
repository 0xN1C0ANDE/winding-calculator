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

void printHelpScreen(void);

int main(int argc, char* argv[]) {
    char needForRes;
    double widthMm, innerDiamMm, outerDiamMm, wireDiamMm, wireSectionMm2, wireLengthMm, wireResistance, resistivity = 0;
    char providedFields = 0;

    if(argc == 2 && !strncmp(argv[1], "-h", 10)) printHelpScreen();
    
    //Retrieve details
    printf("Need for resistance ? [Y/N]\n");
    if(!scanf("%c[YN]", &needForRes)) return 1;

    printf("Please provide winding dimensions [mm] as follow: \"width-innerDiameter-outerDiameter\"\n");
    printf("(Use 0 for unknowns)\n");
    if(scanf("%lf-%lf-%lf", &widthMm, &innerDiamMm, &outerDiamMm) != 3) return 1;

    if(needForRes == 'Y') {
        printf("Please provide wire diameter[mm] OR section area [mm2] as follow: \"wireDiameter-wireSection\"\n");
        printf("(Use 0 for unknown)\n");
        if(scanf("%lf-%lf", &wireDiamMm, &wireSectionMm2) != 2) return 1;

        printf("Please provide wire length[mm], resistance[ohm] and optionaly material resistivity[ohm*m] as follow: \"wireLength-wireResistance-()\"\n");
        printf("(Default resistivity %lf[ohm*m] (Copper @ 20ºC))\n", COPPER_RESISTIVITY);
        printf("(Use 0 for unknowns)\n");
        if(scanf("%lf-%lf-%lf", &wireLengthMm, &wireResistance, &resistivity) < 2) return 1;

    } else {
        printf("Please provide wire length in [mm]:\n");
        if(scanf("%lf", &wireLengthMm) != 1) return 1;
    }

    //Check for inconsistencies in the variables
    if(widthMm < 0 || innerDiamMm < 0 || outerDiamMm < 0 || wireDiamMm < 0 || wireSectionMm2 < 0 || wireLengthMm < 0 || wireResistance < 0 || resistivity < 0) {
        printf("invalid value");
        return 1;
    }

    if(widthMm > 0)         providedFields |= 0x80;
    if(innerDiamMm > 0)     providedFields |= 0x40;
    if(outerDiamMm > 0)     providedFields |= 0x20;
    if(wireDiamMm > 0)      providedFields |= 0x10;
    if(wireSectionMm2 > 0)  providedFields |= 0x08;
    if(wireLengthMm > 0)    providedFields |= 0x04;
    if(wireResistance > 0)  providedFields |= 0x02;
    if(resistivity > 0)     providedFields |= 0x01;

    if(needForRes == 'Y') {
        char diamResistanceField = providedFields & 0x16; //extract wireDiam, wireLengthMm and wireResistance
        char sectionResistanceField = providedFields & 0x0D; //extract wireSection, wireLengthMm and wireResistance
        if(!(diamResistanceField == 0x06 || diamResistanceField == 0x12 || diamResistanceField == 0x14) || !(sectionResistanceField == 0x06 || sectionResistanceField == 0x0A || sectionResistanceField == 0x0C)) {
            printf("Missing variable(s) for solving resistance equation to determine wire length");
            return 1;
        }
    } else {
        if(!(providedFields & 0x04)) {
            printf("Missing variable for wire length");
            return 1;
        }
    }

    //to be continued with spool dimensions
    



    return 0;
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

