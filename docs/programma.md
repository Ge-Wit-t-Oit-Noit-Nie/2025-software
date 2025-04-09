# Programma

Het hele idee van de controller is om een progarmma uit te voeren. Om dit flexibel te maken, is er een basis [instructieset](#instuctieset) gemaakt. Met deze instructieset kan een programma gemaakt worden.

## Programmeren

Maak een bestand met de naam "programma.h" en plaats hierin het programma. Zie [voorbeeld programma](#voorbeeld-programma) voor een voorbeeld.
Ieder programma moet minimaal een `OPCODE_HALT` regel bevatten.

Het programma wordt weggeschreven in een vast stukje van het geheugen: **offset 0x08000000 + grote van FLASH**. Dit is terug te vinden in het onderstaande stukje in de ```STM32F412XX_FLASH.ld```.

We hebben te maken met een 32-bit system, daarom kunnen we uitgaan van de volgende grote:

- ```OPCODE```: 4 bytes
- ```void *```: 4 bytes

Dus de grootte van de struct, voordat eventuele padding wordt toegepast, is: OPCODE + (3 x VOID*) = **16 bytes**.

Voor een array met *255* elementen, is de totale benodigde grootte:
**255 * 16 = 4080 bytes.**

```ld
/*Specify the memory areas*/
MEMORY
{
    RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 256K
    FLASH (rx)     : ORIGIN = 0x08000000, LENGTH = 1024K - 4080
    RESERVED (rw)  : ORIGIN = 0x08000000 + (1024K - 4080), LENGTH = 4080
}
```

### Data gebruik

Na het linken van de code, wordt er een overzicht gegeven van het geheugen en de grote. De volgende 3 regio's zijn aanwezig:

- RAM
- FLASH
- RESERVED

Het programma wordt opgeslagen in het ```RESERVED``` blok. De volledige block is beschikbaar voor het maken van een programma.

Voorbeeld van een output. In deze output is te zien dat *5.49%* gebruikt is.

```sh
[build] Memory region         Used Size  Region Size  %age Used
[build]              RAM:       25872 B       256 KB      9.87%
[build]            FLASH:       55824 B    1044496 B      5.34%
[build]         RESERVED:         224 B       4080 B      5.49%
```

## Instuctieset

Een programma kan gemaakt worden door het opzetten van een instructie set. De onderstaande tabel bevat een overzicht van de functies.

| Functie                     | OpCode | Parameter 0                                                                                                                                                              | Parameter 1                                  | Parameter 2                    | Omschrijving                                                                      |
| --------------------------- | ------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | -------------------------------------------- | ------------------------------ | --------------------------------------------------------------------------------- |
| OPCODE_HALT                 | 0x00   |                                                                                                                                                                          |                                              |                                | Stop met het uitvoeren van de programma.                                          |
| OPCODE_JUMP                 | 0x01   | index (0-bound) van de volgende instructie                                                                                                                               |                                              |                                | Laat het programma "springen" naar de instructie met index Parameter0             |
| OPCODE_STORE_SHUTDOWN_INDEX | 0x02   | Index waar het programma naar toe moet om af te sluiten (0-bound)                                                                                                        |                                              |                                | Laat het programma "spingen" naar deze positie als de pauze trigger gedaan wordt. |
| OPCODE_PIN_TOGGLE           | 0x10   | GPIOx – Where x can be (A..K) to select the GPIO peripheral for STM32F429X device or x can be (A..I) to select the GPIO peripheral for STM32F40XX and STM32F427X devices | GPIO_Pin – Specifies the pins to be toggled. |                                | Zet een pin op hoog als deze laag is; Zet een pin op laag als deze hoog is.       |
| OPCODE_PIN_STATE            | 0x11   | GPIOx – Where x can be (A..K) to select the GPIO peripheral for STM32F429X device or x can be (A..I) to select the GPIO peripheral for STM32F40XX and STM32F427X devices | GPIO_Pin – Specifies the pins to be toggled  | GPIO_PIN_SET of GPIO_PIN_RESET | Zet de status van een pin                                                         |
| OPCODE_DELAY                | 0x20   | De gewenste delay in MS                                                                                                                                                  |                                              |                                | Laat het programma wachten voor x ms                                              |
| OPCODE_LOG_PROGRAM_STATE    | 0x21   | n/a                                                                                                                                                                      |                                              |                                | Stuur de huidige status van het programma (alle registers) naar de log            |

## Voorbeeld programma

### Simple programma om de blauwe led te knipperen

Declareer de blauwe led met ```BLUE_LED_Pin``` en zorg dat deze op ```BLUE_LED_GPIO_Port``` gemount zit.

```c
#ifndef __PROGRAMMA_H__
#define __PROGRAMMA_H__

#include <program_controller.h>
#include <main.h>

volatile MEM_PROGRAM_DATA_BLOCK instruction_t instruction[] = {
    {.opcode = OPCODE_PIN_STATE, .parameter0 = (void *)BLUE_LED_Pin, .parameter1 = (void *)BLUE_LED_GPIO_Port, .parameter2 = (void *)GPIO_PIN_SET},
    {.opcode = OPCODE_DELAY, .parameter0 = (void *)1500},
    {.opcode = OPCODE_PIN_TOGGLE, .parameter0 = (void *)BLUE_LED_Pin, .parameter1 = (void *)BLUE_LED_GPIO_Port},
    {.opcode = OPCODE_JUMP, .parameter0 = (void *)1},
    {.opcode = OPCODE_HALT},
};

#endif // __PROGRAMMA_H__
```
