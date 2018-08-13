void ver_temporizador(int, int, int);
struct Time
{
  signed short int Segundo, Minuto, Hora;
};

void mostrar_segundos(struct Time *reloj)
{
  const row = 2, col = 13;
  ver_temporizador(row, col, reloj->Segundo);
}

void mostrar_minutos(struct Time *reloj)
{
  const row = 2, col = 10;
  ver_temporizador(row, col, reloj->Minuto);
}

void mostrar_horas(struct Time *reloj)
{
  const row = 2, col = 7;
  ver_temporizador(row, col, reloj->Hora);
}

// --------------- funciones para visualizar segundos,minutos , horas, para edicion --------------------
void mostrar_segundos_temp(struct Time *reloj)
{
  const row = 2, col = 13;
  ver_temporizador(row, col, reloj->Segundo);
}

void mostrar_minutos_temp(struct Time *reloj)
{
  const row = 2, col = 10;
  ver_temporizador(row, col, reloj->Minuto);
}

void mostrar_horas_temp(struct Time *reloj)
{
  const row = 2, col = 7;
  ver_temporizador(row, col, reloj->Hora);
}

void parpadear_segundos_temp(struct Time *temporal, short parpadeo)
{ // funcion para hacer parpadear los segundos
  mostrar_minutos_temp(temporal);
  mostrar_horas_temp(temporal);

  if (parpadeo == 1)
    mostrar_segundos_temp(temporal);
  else
  {
    Lcd_Chr(2, 14, (' '));
    Lcd_Chr(2, 13, (' '));
  }
}

void parpadear_minutos_temp(struct Time *temporal, short parpadeo)
{ //funcion para hacer parpadear los minutos
  mostrar_segundos_temp(temporal);
  mostrar_horas_temp(temporal);

  if (parpadeo == 1)
    mostrar_minutos_temp(temporal);
  else
  {
    Lcd_Chr(2, 11, (' '));
    Lcd_Chr(2, 10, (' '));
  }
}

void parpadear_horas_temp(struct Time *temporal, short parpadeo)
{ //funcion para hacer parpadear las horas
  mostrar_segundos_temp(temporal);
  mostrar_minutos_temp(temporal);

  if (parpadeo == 1)
    mostrar_horas_temp(temporal);
  else
  {
    Lcd_Chr(2, 8, (' '));
    Lcd_Chr(2, 7, (' '));
  }
}
