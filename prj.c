#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
extern char **environ; // Corectăm scrierea lui 'envrion' în 'environ'

//fucntia cleanenv sterge varabilele de mediu
//declarun pointer pentru sirul envron pointerul env si cat timp sunt elementein sir noi le vom elimina


// Funcția CLEANENV șterge toate variabilele de mediu.

void CLEANENV() {
    //declaram un pointer prin care vom parcurge elementle di environ
    char **env = environ;
    //setamfiecare env cu NULL pentru a sterge varabilele de mediu 
    while (*env) {

        *env = NULL;
        //trecm la urmatorul mediu 
        env++;
    }
}


/*
Pentru realizarea aceste funtii m-am folosit de modul in care e a a fost implementata in livraria standard stdlib.h
link:https://github.com/lattera/glibc/blob/master/stdlib/getenv.c
functia getenv funtioneaza asa in spate:
fucta getenv retrunneaza valoarea variabilei de mediu daca ceasta exista in caz contrar eturneaza NULL
fucntia stocheaza lungimea sirului intr-o variabila
foloseste varibila __envrion care este un pointer la un array de sruri de caractere care reprezinta
variaiblele de mediu
verifica daca __envrion este NULL sau este un sir gol functai se incheie si returneaza NULL
functia e facuta pentru tratarea a mi multocazuri cu o singura litera, dou litere sau ai multe litere.
Verificam primele doua caractere siapoi compara restul folosind strcmp daca gasim vreo potrivire o returneaza peceasta


Prin urmare voi incerca sa impleemntz acelasi comportamenfara
 */

// Funcția GETENV returnează valoarea variabilei de mediu dacă aceasta există, altfel returnează NULL.
const char *GETENV(const char *name) {
    //verificam daca name este egal cu NULL si in cazul in cre este NULL returnam NULL
    if (name == NULL)
        return NULL;
    //verificam daca exita un sir gol in acel caz returnam NULL
    if (*name == '\0')
    
        return NULL;
    //salvam lungimea variabilepe care o cautam 
    size_t len = strlen(name);
    //interam prin arrayul de mediu environ 
    for (char **env = environ; *env != NULL; env++) {
        //verificam daca variabila pe care o cauta (incepe cu name si  urmat de = )(dupa && verificam daca variabila de pe pozitia len este caracterul =)
        if (strncmp(*env, name, len) == 0 && (*env)[len] == '=') {
            //returnam valoarea care se afla dupa caracterul =
            return *env + len + 1;
        }
    }
    return NULL;
}

/*
setenv()
referinta din bibloteca:https://github.com/lattera/glibc/blob/master/stdlib/setenv.c
*/





//fnctia SETENV  permite setarea unei variabile de mediu cu o anumita valoare
//primeste ca si parametrii un pointer catre un si r de aractere, un pointer catre valoarea pe caere vrem sa o atibuim 
//overwrite rteprezinta daca variabila trebuie suprascrisa sau nu daca e setata pe 1 o suprascriem daca e cu 0 nu o suprascriem daca exita deja o variabila de mediu cu acelasi nume 

int SETENV(const char *name, const char *value, int overwrite) {
    //veririfcam daca numele variabilei de mediu este NULL sau daca sirul esteol  sau daca contine = 
    if (name == NULL || *name == '\0' || strchr(name, '=') != NULL) {
        //in cazul in care cocnditia este adevarata errno este setat pe EINVALcare este eroare de argument nevalid returam -1
        errno = EINVAL;
        return -1;
    }
    //verificam daca variabila de mediu exista deja 
    if (!overwrite && GETENV(name) != NULL) {
        return 0;
    }

    // Creem o copie loca a variabilei de mediu 
    char **local_environ = NULL;
    //counter pentru varabilele de sistem 
    size_t env_count = 0;
    //numaram varabilele de mediu existente in vectorul environ
    for(char **env = environ; *env != NULL; ++env) {
        ++env_count;
    }


    //alocam memorie pentru vectorul creat local care va avea marimea de env_count+2 ca sa ne permita adaugarea unei noi variabile de mediu  si unui terminator NULL
    local_environ = malloc((env_count + 2) * sizeof(char *));
    //verificam daca vectoru local este NULL in cazul in care e NULL returam -1
    if (local_environ == NULL) {
        return -1;
    }



    //iteram prin toate variabilele de mediu din environ 
    for (size_t i = 0; i < env_count; ++i) {
        //facem o copie a variabilei de mediu de pe pozitia i cu strdup alocam memorie pentru un nou sir de caractere 
        local_environ[i] = strdup(environ[i]);
        //verificam daca daca functai strdup a reusit sau a esuat in cazul in care a esuat iteram pana in acelpunct si elibereaza memoria alocata pentru fiecare si de caractere pana in acel moment de asta mergeme la i la j 
        if (local_environ[i] == NULL) {
            for (size_t j = 0; j < i; ++j) {
                free(local_environ[j]);
            }
            //eliberam memoria pentru vectorul local_environ
            free(local_environ);
            //returnam -1 care indica ca a esuat 
            return -1;
        }
    }

    
    //lungimea noii variabiler de mediu si lungimea valorii
    size_t name_len = strlen(name);
    size_t value_len = strlen(value);
    //alocam memorie pentru un nou sir e caractere  care va fi lungimea numelui+lungimea valori +2 deparece +1 pentru = si +1 pentru \0 (terminatorul )
    char *new_env_var = malloc(name_len + value_len + 2); 
    //verifiam daca alocarea memoriei a reusit
    if (new_env_var == NULL) {
        //daca nu a reusit eliberam memoria 
        for (size_t i = 0; i < env_count; ++i) {
            free(local_environ[i]);
        }
        //elibra memoria pentru totate
        free(local_environ);
        return -1;
    }
    //daca alocarea a avut succes constrim varaibila in forma standard NUME=VALOARE
    sprintf(new_env_var, "%s=%s", name, value);

    // Căutăm dacă variabila de mediu există deja
    //variabla found rprezinta pentru a marca daca variabilade mediu exista deja sau nu 
    int found = 0;
    //iteram prin toatevariavilele de mediudin vectorul local 
    for (size_t i = 0; i < env_count; ++i) {
        //verificam daca variabila de mediu ncepe cu numele cautat s este urmat de = 
        if (!strncmp(local_environ[i], name, name_len) && local_environ[i][name_len] == '=') {
            if (overwrite) {
                //daca variabila de mediu este gasita si trbuie suprascria eliberam meoria de pe pozitia i 
                //pe pozitia i punem noua variabila creata 
                //si punem found pe 1
                free(local_environ[i]);
                local_environ[i] = new_env_var;
                found = 1;
            } else {
                //daca variabila de mediu nu trebuie suprascrisa eliberam memoria alocata pentru new_env_var
                free(new_env_var);
                //sio setam found pe 1
                found = 1;
            }
            //daca am gasit variabila de mediu dorita puem break 
            break;
        }
    }
    //daca nu am gasit variabia de mediu dorita in vecorul local adaugam variabila noua creta

    if (!found) {
        //adaugam in vectorul local variabila noua
        local_environ[env_count] = new_env_var;

        //iitializam urmatoarea poziie cu NULL
        local_environ[env_count + 1] = NULL;
    }
    //actualiam modificarile ca sa se regasesca si in environ 
    environ = local_environ;
    //returnam 0
    return 0;
}




/*
referinta din bibloteca:https://github.com/lattera/glibc/blob/master/stdlib/putenv.c
functia putenv care adauga o variabila de mediu sau modifica valoarea unei varabile de mediuexistente.
aceasta functie primeste ca si parametru un pointer la un sir de caractere si returneaza un int
in aceasta functiecautam in sir cu strchr caracterul "=" si returneaza un pointer pentru acesta
verifica daca acel caracter a fost gasit
face niste operatii pentru gestionarea memoriei si dupa apeleaza o functie pentru a adauga variabilde mediu
dupa elibereaza memoriasi returneaza rezultatul
in cazul in care nu a gasit cracterul "=" da return 0
*/




// Funcția PUTENV adaugă o variabilă de mediu sau modifică valoarea unei variabile de mediu existente.
int PUTENV(char *string) {
    //cautam pima aparitie al = daca nu o gasim inseamna ca sirrul nu e intr-un format corect
    char *name_end = strchr(string, '=');
    //verificam daca nu e informat corect returnam -1 
    if (name_end == NULL) {
        return -1;
    }
    //calculam lungimea numelui variabilei de mediu pana la caracterul =  ex:DADA=X+2 in name_len vom avea 4
    size_t name_len = name_end - string;
    ////alocam memorie pentru un nou sir de caractere 
    char *name = (char *)malloc(name_len + 1);
    //verificam daca alocarea memeoriei pentru name a avut succes  in cazul in care nu returnam -1
    if (name == NULL) {
        return -1;
    }
    //copiem numele variabilei de medoiu din string care il primeste ca si input se copiaza in name 
    strncpy(name, string, name_len);
    //ne asiguram ca name se termina adaugam terminatorul la finalul acestuia 
    name[name_len] = '\0';
    //apelam functia creata mai sus SETENV
    int result = SETENV(name, name_end + 1, 1);
    //eliberam memoria
    free(name);
    //returnam valoarea
    return result;
}


int main() {
    // adaugam varaibile d mediu 

    PUTENV("TEST1=VAl1");
    PUTENV("TEST2=VAL2");
    PUTENV("TEST3=VAL3");

    //Verificam variabilele deediu adaugate
    printf("TEST1 = %s\n", GETENV("TEST1"));
    printf("TEST2 = %s\n", GETENV("TEST2"));
    printf("TEST3 = %s\n", GETENV("TEST3"));

    //stergem variabilelde mediu 
    CLEANENV();

    // Verificam dupa stergere
    printf("DUPA CLEANENV:\n");
    printf("TEST1 = %s\n", GETENV("TEST1"));
    printf("TEST2 = %s\n", GETENV("TEST2"));
    printf("TEST3 = %s\n", GETENV("TEST3"));

    return 0;
}



//manual de utulizare 
/*
gcc -o app prj.c
./app
output:
TEST1 = VAl1
TEST2 = VAL2
TEST3 = VAL3
DUPA CLEANENV:
TEST1 = (null)
TEST2 = (null)
TEST3 = (null)


*/

/*
PROBLEME DE CARE AM DAT 

ceea mai mare provocare a fost ca nu stiam initla cum ar trebui sa implementez aceste functii getenv,setenv,putenv,cleanenv
am avut foarte multe core dumped din cauza ca mi-a luat ceva timp sa imi dau seama ca nu am alocat bine memoria 

am incercat pe cat posibil sa ma folosesc de modul in care ele sunt implementate in spate pentru pe net nu am gasit NIMIC folositor apsolut nimic ca norocu' am gasit un repo unde imi aratau cum functioneazfunctiile in spate 



*/