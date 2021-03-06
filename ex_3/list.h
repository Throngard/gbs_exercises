struct list_elem {
    struct list_elem *next;	// Zeiger auf das naechste Element
    pid_t data;	// Zeiger auf ein Datenobject
};

typedef struct list {
    struct list_elem *first;// erstes Element in der Liste
    struct list_elem *last;	// letztes Element in der Liste
} list_t;

/* function prototypes */
list_t           *list_init ();
struct list_elem *list_insert (list_t *list, pid_t data);
struct list_elem *list_append (list_t *list, pid_t data);
int              list_remove (list_t *list, struct list_elem *elem);
void             list_finit (list_t *list);
void             list_print (list_t *list, void (*print_elem) (pid_t));
void             print_string(pid_t data);
int cmp_pid(const pid_t elem1, const pid_t elem2);
struct list_elem *list_find (list_t *list, pid_t data, int (*cmp_elem) (const pid_t, const pid_t));