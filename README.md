# ITo Smart Menu

## Como funciona?
Esta biblioteca implementa um "menu" inteligente cujos elementos chamamos de "item"

### Um item cont�m:
	#### Um ponteiro para texto para mostrar em display ou enviar para serial ou log
	#### Um numero para redirecionamento automatico por timeout
	#### Um conjunto de 8 flags (1 byte) para uso do programador
	#### um ponteiro para um node o qual contera outros elementos

Um node complementa o item tornando-o inteligente permitindo automatizar muitos processos economizando muito codigo do programador

### Um node cont�m:
	#### Um conjunto de ponteiro para item (um para cada evento) indicando o destino de redirecionamento
		##### Estes eventos representam botoes acionados, tag RFID lida, Dados disponiveis, etc
		Ao detectar algum destes eventos, o menu redireciona ao item indicado imediatamente
	#### Um ponteiro para uma cadeia de fun��es a serem executados no inicializa��o do item
		##### Ser�o executadas essas fun��es assim que o item se torna atual
	#### Um ponteiro para uma cadeia de fun��es a serem executados durante a permanencia do item como atual
		##### Ser�o executadas essas fun��es a cada ciclo do loop()
	#### Um ponteiro para uma cadeia de fun��es a serem executados na finaliza��o do item
		##### Ser�o executadas essas fun��es assim que o item deixa de ser atual

### Fun��es auxiliares
	#### Fun��o chamada ao alterar o item atual
		Funciona igual a uma fun��o da inicializa��o de item por�m vale para todas as fun��es
	#### Fun��o chamada ao ocorrer timeout
		Fun��o chamada sempre que ocorrer um evento timeout

Para mais informa��es, exemplos e dicas sobre esta biblioteca, por favor, nos visite no
[Project Smart Menu @ labirito.com](http://www.labirito.com/projetos/ "Labirito projects")

Muito obrigado ao Rui Viana que tem me ensinado, dado dicas, sugest�es, testou e refinou esta biblioteca.

## How use this library?

```cpp
#include <EEPROM.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <IToSmartMenu.h>

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

int acendeLED() { // func�o chamada ao mover para o item 1
  digitalWrite(LED_BUILTIN, HIGH);
  return 0;
}

int apagaLED() { // func�o chamada ao mover para o item 2
  digitalWrite(LED_BUILTIN, LOW);
  return 0;
}

int nop() {}

void setup() {
  char aux[33];
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
  // chama as fun��es do menu para executar func�es durante o loop
  menu.onLoop(0);
}

```

## Observa��es

Ao implementar a biblioteca no Arduino, enfrentamos os mesmos problemas que normalmente temos devido a pouqu�ssima mem�ria dispon�vel.

Portanto, foi utilizado a biblioteca da AVR pgmspace.h para manter os textos do men� na �rea de mem�ria de programa.

Por�m, esta biblioteca parece ter muitos problemas, principalmente ao utilizarmos modelos de placas similares e vers�es diferentes da IDE.

Sintam-se a vontade para comentar nos informando como � sua experiencia com a biblioteca.

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
