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