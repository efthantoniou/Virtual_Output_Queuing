#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define  PCONS 0.5

typedef struct node oura;
oura *all;  //Global gia na thn vlepoyn oi synarthseis.

struct node{
	oura *head;
	oura *tail;
	oura *next;
	int timeslot;
	int input;
	int output;
};

int queue_empty(oura *kappa){
	return kappa == NULL; //epistrefei 1 an einai adeia kai 0 an exei stoixeio/a
}

int createpacket(){
	double p = (double)rand()/(double)RAND_MAX;
	return p <= PCONS; //An einai mikrotero ths statherhs monadas dhmioyrgei to paketo.
}

int assignoutput(int n){
	double thesi = (double)rand()/(double)RAND_MAX;
	return (int)(thesi*n) + 1;// Mas parexei enan pseudotyxaio arithmo sto diasthma [1,n] 
}

void get(int thesh){//Diwxnei to paketo apo thn oyra.
	if(!queue_empty(all[thesh].head)){
		oura *temp = all[thesh].head;
		all[thesh].head = all[thesh].head->next;
		free(temp);
	}
	return;
}

void put(oura kappa, int n, int j, int xronos){//vazei stoixeia sthn oyra afoy ta arxikopoihsei katallhla.
	oura *temp;
	temp = (oura*)malloc(sizeof(oura));
	if(queue_empty(kappa.head)){
		temp->head = temp;
		temp->next = NULL;
		temp->timeslot = xronos; //einai xronosxismh kai xrhsimopoieitai gia ta statistika.
		temp->input = kappa.input;
		temp->output = kappa.output;
		all[j].head = all[j].tail = temp;
	}
	else{
		temp->head = kappa.head;
		temp->next = NULL;
		temp->timeslot = xronos;
		temp->input = kappa.input;
		temp->output = kappa.output;
		all[j].tail->next = temp;
		all[j].tail = temp;
	}
}

int counter(oura *kappa, oura *keepo){//Metraei posa stoixeia exei mia oyra. thn xrhsimopoioume gia voitheia, 
	int count = 0;					  //ston algorithmo epiloghs paketoy.
	oura *temp = kappa;
	
	while(temp != keepo && temp!= NULL && keepo != NULL){
		++count;
		temp = temp->next;
	}
	if(temp != NULL)
		return count + 1;
	else
		return 0;
}

void metagwgeas(oura *kappa, int n){
	int i,j,k,temp_output,eks_i,eks_j,thesh,max,count,father,stat2;
	int *exodoi = NULL;
	count = stat2 = 0;
	exodoi = (int *)malloc(n*sizeof(int));
	for(i=0;i<n;i++){
		exodoi[i] = 0;
	}
	for(father=0;father<n;father++){
			//diadikasia eisodoy
			for(j=0;j<n*n;j=j+n){
				temp_output = 0;
				if(createpacket())
					temp_output = assignoutput(n);
				if(temp_output != 0){
					for(k=j;k<j+n;k++){
						if(kappa[k].output == temp_output){
							put(kappa[k],n,k,father);
						}
					}
				}
			}
			//diadikasia eksodoy
			for(eks_i=0;eks_i<n;eks_i++){//eksodos poy eksetazoyme
				max = 0;
				thesh = 0;
				for(eks_j=0;eks_j<n*n;eks_j=eks_j+n){//eisodos poy eksetazoume
					if(counter(kappa[eks_i + eks_j].head,kappa[eks_i + eks_j].tail) > max){
						max = counter(kappa[eks_i + eks_j].head,kappa[eks_i + eks_j].tail);
						thesh = eks_i + eks_j;
					}
				}
				if(max != 0 ){
					stat2 += father - all[thesh].timeslot;//gia statistika
					get(thesh);
					count++;//metraei ta paketa poy eksyphretithikan
					exodoi[eks_i] = exodoi[eks_i] + 1;
				}
			}
	}
	printf("\n");
	for(i=0;i<n;i++){
		printf("Rythmos metadoshs eksodoy[%d]: %d\n", i, exodoi[i]);
	}
	printf("\nSynolikos rythmos metadoshs: %f\n", (float)count/(float)n);
	printf("\nMesos xronos anamonhs ergasias: %f\n",(float)stat2/(float)count);

}

void freeall(oura *kappa, int n){
	int i;
	oura *temp;
	for(i=0;i<n*n;i++){
		while(kappa[i].head != kappa[i].tail){
			temp = kappa[i].head;
			kappa[i].head = kappa[i].head->next;
			free(temp);
		}
		free(kappa[i].head);
	}
}


void main(void){
	srand(time(NULL));
	int i,j,n,counter;
	do{
		printf("Give input: ");
		scanf("%d",&n);
		printf("\n");
	}while(n <= 1);
	all = (oura *)malloc((n*n)*sizeof(oura));
	for(i=0;i<n*n;i++){
		all[i].head = all[i].tail = all[i].next = NULL;
		all[i].input = (i / n) + 1;
		if( (i + 1) % n == 0){
			all[i].output = n;
		}
		else{
			all[i].output = ((i+1) % n);
		}
	}

	metagwgeas(all,n);

	free(all);
}