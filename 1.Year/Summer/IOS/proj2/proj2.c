/*********************************
**      IOS - 2. projekt        **
**    Santa Claus problem       **
** Autor:       Adrián Horváth  **
** Login:        xhorva14       **      
** Datum:       02. 05. 2021    **
** Škola:       VUT FIT Brno    ** 
*********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>

void cleanup();

typedef struct {
    sem_t workshop;                 // elf caka na otvorenie semaforu
    sem_t santa_going_sleep;        // santa ide spat ked pomoze 3 elfom
    sem_t santa_wake_up;            // santa sa zobudi
    sem_t reindeers_hitching;       // zapriahanie sobov 
    sem_t reindeers_all_hitched;    // vianoce mozu zacat
    sem_t queue;                    // elfovia cakaju pokial pojdu na dovolenku pred dielnou
    sem_t print;
} shm_sem_t;

typedef struct{
    int elves_need_count;           // pocet elfov ktori potrebuju pomoc
    int elves_get_count;            // pocet elfov ktori dostali pomoc
    int elves_in_queue;             //pocet elfov pred dielnou
    int reindeer_return_count;      // pocet sobov vratenych z dovolenky
    int reindeer_chitched;          // pocet zapriahnutych sobov
    bool reindeers_all_returned;    //true ak su vsetci sobi doma
    bool workshop_closed;           //true dielna je zatvorena
    unsigned int total_queue;       //celkovy pocet elfov ktory cakali v rade
    int allow_numbers;              //cisla elfov ktorim santa pomoze

    int line_count;

    FILE *file;

} shm_var_t;




shm_sem_t *sem;
shm_var_t *var;

void print_error(int ERROR_ID){

switch (ERROR_ID)
{
case 0:
    fprintf(stderr, "Too few arguments");
    exit(1);
case 1:
    fprintf(stderr,"Invalid input arguments");
    exit(1);
case 2:
    fprintf(stderr, "Error while initiazing");
    cleanup();
    exit(1);
case 3:
    fprintf(stderr, "Error while releasing");
    exit(1);
default:
    break;
}

}

void initialization()
{
    //inicializacia zdielanej pamate
    sem = mmap(NULL, sizeof(shm_sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    var = mmap(NULL, sizeof(shm_var_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    //inicializacia semaforov
    if (sem_init(&sem->workshop, 1, 0) == -1)
    {
        print_error(2);
        exit(1);
    }

    if (sem_init(&sem->santa_going_sleep, 1, 0) == -1)
    {
        print_error(2);
        exit(1);
    }

    if (sem_init(&sem->santa_wake_up, 1, 0) == -1)
    {
        print_error(2);
        exit(1);
    }

    if (sem_init(&sem->reindeers_hitching, 1, 0) == -1)
    {
        print_error(2);
        exit(1);
    }

    if (sem_init(&sem->reindeers_all_hitched, 1, 0) == -1)
    {
        print_error(2);
        exit(1);
    }
    if (sem_init(&sem->queue, 1, 0) == -1)
    {
        print_error(2);
        exit(1);
    }
    if (sem_init(&sem->print, 1, 1) == -1)
    {
        print_error(2);
        exit(1);
    }

    //nastavenie premennych
    var->elves_need_count = 0;
    var->elves_get_count = 0;
    var->reindeer_return_count = 0;
    var->reindeer_chitched = 0;
    var->reindeers_all_returned = false;
    var->workshop_closed = false;
    var->total_queue = 0;
    var->allow_numbers = 0;
    var->line_count = 1;

}


void cleanup()
{
    if (sem_destroy(&sem->workshop) == -1)
    {
        print_error(3);
        exit(1);
    }

    if (sem_destroy(&sem->santa_going_sleep) == -1)
    {
        print_error(3);
        exit(1);
    }

    if (sem_destroy(&sem->santa_wake_up) == -1)
    {
        print_error(3);
        exit(1);
    }

    if (sem_destroy(&sem->reindeers_hitching) == -1)
    {
        print_error(3);
        exit(1);
    }

    if (sem_destroy(&sem->reindeers_all_hitched) == -1)
    {
        print_error(3);
        exit(1);
    }

    munmap(sem, sizeof(sem));
    munmap(var, sizeof(var));
}

int random_number(int limit)
{
    struct timeval t1;
    gettimeofday(&t1, NULL);
    srand(t1.tv_usec * t1.tv_sec);
    return (rand() % (limit + 1));
}

void p_SANTA()
{
    sem_wait(&sem->print);
    fprintf(var->file, "%d: Santa: going to sleep\n", var->line_count++);
    fflush(var->file);
    sem_post(&sem->print);
    
    
    sem_wait(&sem->santa_wake_up);                  //cakanie na zobudenie

    // kontrola ci ho zobudil elf alebo sob
    if (var->reindeers_all_returned)                //zobudenie sobom
    {
        sem_wait(&sem->print);
        fprintf(var->file, "%d: Santa: closing workshop\n", var->line_count++);
        fflush(var->file);
        sem_post(&sem->print);

        var->workshop_closed = true;                //zatvorenie dielne
        var->allow_numbers +=999;                   //otvori dielnu pre vsetkych elfov aby mohli ist na dovolenku
        sem_post(&sem->workshop);
        sem_post(&sem->reindeers_hitching);
        sem_wait(&sem->reindeers_all_hitched);      //cakanie pokial nezapriahne vsetkych sobov

        sem_wait(&sem->print);
        fprintf(var->file, "%d: Santa: Christmas started\n", var->line_count++);
        fflush(var->file);
        sem_post(&sem->print);

        exit(0);
    }
    else                                            //zobudenie elfom
    {
        var->allow_numbers +=3;
        var->elves_get_count = 0;                   //vynuluje pocitadlo kolko elfom pomohol

        sem_wait(&sem->print);
        fprintf(var->file, "%d: Santa: helping elves\n", var->line_count++);
        fflush(var->file);
        sem_post(&sem->print);

        sem_post(&sem->workshop);                   //pustenie prave 3 elfov dnu
        sem_post(&sem->workshop);
        sem_post(&sem->workshop);

        var->elves_need_count = 0;
        sem_wait(&sem->santa_going_sleep);          //cakanie pokial nepomoze vsetkym elfom
        p_SANTA();                            //ide znova spat
    }
}


void p_ELF(int elfID, int TE)
{   


    if (TE > 0)
    {
        int t = random_number(TE);
        usleep(t * 1000);                                  //elf pracuje
    }
    else
    {
        usleep(TE);
    }
                                              
    sem_wait(&sem->print);
    fprintf(var->file, "%d: Elf %d: need help\n", var->line_count++, elfID);
    fflush(var->file);
    sem_post(&sem->print);
    

    var->elves_need_count++;                        //pocet elfov ktori potrebuju pomoc
    var->total_queue++;
    int my_position = var->total_queue;


    if (var->elves_need_count == 3)                 //ak su v rade 3 zobudia santu
        sem_post(&sem->santa_wake_up);

    if ((var->workshop_closed) && (var->elves_in_queue != 0))   //ak je dielna zatvorena najprv 
    {                                                           // idu na dovolenku elfovia co cakaju
        sem_wait(&sem->queue);
        sem_post(&sem->queue);
    }
    else
        var->elves_in_queue++;                      //aktualny pocet elfov v rade
    

    for (;;)
    {
        sem_wait(&sem->workshop);                   //cakaju na zobudenie santu 
        if (my_position <= var->allow_numbers)      //kontroluju ci su na rade
            break;
        sem_post(&sem->workshop); 
    }
    
    var->elves_in_queue--;                          

    if (var->workshop_closed)                       // ak je dielna zatvorena idu na dovolenku
    {

        if (var->elves_in_queue == 0)               //ak uz nikto necaka idu na dovolenku ustatni
            sem_post(&sem->queue);
        

        sem_wait(&sem->print);
        fprintf(var->file, "%d: Elf %d: taking holidays\n", var->line_count++, elfID);
        fflush(var->file);
        sem_post(&sem->print);

        sem_post(&sem->workshop);
        exit(0);
    }
    else                                            // elfovia dostali pomoc od santu
    {                                            
        sem_wait(&sem->print);
        fprintf(var->file, "%d: Elf %d: get help\n", var->line_count++, elfID);
        fflush(var->file);
        sem_post(&sem->print);
    }

    var->elves_get_count += 1;                      //zvacsi pocet elfov ktori dostali pomoc

    if (var->elves_get_count == 3)                  // ak dostanu pomoc 3 elfovia santa ide spat
        sem_post(&sem->santa_going_sleep);

    p_ELF(elfID, TE);
}

void p_REINDEER(int rdID, int NR, int TR)
{
    if (TR > 0)
    {
        int t = random_number(TR);
        if (t < TR/2)
            t = t + TR/2;

        usleep(t * 1000);
    }
    else
    {
        usleep(TR);
    }                                               //elf je na dovolenke

    sem_wait(&sem->print);
    fprintf(var->file, "%d: RD %d: return home\n", var->line_count++, rdID);
    fflush(var->file);
    sem_post(&sem->print);


    var->reindeer_return_count++;                   //zvacsi pocet sobov ktori sa vratili domov

    if (var->reindeer_return_count == NR)           //ak su vsetci doma zobudia santu
    {
        var->reindeers_all_returned = true;
        sem_post(&sem->santa_wake_up);
    }

    sem_wait(&sem->reindeers_hitching);             //caka pokial ho santa zapriahne


    sem_wait(&sem->print);
    fprintf(var->file, "%d: RD %d: get hitched\n", var->line_count++, rdID);
    fflush(var->file);
    sem_post(&sem->print);


    sem_post(&sem->reindeers_hitching);
    var->reindeer_chitched++;                       //zvacsi pocet zapriahnutych sobov
    

    if (var->reindeer_chitched == NR)               //vsetci sobi zapriahnuti
        sem_post(&sem->reindeers_all_hitched);
    

    exit(0);
}



void m_ELVES(int NE, int TE)                         // generovanie elfov
{
    for (int i=0;i<NE;i++)
    {
        int ID = i+1;
        pid_t elfID = fork();
        if (elfID == 0)
        {
            sem_wait(&sem->print);
            fprintf(var->file, "%d: Elf %d: started\n", var->line_count++, ID);
            fflush(var->file);
            sem_post(&sem->print);

            p_ELF(ID, TE);
        }

    }
}


void m_REINDEERS(int NR, int TR)                 // generovanie sobov
{
    for (int i=0;i<NR;i++)
    {
        int ID = i+1;
        pid_t rdID = fork();
        if (rdID == 0)
        {
            sem_wait(&sem->print);
            fprintf(var->file, "%d: RD %d: rstarted\n", var->line_count++, ID);
            fflush(var->file);
            sem_post(&sem->print);

            p_REINDEER(ID, NR, TR);
        }
    }
}

bool arg_check(int argc, char *argv[])
{
    if (argc != 5)
    {
        print_error(0);
        return false;
    }
    
    int NE = atoi(argv[1]);
    int NR = atoi(argv[2]);
    int TE = atoi(argv[3]);
    int TR = atoi(argv[4]);

    if ((NE <= 0) ||(NE >= 1000))
    {
       print_error(1);
       return false; 
    }
    if ((NR <= 0) ||(NR >= 20))
    {
       print_error(1);
       return false; 
    }
    if ((TE < 0) ||(TE > 1000))
    {
       print_error(1);
       return false; 
    }
    if ((TR < 0) ||(TR > 1000))
    {
       print_error(1);
       return false; 
    }
    return true;
}

int main(int argc, char *argv[])
{
    

    if(arg_check(argc, argv) == false)
    {
        exit(1);
        return 0;
    }

    int NE = atoi(argv[1]);
    int NR = atoi(argv[2]);
    int TE = atoi(argv[3]);
    int TR = atoi(argv[4]);

    initialization();

    var->file = fopen("proj2.out", "w");

    pid_t santa = fork();
    if (santa == 0)
    {
        p_SANTA();
    }
    else
    {
        pid_t gen = fork();
        if (gen == 0)
        {
            m_ELVES(NE, TE);
        }
        else
        {
            m_REINDEERS(NR, TR);
        }
    }

    cleanup();
    exit(0);
    return 0;
}