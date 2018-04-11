/*
  Nome do Aquivo:   Demo3Leds1Button.ino Sketch
  Descrição:        Este sketch faz parte do pacote da biblioteca IToSmartMenu e controla 3 leds e 1 botao
                    A sequencia segue o fluxo abaixo:
                    1 - Estado inicial, item "Home" do menu = todos os leds apagados
                    2 - Estado 2, devido ao evento de botao KEY_ENTER pressionado = item do menu "LED 1 aceso"
                    3 - Evento 3, devido ao evento KEY_TIMEOUT de 5 segundos = item do menu "LED 2 aceso"
                    4 - Evento 4, devido ao evento KEY_TIMEOUT de 5 segundos = item do menu "LED 3 aceso"
                    o evento KEY_TIMEOUT de 5 segundos leva ao estado 2.
                    Os estados 2, 3 e 4 também aceitam o botão KEY_ENTER a qualquer momento para levar ao estado 1

   File version:    20180406
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

#define LED1 7
#define LED2 6
#define LED3 5
#define BOTAO 8

/**
 * **********************************  S M A R T   M E N U  ***************************************
 * Aqui temos as definicoes dos elementos de menu os quais chamamos de menu node
 * Notem que um menu node se compoe de menu item e um array de ponteiros para outros menu item
 * Ha 8 bits no node do menu para uso livre do programador como flag dependente do menu selecionado
 * Deste modo fica automatizado a ativacao ou nao de cada recurso de acordo com o node atual
 * Durante o loop() pode-se tomar alguma acao conforme o flag ativado
 */
// NESTE SKETCH NAO USAMOS OS FLAGS

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
 // temos 1 item para cada estado
IToSmartMenu_text text_home[] = "HOME";
IToSmartMenu_item item_home = {(char*)text_home, -1, 0, 0};
IToSmartMenu_text text_led1[] = "LED 1 aceso";
IToSmartMenu_item item_led1 = {(char*)text_led1, 5, 0, 0};
IToSmartMenu_text text_led2[] = "LED 2 aceso";
IToSmartMenu_item item_led2 = {(char*)text_led2, 5, 0, 0};
IToSmartMenu_text text_led3[] = "LED 3 aceso";
IToSmartMenu_item item_led3 = {(char*)text_led3, 5, 0, 0};

int key_status = menu.EVENT_IDLE; // Inicializa valor do estado das teclas

/**
 * Funcoes definidos pelo desenvolvedor para executar automaticamente pelo menu
 * Estas funcoes devem ser definidas sem parametros e deve retornar um inteiro
 * Elas devem ser declaradas antes da funcao que inicializa o menu pois essas
 * funcoes sao usadas
 */
void home_open() {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  Serial.println(F("HOME"));
}

void led1_open() {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  Serial.println(F("LED 1 aceso"));
}

void led2_open() {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, LOW);
  Serial.println(F("LED 2 aceso"));
}

void led3_open() {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
  Serial.println(F("LED 3 aceso"));
}

/**
 *  Funcoes do menu
 */
void init_menus() {
  // Navegacao automatica de acordo com o evento
  // E importante lembrar de adicionar o sinal ("&") para passar o endereco em vez do objeto
  // As constantes dos eventos sao os mesmos dos flags utilizados na captura de botoes e tags
  // ou seja: EVENT_TIMEOUT, EVENT_NEWEVENT, EVENT_ENTER, EVENT_UP, EVENT_DOWN, EVENT_LEFT, EVENT_RIGHT, EVENT_NEXTSTEP
  menu.setEvent(&item_home, menu.EVENT_ENTER, &item_led1); // tecla enter envia para o estado LED 1

  menu.setEvent(&item_led1, menu.EVENT_TIMEOUT, &item_led2); // timeout envia para menu LED2
  menu.setEvent(&item_led2, menu.EVENT_TIMEOUT, &item_led3); // timeout envia para menu LED3
  menu.setEvent(&item_led3, menu.EVENT_TIMEOUT, &item_led1); // timeout envia para menu LED1

  menu.setEvent(&item_led1, menu.EVENT_ENTER, &item_home); // tecla enter envia para o estado inicial
  menu.setEvent(&item_led2, menu.EVENT_ENTER, &item_home); // tecla enter envia para o estado inicial
  menu.setEvent(&item_led3, menu.EVENT_ENTER, &item_home); // tecla enter envia para o estado inicial

  menu.addOpenFunction(&item_home, home_open); // anexa a funcao que apaga os leds ao item home do menu
  menu.addOpenFunction(&item_led1, led1_open); // anexa a funcao que acende somente o led 1 ao item led1 do menu
  menu.addOpenFunction(&item_led2, led2_open); // anexa a funcao que acende somente o led 2 ao item led2 do menu
  menu.addOpenFunction(&item_led3, led3_open); // anexa a funcao que acende somente o led 3 ao item led3 do menu
}

void setup() {
  char charBuffer[64]; // buffer de uso geral
  pinMode(LED1, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(BOTAO, INPUT_PULLUP);
  Serial.begin(115200);
  while(!Serial) {}
  menu.version(charBuffer);
  Serial.println(charBuffer);

  init_menus();
  // define item do menu inicial
  menu.setItem(&item_home);
}

void loop() { // rotina que processa continuamente de maneira ciclica
  if(!digitalRead(BOTAO)) {
    menu.onLoop(menu.EVENT_ENTER);
  }
  // Esta funcao deve ser posta no loop() para executar as funcoes onLoop() dos itens
  // e a funcao anexada atraves do
  menu.onLoop(menu.EVENT_IDLE);
}

