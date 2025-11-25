#include "others.h"

String intToBin(int valor) {
  return String(valor, BIN);
}

int binToInt(String valor) {
  return (int) strtol(valor.c_str(), NULL, 2);
}

String separarBits(String value, int inicio, int fim) {
  if (inicio < 0) inicio = 0;
  if (fim >= value.length()) fim = value.length() - 1;
  if (inicio > fim) return "";

  return value.substring(inicio, fim + 1);
}

String escreverBits(String value, int inicio, int fim, int valorParaEscrever) {
  if (inicio < 0) inicio = 0;
  if (fim >= value.length()) fim = value.length() - 1;
  if (inicio > fim) return value;

  // Converte valor para binário
  String novo = String(valorParaEscrever, BIN);

  // Garante que o tamanho do binário se encaixa no intervalo
  int tamIntervalo = fim - inicio + 1;
  while (novo.length() < tamIntervalo) novo = "0" + novo;   // preenche zeros à esquerda
  if (novo.length() > tamIntervalo) 
    novo = novo.substring(novo.length() - tamIntervalo);     // corta excesso

  // Monta a nova string
  String antes  = value.substring(0, inicio);
  String depois = value.substring(fim + 1);

  return antes + novo + depois;
}
