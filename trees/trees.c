#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

void *myRoot = NULL;

typedef struct{
    char* name;
    int born;
}PersonData;


int compare(const void* pa, const void* pb){
    PersonData* a;
    PersonData* b;

    a = (PersonData*)pa;
    b = (PersonData*)pb;

    if( a->born < b->born ){
        return -1;
    }
    else if( a->born > b->born ){
        return 1;
    }
    else{
        return 0;
    }
}


void action(const void *nodep, const VISIT which, const int depth){


    
    PersonData* d = *((PersonData**)nodep);

    switch(which){
    case postorder:
        printf("preorder %s %d\n",d->name,d->born);
        break;
    case leaf:
        printf("leaf %s %d\n",d->name, d->born);
        break;
    }
}

int main(int argc, char* argv[]) {
    int i;
    const int n = 3;
    PersonData* d;
    ENTRY item;
    ENTRY* found = 0;

    PersonData data[] = {
                         {"pop", 1947},
                         {"nana", 1948},
                         {"gabe", 2001},
                         {"josh", 1999},
                         {"abel", 22022}
    };

    void* myVal;

    const int entries = sizeof(data)/sizeof(PersonData);

    hcreate(entries);


    // populate the tree with a bunch of stuff
    for ( i = 0; i < sizeof(data)/sizeof(PersonData);  i++ ){
        //        myVal = malloc(sizeof(PersonalData));

        PersonData* d = malloc(sizeof(PersonData));
        *d  = data[i]; // copy in

        item.key = d->name;
        item.data = d;

        hsearch(item,ENTER);

        // populate the tree
        ///        PersonData* x = (PersonData*)tsearch((void*)d, &myRoot, compare);

        //        printf("%s, born %d\n",x->name,x->born);
    }


    // Now lets look for it

    item.key = "gabe";

    found = hsearch(item,FIND);

    if( found ){
        PersonData* f = (PersonData*)found->data;
        printf("found %s %d\n", f->name, f->born);
    }


    //    tdestroy(myRoot, free);

    return 0;
}
