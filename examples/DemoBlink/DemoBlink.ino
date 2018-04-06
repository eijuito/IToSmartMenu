#include <EEPROM.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <IToSmartMenu.h>

char aux[33];

// construtor do menu
IToSmartMenu menu;

// itens do menu
IToSmartMenu_text text_0_0[] = "Menu item 1";
IToSmartMenu_item item_0_0 = {(char*)text_0_0, 1, 0x00, 0};

IToSmartMenu_text text_1_0[] = "Menu item 2";
IToSmartMenu_item item_1_0 = {(char*)text_1_0, 1, 0x01, 0};

// item de mensagem - aparece temprariamento conforme o tempo de timeout
IToSmartMenu_text text_mensagem[] = "Processando";
IToSmartMenu_item item_mensagem = {(char*)text_mensagem, 3, 0x00, 0};

int acendeLED() { // funcão chamada ao mover para o item 1
  digitalWrite(LED_BUILTIN, HIGH);
  menu.getText(aux);
  Serial.println(aux);
  return 0;
}

int apagaLED() { // funcão chamada ao mover para o item 2
  digitalWrite(LED_BUILTIN, LOW);
  menu.getText(aux);
  Serial.println(aux);
  return 0;
}

int nop() {}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(115200);
  while(!Serial) {}
  menu.version(aux);
  Serial.println(aux);
  menu.setOnChangeFunction(nop);
  menu.setOnTimeoutFunction(nop);

  // define eventos para movimento automatico por timeout
  menu.setEvent(&item_0_0, menu.EVENT_TIMEOUT, &item_1_0);
  menu.addOpenFunction(&item_0_0, acendeLED);

  menu.setEvent(&item_1_0, menu.EVENT_TIMEOUT, &item_0_0);
  menu.addOpenFunction(&item_1_0, apagaLED);

  menu.setItem(&item_1_0);
}

void loop() {
  // chama as funções do menu para executar funcões durante o loop
  menu.onLoop(0);
}

