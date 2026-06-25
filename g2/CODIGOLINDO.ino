#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ======================
// Keypad
// ======================

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  ROWS,
  COLS
);

// ======================
// Variáveis
// ======================

int paginaAtual = 1;
int totalPaginas = 1;
String morseAtual = "";

int pagina = 0;
String telaAtual = "";

String grupo = "";
String frase = "";

// ======================
// Número -> Letra
// ======================

char traduzirNumero(String codigo)
{
  if(codigo == "2") return 'A';
  if(codigo == "22") return 'B';
  if(codigo == "222") return 'C';

  if(codigo == "3") return 'D';
  if(codigo == "33") return 'E';
  if(codigo == "333") return 'F';

  if(codigo == "4") return 'G';
  if(codigo == "44") return 'H';
  if(codigo == "444") return 'I';

  if(codigo == "5") return 'J';
  if(codigo == "55") return 'K';
  if(codigo == "555") return 'L';

  if(codigo == "6") return 'M';
  if(codigo == "66") return 'N';
  if(codigo == "666") return 'O';

  if(codigo == "7") return 'P';
  if(codigo == "77") return 'Q';
  if(codigo == "777") return 'R';
  if(codigo == "7777") return 'S';

  if(codigo == "8") return 'T';
  if(codigo == "88") return 'U';
  if(codigo == "888") return 'V';

  if(codigo == "9") return 'W';
  if(codigo == "99") return 'X';
  if(codigo == "999") return 'Y';
  if(codigo == "9999") return 'Z';

  return '?';
}

// ======================
// Letra -> Morse
// ======================

String letraParaMorse(char c)
{
  switch(toupper(c))
  {
    case 'A': return ".-";
    case 'B': return "-...";
    case 'C': return "-.-.";
    case 'D': return "-..";
    case 'E': return ".";
    case 'F': return "..-.";
    case 'G': return "--.";
    case 'H': return "....";
    case 'I': return "..";
    case 'J': return ".---";
    case 'K': return "-.-";
    case 'L': return ".-..";
    case 'M': return "--";
    case 'N': return "-.";
    case 'O': return "---";
    case 'P': return ".--.";
    case 'Q': return "--.-";
    case 'R': return ".-.";
    case 'S': return "...";
    case 'T': return "-";
    case 'U': return "..-";
    case 'V': return "...-";
    case 'W': return ".--";
    case 'X': return "-..-";
    case 'Y': return "-.--";
    case 'Z': return "--..";
    case ' ': return "/";
  }

  return "";
}

// ======================
// Frase -> Morse
// ======================

String fraseParaMorse(String texto)
{
  String resultado = "";

  for(int i = 0; i < texto.length(); i++)
  {
    resultado += letraParaMorse(texto[i]);
    resultado += " ";
  }

  return resultado;
}

// ======================
// Setup
// ======================

void setup()
{
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Tradutor");
  delay(1500);

  lcd.clear();
}

// ======================
// Loop
// ======================

void loop()
{
  char tecla = keypad.getKey();

  if(!tecla)
    return;

  // Digitos
  if(tecla >= '2' && tecla <= '9')
  {
    grupo += tecla;

    lcd.clear();
    lcd.print(grupo);
  }

  // Espaço
  else if(tecla == '0')
  {
    frase += ' ';

    lcd.clear();
    lcd.print(frase);
  }

  // Confirmar letra
  else if(tecla == '#')
  {
    char letra = traduzirNumero(grupo);

    frase += letra;
    grupo = "";

    lcd.clear();
    lcd.print(frase);
  }

  // Apagar
  else if(tecla == '*')
  {
    if(frase.length() > 0)
    {
      frase.remove(frase.length() - 1);
    }

    lcd.clear();
    lcd.print(frase);
  }

  // Mostrar Morse
  else if(tecla == 'A')
  {
  morseAtual = fraseParaMorse(frase);

  totalPaginas = ceil(frase.length() / 3.0);

  paginaAtual = 1;

  mostrarPagina();
  }

  // Limpar tudo
  else if(tecla == 'B')
  {
    frase = "";
    grupo = "";

    lcd.clear();
  }

  // Mostrar texto
  else if(tecla == 'C')
  {
    lcd.clear();
    lcd.print(frase);
  }

  // Próxima página
  else if(tecla == 'D')
  {
  paginaAtual++;

  if(paginaAtual > totalPaginas)
  {
    paginaAtual = 1;
  }

  mostrarPagina();
  }
}

// ======================
// Função
// ======================

void mostrarPagina()
{
  lcd.clear();


  // posição inicial das letras
  int inicio = (paginaAtual - 1) * 3;


  String letrasPagina = "";


  for(int i = inicio; i < inicio + 3; i++)
  {
    if(i < frase.length())
    {
      letrasPagina += frase[i];
    }
  }



  // =========================
  // Linha 1 - Morse
  // =========================

  String linhaMorse = "";


  for(int i = 0; i < letrasPagina.length(); i++)
  {
    linhaMorse += letraParaMorse(letrasPagina[i]);

    if(i < letrasPagina.length()-1)
      linhaMorse += " ";
  }


  lcd.setCursor(0,0);
  lcd.print(linhaMorse);



  // =========================
  // Linha 2 - Página
  // =========================

  lcd.setCursor(0,1);


  lcd.print("Pg.");
  lcd.print(paginaAtual);
  lcd.print(" [");


  lcd.print(letrasPagina);


  if(inicio + 3 < frase.length())
  {
    lcd.print("...");
  }


  lcd.print("]");
}