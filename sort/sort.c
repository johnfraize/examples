#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

typedef struct MeData_s{
    int value;  // what I am doing this for
    LIST_ENTRY(MeData_s) listCrap;
}MeData;

int main(int argc, char* argv[]){
    int i;

    LIST_HEAD(listhead, MeData_s) head;
    LIST_INIT(&head);

    MeData* entry;
    MeData* teste;
    MeData* most = 0; // the current highest one

    // for each input item create an ordered list 
    for(i = 1; i < argc; i ++){

        entry = malloc(sizeof(MeData));

        entry->value = atoi(argv[i]);

        // if the LIST is empty just add it
        if( LIST_EMPTY(&head) ){
            LIST_INSERT_HEAD(&head,entry,listCrap);
        }
        else{
            most = LIST_FIRST(&head);
            // now lets figure out where to place it
            LIST_FOREACH(teste, &head, listCrap) {
                if( teste->value > most->value ){
                    most = teste;
                }
            }
            if ( entry->value > most->value ) {
                LIST_INSERT_AFTER(most,entry,listCrap);
            }
            else{
                LIST_INSERT_BEFORE(most,entry,listCrap);
            }
        }
    }

    // now print out the list
    LIST_FOREACH(entry, &head, listCrap) {
        printf("%d ", entry->value);
    }
    printf("\n");

    return 0;
}
