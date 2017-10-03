#include "IQMath.h"











_IQ IQMultiply(_IQ Muliplicando, _IQ Multiplicador){
    _IQ result = {0, 0};
    result.Point = Muliplicando.Point + Multiplicador.Point;
    return result;
}


_IQ IQSat(_IQ number){
    return number;
} 