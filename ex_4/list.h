typedef struct thread_t{
    int prio;
    //Zeit nach der der prozess startet
    int start;
    //zeit die der prozes benötigt
    int lauf;
    //nummer des threads
    int num;
}thread_t;

typedef enum alg_e {RR,PRR,SRTN,T} alg_e;


struct list_elem {
    struct list_elem *next;	// Zeiger auf das naechste Element
    thread_t *data;	// Zeiger auf ein Datenobject
};

typedef struct list {
    struct list_elem *first;// erstes Element in der Liste
    struct list_elem *last;	// letztes Element in der Liste
} list_t;

/* function prototypes */
list_t           *list_init ();
struct list_elem *list_insert (list_t *list, thread_t *data);
struct list_elem *list_append (list_t *list, thread_t *data);
int              list_remove (list_t *list, struct list_elem *elem);
void             list_finit (list_t *list);
void             list_print (list_t *list, void (*print_elem) (thread_t *));
void             print_string(thread_t *data, int time);
int cmp_pid(const thread_t *elem1, const thread_t *elem2);
struct list_elem *list_find (list_t *list, thread_t *data, int (*cmp_elem) (const thread_t *, const thread_t *));
