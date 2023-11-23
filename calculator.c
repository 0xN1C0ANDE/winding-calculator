/*
    * @brief Simple winding calculator.
    * @file calculator.c
    * @author Nicolas Andersson
    * @date 21.11.23
    * @todo detection of variables conflicts
*/

#include "calculator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

    printf("Please provide winding turns and dimensions[mm] as follow: \"turns-width-innerDiameter-outerDiameter\"\n");
    printf("(Use 0 for unknowns)\n");
    if(scanf("%lf-%lf-%lf-%lf", &var->windingTurns, &var->widthMm, &var->innerDiamMm, &var->outerDiamMm) != 3) return 1;

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
    if(var->widthMm < 0 || var->innerDiamMm < 0 || var->outerDiamMm < 0 || var->windingTurns < 0 || var->wireDiamMm < 0 || var->wireSectionMm2 < 0 || var->wireLengthMm < 0 || var->wireResistance < 0 || var->resistivity < 0) {
        printf("invalid value");
        return 1;
    }

    if(var->wireDiamMm > 0 && var->wireSectionMm2 > 0) {
        printf("Please provide only wireDiamMm or wireSectionMm2");
        var->wireSectionMm2 = 0;
        return 1;
    }

    //saved what we have
    if(var->widthMm > 0)         var->providedFields |= WIDTHMM_BIT;
    if(var->innerDiamMm > 0)     var->providedFields |= INNERDIAM_BIT;
    if(var->outerDiamMm > 0)     var->providedFields |= OUTERDIAM_BIT;
    if(var->windingTurns > 0)    var->providedFields |= WINDINGTURNS_BIT;
    if(var->wireDiamMm > 0)      var->providedFields |= WIREDIAMMM_BIT;
    if(var->wireSectionMm2 > 0)  var->providedFields |= WIRESECTIONMM2_BIT;
    if(var->wireLengthMm > 0)    var->providedFields |= WIRELENGTHMM_BIT;
    if(var->wireResistance > 0)  var->providedFields |= WIRERESISTANCE_BIT;

    return 0;
}

void solver(variables *var) {
    if(var->needForRes == 'Y') {
        if(bitCounter(var->providedFields) < 6) {
            printf("Too many unknowns");
        } else {
            while(~var->providedFields) {
                printf("Solving with resistance...\n");
                solveEquation_wireDiam_wireSectionMm2(var);
                solveEquation_wireResistance_resistivity_wireLengthMm_wireSectionMm2(var);
                solveEquation_windingTurns_widthMm_innerDiamMm_outerDiamMm_wireDiamMm_wireLengthMm(var);
            }
        }
        
    } else {
        if(bitCounter(var->providedFields) < 5) {
            printf("Too many unknowns");
        } else {
            while(~var->providedFields) {
                printf("Solving...\n");
                solveEquation_wireDiam_wireSectionMm2(var);
                solveEquation_windingTurns_widthMm_innerDiamMm_outerDiamMm_wireDiamMm_wireLengthMm(var);
            }
        }
    }
}

int bitCounter(char bitPattern) {
    //Not counting bit 0 because not interested in resistivity
    int c=0;
    for(int i=1, i<=7, i++) {
        if((bitPattern >> i) & 0x01) c++;
    }
    return c;
}

void solveEquation_wireDiam_wireSectionMm2(variables *var) {
    //Calc wireDiamMm
    if(var->providedFields & WIRESECTIONMM2_BIT && ~var->providedFields & WIREDIAMMM_BIT) {
        var->wireDiamMm = sqrt(var->wireSectionMm2);
        var->providedFields |= WIREDIAMMM_BIT;
    }
    //Calc wireSectionMm2
    if(var->providedFields & WIREDIAMMM_BIT && ~var->providedFields & WIRESECTIONMM2_BIT) {
        var->wireSectionMm2 = var->wireDiamMm*var->wireDiamMm;
        var->providedFields |= WIRESECTIONMM2_BIT;
    }
}

void solveEquation_wireResistance_resistivity_wireLengthMm_wireSectionMm2(variables *var) {
    //Set resistivity
    if(!var->resistivity) var->resistivity = COPPER_RESISTIVITY;

    //Calc wireResistance
    if(var->providedFields & (WIRELENGTHMM_BIT & WIRESECTIONMM2_BIT) && ~var->providedFields & WIRERESISTANCE_BIT) {
        var->wireResistance = var->resistivity*(var->wireLengthMm/var->wireSectionMm2);
        var->providedFields |= WIRERESISTANCE_BIT;
    }
    //Calc wireLengthMm
    if(var->providedFields & (WIRERESISTANCE_BIT & WIRESECTIONMM2_BIT) && ~var->providedFields & WIRELENGTHMM_BIT) {
        var->wireLengthMm = var->wireResistance/var->resistivity*var->wireSectionMm2;
        var->providedFields |= WIRELENGTHMM_BIT;
    }
    //Calc  wireSectionMm2
    if(var->providedFields & (WIRERESISTANCE_BIT & WIRELENGTHMM_BIT) && ~var->providedFields & WIRESECTIONMM2_BIT) {
        var->wireSectionMm2 = var->wireResistance/var->resistivity/var->wireLengthMm;
        var->providedFields |= WIRESECTIONMM2_BIT;
    }
}

void solveEquation_windingTurns_widthMm_innerDiamMm_outerDiamMm_wireDiamMm_wireLengthMm(variables *var) {
    //Calc windingTurns
    if(var->providedFields & (WIDTHMM_BIT & INNERDIAM_BIT & OUTERDIAM_BIT & WIREDIAMMM_BIT & WIRELENGTHMM_BIT) && ~var->providedFields & WINDINGTURNS_BIT) {
        double t = 0;
        double currWindingDiamMM = var->wireDiamMm + var->wireDiamMm;

        while(var->wireLengthMm > 0) {
            double tInLayer = 0;

            if(var->wireLengthMm >= M_PI*currWindingDiamMM*var->widthMm/var->wireDiamMm) {
                //Full layer
                tInLayer = var->widthMm/var->wireDiamMm;
            } else {
                //Partial layer
                tInLayer = var->wireLengthMm/M_PI*currWindingDiamMM;
            }

            t += tInLayer;
            var->wireLengthMm -= M_PI*currWindingDiamMM*tInLayer;
            currWindingDiamMM += var->wireDiamMm;
        }

        var->windingTurns = t;
    }
    //Calc widthMm
    if(var->providedFields & (WINDINGTURNS_BIT & INNERDIAM_BIT & OUTERDIAM_BIT & WIREDIAMMM_BIT & WIRELENGTHMM_BIT) && ~var->providedFields & WIDTHMM_BIT) {
        
    }
    //Calc innerDiamMm
    if(var->providedFields & (WINDINGTURNS_BIT & WIDTHMM_BIT & OUTERDIAM_BIT & WIREDIAMMM_BIT & WIRELENGTHMM_BIT) && ~var->providedFields & WINDINGTURNS_BIT) {
        
    }
    //Calc outerDiamMm
    if(var->providedFields & (WINDINGTURNS_BIT & WIDTHMM_BIT & INNERDIAM_BIT & WIREDIAMMM_BIT & WIRELENGTHMM_BIT) && ~var->providedFields & WIDTHMM_BIT) {
        
    }
    //Calc wireDiamMm
    if(var->providedFields & (WINDINGTURNS_BIT & WIDTHMM_BIT & OUTERDIAM_BIT & INNERDIAM_BIT & WIRELENGTHMM_BIT) && ~var->providedFields & WIDTHMM_BIT) {
        
    }
    //Calc wireLength
    if(var->providedFields & (WINDINGTURNS_BIT & WIDTHMM_BIT & OUTERDIAM_BIT & INNERDIAM_BIT & WIREDIAMMM_BIT) && ~var->providedFields & WIDTHMM_BIT) {
        
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

