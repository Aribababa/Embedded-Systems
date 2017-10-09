#include "IQMath.h"











_IQ IQMultiply(_IQ Muliplicando, _IQ Multiplicador, unsigned char point){
    _IQ result = {0, 0};
    
    result.Number = Muliplicando.Number * Multiplicador.Number;
    result.Point = Muliplicando.Point + Multiplicador.Point;
    
    if(point >= result.Point){
        result.Number <<= (point - result.Point);
        result.Point += (point - result.Point);
    } else {
        result.Number >>= (point - result.Point);
        result.Point -= (point - result.Point);
    }  
    return result;
}


_IQ IQSat(_IQ number){
    return number;
} 