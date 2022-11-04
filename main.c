#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RED 0
#define BLACK 1
struct tree{
	int data,color;
	struct tree *parent;
	struct tree *left;
	struct tree *right;
};

int height(struct tree *root){
	if(root == NULL) return 0;
	else{
		int l = height(root->left);
		int r = height(root->right);
		if(l>r) return l+1;
		else return r+1;
	}
}

void inorder(struct tree *root){
	if(root == NULL) return;
	inorder(root->left);
	printf("%d\n",root->data);
	inorder(root->right);
}

void print(struct tree *root,struct tree *proot,int level,int cur,int d,char *str){
	char *tmp = (char*)malloc(sizeof(char)*1000);
	if(root == NULL) return;
	if(cur == level){
	if(root->color == BLACK) sprintf(tmp,"%d B (%d %c) ",root->data,proot->data,d);
	else if(root->color == RED) sprintf(tmp,"%d R (%d %c) ",root->data,proot->data,d);
	strcat(str,tmp);
	}
	print(root->left,root,level,cur+1,'L',str);
	print(root->right,root,level,cur+1,'R',str);
	return;
}
void print_level(struct tree *root,int max){	
	printf("%d B\n",root->data);
	for(int i=1;i<max;i++){
		char *str = (char*)malloc(sizeof(char)*1000);
		print(root,root,i,0,'L',str);
		while(*(str+1) != '\0') {printf("%c",(*str)); str++;}
		printf("\n");
	}
}

void s1(struct tree *root,struct tree **ace);
void repaint(struct tree *root,struct tree **ace);
struct tree *RR(struct tree *root,struct tree *as){
	struct tree *temp = root;
	struct tree *temp2 = root->left->right;
	struct tree *tg = root->parent;
	root = root->left;
	root->right = temp;
	temp->left = NULL;
	temp->left = temp2;
	//printf("root = %d, root->right = %d, root->left = %d\n",root->data,root->right->data,root->left->data);
	return root;
}
struct tree *LL(struct tree *root,struct tree *as){
	struct tree *temp = root;
	struct tree *temp2 = root->right->left;
	root = root->right;
	struct tree *tg = root->parent;
	root->left = temp;
	temp->right = NULL;
	temp->right = temp2;
	return root;
}
struct tree *LR(struct tree *root){
	struct tree *temp = root;
	struct tree *temp2 = root->left;
	root->left = root->left->right;
	printf("-------\n");
	root->left->left = temp2;
	temp2->right = NULL;
	root = RR(root,root);
	root->right = temp;
	return root;
}

void repaint(struct tree *root,struct tree **ace){
	if(root == (*ace)) return;
	if(root->color == RED)root->color = BLACK;
	else root->color = RED;
}
struct tree *get_gu(struct  tree *root,struct tree **grand){
	*grand = root->parent->parent;
	if((*grand)->left == root->parent) return (*grand)->right;
	else return (*grand)->left;
}
void s5(struct tree *root,struct tree **ace){
	struct tree *grand;
	//printf("s5\n");
	get_gu(root,&grand);
	if(root == root->parent->left && root->parent == grand->left){
		printf("s5\n");
		if((*ace) == grand){
			grand = RR(grand,grand);
			root = root->parent->right;
			(*ace) = grand;
		}
		else{
			struct tree *temp = grand->parent;
			grand = RR(grand,grand);
			temp->left = NULL;
			temp->left = grand;
			root = root->parent->right;
		}
	repaint(root,ace);
	repaint(grand,ace);
	}
	else if(root== root->parent->right && root->parent == grand->right){
		if((*ace) == grand){
			grand = LL(grand,grand);
			root = root->parent->left;
			(*ace) = grand;
		}
		else{
			//LL(grand,grand);
			struct tree *temp = grand->parent;
			grand = LL(grand,grand);
			temp->right = NULL;
			temp->right = grand;
			root = root->parent->left;
		}
		repaint(root,ace);
	repaint(grand,ace);
	}

}

void s4(struct tree *root,struct tree **ace){
	struct tree *grand;
	struct tree *uncle = get_gu(root,&grand);
	//printf("s4\n");
	if(root == root->parent->right && grand->left == root->parent){
		if((*ace) == grand){
			printf("---n");
			struct tree *temp = root->parent;
			temp = LL(temp,temp);
			root = root->left;
			root->parent = temp;
			temp->parent = grand;
			grand->left = temp;
			repaint(grand,ace);
			temp = grand->parent;
			printf("%p\n",temp);
			grand = RR(grand,grand);
			grand->parent = temp;
			grand->left->parent = grand;
			grand->right->parent = grand;
			repaint(grand,ace);
			(*ace) = grand;
		}
		else{
			// TRUEEEEE
			//printf("root: %d\n",root->data);
			//inorder(*ace);
			//printf("---\n");
			struct tree *temp = root->parent;
			temp = LL(temp,temp);
			root = root->left;
			root->parent = temp;
			temp->parent = grand;		
			grand->left = temp;
			repaint(grand,ace);
			int yon=0;
			temp = grand->parent;
			if(temp->left == grand) yon =1;
			grand = RR(grand,grand);
			grand->parent = temp;
			grand->left->parent = grand;
			grand->right->parent = grand;
			if(yon)temp->left = grand;
			else temp->right = grand;
			repaint(grand,ace);
			//inorder(*ace);
		}
		return ;
	}
	else if(root == root->parent->left && grand->right == root->parent){
		printf("saşfas\n");
		if((*ace) == grand){
			struct tree *temp = root->parent;
			temp = RR(temp,temp);
			root->parent = temp;
			grand->right = temp;
			repaint(grand,ace);
			grand = LL(grand,grand);
			repaint(grand,ace);
			(*ace) = grand;
		}
		else{
			struct tree *temp = root->parent;
			temp = RR(temp,temp);
			root->parent = temp;		
			grand->right = temp;
			repaint(grand,ace);
			int yon=1;
			temp = grand->parent;
			if(temp->left == grand) yon =1;
			grand = LL(grand,grand);
			grand->left->parent = grand;
			grand->right->parent = grand;
			if(yon)temp->left = grand;
			else temp->right = grand;
			repaint(grand,ace);
		}
		return;
	}
	s5(root,ace);


}

void s3(struct tree *root,struct tree **ace){
	struct tree *grand;
	struct tree *uncle = get_gu(root,&grand);
	//printf("%d\n",root->parent->data);
	//printf("s3 uncle = %d\n",uncle->data);
	if(uncle != NULL && uncle->color == RED){
		repaint(uncle,ace);
		repaint(root->parent,ace);
		repaint(grand,ace);
		//inorder((*ace));
		s1(grand,ace);
	}
	else{
		s4(root,ace);
	}

}
void s2(struct tree *root,struct tree **ace){
	//printf("s2\n");
	if(root->parent->color == BLACK) return ;
	else s3(root,ace);
}
void s1(struct tree *root,struct tree **ace){
	//printf("s1\n");
	if(root->parent == NULL){
		repaint(root,ace);
	}else s2(root,ace);
}

void insert(struct tree **root,int data){
	//printf("data:%d\n",data);
	struct tree* newnode = (struct tree*)malloc(sizeof(struct tree));
	newnode->left = NULL;
	newnode->right = NULL;
	newnode->data = data;
	newnode->parent = NULL;
	newnode->color = 0;
	struct tree* cpy = (*root);
	struct tree* par = (*root);
	int cont = -1;
	while(cpy!= NULL){
		par = cpy;
		if(newnode->data < cpy->data){
		 cpy= cpy->left;
		cont =0;
		}
		else {
		cpy= cpy->right;
		cont =1;
		}
	 }
	cpy = newnode;
	cpy->parent = par;
	if(cont ==0)par->left = cpy;
	else if(cont == 1) par->right = cpy;
	s1(cpy,root);
	if((*root)!= NULL)
	return;
	else (*root) = cpy;
	return;


}


struct tree *search(struct tree *root,int data){
	if(root == NULL || root->data == data) return root;
	search(root->left,data);
	search(root->right,data);
}

// 56, 78, 23, 90, 1, 2, 3, 4, 5, 123, 45

int main()
{
	struct tree **root = (struct tree**)malloc(sizeof(struct tree*));
	*root = NULL;
	int id;
	do{
		scanf("%d",&id);
		if(id!= -1) insert(root,id);	
	}while(id != -1);
	//printf("%d l:  r: \n",(*root)->left->right->data,(*root)->left->right->left->data);
	printf("root = %d\n",(*root)->data);
	inorder((*root));
	printf("\n");
	int h = height((*root));
	print_level((*root),h);


    return 0;
}

