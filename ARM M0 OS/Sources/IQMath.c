#include "IQMath.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long IQMultiply(unsigned int x, unsigned int y, unsigned char qMultiplicando, unsigned char qMultiplicador, unsigned char qUsuario){
        
        unsigned long xlong;
        unsigned long ylong;
        unsigned long result;    
    
        xlong = x;
        ylong = y;
    
        result = x * y;
        
        if(xlong > 0x8000){
            xlong |= 0xFFFF0000;
        }
        if(ylong > 0x8000){
            ylong |= 0xFFFF0000;
        }
        
        if(qUsuario == qMultiplicando + qMultiplicador){
            return result;
        }
        else if(qUsuario > qMultiplicando + qMultiplicador){
            result <<= (qUsuario - (qMultiplicando + qMultiplicador));
            return result;
        }
        else if(qUsuario < qMultiplicando + qMultiplicador){
            result >>= ((qMultiplicando + qMultiplicador) - qUsuario);
            return result;
        }
    return result;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long IQSuma (unsigned int x, unsigned int y,unsigned char qx, unsigned char qy, unsigned char qr){
    if(qr > qx){
        x <<= qr-qx;
    }
    else if(qr < qx){
        x >>= qx-qr;
    }
    if(qr > qy){
        y <<= qr-qy;
    }
    else if(qr < qy){
        y >>= qy-qr;
    }
    return x+y;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long IQResta (unsigned int x, unsigned int y, unsigned char qx, unsigned char qy, unsigned char qr){
    if(qr > qx){
        x <<= qr-qx;
    }
    else if(qr < qx){
        x >>= qx-qr;
    }
    if(qr > qy){
        y <<= qr-qy;
    }
    else if(qr < qy){
        y >>= qy-qr;
    }
    return x-y;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
signed long IQDivide (unsigned int x, unsigned int y, unsigned char qDividendo, unsigned char qDivisor, unsigned char qUsuario){
    unsigned char i = 0;
    unsigned long result;
    unsigned char qResultante;
    

    qDivisor = qDivisor << 16;
    
    qResultante = qDividendo - qDivisor;
    
    result = x/y;
    
    if(qResultante == qUsuario){
        result = result >> 16;
    }
        
    if(qDividendo > qDivisor){
        result = result >> 16;
        result <<= qDividendo-qDivisor;
    }
    
    if(qDivisor > qDividendo){
        result = result >> 16;
        result >>= qDividendo-qDivisor;
    }
    return result;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

_IQ IQSat(_IQ number){
    return number;
} 