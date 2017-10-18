typedef struct{
    signed long Number;
    unsigned char Point;
}   _IQ;

unsigned long IQSuma (unsigned int x, unsigned int y, unsigned char qx, unsigned char qy, unsigned char point);

unsigned long IQResta (unsigned int x, unsigned int y, unsigned char qx, unsigned char qy, unsigned char point);

unsigned long IQMultiply(unsigned int x, unsigned int y, unsigned char qMultiplicando, unsigned char qMultiplicador, unsigned char qUsuario);

signed long IQDivide (unsigned int x, unsigned int y, unsigned char qDividendo, unsigned char qDivisor, unsigned char qUsuario);

_IQ IQSat(_IQ number);