# USB Host App Demo

Sistema operativo: Windows 10 / IDE: STM32CUBEIDE / Lenguajes = C

En este ejemplo, la placa de desarrollo STM32F4 Discovery posee habilitada la conexión USB en modo host.
Se conecta un pendrive, que de poder montar el file system lo indicará con un led (verde), caso contrario también lo indicará con otro led (rojo).
Luego si se presiona el botón azul de usuario se escribirá un archivo .txt en la memoria del pendrive. De no poder hacerlo lo indicará con el led rojo.
Al desconectar la unidad se encenderá el led naranja.

Es necesario que la unidad esté formateada en formato FAT32 con un allocation unit size entre 512 bytes y 4096 bytes.
