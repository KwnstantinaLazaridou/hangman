/*Lazaridou Kwnstantina A.E.M:915 email:kolazarid@uth.gr
 *Hatzivasileiou Eleni A.E.M:1050 email:hatzivas@inf.uth,gr */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define WORDLEN 30

/* Given the name of a file, read and return the next word from it, 
or NULL if there are no more words */

char *getWord(char *filename)  {
	char formatstr[15], *word;
	static FILE *input;
	static int firstTime = 1;
	if (firstTime) { 
		input = fopen(filename, "r");
		if (input == NULL) {
			printf("ERROR: Could not open file \"%s\"\n", filename);
			/*getch();*/
			exit(1);
		}
		firstTime = 0;
	}
	word = (char*)malloc(sizeof(char)*WORDLEN);
	if (word == NULL) {
		printf("ERROR: Memory allocation error in getWord\n");
		exit(1);
	}
	sprintf(formatstr, "%%%ds", WORDLEN-1);
	fscanf(input, formatstr, word);
	if (feof(input)) {
		fclose(input);
		return NULL;
	}
	return word;
}

/* domi gia tin lista me tis lekseis */
struct DictList{
    char *word;
    struct DictList *next;
};

/*domi gia tin lista me tis categories*/
struct CategList {
    struct DictList **array;
    struct CategList *next;
    char *representator;
    int size;
};

/*domi gia tis dio listes gia ta grammata ( 1 lista gia ta grammata pou exoun 
  xrisimopoiithei kai alli mia gia ta grammata pou dn exoun xrisimopoiithei) */
struct LetterList {
    char letter;
    struct LetterList *next;
};
  
/*edo dimiourgoume tin lista me tis lekseis*/
void createDictList(int wordLen, struct DictList **head, char *dictionary) {
     char *word;
     char flag=0;/*flag gia tin dimiougia tou head tis listas*/
     struct DictList *cur,*temp;
     while(1){
         word = getWord("./dictionary.txt");
         if (word==NULL) {
             break;
         }
         if (strlen(word) != wordLen) {
         	continue;
         }
         if (flag==0) {/*dimiourgia tou head tis listas*/
         	*head=(struct DictList*) malloc(sizeof(struct DictList));
         	(*head)->word=word;
         	(*head)->next=NULL;
         	cur=*head;
         	flag=1;
       	 }
		 else {/*dimiourgia ton ipoloipon theseon tis listas*/
         	temp=(struct DictList*)malloc(sizeof(struct DictList));
         	temp->word=word;
         	temp->next=NULL;
         	cur->next=temp;
         	cur=temp;
       	 }
     }
}

/*dimiourgia tou head tis listas me tis katigories*/
void createHeadOfCateg(struct CategList **categHead, struct DictList *dictHead, int size, int wordLen)
{
    int i;
    struct DictList *dictCur;
    *categHead = (struct CategList*)malloc(sizeof(struct CategList));
    (*categHead)->next = NULL;
    (*categHead)->size = size;
    (*categHead)->representator = (char*)malloc(sizeof(char)*(wordLen+1));
    (*categHead)->array = (struct DictList**)malloc(sizeof(struct DictList*)*size);
  
    /*dimiourgia tou antiprosopou*/
    for (i=0;i<wordLen;i++) {
    	(*categHead)->representator[i] = '-';
    }
    (*categHead)->representator[wordLen] = '\0';
  
    dictCur = dictHead;
    i = 0;
    
    /*dimiourgia tou dinamikou pinaka kai antistoixisi me tis lekseis pou exei i 
      lista me tis lekseis */
  
    while (dictCur!=NULL) {
    	(*categHead)->array[i] = dictCur;
    	dictCur = dictCur->next;
    	i++;
  	}
  
	/*printf("%s\n",(*categHead)->array[0]->word);
    printf("%s\n",(*categHead)->representator); */
}

/*epistrofi tou megethous tis listas me tis lekseis */
int getSizeOfDictList (struct DictList *head) 
{
    struct DictList *cur;
    int size = 0;
    cur = head;
  
    while (cur != NULL) {
    	/*printf("word:%s\n",cur->word);*/
    	size++;
    	cur=cur->next;
  	}
  	return size;
}

/*sinartisi pou pairnei to head ton katigorion kai enan antiprosopo kai 
  epistrefei 1 an o antiprosopos iparxei idi stin lista, diaforetika epistrefei 0 */
int findCategory (struct CategList *categHead,char *representator)
{
    struct CategList *cur;
    int ret;
    cur = categHead;
  
    while (cur != NULL) {
    	ret=strcmp(representator,cur->representator);
    	if (ret == 0) {
      		cur->size++;
      		return 1;
    	}
    	cur=cur->next;
  	}
  	return 0;
}

/*dimiougia katigorias */
void createCategory(struct CategList *categHead, char letter, int wordLen, char *word)
{
    char *representator=(char*)malloc(sizeof(char)*(wordLen+1));
 	struct CategList *category;
 	int i,ret;
 
/*edo dimiourgoume ton antiprosopo me vasi to gramma pou exei dothei kai mia
  sigekrimeni leksi pou iparxei stin lista me tis lekseis */
	for (i=0;i<wordLen;i++) {
   		if (word[i] != letter) {
     		representator[i]='-';
   		}
		else {
     		representator[i]=letter;
   		}
   		
		if(categHead->representator[i]!='-'){
     		representator[i]=categHead->representator[i];
   		}
 	}
 	representator[wordLen]='\0';
 
/*edo psaxnoume an o antiprosopos iparxei idi, an den iparxei tote dimiourgoume
  mia nea katigoria (xoris tin dimiourgia tou dinamikou pinaka), 
  diaforetika den kanoume tipota */
	ret=findCategory(categHead,representator);
 
	if (ret == 0) {
   		category = (struct CategList*)malloc(sizeof(struct CategList));
   		category->next = categHead->next;
   		categHead->next = category;
   		category->size = 1;
   		category->representator = representator;
 	}
}

/*sinartisi gia debugging */
void printCategories (struct CategList *categHead) 
{
    struct CategList *cur;
    int i;
    cur=categHead;
    while (cur != NULL) {
    	printf("representator:%s\n",cur->representator);
    	printf("size:%i\n",cur->size);
    	/*for(i=0;i<cur->size;i++){
      		printf("  word:%s\n",cur->array[i]->word);
    	}*/
    	cur=cur->next;
  	}
}

/*dimiourgia ton dinamikon pinakon gia tis katigories pou exoume dimiourgisei */
void createArrays(struct CategList *categHead)
{
    struct CategList *cur;
    cur=categHead->next;
    while (cur != NULL) {
    	cur->array = (struct DictList**)malloc(sizeof(struct DictList*)*cur->size);
    	cur->size = 0;
    	cur = cur->next;
  	}
}

/*edo psaxnoume na vroume mia katigoria me vasi ton antiprosopo kai tin prosthetoume stin lista me tis katigories */
void findAndAddtoCategory(struct CategList *categHead,char *representator,struct DictList *wordPointer)
{
    struct CategList *cur;
    int ret;
    cur = categHead;
    while (cur != NULL) {
    	ret = strcmp(representator,cur->representator);
    	if(ret == 0) {
        	cur->array[cur->size]=wordPointer;
        	cur->size++;
        	return;
    	}
    	cur=cur->next;
  	}
}

/*ayth i sinartisi pairnei ton pointer apo mia sigekrimeni leksi tis listas me tis lekseis
 kai tin prostetei stin katigoria pou tairiazei me vasei ton antiprosopo */
void addToCategory(struct CategList *categHead, char letter, int wordLen, struct DictList *wordPointer)
{
    char *representator = (char*)malloc(sizeof(char)*(wordLen+1));
    struct CategList *category;
    int i, ret;

    for (i=0;i<wordLen;i++) {
   		if (wordPointer->word[i]!=letter) {
     		representator[i]='-';
   		}
		else {
     		representator[i]=letter;
   		}
   
		if (categHead->representator[i]!='-') {
     		representator[i]=categHead->representator[i];
   		}
 	}
 	representator[wordLen]='\0';
 	findAndAddtoCategory(categHead,representator,wordPointer);
 	free(representator);
}

/*sinartisi pou dimiourgei oles tis katigories me vasi to gramma pou exei dothei */
void createCategories(struct CategList *categHead,char letter,int wordLen)
{
    int i,j;
    /*dimiourgia tis listas me tis katigories xoris tin dimiourgia ton dinamikon pinakon */
  	for (i=0;i<categHead->size;i++) {
    	for (j=0;j<wordLen;j++) {
      		if (categHead->array[i]->word[j]==letter) {                                      
        		createCategory(categHead,letter,wordLen,categHead->array[i]->word);
        		break;
      		}
    	}                       
  	}
  
	/*dimiourgia ton dinamikon pinakon gia oles tis katigories */
	createArrays(categHead);
  	/*prosthiki ton lekseon pou iparxoun stis antistoixes katigories */
  
	for(i=0;i<categHead->size;i++) {
	    for(j=0;j<wordLen;j++) {
      		if(categHead->array[i]->word[j]==letter){
        		addToCategory(categHead,letter,wordLen,categHead->array[i]);
        		break;
      		}
    	}                       
  	}
  	/*printCategories(categHead); */
}

/*elegxos ton orismaton pou dinontai sto programma */
int checkArguments(int argc, char **argv, int *retWordLen,int *retTurns)
{
    int wordLen,turns;
    if(argc<4) {
    	printf("Insufficient arguments\n");
    	return 0;
  	} 
	else if(argc>4) {
	    printf("Too many arguments\n");
	    return 0;
	}
  
	wordLen=atoi(argv[1]);
  	while(1) {
    	if(wordLen<1||wordLen>29||wordLen==26||wordLen==27){    
      		printf("There are no words of Length %i in the dictionary\n"); 
      		printf("Specify a different length:");
      		scanf("%i",&wordLen);  
    	}
		else {
      		break;
    	} 
  	}
  	turns=atoi(argv[2]);
   
	while(1){
	    if (turns<1) {    
      		printf("You may not have fewer than one turns\n"); 
      		printf("Specify a different number of turns:");
      		scanf("%i",&turns);  
    	}
		else {
      		break;
    	} 
  	}
  	*retWordLen=wordLen;
  	*retTurns=turns;
  	return 1;
}

/*dimiourgia listas me ta grammata pou den exoun xrisimopoiithei */
void createUnusedLetterList(struct LetterList **head){
     struct LetterList *cur,*temp;
     int i;
     for (i=97;i<123;i++) {
         temp=(struct LetterList*) malloc(sizeof(struct LetterList));
         temp->letter=i;
         temp->next=NULL;
         if (i==97) {
             *head=temp;
        	 cur=temp;
       	 }
		 else{
         	cur->next=temp;
         	cur=temp;
       	 }
     }
}

/*tipoma tis listas me grammata pou exoun xrisimopoieithei i pou den exoun xrisimopoeithei */
void printLetterList(struct LetterList *head)
{
    struct LetterList *cur;
    cur=head;
  
    while (cur!=NULL) {
        printf("%c ",cur->letter);
        cur=cur->next;
    }
    printf("\n");
}

/*prosthiki struct stin lista me ta xrisimopoiimena grammata */
void addToUsedList(struct LetterList **head, struct LetterList *addLetter)
{
    struct LetterList *cur,*prev;
    if (*head==NULL) {
   		*head=addLetter;
   		addLetter->next=NULL;
 	}
	else{
   		cur=*head;
   		prev=*head;
   		while(cur!=NULL) {
        	if(cur->letter<addLetter->letter){
       			prev=cur;
       			cur=cur->next;
     		}
			else{
       			if(cur==*head){
        			addLetter->next=*head;
        			*head=addLetter;
        			return;
       			}
				else{
         			prev->next=addLetter;
         			addLetter->next=cur;
         			return;
       			}
     		} 
   		}
   		prev->next=addLetter;
   	    addLetter->next=NULL;
 	}
}

/*afairesi struct apo tin lista me ta mi xrisimopoiimena grammata */
struct LetterList * removeUnusedLetterList(struct LetterList **head, char letter)
{
    struct LetterList *cur,*prev;
    cur=*head;
    prev=*head;

    while(cur!=NULL) {
    	if(cur->letter==letter){
      		if(cur==*head){
        		*head=(*head)->next;
      		}
			else {
       			prev->next=cur->next;
      		}
      		return cur;
    	}
    	prev = cur;
    	cur = cur->next;
  	}
  	return NULL;
}

/*eyresi an iparxei ena sigekrimeno gramma stin lista me ta xrisimopoiimena grammata */
int searchUsedLetterList(struct LetterList *head,char letter){
    struct LetterList *cur;
    cur=head;
  
	while(cur!=NULL){
    	if(cur->letter==letter){
      		return 1;
    	}
    	cur=cur->next;
  	}
  	return 0;
}

/*sinartisi pou zitaei apo ton xristi na mantepsei ena gramma */
char getLetter(struct LetterList *usedHead){
  	char letter;
  	int ret;
  	
	while(1) {
	    printf("Guess a letter:");
    	scanf("%c",&letter);
    	/*printf("\n");*/
    
		if(letter<65||(letter>90&&letter<97)||letter>122) {
	    	continue;
    	}
		else if(letter<97) {
      		letter	+=	32;
      		ret	= searchUsedLetterList(usedHead,letter);
      		if (ret==1) {
        		continue;
      		}
			else{
        		break;
      		}
    	}
		else{
      		ret = searchUsedLetterList(usedHead,letter);
      		if(ret==1){
        		continue;
      		}
			else{
        		break;
      		}
    	}
  	} 
  	return letter;
}

/*dimiourgia tis leksis me vasei ta grammata pou exei mantepsei o xristis, gia 
 ta ipoloipa grammata xrisimopoioume tin kato payla */
void createWord(struct CategList *categ,char *word,int wordLen){
  int i;
  for(i=0;i<wordLen;i++){
    if(categ->representator[i]=='-'){
      word[i]='_';
    }else{
      word[i]=categ->representator[i];
    }
  }
}

/*epilogi tis poliplithesteris katigorias */
void chooseCategory(struct CategList **head){
    struct CategList *cur,*best=NULL,*temp;
    cur=(*head)->next;
  
	if (cur==NULL) {
	    return;
	}
    
    while (cur!=NULL) {
    	if(best==NULL){
      		best=cur;
    	}
		else if (cur->size>best->size) {
      		best=cur;
    	}
    	cur=cur->next;
  	}
  	*head=best;
  	best->next=NULL;
}

/*epistrofi tis leksis pou exei to head tis listas me tis katigories */
char *chooseWord(struct CategList *head){
	return head->array[0]->word;
}

/*ayth i sinartisi epistrefei an 0 an exei oloklirothei i leksi kai 0 an den exei
 oloklirothei*/
int findWord(struct CategList *head, int wordLen){
    int i;
  
    for(i=0; i<wordLen; i++){
   		if(head->representator[i] == '-'){
    		return 0;
   		}
  	}
  	printf("find:%s\n",head->representator);
  	return 1;
}

/*apodesmeysi tis listas me ta grammata pou exoun xrisimopoiithei */
void clearUsedList(struct LetterList **head){
    struct LetterList *cur,*temp;
    cur=*head;

	while (cur!=NULL) {
   		temp=cur;
   		cur=cur->next;
   		free(temp); 
  	}
  	*head=NULL; 
}

/*apodesmeysi tis listas me ta grammata pou den exoun xrisimopoiithei */
void clearUnusedList(struct LetterList **head) {
    struct LetterList *cur,*temp;
    cur=*head;
    
	while(cur!=NULL){
        temp=cur;
   		cur=cur->next;
   		free(temp); 
  	}
  	*head=NULL; 
}

/*apodesmeysi tis listas me tis katigories, opos episis apodesmeysi kai ton dinamikon pinakon */
void clearCategList(struct CategList **head){
    struct CategList *cur,*temp;
    cur = *head;
  
	while (cur!=NULL) {
	    temp=cur;
   		cur=cur->next;
   		free(temp->array);
   		free(temp); 
  	}
    *head=NULL; 
}

/*sinartisi pou kanei klisi gia tin apodesmeysi olon ton liston */
void clearLists(struct LetterList **usedHead, struct LetterList **unusedHead, struct CategList **categHead){
    clearUsedList(usedHead);
    clearUnusedList(usedHead);
    clearCategList(categHead);
}

int main(int argc, char *argv[])
{
    int i;
    char temp;
    int wordLen,turns,ret;
    int sizeOfDictList;
    int currentTurn;
    struct DictList *dictHead=NULL;
    struct LetterList *usedHead=NULL, *unusedHead=NULL,*addLetter;
    struct CategList *categHead=NULL;
    char *word;
    char letter;

    /*elegxos orismaton*/
    ret=checkArguments(argc,argv,&wordLen,&turns);

    if(ret==0){
      printf("\t LENGTH: requested length of mystery word. Must be >1 , <30, !=26, !=27\n");
      printf("\t TURNS: requested number of turns. Must be >0\n");
      printf("\t DICTIONARY: name of dictionary file\n");
      exit(1);
    };

    word=(char*)malloc(sizeof(char)*(wordLen+1));
    word[wordLen]='\0';
    
	/*dimiourgia tis listas me tis lekseis*/
    createDictList(wordLen,&dictHead,argv[3]);
    /*megethos tis listas me tis lekseis*/
    sizeOfDictList=getSizeOfDictList(dictHead);

    while(1){
        /*dimiourgia tis listas me ta grammata pou den exoun xrisimopoiithei*/
        createUnusedLetterList(&unusedHead);
        /*dimiourgia tou head tis listas me tis katigories*/
        createHeadOfCateg(&categHead,dictHead,sizeOfDictList,wordLen);
        currentTurn=0;
        i=0;
        /*elegxos an iparxei giros */
        
		while(turns-currentTurn>0){
	        printf("Remaining guesses:%i\n",turns-currentTurn);
	        printf("Used letters:");
	        /*emfanisi ton grammaton pou exoun xrisimopoiithei*/
	        printLetterList(usedHead);
	        printf("Unused letters:");

	        /*emfanisi ton grammaton pou den exoun xrisimopoiithei*/
	        printLetterList(unusedHead);

	        /*dimiourgia tis lekseis (me tis kato pavles kai ta grammata pou exei mantepsei o xristis)
	          pou prepei na emfanisoume ston xristi*/
	        createWord(categHead,word,wordLen);
	        printf("Word:%s\n",word);
	        /*zitame to gramma pou prepei na dosei o xristis gia na proxorisei ston epomeno giro*/
	        letter=getLetter(usedHead);

	        /*afairoume to gramma apo tin lista me ta mi xrisimopoiimena grammata */
	        addLetter=removeUnusedLetterList(&unusedHead,letter); 
	
	        if (addLetter==NULL) {
	            printf("You have already guessed this letter\n");
          		continue;
        	}

 	       /*printf("letter:%i\n",letter);
 	       prosthetoume to gramma pou edose o xristis stin lista me ta xrisimopoiimena grammata*/
 	       addToUsedList(&usedHead,addLetter);
 	       /*dimiourgoume tis katigories me vasei to gramma pou edose o xristis*/
 	       createCategories(categHead,letter,wordLen);
 	       /*printCategories(categHead);*/
 	       /*epilegoume mia katigoria apo tis iparxouses*/
 	       chooseCategory(&categHead);
     
 	       currentTurn++;
 	       /*elegxoume an exei sximatistei i leksi*/
 	
	       ret=findWord(categHead,wordLen);

           if (ret) {/*i leksi sximatistike kai me vasei to flag "i=1" leme ston xristi parakato oti kerdise*/
               i=1;
               break;
           }
           scanf("%c",&temp);
       }
      
	   if (i) {/*o xristis mantepse sosta tin leksi*/
       		word=chooseWord(categHead);
        	printf("YOU WON! The word is %s\n",word);
       }
	   else{/*teliosan oi giroi kai o xristis dn mantepse sosta tin leksi*/
       		word=chooseWord(categHead);
        	printf("YOU LOST! The word was %s\n",word);
       }
       /*katharizoume tis listes kai rotame stin sinexeia an o xristis thelei na ksanapaiksei*/
      
	   clearLists(&usedHead,&unusedHead,&categHead);
       printf("Play again? (y/n): ");
       scanf("%c",&temp);
       if (temp=='y' || temp=='Y') {
           continue;
       }
	   else if(temp=='n' || temp=='N') {
           break;
       }
	   else{
           /*mesa apo ayto to loop zitame apo ton xristi na dosei sosta tin apantisi an thelei na ksanapaiksei i oxi*/
           while(1){
           		printf("Please enter y for yes or n for no\n");
          		printf("Play again? (y/n): ");
          		scanf("%c",&temp);
          		i=0;
          		if (temp=='y' || temp=='Y') {
            		i=1;
            		break;
          		} 
				else if(temp=='n' || temp=='N') {
	            	i=0;
            		break;
          		}
				else{
            		continue;
          		}
           }
           
		   if (i==1) {
          		continue;
           }
		   else {
          		break;
           }
       }
   }
}
