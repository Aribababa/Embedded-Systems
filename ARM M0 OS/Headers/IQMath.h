

typedef struct{
    signed long Number;
    unsigned char Point;
}   _IQ;


_IQ IQMultiply(_IQ Muliplicando, _IQ Multiplicador);

_IQ IQDivide(_IQ Divisor, _IQ Dividendo);

_IQ IQSat(_IQ number);