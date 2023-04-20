#include <math.h>
#include "cell.hpp"


double r_blast_1(int size){return (pow(size, 0.33) * 2.2);}
double r_blast_3(int size){return (pow(size, 0.33) * 1.0);}
double r_blast_5(int size){return (pow(size, 0.33) * 0.71);}
double r_blast_10(int size){return (pow(size, 0.33) * 0.45);}
double r_blast_20(int size){return (pow(size, 0.33) * 0.28);}

bDamage scale(int size, int distance){
    
    if (distance <= round(r_blast_20(size)))
        return psi_20;
    else if (distance <= round(r_blast_10(size)))
        return psi_10;
    else if (distance <= round(r_blast_5(size)))
        return psi_5;
    else if (distance <= round(r_blast_3(size)))
        return psi_3;
    else if (distance <= round(r_blast_1(size)))
        return psi_1;
    else
        return none;
}
