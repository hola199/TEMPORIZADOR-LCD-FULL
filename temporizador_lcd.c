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

sbit LED_ALARMA at RE0_bit;
sbit DISPOSITIVO_1 at RC0_bit;
sbit DISPOSITIVO_2 at RC1_bit;
sbit DISPOSITIVO_3 at RC2_bit;
sbit DISPOSITIVO_4 at RC4_bit;

// contadores
int
    contador = 0,
    apagar_parpadeo = 0,
    contador2 = 0,
    cnt2 = 0,
    cnt3 = 0,
    cnt_alarma = 0,
    ii,
    j;

short
    activar = 1,
    clear = 0,
    nn = 1,
    habilitar_alarma = 0,
    h = 0,
    selector = 0,
    parpadeo = 1,
    habilitar_EEPROM = 1,
    habilitar_parpadeo = 0,
    modo_reposo = 1,
    habilitar_puntos = 0,
    Inc = 0,
    Dec = 0,
    OK = 0,
    init_timer = 0, //variables para el manejo del programa desde las interrupciones
    programa = 0,
    N0_Temp = 0,
    estado[] = {1, 1, 1, 1},
    alarmas[] = {0, 0, 0, 0};

struct Time
{
  signed short int Segundo, Minuto, Hora;
} Temp, arrayTemp[3], arrayEprom[3];

const pinInc = 4,
      pinDec = 5,
      pinOk = 6,
      pinInit = 7;

//Mensajes a visulizar
char onn[] = "ONN";
char off[] = "OFF";

char sistema[] = "SISTEMA";
char alarma_text[] = "ALARMA";
char temporizadores[] = "TEMPORIZADORES";
char activado[] = "ACTIVADO";
char desactivado[] = "DESACTIVADO";

int disp1(int);
int disp2(int);
int disp3(int);
int disp4(int);

int (*dispositivos[])(int) = {disp1, disp2, disp3, disp4};

void btn(int pin, void (*func)());
// tercer parametro sera el parametro de la funcion
void btn_Parameter(int pin, void (*func)(int), int parametro);

void ver_temporizador(int, int, int);
//funcion para visualizar segundos
void mostrar_segundos(struct Time *reloj);

//funcion para visualizar minutos
void mostrar_minutos(struct Time *reloj);

//funcion para visualizar horas
void mostrar_horas(struct Time *reloj);

//funcion para visualizar segundos (edicion)
void mostrar_segundos_temp()
{
  const row = 2, col = 13;
  ver_temporizador(row, col, Temp.Segundo);
}

//funcion para visualizar minutos (edicion)
void mostrar_minutos_temp()
{
  const row = 2, col = 10;
  ver_temporizador(row, col, Temp.Minuto);
}

//funcion para visualizar horas  (edicion)
void mostrar_horas_temp()
{
  const row = 2, col = 7;
  ver_temporizador(row, col, Temp.Hora);
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

void mostrar_puntos(short);

void cuenta_regresiva(int numero)
{ //configuracion/cuenta regresiva ( dispositovo1)
  if (arrayTemp[numero].Segundo > 0 || arrayTemp[numero].Minuto > 0 || arrayTemp[numero].Hora > 0)
  {
    if (arrayTemp[numero].Segundo < 0)
    {
      arrayTemp[numero].Minuto--;
      arrayTemp[numero].Segundo = 59;
    }

    if (arrayTemp[numero].Minuto < 0)
    {
      arrayTemp[numero].Hora--;
      arrayTemp[numero].Minuto = 59;
    }
    if (arrayTemp[numero].Hora < 0)
      arrayTemp[numero].Hora = 0;
  }
}

void pausa()
{ // pausa
  delay_ms(120);
}

void leer(int uno, int dos, int tres)
{
  arrayEprom[N0_temp].Segundo = EEPROM_Read(uno);
  arrayEprom[N0_temp].Minuto = EEPROM_Read(dos);
  arrayEprom[N0_temp].Hora = EEPROM_Read(tres);
}

//tranferir los datos de la EEPROM a vaiables de conteo
void leer_EEPROM()
{
  if (programa == 0)
  {
    switch (N0_Temp)
    {
    case 0:
      leer(0x00, 0x001, 0x02);
      break;

    case 1:
      leer(0x03, 0x04, 0x05);
      break;

    case 2:
      leer(0x06, 0x07, 0x08);
      break;

    case 3:
      leer(0x09, 0x10, 0x11);
      break;
    }
  }

  arrayTemp[N0_Temp] = arrayEprom[N0_Temp];
}

void grabar(int uno, int dos, int tres)
{
  EEPROM_Write(uno, arrayEprom[N0_temp].Segundo);
  EEPROM_Write(dos, arrayEprom[N0_temp].Minuto);
  EEPROM_Write(tres, arrayEprom[N0_temp].Hora);
}

void grabar_EEPROM()
{
  arrayEprom[N0_Temp] = Temp;

  switch (N0_temp)
  {
  case 0:
    grabar(0x00, 0x01, 0x02);
    break;

  case 1:
    grabar(0x03, 0x04, 0x05);
    break;

  case 2:
    grabar(0x06, 0x07, 0x08);
    break;

  case 3:
    grabar(0x09, 0x10, 0x11);
    break;
  }
}

void reposar()
{
  if (DISPOSITIVO_1 == 0 && DISPOSITIVO_2 == 0 && DISPOSITIVO_3 && DISPOSITIVO_4 == 0)
    modo_reposo = 1;
}

void increment_NO_Temp()
{
  Lcd_Cmd(_LCD_CLEAR);
  N0_Temp++;
  if (N0_Temp > 3)
    N0_Temp = 3;
}

void decrement_N0_Temp()
{
  Lcd_Cmd(_LCD_CLEAR);
  N0_Temp--;
  if (N0_Temp < 0)
    N0_Temp = 0;
}

void Inc_Dec_N0Timer()
{
  btn(pinInc, increment_NO_Temp);
  btn(pinDec, decrement_N0_Temp);
  //  reposar();
}

void Lcd_N_Timer(int, int, int);

void visualizar_N0_Timer()
{
  const row = 1, col = 2;
  Lcd_N_Timer(row, col, N0_temp);

  for (ii = 0; ii < 4; ii++)
  {
    if (N0_Temp == ii)
    {
      // permitir parpadear los dos puntos : cuando el DISPOSITIVO este en 1
      if (dispositivos[N0_Temp](3) == 1)
        habilitar_puntos = 1;
      else
      {
        habilitar_puntos = 0;
        activar = 1;
      }
      //visualizar el estado del dispositivo
      if (dispositivos[N0_Temp](3) == 1)
        Lcd_Out(2, 2, onn);
      else
        Lcd_Out(2, 2, off);
    }
  }
}

void LCD_N0_Timer(int);

void visualizar_N0_Timer2()
{
  LCD_N0_Timer(N0_temp);
  if (N0_temp == 3)
    Lcd_Chr(1, 14, '<');
  else
    Lcd_Chr(1, 14, '>');
}

void Inc_Timer()
{
  switch (selector) //cada estado de "selector" decrementa una variable
  {
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
  Temp = arrayEprom[N0_Temp];
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

void blink_led_alarma()
{
  LED_ALARMA = 1;
  delay_ms(50);
  LED_ALARMA = 0;
}

void reset_alarma()
{
  for (ii = 0; ii < 4; ii++)
    alarmas[ii] = 0;

  Lcd_Cmd(_LCD_CLEAR);
}

void alarma()
{
  modo_reposo = 0;
  if (nn == 1)
  {
    Lcd_Out(1, 2, "              ");

    for (ii = 0; ii < 4; ii++)
    {
      if (alarmas[ii])
      {
        Lcd_Out(1, 2, "Temporizador");
        Lcd_Chr(1, 15, ii + 48);
      }
    }

    blink_led_alarma();
  }

  if (nn == 2)
  {
    Lcd_Out(1, 2, "              ");
    Lcd_Out(1, 4, "Finalizado.");
    blink_led_alarma();
  }

  btn(pinInc, reset_alarma);
  btn(pinDec, reset_alarma);
  btn(pinOk, reset_alarma);
  btn(pinInit, reset_alarma);
}

//funcion para visualizar los textos "Temporizador" y "Reprogramable"
//de forma secuencial
void presentacion();

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

isZeroTemp(int N)
{
  return arrayTemp[N].Segundo == 0 && arrayTemp[N].Minuto == 0 && arrayTemp[N].Hora == 0;
}

void start()
{
  pinInit, dispositivos[N0_Temp](1);
}

void encender_dispositivo()
{
  if (estado[N0_Temp] == 1)
    habilitar_EEPROM = 1;

  btn(pinInit, start);
}

void off_disp()
{
  estado[N0_Temp] = 0;
  dispositivos[N0_Temp](0);
}

void apagar_dispositivo()
{
  btn(pinInit, off_disp);
}

void desactivar_reposo()
{
  modo_reposo = 0;
  cnt2 = 0;
}

void interrupt()
{

  if (RBIF_bit)
  {
    if (RB7_bit == 0)
    {
    }
    RBIF_bit = 0;
  }

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
          arrayTemp[0].Segundo--;
        if (DISPOSITIVO_2 == 1)
          arrayTemp[1].Segundo--;
        if (DISPOSITIVO_3 == 1)
          arrayTemp[2].Segundo--;
        if (DISPOSITIVO_4 == 1)
          arrayTemp[3].Segundo--;
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
      if (alarmas[0] == 1 || alarmas[1] == 1 || alarmas[2] == 1 || alarmas[3] == 1)
      {
        cnt_alarma++;
        if (cnt_alarma == 300)
        {
          alarmas[0] = alarmas[1] = alarmas[2] = alarmas[3] = 0;
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

void configureToStart()
{
  habilitar_EEPROM = 0;
  habilitar_parpadeo = 0;
  apagar_parpadeo = 0;
  modo_reposo = 0;
  cuenta_regresiva(N0_Temp);
}

void encender_alarma()
{
  habilitar_EEPROM = 1; // habilitar leer la EEPROM
  dispositivos[N0_Temp](0);
  if (habilitar_alarma == 1)
  {
    alarmas[N0_Temp] = 1;
  }
  else
    modo_reposo = 1;
  cnt_alarma = 0;
  Lcd_Cmd(_LCD_CLEAR);
}

void change_program(int program)
{
  Lcd_Cmd(_LCD_CLEAR);
  programa = program;
}

void edit_temp(int program)
{
  change_program(program);
  transmitir();
}

void guardar_emprom()
{
  selector++;
  if (selector > 2)
    selector = 0;
  grabar_EEPROM();
  habilitar_EEPROM = 1;
}

void incrementar_timer()
{
  contador2 = 0;       // se reinica la variable que se encarda de apagar el parpadeo.
  apagar_parpadeo = 0; // se reinicia la variable que se encarga de proporcionar el tiempo de parpadeo.
  Inc_timer();
}

void decrementar_timer()
{
  contador2 = 0;       // se reinicia la variable que se encarga de proporcionar el tiempo de parpadeo.
  apagar_parpadeo = 0; // se reinica la variable que se encarda de apagar el parpadeo
  Dec_timer();
}

void configurar_alarma()
{
  Lcd_Cmd(_lCD_CLEAR);
  h = 1;
}

void configurar_temporizador()
{
  Lcd_Cmd(_lCD_CLEAR);
  h = 0;
}

void cambiar_alarma(int estado)
{
  Lcd_Cmd(_LCD_CLEAR);
  habilitar_alarma = estado;
  EEPROM_Write(0X12, habilitar_alarma);
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
      mostrar_puntos(activar);
      if (habilitar_EEPROM == 1)
        leer_EEPROM();

      //visualizar segundos,minutos,horas en la LCD
      mostrar_segundos(&arrayTemp[N0_Temp]);
      mostrar_minutos(&arrayTemp[N0_Temp]);
      mostrar_horas(&arrayTemp[N0_Temp]);

      if (alarmas[0] == 0 && alarmas[1] == 0 && alarmas[2] == 0 && alarmas[3] == 0)
      {
        visualizar_N0_timer(); //visualizar numero de dispositivo
        nn = 1;
      }

      Inc_Dec_N0timer(); // cambiar numero de dispositivo

      // pasar al programa 1( en el programa 1
      // se visualiza el tipo de ajuste que se dese realizar)
      btn_Parameter(pinOk, change_program, 1);
      //Encender DISPOSITIVO
      for (ii = 0; ii < 4; ii++)
      {
        if (dispositivos[ii](3) == 0 && N0_Temp == ii)
        {
          encender_dispositivo();
        }
      }

      //apagar el temporizador para dispositivos
      for (ii = 0; ii < 4; ii++)
      {
        if (dispositivos[ii](3) == 1 && N0_Temp == ii)
        {
          configureToStart();
          //apagar el temporizador para dispositivo 4
          apagar_dispositivo();
        }
      }

      // inicia alarmas
      for (ii = 0; ii < 4; ii++)
      {
        if (isZeroTemp(ii) && dispositivos[ii](3) == 1)
        {
          encender_alarma();
        }
      }

      if (alarmas[0] == 1 || alarmas[1] == 1 || alarmas[2] == 1 || alarmas[3] == 1)
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
        Lcd_Out(1, 2, "CONFIGURAR >");
        Lcd_Out(2, 3, alarma_text);
        // Cambiar a, configurar temporizadores
        btn(pinDec, configurar_temporizador);

        //ir a  configurar la alarma
        btn_Parameter(pinOk, change_program, 4);
      }
      //Opcion para configurar los temporizadores
      if (h == 0)
      {
        Lcd_Out(1, 2, "CONFIGURAR < ");
        Lcd_Out(2, 2, temporizadores);

        btn(pinInc, configurar_alarma);

        // Confirmacion para configurar los temporizadores
        btn_Parameter(pinOk, change_program, 2);
      }
      //regresar al programa principal
      btn_Parameter(pinInit, change_program, 0);
    }
    //muestra en la LCD en numero de temporizador que se desee ajustar
    while (programa == 2)
    {

      Inc_Dec_N0Timer();
      visualizar_N0_Timer2();

      // editar los temporizadores
      btn_Parameter(pinOk, edit_temp, 3);
      btn_Parameter(pinInit, change_program, 1);
    }

    //ajustar los temporizadores
    while (programa == 3)
    {
      const row = 1, col = 3;
      mostrar_puntos(activar);
      Lcd_N_Timer(row, col, N0_temp);

      btn(pinOk, guardar_emprom);

      // botones para modificar el tiempo
      btn(pinInc, incrementar_timer);
      btn(pinDec, decrementar_timer);

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

      btn_Parameter(pinInit, change_program, 2);
    }

    // configuracion de la alarma
    while (programa == 4)
    {
      if (habilitar_alarma)
      {
        Lcd_Out(1, 3, activado);
        btn_Parameter(pinOk, cambiar_alarma, 0);
      }
      else
      {
        Lcd_Out(1, 3, desactivado);
        btn_Parameter(pinOk, cambiar_alarma, 1);
      }

      btn_Parameter(pinInit, change_program, 1);
    }
  }
}