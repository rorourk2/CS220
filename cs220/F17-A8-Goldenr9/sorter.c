#include <stdio.h>
#include <stdlib.h>
#include "node.h"
Node *sort_nodes(Node *head){
	Node *a;
	Node *b;
	Node *c;
	Node *d;
	Node *ne;
	Node *pr;
	int i=1;
	a=head;
	while(i==1){
		i=0;
		b=a->next;
		
		if(a<b){
			c=a;
			a=b;
			ne=b->next;
			a->prev=c->prev;
			a->next=c;
			c->next=ne;
			c->prev=a;
			b=c;
			
			i=1;
		}
		while(b->next!=NULL){
			c=b->next;
			if(b<c){
				d=b;
				b=c;
				ne=c->next;
				b->prev=d->prev;
				b->next=d;
				d->next=ne;
				d->prev=b;
				b->prev->next=b;
				i=1;
			}
			b=b->next;
		}
		
	}
	
	return a;
}
/*Node *get_head(Node *p)
{
	return p == NULL ?
		NULL :
		((Node *)((char *)(p)-(unsigned long)(&((Node *)0)->next)));
}
void print_node(Node *node)
{
	printf("%p\n", node);
}

void print_list(Node *head)
{
	while(head) {
		print_node(head);
		printf(" -> ");
		head = get_head(head->next);
	}
	printf(" NULL\n");
}*/

/*int main(){
	Node *a = malloc(sizeof(Node));
	Node *b= malloc(sizeof(Node));
	Node *c= malloc(sizeof(Node));
	Node *d= malloc(sizeof(Node));
	a->next=b;
	a->prev=NULL;
	b->prev=a;
	b->next=c;
	c->prev=b;
	c->next=d;
	d->prev=c;
	d->next=NULL;
	print_node(a);
	print_node(b);
	print_node(c);
	print_node(d);
	Node *g=sort_nodes(a);
	print_list(g);
	return 0;
}*/
	
