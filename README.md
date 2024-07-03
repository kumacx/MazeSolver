
# Mazesolver

---

## Zasady działania

Program **Mazesolver** w języku C służy do rozwiązywania labiryntów. Aby ograniczyć używaną przez
program pamięć, rozwiązywany labirynt dzielony jest na elementy nazywane chunk’ami.
Program przy szukaniu rozwiązania labiryntu przechodzi przez niego wczytując potrzebne
w danym momencie chunki. Dla labiryntu o rozmiarach 1024 na 1024 w domyślnych parametrach program użyje nie
więcej niż 512kB pamięci.

### Przykład użycia

Aby uruchomić program, należy postępować zgodnie z poniższymi krokami:

1. Pobierz pliki programu.
2. Umieść labirynt w formacie opisanym w sekcji "Format przekazywanego labiryntu" w katalogu z pobranymi plikami.
3. Uruchom plik makefile.
4. W terminalu wpisz: `./labsolver` z odpowiednimi parametrami.

Przykłady użycia:
```bash
./labsolver [parametry]
```


## Parametry wejściowe 

Parametry wejściowe dostępne są w instrukcji HELP.

## Dane wejściowe i wyjściowe

Labirynt na wejściu i wyjściu przekazywany jest w formie tekstowej lub binarnej.


---

## Instrukcja HELP

Aby uzyskać pomoc, wpisz:
```bash
./labsolver -h
```

Treść instrukcji help:

Program znajduje najszybsza sciezke w labiryncie i zapisuje ja do pliku


Argumenty:

-t <tryb pracy>

0 - Wejscie z pliku tekstowego, wyjscie w pliku tekstowym (flaga -o wymagana)

1 - Wejscie z pliku binarnego, wyjscie w pliku binarnym (flaga -o zabroniona)

2 - Wejscie z pliku binarnego, wyjscie w pliku tekstowym (flaga -o wymagana)



-n <nazwa pliku z labiryntem> (wymagany)
	

-o <nazwa pliku wynikowego> (czy wymagane zalezy od flagi t)

"W przypadku braku domyslnym plikiem jest output.txt
	

-s <rozmiar chunku> (niewymagany)

Rozmiar chunku\nIm wiecej, tym wieksze zuzycie pamieci RAM

Warunki: s >= 2
	

-c <rozmiar cache> (niewymagany)

Liczba chunkow przechowywana w pamieci programu\nIm wiecej, tym wieksze zuzycie pamieci RAM

Warunki: 2 <= c <= liczba chunkow w labiryncie
	

-r <rozmiar rekordu> (niewymagany)

Definiuje rozmiar jednego rekordu w pliku\nIm wiecej, tym wieksze zapelnienie dysku podczas dzialania programu

Warunki: r >= 15
	

-d (niewymagany)

Uruchamia program w trybie debug mode
