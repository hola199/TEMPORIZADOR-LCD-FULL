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
