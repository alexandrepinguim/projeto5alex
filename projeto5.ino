#include <LiquidCrystal.h>

#define b1 6 // Botão A
#define b2 7 // Botão B

int numLetra = 0;             // Índice do caractere atual
char nome[15] = "";           // Nome do usuário (14 caracteres + terminador nulo)
int pos = 0;                  // Posição atual para escrita no LCD
unsigned long b1PressTime = 0;
unsigned long b2PressTime = 0;

LiquidCrystal lcd_1(12, 11, 5, 4, 3, 2);
char letras[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz .<";

void setup() {
  lcd_1.begin(16, 2);
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  lcd_1.print("Introd. o Nome:");
  lcd_1.setCursor(0, 1);
}

void loop() {
  // Verificar botão 'A' (navegar caracteres)
  if (digitalRead(b1) == LOW) {
    if (b1PressTime == 0) b1PressTime = millis();
    if (millis() - b1PressTime > 2000) { // Long press: reiniciar
      reiniciarEntrada();
      b1PressTime = 0;
    }
  } else if (b1PressTime != 0) { // Short press
    if (millis() - b1PressTime < 2000) {
      lcd_1.setCursor(pos, 1);
      lcd_1.print(letras[numLetra]);
      numLetra++;
      if (numLetra >= sizeof(letras) - 1)
        numLetra = 0; // Voltar ao início
    }
    b1PressTime = 0;
  }

  // Verificar botão 'B' (confirmar caractere)
  if (digitalRead(b2) == LOW) {
    if (b2PressTime == 0) b2PressTime = millis();
    if (millis() - b2PressTime > 2000) { // Long press: concluir
      concluirEntrada();
      b2PressTime = 0;
    }
  } else if (b2PressTime != 0) { // Short press
    if (millis() - b2PressTime < 2000) {
      confirmarCaractere();
    }
    b2PressTime = 0;
  }
}

void reiniciarEntrada() {
  pos = 0;
  numLetra = 0;
  memset(nome, 0, sizeof(nome)); // Apagar o nome
  lcd_1.clear();
  lcd_1.print("Introd. o Nome:");
  lcd_1.setCursor(0, 1);
}

void concluirEntrada() {
  lcd_1.clear();
  lcd_1.print("Ola, ");
  lcd_1.print(nome);
  delay(3000); // Mostrar por 3 segundos
  reiniciarEntrada(); // Reiniciar para nova entrada
}

void confirmarCaractere() {
  char c = letras[numLetra];
  if (c == '<') { // Apagar último caractere
    if (pos > 0) {
      pos--;
      nome[pos] = '\0';
      lcd_1.setCursor(pos, 1);
      lcd_1.print(' ');
      lcd_1.setCursor(pos, 1);
    }
  } else if (pos < 14) { // Adicionar caractere (até o limite)
    nome[pos] = c;
    pos++;
    lcd_1.setCursor(pos, 1);
  }
  numLetra = 0; // Resetar para o primeiro caractere
}
