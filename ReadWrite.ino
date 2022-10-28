/*
PROYECTO 2 DIGITAL 2

Cristian Catú
Juan Emilio Reyes
 */
//***************************************************************************************************************************************
//   LIBRERIAS
//***************************************************************************************************************************************
#include <SPI.h>
#include <SD.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"

//***************************************************************************************************************************************
// PINES
//***************************************************************************************************************************************
#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};  

//***************************************************************************************************************************************
//VARIABLES
//***************************************************************************************************************************************
uint8_t push1 = 0;
uint8_t push2 = 0;
uint8_t push3 = 0;
uint8_t push4 = 0;
uint8_t push5 = 0;
uint8_t push6 = 0;
uint8_t bandera = 0;
uint8_t bandera1 = 0;
uint16_t modo = 0;
uint8_t y_flecha = 103;
uint8_t x_flecha2 = 40;
uint8_t opcion_menu = 0;
uint8_t opcion_menu1 = 0;
uint8_t opcion_menu2 = 0;
uint8_t opcion_menu3 = 0;
uint8_t bandera_modo4 = 0;
uint8_t f1 = 1;
uint8_t f2 = 1;
uint8_t f3 = 1;
uint8_t f4 = 1;
uint8_t f5 = 1;
uint8_t f6 = 1;
uint8_t f7 = 1;
uint8_t f8 = 1;
uint8_t f9 = 1;
uint8_t f10 = 1;
uint8_t f11 = 1;
uint8_t f12 = 1;
uint8_t f13 = 1;
uint8_t f14 = 1;
uint8_t f15 = 1;
uint8_t f16 = 1;
uint8_t f17 = 1;
uint8_t f18 = 1;
uint8_t f19 = 1;
uint8_t f20 = 1;
uint8_t f21 = 1;
uint8_t f22 = 1;
uint8_t f23 = 1;
uint8_t f24 = 1;
uint8_t dificultad = 1;
int enviar = 0;
int color_jugar;
int tiempo_juego;
uint16_t temp = 0;
String text_tiempo;
uint8_t x=0;
uint8_t fantasma1 = 0;
uint8_t fantasma2 = 0;
uint8_t fantasma3 = 0;
uint8_t fantasma4 = 0;
uint8_t fantasma5 = 0;
uint8_t fantasma6 = 0;
uint8_t fantasma7 = 0;
uint8_t fantasma8 = 0;
uint8_t fantasma9 = 0;
uint8_t fantasma10 = 0;
uint8_t fantasma11 = 0;
uint8_t fantasma12 = 0;
uint8_t fantasma13 = 0;
uint8_t fantasma14 = 0;
uint8_t fantasma15 = 0;
uint8_t fantasma16 = 0;
uint8_t fantasma17 = 0;
uint8_t fantasma18 = 0;
uint8_t fantasma19 = 0;
uint8_t fantasma20 = 0;
uint8_t fantasma21 = 0;
uint8_t fantasma22 = 0;
uint8_t fantasma23 = 0;
uint8_t fantasma24 = 0;
uint16_t x2 = 162;
uint8_t personaje = 0;
uint8_t personaje2 = 0;
uint8_t mapa = 0;
unsigned long Valor_actual;
unsigned long Valor_anterior;
unsigned long millis();
const int tiempo = 20000;
File myFile;
File myFile2;
char contenidoSD;
uint16_t caracter;
unsigned char data[15000]={};
unsigned char data2[10000]={};
int indice = 0;
char inChar;
String Jugador1 = "J1";
String Jugador2 = "J2";
uint8_t p1 = 0;
uint8_t p2 = 0;

//***************************************************************************************************************************************
// Prototipos de función
//***************************************************************************************************************************************
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
void Read_SD(void);
void Read_SD2(void);

//***************************************************************************************************************************************
// SETUP
//***************************************************************************************************************************************
void setup()
{
  pinMode(PD_6,INPUT); //Para comunicación UART con Serial 2
  pinMode(PC_6,INPUT); //Para comunicación UART con Serial 3
    // Frecuencia de reloj (utiliza TivaWare)
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  // Configuración del puerto (utiliza TivaWare)
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  
  LCD_Init(); //Incialización de la LCD de la tiva
  LCD_Clear(0x0000); //Fondo inical del juego

  //Configuración para comunicación de la SD
  SPI.setModule(0); //SPI con el modulo cero
  pinMode(PA_3, OUTPUT); //el chip select de SPI

  if (!SD.begin(PA_3)) { //Verificamos comunicación con SD
    return;
  }

  ////////////////////////////////////  Pantalla Inicial del juego   ////////////////////////////////////////////
  //Titulo del juego
  String text1 = "PAC-MAN";
  String text2 = " GHOSTS RAIN ";
  String text3 = "UVG Games";
  LCD_Print(text1, 110, 40, 2, 0xffff, 0x0000);
  LCD_Print(text2, 117, 60, 1, 0x0000, 0xffc0);
  LCD_Print(text3, 125, 180, 1, 0xffff, 0x0000);

  //logo del juego
  int y_in = 85;
  int x_in = 45;
  myFile = SD.open("pac1.txt");
  Read_SD();
  LCD_Bitmap(x_in, y_in, 35, 36, data);
  myFile = SD.open("pac2.txt");
  Read_SD();
  LCD_Bitmap(x_in + 35, y_in, 35, 36, data);
  myFile = SD.open("pac3.txt");
  Read_SD();
  LCD_Bitmap(x_in, y_in + 36, 36, 38, data);
  myFile = SD.open("pac5.txt");
  Read_SD();
  LCD_Bitmap(x_in + 35, y_in + 36, 35, 38, data);
  int x_fan = 120;
  int y_fan = 100;
  myFile = SD.open("rojo.txt");
  Read_SD();
  LCD_Bitmap(x_fan, y_fan, 38, 42, data);
  myFile = SD.open("azul.txt");
  Read_SD();
  LCD_Bitmap(x_fan + 38, y_fan-10, 37, 45, data);
  myFile = SD.open("verde.txt");
  Read_SD();
  LCD_Bitmap(x_fan + 38+37, y_fan, 36, 48, data);
  myFile = SD.open("rosa.txt");
  Read_SD();
  LCD_Bitmap(x_fan + 38+37+36, y_fan-10, 40, 47, data);
  delay(5000);

  ///////////////////////////////////////// MENU PRINCIPAL /////////////////////////////////////////////
  LCD_Clear(0x0000);
  LCD_Print(text1, 110, 40, 2, 0xffff, 0x0000);
  LCD_Print(text2, 117, 60, 1, 0x0000, 0xffc0);
  
  String text4 = "Elegir personaje";
  String text5 = "Elegir dificultad";
  String text6 = "Elegir mapa";
  String text7 = "Jugar";
  LCD_Print(text4, 100, 100, 1, 0xffff, 0x0000);
  LCD_Print(text5, 100, 120, 1, 0xffff, 0x0000);
  LCD_Print(text6, 100, 140, 1, 0xffff, 0x0000);
  LCD_Print(text7, 100, 160, 1, 0xffff, 0x0000);

  Rect(20, 20, 280, 200, 0xffff);
  Rect(10, 10, 300, 220, 0xffff);
  FillRect(88, y_flecha, 6, 6, 0xffff);
}


//***************************************************************************************************************************************
// LOOP principal
//***************************************************************************************************************************************

void loop(){
  /////////////////////////// MODO JUEGO ///////////////////////////////////
  if(modo == 4){ // Verificamos si estamos en modo juego
    if(bandera_modo4 == 0){ //una bandera de inicialización
      Valor_anterior = millis(); //Leemos el tiempo de la programación
      Valor_actual = millis();
      //Verificamos el mapa escogido
      if(mapa == 0){ 
        myFile = SD.open("Mapa1.txt");
        Read_SD();
      }
      else if(mapa == 1){
        myFile = SD.open("Mapa2.txt");
        Read_SD();
      }
      else if(mapa == 2){
        myFile = SD.open("Mapa3.txt");
        Read_SD();
      }
      
      for(int x3 = 0; x3 <289; x3+=32){
        LCD_Bitmap(x3, 240-32, 32, 32, data);
      }

      // Linea para dividir la pantalla
      V_line(160, 0, 240, 0x0000);
      V_line(161, 0, 240, 0x0000);
      bandera_modo4 = 1;
      //  Verificamos el personaje escogido del primer jugador
      if(personaje == 0){
        myFile = SD.open("Pacman.txt");
        Read_SD();
      }
      else if(personaje == 1){
        myFile = SD.open("Dedede.txt");
        Read_SD();
      }
      else if(personaje == 2){
        myFile = SD.open("Sonic.txt");
        Read_SD();
      }
      else if(personaje == 3){
        myFile = SD.open("spidey.txt");
        Read_SD();
      }
      else if(personaje == 4){
        myFile = SD.open("megaman.txt");
        Read_SD();
      }
      else if(personaje == 5){
        myFile = SD.open("kirby.txt");
        Read_SD();
      }

      // Verificamos el personaje escogido del segundo jugador
      if(personaje2 == 0){
        myFile2 = SD.open("Pacman.txt");
        Read_SD2();
      }
      else if(personaje2 == 1){
        myFile2 = SD.open("Dedede.txt");
        Read_SD2();
      }
      else if(personaje2 == 2){
        myFile2 = SD.open("Sonic.txt");
        Read_SD2();
      }
      else if(personaje2 == 3){
        myFile2 = SD.open("spidey.txt");
        Read_SD2();
      }
      else if(personaje2 == 4){
        myFile2 = SD.open("megaman.txt");
        Read_SD2();
      }
      else if(personaje2 == 5){
        myFile2 = SD.open("kirby.txt");
        Read_SD2();
      }
      LCD_Sprite(x, 240-63, 30, 30, data, 4, 1, 0, 0);
      LCD_Sprite(x2, 240-63, 30, 30, data2, 4, 1, 0, 0);
    }
    
    ///////////////// JUEGO mientras no haya acabado el tiempo //////////////////
    if(Valor_actual-Valor_anterior <= tiempo){
      Valor_actual = millis(); /////////// Actualizamos el valor actual
      tiempo_juego = (Valor_actual-Valor_anterior)/1000; // Vemos el tiempo del juego en segundos
      // Imprimimos el tiempo
      if(tiempo/1000-tiempo_juego < 10){
        text_tiempo = "0"+String(tiempo/1000-tiempo_juego);
      }
      else{
        text_tiempo = String(tiempo/1000-tiempo_juego);
      }

      ///////////////////// FANTASMAS JUGADOR1 ///////////////////////////////////////
      if((tiempo_juego >= 2) && f1){ // Verificamos si se cumplió el tiempo del fantasma y si ya salió en la partida
        FillRect(20, fantasma1, 20, dificultad, 0x1c0e); //Pequeño rectangulo del grosor de la dificultad según se mueva el fantasma
        fantasma1 = fantasma1 + dificultad; // Aumentamos la posicion vertical del fantasma
        LCD_Bitmap(20,fantasma1,20,20,bitmap_fantasma1); // Imprimimos el fantasma
        if (fantasma1 >= 186){ // Si llegó al piso lo borramos y quitamos la bandera para que no se muestre otra vez
          FillRect(20, fantasma1, 20, 20, 0x1c0e); //Borrar fantasma
          f1 = 0; // Quitar bandera en uno para que no se muestre
        }
        else if((fantasma1 >= 159)&&((x >= 0)&& (x <= 40))){ //Si llegó a la posición de jugador 1 y el jugador está en cierto rango de posición
          FillRect(20, fantasma1, 20, 20, 0x1c0e); //Borrar fantasma
          f1 = 0;  // Quitar bandera para que no se muestre otra vez
          p1++; //Sumar al marcador
        }
      }
      //Se repite todo otra vez para los 24 fantasmas
      if((tiempo_juego >= 3) && f2){
        FillRect(100, fantasma2, 20, dificultad, 0x1c0e);
        fantasma2 = fantasma2 + dificultad;
        LCD_Bitmap(100,fantasma2,20,20,bitmap_fantasma2);
        if (fantasma2 >= 186){
          FillRect(100, fantasma2, 20, 20, 0x1c0e);
          f2 = 0;
        }
        else if((fantasma2 >= 159)&&((x >= 70)&& (x <= 120))){
          FillRect(100, fantasma2, 20, 20, 0x1c0e);
          f2 = 0;
          p1++;
        }
      }
      if((tiempo_juego >= 4) && f3){
        FillRect(80, fantasma3, 20, dificultad, 0x1c0e);
        fantasma3 = fantasma3 + dificultad;
        LCD_Bitmap(80,fantasma3,20,20,bitmap_fantasma3);
        if (fantasma3 >= 186){
          FillRect(80, fantasma3, 20, 20, 0x1c0e);
          f3 = 0;
        }
        else if((fantasma3 >= 159)&&((x >= 50)&& (x <= 100))){
          FillRect(80, fantasma3, 20, 20, 0x1c0e);
          f3 = 0;
          p1++;
        }
      }
      if((tiempo_juego >= 5) && f4){
        FillRect(30, fantasma4, 20, dificultad, 0x1c0e);
        fantasma4 = fantasma4 + dificultad;
        LCD_Bitmap(30,fantasma4,20,20,bitmap_fantasma4);
        if (fantasma4 >= 186){
          FillRect(30, fantasma4, 20, 20, 0x1c0e);
          f4 = 0;
        }
        else if((fantasma4 >= 159)&&((x >= 0)&& (x <= 50))){
          FillRect(30, fantasma4, 20, 20, 0x1c0e);
          f4 = 0;
          p1++;
        }
      }
      if((tiempo_juego >= 7) && f5){
        FillRect(50, fantasma5, 20, dificultad, 0x1c0e);
        fantasma5 = fantasma5 + dificultad;
        LCD_Bitmap(50,fantasma5,20,20,bitmap_fantasma2);
        if (fantasma5 >= 186){
          FillRect(50, fantasma5, 20, 20, 0x1c0e);
          f5 = 0;
        }
        else if((fantasma5 >= 159)&&((x >= 20)&& (x <= 70))){
          FillRect(50, fantasma5, 20, 20, 0x1c0e);
          f5 = 0;
          p1++;
        }
      }
      if((tiempo_juego >= 8) && f6){
        FillRect(135, fantasma6, 20, dificultad, 0x1c0e);
        fantasma6 = fantasma6 + dificultad;
        LCD_Bitmap(135,fantasma6,20,20,bitmap_fantasma1);
        if (fantasma6 >= 186){
          FillRect(135, fantasma6, 20, 20, 0x1c0e);
          f6 = 0;
        }
        else if((fantasma6 >= 159)&&((x >= 105)&& (x <= 155))){
          FillRect(135, fantasma6, 20, 20, 0x1c0e);
          f6 = 0;
          p1++;
        }
      }
      if((tiempo_juego >= 9) && f7){
        FillRect(110, fantasma7, 20, dificultad, 0x1c0e);
        fantasma7 = fantasma7 + dificultad;
        LCD_Bitmap(110,fantasma7,20,20,bitmap_fantasma1);
        if (fantasma7 >= 186){
          FillRect(110, fantasma7, 20, 20, 0x1c0e);
          f7 = 0;
        }
        else if((fantasma7 >= 159)&&((x >= 80)&& (x <= 130))){
          FillRect(110, fantasma7, 20, 20, 0x1c0e);
          f7 = 0;
          p1++;
        }
      }
      if((tiempo_juego >= 11) && f8){
        FillRect(75, fantasma8, 20, dificultad, 0x1c0e);
        fantasma8 = fantasma8 + dificultad;
        LCD_Bitmap(75,fantasma8,20,20,bitmap_fantasma2);
        if (fantasma8 >= 186){
          FillRect(75, fantasma8, 20, 20, 0x1c0e);
          f8 = 0;
        }
        else if((fantasma8 >= 159)&&((x >= 45)&& (x <= 95))){
          FillRect(75, fantasma8, 20, 20, 0x1c0e);
          f8 = 0;
          p1++;
        }
      }
      if((tiempo_juego >= 12) && f9){
        FillRect(10, fantasma9, 20, dificultad, 0x1c0e);
        fantasma9 = fantasma9 + dificultad;
        LCD_Bitmap(10,fantasma9,20,20,bitmap_fantasma3);
        if (fantasma9 >= 186){
          FillRect(10, fantasma9, 20, 20, 0x1c0e);
          f9 = 0;
        }
        else if((fantasma9 >= 159)&&((x >= 0)&& (x <= 30))){
          FillRect(10, fantasma9, 20, 20, 0x1c0e);
          f9 = 0;
          p1++;
        }
      }
      if((tiempo_juego >= 15) && f10){
        FillRect(55, fantasma10, 20, dificultad, 0x1c0e);
        fantasma10 = fantasma10 + dificultad;
        LCD_Bitmap(55,fantasma10,20,20,bitmap_fantasma4);
        if (fantasma10 >= 186){
          FillRect(55, fantasma10, 20, 20, 0x1c0e);
          f10 = 0;
        }
        else if((fantasma10 >= 159)&&((x >= 25)&& (x <= 75))){
          FillRect(55, fantasma10, 20, 20, 0x1c0e);
          f10 = 0;
          p1++;
        }
      }
      if((tiempo_juego >= 16) && f11){
        FillRect(100, fantasma11, 20, dificultad, 0x1c0e);
        fantasma11 = fantasma11 + dificultad;
        LCD_Bitmap(100,fantasma11,20,20,bitmap_fantasma2);
        if (fantasma11 >= 186){
          FillRect(100, fantasma11, 20, 20, 0x1c0e);
          f11 = 0;
        }
        else if((fantasma11 >= 28)&&((x >= 70)&& (x <= 120))){
          FillRect(100, fantasma11, 20, 20, 0x1c0e);
          f11 = 0;
          p1++;
        }
      }
      if((tiempo_juego >= 15) && f12){
        FillRect(20, fantasma12, 20, dificultad, 0x1c0e);
        fantasma12 = fantasma12 + dificultad;
        LCD_Bitmap(20,fantasma12,20,20,bitmap_fantasma1);
        if (fantasma12 >= 186){
          FillRect(20, fantasma12, 20, 20, 0x1c0e);
          f12 = 0;
        }
        else if((fantasma12 >= 159)&&((x >= 0)&& (x <= 40))){
          FillRect(20, fantasma12, 20, 20, 0x1c0e);
          f12 = 0;
          p1++;
        }
      }

//////////////////////// FANTASMAS JUGADOR2 /////////////////////////////////
    if((tiempo_juego >= 3) && f13){
        FillRect(200, fantasma13, 20, dificultad, 0x1c0e);
        fantasma13 = fantasma13 + dificultad;
        LCD_Bitmap(200,fantasma13,20,20,bitmap_fantasma3);
        if (fantasma13 >= 186){
          FillRect(200, fantasma13, 20, 20, 0x1c0e);
          f13 = 0;
        }
        else if((fantasma13 >= 159)&&((x2 >= 170)&& (x2 <= 220))){
          FillRect(200, fantasma13, 20, 20, 0x1c0e);
          f13 = 0;
          p2++;
        }
      }
      if((tiempo_juego >= 5) && f14){
        FillRect(190, fantasma14, 20, dificultad, 0x1c0e);
        fantasma14 = fantasma14 + dificultad;
        LCD_Bitmap(190,fantasma14,20,20,bitmap_fantasma2);
        if (fantasma14 >= 186){
          FillRect(190, fantasma14, 20, 20, 0x1c0e);
          f14 = 0;
        }
        else if((fantasma14 >= 159)&&((x2 >= 160)&& (x2 <= 210))){
          FillRect(190, fantasma14, 20, 20, 0x1c0e);
          f14 = 0;
          p2++;
        }
      }
      if((tiempo_juego >= 6) && f15){
        FillRect(250, fantasma15, 20, dificultad, 0x1c0e);
        fantasma15 = fantasma15 + dificultad;
        LCD_Bitmap(250,fantasma15,20,20,bitmap_fantasma1);
        if (fantasma15 >= 186){
          FillRect(250, fantasma15, 20, 20, 0x1c0e);
          f15 = 0;
        }
        else if((fantasma15 >= 159)&&((x2 >= 220)&& (x2 <= 270))){
          FillRect(250, fantasma15, 20, 20, 0x1c0e);
          f15 = 0;
          p2++;
        }
      }
      if((tiempo_juego >= 7) && f16){
        FillRect(240, fantasma16, 20, dificultad, 0x1c0e);
        fantasma16 = fantasma16 + dificultad;
        LCD_Bitmap(240,fantasma16,20,20,bitmap_fantasma4);
        if (fantasma16 >= 186){
          FillRect(240, fantasma16, 20, 20, 0x1c0e);
          f16 = 0;
        }
        else if((fantasma16 >= 159)&&((x2 >= 210)&& (x2 <= 260))){
          FillRect(240, fantasma16, 20, 20, 0x1c0e);
          f16 = 0;
          p2++;
        }
      }
      if((tiempo_juego >= 8) && f17){
        FillRect(255, fantasma17, 20, dificultad, 0x1c0e);
        fantasma17 = fantasma17 + dificultad;
        LCD_Bitmap(255,fantasma17,20,20,bitmap_fantasma2);
        if (fantasma17 >= 186){
          FillRect(255, fantasma17, 20, 20, 0x1c0e);
          f17 = 0;
        }
        else if((fantasma17 >= 159)&&((x2 >= 225)&& (x2 <= 275))){
          FillRect(255, fantasma17, 20, 20, 0x1c0e);
          f17 = 0;
          p2++;
        }
      }
      if((tiempo_juego >= 9) && f18){
        FillRect(210, fantasma18, 20, dificultad, 0x1c0e);
        fantasma18 = fantasma18 + dificultad;
        LCD_Bitmap(210,fantasma18,20,20,bitmap_fantasma1);
        if (fantasma18 >= 186){
          FillRect(210, fantasma18, 20, 20, 0x1c0e);
          f18 = 0;
        }
        else if((fantasma18 >= 159)&&((x2 >= 180)&& (x2 <= 230))){
          FillRect(210, fantasma18, 20, 20, 0x1c0e);
          f18 = 0;
          p2++;
        }
      }
      if((tiempo_juego >= 11) && f19){
        FillRect(270, fantasma19, 20, dificultad, 0x1c0e);
        fantasma19 = fantasma19 + dificultad;
        LCD_Bitmap(270,fantasma19,20,20,bitmap_fantasma3);
        if (fantasma19 >= 186){
          FillRect(270, fantasma19, 20, 20, 0x1c0e);
          f19 = 0;
        }
        else if((fantasma19 >= 159)&&((x2 >= 240)&& (x2 <= 290))){
          FillRect(270, fantasma19, 20, 20, 0x1c0e);
          f19 = 0;
          p2++;
        }
      }
      if((tiempo_juego >= 13) && f20){
        FillRect(180, fantasma20, 20, dificultad, 0x1c0e);
        fantasma20 = fantasma20 + dificultad;
        LCD_Bitmap(180,fantasma20,20,20,bitmap_fantasma4);
        if (fantasma20 >= 186){
          FillRect(180, fantasma20, 20, 20, 0x1c0e);
          f20 = 0;
        }
        else if((fantasma20 >= 159)&&((x2 >= 150)&& (x2 <= 200))){
          FillRect(180, fantasma20, 20, 20, 0x1c0e);
          f20 = 0;
          p2++;
        }
      }
      if((tiempo_juego >= 14) && f21){
        FillRect(250, fantasma21, 20, dificultad, 0x1c0e);
        fantasma21 = fantasma21 + dificultad;
        LCD_Bitmap(250,fantasma21,20,20,bitmap_fantasma1);
        if (fantasma21 >= 186){
          FillRect(250, fantasma21, 20, 20, 0x1c0e);
          f21 = 0;
        }
        else if((fantasma21 >= 159)&&((x2 >= 220)&& (x2 <= 270))){
          FillRect(250, fantasma21, 20, 20, 0x1c0e);
          f21 = 0;
          p2++;
        }
      }
      if((tiempo_juego >= 16) && f22){
        FillRect(270, fantasma22, 20, dificultad, 0x1c0e);
        fantasma22 = fantasma22 + dificultad;
        LCD_Bitmap(270,fantasma22,20,20,bitmap_fantasma2);
        if (fantasma22 >= 186){
          FillRect(270, fantasma22, 20, 20, 0x1c0e);
          f22 = 0;
        }
        else if((fantasma22 >= 159)&&((x2 >= 240)&& (x2 <= 290))){
          FillRect(270, fantasma22, 20, 20, 0x1c0e);
          f22 = 0;
          p2++;
        }
      }
      if((tiempo_juego >= 17) && f23){
        FillRect(190, fantasma23, 20, dificultad, 0x1c0e);
        fantasma23 = fantasma23 + dificultad;
        LCD_Bitmap(190,fantasma23,20,20,bitmap_fantasma1);
        if (fantasma23 >= 186){
          FillRect(190, fantasma23, 20, 20, 0x1c0e);
          f23 = 0;
        }
        else if((fantasma23 >= 159)&&((x2 >= 160)&& (x2 <= 210))){
          FillRect(190, fantasma23, 20, 20, 0x1c0e);
          f23 = 0;
          p2++;
        }
      }
      if((tiempo_juego >= 14) && f24){
        FillRect(230, fantasma24, 20, dificultad, 0x1c0e);
        fantasma24 = fantasma24 + dificultad;
        LCD_Bitmap(230,fantasma24,20,20,bitmap_fantasma4);
        if (fantasma24 >= 186){
          FillRect(230, fantasma24, 20, 20, 0x1c0e);
          f24 = 0;
        }
        else if((fantasma24 >= 159)&&((x2 >= 200)&& (x2 <= 250))){
          FillRect(230, fantasma24, 20, 20, 0x1c0e);
          f24 = 0;
          p2++;
        }
      }

      //Imprimimos los marcadores
      LCD_Print(text_tiempo, 145, 20, 2, 0x0000, 0xfc83);
      if (p1 <= 9){
        LCD_Print("0"+String(p1), 10, 10, 1, 0xffff, 0xf800);
      }
      else{
        LCD_Print(String(p1), 10, 10, 1, 0xffff, 0xf800);
      }
      if (p2 <= 9){
        LCD_Print("0"+String(p2), 290, 10, 1, 0xffff, 0x101f);
      }
      else{
        LCD_Print(String(p2), 290, 10, 1, 0xffff, 0x101f);
      }

      ///////////////////////// /Movemos los jugadores ///////////////////
      if(push4 == 1){ ////////////Botón 1 del juador 2 presionado
        if(x>0){
          x--;
          int index_roll = (x/20) % 3; // El módulo debe ser igual a la cantidad de columnas de su sprite.
          LCD_Sprite(x, 240-63, 30, 30, data, 4, index_roll, 1, 0); //Mostramos el Sprite
          V_line(x + 31, 240-63, 30, 0x1c0e);//////    Borramos linea anterior
        }
        
      }
      else if(push5 == 1){     ////////////Botón 2 del juador 2 presionado
        if(x<129){
          x++;
          int animationSpeed = (x/20) % 3; // El módulo debe ser igual a la cantidad de columnas de su sprite.
          LCD_Sprite(x, 240-63, 30, 30, data, 4, animationSpeed, 0, 0);
          V_line(x - 1, 240-63, 30, 0x1c0e);
        }
      }
      
      if(push1 == 1){ ////////////Botón 1 del juador 1 presionado
        if(x2>162){
          x2--;
          int index_roll = (x2/20) % 3; // El módulo debe ser igual a la cantidad de columnas de su sprite. 
          LCD_Sprite(x2, 240-63, 30, 30, data2, 4, index_roll, 1, 0); //Mostramos el Sprite
          V_line(x2 + 31, 240-63, 30, 0x1c0e); //////    Borramos linea anterior
        }
        
      }
      else if(push2 == 1){    ////////////Botón 2 del juador 1 presionado
        if(x2<290){
          x2++;
          int animationSpeed = (x2/20) % 3; // El módulo debe ser igual a la cantidad de columnas de su sprite.
          LCD_Sprite(x2, 240-63, 30, 30, data2, 4, animationSpeed, 0, 0);
          V_line(x2 - 1, 240-63, 30, 0x1c0e);
        }
      }      
    }
    else{
      /////////// TERMINÓ EL JUEGO //////////////////////////
      // Habilitamos fantasmas para otra partida
      f1 = 1;
      f2 = 1;
      f3 = 1;
      f4 = 1;
      f5 = 1;
      f6 = 1;
      f7 = 1;
      f8= 1;
      f9 = 1;
      f10 = 1;
      f11 = 1;
      f12 = 1;
      f13 = 1;
      f14 = 1;
      f15 = 1;
      f16 = 1;
      f17 = 1;
      f18 = 1;
      f19 = 1;
      f20 = 1;
      f21 = 1;
      f22 = 1;
      f23 = 1;
      f24 = 1;

      // reseteamos la posicion de los fantasmas para otra partida
      fantasma1 = 0;
      fantasma2 = 0;
      fantasma3 = 0;
      fantasma4 = 0;
      fantasma5 = 0;
      fantasma6 = 0;
      fantasma7 = 0;
      fantasma8 = 0;
      fantasma9 = 0;
      fantasma10 = 0;
      fantasma11 = 0;
      fantasma12 = 0;
      fantasma13 = 0;
      fantasma14 = 0;
      fantasma15 = 0;
      fantasma16 = 0;
      fantasma17 = 0;
      fantasma18 = 0;
      fantasma19 = 0;
      fantasma20 = 0;
      fantasma21 = 0;
      fantasma22 = 0;
      fantasma23 = 0;
      fantasma24 = 0;

      ///////// Imprimimos resultado final según el resultado ////////////////////
      String resultado;
      int color_resultado;
      if(p1 < p2){
        resultado = "GANA JUGADOR 2";
        color_resultado = 0x001f;
      }
      else if(p1 > p2){
        resultado = "GANA JUGADOR 1";
        color_resultado = 0xf800;
      }
      else {
        resultado = "    EMPATE    ";
        color_resultado = 0x06C2;
        
      }
      FillRect(30, 50, 260, 140, color_resultado);
      LCD_Print(resultado, 46, 115, 2, 0xffff, color_resultado);
      p1 = 0;
      p2 = 0;
      delay(3000); ///////////// Esperamos tres segundos ///////////////////////

      // Volvemos al menú y mostramos todo otra vez
      bandera_modo4 = 0;
      modo = 0;
      LCD_Clear(0x0000);
      String text1 = "PAC-MAN";
      String text2 = " GHOSTS RAIN ";
      LCD_Print(text1, 110, 40, 2, 0xffff, 0x0000);
      LCD_Print(text2, 117, 60, 1, 0x0000, 0xffc0);
      
      String text4 = "Elegir personaje";
      String text5 = "Elegir dificultad";
      String text6 = "Elegir mapa";
      String text7 = "Jugar";
      LCD_Print(text4, 100, 100, 1, 0xffff, 0x0000);
      LCD_Print(text5, 100, 120, 1, 0xffff, 0x0000);
      LCD_Print(text6, 100, 140, 1, 0xffff, 0x0000);
      LCD_Print(text7, 100, 160, 1, 0xffff, 0x0000);
    
      Rect(20, 20, 280, 200, 0xffff);
      Rect(10, 10, 300, 220, 0xffff);
      FillRect(88, y_flecha, 6, 6, 0xffff);
    }
  }
}


//***************************************************************************************************************************************
// Comunicación PIC1
//***************************************************************************************************************************************
void serialEvent() {
  while (Serial3.available()) {
    // obtener el caracter enviado desde el PIC
    inChar = (char)Serial3.read();
    ////  Dependiendo del caracter me indica que boton se presionó o se soltó
    if (inChar == 'A'){
      push4 = 1;
    }
    else if (inChar == 'B'){
      push4 = 0;
    }
    else if (inChar == 'C'){
      push5 = 1;
    }
    else if (inChar == 'D'){
      push5 = 0;
    }
    else if (inChar == 'E'){
      push6 = 1;
    }
    else if (inChar == 'F'){
      push6 = 0;
    }
    ///////////////////////// MENÚ PRINICPAL ///////////////////////////
    if (modo == 0){
      // Limitamos las opciones a escoger en el menú según el botón
      if((push4 == 1) && (opcion_menu < 3)){ // presionó el primer botón
        opcion_menu++; //Aumentamos la opcion del menu
        FillRect(88, y_flecha, 6, 6, 0x0000); // Limpiamos la posicion anterior de la flecha
        y_flecha = y_flecha + 20; // Aumentamos posicion de la flecha
        FillRect(88, y_flecha, 6, 6, 0xffff);
      }
      else if((push5 == 1)&& (opcion_menu > 0)){ //presiono el segundo botón
        opcion_menu--; //Disminuimos la opcion del menu
        FillRect(88, y_flecha, 6, 6, 0x0000); // Limpiamos la posicion anterior de la flecha
        y_flecha = y_flecha - 20; //Disminuimos la posicion de la flecha
        FillRect(88, y_flecha, 6, 6, 0xffff);
      }
      else if(push6 == 1){ //opcion elegir presonaje
        if (opcion_menu == 0){ //Opcion 1

          //Imprimimos el menú a mostrar
          LCD_Clear(0x0000);
          modo = 1;
          String text8 = "Elija personaje";
          LCD_Print(text8, 40, 15, 2, 0xffff, 0x0000);
          String personaje1 = "P1";
          LCD_Print(personaje1, 50, 100, 1, 0xffff, 0x0000);
          String personaje2 = "P2";
          LCD_Print(personaje2, 150, 100, 1, 0xffff, 0x0000);
          String personaje3 = "P3";
          LCD_Print(personaje3, 250, 100, 1, 0xffff, 0x0000);
          String personaje4 = "P4";
          LCD_Print(personaje4, 50, 190, 1, 0xffff, 0x0000);
          String personaje5 = "P5";
          LCD_Print(personaje5, 150, 190, 1, 0xffff, 0x0000);
          String personaje6 = "P6";
          LCD_Print(personaje6, 250, 190, 1, 0xffff, 0x0000);
          String personaje7= "Atras";
          LCD_Print(personaje7, 270, 220, 1, 0x0000, 0xffc0);
          FillRect(40, 103, 6, 6, 0xffff);
          myFile = SD.open("Pacman1.txt");
          Read_SD();
          LCD_Bitmap(35, 50, 41, 41, data);
          myFile = SD.open("Dedede1.txt");
          Read_SD();
          LCD_Bitmap(135, 50, 41, 41, data);
          myFile = SD.open("Sonic1.txt");
          Read_SD();
          LCD_Bitmap(235, 55, 41, 41, data);

          myFile = SD.open("spidey1.txt");
          Read_SD();
          LCD_Bitmap(35, 140, 41, 41, data);
          myFile = SD.open("megaman1.txt");
          Read_SD();
          LCD_Bitmap(135, 140, 41, 41, data);
          myFile = SD.open("kirby1.txt");
          Read_SD();
          LCD_Bitmap(235, 145, 41, 41, data);

          
        }
        else if (opcion_menu == 1){ //opcion elegir dificultad
          // imprmimos el menú a mostrar
          LCD_Clear(0x0000);
          modo = 2;
          String text9 = "Elija dificultad";
          LCD_Print(text9, 40, 25, 2, 0xffff, 0x0000);
          String dificultad1 = "Facil";
          LCD_Print(dificultad1, 50, 170, 1, 0xffff, 0x0000);
          String dificultad2 = "Medio";
          LCD_Print(dificultad2, 140, 170, 1, 0xffff, 0x0000);
          String dificultad3 = "Dificl";
          LCD_Print(dificultad3, 230, 170, 1, 0xffff, 0x0000);
          String dificultad4 = "Atras";
          LCD_Print(dificultad4, 270, 220, 1, 0x0000, 0xffc0);
          FillRect(40, 173, 6, 6, 0xffff);
          myFile = SD.open("Facil.txt");
          Read_SD();
          LCD_Bitmap(50, 110, 40, 40, data);
          myFile = SD.open("Medio.txt");
          Read_SD();
          LCD_Bitmap(140, 110, 40, 40, data);
          myFile = SD.open("Dificil.txt");
          Read_SD();
          LCD_Bitmap(235, 110, 40, 40, data);
        }
        else if (opcion_menu == 2){
          // imprimimos el menú a mostrar
          LCD_Clear(0x0000);
          modo = 3;
          String text10 = "Elija mapa";
          LCD_Print(text10, 80, 25, 2, 0xffff, 0x0000);
          String mapa1 = "Mapa 1";
          LCD_Print(mapa1, 50, 170, 1, 0xffff, 0x0000);
          String mapa2 = "Mapa 2";
          LCD_Print(mapa2, 140, 170, 1, 0xffff, 0x0000);
          String mapa3 = "Mapa 3";
          LCD_Print(mapa3, 230, 170, 1, 0xffff, 0x0000);
          String mapa4 = "Atras";
          LCD_Print(mapa4, 270, 220, 1, 0x0000, 0xffc0);
          FillRect(40, 173, 6, 6, 0xffff);
          myFile = SD.open("Mapa1.txt");
          Read_SD();
          LCD_Bitmap(55, 120, 32, 32, data);
          myFile = SD.open("Mapa2.txt");
          Read_SD();
          LCD_Bitmap(145, 120, 32, 32, data);
          myFile = SD.open("Mapa3.txt");
          Read_SD();
          LCD_Bitmap(235, 120, 32, 32, data);
        }
        else if (opcion_menu == 3){ // Solo pasamos al modo juego
          LCD_Clear(0x1c0e);
          modo = 4; // modo juego

        }
      }
    }
    ///////////////////////// MENU ELEGIR PERSONAJE ///////////////////////////
    else if (modo == 1){
      if(push6 == 1){ // tercer botón elige el presonaje e imprimimos la linea en donde escogimos
        if (opcion_menu1 == 0){
          personaje = 0;
          H_line(30, 115, 290, 0x0000);
          H_line(30, 205, 290, 0x0000);
          H_line(30, 115, 45, 0xffff);
        }
        else if (opcion_menu1 == 1){
          personaje = 1;
          H_line(30, 115, 280, 0x0000);
          H_line(30, 205, 280, 0x0000);
          H_line(130, 115, 45, 0xffff);
        }
        else if (opcion_menu1 == 2){
          personaje = 2;
          H_line(30, 115, 290, 0x0000);
          H_line(30, 205, 290, 0x0000);
          H_line(230, 115, 45, 0xffff);
        }
        else if (opcion_menu1 == 3){
          personaje = 3;
          H_line(30, 115, 290, 0x0000);
          H_line(30, 205, 290, 0x0000);
          H_line(30, 205, 45, 0xffff);
        }
        else if (opcion_menu1 == 4){
          personaje = 4;
          H_line(30, 115, 290, 0x0000);
          H_line(30, 205, 290, 0x0000);
          H_line(130, 205, 45, 0xffff);
        }
        else if (opcion_menu1 == 5){
          personaje = 5;
          H_line(30, 115, 290, 0x0000);
          H_line(30, 205, 290, 0x0000);
          H_line(230, 205, 45, 0xffff);
        }
        else if (opcion_menu1 == 6){// si escogemos esto volvemos al menú principal
          opcion_menu1 = 0;
          modo = 0;
          LCD_Clear(0x0000);
          String text1 = "PAC-MAN";
          String text2 = " GHOSTS RAIN ";
          LCD_Print(text1, 110, 40, 2, 0xffff, 0x0000);
          LCD_Print(text2, 117, 60, 1, 0x0000, 0xffc0);
          
          String text4 = "Elegir personaje";
          String text5 = "Elegir dificultad";
          String text6 = "Elegir mapa";
          String text7 = "Jugar";
          LCD_Print(text4, 100, 100, 1, 0xffff, 0x0000);
          LCD_Print(text5, 100, 120, 1, 0xffff, 0x0000);
          LCD_Print(text6, 100, 140, 1, 0xffff, 0x0000);
          LCD_Print(text7, 100, 160, 1, 0xffff, 0x0000);
        
          Rect(20, 20, 280, 200, 0xffff);
          Rect(10, 10, 300, 220, 0xffff);
          FillRect(88, y_flecha, 6, 6, 0xffff);
        }
      }
      else{
        if((push4 == 1) && (opcion_menu1 < 6)){// si presionamos el primer botón aumentamos la opcion
          opcion_menu1++;
        }
        else if((push5 == 1) && (opcion_menu1 > 0)){ // si presionamos el segundo boton disminuimos la opcion
          opcion_menu1--;
        }
        // Imprimimos la opcion segun la opcion que deseamos
        if(opcion_menu1 == 0){
          FillRect(40, 103, 6, 6, 0xffff);
          FillRect(140, 103, 6, 6, 0x0000);
        }
        else if(opcion_menu1 == 1){
          FillRect(40, 103, 6, 6, 0x0000);
          FillRect(140, 103, 6, 6, 0xffff);
          FillRect(240, 103, 6, 6, 0x0000);
        }
        else if(opcion_menu1 == 2){
          FillRect(140, 103, 6, 6, 0x0000);
          FillRect(240, 103, 6, 6, 0xffff);
          FillRect(40, 193, 6, 6, 0x0000);
        }
        else if(opcion_menu1 == 3){
          FillRect(240, 103, 6, 6, 0x0000);
          FillRect(40, 193, 6, 6, 0xffff);
          FillRect(140, 193, 6, 6, 0x0000);
        }
        else if(opcion_menu1 == 4){
          FillRect(40, 193, 6, 6, 0x0000);
          FillRect(140, 193, 6, 6, 0xffff);
          FillRect(240, 193, 6, 6, 0x0000);
        }
        else if(opcion_menu1 == 5){
          FillRect(140, 193, 6, 6, 0x0000);
          FillRect(240, 193, 6, 6, 0xffff);
          FillRect(260, 223, 6, 6, 0x0000);
        }
        else if(opcion_menu1 == 6){
          FillRect(240, 193, 6, 6, 0x0000);
          FillRect(260, 223, 6, 6, 0xffff);
        }
      }      
    }
    ///////////////////////// MENU ELEGIR DIFICULTAD ///////////////////////////          MISMA LOGICA
    else if (modo == 2){
      if(push6 == 1){
        if (opcion_menu2 == 0){
          dificultad = 1;
          H_line(40, 185, 280, 0x0000);
          H_line(40, 185, 60, 0xffff);
        }
        else if (opcion_menu2 == 1){
          dificultad = 2;
          H_line(40, 185, 280, 0x0000);
          H_line(130, 185, 60, 0xffff);
        }
        else if (opcion_menu2 == 2){
          dificultad = 3;
          H_line(40, 185, 280, 0x0000);
          H_line(220, 185, 60, 0xffff);
        }
        else if (opcion_menu2 == 3){
          opcion_menu2 = 0;
          modo = 0;
          LCD_Clear(0x0000);
          String text1 = "PAC-MAN";
          String text2 = " GHOSTS RAIN ";
          LCD_Print(text1, 110, 40, 2, 0xffff, 0x0000);
          LCD_Print(text2, 117, 60, 1, 0x0000, 0xffc0);
          
          String text4 = "Elegir personaje";
          String text5 = "Elegir dificultad";
          String text6 = "Elegir mapa";
          String text7 = "Jugar";
          LCD_Print(text4, 100, 100, 1, 0xffff, 0x0000);
          LCD_Print(text5, 100, 120, 1, 0xffff, 0x0000);
          LCD_Print(text6, 100, 140, 1, 0xffff, 0x0000);
          LCD_Print(text7, 100, 160, 1, 0xffff, 0x0000);
        
          Rect(20, 20, 280, 200, 0xffff);
          Rect(10, 10, 300, 220, 0xffff);
          FillRect(88, y_flecha, 6, 6, 0xffff);
        }
      }  
      else{
        if((push4 == 1) && (opcion_menu2 < 3)){
          opcion_menu2++;
        }
        else if((push5 == 1) && (opcion_menu2 > 0)){
          opcion_menu2--;
        }

        if(opcion_menu2 == 0){
          FillRect(40, 173, 6, 6, 0xffff);
          FillRect(130, 173, 6, 6, 0x0000);
        }
        else if(opcion_menu2 == 1){
          FillRect(40, 173, 6, 6, 0x0000);
          FillRect(130, 173, 6, 6, 0xffff);
          FillRect(220, 173, 6, 6, 0x0000);
        }
        else if(opcion_menu2 == 2){
          FillRect(130, 173, 6, 6, 0x0000);
          FillRect(220, 173, 6, 6, 0xffff);
          FillRect(260, 223, 6, 6, 0x0000);
        }
        else if(opcion_menu2 == 3){
          FillRect(220, 173, 6, 6, 0x0000);
          FillRect(260, 223, 6, 6, 0xffff);
        }
      }  
    }
    ///////////////////////// MENU ELEGIR MAPA ///////////////////////////            MISMA LOGICA
    else if (modo == 3){
      if(push6 == 1){
        if (opcion_menu3 == 0){
          mapa = 0;
          H_line(40, 185, 280, 0x0000);
          H_line(40, 185, 60, 0xffff);
        }
        else if (opcion_menu3 == 1){
          mapa = 1;
          H_line(40, 185, 280, 0x0000);
          H_line(130, 185, 60, 0xffff);
        }
        else if (opcion_menu3 == 2){
          mapa = 2;
          H_line(40, 185, 280, 0x0000);
          H_line(220, 185, 60, 0xffff);
        }
        else if (opcion_menu3 == 3){
          opcion_menu3 = 0;
          modo = 0;
          LCD_Clear(0x0000);
          String text1 = "PAC-MAN";
          String text2 = " GHOSTS RAIN ";
          LCD_Print(text1, 110, 40, 2, 0xffff, 0x0000);
          LCD_Print(text2, 117, 60, 1, 0x0000, 0xffc0);
          
          String text4 = "Elegir personaje";
          String text5 = "Elegir dificultad";
          String text6 = "Elegir mapa";
          String text7 = "Jugar";
          LCD_Print(text4, 100, 100, 1, 0xffff, 0x0000);
          LCD_Print(text5, 100, 120, 1, 0xffff, 0x0000);
          LCD_Print(text6, 100, 140, 1, 0xffff, 0x0000);
          LCD_Print(text7, 100, 160, 1, 0xffff, 0x0000);
        
          Rect(20, 20, 280, 200, 0xffff);
          Rect(10, 10, 300, 220, 0xffff);
          FillRect(88, y_flecha, 6, 6, 0xffff);
        }
      }
      else{
        if((push4 == 1) && (opcion_menu3 < 3)){
          opcion_menu3++;
        }
        else if((push5 == 1) && (opcion_menu3 > 0)){
          opcion_menu3--;
        }

        if(opcion_menu3 == 0){
          FillRect(40, 173, 6, 6, 0xffff);
          FillRect(130, 173, 6, 6, 0x0000);
        }
        else if(opcion_menu3 == 1){
          FillRect(40, 173, 6, 6, 0x0000);
          FillRect(130, 173, 6, 6, 0xffff);
          FillRect(220, 173, 6, 6, 0x0000);
        }
        else if(opcion_menu3 == 2){
          FillRect(130, 173, 6, 6, 0x0000);
          FillRect(220, 173, 6, 6, 0xffff);
          FillRect(260, 223, 6, 6, 0x0000);
        }
        else if(opcion_menu3 == 3){
          FillRect(220, 173, 6, 6, 0x0000);
          FillRect(260, 223, 6, 6, 0xffff);
        }
      }
    }
  }


//***************************************************************************************************************************************
// Comunicación PIC2                        MISMA LOGICA
//***************************************************************************************************************************************
  while (Serial2.available()) {
    // get the new byte:
    inChar = (char)Serial2.read();
    Serial.print(inChar);
    if (inChar == 'A'){
      push1 = 1;
    }
    else if (inChar == 'B'){
      push1 = 0;
    }
    else if (inChar == 'C'){
      push2 = 1;
    }
    else if (inChar == 'D'){
      push2 = 0;
    }
    else if (inChar == 'E'){
      push3 = 1;
    }
    else if (inChar == 'F'){
      push3 = 0;
    }
    ///////////////////////// MENÚ PRINICPAL ///////////////////////////
    if (modo == 0){
      if((push1 == 1) && (opcion_menu < 3)){
        opcion_menu++;
        FillRect(88, y_flecha, 6, 6, 0x0000);
        y_flecha = y_flecha + 20;
        FillRect(88, y_flecha, 6, 6, 0xffff);
      }
      else if((push2 == 1)&& (opcion_menu > 0)){
        opcion_menu--;
        FillRect(88, y_flecha, 6, 6, 0x0000);
        y_flecha = y_flecha - 20;
        FillRect(88, y_flecha, 6, 6, 0xffff);
      }
      else if(push3 == 1){
        if (opcion_menu == 0){
          LCD_Clear(0x0000);
          modo = 1;
          String text8 = "Elija personaje";
          LCD_Print(text8, 40, 15, 2, 0xffff, 0x0000);
          String personaje1 = "P1";
          LCD_Print(personaje1, 50, 100, 1, 0xffff, 0x0000);
          String personaje2 = "P2";
          LCD_Print(personaje2, 150, 100, 1, 0xffff, 0x0000);
          String personaje3 = "P3";
          LCD_Print(personaje3, 250, 100, 1, 0xffff, 0x0000);
          String personaje4 = "P4";
          LCD_Print(personaje4, 50, 190, 1, 0xffff, 0x0000);
          String personaje5 = "P5";
          LCD_Print(personaje5, 150, 190, 1, 0xffff, 0x0000);
          String personaje6 = "P6";
          LCD_Print(personaje6, 250, 190, 1, 0xffff, 0x0000);
          String personaje7= "Atras";
          LCD_Print(personaje7, 270, 220, 1, 0x0000, 0xffc0);
          FillRect(40, 173, 6, 6, 0xffff);
          myFile = SD.open("Pacman1.txt");
          Read_SD();
          LCD_Bitmap(35, 50, 41, 41, data);
          myFile = SD.open("Dedede1.txt");
          Read_SD();
          LCD_Bitmap(135, 50, 41, 41, data);
          myFile = SD.open("Sonic1.txt");
          Read_SD();
          LCD_Bitmap(235, 55, 41, 41, data);

          myFile = SD.open("spidey1.txt");
          Read_SD();
          LCD_Bitmap(35, 140, 41, 41, data);
          myFile = SD.open("megaman1.txt");
          Read_SD();
          LCD_Bitmap(135, 140, 41, 41, data);
          myFile = SD.open("kirby1.txt");
          Read_SD();
          LCD_Bitmap(235, 145, 41, 41, data);
          
        }
        else if (opcion_menu == 1){
          LCD_Clear(0x0000);
          modo = 2;
          String text9 = "Elija dificultad";
          LCD_Print(text9, 40, 25, 2, 0xffff, 0x0000);
          String dificultad1 = "Facil";
          LCD_Print(dificultad1, 50, 170, 1, 0xffff, 0x0000);
          String dificultad2 = "Medio";
          LCD_Print(dificultad2, 140, 170, 1, 0xffff, 0x0000);
          String dificultad3 = "Dificl";
          LCD_Print(dificultad3, 230, 170, 1, 0xffff, 0x0000);
          String dificultad4 = "Atras";
          LCD_Print(dificultad4, 270, 220, 1, 0x0000, 0xffc0);
          FillRect(40, 173, 6, 6, 0xffff);
          myFile = SD.open("Facil.txt");
          Read_SD();
          LCD_Bitmap(50, 110, 40, 40, data);
          myFile = SD.open("Medio.txt");
          Read_SD();
          LCD_Bitmap(140, 110, 40, 40, data);
          myFile = SD.open("Dificil.txt");
          Read_SD();
          LCD_Bitmap(235, 110, 40, 40, data);
        }
        else if (opcion_menu == 2){
          LCD_Clear(0x0000);
          modo = 3;
          String text10 = "Elija mapa";
          LCD_Print(text10, 80, 25, 2, 0xffff, 0x0000);
          String mapa1 = "Mapa 1";
          LCD_Print(mapa1, 50, 170, 1, 0xffff, 0x0000);
          String mapa2 = "Mapa 2";
          LCD_Print(mapa2, 140, 170, 1, 0xffff, 0x0000);
          String mapa3 = "Mapa3";
          LCD_Print(mapa3, 230, 170, 1, 0xffff, 0x0000);
          String mapa4 = "Atras";
          LCD_Print(mapa4, 270, 220, 1, 0x0000, 0xffc0);
          FillRect(40, 173, 6, 6, 0xffff);
          myFile = SD.open("Mapa1.txt");
          Read_SD();
          LCD_Bitmap(55, 120, 32, 32, data);
          myFile = SD.open("Mapa2.txt");
          Read_SD();
          LCD_Bitmap(145, 120, 32, 32, data);
          myFile = SD.open("Mapa3.txt");
          Read_SD();
          LCD_Bitmap(235, 120, 32, 32, data);
        }
        else if (opcion_menu == 3){
          LCD_Clear(0x1c0e);
          modo = 4;

        }
      }
    }
    ///////////////////////// MENÚ ELEGIR PRINCIPAL ///////////////////////////
    else if (modo == 1){
      if(push3 == 1){
        if (opcion_menu1 == 0){
          personaje2 = 0;
          H_line(30, 115, 290, 0x0000);
          H_line(30, 205, 290, 0x0000);
          H_line(30, 115, 45, 0xffff);
        }
        else if (opcion_menu1 == 1){
          personaje2 = 1;
          H_line(30, 115, 280, 0x0000);
          H_line(30, 205, 280, 0x0000);
          H_line(130, 115, 45, 0xffff);
        }
        else if (opcion_menu1 == 2){
          personaje2 = 2;
          H_line(30, 115, 290, 0x0000);
          H_line(30, 205, 290, 0x0000);
          H_line(230, 115, 45, 0xffff);
        }
        else if (opcion_menu1 == 3){
          personaje2 = 3;
          H_line(30, 115, 290, 0x0000);
          H_line(30, 205, 290, 0x0000);
          H_line(30, 205, 45, 0xffff);
        }
        else if (opcion_menu1 == 4){
          personaje2 = 4;
          H_line(30, 115, 290, 0x0000);
          H_line(30, 205, 290, 0x0000);
          H_line(130, 205, 45, 0xffff);
        }
        else if (opcion_menu1 == 5){
          personaje2 = 5;
          H_line(30, 115, 290, 0x0000);
          H_line(30, 205, 290, 0x0000);
          H_line(230, 205, 45, 0xffff);
        }
        else if (opcion_menu1 == 6){
          opcion_menu1 = 0;
          modo = 0;
          LCD_Clear(0x0000);
          String text1 = "PAC-MAN";
          String text2 = " GHOSTS RAIN ";
          LCD_Print(text1, 110, 40, 2, 0xffff, 0x0000);
          LCD_Print(text2, 117, 60, 1, 0x0000, 0xffc0);
          
          String text4 = "Elegir personaje";
          String text5 = "Elegir dificultad";
          String text6 = "Elegir mapa";
          String text7 = "Jugar";
          LCD_Print(text4, 100, 100, 1, 0xffff, 0x0000);
          LCD_Print(text5, 100, 120, 1, 0xffff, 0x0000);
          LCD_Print(text6, 100, 140, 1, 0xffff, 0x0000);
          LCD_Print(text7, 100, 160, 1, 0xffff, 0x0000);
        
          Rect(20, 20, 280, 200, 0xffff);
          Rect(10, 10, 300, 220, 0xffff);
          FillRect(88, y_flecha, 6, 6, 0xffff);
        }
      }
      else{
        if((push1 == 1) && (opcion_menu1 < 6)){
          opcion_menu1++;
        }
        else if((push2 == 1) && (opcion_menu1 > 0)){
          opcion_menu1--;
        }
        
        if(opcion_menu1 == 0){
          FillRect(40, 103, 6, 6, 0xffff);
          FillRect(140, 103, 6, 6, 0x0000);
        }
        else if(opcion_menu1 == 1){
          FillRect(40, 103, 6, 6, 0x0000);
          FillRect(140, 103, 6, 6, 0xffff);
          FillRect(240, 103, 6, 6, 0x0000);
        }
        else if(opcion_menu1 == 2){
          FillRect(140, 103, 6, 6, 0x0000);
          FillRect(240, 103, 6, 6, 0xffff);
          FillRect(40, 193, 6, 6, 0x0000);
        }
        else if(opcion_menu1 == 3){
          FillRect(240, 103, 6, 6, 0x0000);
          FillRect(40, 193, 6, 6, 0xffff);
          FillRect(140, 193, 6, 6, 0x0000);
        }
        else if(opcion_menu1 == 4){
          FillRect(40, 193, 6, 6, 0x0000);
          FillRect(140, 193, 6, 6, 0xffff);
          FillRect(240, 193, 6, 6, 0x0000);
        }
        else if(opcion_menu1 == 5){
          FillRect(140, 193, 6, 6, 0x0000);
          FillRect(240, 193, 6, 6, 0xffff);
          FillRect(260, 223, 6, 6, 0x0000);
        }
        else if(opcion_menu1 == 6){
          FillRect(240, 193, 6, 6, 0x0000);
          FillRect(260, 223, 6, 6, 0xffff);
        }
      }      
    }
    ///////////////////////// MENÚ DIFICULTAD ///////////////////////////
    else if (modo == 2){
      if(push3 == 1){
        if (opcion_menu2 == 0){
          dificultad = 1;
          H_line(40, 185, 280, 0x0000);
          H_line(40, 185, 60, 0xffff);
        }
        else if (opcion_menu2 == 1){
          dificultad = 2;
          H_line(40, 185, 280, 0x0000);
          H_line(130, 185, 60, 0xffff);
        }
        else if (opcion_menu2 == 2){
          dificultad = 3;
          H_line(40, 185, 280, 0x0000);
          H_line(220, 185, 60, 0xffff);
        }
        else if (opcion_menu2 == 3){
          opcion_menu2 = 0;
          modo = 0;
          LCD_Clear(0x0000);
          String text1 = "PAC-MAN";
          String text2 = " GHOSTS RAIN ";
          LCD_Print(text1, 110, 40, 2, 0xffff, 0x0000);
          LCD_Print(text2, 117, 60, 1, 0x0000, 0xffc0);
          
          String text4 = "Elegir personaje";
          String text5 = "Elegir dificultad";
          String text6 = "Elegir mapa";
          String text7 = "Jugar";
          LCD_Print(text4, 100, 100, 1, 0xffff, 0x0000);
          LCD_Print(text5, 100, 120, 1, 0xffff, 0x0000);
          LCD_Print(text6, 100, 140, 1, 0xffff, 0x0000);
          LCD_Print(text7, 100, 160, 1, 0xffff, 0x0000);
        
          Rect(20, 20, 280, 200, 0xffff);
          Rect(10, 10, 300, 220, 0xffff);
          FillRect(88, y_flecha, 6, 6, 0xffff);
        }
      }  
      else{
        if((push1 == 1) && (opcion_menu2 < 3)){
          opcion_menu2++;
        }
        else if((push2 == 1) && (opcion_menu2 > 0)){
          opcion_menu2--;
        }

        if(opcion_menu2 == 0){
          FillRect(40, 173, 6, 6, 0xffff);
          FillRect(130, 173, 6, 6, 0x0000);
        }
        else if(opcion_menu2 == 1){
          FillRect(40, 173, 6, 6, 0x0000);
          FillRect(130, 173, 6, 6, 0xffff);
          FillRect(220, 173, 6, 6, 0x0000);
        }
        else if(opcion_menu2 == 2){
          FillRect(130, 173, 6, 6, 0x0000);
          FillRect(220, 173, 6, 6, 0xffff);
          FillRect(260, 223, 6, 6, 0x0000);
        }
        else if(opcion_menu2 == 3){
          FillRect(220, 173, 6, 6, 0x0000);
          FillRect(260, 223, 6, 6, 0xffff);
        }
      }  
    }
    ///////////////////////// MENÚ ELEGIR MAPA ///////////////////////////
    else if (modo == 3){
      if(push3 == 1){
        if (opcion_menu3 == 0){
          mapa = 0;
          H_line(40, 185, 280, 0x0000);
          H_line(40, 185, 60, 0xffff);
        }
        else if (opcion_menu3 == 1){
          mapa = 1;
          H_line(40, 185, 280, 0x0000);
          H_line(130, 185, 60, 0xffff);
        }
        else if (opcion_menu3 == 2){
          mapa = 2;
          H_line(40, 185, 280, 0x0000);
          H_line(220, 185, 60, 0xffff);
        }
        else if (opcion_menu3 == 3){
          opcion_menu3 = 0;
          modo = 0;
          LCD_Clear(0x0000);
          String text1 = "PAC-MAN";
          String text2 = " GHOSTS RAIN ";
          LCD_Print(text1, 110, 40, 2, 0xffff, 0x0000);
          LCD_Print(text2, 117, 60, 1, 0x0000, 0xffc0);
          
          String text4 = "Elegir personaje";
          String text5 = "Elegir dificultad";
          String text6 = "Elegir mapa";
          String text7 = "Jugar";
          LCD_Print(text4, 100, 100, 1, 0xffff, 0x0000);
          LCD_Print(text5, 100, 120, 1, 0xffff, 0x0000);
          LCD_Print(text6, 100, 140, 1, 0xffff, 0x0000);
          LCD_Print(text7, 100, 160, 1, 0xffff, 0x0000);
        
          Rect(20, 20, 280, 200, 0xffff);
          Rect(10, 10, 300, 220, 0xffff);
          FillRect(88, y_flecha, 6, 6, 0xffff);
        }
      }
      else{
        if((push1 == 1) && (opcion_menu3 < 3)){
          opcion_menu3++;
        }
        else if((push2 == 1) && (opcion_menu3 > 0)){
          opcion_menu3--;
        }

        if(opcion_menu3 == 0){
          FillRect(40, 173, 6, 6, 0xffff);
          FillRect(130, 173, 6, 6, 0x0000);
        }
        else if(opcion_menu3 == 1){
          FillRect(40, 173, 6, 6, 0x0000);
          FillRect(130, 173, 6, 6, 0xffff);
          FillRect(220, 173, 6, 6, 0x0000);
        }
        else if(opcion_menu3 == 2){
          FillRect(130, 173, 6, 6, 0x0000);
          FillRect(220, 173, 6, 6, 0xffff);
          FillRect(260, 223, 6, 6, 0x0000);
        }
        else if(opcion_menu3 == 3){
          FillRect(220, 173, 6, 6, 0x0000);
          FillRect(260, 223, 6, 6, 0xffff);
        }
      }
    }
  }
  
}

//***************************************************************************************************************************************
//***************************************************************************************************************************************
//***************************************************************************************************************************************
//                                                         FUNCIONES
//***************************************************************************************************************************************
//***************************************************************************************************************************************
//***************************************************************************************************************************************


// Función para inicializar LCD
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++){
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER) 
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40|0x80|0x20|0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
//  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on 
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}

//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}

//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}

//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);   
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);   
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);   
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);   
  LCD_CMD(0x2c); // Write_memory_start
}

//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c){  
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);   
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
    }
  digitalWrite(LCD_CS, HIGH);
} 

//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
  }
  digitalWrite(LCD_CS, HIGH);
}

//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i,j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8); 
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);  
}

//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}

//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+w;
  y2 = y+h;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = w*h*2-1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
      
      //LCD_DATA(bitmap[k]);    
      k = k - 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}

//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background) 
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;
  
  if(fontSize == 1){
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if(fontSize == 2){
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }
  
  char charInput ;
  int cLength = text.length();
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength+1];
  text.toCharArray(char_array, cLength+1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1){
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2){
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}

//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]){  
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+width;
  y2 = y+height;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      //LCD_DATA(bitmap[k]);    
      k = k + 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}

//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 

  unsigned int x2, y2;
  x2 =   x+width;
  y2=    y+height;
  SetWindows(x, y, x2-1, y2-1);
  int k = 0;
  int ancho = ((width*columns));
  if(flip){
  for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width -1 - offset)*2;
      k = k+width*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k - 2;
     } 
  }
  }else{
     for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width + 1 + offset)*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k + 2;
     } 
  }
    
    
    }
  digitalWrite(LCD_CS, HIGH);
}

//***************************************************************************************************************************************
// Función para leer de la SD
//***************************************************************************************************************************************
void Read_SD(void){
  bandera = 0;  
  memset(data, 0, 15000);
  indice = 0;
  if (myFile){    
    while (myFile.available()) {
       contenidoSD = myFile.read();
       if (contenidoSD == 'f'){
          caracter = 15;
       }
       else if (contenidoSD == 'a'){
          caracter = 10;
       }
       else if (contenidoSD == 'b'){
          caracter = 11;
       }
       else if (contenidoSD == 'c'){
          caracter = 12;
       }
       else if (contenidoSD == 'd'){
          caracter = 13;
       }
       else if (contenidoSD == 'e'){
          caracter = 14;
       }
       else if (contenidoSD == '0'){
          caracter = 0;
       }
       else if (contenidoSD == '1'){
          caracter = 1;
       }
       else if (contenidoSD == '2'){
          caracter = 2;
       }
       else if (contenidoSD == '3'){
          caracter = 3;
       }
       else if (contenidoSD == '4'){
          caracter = 4;
       }
       else if (contenidoSD == '5'){
          caracter = 5;
       }
       else if (contenidoSD == '6'){
          caracter = 6;
       }
       else if (contenidoSD == '7'){
          caracter = 7;
       }
       else if (contenidoSD == '8'){
          caracter = 8;
       }
       else if (contenidoSD == '9'){
          caracter = 9;
       }
       if (bandera == 0){
        temp = (caracter*16);        
       }
       if (bandera == 1){
        temp = (temp + (caracter));
        data[indice] = temp;
        indice++;
       }
       bandera = !bandera;
       
    }
    myFile.close();
  }
  
} 


//***************************************************************************************************************************************
// Función 2 para leer de la SD
//***************************************************************************************************************************************
void Read_SD2(void){
  memset(data2, 0, 10000);
  bandera = 0;  
  indice = 0;
  if (myFile2){    
    while (myFile2.available()) {
       contenidoSD = myFile2.read();
       if (contenidoSD == 'f'){
          caracter = 15;
       }
       else if (contenidoSD == 'a'){
          caracter = 10;
       }
       else if (contenidoSD == 'b'){
          caracter = 11;
       }
       else if (contenidoSD == 'c'){
          caracter = 12;
       }
       else if (contenidoSD == 'd'){
          caracter = 13;
       }
       else if (contenidoSD == 'e'){
          caracter = 14;
       }
       else if (contenidoSD == '0'){
          caracter = 0;
       }
       else if (contenidoSD == '1'){
          caracter = 1;
       }
       else if (contenidoSD == '2'){
          caracter = 2;
       }
       else if (contenidoSD == '3'){
          caracter = 3;
       }
       else if (contenidoSD == '4'){
          caracter = 4;
       }
       else if (contenidoSD == '5'){
          caracter = 5;
       }
       else if (contenidoSD == '6'){
          caracter = 6;
       }
       else if (contenidoSD == '7'){
          caracter = 7;
       }
       else if (contenidoSD == '8'){
          caracter = 8;
       }
       else if (contenidoSD == '9'){
          caracter = 9;
       }
       if (bandera == 0){
        temp = (caracter*16);        
       }
       if (bandera == 1){
        temp = (temp + (caracter));
        data2[indice] = temp;
        indice++;
       }
       bandera = !bandera;
       
    }
    myFile2.close();
  }
}
