/*
  Nome do Aquivo:   IToSmartMenu.h Library
  Descrição:        Library to create menu by dynamic elements linked with chain of function nodes
                    The menu starts with a home element
                    Create menu elements and link to home and each other
                    Menu elements have:
                      char array to display the menu option like LCD, Serial, etc
                      chain of links to automatic select next menu element
                      chain of functions to execute on item open() event
                      chain of functions to execute on item loop() event
                      chain of functions to execute on item exit() event
                      timeout value
                      chain of flags indicating status

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
   2018-04-04  0.12 Menu working, events working, functions working, messages working
   2018-04-02  0.11 Menu working with text_n_n and itosm_item in PROGMEM
   2018-03-23  0.10 Menu working without events enrolled. Only 19 nodes can be instantiated
   2018-02-28  0.09 typedef struct itosm_node, itosm_function and itosm_event defined
   2018-02-25  0.08 Example code can simulate key press and tag pass through Serial TX
   2018-02-20  0.05 First version
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   NEXT STEPS:      (priority first)
   none
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

#ifndef ITOSMARTMENU_H_
#define ITOSMARTMENU_H_

#include "arduino.h"

// =============================================================================
// Options to be included into sketch to select textsize, operation mode, debug or reduced code
// -----------------------------------------------------------------------------
#define ITOSMARTMENU_DEBUG
#define ITOSMARTMENU_QTD_EVENTS 8
#define IToSmartMenu_text const PROGMEM static char
#define IToSmartMenu_item struct itosm_item

IToSmartMenu_text ITOSMARTMENU_VERSION[] = "IToSmartMenu v0.12";
/**
 * Para facilitar o uso do menu, declaramos o seguinte enum
 * que servirá para selecionar qual item da lista de "Proximo item de menu" sera direcionado
 * Fica facil usar a mesma lista para identificar os comandos recebidos pois o repasse sera direto
 * Lembrando que o programa roda durante o loop(), portanto comandos sao lidos e executados ciclicamente
 * os nomes podem ser alterados para falicitar memorizar os comandos
 */
// ######### Estrutura de nós com ponteiro para função
typedef struct itosm_item { // sizeof this class = 2 + 2 + 1 + 2
  char *text; // Texto do menu
  int timeout; // Passa para o proximo passo automaticamente depois de timeout segundos. Default = -1
  uint8_t flags; // Ativa ou desativa flags de uso do programador
  struct itosm_node *node;
};

typedef struct itosm_function {
  uint16_t (*function)(void); // pointer to function
  struct itosm_function *next;
};

typedef struct itosm_node { // sizeof this class = 8*2 + 2 + 2 + 2
  const struct itosm_item *events[ITOSMARTMENU_QTD_EVENTS]; // Navega cadeia de itosm_node de acordo com itosm_event apontando pro itosm_node destino. Defaul = NULL
  struct itosm_function *fOnOpen; // executa ao entrar no nó. Desabilita com nop
  struct itosm_function *fOnLoop; // executa a cada vez que passa pelo loop
  struct itosm_function *fOnExit; // executa ao entrar no nó. Desabilita com nop
};

class IToSmartMenu {
  public:
    enum itosm_event {
      EVENT_IDLE = -1, // Representa a acao nula durante o loop
      EVENT_TIMEOUT, // Comando ou tecla pressionada indicando tempo expirado
      EVENT_NEWEVENT, // Comando indicando novo evento para processar
      EVENT_ENTER, // Comando ou tecla Enter ou Ok pressionada
      EVENT_UP, // Comando ou tecla indicando para cima ou incremento
      EVENT_DOWN, // Comando ou tecla indicando para baixo ou decremento
      EVENT_LEFT, // Comando ou tecla indicando esquerda ou retornar
      EVENT_RIGHT, // Comando ou tecla indicando direita ou entrar
      EVENT_NEXTSTEP, // Comando automatico de destino padrao
    };
    enum itosm_error {
      ERROR_NA = 0,
      ERROR_EMPTY,
      ERROR_NULL  
    };

    virtual IToSmartMenu(void); // constructor
    virtual ~IToSmartMenu(void); // destructor
    void version(char * textBuffer); // get the version of this library

    int setEvent(struct itosm_item *itemTarget, int event, struct itosm_item *itemOfTheEvent);
    int addOpenFunction(struct itosm_item *itemTarget, int (*newFunction)());
    int addLoopFunction(struct itosm_item *itemTarget, int (*newFunction)());
    int addExitFunction(struct itosm_item *itemTarget, int (*newFunction)());

    int setItem(struct itosm_item *nextItem);
    int setItemMessage(struct itosm_item *nextItem, struct itosm_item *itemOfTheEvent);
    int setOnChangeFunction(int (*newFunction)());
    int setOnTimeoutFunction(int (*newFunction)());
    int onLoop(int event); // execute every loop() cycle
    void printItem(struct itosm_item *nextItem);
    
    uint8_t getText(char * textBuffer); // copy text from current node to textBuffer and returns the length
    long getTimeout(); // get timeout in seconds from current node
    uint8_t getFlags(); // get flags from current node
    boolean getFlag(uint8_t flag); // get status of the flag

  private:
    void newNode(struct itosm_item *itemTarget);
    int addFunction(struct itosm_function **nodeFunction, int (*newFunction)());
    void executeFunctionChain(struct itosm_function *functionChain);

    struct itosm_item* _currentItem;
    long _currentTimer;
    uint16_t (*_onChangeFunction)(void); // pointer to function
    uint16_t (*_onTimeoutFunction)(void); // pointer to function

}; // class IToSmartMenu

#endif /* ITOSMARTMENU_H_ */

