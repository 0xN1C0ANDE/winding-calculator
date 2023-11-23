/*
    * @brief Simple winding calculator.
    * @file calculator.h
    * @author Nicolas Andersson
    * @date 22.11.23
*/

#define COPPER_RESISTIVITY 1.68E-8 // roh: ohm-meter
#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

#define WIDTHMM_BIT 0x80
#define INNERDIAM_BIT 0x40
#define OUTERDIAM_BIT 0x20
#define WINDINGTURNS_BIT 0x10
#define WIREDIAMMM_BIT 0x08
#define WIRESECTIONMM2_BIT 0x04
#define WIRELENGTHMM_BIT 0x02
#define WIRERESISTANCE_BIT 0x01

typedef struct Variables {
    char needForRes;
    double widthMm, innerDiamMm, outerDiamMm, windingTurns, wireDiamMm, wireSectionMm2, wireLengthMm, wireResistance, resistivity;
    char providedFields; //excludes resistivity
} variables;

int getVariables(variables *var);
int dataChecker(variables *var);
void solver(variables *var);
void solveEquation_wireDiam_wireSectionMm2(variables *var);
void solveEquation_wireResistance_resistivity_wireLengthMm_wireSectionMm2(variables *var);
void solveEquation_windingTurns_widthMm_innerDiamMm_outerDiamMm_wireDiamMm_wireLengthMm(variables *var);
void printHelpScreen(void);
