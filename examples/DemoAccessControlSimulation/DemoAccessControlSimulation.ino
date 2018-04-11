/*
  Nome do Aquivo:   IToSmartMenu-YYYYMMDDa.ino Sketch
  Descrição:        Este sketch faz parte do pacote da biblioteca IToSmartMenu e simula um controle de acesso.
                    O controle de acesso simula os seguintes modulos:
                    1 - Display LCD de 16 caracteres e 2 linhas
                        O menu contera string de texto anexado a cada item de menu que sera enviado ao LCD
                        toda vez que for alterado. A simulacao enviara os dados do LCD a porta serial
                        e se apresentara como o que temos abaixo:
                        /|Menu HOME  v0.01|\
                        \|                |/

                    Cada elemento do menu se chama node que possui a estrutura definida como itosm_node
                    Exemplo do uso do menu usando a serial simulando botoes e leitor RFID
                    Simulacao de botoes: Recebe numeros pela serial simulando o seguinte:
                      5 - ENTER // Comando ou tecla Enter ou Ok pressionada
                      8 - UP    // Comando ou tecla indicando para cima ou incremento
                      2 - DOWN  // Comando ou tecla indicando para baixo ou decremento
                      4 - LEFT  // Comando ou tecla indicando esquerda ou retornar
                      6 - RIGHT // Comando ou tecla indicando direita ou entrar
                    Simulacao de leitor RFID: Recebe letras pela serial simulando caracteres
                    Hexadecimais recebidos pelo leitor.
                      Ex.: Letra A = 0x414141414141, letra a = 0x616161616161
                    Create menu elements and link to home and each other
                    Menu elements have:
                      char array to display the menu option like LCD, Serial, etc
                      chain of links to automatic select next menu element
                      chain of functions to execute on item open() event
                      chain of functions to execute on item loop() event
                      chain of functions to execute on item exit() event
                      timeout value to automate redirection after specific time
                      chain of flags indicating status
                    5 - Modulo de Rele
                      Simulado pelo LED do Arduino
                    
                      
                    
   File version:    20160403
   Dependency:      Arduino.h                       // Base do Arduino
   MCU:             ATMEGA 328P 16 MHz / ATMEGA 2560 16 MHz
   Board:           Arduino Uno/Mega/Mini
   Compiler         N/A
   IDE:             Eclipse Mars CDT 8.8.1 with plugin Arduino C++ Jantje Baeyens 4.0.0
   Hardware:        Arduino UNO / Arduino Mega 2560
   Owner:           Celso Eiju Ito - eijuito@gmail.com
   Colaborators:    Celso Eiju Ito - eijuito@labirito.com
   Uso:             Labirito
   Diagrams:        N/A
   Copyright ®      2016 Celso Eiju Ito eijuito@gmail.com (www.itosa.com.br)(www.labirito.com)
                    This program belongs to Celso Eiju Ito (eijuito@gmail.com)
                    The total or partial copy by unauthorized people is denied
                    It is not for commercial and lease use
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   REVISIONS:       (latest entry first)
   2018-02-20       First version
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   NEXT STEPS:      (priority first)
   none
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

#include "IToSmartMenu.h"

/**
 * **********************************  S M A R T   M E N U  ***************************************
 * Aqui temos as definicoes dos elementos de menu os quais chamamos de menu node
 * Notem que um menu node se compoe de menu item e um array de ponteiros para outros menu item
 * Ha 8 bits no node do menu para uso livre do programador como flag dependente do menu selecionado
 * Deste modo fica automatizado a ativacao ou nao de cada recurso de acordo com o node atual
 * Durante o loop() pode-se tomar alguma acao conforme o flag ativado
 */
#define MENU_FLAG_ALL_CLEAR   0x00 // all flags clear
#define MENU_FLAG_RFID_ENABLE 0x01 // bit 0 indica se o leitor RFID deve estar habilitado
#define MENU_FLAG_RTC_MODO1   0x02 // bit 1 indica se o Modo 1 do RTC sera atualizado no display
#define MENU_FLAG_RTC_MODO2   0x04 // bit 2 indica se o Modo 2 do RTC sera atualizado no display

/**
 * Chama o construtor para criar o menu
 */
IToSmartMenu menu; // instancia o objeto menu
/**
 * Declaracao dos itens do menu, através de 2 linhas para cada item
 * A primeira linha declara o string que será ligado ao item de menu
 * A segunda linha é o item de menu, recebendo o string
 * Note que estao com PROGMEM para economizar memoria RAM
 * Importante: siga exatamente os exemplos fornecidos 
 */
IToSmartMenu_text text_0_0[] = "Menu HOME  v0.01                ";
IToSmartMenu_item item_0_0 = {(char*)text_0_0, -1, MENU_FLAG_RFID_ENABLE | MENU_FLAG_RTC_MODO2, 0};

IToSmartMenu_text text_1_0[] = "Mensagens por   item de menu    ";
IToSmartMenu_item item_1_0 = {(char*)text_1_0, 20, MENU_FLAG_ALL_CLEAR, 0};
IToSmartMenu_text text_1_1[] = "Passe Tag Mestrepara prosseguir ";
IToSmartMenu_item item_1_1 = {(char*)text_1_1, 30, MENU_FLAG_RFID_ENABLE, 0};
IToSmartMenu_text text_1_2[] = "Mensagem de     tag correta !!! ";
IToSmartMenu_item item_1_2 = {(char*)text_1_2, 10, MENU_FLAG_ALL_CLEAR, 0};
IToSmartMenu_text text_1_3[] = "Mensagem de     tag incorreta!!!";
IToSmartMenu_item item_1_3 = {(char*)text_1_3, 10, MENU_FLAG_ALL_CLEAR, 0};

IToSmartMenu_text text_2_0[] = "Mensagens por   item de mensagem";
IToSmartMenu_item item_2_0 = {(char*)text_2_0, 20, MENU_FLAG_ALL_CLEAR, 0};
IToSmartMenu_text text_2_1[] = "Passe Tag Mestrepara prosseguir ";
IToSmartMenu_item item_2_1 = {(char*)text_2_1, 30, MENU_FLAG_RFID_ENABLE, 0};

IToSmartMenu_text text_3_0[] = "Demonstracao de funcoes de itens";
IToSmartMenu_item item_3_0 = {(char*)text_3_0, 20, MENU_FLAG_ALL_CLEAR, 0};
IToSmartMenu_text text_3_1[] = "Veja mensagens  na porta serial ";
IToSmartMenu_item item_3_1 = {(char*)text_3_1, 5, MENU_FLAG_RFID_ENABLE, 0};

IToSmartMenu_text text_liberaacesso[] = "Acesso permitido****************";
IToSmartMenu_item item_liberaacesso = {(char*)text_liberaacesso, 5, MENU_FLAG_ALL_CLEAR, 0};

IToSmartMenu_text text_sucesso[] = "** Executado  ****com sucesso **";
IToSmartMenu_item item_sucesso = {(char*)text_sucesso, 3, MENU_FLAG_ALL_CLEAR, 0};

IToSmartMenu_text text_taginvalida[] = "**Tag invalida***acesso negado**";
IToSmartMenu_item item_taginvalida = {(char*)text_taginvalida, 3, MENU_FLAG_ALL_CLEAR, 0};




int newCommand = 0; // Indica que tem novo comando para executar
int key_status = menu.EVENT_IDLE; // Inicializa valor do estado das teclas
                             // observe que estamos reutilizando o itosm_status aqui
/**
 * Simulador de leitor RFID
 */
uint8_t rfid_number[6]; // mantem o codigo da tag lida
// Flags para uso na variavel de flags do menu
enum rfid_constants {
  RFID_IDLE,
  RFID_NEWTAG,
};
uint8_t RFID_status = RFID_IDLE; // indica o estado atual de rfid
void RFID_lertag();

/**
 * Simulador do display LCD
 */
char LCD_buffer[32];
void LCD_write(uint8_t coluna, uint8_t linha, char * texto);
void LCD_mostrar();

/**
 * Simulador do RTC
 */
uint8_t RTC_modo = 0;
uint16_t RTC_timer = 0;
void RTC_mostrar();

/**
 * Funcoes definidos pelo desenvolvedor para executar automaticamente pelo menu
 * Estas funcoes devem ser definidas sem parametros e deve retornar um inteiro
 * Elas devem ser declaradas antes da funcao que inicializa o menu pois essas
 * funcoes sao usadas
 */
void rtc_modo0() { RTC_modo = 0; Serial.println(F("Esconde o RTC")); }
void rtc_modo1() { RTC_modo = 1; Serial.println(F("Mostra RTC na linha 1")); }
void rtc_modo2() { RTC_modo = 2; Serial.println(F("Mostra RTC na linha 2")); }

void p_0_0_loop() { // processa a tag lida para liberar o acesso
  uint8_t count = 0;
  if(RFID_status != RFID_NEWTAG) return; // caso nao haja tag lida, nao ha acao
  for (int i = 0; i < 6; i++) { // compara a tag lida com "aaaaaa"
    if(rfid_number[i] == 'a') count++;
  }
  if(count == 6) menu.setItemMessage(&item_liberaacesso, &item_0_0); // libera o acesso
  else menu.setItemMessage(&item_taginvalida, &item_0_0); // mostra mensagem de tag invalida e retorna ao home
  RFID_status = RFID_IDLE;
}
void libera_porta() { Serial.println(F("Porta liberada!!!")); digitalWrite(LED_BUILTIN, HIGH); } // destrava o rele
void trava_porta() { digitalWrite(LED_BUILTIN, LOW); Serial.println(F("Porta travada!!!")); } // trava o rele

void p_1_1_loop() { // processa a tag lida para acesso as mensagens
  uint8_t count = 0;
  if(RFID_status != RFID_NEWTAG) return; // caso nao haja tag lida, nao ha acao
  for (int i = 0; i < 6; i++) { // compara a tag lida com "aaaaaa"
    if(rfid_number[i] == 'a') count++; // rejeita se for "bbbbbb" ou "cccccc"
  }
  if(count == 6) menu.setItem(&item_1_2); // envia para o item 1_2 com mensagem de tag valida
  else menu.setItem(&item_1_3); // envia para o item 1_3 com mensagem de tag invalida
  RFID_status = RFID_IDLE;
}

void p_2_1_loop() { // processa a tag lida para acesso as mensagens
  uint8_t count = 0;
  if(RFID_status != RFID_NEWTAG) return; // caso nao haja tag lida, nao ha acao
  for (int i = 0; i < 6; i++) { // compara a tag lida com "aaaaaa"
    if(rfid_number[i] == 'a') count++; // rejeita se for "bbbbbb" ou "cccccc"
  }
  if(count == 6) menu.setItemMessage(&item_sucesso, &item_0_0); // mostra mensagem de tag valida e retorna ao home
  else menu.setItemMessage(&item_taginvalida, &item_0_0); // mostra mensagem de tag invalida e retorna ao home
  RFID_status = RFID_IDLE;
}

void p_3_1_open0() { Serial.println(F("Funcao open 0")); }
void p_3_1_open1() { Serial.println(F("Funcao open 1")); }
void p_3_1_open2() { Serial.println(F("Funcao open 2")); }
void p_3_1_loop() { Serial.print(F(".")); }
void p_3_1_exit0() { Serial.println(F("Funcao exit 0")); }
void p_3_1_exit1() { Serial.println(F("Funcao exit 1")); }

void executaSempreQueAlteraItem() {
  // mostra o valor do relogio do LCD, linha 1, linha 2 ou nenhum
  if(menu.getFlag(MENU_FLAG_RTC_MODO1)) rtc_modo1();
  else if(menu.getFlag(MENU_FLAG_RTC_MODO2)) rtc_modo2();
  else  rtc_modo0();
  Serial.flush();
  LCD_mostrar();
}

void executaSempreQueOcorrerTimeout() {
  Serial.println(F(" ⇒ KEY_TIMEOUT"));
}

/**
 *  Funcoes do menu
 */
void init_menus() {
  // struct itosm_item *item = &item_0_0;
  Serial.print(F("#init_cmd# inicio - sizeof(struct itosm_item) = "));
  Serial.print(sizeof(struct itosm_item));
  Serial.print(F(" ## sizeof(struct itosm_node) = "));
  Serial.println(sizeof(struct itosm_node));
  // lcd0[17],lcd1[17],timeout,rfidOn,{idle,enter,up,dowm,left,right},(*fOnOpen)(),(*fOnLoop)(),(*fOnExit)()

  // Navegacao automatica de acordo com o evento
  // E importante lembrar de adicionar o sinal ("&") para passar o endereco em vez do objeto
  // As constantes dos eventos sao os mesmos dos flags utilizados na captura de botoes e tags
  // ou seja: EVENT_TIMEOUT, EVENT_NEWEVENT, EVENT_ENTER, EVENT_UP, EVENT_DOWN, EVENT_LEFT, EVENT_RIGHT, EVENT_NEXTSTEP
  menu.setEvent(&item_0_0, menu.EVENT_LEFT, &item_3_0); // tecla esquerda altera menu anterior
  menu.setEvent(&item_0_0, menu.EVENT_RIGHT, &item_1_0); // tecla direita altera menu posterior
  // menu.setEvent(&item_0_0, menu.EVENT_TIMEOUT, &item_1_0); // timeout envia para menu inicial
  menu.addLoopFunction(&item_0_0, p_0_0_loop);
    
  menu.setEvent(&item_1_0, menu.EVENT_LEFT, &item_0_0); // tecla esquerda altera menu anterior
  menu.setEvent(&item_1_0, menu.EVENT_RIGHT, &item_2_0); // tecla direita altera menu posterior
  menu.setEvent(&item_1_0, menu.EVENT_TIMEOUT, &item_0_0); // timeout envia para menu inicial
  menu.setEvent(&item_1_0, menu.EVENT_ENTER, &item_1_1); // tecla enter envia para solicitar tag mestre
  menu.setEvent(&item_1_1, menu.EVENT_TIMEOUT, &item_0_0); // timeout envia para menu inicial
  menu.addLoopFunction(&item_1_1, p_1_1_loop);
  menu.setEvent(&item_1_2, menu.EVENT_TIMEOUT, &item_0_0); // timeout envia para menu inicial
  menu.setEvent(&item_1_3, menu.EVENT_TIMEOUT, &item_0_0); // timeout envia para menu inicial

  menu.setEvent(&item_2_0, menu.EVENT_LEFT, &item_1_0); // tecla esquerda altera menu anterior
  menu.setEvent(&item_2_0, menu.EVENT_RIGHT, &item_3_0); // tecla direita altera menu posterior
  menu.setEvent(&item_2_0, menu.EVENT_TIMEOUT, &item_0_0); // timeout envia para menu inicial
  menu.setEvent(&item_2_0, menu.EVENT_ENTER, &item_2_1); // tecla enter envia para solicitar tag mestre
  menu.setEvent(&item_2_1, menu.EVENT_TIMEOUT, &item_0_0); // timeout envia para menu inicial
  menu.addLoopFunction(&item_2_1, p_2_1_loop);

  menu.setEvent(&item_3_0, menu.EVENT_LEFT, &item_2_0); // tecla esquerda altera menu anterior
  menu.setEvent(&item_3_0, menu.EVENT_RIGHT, &item_0_0); // tecla direita altera menu posterior
  menu.setEvent(&item_3_0, menu.EVENT_TIMEOUT, &item_0_0); // timeout envia para menu inicial
  menu.setEvent(&item_3_0, menu.EVENT_ENTER, &item_3_1); // tecla enter envia para o item de menu com varias funcoes anexadas
  menu.setEvent(&item_3_1, menu.EVENT_TIMEOUT, &item_0_0); // timeout envia para menu inicial
  menu.addOpenFunction(&item_3_1, p_3_1_open0);
  menu.addOpenFunction(&item_3_1, p_3_1_open1);
  menu.addOpenFunction(&item_3_1, p_3_1_open2);
  menu.addLoopFunction(&item_3_1, p_3_1_loop);
  menu.addExitFunction(&item_3_1, p_3_1_exit0);
  menu.addExitFunction(&item_3_1, p_3_1_exit1);

  menu.addOpenFunction(&item_liberaacesso, libera_porta);
  menu.addExitFunction(&item_liberaacesso, trava_porta);

  Serial.println(F("#init_cmd# fim"));
  return;
}


//===========================================================================================
void setup() {
  char charBuffer[64]; // buffer de uso geral
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(115200);
  while(!Serial) {}
  menu.version(charBuffer);
  Serial.println(charBuffer);
  menu.setOnChangeFunction(executaSempreQueAlteraItem);
  menu.setOnTimeoutFunction(executaSempreQueOcorrerTimeout);

  // Esta funcao foi criada somente para separar o codigo da construcao do menu
  // em vez de se misturar com o codigo no setup()
  init_menus();
  // define item do menu inicial
  menu.setItem(&item_0_0);
  // mostra como obter alguns valores do item de menu atual
  // mostra o texto do menu
  Serial.print("=> menu.getText(charBuffer)[");
  Serial.print(menu.getText(charBuffer));
  Serial.print("]: ");
  Serial.println(charBuffer);
  // mostra o valor do timeout do item atual
  Serial.print("=> menu.getTimeout(): ");
  Serial.println(menu.getTimeout());
  // mostra o valor dos flags do item atual
  Serial.print("=> menu.getFlags(): ");
  Serial.println(menu.getFlags());
  menu.printItem(&item_0_0);
}
//===========================================================================================
void loop() { // rotina que processa continuamente de maneira ciclica
  lerTeclas(); // a leitura das teclas sempre esta ativa e tem prioridade sobre outras funcoes

  // temos aqui a leitura de flag de item atual
  // o leitor RFID ira ler tags somente se o flag MENU_FLAG_RFID_ENABLE do item atual estiver ligado
  if(menu.getFlag(MENU_FLAG_RFID_ENABLE)) RFID_lertag();
  else Serial.read(); // consome caracteres que vieram pela serial e nao foram identificadas como teclas ou tags

  RTC_mostrar(); // mostrar data e hora quando os flags indicarem RTC_modo = 1 ou 2

  // Esta funcao deve ser posta no loop() para executar as funcoes onLoop() dos itens
  // e a funcao anexada atraves do
  menu.onLoop(key_status);
  newCommand = 0;
  delay(100);
}
//===========================================================================================
void lerTeclas() {
  int x = -1;
  if(Serial.available() > 0) {
    x = Serial.peek();
  }
  if (x == 54) { // tecla RIGHT sendo pressionada
    if(key_status != menu.EVENT_RIGHT) {
      key_status = menu.EVENT_RIGHT;
      Serial.println(F(" ⇒ KEY_RIGHT"));
      newCommand = 1;
    }
  } else if (x == 56) { // tecla UP sendo pressionada
    if(key_status != menu.EVENT_UP) {
      key_status = menu.EVENT_UP;
      Serial.println(F(" ⇒ KEY_UP"));
      newCommand = 1;
    }
  } else if (x == 50) { // tecla DOWN sendo pressionada
    if(key_status != menu.EVENT_DOWN) {
      key_status = menu.EVENT_DOWN;
      Serial.println(F(" ⇒ KEY_DOWN"));
      newCommand = 1;
    }
  } else if (x == 52) { // tecla LEFT sendo pressionada
    if(key_status != menu.EVENT_LEFT) {
      key_status = menu.EVENT_LEFT;
      Serial.println(F(" ⇒ KEY_LEFT"));
      newCommand = 1;
    }
  }
  else if (x == 53) { // tecla SELECT sendo pressionada
    if(key_status != menu.EVENT_ENTER) {
      key_status = menu.EVENT_ENTER;
      Serial.println(F(" ⇒ KEY_ENTER"));
      newCommand = 1;
    }
  } else if(x == -1) { // nenhuma tecla pressionada
    if(key_status != menu.EVENT_IDLE)
      key_status = menu.EVENT_IDLE;
  }
  if(newCommand > 0) {
    x = Serial.read();
  }
}

void RFID_lertag() {
  int x = -1;
  if(Serial.available() < 1) { return; } // ignora se não houver tag sendo lida
  if(Serial.available() > 0) {
    x = Serial.peek();
    if( (x > 96) && (123 > x) ) {
      x = Serial.read();
    } else {
      if(newCommand == 0) {
        x = Serial.read();
      }
      return;
    }
  }
  Serial.print("ID TAG: "); //Mostra os dados no serial monitor
  for (int i = 0; i < 6; i++) {
    rfid_number[i] = x;
    Serial.print(rfid_number[i], HEX);
    Serial.print(".");
  }
  Serial.println(F(" passou em DOW 2000.10.25 as 20:00:00")); // Exibe o horario
  RFID_status = RFID_NEWTAG;
}

void RTC_mostrar() {
  if(RTC_modo == 0) return;
  if((millis() - RTC_timer) > 1000) {
    LCD_write(0, RTC_modo - 1, "25.10.2000 22:00");
    RTC_timer = millis();
  }
}

void LCD_mostrar() {
  char texto[33];
  menu.getText(texto);
  LCD_write(0, 0, texto);
}

void LCD_write(uint8_t coluna, uint8_t linha, char * texto) {
  uint8_t pos = coluna + linha * 16;
  for(uint8_t i = pos; i < 32; i++) {   
    if(i - pos < strlen(texto)) LCD_buffer[i] = texto[i - pos];
  }
  Serial.print(F("\n/|"));
  for(uint8_t i = 0; i < 16; i++) Serial.print(LCD_buffer[i]);
  Serial.print(F("|\\\n\\|"));
  for(uint8_t i = 16; i < 32; i++) Serial.print(LCD_buffer[i]);
  Serial.println(F("|/"));
}

