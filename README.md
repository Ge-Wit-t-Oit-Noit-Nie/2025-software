# 2025-software

If you want to know how to add features or bugfixes, read [CONTRIBUTING.md](CONTRIBUTING.md "Reference to the CONTRIBUTING.md").

## Prerequisites

De volgende applicaties zijn nodig:

- VSCode: 1.98.2 of hoger
- [STM32CubeCLT](https://www.st.com/en/development-tools/stm32cubeclt.html#st-get-software): >= 1.18.0
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html): >= 6.14.0
- [ST-MCU-FINDER-PC](https://www.st.com/en/development-tools/st-mcu-finder-pc.html): >= 6.1.0
- [STM32 VSCode Extension](https://marketplace.visualstudio.com/items?itemName=STMicroelectronics.stm32-vscode-extension)

Verder worden de volgende extenties aangeraden voor VSCode:

### GitHub Suport

- [GitHub Pull Requests](https://marketplace.visualstudio.com/items?itemName=GitHub.vscode-pull-request-github)

### Markdown support

- [Markdown All in One](https://marketplace.visualstudio.com/items?itemName=yzhang.markdown-all-in-one)
- [Markdown Table](https://marketplace.visualstudio.com/items?itemName=TakumiI.markdowntable)
- [markdownlint](https://marketplace.visualstudio.com/items?itemName=DavidAnson.vscode-markdownlint)

### CMake

- [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)

### C/C++

- [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
- [C/C++ Extension pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack)

## Assignments

- *USART3*: Serial port voor sturen van informatie
- *SYS*: Debug: Serial wire, Timebase Source: TIM1
- *RCC*: High speed clock and low speed clock based on cristals build in  
- *SPI1*: SD Card writer over SPI

### PINS

| PIN  | Assignment  |
| ---- | ----------- |
| PA4  | SPI1_CS     |
| PA5  | SPI1_SCK    |
| PA6  | SPI1_MISO   |
| PA7  | SPI1_MOSI   |
| PA13 | SYS_JTMS-SWDIO |
| PA14 | SYS_JTMS-SWCLK |
| PB0  | LED 1 [Green] - DefaultTask running |
| PB1  | GPIO External Interrupt mode with Rise Edge trigger detection - Emergency button |
| PB3  | SYT_JTDO-SWO |
| PB7  | LED 2 [Blue] |
| PB14 | LED 3 [Red] - HardFault_Handler entered |
| PC14 | RCC_OSC32 |
| PC15 | RCC_OSC32_OUT |
| PD8  | UART3_TX |
| PD9  | UART3_RX |
| PH0  | RCC_OSC_IN |
| PH1  | RCC_OSC_OUT |

## Hardware

Voor het ontwikkelen van de software wordt gebruik gemaakt van de **Nucleo-F412ZG Development** board

## Middlewares

Voor het schrijven van de log op de SD card wordt gebruik gemaakt van een stukje code in `Middlewares/sd_logger.c`.
Deze code is betrekkelijk simpel. In het vervolg moet hier de logica zitten om de juiste info in de file te krijgen.

> [!IMPORTANT]
> Voor de `startLogTask` is *3000 WORDS* gereserveerd op de stack. We moeten dus opletten met de code (en dus ook wat opschonen).

De functie `startLogTask` wacht op een item in de queue. Wanneer deze binnen komt, dan wordt er een string gemaakt. Deze string wordt naar de UART gestuurd en ook naar de SD_LOGGER.
Voor de sd_logger wordt ook gebruik gemaakt van een MUTEX om te voorkomen dat er 2 schrijfacties tegelijk plaatsvinden.

Voor mee details kan je kijken in de beschrijving van de [bibliotheek](./docs/gwtonn_library.md).

## Contributors

<table>
<tr>
    <td align="center" style="word-wrap: break-word; width: 150.0; height: 150.0">
        <a href=https://github.com/mrBussy>
            <img src=https://avatars.githubusercontent.com/u/1843912?v=4 width="100;"  style="border-radius:50%;align-items:center;justify-content:center;overflow:hidden;padding-top:10px" alt=Rudi Middel/>
            <br />
            <sub style="font-size:14px"><b>Rudi Middel</b></sub>
        </a>
    </td>
    <td align="center" style="word-wrap: break-word; width: 150.0; height: 150.0">
        <a href=https://github.com/basebom>
            <img src=https://avatars.githubusercontent.com/u/119297631?v=4 width="100;"  style="border-radius:50%;align-items:center;justify-content:center;overflow:hidden;padding-top:10px" alt=basebom/>
            <br />
            <sub style="font-size:14px"><b>basebom</b></sub>
        </a>
    </td>
</tr>
</table>


## Tips & Tricks

### Hoe vindt ik mijn STM32 Nuleo USB port in Windows 11 met PowerShell

```ps
Get-PnpDevice -PresentOnly | Where-Object { $_.InstanceId -match '^USB' }
```

### Debug of upload geeft een foutmelding

![Debug settings](docs/images/set_debugger.png)

### Mijn VSCode can de code niet compileren

Controleer of the juiste instellingen voor STM32 zijn gemaakt.

![STM32 Extension settings](docs/images/stm32_extention_settings.png)
