#include "IToSmartMenu.h"

char aux[33];

// construtor do menu
IToSmartMenu menu;

// itens do menu
IToSmartMenu_text text_0_0[] = "Menu item 1"; // texto do item 1 do menu
IToSmartMenu_item item_0_0 = {(char*)text_0_0, // link para o texto
                                            1, // tempo em segundos da permanencia neste item
                                         0x00, // Flags não são usados neste exemplo
                                           0}; // Sempre zero

IToSmartMenu_text text_1_0[] = "Menu item 2"; // texto do item 2 do menu
IToSmartMenu_item item_1_0 = {(char*)text_1_0, // link para o texto
                                            1, // tempo em segundos da permanencia neste item
                                         0x01, // Flags não são usados neste exemplo
                                           0}; // Sempre zero

void acendeLED() { // funcão chamada ao mover para o item 1
  digitalWrite(LED_BUILTIN, HIGH);
  menu.getText(aux);
  Serial.println(aux);
}

void apagaLED() { // funcão chamada ao mover para o item 2
  digitalWrite(LED_BUILTIN, LOW);
  menu.getText(aux);
  Serial.println(aux);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(115200);
  while(!Serial) {}
  menu.version(aux);
  Serial.println(aux);

  // define eventos para movimento automatico por timeout
  menu.setEvent(&item_0_0, menu.EVENT_TIMEOUT, &item_1_0); // para o item 0_0, ao ocorrer timeout, vá para o item 1_0
  menu.addOpenFunction(&item_0_0, acendeLED); // Ao abrir o item 0_0, acende o LED

  menu.setEvent(&item_1_0, menu.EVENT_TIMEOUT, &item_0_0); // para o item 0_0, ao ocorrer timeout, vá para o item 1_0
  menu.addOpenFunction(&item_1_0, apagaLED); // Ao abrir o item 0_0, apaga o LED

  menu.setItem(&item_1_0); // inicie o ciclo com o item 1_0 selecionado
}

void loop() {
  // chama as funções do menu para executar funcões durante o loop
  menu.onLoop(0);
}


