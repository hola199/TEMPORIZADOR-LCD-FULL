
//////////////////////declaracion de conexiones para la lcd
sbit LCD_RS at RD0_bit;
sbit LCD_EN at RD1_bit;
sbit LCD_D4 at RD2_bit;
sbit LCD_D5 at RD3_bit;
sbit LCD_D6 at RD4_bit;
sbit LCD_D7 at RD5_bit;
sbit LCD_RS_Direction at TRISD0_bit;
sbit LCD_EN_Direction at TRISD1_bit;
sbit LCD_D4_Direction at TRISD2_bit;
sbit LCD_D5_Direction at TRISD3_bit;
sbit LCD_D6_Direction at TRISD4_bit;
sbit LCD_D7_Direction at TRISD5_bit;
/////////////////final de la declaracion de conexiones para la lcd
sbit LED_ALARMA at RE0_bit;
sbit DISPOSITIVO_1 at RC0_bit;
sbit DISPOSITIVO_2 at RC1_bit;
sbit DISPOSITIVO_3 at RC2_bit;
sbit DISPOSITIVO_4 at RC4_bit;
/////////////////////////////////////////////variables
signed short int segundos = 0, minutos = 0, horas = 0;

short decrementar = 0, j, k = 0, activar = 1, clear = 0, nn = 1, habilitar_alarma = 0, h = 0, py = 0;
int contador = 0, apagar_parpadeo = 0, contador2 = 0, cnt2 = 0, cnt3 = 0, cnt4 = 0, cnt_alarma = 0; // todo los contadores son de tipo int
short selector = 0, parpadeo = 1, habilitar_EEPROM = 1,
      habilitar_parpadeo = 0, modo_reposo = 1, habilitar_puntos = 0;
short Inc = 0, Dec = 0, OK = 0, init_timer = 0; //variables para el manejo del programa desde las interrupciones
short programa = 0, N0_Temp = 0, estado1 = 1, estado2 = 1, estado3 = 1, estado4 = 1,
      alarma1 = 0, alarma2 = 0, alarma3 = 0, alarma4 = 0;

struct Time
{
  signed short int Segundo, Minuto, Hora;
}
//Variables para visualizar el tiempo en la lcd
T0,
    T1, T2, T3,
    //Variables para guardar en la EEPROm el tiempo ajustado en los temporizadores
    E0, E1, E2, E3,
    //tiempo temporal o de modificacion
    Temp;

//Mensajes a visulizar
char disp_1[] = "DISPOSITIVO 1";
char disp_2[] = "DISPOSITIVO 2";
char disp_3[] = "DISPOSITIVO 3";
char disp_4[] = "DISPOSITIVO 4";

char configurar1[] = "CONFIGURAR >";
char configurar2[] = "CONFIGURAR <";

char onn[] = "ONN";
char off[] = "OFF";

char sistema[] = "SISTEMA";
char alarma_text[] = "ALARMA";
char temporizadores[] = "TEMPORIZADORES";
char activado[] = "ACTIVADO";
char desactivado[] = "DESACTIVADO";

void mostrar_segundos()
{ //funcion para visualizar segundos
  int decenas, unidades;
  switch (N0_Temp)
  {
  case 0:
    decenas = T0.Segundo / 10;
    unidades = T0.Segundo % 10;
    break;

  case 1:
    decenas = T1.Segundo / 10;
    unidades = T1.Segundo % 10;
    break;

  case 2:
    decenas = T2.Segundo / 10;
    unidades = T2.Segundo % 10;
    break;

  case 3:
    decenas = T3.Segundo / 10;
    unidades = T3.Segundo % 10;
    break;
  }
  Lcd_Chr(2, 14, (unidades + 48));
  Lcd_Chr(2, 13, (decenas + 48));
}
void mostrar_minutos()
{ //funcion para visualizar minutos
  int decenas, unidades;
  switch (N0_Temp)
  {
  case 0:
    decenas = T0.Minuto / 10;
    unidades = T0.Minuto % 10;
    break;

  case 1:
    decenas = T1.Minuto / 10;
    unidades = T1.Minuto % 10;
    break;

  case 2:
    decenas = T2.Minuto / 10;
    unidades = T2.Minuto % 10;
    break;

  case 3:
    decenas = T3.Minuto / 10;
    unidades = T3.Minuto % 10;
    break;
  }
  Lcd_Chr(2, 11, (unidades + 48));
  Lcd_Chr(2, 10, (decenas + 48));
}
void mostrar_horas()
{ //funcion para visualizar horas
  int decenas, unidades;
  switch (N0_Temp)
  {
  case 0:
    decenas = T0.Hora / 10;
    unidades = T0.Hora % 10;
    break;

  case 1:
    decenas = T1.Hora / 10;
    unidades = T1.Hora % 10;
    break;

  case 2:
    decenas = T2.Hora / 10;
    unidades = T2.Hora % 10;
    break;

  case 3:
    decenas = T3.Hora / 10;
    unidades = T3.Hora % 10;
    break;
  }
  Lcd_Chr(2, 8, (unidades + 48));
  Lcd_Chr(2, 7, (decenas + 48));
}

void mostrar_segundos_temp()
{ //funcion para visualizar segundos (edicion)
  int decenas, unidades;
  decenas = Temp.Segundo / 10;
  unidades = Temp.Segundo % 10;
  Lcd_Chr(2, 14, (unidades + 48));
  Lcd_Chr(2, 13, (decenas + 48));
}
void mostrar_minutos_temp()
{ //funcion para visualizar minutos (edicion)
  int decenas, unidades;
  decenas = Temp.Minuto / 10;
  unidades = Temp.Minuto % 10;
  Lcd_Chr(2, 11, (unidades + 48));
  Lcd_Chr(2, 10, (decenas + 48));
}

void mostrar_horas_temp()
{ //funcion para visualizar horas  (edicion)
  int decenas, unidades;
  decenas = Temp.Hora / 10;
  unidades = Temp.Hora % 10;
  Lcd_Chr(2, 8, (unidades + 48));
  Lcd_Chr(2, 7, (decenas + 48));
}

void parpadear_segundos_temp()
{ // funcion para hacer parpadear los segundos
  mostrar_minutos_temp();
  mostrar_horas_temp();

  if (parpadeo == 1)
    mostrar_segundos_temp();
  else
  {
    Lcd_Chr(2, 14, (' '));
    Lcd_Chr(2, 13, (' '));
  }
}

void parpadear_minutos_temp()
{ //funcion para hacer parpadear los minutos
  mostrar_segundos_temp();
  mostrar_horas_temp();

  if (parpadeo == 1)
    mostrar_minutos_temp();
  else
  {
    Lcd_Chr(2, 11, (' '));
    Lcd_Chr(2, 10, (' '));
  }
}

void parpadear_horas_temp()
{ //funcion para hacer parpadear las horas
  mostrar_segundos_temp();
  mostrar_minutos_temp();

  if (parpadeo == 1)
    mostrar_horas_temp();
  else
  {
    Lcd_Chr(2, 8, (' '));
    Lcd_Chr(2, 7, (' '));
  }
}

void mostrar_puntos()
{ //funcion para visualizar y hacer parpadear los dos puntos " : "
  if (activar == 1)
  {
    Lcd_Chr(2, 12, 58);
    Lcd_Chr(2, 9, 58);
  }
  else
  {
    Lcd_Chr(2, 12, ' ');
    Lcd_Chr(2, 9, ' ');
  }
}

void cuenta_regresiva1()
{ //configuracion/cuenta regresiva ( dispositovo1)
  if (T0.Segundo > 0 || T0.Minuto > 0 || T0.Hora > 0)
  {
    if (T0.Segundo < 0)
    {
      T0.Minuto--;
      T0.Segundo = 59;
    }

    if (T0.Minuto < 0)
    {
      T0.Hora--;
      T0.Minuto = 59;
    }
    if (T0.Hora < 0)
      T0.Hora = 0;
  }
}

void cuenta_regresiva2()
{ //configuracion/cuenta regresiva(dispositivo2)
  if (T1.Segundo > 0 || T1.Minuto > 0 || T1.Hora > 0)
  {
    if (T1.Segundo < 0)
    {
      T1.Minuto--;
      T1.Segundo = 59;
    }

    if (T1.Minuto < 0)
    {
      T1.Hora--;
      T1.Minuto = 59;
    }
    if (T1.Hora < 0)
      T1.Hora = 0;
  }
}

void cuenta_regresiva3()
{ //configuracion/cuenta regresiva (dispositivo 3)
  if (T2.Segundo > 0 || T2.Minuto > 0 || T2.Hora > 0)
  {
    if (T2.Segundo < 0)
    {
      T2.Minuto--;
      T2.Segundo = 59;
    }

    if (T2.Minuto < 0)
    {
      T2.Hora--;
      T2.Minuto = 59;
    }
    if (T2.Hora < 0)
      T2.Hora = 0;
  }
}

void cuenta_regresiva4()
{ //configuracion/cuenta regresiva ( dispositivo 4)
  if (T3.Segundo > 0 || T3.Minuto > 0 || T3.Hora > 0)
  {
    if (T3.Segundo < 0)
    {
      T3.Minuto--;
      T3.Segundo = 59;
    }

    if (T3.Minuto < 0)
    {
      T3.Hora--;
      T3.Minuto = 59;
    }
    if (T3.Hora < 0)
      T3.Hora = 0;
  }
}

void pausa()
{ // pausa
  delay_ms(120);
}

void leer_EEPROM()
{ //tranferir los datos de la EEPROM a vaiables de conteo

  if (N0_Temp == 0 && programa == 0)
  {
    E0.Segundo = EEPROM_Read(0x00);
    E0.Minuto = EEPROM_Read(0x001);
    E0.Hora = EEPROM_Read(0x02);

    T0.Segundo = E0.Segundo;
    T0.Minuto = E0.Minuto;
    T0.Hora = E0.Hora;
  }

  if (N0_Temp == 1 && programa == 0)
  {
    E1.Segundo = EEPROM_Read(0x03);
    E1.Minuto = EEPROM_Read(0x04);
    E1.Hora = EEPROM_Read(0x05);

    T1.Segundo = E1.Segundo;
    T1.Minuto = E1.Minuto;
    T1.Hora = E1.Hora;
  }

  if (N0_Temp == 2 && programa == 0)
  {
    E2.Segundo = EEPROM_Read(0x06);
    E2.Minuto = EEPROM_Read(0x07);
    E2.Hora = EEPROM_Read(0x08);

    T2.Segundo = E2.Segundo;
    T2.Minuto = E2.Minuto;
    T2.Hora = E2.Hora;
  }

  if (N0_Temp == 3 && programa == 0)
  {
    E3.Segundo = EEPROM_Read(0x09);
    E3.Minuto = EEPROM_Read(0x10);
    E3.Hora = EEPROM_Read(0x11);

    T3.Segundo = E3.Segundo;
    T3.Minuto = E3.Minuto;
    T3.Hora = E3.Hora;
  }
}
void grabar_EEPROM()
{
  if (N0_temp == 0)
  {
    E0.Segundo = Temp.Segundo;
    E0.Minuto = Temp.Minuto;
    E0.Hora = Temp.Hora;

    EEPROM_Write(0x00, E0.Segundo);
    EEPROM_Write(0x01, E0.Minuto);
    EEPROM_Write(0x02, E0.Hora);
  }

  if (N0_temp == 1)
  {
    E1.Segundo = Temp.Segundo;
    E1.Minuto = Temp.Minuto;
    E1.Hora = Temp.Hora;

    EEPROM_Write(0x03, E1.Segundo);
    EEPROM_Write(0x04, E1.Minuto);
    EEPROM_Write(0x05, E1.Hora);
  }

  if (N0_temp == 2)
  {
    E2.Segundo = Temp.Segundo;
    E2.Minuto = Temp.Minuto;
    E2.Hora = Temp.Hora;

    EEPROM_Write(0x06, E2.Segundo);
    EEPROM_Write(0x07, E2.Minuto);
    EEPROM_Write(0x08, E2.Hora);
  }

  if (N0_temp == 3)
  {
    E3.Segundo = Temp.Segundo;
    E3.Minuto = Temp.Minuto;
    E3.Hora = Temp.Hora;

    EEPROM_Write(0x09, E3.Segundo);
    EEPROM_Write(0x10, E3.Minuto);
    EEPROM_Write(0x11, E3.Hora);
  }
}

void Inc_Dec_N0Timer()
{
  if (Inc)
  {
    while (Inc)
      ;
    Lcd_Cmd(_LCD_CLEAR);
    N0_Temp++;
    if (N0_Temp > 3)
      N0_Temp = 3;
  }
  else
  {
    if (DISPOSITIVO_1 == 0 && DISPOSITIVO_2 == 0 && DISPOSITIVO_3 && DISPOSITIVO_4 == 0)
      modo_reposo = 1;
  }

  if (Dec)
  {
    while (Dec)
      ;
    Lcd_Cmd(_LCD_CLEAR);
    N0_Temp--;
    if (N0_Temp < 0)
      N0_Temp = 0;
  }
  else
  {
    if (DISPOSITIVO_1 == 0 && DISPOSITIVO_2 == 0 && DISPOSITIVO_3 && DISPOSITIVO_4 == 0)
      modo_reposo = 1;
  }
}

void visualizar_N0_Timer()
{

  switch (N0_Temp)
  {
  case 0:
    Lcd_Out(1, 2, disp_1); //visualizar el numero de DISPOSITIVO
                           // activar los dos puntos : cuando el DISPOSITIVO este en 1
    if (DISPOSITIVO_1 == 1)
      habilitar_puntos = 1;
    else
    {
      habilitar_puntos = 0;
      activar = 1;
    }
    //visualizar el estado del dispositivo
    if (DISPOSITIVO_1 == 1)
      Lcd_Out(2, 2, onn);
    else
      Lcd_Out(2, 2, off);

    break;

  case 1:
    Lcd_Out(1, 2, disp_2);

    if (DISPOSITIVO_2 == 1)
      habilitar_puntos = 1;
    else
      habilitar_puntos = 0;

    if (DISPOSITIVO_2 == 1)
      Lcd_Out(2, 2, onn);
    else
    {
      Lcd_Out(2, 2, off);
      activar = 1;
    }
    break;

  case 2:
    Lcd_Out(1, 2, disp_3);

    if (DISPOSITIVO_3 == 1)
      habilitar_puntos = 1;
    else
    {
      habilitar_puntos = 0;
      activar = 1;
    }
    if (DISPOSITIVO_3 == 1)
      Lcd_Out(2, 2, onn);
    else
      Lcd_Out(2, 2, off);

    break;

  case 3:
    Lcd_Out(1, 2, disp_4);

    if (DISPOSITIVO_4 == 1)
      habilitar_puntos = 1;
    else
    {
      habilitar_puntos = 0;
      activar = 1;
    }
    if (DISPOSITIVO_4 == 1)
      Lcd_Out(2, 2, onn);
    else
      Lcd_Out(2, 2, off);

    break;
  }
}

void visualizar_N0_Timer2()
{
  switch (N0_Temp)
  {
  case 0:
    Lcd_Out(1, 3, configurar1);
    Lcd_Out(2, 2, disp_1);
    break;

  case 1:
    Lcd_Out(1, 3, configurar1);
    Lcd_Out(2, 2, disp_2);
    break;

  case 2:
    Lcd_Out(1, 3, configurar1);
    Lcd_Out(2, 2, disp_3);
    break;

  case 3:
    Lcd_Out(1, 3, configurar2);
    Lcd_Out(2, 2, disp_4);
    break;
  }
}

void Inc_Timer()
{
  switch (selector)
  { //cada estado de "selector" decrementa una variable

  case 0:
    pausa();
    Temp.Segundo++;
    if (Temp.Segundo > 59)
      Temp.Segundo = 0;
    break;

  case 1:
    pausa();
    Temp.Minuto++;
    if (Temp.Minuto > 59)
      Temp.Minuto = 0;
    break;

  case 2:
    pausa();
    Temp.Hora++;
    if (Temp.Hora > 23)
      Temp.Hora = 0;
    break;
  }
}

void Dec_Timer()
{
  switch (selector)
  { // cada estado de "selector" decrementa una variable
  case 0:
    pausa();
    Temp.Segundo--;
    if (Temp.Segundo < 0)
      Temp.Segundo = 59;
    break;

  case 1:
    pausa();
    Temp.Minuto--;
    if (Temp.Minuto < 0)
      Temp.Minuto = 59;
    break;

  case 2:
    pausa();
    Temp.Hora--;
    if (Temp.Hora < 0)
      Temp.Hora = 23;
    break;
  }
}

void transmitir()
{
  if (N0_Temp == 0)
  {
    Temp.Segundo = E0.Segundo;
    Temp.Minuto = E0.Minuto;
    Temp.Hora = E0.Hora;
  }

  if (N0_Temp == 1)
  {
    Temp.Segundo = E1.Segundo;
    Temp.Minuto = E1.Minuto;
    Temp.Hora = E1.Hora;
  }

  if (N0_Temp == 2)
  {
    Temp.Segundo = E2.Segundo;
    Temp.Minuto = E2.Minuto;
    Temp.Hora = E2.Hora;
  }

  if (N0_Temp == 3)
  {
    Temp.Segundo = E3.Segundo;
    Temp.Minuto = E3.Minuto;
    Temp.Hora = E3.Hora;
  }
}

void reposar_pic()
{
  if (modo_reposo == 1)
  {
    j++;
    if (j == 40)
    {
      asm sleep
          j = 0;
    }
  }
}

void alarma()
{
  modo_reposo = 0;
  if (nn == 1)
  {
    Lcd_Out(1, 2, "              ");

    if (alarma1)
      Lcd_Out(1, 2, "Temporizador 1");
    else if (alarma2)
      Lcd_Out(1, 2, "Temporizador 2");
    else if (alarma3)
      Lcd_Out(1, 2, "Temporizador 3");
    else if (alarma4)
      Lcd_Out(1, 2, "Temporizador 4");

    LED_ALARMA = 1;
    delay_ms(50);
    LED_ALARMA = 0;
  }

  if (nn == 2)
  {
    Lcd_Out(1, 2, "              ");
    Lcd_Out(1, 4, "Finalizado.");
    LED_ALARMA = 1;
    delay_ms(50);
    LED_ALARMA = 0;
  }
  if (Inc)
  {
    while (Inc)
      alarma1 = alarma2 = 0;
    Lcd_Cmd(_LCD_CLEAR);
  }

  if (Dec)
  {
    while (Dec)
      alarma1 = alarma2 = 0;
    Lcd_Cmd(_LCD_CLEAR);
  }

  if (OK)
  {
    while (OK)
      alarma1 = alarma2 = 0;
    Lcd_Cmd(_LCD_CLEAR);
  }

  if (Init_timer)
  {
    while (Init_timer)
      alarma1 = alarma2 = 0;
    Lcd_Cmd(_LCD_CLEAR);
  }
}

//funcion para visualizar los textos "Temporizador" y "Reprogramable"
//de forma secuencial
void presentacion()
{
  char text[13] = "Temporizador";
  char text1[14] = "Reprogramable";
  char i;
  delay_ms(300);
  for (i = 3; i < 15; i++)
  {
    Lcd_Chr(1, i, text[i - 3]);
    delay_ms(100);
  }

  for (i = 2; i < 15; i++)
  {
    Lcd_Chr(2, i, text1[i - 2]);
    delay_ms(100);
  }

  delay_ms(200);
  Lcd_Cmd(_LCD_CLEAR);
}

void configuracion_interruptiones()
{ //funcion para configurar y habilitar interrupciones
  ADCON1 = 0X0F;
  T0CON = 0XC6;
  INTCON = 0XA8;
  INTCON2 = 0X05;
  INTCON3 = 0X00;
}
void init_main()
{ // configuracion de puertos
  TRISB = 0XFF;
  TRISC = 0X00;
  TRISE = 0;
  PORTB = PORTC = PORTE = 0X00;
}

void interrupt()
{
  // interrupciones por cambio de flanco en el PORTB
  if (Button(&PORTB, 4, 1, 0))
  {
    modo_reposo = 0;
    Inc = 1;
    cnt2 = 0;
  }
  else
    Inc = 0;

  if (Button(&PORTB, 5, 1, 0))
  {
    modo_reposo = 0;
    Dec = 1;
    cnt2 = 0;
  }
  else
    Dec = 0;

  if (Button(&PORTB, 6, 1, 0))
  {
    modo_reposo = 0;
    OK = 1;
    cnt2 = 0;
  }
  else
    OK = 0;

  if (Button(&PORTB, 7, 1, 0))
  {
    modo_reposo = 0;
    init_timer = 1;
    cnt2 = 0;
  }
  else
    init_timer = 0;

  RBIF_bit = 0;

  if (T0IF_bit)
  { // interrupciones por el TMR0

    if (habilitar_parpadeo == 1)
    {
      apagar_parpadeo++;
      if (apagar_parpadeo == 600)
      {
        habilitar_parpadeo = 0;
        apagar_parpadeo = 0;
        modo_reposo = 1;
      }

      contador2++;
      if (contador2 == 50)
      {
        parpadeo = 0;
      }

      if (contador2 == 70)
      {
        parpadeo = 1;
        contador2 = 0;
      }
    }

    if (DISPOSITIVO_1 == 1 || DISPOSITIVO_2 == 1 || DISPOSITIVO_3 == 1 || DISPOSITIVO_4 == 1)
    {
      contador++;
      if (contador == 50)
      {
        if (habilitar_puntos == 1)
          activar = 0;
      }
      if (contador == 100)
      {
        //decremento de segundos, segun lo indique el estado de cada dispositivo
        if (DISPOSITIVO_1 == 1)
          T0.Segundo--;
        if (DISPOSITIVO_2 == 1)
          T1.Segundo--;
        if (DISPOSITIVO_3 == 1)
          T2.Segundo--;
        if (DISPOSITIVO_4 == 1)
          T3.Segundo--;
        if (habilitar_puntos == 1)
          activar = 1;
        contador = 0;
      }
    }
    // Contador para la alrma
    if (DISPOSITIVO_1 == 0 || DISPOSITIVO_2 == 0 || DISPOSITIVO_3 == 0 || DISPOSITIVO_4 == 0)
    {
      if (programa == 0)
      {
        cnt3++;
        if (cnt3 == 150)
          nn = 1;
        if (cnt3 == 300)
        {
          nn = 2;
          cnt3 = 0;
        }
      }
      // contador para apagar la alarma
      if (alarma1 == 1 || alarma2 == 1 || alarma3 == 1 || alarma4 == 1)
      {
        cnt_alarma++;
        if (cnt_alarma == 300)
        {
          alarma1 = alarma2 = alarma3 = alarma4 = 0;
          cnt_alarma = 0;
          modo_reposo = 1;
          clear = 1;
        }
      }
    }

    //conteo para entrar en modo reposo, despues de que el
    //programa sea mayor que 0

    if (programa > 0)
    {
      cnt2++;
      if (cnt2 == 500)
      {
        clear = 1;
        programa = 0;
        cnt2 = 0;
      }
    }

    TMR0L = 177; // interrupciones cada 10 milisegundos
    T0IF_bit = 0;
  }
}

void main()
{
  init_main();
  configuracion_interruptiones();
  Lcd_Init();
  Lcd_Cmd(_LCD_CLEAR);
  Lcd_Cmd(_LCD_CURSOR_OFF);
  //presentacion();
  TMR0L = 177;
  activar = 1;
  while (1)
  {

    while (programa == 0)
    {

      if (clear)
      {
        Lcd_Cmd(_LCD_CLEAR);
        delay_ms(30);
        clear = 0;
      }

      habilitar_alarma = EEPROM_Read(0X12);
      mostrar_puntos();
      if (habilitar_EEPROM == 1)
        leer_EEPROM();

      //visualizar segundos,minutos,horas en la LCD
      mostrar_segundos();
      mostrar_minutos();
      mostrar_horas();

      if (alarma1 == 0 && alarma2 == 0 && alarma3 == 0 && alarma4 == 0)
      {
        visualizar_N0_timer(); //visualizar numero de dispositivo
        nn = 1;
      }

      Inc_Dec_N0timer(); // cambiar numero de dispositivo

      // pasar al programa 1( en el programa 1
      // se visualiza el tipo de ajuste que se dese realizar)
      if (OK)
      {
        while (OK)
          ;
        Lcd_Cmd(_LCD_CLEAR);
        programa = 1;
      }

      //Encender dispositivo 1
      if (DISPOSITIVO_1 == 0 && N0_Temp == 0)
      {
        if (estado1 == 1)
          habilitar_EEPROM = 1;
        if (init_timer)
        {
          while (init_timer)
            ;
          DISPOSITIVO_1 = 1;
        }
      }
      //Encender dispostivo 2
      if (DISPOSITIVO_2 == 0 && N0_Temp == 1)
      {
        if (estado2 == 1)
          habilitar_EEPROM = 1;
        if (init_timer)
        {
          while (init_timer)
            ;
          DISPOSITIVO_2 = 1;
        }
      }
      //Encender dispostivo 3
      if (DISPOSITIVO_3 == 0 && N0_Temp == 2)
      {
        if (estado3 == 1)
          habilitar_EEPROM = 1;
        if (init_timer)
        {
          while (init_timer)
            ;
          DISPOSITIVO_3 = 1;
        }
      }
      //Encender dispositivo 4
      if (DISPOSITIVO_4 == 0 && N0_Temp == 3)
      {
        if (estado4 == 1)
          habilitar_EEPROM = 1;
        if (init_timer)
        {
          while (init_timer)
            ;
          DISPOSITIVO_4 = 1;
        }
      }

      //inciar el temporizador para dispositivo 1
      if (DISPOSITIVO_1 == 1 && N0_Temp == 0)
      {
        habilitar_EEPROM = 0;
        habilitar_parpadeo = 0;
        apagar_parpadeo = 0;
        modo_reposo = 0;
        cuenta_regresiva1();
        //apagar el temporizador para dispositivo 1
        if (N0_Temp == 0)
        {
          if (init_timer)
          {
            while (init_timer)
              ;
            estado1 = 0;
            DISPOSITIVO_1 = 0;
          }
        }
      }
      //iniciar el temporizador para dispostivo 2
      if (DISPOSITIVO_2 == 1 && N0_Temp == 1)
      {
        habilitar_EEPROM = 0;
        habilitar_parpadeo = 0;
        apagar_parpadeo = 0;
        modo_reposo = 0;
        cuenta_regresiva2();
        //apagar el temporizador para dispositivo 2
        if (N0_Temp == 1)
        {
          if (init_timer)
          {
            while (init_timer)
              ;
            estado2 = 0;
            DISPOSITIVO_2 = 0;
          }
        }
      }
      //iniciar el temporizador para dispostivo 3
      if (DISPOSITIVO_3 == 1 && N0_Temp == 2)
      {
        habilitar_EEPROM = 0;
        habilitar_parpadeo = 0;
        apagar_parpadeo = 0;
        modo_reposo = 0;
        cuenta_regresiva3();
        //apagar el temporizador para dispositivo 3
        if (N0_Temp == 2)
        {
          if (init_timer)
          {
            while (init_timer)
              ;
            estado3 = 0;
            DISPOSITIVO_3 = 0;
          }
        }
      }
      //iniciar el temporizador para dispostivo 4
      if (DISPOSITIVO_4 == 1 && N0_Temp == 3)
      {
        habilitar_EEPROM = 0;
        habilitar_parpadeo = 0;
        apagar_parpadeo = 0;
        modo_reposo = 0;
        cuenta_regresiva4();
        //apagar el temporizador para dispositivo 4
        if (N0_Temp == 3)
        {
          if (init_timer)
          {
            while (init_timer)
              ;
            estado4 = 0;
            DISPOSITIVO_4 = 0;
          }
        }
      }

      if (T0.Segundo == 0 && T0.Minuto == 0 && T0.Hora == 0 && DISPOSITIVO_1 == 1)
      {
        habilitar_EEPROM = 1; // habilitar leer la EEPROM
        DISPOSITIVO_1 = 0;
        if (habilitar_alarma == 1)
        {
          alarma1 = 1;
        }
        else
          modo_reposo = 1;
        cnt_alarma = 0;
        Lcd_Cmd(_LCD_CLEAR);
      }

      if (T1.Segundo == 0 && T1.Minuto == 0 && T1.Hora == 0 && DISPOSITIVO_2 == 1)
      {
        habilitar_EEPROM = 1; // habilitar leer la EEPROM
        DISPOSITIVO_2 = 0;
        if (habilitar_alarma == 1)
        {
          alarma2 = 1;
        }
        else
          modo_reposo = 1;
        cnt_alarma = 0;
        Lcd_Cmd(_LCD_CLEAR);
      }

      if (T2.Segundo == 0 && T2.Minuto == 0 && T2.Hora == 0 && DISPOSITIVO_3 == 1)
      {
        habilitar_EEPROM = 1; // habilitar leer la EEPROM
        DISPOSITIVO_3 = 0;
        modo_reposo = 1;
        if (habilitar_alarma == 1)
        {
          alarma3 = 1;
        }
        else
          modo_reposo = 1;
      }

      if (T3.Segundo == 0 && T3.Minuto == 0 && T3.Hora == 0 && DISPOSITIVO_4 == 1)
      {
        habilitar_EEPROM = 1; // habilitar leer la EEPROM
        DISPOSITIVO_4 = 0;
        modo_reposo = 1;
        if (habilitar_alarma == 1)
        {
          alarma4 = 1;
        }
        else
          modo_reposo = 1;
      }

      if (alarma1 == 1 || alarma2 == 1 || alarma3 == 1 || alarma4 == 1)
      {
        apagar_parpadeo = 0;
        modo_reposo = 0;
        alarma();
      }

      reposar_pic();
    }

    // muestra en la LCD la opcion para configurar la alarma y los temporizadores
    while (programa == 1)
    {
      //Opcion para configurar la alarma
      if (h)
      {
        Lcd_Out(1, 2, configurar1);
        Lcd_Out(2, 3, alarma_text);
        // Cambiar a, configurar temporizadores
        if (Dec)
        {
          while (Dec)
            ;
          Lcd_Cmd(_lCD_CLEAR);
          h = 0;
        }
        //Pasar a configrar la alarma
        if (OK)
        {
          while (OK)
            ;
          Lcd_Cmd(_lCD_CLEAR);
          programa = 4;
        }
      }
      //Opcion para configurar los temporizadores
      if (h == 0)
      {
        Lcd_Out(1, 2, configurar2);
        Lcd_Out(2, 2, temporizadores);

        if (Inc)
        { // regresar a, configurar la alarma
          while (Inc)
            ;
          Lcd_Cmd(_lCD_CLEAR);
          h = 1;
        }

        if (OK)
        { // Confirmacion para configurar los temporizadores
          while (OK)
            ;
          Lcd_Cmd(_lCD_CLEAR);
          programa = 2;
        }
      }
      //regresar al programa principal
      if (init_timer)
      {
        while (init_timer)
          ;
        Lcd_Cmd(_LCD_CLEAR);
        programa = 0;
      }
    }
    //muestra en la LCD en numero de temporizador que se desee ajustar
    while (programa == 2)
    {

      Inc_Dec_N0Timer();
      visualizar_N0_Timer2();

      if (OK)
      {
        while (OK)
          ;
        Lcd_Cmd(_LCD_CLEAR);
        programa = 3;
        transmitir();
      }

      if (init_timer)
      {
        while (init_timer)
          ;
        Lcd_Cmd(_LCD_CLEAR);
        programa = 1;
      }
    }

    //ajustar los temporizadores
    while (programa == 3)
    {

      mostrar_puntos();

      if (N0_Temp == 0)
        Lcd_Out(1, 3, disp_1);
      if (N0_Temp == 1)
        Lcd_Out(1, 3, disp_2);
      if (N0_Temp == 2)
        Lcd_Out(1, 3, disp_3);
      if (N0_Temp == 3)
        Lcd_Out(1, 3, disp_4);

      if (OK)
      {
        while (OK)
          ;
        selector++;
        if (selector > 2)
          selector = 0;
        grabar_EEPROM();
        habilitar_EEPROM = 1;
      }

      if (Inc)
      {                      //incrementar
        contador2 = 0;       // se reinica la variable que se encarda de apagar el parpadeo.
        apagar_parpadeo = 0; // se reinicia la variable que se encarga de proporcionar el tiempo de parpadeo.
        Inc_timer();
      }

      if (Dec)
      {                      //decender
        contador2 = 0;       // se reinicia la variable que se encarga de proporcionar el tiempo de parpadeo.
        apagar_parpadeo = 0; // se reinica la variable que se encarda de apagar el parpadeo
        Dec_timer();
      }

      habilitar_parpadeo = 1;
      switch (selector)
      {
      case 0:
        parpadear_segundos_temp();
        break;
      case 1:
        parpadear_minutos_temp();
        break;
      case 2:
        parpadear_horas_temp();
        break;
      }

      if (init_timer)
      {
        while (init_timer)
          ;
        Lcd_Cmd(_LCD_CLEAR);
        programa = 2;
      }
    }

    // configuracion de la alarma
    while (programa == 4)
    {

      if (habilitar_alarma)
      {
        Lcd_Out(1, 3, activado);
        if (Dec || OK)
        {
          while (Dec || OK)
            ;
          Lcd_Cmd(_LCD_CLEAR);
          habilitar_alarma = 0;
          EEPROM_Write(0X12, habilitar_alarma);
        }
      }
      else
      {
        Lcd_Out(1, 3, desactivado);
        if (Inc || OK)
        {
          while (Inc || OK)
            ;
          Lcd_Cmd(_LCD_CLEAR);
          habilitar_alarma = 1;
          EEPROM_Write(0X12, habilitar_alarma);
        }
      }

      if (init_timer)
      {
        while (init_timer)
          ;
        Lcd_Cmd(_LCD_CLEAR);
        programa = 1;
      }
    }
  }
}