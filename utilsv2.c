#include "utilsv2.h"

////////////////////////////////////IMPLEMENTATION FONCTIONS////////////////////////////////////////////


int pioche(int min, int max) //pioche un chiffre entre un minimum et maximum. création d'une valeur quasi-aléatoire (dépend du temps)
{
  int a;
  a = ((rand()%max)+min);
  return a;
}

int probabilite(int PROBA1, int PROBA2, int PROBA3)
{
    int nbrAlea = pioche(1,100);//nbrAlea pioché entre 1 et PROBA1 + PROBA2+ PROBA3
    if (nbrAlea <= PROBA1)
    {
        return 1;
    }
    else if (nbrAlea <= PROBA1+PROBA2)
    {
        return 2;
    }
    else if (nbrAlea <= PROBA1+PROBA2+PROBA3)
    {
        return 3;
    }
    else
        return 4;
}


/* msleep(): Sleep for the requested number of milliseconds. */
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}
