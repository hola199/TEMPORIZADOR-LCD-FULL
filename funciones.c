void ver_temporizador(int row, int col, int minuto)
{
  int decenas, unidades;
  decenas = minuto / 10;
  unidades = minuto % 10;
  Lcd_Chr(row, col + 1, (unidades + 48));
  Lcd_Chr(row, col, (decenas + 48));
}

int disp1(int estado)
{
  if (estado == 3)
    return RC0_bit;
  RC0_bit = estado;
  return 20;
}

int disp2(int estado)
{
  if (estado == 3)
    return RC1_bit;
  RC1_bit = estado;
  return 20;
}

int disp3(int estado)
{
  if (estado == 3)
    return RC2_bit;
  RC2_bit = estado;
  return 20;
}

int disp4(int estado)
{
  if (estado == 3)
    return RC4_bit;
  RC4_bit = estado;
  return 20;
}

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

void mostrar_puntos(short bandera)
{ //funcion para visualizar y hacer parpadear los dos puntos " : "
  if (bandera == 1)
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


void Lcd_N_Timer(int row, int col, int N0_Temp)
{
  Lcd_Out(row, col, "DISPOSITIVO"); //visualizar el numero de DISPOSITIVO
  Lcd_Chr(row, col + 12, N0_Temp + 49);
}

void LCD_N0_Timer(int N)
{
  const row = 2, col = 2;
  Lcd_N_Timer(row, col, N);
  Lcd_Out(1, 3, "CONFIGURAR");
}

// btn PORTB
void btn(int pin, void (*func)())
{
  if (Button(&PORTB, pin, 1, 0))
  {
    while (Button(&PORTB, pin, 1, 0))
      ;
    func();
  }
}
