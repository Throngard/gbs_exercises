#include "list.h"
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/******************************************************
* void alg_insert(list_t *,thread_t *);(RR,PRR,SRTN)  *
******************************************************/

void rr_insert(list_t *,thread_t*);
void prr_insert(list_t*,thread_t*);
void prr_sort(list_t*);
void srtn_insert(list_t*,thread_t*);
void t_insert(list_t*,thread_t*);


/*******************************************************
*Returns the data of the first list element and frees  *
* the element pointer                                  *
*******************************************************/
thread_t *queue_pop(list_t*);

/*******************************************************
*Returns the data of the first element. List stays     *
*unchanged                                             *
*******************************************************/
thread_t *queue_peek(list_t*);


/********************************************************
*Inserts an element into the threadqueue so that the    *
*first one is the next to run                           *
********************************************************/
void alg_insert(alg_e,list_t*,thread_t*);

/*********************************************************
*Inserts the elements into a list sorted by starting time*
*********************************************************/
void queue_insert(list_t*,thread_t*);

void print_time_step (int,int);
struct list_elem *list_switch(struct list_elem *l1, struct list_elem *l2);

void shed_start(list_t *li, list_t *queue,int time,alg_e alg){
    while(queue->first != NULL && queue_peek(queue)->start <= time){
        alg_insert(alg,li,queue_pop(queue));
    }
}

void threadshed(int N,int t, int q,alg_e alg){
    list_t *li = list_init();
    list_t *queue = list_init();
    for(int i = 0;i < N;i++){
        int p,s,l;
        scanf("%d %d %d",&p,&s,&l);
        thread_t *thr = malloc(sizeof(thread_t));
        thr->prio = p;
        thr->start = s;
        thr->lauf = l;
        thr->num = i+1;
        queue_insert(queue,thr);
    }
    int time = 0;
    while(li->first != NULL || queue->first != NULL){
        shed_start(li,queue,time,alg);
        thread_t *thr = queue_pop(li);
        for(int i = 0;i<q;i+=t){
            print_time_step(time,thr==NULL?0:thr->num);
            if(thr!=NULL)
                thr->lauf -=t;
            if(thr!=NULL && thr->lauf > 0 && i == q-t){
                shed_start(li,queue,time+t,alg);
                if (thr->lauf > 0) {
                    alg_insert(alg, li, thr);
                }
            }
            else if(thr==NULL){
                time += t;
                break;
            }
            time += t;
        }
    }
    list_finit(li);
    list_finit(queue);
}


int main(int argc,char **argv){
    int N,t,q;
    alg_e alg;
    int option = 0;
    while((option = getopt(argc,argv,"n:t:q:a:"))!=-1){
        switch(option){
            case 'n' :
                N = atoi(optarg);
                break;
            case 't':
                t = atoi(optarg);
                break;
            case 'q':
                q = atoi(optarg);
                break;
            case 'a':
                if (!strcmp("RR",optarg))
                    alg = RR;
                else if(!strcmp("PRR",optarg))
                    alg = PRR;
                else if(!strcmp("SRTN",optarg))
                    alg = SRTN;
                else if(!strcmp("T",optarg))
                    alg = T;
                else {
                    printf("No such algorithm\n");
                    exit(-1);
                }
                break;

        }
    }
    threadshed(N,t,q,alg);

}

thread_t *queue_peek(list_t *q){
    if(q->first != NULL)
        return (q->first->data);
    return NULL;
}

thread_t *queue_pop(list_t *q){
    if(q->first == NULL)
        return NULL;
    thread_t *out = (q->first->data);
    struct list_elem *f = q->first;
    q->first = q->first->next;
    if(q->last == f){
        q->last = NULL;
    }
    free(f);
    return out;
}

void queue_insert(list_t *li,thread_t *thr){
    if(li->first == NULL){
        list_insert(li,thr);
        return;
    }
    struct list_elem *ael = li->first;
    struct list_elem *bel = ael;
    while(thr->start>=((thread_t*)(ael->data))->start){
        bel = ael;
        if(bel == li->last){
            list_append(li,thr);
            return;
        }
        ael = ael->next;
    }
    if(li->first == ael){
        list_insert(li,thr);
        return;
    }
    struct list_elem *nel = malloc(sizeof(struct list_elem));
    nel->data = thr;
    nel->next = ael;
    bel->next = nel;
}

void alg_insert(alg_e alg,list_t *li,thread_t *thr){
    switch(alg){
        case RR:
            rr_insert(li,thr);
            break;
        case PRR:
            prr_insert(li,thr);
            break;
        case SRTN:
            srtn_insert(li,thr);
            break;
        case T:
            t_insert(li,thr);
            break;
        default:
            printf("No such algorithmi\n");
            exit(-1);
    }
}
void print_time_step (int time, int thread_num) {
    static int    first_time = 1;
    int           i;

    if (first_time) {
        printf ("  Time |  1  2  3  4  5  6  7  8  9  10\n");
        printf ("-------+--------------------------------\n");
        first_time = 0;
    }
    printf ("%06d |", time);
    if (thread_num) {
        for (i = 1; i < thread_num; i++)
            printf ("   ");
        printf ("  %d\n", thread_num);
    } else
        printf ("\n");
}

void rr_insert(list_t *li,thread_t *thr){
    list_append(li, thr);
}
void prr_insert(list_t *li,thread_t *thr){
    list_append(li, thr);
    prr_sort(li);
}
void prr_sort(list_t *li) {
    if (li->first == NULL
        || li->first->data->num == li->last->data->num) {
        return;
    }

    struct list_elem *tempHead, *current, *next;
    int hasChanged = 1;

    tempHead = malloc(sizeof(struct list_elem));
    tempHead->data = NULL;
    tempHead->next = li->first;

    while(hasChanged) {
        hasChanged = 0;
        current = tempHead;
        next = current->next;

        while (next->next != NULL) {
            if(next->data->prio > next->next->data->prio) {
                current->next = list_switch(next, next->next);
                hasChanged = 1;
            }
            current = next;
            if( next->next != NULL )
                next = next->next;
        }
    }
    li->first = tempHead->next;
    free(tempHead);
    current = li->first;
    while(current->next != NULL) {
        current = current->next;
    }
    li->last = current;
}
void srtn_insert(list_t *li,thread_t *thr){}
void t_insert(list_t *li,thread_t *thr){
    list_append(li,thr);
}
struct list_elem *list_switch(struct list_elem *l1, struct list_elem *l2)
{
    l1->next = l2->next;
    l2->next = l1;
    return l2;
}