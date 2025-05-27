# Proiect Utilitare Terminal (Proiectarea Algoritmilor)

Acest proiect conține o suită de utilitare pentru terminal dezvoltate în C, axate pe manipularea fișierelor, directoarelor și procesarea argumentelor din linia de comandă. Proiectul explorează utilizarea diverselor structuri de date precum stive, arbori și grafuri pentru a implementa funcționalități similare comenzilor standard din Linux (de ex., `ls`, `diff`, `cd`).

## Utilitare Dezvoltate și Funcționalitatea Lor

Proiectul este împărțit în patru task-uri principale, fiecare având ca rezultat un utilitar sau o componentă software distinctă:

### 1. Task 1: Parser de Argumente în Linia de Comandă

* **Ce face funcția?**
    Această componentă este responsabilă pentru preluarea și interpretarea argumentelor, opțiunilor și flag-urilor transmise unui program atunci când este apelat din terminal[cite: 5]. Programul parsează șirul de intrare (așa cum este primit de `main(int argc, char** argv)`) și stochează detaliile fiecărui parametru (tip, nume/identificator, valoare, format lung/scurt) în structuri de date interne definite[cite: 26, 39]. Se gestionează diferitele formate pentru opțiuni (lungi cu `--`, scurte cu `-`) și diferiți delimitatori (`=` sau spațiu) pentru valori[cite: 17, 20, 21]. De asemenea, se iau în considerare flag-urile, care acționează ca switch-uri on/off și pot avea și ele formate lungi sau scurte[cite: 24].

* **Pentru ce este utilă?**
    Un parser de argumente robust este fundamental pentru orice program care necesită o configurare flexibilă din linia de comandă. Permite programelor să primească inputuri variate și să își modifice comportamentul în funcție de acestea, fără a necesita recompilare sau modificări în codul sursă. Abilitatea de a defini argumente, opțiuni cu valori și flag-uri oferă o interfață standard și puternică pentru utilizatori, similară cu cea a uneltelor consacrate din sistemele de operare. Această componentă este esențială pentru a construi utilitarele din task-urile următoare, permițându-le să fie controlate prin parametri specifici.

---

### 2. Task 2: `util_ls` (Listare Conținut Director)

* **Ce face funcția?**
    `util_ls` este o versiune simplificată a comenzii `ls` din Linux[cite: 66]. Primește ca intrare calea absolută către un director și, pe baza opțiunilor specificate, afișează conținutul acestuia[cite: 67, 68]. Informațiile despre subdirectoare și fișiere (nume, cale, adâncime, dimensiune pentru fișiere) sunt stocate intern folosind o structură de date de tip stivă[cite: 67, 79]. Funcționalitățile includ:
    * Afișarea numelor subdirectoarelor și fișierelor din directorul curent, sortate alfabetic[cite: 68]. Se afișează doar elementele din directorul cerut, nu și conținutul acestora[cite: 69].
    * Afișarea doar a numelor subdirectoarelor, sortate alfabetic (similar cu `ls --directory` sau `ls -d`)[cite: 70, 73].
    * Afișarea dimensiunii fișierelor într-un format "human-readable" (K, M, G), pe lângă numele acestora (similar cu `ls -h` sau `ls --human-readable`)[cite: 71, 73].
    * Afișarea recursivă a conținutului subdirectoarelor (similar cu `ls -R` sau `ls --recursive`)[cite: 72, 73].
    Indentarea este folosită pentru a reflecta adâncimea structurii de directoare în cazul afișării recursive[cite: 94]. Pentru a obține informații despre tipul elementelor (fișier/director) și dimensiunea fișierelor, se utilizează bibliotecile `dirent.h` și `sys/stat.h`[cite: 74, 77].

* **Pentru ce este utilă?**
    Acest utilitar este util pentru navigarea și inspectarea sistemului de fișiere dintr-un program C. Permite utilizatorilor să obțină rapid informații despre structura și conținutul unui director, cu opțiuni de personalizare a afișajului. Implementarea sa necesită înțelegerea modului de interacțiune cu sistemul de fișiere la nivel de program și gestionarea structurilor de date pentru a stoca și sorta eficient aceste informații. Este un exercițiu practic pentru lucrul cu operații I/O pe fișiere și directoare și pentru sortarea datelor.

---

### 3. Task 3: `util_cd` (Calculare Cale Relativă)

* **Ce face funcția?**
    `util_cd` are scopul de a calcula și afișa calea relativă între două directoare specificate prin căi absolute (un director sursă și un director destinație)[cite: 99]. Pentru a realiza acest lucru, se construiește un arbore general (ne-binar) care reprezintă structura de directoare pornind de la ultimul director comun al celor două căi[cite: 99, 100, 103]. Fiecare nod din arbore conține numele directorului, numărul de subdirectoare și pointeri către nodurile copil[cite: 104]. Navigarea în arbore pentru a determina calea relativă implică urcarea de la directorul sursă către directorul comun (reprezentată prin `../`) și apoi coborârea către directorul destinație (specificând numele directoarelor intermediare)[cite: 105]. Se folosesc funcționalități din `dirent.h` și `sys/stat.h` pentru a obține structura de directoare[cite: 101], iar funcțiile din Task 2 pot fi utilizate pentru stocarea temporară a acestei structuri într-o stivă[cite: 102].

* **Pentru ce este utilă?**
    Calcularea căilor relative este o funcționalitate importantă în multe aplicații, inclusiv scripturi de automatizare, sisteme de build și unelte de versionare. Permite specificarea locațiilor într-un mod mai portabil și mai flexibil decât utilizarea exclusivă a căilor absolute, mai ales atunci când structura proiectului poate fi mutată sau partajată. Acest task dezvoltă abilități în reprezentarea ierarhiilor (cum ar fi sistemele de fișiere) sub formă de arbori și în implementarea algoritmilor de parcurgere a acestora pentru a extrage informații utile.

---

### 4. Task 4: `util_diff` (Comparare Fișiere)

* **Ce face funcția?**
    `util_diff` implementează o funcționalitate similară comenzii `diff` din Linux, comparând conținutul a două fișiere text, linie cu linie[cite: 109, 111]. Scopul este de a calcula numărul minim de operații de editare (ștergere, inserare, înlocuire) necesare pentru a transforma fiecare linie din primul fișier în linia corespunzătoare din cel de-al doilea fișier[cite: 146]. Implementarea este inspirată de algoritmul lui Eugene Myers pentru problema "shortest edit script" [cite: 115] și utilizează o adaptare a algoritmului lui Dijkstra pe un graf (matrice) al distanțelor de editare. Operațiile de ștergere (`Delete`) și inserare (`Insert`) au cost 1, înlocuirea (`Replace`) are cost 2 (considerată o ștergere urmată de o inserare), iar potrivirile (`Match`) au cost 0[cite: 141]. Rezultatul afișat pentru fiecare pereche de linii este numărul de operații de ștergere (D), inserare (I) și înlocuire (R)[cite: 150]. Dacă fișierele au un număr diferit de linii, comparația se face considerând numărul maxim de linii dintre cele două[cite: 151].

* **Pentru ce este utilă?**
    Acest utilitar este extrem de util pentru a identifica diferențele dintre versiuni de fișiere text, fiind esențial în dezvoltarea software (de ex., compararea output-ului programului cu un output de referință, analiza modificărilor în codul sursă) și în alte sarcini care implică gestionarea documentelor. Implementarea sa oferă o perspectivă practică asupra algoritmilor de programare dinamică și de căutare în grafuri, aplicate la procesarea textului și la probleme de optimizare.

---

## Structura Proiectului

*(Aici poți adăuga o scurtă descriere despre cum sunt organizate fișierele în repository-ul tău. De exemplu:)*
* Fiecare task principal (`parser`, `util_ls`, `util_cd`, `util_diff`) este implementat în fișiere `.c` și `.h` dedicate.
* Există un fișier `main.c` (sau fișiere `main_taskX.c`) care orchestrează apelarea funcționalităților.
* Makefile-ul gestionează compilarea întregului proiect sau a modulelor individuale.
* ... (alte detalii relevante despre structura ta)

---

## Pentru Contribuitori (Cei care vor să modifice codul)

Dacă doriți să contribuiți la acest proiect sau să modificați codul, iată câțiva pași și sugestii:

1.  **Prerechizite**:
    * Asigurați-vă că aveți un compilator C instalat (de preferință GCC, deoarece este utilizat în exemple).
    * Un sistem de operare compatibil POSIX este recomandat pentru a lucra cu bibliotecile `dirent.h` și `sys/stat.h`.

2.  **Clonarea Repository-ului**:
    ```bash
    git clone [https://github.com/Mihaifv1/proiect_terminal.git](https://github.com/Mihaifv1/proiect_terminal.git)
    cd proiect_terminal
    ```

3.  **Înțelegerea Structurii**:
    * Analizați secțiunea "Structura Proiectului" de mai sus (pe care o veți completa).
    * Examinați fișierele sursă și antetele pentru a înțelege cum sunt împărțite modulele și cum interacționează.
    * Documentul PDF (`PA_proiect_utilitare (2).pdf`) conține descrierea detaliată a cerințelor pentru fiecare task.

4.  **Modificarea Codului**:
    * Alegeți task-ul sau funcționalitatea pe care doriți să o îmbunătățiți sau corectați.
    * Implementați modificările, respectând stilul de cod existent și adăugând comentarii relevante unde este necesar.

5.  **Compilarea și Testarea Modificărilor**:
    * Consultați secțiunea "Compilare și Rulare (Pas cu Pas)" de mai jos pentru a compila modulul la care ați lucrat.
    * Testați modificările cu diverse inputuri pentru a vă asigura că funcționează corect și nu introduce regresii. Documentul PDF oferă exemple de input/output pentru testare[cite: 51, 57, 61, 88, 106, 148, 152].

---

## Compilare și Rulare (Pas cu Pas)

Mai jos sunt instrucțiuni generale pentru compilarea și rularea fiecărui utilitar. **Va trebui să adaptezi denumirile fișierelor sursă și ale executabilelor conform structurii reale din repository-ul tău.**

### Task 1: Parser de Argumente
Acest task este mai mult o bibliotecă/modul utilizat de celelalte utilitare. Dacă aveți un program de test dedicat pentru parser:

* **Fișiere relevante**: `[... exemplu: argument_parser.c, test_parser.c ...]`
* **Comanda de compilare**:
    ```bash
    gcc -o test_parser [... argument_parser.c, test_parser.c ...] -Wall -Wextra -std=c99
    ```
* **Exemplu de rulare** (presupunând că `test_parser` demonstrează parsarea):
    ```bash
    ./test_parser --optiune=valoare -f argument1 --flag2 date.txt
    ```

### Task 2: `util_ls`
* **Nume executabil sugerat**: `util_ls`
* **Fișiere relevante**: `[... exemplu: util_ls.c, main_ls.c, (posibil și argument_parser.c) ...]`
* **Comanda de compilare**:
    ```bash
    gcc -o util_ls [... util_ls.c, main_ls.c ...] -Wall -Wextra -std=c99
    ```
* **Exemplu de rulare**:
    ```bash
    ./util_ls /cale/absoluta/catre/un/director
    ./util_ls /alt/director -r -h 
    ./util_ls /director -d
    ```

### Task 3: `util_cd`
* **Nume executabil sugerat**: `util_cd`
* **Fișiere relevante**: `[... exemplu: util_cd.c, main_cd.c, (posibil și argument_parser.c, util_ls_componente.c) ...]`
* **Comanda de compilare**:
    ```bash
    gcc -o util_cd [... util_cd.c, main_cd.c ...] -Wall -Wextra -std=c99
    ```
* **Exemplu de rulare**:
    ```bash
    ./util_cd /cale/absoluta/director_sursa /cale/absoluta/director_destinatie
    ```

### Task 4: `util_diff`
* **Nume executabil sugerat**: `util_diff`
* **Fișiere relevante**: `[... exemplu: util_diff.c, main_diff.c, (posibil și argument_parser.c) ...]`
* **Comanda de compilare**:
    ```bash
    gcc -o util_diff [... util_diff.c, main_diff.c ...] -Wall -Wextra -std=c99
    ```
* **Exemplu de rulare**:
    ```bash
    ./util_diff fisier1.txt fisier2.txt
    ```
    (Conform PDF, `util_diff` ar trebui să scrie output-ul într-un fișier, deci ar putea avea și argumente pentru fișierele de output, gestionate de parserul din Task 1, ex. `--out fisier_output.txt`[cite: 56], deși Task 4 specific nu menționează `--out` pentru `util_diff` în sine, ci pentru programul de testare al Task-ului 1 [cite: 58]).

---

## Autor

* **Mihaifv1** (Mihai F. V.)
