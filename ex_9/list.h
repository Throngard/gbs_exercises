enum mem_status {unused, used};

struct list_elem {
    struct list_elem *next;	// Zeiger auf das naechste Element
    char	     *addr;	// Zeiger auf ein Datenobject
    void *mem_pointer;
    enum mem_status status;
    unsigned int size;
    unsigned int in_use;
};

typedef struct list {
    struct list_elem *first;// erstes Element in der Liste
    struct list_elem *last;	// letztes Element in der Liste
} list_t;

/* function prototypes */
list_t           *list_init ();
struct list_elem *list_insert (list_t *list, void *mem_pointer, unsigned int size, unsigned int in_use);
struct list_elem *list_append (list_t *list, void *mem_pointer, unsigned int size, unsigned int in_use);
struct list_elem *list_put (list_t *list, struct list_elem *current_elem, void *mem_pointer, unsigned int size, unsigned int in_use);
int              list_remove (list_t *list, struct list_elem *elem);
void             list_finit (list_t *list);
void             list_print (list_t *list, void (*print_elem) (struct list_elem *elem));
void             print_string(struct list_elem *elem);
struct list_elem *list_find (list_t *list, char *data, int (*cmp_elem) (const char *, const char *));
struct list_elem *list_find_addr (list_t *list, void *addr);