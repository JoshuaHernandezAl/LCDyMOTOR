#include <xc.h>
#include "Configuracion.h"
#include "LCD_libreria.h"
const char stick[]={    0b00010111,
                        0b00010111,
                        0b00010010,
                        0b00001111,
                        0b00000010,
                        0b00000101,
                        0b00001001,
                        0b00000000,0
};
void __interrupt(high_priority) JoshuaTimer0(void);
void LCD_Init(void){
    OpenXLCD(FOUR_BIT & LINES_5X7 );
    while(BusyXLCD()); // LCD esta ocupado
    WriteCmdXLCD(0x06); //Mover el cursor a la derecha
    WriteCmdXLCD(0x0C); //Desactivar el cursor
}
int caratula=0,caratulaAccion=0,i=0;
char bandera=0,lineaAgain=0;

void main(void) {
    //configuracion del timer
    INTCONbits.GIE=0;
    T0CONbits.TMR0ON=0;
    T0CONbits.T0CS=0;
    T0CONbits.T0SE=0;
    T0CONbits.T08BIT=0;
    T0CONbits.PSA=0;
    T0CONbits.T0PS=0x06;
    TMR0=0xF0BE;
    INTCONbits.T0IE=1;
    INTCONbits.T0IF=0;
    
    //Accion reguladas por el timer
    TRISBbits.RB0=1;
    TRISBbits.RB1=0;
    TRISBbits.RB2=0;
    LATBbits.LB1=0;
    LATBbits.LB2=0;
    
    INTCONbits.INT0IE=1;//habilitamos la interrupcion de int0 atraves de la terminal rb0
    INTCON2bits.INTEDG0=1;//asignamos un flanco ascendente para la interrupcion
    INTCONbits.INT0IF=0;//Inicializamos en cero la bandera 
    RCONbits.IPEN=0;//Inhablitamos prioridades, usamos el modo de compatibilidad
    
    INTCONbits.GIE=1;
    T0CONbits.TMR0ON=1;
    
    LCD_Init();
    //LCD_gotoXY(0,0);
    //putrsXLCD("**POKEMON GO!***");
    //__delay_ms(100);
    //LCD_WriteChr_CGRAM(wels,0);
    //__delay_ms(100);
    //LCD_gotoXY(1,0);
    //putcXLCD(0);
    //__delay_ms(100);
    WriteCmdXLCD(0x0C);
    while(1);
    return;
}
void __interrupt(high_priority) JoshuaTimer0(void){
    if(INTCONbits.INT0IF==1){
        if(caratulaAccion%2==0){
            TMR0=0xB3B5;
            bandera=1;
        }else{
            TMR0=0x6769;
            bandera=2;
            WriteCmdXLCD(BORRAR_LCD);
    }
    }else if(INTCONbits.T0IF==1){
        if(caratula==0){
            LCD_gotoXY(0,0);
            putrsXLCD("/IPN-ESIME-uCONTROLADORES->JoshuaHdz/");
            TMR0=0xF0BE;
            caratula++;
        }else if(caratula==1){
            LCD_WriteChr_CGRAM(stick,0);
            TMR0=0xF0BE;
            caratula++;
        }else if(caratula==2){
            LCD_gotoXY(1,0);
            putcXLCD(0);
            TMR0=0xF0BE;
            caratula++;
        }
        if(caratulaAccion%2==0 && i<=39 && bandera==1){
            WriteCmdXLCD(0x18);
            TMR0=0xB3B5;
            i++;
            if(i>39){
                caratulaAccion++;
                i=0;
                bandera=0;
            }
        }else if(caratulaAccion%2!=0 && bandera==2){
            LATBbits.LB1= 1;
            LCD_gotoXY(0,0);
            if(i==0){
                putrsXLCD("3 seg");
            }else if(i==1){
                putrsXLCD("2 seg");
            }else if(i==2){
                putrsXLCD("1 seg");
            }else if(i==0){
                putrsXLCD("0 seg");
            }
            TMR0=0x6769;
            i++;
            if(i>3){
                caratulaAccion+=2;
                i=0;
                bandera=3;
                LATBbits.LB1=0;
                WriteCmdXLCD(BORRAR_LCD);
                TMR0=0xF0BE;
            }
        }else if(bandera==3){
            if(lineaAgain==0){
                LCD_gotoXY(0,0);
                putrsXLCD("Otra?");
                TMR0=0xF0BE;
                lineaAgain=1;
            }else if(lineaAgain==1){
                LCD_gotoXY(1,0);
                putrsXLCD("Pulsa de nuevo=D");
                TMR0=0xF0BE;
                lineaAgain=0;
                bandera=0;
            }
            
        }
    }
    INTCONbits.INT0IF=0;
    INTCONbits.T0IF=0;
}//FUNCION