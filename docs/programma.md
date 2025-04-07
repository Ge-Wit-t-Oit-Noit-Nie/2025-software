# Programma

Het hele idee van de controller is om een progarmma uit te voeren. Om dit flexibel te maken, is er een basis [instructieset](#instuctieset) gemaakt. Met deze instructieset kan een programma gemaakt worden.

## Programmeren

Maak een bestand met de naam "programma.h" en plaats hierin het programma. Het volgende programma is hier een voorbeeld van.

```c
#ifndef __PROGRAMMA_H__
#define __PROGRAMMA_H__

#include <program_controller.h>
#include <main.h>

const instruction_t instruction[] = {
    {.opcode = OPCODE_PIN_STATE, .parameter0 = (void *)BLUE_LED_Pin, .parameter1 = (void *)BLUE_LED_GPIO_Port, .parameter2 = (void *)GPIO_PIN_SET},
    {.opcode = OPCODE_DELAY, .parameter0 = (void *)1500},
    {.opcode = OPCODE_PIN_TOGGLE, .parameter0 = (void *)BLUE_LED_Pin, .parameter1 = (void *)BLUE_LED_GPIO_Port},
    {.opcode = OPCODE_JUMP, .parameter0 = (void *)1},
};

#endif // __PROGRAMMA_H__
```


## Instuctieset

Een programma kan gemaakt worden door het opzetten van een instructie set. De onderstaande tabel bevat een overzicht van de functies.

| Functie           | OpCode | Parameters                                                                                                                                                                                                                                     | Omschrijving                                                                |
| ----------------- | ------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------- |
| OPCODE_PIN_TOGGLE | 0x00   | Parameter0: GPIOx – Where x can be (A..K) to select the GPIO peripheral for STM32F429X device or x can be (A..I) to select the GPIO peripheral for STM32F40XX and STM32F427X devices; Parameter1: GPIO_Pin – Specifies the pins to be toggled. | Zet een pin op hoog als deze laag is; Zet een pin op laag als deze hoog is. |
| OPCODE_PIN_STATE | 0x01 | Parameter0: GPIOx – Where x can be (A..K) to select the GPIO peripheral for STM32F429X device or x can be (A..I) to select the GPIO peripheral for STM32F40XX and STM32F427X devices; Parameter1: GPIO_Pin – Specifies the pins to be toggled; Parameter2: GPIO_PIN_SET of GPIO_PIN_RESET | Zet de status van een pin |
| OPCODE_DELAY | 0x11 | Parameter0: De gewenste delay in MS | Laat het programma wachten voor x ms |
| OPCODE_LOG_PROGRAM_STATE | 0x12 | n/a | Stuur de huidige status van het programma (alle registers) naar de log |
| OPCODE_JUMP | 0x21 | Parameter0: index (0-bound) van de volgende instructie | Laat het programma "springen" naar de instructie met index Parameter0 |
| OPCODE_HALT | 0x03 | | Stop met het uitvoeren van de programma. |
| OPCODE_STORE_SHUTDOWN_INDEX  | 0x02 | Parameter0: Index waar het programma naar toe moet om af te sluiten (0-bound) | Laat het programma "spingen" naar deze positie als de pauze trigger gedaan wordt.  |

## Voorbeeld programma

### Simple programma om de blauwe led te knipperen

Declareer de blauwe led met ```BLUE_LED_Pin``` en zorg dat deze op ```BLUE_LED_GPIO_Port``` gemount zit.

```c
const instruction_t instruction[] = {
  { .opcode = OPCODE_PIN_TOGGLE, .parameter0 = (void*) BLUE_LED_Pin, .parameter1 = (void*) BLUE_LED_GPIO_Port },
  { .opcode = OPCODE_DELAY, .parameter0 = (void *)pdMS_TO_TICKS(1500)},
  };
```
