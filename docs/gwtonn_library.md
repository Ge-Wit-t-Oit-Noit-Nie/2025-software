# Ge Wit't Oit Noit Nie bibliotheek

## SD Card Logger - FreeRTOS implementation

De `sd_logger` implementeerd functionaliteiten om een log te schrijven op een SD kaart (Via de SPI Interface).

De `void startLogTask(void *argument)` is geimplementeerd in the bilbiotheek. De functie is `__weak` gemaakt in `freertos.c`. Om dit te laten werken, is in de STM32CubeMX programma een FreeRTOS taak gemaakt met de naam **logTask** en de functie `startLogTask`. De **Code Generation Option** is ingesteld op **As Weak**.

## Message mechanisme

Om een berichte te schrijven op de SD kaart, kan een specifieke wachtrij gevuld worden. Deze rij heet **loggerQueue**. De queue wordt uitmatisch gemaakt bij het schrijven van de broncode vanuit STM32CubeMX.

Om een bericht te sturen moet je `sd_logger.h` laden in de c code. Daarna kan de volgende code toegevoegd worden:

```C
 MSGQUEUE_OBJ_t msg;

msg.index=1;
msg.message = 'A';
osMessageQueuePut(loggerQueueHandle, &msg, 0, 0U);
```

## Programma controller

Voor het uitvoeren van de programma, is er een programma controller. De details staan beschreven in [programma.md](./programma.md).