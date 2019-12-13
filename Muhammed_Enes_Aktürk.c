#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <locale.h>
#include <math.h>

#define PATH_NAME 100
#define TXT_SIZE 20
#define DOCUMENTNUMBER 3
/*
	Muhammed Enes AKTÜRK  150117036
	path --> line 68 
*/

struct MLL{
	int documentAndTimes[DOCUMENTNUMBER][2]; 
	int times ;
	int txt[20];
	char words[TXT_SIZE];
	struct MLL * next ;
	struct First_Order * first_order  ;
	struct Second_Order * second_order ;
	struct Third_Order * third_order ;
};

struct First_Order{
	struct MLL  * word ;
	struct First_Order * next ;
};

struct Second_Order{
	struct MLL * word ;
	struct Second_Order * next ;
};

struct Third_Order{
	struct MLL * word ;
	struct Third_Order * next ;
};

typedef struct MLL master_linked_list ;
typedef struct First_Order first_order ;
typedef struct Second_Order second_order ;
typedef struct Third_Order third_order ;

master_linked_list * controlAndInsert(master_linked_list * root , char word[TXT_SIZE] , int document_number , int txt_number);
void bastir(master_linked_list * root);
void first_ekle(master_linked_list * root , master_linked_list * temp , int txt_number);
int first_bastir(master_linked_list * iter ,first_order * first_iter,int first_sayisi);
void second_ekle(master_linked_list * root ,master_linked_list * temp , int txt_number);
int second_bastir(master_linked_list * iter,second_order * second_iter,int first_sayisi);
void third_ekle	(master_linked_list * root,master_linked_list * temp ,int txt_number);
int third_bastir(master_linked_list * iter, third_order * third_iter,int third_sayisi);
void findMostTen(master_linked_list * root , int txt_number , int arr[][2]);

int main() {
	
	setlocale(LC_ALL,"Turkish");
	
	master_linked_list * root ;
	root = NULL ;
	DIR * F1 ;
	DIR * F2 ;
 	struct dirent * top_file ;
    struct dirent * in_file ;
    
    // PATH FOR FILES *************************************************************************************************************
    char    path_name[PATH_NAME] = "small dataset";
    
	F1 = opendir(path_name);
	
	if(F1 == NULL){
		printf("File path name is not correct.");
		return 1 ;
	}
	int docandtxt[DOCUMENTNUMBER][2] ;
	int i ;
	for(i=0 ; i<DOCUMENTNUMBER ; i++){
		docandtxt[i][0]=1;
		docandtxt[i][1]=0;		
	}
	int file_number = 0;
	int txt_number = 0;
	while( top_file=readdir(F1) ){
		if(!strcmp(top_file->d_name,".")){
			continue ;
		}
		if(!strcmp(top_file->d_name,"..")){
			continue ;
		}
		file_number++;
		char file_path_name[PATH_NAME] = "" ; 
		strcat(file_path_name,path_name);
		strcat(file_path_name,"//");
		strcat(file_path_name,top_file->d_name);
		F2 = opendir(file_path_name);
		if(F2 == NULL){
			break ;
		}
		while(in_file=readdir(F2)){
			if(!strcmp(in_file->d_name,".")){
				continue ;
			}
			if(!strcmp(in_file->d_name,"..")){
				continue ;
			}
			FILE * file ;
			char txt_path_name[PATH_NAME] ;
			strcpy(txt_path_name,file_path_name);
			strcat(txt_path_name,"\\");
			strcat(txt_path_name,in_file->d_name);
			file = fopen(txt_path_name,"r");
			if(file==NULL){
				break;
			}
			txt_number++;
			char word[TXT_SIZE];
			while(fscanf(file,"%s",word)==1){
				root = controlAndInsert(root,word,file_number,txt_number);		
			}
			docandtxt[file_number-1][1]++;				
		}
	}

	closedir(F1);
	closedir(F2);

	master_linked_list * iter ;
	printf("First order : \n");
	iter=root;
	while(iter->next!=NULL){
		first_ekle(root,iter,txt_number);
		iter=iter->next;
	}
	first_ekle(root,iter,txt_number);
	
	printf("\nSecond order : \n");
	iter=root;
	while(iter->next!=NULL){
		second_ekle(root,iter,txt_number);
		iter=iter->next;
	}
	second_ekle(root,iter,txt_number);

	printf("\nThird order : \n");
	iter=root;
	while(iter->next!=NULL){
		third_ekle(root,iter,txt_number);
		iter=iter->next;
	}
	third_ekle(root,iter,txt_number);

	findMostTen(root,txt_number,docandtxt);
	
}

master_linked_list * controlAndInsert(master_linked_list * root , char word[TXT_SIZE] , int document_number , int txt_number){
	if(root==NULL){
		root = (master_linked_list*)malloc(sizeof(master_linked_list)) ;
		int i;
		for(i=0; i<DOCUMENTNUMBER ; i++){
			root->documentAndTimes[i][0]=0;
			root->documentAndTimes[i][1]=0;
		}
		strcpy(root->words,word);	
		root->txt[txt_number-1]=1;
		root->documentAndTimes[document_number-1][0]=1;
		root->documentAndTimes[document_number-1][1]=1;
		root->next=NULL;
		root->first_order=NULL;
		root->second_order=NULL;
		root->third_order=NULL;
		root->times=1;
		return root ;
	}
	else{
		master_linked_list * iter ;
		iter = root ;
		while(iter->next!=NULL){
			if(strcmp(iter->words,word)==0){
				iter->txt[txt_number-1]=1 ;
				iter->documentAndTimes[document_number-1][0]=1;
				iter->documentAndTimes[document_number-1][1]++;
				iter->times++;
				return root ;
			}
			iter=iter->next;
		}
		if(strcmp(iter->words,word)==0){
				iter->txt[txt_number-1]=1;
				iter->documentAndTimes[document_number-1][0]=1;
				iter->documentAndTimes[document_number-1][1]++ ;
				iter->times++;
				return root ;
		}
		else{
			iter->next=(master_linked_list*)malloc(sizeof(master_linked_list));
			int i ;
			for(i=0; i<DOCUMENTNUMBER ; i++){
				iter->next->documentAndTimes[i][0]=0;
				iter->next->documentAndTimes[i][1]=0;
			}
			strcpy(iter->next->words,word);
			iter->next->txt[txt_number-1]=1;
			iter->next->times=1;
			iter->next->documentAndTimes[document_number-1][0]=1;
			iter->next->documentAndTimes[document_number-1][1]=1;
			iter->next->next=NULL;
			iter=iter->next ;
			iter->first_order=NULL;
			iter->second_order=NULL;
			iter->third_order=NULL;
			return root ;
		}
		
	}
	
}

void first_ekle(master_linked_list * root , master_linked_list * temp , int txt_number){
	
	master_linked_list * a ;
	a=root ;
	temp->first_order=NULL;
	first_order * first;
	while(a!=NULL){
		if(strcmp(a->words,temp->words)==0)
			a=a->next;
		if(a==NULL)
			break;
		int i;
		for(i=0 ; i<txt_number; i++){
			if(a->txt[i]==1 && temp->txt[i]==1){
				int kontrol = 0 ;
				first_order * control_first ;
				if(a->first_order!=NULL){
					control_first=a->first_order;
					while(control_first!=NULL){
						if(strcmp(control_first->word->words,temp->words)==0){
							kontrol=1;
							break;
						}
						control_first=control_first->next;
					}						
				}
				if(kontrol==0){
					if(temp->first_order==NULL){
						temp->first_order=(first_order*)malloc(sizeof(first_order));
						temp->first_order->word = a ;
						temp->first_order->next = NULL ;
						first = temp->first_order ;
						printf("{%s,%s},",temp->words,a->words);
					}
					else{
						first->next=(first_order*)malloc(sizeof(first_order));
						first->next->word = a ;
						first->next->next = NULL;
						first=first->next;
						printf("{%s,%s},",temp->words,a->words);
					}
				}
				break;			
			}
		}
		a=a->next;
	}
		
}

int first_bastir(master_linked_list * iter ,first_order * first_iter,int first_sayisi){
	
	while(first_iter->next!=NULL){
		printf("{%s,%s} , ",iter->words,first_iter->word->words);
		first_sayisi++;
		first_iter=first_iter->next;
	}
	printf("{%s,%s} , ",iter->words,first_iter->word->words);

}

void second_ekle(master_linked_list *root , master_linked_list * temp , int txt_number){
	
	master_linked_list * a ;
	a=root ;
	temp->first_order=NULL;
	first_order * first;
	while(a!=NULL){
		if(strcmp(a->words,temp->words)==0)
			a=a->next;
		if(a==NULL)
			break;
		int i;
		for(i=0 ; i<txt_number; i++){
			if(a->txt[i]==1 && temp->txt[i]==1){
				if(temp->first_order==NULL){
					temp->first_order=(first_order*)malloc(sizeof(first_order));
					temp->first_order->word = a ;
					temp->first_order->next = NULL ;
					first = temp->first_order ;
				}
				else{
					first->next=(first_order*)malloc(sizeof(first_order));
					first->next->word = a ;
					first->next->next = NULL;
					first=first->next;
				}
				break;			
			}
		}
		a=a->next;
	}
		
	first_order *iter ;
	iter=NULL;
	if(temp->first_order!=NULL)
		iter=temp->first_order;
	
	first_order * bakilan ;
	bakilan = NULL ;
	second_order  * sec_iter , * control_sec ;
	first_order * control_first ;
	temp->second_order=NULL;
	while(iter!=NULL){	
		if(iter->word->first_order!=NULL)
			bakilan=iter->word->first_order ;		
		while(bakilan!=NULL){
			int a=0; 
			control_first = temp->first_order;
			while(control_first!=NULL){
				if(strcmp(control_first->word->words,bakilan->word->words)==0){
					a=1;
					break;
				}
				control_first=control_first->next;
			}
			if(a==0){
				control_sec = temp->second_order;
				while(control_sec!=NULL){
					if(strcmp(control_sec->word->words,bakilan->word->words)==0){
						a=1;
						break;
					}
					control_sec=control_sec->next;
				}				
			}
			if(a==0){
				control_sec = bakilan->word->second_order ;
				while(control_sec!=NULL){
					if(strcmp(control_sec->word->words,temp->words)==0){
						a=1;
						break;
					}
					control_sec=control_sec->next;
				}				
			}	
			if(strcmp(temp->words,bakilan->word->words)==0){
				a=1;
			}		
			if(a==0){
				if(temp->second_order==NULL){
					temp->second_order=(second_order*)malloc(sizeof(second_order));
					temp->second_order->word = bakilan->word ;
					temp->second_order->next = NULL ;
					sec_iter = temp->second_order ;
					printf("{%s,%s},",temp->words,bakilan->word->words);
				}
				else{
					sec_iter->next=(second_order*)malloc(sizeof(second_order));
					sec_iter->next->word = bakilan->word ;
					sec_iter->next->next = NULL;
					sec_iter=sec_iter->next;
					printf("{%s,%s},",temp->words,bakilan->word->words);
				}			
			}
			bakilan=bakilan->next ;
		}
		iter=iter->next;
	}

}

int second_bastir(master_linked_list * iter, second_order * second_iter,int second_sayisi){
	
	while(second_iter->next!=NULL){
		printf("{%s,%s} , ",iter->words,second_iter->word->words);
		second_sayisi++;
		second_iter=second_iter->next;
	}
	printf("{%s,%s} , ",iter->words,second_iter->word->words);
	
}

void third_ekle(master_linked_list * root ,master_linked_list * temp ,int txt_number){

	first_order *iter ;
	iter=NULL;
	if(temp->first_order!=NULL)
		iter=temp->first_order;
	
	first_order * bakilan , *control_first ;
	bakilan = NULL ;
	second_order  * sec_iter , * control_sec ;
	temp->second_order=NULL;
	
	while(iter!=NULL){	
		if(iter->word->first_order!=NULL)
			bakilan=iter->word->first_order ;		
		while(bakilan!=NULL){
			int a=0; 
			control_first = temp->first_order;
			while(control_first!=NULL){
				if(strcmp(control_first->word->words,bakilan->word->words)==0){
					a=1;
					break;
				}
				control_first=control_first->next;
			}
			control_sec = temp->second_order;
			while(control_sec!=NULL){
				if(strcmp(control_sec->word->words,bakilan->word->words)==0){
					a=1;
					break;
				}
				control_sec=control_sec->next;
			}	
			if(strcmp(temp->words,bakilan->word->words)==0){
				a=1;
			}		
			if(a==0){
				if(temp->second_order==NULL){
					temp->second_order=(second_order*)malloc(sizeof(second_order));
					temp->second_order->word = bakilan->word ;
					temp->second_order->next = NULL ;
					sec_iter = temp->second_order ;
				}
				else{
					sec_iter->next=(second_order*)malloc(sizeof(second_order));
					sec_iter->next->word = bakilan->word ;
					sec_iter->next->next = NULL;
					sec_iter=sec_iter->next;
				}			
			}
			bakilan=bakilan->next ;
		}
		iter=iter->next;
	}
	temp->third_order=NULL;
	first_order * bakilan_first  ;
	second_order * bakilan_second ;
	third_order * control_third , *iter_third ;
	bakilan_second=NULL;
	control_third=NULL;
	iter_third=NULL;
	if(temp->second_order!=NULL)
		bakilan_second=temp->second_order;	
	while(bakilan_second!=NULL){
		bakilan_first=NULL;
		if(bakilan_second->word->first_order!=NULL)
			bakilan_first=bakilan_second->word->first_order;	
		while(bakilan_first!=NULL){		
			if(temp->third_order==NULL){
				int a=0;
				control_first = temp->first_order ;
				while(control_first!=NULL){
					if(strcmp(control_first->word->words,bakilan_first->word->words)==0){
						a=1;
						break;
					}
					control_first=control_first->next;
				}
				if(a==0){
					control_third = bakilan_first->word->third_order;
					while(control_third!=NULL){
						if(strcmp(control_third->word->words,temp->words)==0){
							a=1;
							break;
						}
						control_third=control_third->next;
					}	
				}				
				control_sec = temp->second_order ;
				while(control_sec!=NULL){
					if(strcmp(control_sec->word->words,bakilan_first->word->words)==0){
						a=1;
						break;
					}
					control_sec=control_sec->next;
				}	
				if(strcmp(temp->words,bakilan_first->word->words)!=0 && a==0){
					temp->third_order=(third_order*)malloc(sizeof(third_order));
					temp->third_order->word = bakilan_first->word ;
					temp->third_order->next = NULL ;
					iter_third = temp->third_order ;
					printf("{%s,%s},",temp->words,bakilan_first->word->words);					
				}
			}
			else{
				int a = 0 ;
				control_third = temp->third_order ;
				while(control_third!=NULL){
					if(strcmp(control_third->word->words,bakilan_first->word->words)==0 || strcmp(temp->words,bakilan_first->word->words)==0){
						a=1;
						break;
					}
					control_third=control_third->next;
				}
				if(a==0){
					control_third = bakilan_first->word->third_order;
					while(control_third!=NULL){
						if(strcmp(control_third->word->words,temp->words)==0){
							a=1;
							break;
						}
						control_third=control_third->next;
					}	
				}
				if(a==0){
					control_first = temp->first_order ;
					while(control_first!=NULL){
						if(strcmp(control_first->word->words,bakilan_first->word->words)==0){
							a=1;
							break;
						}
						control_first=control_first->next;
					}					
				}
				if(a==0){
					control_sec = temp->second_order ;
					while(control_sec!=NULL){
						if(strcmp(control_sec->word->words,bakilan_first->word->words)==0){
							a=1;
							break;
						}
						control_sec=control_sec->next;
					}					
				}
				if(a==0){
					iter_third->next=(third_order*)malloc(sizeof(third_order));
					iter_third->next->word = bakilan_first->word ;
					iter_third->next->next = NULL;
					iter_third=iter_third->next;
					printf("{%s,%s},",temp->words,bakilan_first->word->words);					
				}
			}
			bakilan_first=bakilan_first->next;
			if(bakilan_first==NULL)
				break;
		}
		bakilan_second=bakilan_second->next;
		if(bakilan_second==NULL)
			break;
	}
}

int third_bastir(master_linked_list * iter, third_order * third_iter , int third_sayisi){
	if(third_iter!=NULL){
		while(third_iter->next!=NULL){
			printf("{%s,%s} , ",iter->words,third_iter->word->words);
			third_sayisi++;
			third_iter=third_iter->next;
		}
		printf("{%s,%s} , ",iter->words,third_iter->word->words);
	}
}

void findMostTen(master_linked_list * root , int txt_number , int arr[][2]){
	
	master_linked_list * iter ;
	iter = root ;
	master_linked_list * most_word ;
	char word_arr[DOCUMENTNUMBER*10][20];
	int word_times[DOCUMENTNUMBER*10];
	double idf_value[DOCUMENTNUMBER*10];
	int file ;
	for(file=0 ; file<DOCUMENTNUMBER ; file++){
		int i ;
		for(i=0 ; i<5 ; i++){
			int q=0;
			most_word=root;
			int most ;
			most = 0 ;
			while(iter!=NULL){
				int k=0;
				if(most <= iter->documentAndTimes[file][1]){
					int a ;
					for(a=(file*5); a<((file+1)*5) ; a++){
						if(strcmp(word_arr[a],iter->words)==0){
							k=1;
							break;
						}
					}
					if(k==0){
						q=iter->documentAndTimes[file][1];
						most_word=iter;
						most = iter->documentAndTimes[file][1];
					}
				}
				iter=iter->next;
				if(iter==NULL)
					break;
			}
			strcpy(word_arr[i+(file*5)],most_word->words);
			word_times[i+(file*5)]=q;
			int file_value = 0 ;
			double txtSayisi = arr[file][1];
			int doc ;
			if(file==0){
				for(doc=0 ; doc<arr[0][1] ; doc++){
					file_value += most_word->txt[doc] ;
				}	
			}
			else if(file==1){
				for(doc=arr[0][1] ; doc<arr[0][1]+arr[1][1] ; doc++){
					file_value += most_word->txt[doc] ;
				}
			}
			else if(file==2){
				for(doc=arr[1][1] ; doc<arr[0][1]+arr[1][1]+arr[2][1] ; doc++){
					file_value += most_word->txt[doc] ;
				}
			}
			idf_value[i+(file*5)]=q*log(txtSayisi/file_value);
			iter=root;
		}
	}	
	
	printf("\n\n");
	int i ;	
	for(i=0; i<DOCUMENTNUMBER*5 ;i++){
		if(i==0 || i==5 || i==10){
			printf("\nTF VALUE FOR FILE %d\n\n",(i/5)+1);
		}
		printf("%s;%d\n",word_arr[i],word_times[i]);
	}
	
	for(i=0 ; i<3 ; i++){
		int j ;
		for(j=i*5 ; j<(i+1)*5-1 ; j++){
			if(idf_value[j]<idf_value[j+1]){
				double temp = idf_value[j];
				idf_value[j] = idf_value[j+1] ;
				idf_value[j+1] = temp ;
				char word[20] = "";
				strcat(word,word_arr[j]);
				strcpy(word_arr[j],word_arr[j+1]);
				strcpy(word_arr[j+1],word);
				j=i*5 - 1 ;
			}
		}
	}
	printf("\n\n");
	for(i=0; i<DOCUMENTNUMBER*5 ;i++){
		if(i==0 || i==5 || i==10){
			printf("\nIDF VALUE FOR FILE %d\n\n",(i/5)+1);
		}
		printf("%s; %lf\n",word_arr[i],idf_value[i]);
	}
		
}

