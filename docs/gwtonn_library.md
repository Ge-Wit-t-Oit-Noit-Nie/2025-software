# Ge Wit't Oit Noit Nie bibliotheek

- [Ge Wit't Oit Noit Nie bibliotheek](#ge-witt-oit-noit-nie-bibliotheek)
  - [Programma controller](#programma-controller)
  - [SD Card Logger - FreeRTOS implementation](#sd-card-logger---freertos-implementation)
  - [Message mechanisme](#message-mechanisme)
  - [FatFS met de SD Card Controller via SPI](#fatfs-met-de-sd-card-controller-via-spi)

Om het overzichtelijk te houden, is er een losse bibliotheek gemaakt voor Ge Wit't Oit Noit Nie. De bibliotheek is te vinden in `{workspace}/Middlewares/gwtonn`.

## Programma controller

Voor het uitvoeren van de programma, is er een programma controller. De details staan beschreven in [programma.md](programma.md).

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

## FatFS met de SD Card Controller via SPI

Om bestanden te kunnen maken op een SD kaart, gebruiken we een SD Card reader. Deze wordt aangesloten via een SPI Interface met de Nucleo board. Vervolgens gebruiken we de FatFS library om een bestandsysteem te simuleren.

De FatFS staat aangemerkt als `USER_Driver` in STM32CubeMX. Deze userdriver is gedefineerd in [user_diskio.h](../FATFS/Target/user_diskio.h) en geimplementeerd in  [user_diskio.c](../FATFS/Target/user_diskio.c).

De vertaalslag van de FATFS implentatie naar SPI commando's gebeurt in [fatfs_sd.c](../Middlewares/gwtonn/fatfs_sd.c).
