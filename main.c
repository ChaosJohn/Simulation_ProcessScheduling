#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 

typedef struct _pcb {
  int pid; 
  char *pname; 
  int priority; 
  int needtime; 
  int runtime; 
  int waittime; 
  struct _pcb *next; 
} PCBNode, *PCBList, *PCBPointer; 

PCBList pcbList; 

void 
inputWithPriority(int pcbSize) {
  int i;
  PCBPointer pcbPointer; 
  pcbList = (PCBPointer)malloc(sizeof(PCBNode)); 
  pcbList->next = NULL; 
  PCBPointer pcbCursor = pcbList; 
  for (i = 0; i < pcbSize; i++) {
    pcbPointer = (PCBPointer)malloc(sizeof(PCBNode)); 
    printf("建立第  %d  个进程\n", i); 
    pcbPointer->pid = i; 
    /*printf("\t进程名为: "); scanf("%s", pcbPointer->pname); fflush(stdin); */
    /*printf("\t进程名为: "); gets(pcbPointer->pname); fflush(stdin); */
    printf("\t优先级为: "); scanf("%d", &(pcbPointer->priority)); fflush(stdin);  
    printf("\t服务时间: "); scanf("%d", &(pcbPointer->needtime)); fflush(stdin);  
    pcbPointer->runtime = 0; 
    pcbPointer->next = NULL; 
    pcbCursor->next = pcbPointer; 
    pcbCursor = pcbCursor->next; 
  }
}

void 
print4test() {
  PCBPointer cursor = pcbList->next; 
  while (cursor != NULL) {
    printf("%d\t%s\t%d\t%d\n", cursor->pid, cursor->pname, cursor->priority, cursor->needtime); 
    /*printf("%d\t%d\n", cursor->pid, cursor->priority); */
    cursor = cursor->next; 
  }
}

void 
printHorizontalBar() {
  int i; 
  for (i = 0; i <= 33; i++) 
    printf("-"); 
  printf("\n"); 
}

void 
display() {
  PCBPointer cursor = pcbList->next; 
  printHorizontalBar(); 
  printf("|\t     运行中    \t\t |\n"); 
  printHorizontalBar(); 
  /*printf("\n|  pid\t| pname\t| prior\t|ndtime\t|uptime|\n"); */
  printf("|进程号\t|优先级\t|已运行 |服务时间|\n"); 
  printf("|%d\t|%d\t|%d\t|%d\t |\n", cursor->pid, cursor->priority, cursor->runtime, cursor->needtime); 
  printHorizontalBar(); 
  cursor = cursor->next; 
  if (cursor == NULL) 
    return; 
  printf("|\t     阻塞队列    \t |\n"); 
  printHorizontalBar(); 
  while(cursor != NULL) {
    /*printf("|%d\t|%s\t|%d\t|%d\t|%d\t |\n", cursor->pid, cursor->pname, cursor->priority, cursor->runtime, cursor->needtime); */
    printf("|%d\t|%d\t|%d\t|%d\t |\n", cursor->pid, cursor->priority, cursor->runtime, cursor->needtime); 
   cursor = cursor->next;  
  }
  printHorizontalBar(); 
}

void 
changePriority() {
  PCBPointer cursor = pcbList->next->next; 
  while(cursor != NULL) {
    cursor->priority *= 2; 
    cursor = cursor->next; 
  }
}

void 
sortByPriority() {
  PCBPointer head = pcbList; 
  PCBPointer cursor; 
  PCBPointer cursorPrev; 
  PCBPointer maxPrev; 
  PCBPointer max; 
  while(head->next->next != NULL) {
    for (cursor = head->next, max = cursor, cursorPrev = head, maxPrev = cursorPrev; 
        cursor != NULL; 
        cursor = cursor->next, cursorPrev = cursorPrev->next) {
      if (cursor->priority > max->priority) {
        max = cursor; 
        maxPrev = cursorPrev; 
      }
    }
    maxPrev->next = max->next; 
    max->next = head->next; 
    head->next = max; 
    head = head->next; 
    /*print4test(); */
  }
}


void 
run() {
  PCBPointer head = pcbList; 
  /*PCBPointer cursor; */
  PCBPointer running; 
  while (head->next != NULL) {
    sortByPriority(); 
    running = head->next;
    printf("\n#=>第  %d  个进程正在运行第  %d  个时间片\n", running->pid, running->runtime); 
    display();    
    /*fflush(stdin); */
    /*getchar(); */
    sleep(1);
    running->runtime++; 
    changePriority(); 
    if (running->runtime > running->needtime) {
      head->next = head->next->next; 
      printf("\n#=>第  %d  个进程已结束\n\n", running->pid); 
    }
  }
}


int 
main(int argc, 
    char **argv) {
  int pcbSize = 0; 
  printf("需要建立的进程个数: "); scanf("%d", &pcbSize); fflush(stdin); 
  inputWithPriority(pcbSize); 
  run(); 
  /*display(); */
  printf("#=>全部进程已结束\n"); 

  return 0; 
}
