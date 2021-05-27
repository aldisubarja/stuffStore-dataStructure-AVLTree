#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct data{
	char name[50];
	int value;
	int height;
	struct data *left, *right;
};

struct data *newData(char name[], int value){
	struct data *temp = (struct data*)malloc(sizeof(struct data));
	strcpy(temp->name,name);
	temp->value = value;
	temp->height = 1;
	temp->left = temp->right = NULL;
	return temp;
}

int getHeight(struct data *root){
	if(root==NULL) return 0;
	return root->height;
}

int max(int a,int b){
	if(a>b) return a;
	else return b;
}

int getBF(struct data *root){
	if(root == NULL) return 0;
	return getHeight(root->left) - getHeight(root->right);
}

struct data *rightRotate(struct data *y){
	struct data *x = y->left;
	struct data *bwh = x->right;
	
	x->right = y;
	y->left = bwh;
	
	y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
	x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
	return x;
}

struct data *leftRotate(struct data *x){
	struct data *y = x->right;
	struct data *bwh = x->left;
	
	y->left = x;
	x->right = bwh;
	
	x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
	y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
	return y;
}

struct data *rebalance(struct data *root){
	root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
	int currBalance = getBF(root);
	
	if(currBalance > 1 && getBF(root->left) >= 0) return rightRotate(root);
	if(currBalance > 1 && getBF(root->left) < 0 ){
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}
	
	if(currBalance < -1 && getBF(root->right) <= 0) return leftRotate(root);
	if(currBalance < -1 && getBF(root->right) > 0 ){
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}	
	return root;
	
}

struct data *insert(struct data *root, char name[] ,int value){
	if(root==NULL)return newData(name,value);
	else if(root->value > value) root->left = insert(root->left,name,value);
	else if(root->value < value) root->right = insert(root->right,name,value);
	else return root;
	
	return rebalance(root);
}

struct data *updateName(struct data *root, char name[]){
	if(root==NULL)return root;
	if(strcmp(root->name,name)==0){
			int flag1=0;
			int flag2=0;
			char newName[50];
			do{
				flag1=0;
				flag2=0;
				printf("Input New Name [Must contains alphabet and numeric]: ");
				scanf("%[^\n]",newName); getchar();
				for(int i=0;i<strlen(name);i++){
					if(name[i]>='0'&&name[i]<='9'){
						flag1++;
					}
					if((name[i]>='A'&&name[i]<='Z')||(name[i]>='a'&&name[i]<='z')){
						flag2++;
					}
				}
			}while(flag1==0||flag2==0);
			strcpy(root->name,newName);
			return root;
	}
	else return root;
	root->left = updateName(root,name);
	root->right = updateName(root,name);
}

void inorder(struct data *root){
	if(root!=NULL){
		inorder(root->left);
		printf("Name : %s\nValue : %d\nHeight : %d\n\n",root->name ,root->value, root->height);
		inorder(root->right);
	}
}

struct data *deleteAll(struct data *root){
	if(root != NULL){
		deleteAll(root->left);
		deleteAll(root->right);
		free(root);
	}
	return NULL;
}

struct data *succar(struct data *root){
	while(root && root->left) root = root->left;
	return root;
}

struct data *deletedata(struct data *root,int value){
	if(!root)return root;
	else if(root->value > value) root->left = deletedata(root->left, value);
	else if(root->value < value) root->right = deletedata(root->right, value);
	else {
		if(!root->left && !root->right){
			free(root);
			root = NULL;
		}
		else if(root->left && !root->right){
			struct data *del = root->left;
			*root = *(root->left);
			free(del);
		}
		else if(!root->left && root->right){
			struct data *del = root->right;
			*root = *(root->right);
			free(del);
		}
		else{
			struct data *suc = succar(root->right);
			root->value = suc->value;
			root->right = deletedata(root->right, root->value);
		}
	}
	if(!root) return 0;
	return rebalance(root);
}

void cls(){
	for(int i=0;i<30;i++){
		printf("\n");
	}
}

void menu(){
	struct data *root=NULL;
	int choose;
	
	do{
		cls();
		printf("Stuff Store\n");
		printf("1. Insert\n");
		printf("2. Delete by Value\n");
		printf("3. Update Name\n");
		printf("4. View\n");
		printf("5. Exit\n");
		printf(">> ");
		scanf("%d",&choose); getchar();
		if(choose==1){
			int flag1=0;
			int flag2=0;
			char name[50];
			do{
				flag1=0;
				flag2=0;
				printf("Input Name [Must contains alphabet and numeric]: ");
				scanf("%[^\n]",name); getchar();
				for(int i=0;i<strlen(name);i++){
					if(name[i]>='0'&&name[i]<='9'){
						flag1++;
					}
					if((name[i]>='A'&&name[i]<='Z')||(name[i]>='a'&&name[i]<='z')){
						flag2++;
					}
				}
			}while(flag1==0||flag2==0);
			int value;
			do{
				printf("Input Value [Must between 0 and 50000]: ");
				scanf("%d",&value); getchar();
			}while(value<0||value>50000);
			root = insert(root,name,value);
			inorder(root);
			getchar();
		}else if(choose==2){
			int value;
			printf("Input Value : ");
			scanf("%d",&value); getchar();
			root=deletedata(root,value);
		}else if(choose==3){
			char name[50];
			printf("Input name : ");
			scanf("%[^\n]",name); getchar();
			root=updateName(root,name);
		}else if(choose==4){
			inorder(root);
			getchar();
		}
	}while(choose!=5);
	root = deleteAll(root);
}

int main(){
	menu();
	return 0;
}
