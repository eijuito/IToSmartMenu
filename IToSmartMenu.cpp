/**
 * Nome do Aquivo:   IToSmartMenu.cpp Library
 * Descrição:        See IToSmartMenu.h
 * 
 * File version:    20160403
 * Dependency:      Arduino.h                       // Base do Arduino
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * See IToSmartMenu-YYYYMMDDa.h to see more details
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#include <Arduino.h>
#include "IToSmartMenu.h"

/**
 * **********************************  Public functions  ***************************************
 */
/**
 * Constructor
 */
IToSmartMenu::IToSmartMenu(void) {
  _currentItem = NULL;
}
/**
 * Destructor
 */
IToSmartMenu::~IToSmartMenu(void) {
}

void IToSmartMenu::version(char * textBuffer) {
  int i = 0;
  for(; pgm_read_byte_near(ITOSMARTMENU_VERSION + i) > 0; i++)
    textBuffer[i] = pgm_read_byte_near(ITOSMARTMENU_VERSION + i);
  textBuffer[i] = '\0';
}

void IToSmartMenu::setEvent(struct itosm_item *itemTarget, int event, struct itosm_item* itemOfTheEvent) {
  if(itemOfTheEvent == NULL) return;
  if(itemTarget == NULL) return;
  if(itemTarget->node == NULL) newNode(itemTarget);
  itemTarget->node->events[event] = itemOfTheEvent;
}

void IToSmartMenu::addOpenFunction(struct itosm_item *itemTarget,  void (*newFunction)()) {
  if(itemTarget == NULL) return;
  if(itemTarget->node == NULL) newNode(itemTarget);
  addFunction(&itemTarget->node->fOnOpen, newFunction);
}

void IToSmartMenu::addLoopFunction(struct itosm_item *itemTarget, void (*newFunction)()) {
  if(itemTarget == NULL) return;
  addFunction(&itemTarget->node->fOnLoop, newFunction);
}

void IToSmartMenu::addExitFunction(struct itosm_item *itemTarget, void (*newFunction)()) {
  if(itemTarget == NULL) return;
  addFunction(&itemTarget->node->fOnExit, newFunction);
}

int IToSmartMenu::setItem(struct itosm_item *nextItem) {
  return setItemMessage(nextItem, NULL);
}

int IToSmartMenu::setItemMessage(struct itosm_item *nextItem, struct itosm_item *itemOfTheEvent) {
  if(nextItem == NULL) return ERROR_NULL;
//  if(_currentItem != NULL) executeFunctionChain((uint16_t)_currentItem->node->fOnExit);
  if(_currentItem != NULL) executeFunctionChain(_currentItem->node->fOnExit);
  _currentItem = nextItem; // point to new item
  if(itemOfTheEvent) setEvent(nextItem, EVENT_TIMEOUT, itemOfTheEvent);
  _currentTimer = millis();
  if(_onChangeFunction) _onChangeFunction();
//  executeFunctionChain((uint16_t)_currentItem->node->fOnOpen);
  executeFunctionChain(_currentItem->node->fOnOpen);
  return ERROR_NA;
}

void IToSmartMenu::setOnChangeFunction(void (*newFunction)()) {
  _onChangeFunction = newFunction;
}

void IToSmartMenu::setOnTimeoutFunction(void (*newFunction)()) {
  _onTimeoutFunction = newFunction;
}

void IToSmartMenu::onLoop(int event) {
  if(event > 0) {
    if(_currentItem->node->events[event] > 0)
      setItem(_currentItem->node->events[event]);
  } else {
    if((getTimeout() > -1) // if timeout is active
        && (_currentItem->node->events[EVENT_TIMEOUT] != NULL) // there is valid pointer for timeout event
        && ((millis() - _currentTimer) > getTimeout())) { // and time has expired
      if(_onTimeoutFunction) _onTimeoutFunction(); // executa a funcao registrada para timeouts
      setItem(_currentItem->node->events[EVENT_TIMEOUT]); // alterna para o item de menu conforme o evento
    }
  }
//  executeFunctionChain((uint16_t)_currentItem->node->fOnLoop); // executa a cadeia de funcoes on loop do item do menu corrente
  executeFunctionChain(_currentItem->node->fOnLoop); // executa a cadeia de funcoes on loop do item do menu corrente
}

void IToSmartMenu::printItem(struct itosm_item *item) {
  uint8_t i = 0;
  char charBuffer[64];
  if(item == NULL) item = _currentItem;
  Serial.print(F("##PrintItem=> "));
//  Serial.print((uint16_t) item);
  Serial.print((unsigned int) item);
  if(item == NULL) {
    Serial.println("");
    return;
  }
  Serial.print(F(" text:["));
  for(i = 0; pgm_read_byte_near(item->text + i) > 0; i++)
    charBuffer[i] = pgm_read_byte_near(item->text + i);
  charBuffer[i] = '\0';
  Serial.print(charBuffer);
  Serial.print(F("] timeout:"));
  Serial.print(item->timeout);
  Serial.print(F(" flags:"));
  Serial.print(item->flags);
  Serial.print(F(" node:"));
  Serial.println((unsigned int) item->node);
  if(item->node == NULL) return;
  Serial.print(F("....fOnOpen:"));
//  Serial.print((uint16_t) item->node->fOnOpen);
  Serial.print((unsigned int) item->node->fOnOpen);
  Serial.print(F(" fOnLoop:"));
//  Serial.print((uint16_t) item->node->fOnLoop);
  Serial.print((unsigned int) item->node->fOnLoop);
  Serial.print(F(" fOnExit:"));
//  Serial.println((uint16_t) item->node->fOnExit);
  Serial.println((unsigned int) item->node->fOnExit);
  Serial.print(F("....EVENT_TIMEOUT:"));
//  Serial.print((uint16_t) item->node->events[EVENT_TIMEOUT]);
  Serial.print((unsigned int) item->node->events[EVENT_TIMEOUT]);
  Serial.print(F(" EVENT_RETURN:"));
//  Serial.print((uint16_t) item->node->events[EVENT_RETURN]);
  Serial.print((unsigned int) item->node->events[EVENT_RETURN]);
  Serial.print(F(" EVENT_ENTER:"));
//  Serial.print((uint16_t) item->node->events[EVENT_ENTER]);
  Serial.print((unsigned int) item->node->events[EVENT_ENTER]);
  Serial.print(F(" EVENT_UP:"));
//  Serial.println((uint16_t) item->node->events[EVENT_UP]);
  Serial.println((unsigned int) item->node->events[EVENT_UP]);
  Serial.print(F("....EVENT_DOWN:"));
//  Serial.print((uint16_t) item->node->events[EVENT_DOWN]);
  Serial.print((unsigned int) item->node->events[EVENT_DOWN]);
  Serial.print(F(" EVENT_LEFT:"));
//  Serial.print((uint16_t) item->node->events[EVENT_LEFT]);
  Serial.print((unsigned int) item->node->events[EVENT_LEFT]);
  Serial.print(F(" EVENT_RIGHT:"));
//  Serial.print((uint16_t) item->node->events[EVENT_RIGHT]);
  Serial.print((unsigned int) item->node->events[EVENT_RIGHT]);
  Serial.print(F(" EVENT_NEXT:"));
//  Serial.println((uint16_t) item->node->events[EVENT_NEXT]);
  Serial.println((unsigned int) item->node->events[EVENT_NEXT]);
  return;
}

uint8_t IToSmartMenu::getText(char * textBuffer) {
  uint8_t i = 0;
  for(; pgm_read_byte_near(_currentItem->text + i) > 0; i++) {
    textBuffer[i] = pgm_read_byte_near(_currentItem->text + i);
  }
  textBuffer[i] = '\0';
  return i;
  // strcpy_P(textBuffer, (char*)pgm_read_word_near(&(_currentNode->item->text))); // Necessary casts and dereferencing, just copy.
  // return strlen(textBuffer);
}

long IToSmartMenu::getTimeout() {
  // return (int)pgm_read_word_near(&_currentNode->item->timeout);
  return (long)_currentItem->timeout * 1000;
}

uint8_t IToSmartMenu::getFlags() {
  // return (uint8_t)pgm_read_word_near(&_currentNode->item->flags);
  return (uint8_t)_currentItem->flags;
}

boolean IToSmartMenu::getFlag(uint8_t flag) {
  // return (uint8_t)pgm_read_word_near(&_currentNode->item->flags);
  return (getFlags() & flag) > 0;
}

/**
 * **********************************  Private functions  ***************************************
 */
void IToSmartMenu::newNode(struct itosm_item *itemTarget) {
    itemTarget->node = (struct itosm_node *) malloc(sizeof(struct itosm_node)); // alloc memory of menu node
    if(!itemTarget->node) Serial.print(F("#malloc() error"));
    for(uint8_t i = 0; i < ITOSMARTMENU_QTD_EVENTS; i++) itemTarget->node->events[i] = NULL;
    itemTarget->node->fOnOpen = NULL;
    itemTarget->node->fOnLoop = NULL;
    itemTarget->node->fOnExit = NULL;
}

void IToSmartMenu::addFunction(struct itosm_function **nodeFunction, void (*newFunction)()) {
  if(newFunction == NULL) return;
  while(*nodeFunction != NULL) {
    nodeFunction = &(*nodeFunction)->next;
  }
  *nodeFunction = (struct itosm_function *)malloc(sizeof(struct itosm_function)); // alloc memory of function node
  (*nodeFunction)->function = newFunction;
  (*nodeFunction)->next = NULL;
}

void IToSmartMenu::executeFunctionChain(struct itosm_function *functionChain) {
  struct itosm_function **fNode;
  if(functionChain == NULL) return;
  fNode = &functionChain;
  while(*fNode != NULL) {
    (*fNode)->function();
    fNode = &(*fNode)->next;
  }
  return;
}

