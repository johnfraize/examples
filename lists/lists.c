#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>



typedef struct PersonData_s{
    char* name;
    int born;
    LIST_ENTRY(PersonData_s) listStuff; // crap you need to put in your data to contain the list info
}PersonData;


int main(int argc, char* argv[]) {
    int i;
    const int n = 3;

    PersonData data[] = {
                         {"pop", 1947},
                         {"nana", 1948},
                         {"gabe", 2001},
                         {"josh", 1999},
                         {"abel", 22022}
    };

    const int entries = sizeof(data)/sizeof(PersonData);


    LIST_HEAD(listhead, PersonData_s) head;
    LIST_INIT(&head);

    PersonData* person;

    for(i=0;i<entries;i++){
        person  = malloc(sizeof(PersonData));
        person->name = data[i].name;
        person->born = data[i].born;
        LIST_INSERT_HEAD(&head, person, listStuff);
    }

    // iterate the list 
    LIST_FOREACH(person, &head, listStuff) {
        printf("Read %s, %d\n", person->name, person->born);
        if( person->name == "gabe" ){
            printf("it is gabe\n");
        }
    }

    // now remove evething from the list
    while (!LIST_EMPTY(&head)) {
        person = LIST_FIRST(&head);
        LIST_REMOVE(person,listStuff);
        free(person);
    }

    return 0;
}
