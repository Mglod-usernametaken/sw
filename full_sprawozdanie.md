# Marcin Głód 164004
# Systemy Wbudowane - Sprawozdanie

---
# Spis Treści

1. [Cel Ćwiczeń](#cel-ćwiczeń)
2. [Opis Środowiska Pracy](#opis-środowiska-pracy)
    1. [MPLAB X IDE](#mplab-x-ide)
    2. [Explorer 16/32](#explorer-1632)
        1. [Funkcje i przeznaczenie](#funkcje-i-przeznaczenie)
        2. [Zastosowanie](#zastosowanie)
        3. [Moduły](#moduły)
        4. [Kompatybilne procesory](#kompatybilne-procesory)
    3. [Procesor PIC24FJ128GA010](#procesor-pic24fj128ga010)
        1. [Taktowanie](#taktowanie)
        2. [RAM](#ram)
        3. [Parametry](#parametry)
        4. [Zastosowanie IRL (In Real Life)](#zastosowanie-irl-in-real-life)
3. [Opis Zrealizowanych Zadań](#opis-zrealizowanych-zadań)
    1. [Liczniki Binarne](#liczniki-binarne)
    2. [Potencjometr](#potencjometr)
    3. [Mikrofala](#mikrofala)
    4. [Zegar Szachowy](#zegar-szachowy)
4. [Problemy i Trudności](#problemy-i-trudności)
5. [Źródła](#źródła)

---

## Cel Ćwiczeń
- Zapoznanie z działaniem mikrokontrolera
- Poznanie procesu prototypowania systemów opartych o mikrokontrolery
- Zaznajomienie z procesem debugowania oprogramowania mikrokontrolerów
- Podstawy tworzenia oprogramowania w oparciu o płytki rozwojowe

---

## Opis Środowiska Pracy
### MPLAB X IDE
Do programowania i debugowania mikrokontrolerów używałem środowiska MPLAB X IDE, głównie ze względu na wbudowane narzędzie do debugowania kodu i programowania podłączonego mikrokontrolera. Pomimo iż jest to narzędzie dedykowane dla systemów wbudowanych, niestety wymaga użycia dodatkowych narzędzi, by pracować z nim efektywnie. Celem uzupełnienia środowiska deweloperskiego do pisania kodu użyłem również edytora neovim uzupełnionego o clangd LSP oraz systemu kontroli wersji git, aby poprawić komfort pracy i efektywność programowania.

### Explorer 16/32
Płytka rozwojowa Explorer 16/32 jest wszechstronnym narzędziem do prototypowania systemów opartych na mikrokontrolerach. Oferuje szereg modułów i funkcji, dzięki którym rozwój oprogramowania jest procesem szybszym i wygodniejszym.
Głównymi wyróżnikami opisywanej płytki są:

- **Funkcje i przeznaczenie:** Umożliwia szybkie testowanie i rozwój oprogramowania dla różnych mikrokontrolerów produkowanych przez Microchip.
- **Zastosowanie:** Używana do edukacji, projektów badawczych i rozwoju prototypów.
- **Moduły wykorzystane na zajęciach:** Płytka Explorer 16/32 zawiera wbudowane moduły takie jak zasilacz, konwerter UART/I2C na USB(dzięki któremu mogliśmy komunikować się z komputerem), diody, przyciski, potencjometr, oraz ekran LCD (alfanumeryczny 16x2 znaki).
- **pozostałe Moduły:** Wśród modułów niewykorzystanych podczas ćwiczeń znaleźć możemy: piny GPIO, czujnik temperatury, złącze portu Serial, oraz JTAG.
- **Kompatybilne procesory:** Obsługuje szeroką gamę mikrokontrolerów z rodzin PIC24, dsPIC33, oraz PIC32.

### Procesor PIC24FJ128GA010
Mikrokontroler PIC24FJ128GA010 charakteryzuje się następującymi parametrami:

- **Taktowanie:** Maksymalna częstotliwość taktowania wynosi 32 MHz.
- **RAM:** 16 KB pamięci RAM.
- **Parametry:** Oferuje 128 KB pamięci Flash, 21 kanałów ADC, 5 Timerów 16-bitowych oraz 3 interfejsy UART.
- **Zastosowanie IRL (In Real Life):** Znajduje zastosowanie w aplikacjach wymagających średniej mocy obliczeniowej, takich jak kontrolery przemysłowe, urządzenia medyczne i systemy wbudowane w automatyce domowej.

---

## Opis Zrealizowanych Zadań
### Liczniki Binarne
*Uzupełnij opis realizacji zadania.*

### Potencjometr
*Uzupełnij opis realizacji zadania.*

### Mikrofala
*Uzupełnij opis realizacji zadania.*

### Zegar Szachowy
*Uzupełnij opis realizacji zadania.*

---

## Problemy i Trudności
Oto główne trudności z którymi spotkałem się podczas rozwiązywania zadań w ramach przedmiotu Systemy Wbudowane:

- **Problemy z MPLAB X IDE na Linuxie:** Środowisko działa niestabilnie i często sprawia problemy podczas pracy na systemie Linux (oficjalna dokumentacja zatrzymuje się na Ubuntu 14.04 (End of Life od 04.2019), oraz na Fedorze 20 (End of Life 06.2015) ).
- **Instalacja:** Instalacja za pomocą pliku .sh o wadze 900MB jest nieefektywna i stwarza problemy związane z bezpieczeństwem.
- **Niszowa, droga i niedostępna platforma:** Platforma jest mniej dostępna i droższa w porównaniu do bardziej popularnych i tańszych alternatyw jak ATmega328, ESP32 czy PIC16F877A, które są łatwiej dostępne dla studentów do użytku domowego i hobbystycznego.

---

## Źródła
- Dokumentacja od Microchip
- kod Demo z oficjalnej strony Microchip
- Kanał YouTube BinderTronics
- [learnxinyminutes.com/docs/c/](https://learnxinyminutes.com/docs/c/)
- [Microcontroller Embedded C Programming Absolute Beginners Tutorial](https://github.com/a5221985/tutorials/blob/master/Embedded_Systems/microcontroller_embedded_c_programming_absolute_beginners.md)

