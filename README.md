# Proiect Utilitare Terminal (Proiectarea Algoritmilor)

Acest proiect conține o suită de utilitare pentru terminal dezvoltate în C, axate pe manipularea fișierelor, directoarelor și procesarea argumentelor din linia de comandă. Proiectul explorează utilizarea diverselor structuri de date precum stive, arbori și grafuri pentru a implementa funcționalități similare comenzilor standard din Linux (de ex., `ls`, `diff`, `cd`).

## Utilitare Dezvoltate și Funcționalitatea Lor

Proiectul este împărțit în patru task-uri principale, fiecare având ca rezultat un utilitar sau o componentă software distinctă:

### 1. Task 1: Parser de Argumente în Linia de Comandă

* **Ce face funcția?**
    Această componentă este responsabilă pentru preluarea și interpretarea argumentelor, opțiunilor și flag-urilor transmise unui program atunci când este apelat din terminal. [cite: 5] Programul parsează șirul de intrare (așa cum este primit de `main(int argc, char** argv)`) și stochează detaliile fiecărui parametru (tip, nume/identificator, valoare, format lung/scurt) în structuri de date interne definite. [cite: 26, 39] Se gestionează diferitele formate pentru opțiuni (lungi cu `--`, scurte cu `-`) și diferiți delimitatori (`=` sau spațiu) pentru valori. [cite: 17, 20, 21] De asemenea, se iau în considerare flag-urile, care acționează ca switch-uri on/off și pot avea și ele formate lungi sau scurte. [cite: 24]

* **Pentru ce este utilă?**
    Un parser de argumente robust este fundamental pentru orice program care necesită o configurare flexibilă din linia de comandă. Permite programelor să primească inputuri variate și să își modifice comportamentul în funcție de acestea, fără a necesita recompilare sau modificări în codul sursă. Abilitatea de a defini argumente, opțiuni cu valori și flag-uri oferă o interfață standard și puternică pentru utilizatori, similară cu cea a uneltelor consacrate din sistemele de operare. Această componentă este esențială pentru a construi utilitarele din task-urile următoare, permițându-le să fie controlate prin parametri specifici.

---

### 2. Task 2: `util_ls` (Listare Conținut Director)

* **Ce face funcția?**
    `util_ls` este o versiune simplificată a comenzii `ls` din Linux. [cite: 66] Primește ca intrare calea absolută către un director și, pe baza opțiunilor specificate, afișează conținutul acestuia. [cite: 67, 68] Informațiile despre subdirectoare și fișiere (nume, cale, adâncime, dimensiune pentru fișiere) sunt stocate intern folosind o structură de date de tip stivă. [cite: 67, 79] Funcționalitățile includ:
    * Afișarea numelor subdirectoarelor și fișierelor din directorul curent, sortate alfabetic. [cite: 68] Se afișează doar elementele din directorul cerut, nu și conținutul acestora. [cite: 69]
    * Afișarea doar a numelor subdirectoarelor, sortate alfabetic (similar cu `ls --directory` sau `ls -d`). [cite: 70, 73]
    * Afișarea dimensiunii fișierelor într-un format "human-readable" (K, M, G), pe lângă numele acestora (similar cu `ls -h` sau `ls --human-readable`). [cite: 71, 73]
    * Afișarea recursivă a conținutului subdirectoarelor (similar cu `ls -R` sau `ls --recursive`). [cite: 72, 73]
    Indentarea este folosită pentru a reflecta adâncimea structurii de directoare în cazul afișării recursive. [cite: 94] Pentru a obține informații despre tipul elementelor (fișier/director) și dimensiunea fișierelor, se utilizează bibliotecile `dirent.h` și `sys/stat.h`. [cite: 74, 77]

* **Pentru ce este utilă?**
    Acest utilitar este util pentru navigarea și inspectarea sistemului de fișiere dintr-un program C. Permite utilizatorilor să obțină rapid informații despre structura și conținutul unui director, cu opțiuni de personalizare a afișajului. Implementarea sa necesită înțelegerea modului de interacțiune cu sistemul de fișiere la nivel de program și gestionarea structurilor de date pentru a stoca și sorta eficient aceste informații. Este un exercițiu practic pentru lucrul cu operații I/O pe fișiere și directoare și pentru sortarea datelor.

---

### 3. Task 3: `util_cd` (Calculare Cale Relativă)

* **Ce face funcția?**
    `util_cd` are scopul de a calcula și afișa calea relativă între două directoare specificate prin căi absolute (un director sursă și un director destinație). [cite: 99] Pentru a realiza acest lucru, se construiește un arbore general (ne-binar) care reprezintă structura de directoare pornind de la ultimul director comun al celor două căi. [cite: 99, 100, 103] Fiecare nod din arbore conține numele directorului, numărul de subdirectoare și pointeri către nodurile copil. [cite: 104] Navigarea în arbore pentru a determina calea relativă implică urcarea de la directorul sursă către directorul comun (reprezentată prin `../`) și apoi coborârea către directorul destinație (specificând numele directoarelor intermediare). [cite: 96, 105] Se folosesc funcționalități din `dirent.h` și `sys/stat.h` pentru a obține structura de directoare[cite: 101], iar funcțiile din Task 2 pot fi utilizate pentru stocarea temporară a acestei structuri într-o stivă. [cite: 102]

* **Pentru ce este utilă?**
    Calcularea căilor relative este o funcționalitate importantă în multe aplicații, inclusiv scripturi de automatizare, sisteme de build și unelte de versionare. Permite specificarea locațiilor într-un mod mai portabil și mai flexibil decât utilizarea exclusivă a căilor absolute, mai ales atunci când structura proiectului poate fi mutată sau partajată. Acest task dezvoltă abilități în reprezentarea ierarhiilor (cum ar fi sistemele de fișiere) sub formă de arbori și în implementarea algoritmilor de parcurgere a acestora pentru a extrage informații utile.

---

### 4. Task 4: `util_diff` (Comparare Fișiere)

* **Ce face funcția?**
    `util_diff` implementează o funcționalitate similară comenzii `diff` din Linux, comparând conținutul a două fișiere text, linie cu linie. [cite: 111, 146] Scopul este de a calcula numărul minim de operații de editare (ștergere, inserare, înlocuire) necesare pentru a transforma fiecare linie din primul fișier în linia corespunzătoare din cel de-al doilea fișier. [cite: 113, 146] Implementarea este inspirată de algoritmul lui Eugene Myers pentru problema "shortest edit script" [cite: 115] și utilizează o adaptare a algoritmului lui Dijkstra pe un graf (matrice) al distanțelor de editare. [cite: 118, 141] Operațiile de ștergere (`Delete`) și inserare (`Insert`) au cost 1, înlocuirea (`Replace`) are cost 2 (considerată o ștergere urmată de o inserare), iar potrivirile (`Match`) au cost 0. [cite: 141] Rezultatul afișat pentru fiecare pereche de linii este numărul de operații de ștergere (D), inserare (I) și înlocuire (R). [cite: 150] Dacă fișierele au un număr diferit de linii, comparația se face considerând numărul maxim de linii dintre cele două. [cite: 151]

* **Pentru ce este utilă?**
    Acest utilitar este extrem de util pentru a identifica diferențele dintre versiuni de fișiere text, fiind esențial în dezvoltarea software (de ex., compararea output-ului programului cu un output de referință[cite: 110], analiza modificărilor în codul sursă) și în alte sarcini care implică gestionarea documentelor. Implementarea sa oferă o perspectivă practică asupra algoritmilor de programare dinamică și de căutare în grafuri, aplicate la procesarea textului și la probleme de optimizare.

---

## Compilare (Exemplu General)

Pentru a compila utilitarele din acest proiect, veți avea nevoie de un compilator C (de exemplu, GCC). Navigați în directorul corespunzător task-ului și utilizați o comandă similară cu:

```bash
gcc -o nume_utilitar fisier_sursa1.c fisier_sursa2.c ... -Wall -Wextra -std=c99
