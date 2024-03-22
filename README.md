
# Labsolver

---

## Zasady działania

Program **Labsolver** w języku C służy do rozwiązywania labiryntów. Aby ograniczyć używaną przez
program pamięć, rozwiązywany labirynt dzielony jest na elementy nazywane chunk’ami.
Program przy szukaniu rozwiązania labiryntu przechodzi przez niego wczytując potrzebne
w danym momencie chunki. Dla labiryntu o rozmiarach 1024 na 1024 program użyje nie
więcej niż 512kB pamięci.

### Przykład użycia

Aby uruchomić program, należy postępować zgodnie z poniższymi krokami:

1. Pobierz pliki programu.
2. Umieść labirynt w formacie opisanym w sekcji "Format przekazywanego labiryntu" w katalogu z pobranymi plikami.
3. Uruchom plik makefile.
4. W terminalu wpisz: `./labsolver` z odpowiednimi parametrami.

Przykłady użycia:
```bash
./labsolver -l labirynt.txt -f rozwiazanie.txt
./labsolver -l labirynt.txt -d
```

---

## Parametry wejściowe 
- -h → Wywołanie instrukcji help 
- -l [nazwa-pliku] → Nazwa rozwiązywanego labiryntu 
- -f [nazwa-pliku] → Nazwa powstałego pliku wyjściowego 
- -d → Flaga powodująca wyświetlanie błędów na stdout zamiast w pliku log.txt

## Dane wejściowe i wyjściowe

Labirynt jest reprezentowany w formie tekstowej, gdzie:
- `P` oznacza punkt wejścia,
- `K` oznacza punkt wyjścia,
- `X` oznacza ścianę labiryntu,
- spacja oznacza miejsce, po którym można się poruszać.

Program jako wynik generuje plik tekstowy zawierający instrukcje przejścia w formacie:\
(przykład)\
START\
FORWARD 1\
TURNLEFT\
FORWARD 4\
TURNRIGHT\
FORWARD 3\
STOP


### Obsługa błędów

W przypadku błędów, informacje są zapisywane do pliku `log.txt`. 
Znaczenia kodów błędów:

- `11` - Zły format labiryntu
- `12` - Błąd zapisu pliku wyjściowego
- `13` - Niepoprawny parametr wejściowy

Więcej informacji na temat kodów błędów znajdziesz w dokumentacji programu.

---

## Instrukcja HELP

Aby uzyskać pomoc, wpisz:
```bash
./labsolver -h
```

Treść instrukcji help znajdziesz w dokumentacji programu.
