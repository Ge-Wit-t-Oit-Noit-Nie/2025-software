# 2025-software

## Development omgeving

De sofware wordt gemaakt in een STM32CubeIDE omgeving. 

### Assignments

- *USART3*: Serial port voor sturen van informatie
- *SYS*: Debug: Serial wire, Timebase Source: TIM1

#### PINS

| PIN  | Assignment  |
| ---- | ----------- |
| PA13 | SYS_JTMS-SWDIO |
| PA14 | SYS_JTMS-SWCLK |
| PB0  | LED 1 [Green] - DefaultTask running |
| PB3  | SYT_JTDO-SWO | 
| PB7  | LED 2 [Blue] | 
| PB14 | LED 3 [Red] - HardFault_Handler entered | 
| PD8  | UART3_TX |
| PD9  | UART3_RX |

## Hardware

Voor het ontwikkelen van de software wordt gebruik gemaakt van de **Nucleo-F412ZG Development** board

## Tips & Tricks

### Hoe vindt ik mijn STM32 Nuleo USB port in Windows 11 met PowerShell

```
Get-PnpDevice -PresentOnly | Where-Object { $_.InstanceId -match '^USB' }
```

### Debug of upload geeft een foutmelding

![Debug settings](docs/images/set_debugger.png)