# ITo Smart Menu

Esta biblioteca implementa um "menu" inteligente cujos elementos chamamos de "item"

Para mais informações, exemplos e dicas sobre esta biblioteca, por favor, nos visite no
[Project Smart Menu @ labirito.com](http://www.labirito.com/projetos/ "Labirito projects")

Muito obrigado ao Rui Viana que tem me ensinado, dado dicas, sugestões, testou e refinou esta biblioteca.

## How use this library?

```cpp
#include <EEPROM.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include "IToSmartMenu.h"

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
  return 0;
}

int apagaLED() { // funcão chamada ao mover para o item 2
  digitalWrite(LED_BUILTIN, LOW);
  return 0;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(115200);
  while(!Serial) {}

  // define eventos para movimento automatico por timeout
  menu.setEvent(&item_0_0, menu.EVENT_TIMEOUT, &item_1_0);
  menu.addOpenFunction(&item_0_0, acendeLED);

  menu.setEvent(&item_1_0, menu.EVENT_TIMEOUT, &item_0_0);
  menu.addOpenFunction(&item_1_0, apagaLED);

  menu.setItem(&item_0_0);
}

void loop() {
  // chama as funções do menu para executar funcões durante o loop
  menu.onLoop(0);
}


```

## Observações

Ao implementar a biblioteca no Arduino, enfrentamos os mesmos problemas que normalmente temos devido a pouquíssima memória disponível.

Portanto, foi utilizado a biblioteca da AVR pgmspace.h para manter os textos do menú na área de memória de programa.

Porém, esta biblioteca parece ter muitos problemas, principalmente ao utilizarmos modelos de placas similares e versões diferentes da IDE.

Sintam-se a vontade para comentar nos informando como é sua experiencia com a biblioteca.

## License

**Copyright (c) 2010 Labirito. All right reserved.**

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
