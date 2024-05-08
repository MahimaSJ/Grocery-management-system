
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LOW_STOCK_ALERT_THRESHOLD 10
#define DAYS_BEFORE_EXPIRATION_ALERT 7

struct GroceryProduct {
    int productId;
    char name[50];
    char category[20];
    int quantity;
    float unitPrice;
    char expirationDate[15];
    struct GroceryProduct* next;
};

struct User {
    char username[20];
    char password[20];
};

struct GroceryProduct* inventoryHead = NULL;
struct GroceryProduct* inventoryTail = NULL;
struct User admin = {"admin", "adminpass"};
struct User staff={"staff","staffpass"};

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int authenticateUser(char enteredUsername[20],char enteredPassword[20]) {
   
    if (strcmp(admin.username, enteredUsername) == 0 && strcmp(admin.password, enteredPassword) == 0) {
        return 1;
    } else {
        return 0;
    }
}

int authenticateUser2(char enteredUsername[20],char enteredPassword[20]) {
    if (strcmp(staff.username, enteredUsername) == 0 && strcmp(staff.password, enteredPassword) == 0) {
        return 1;
    } else {
        return 0;
    }
}


void addProduct() {
    struct GroceryProduct* newProduct = (struct GroceryProduct*)malloc(sizeof(struct GroceryProduct));

    if (newProduct == NULL) {
        printf("Error: Unable to allocate memory for the new product.\n");
        return;
    }

    printf("Enter product name: ");
    scanf("%s", newProduct->name);
    printf("Enter product Id: ");
    scanf("%d",&newProduct->productId);
    printf("Enter product category: ");
    scanf("%s", newProduct->category);
    printf("Enter quantity: ");
    scanf("%d", &newProduct->quantity);
    printf("Enter unit price: ");
    scanf("%f", &newProduct->unitPrice);
    printf("Enter expiration date: ");
    scanf("%s", newProduct->expirationDate);

    //newProduct->productId = (inventoryTail == NULL) ? 1 : (inventoryTail->productId + 1);
    newProduct->next = NULL;

    if (inventoryHead == NULL) {
        inventoryHead = newProduct;
        inventoryTail = newProduct;
    } else {
        inventoryTail->next = newProduct;
        inventoryTail = newProduct;
    }

    printf("Product added successfully.\n");

    if (newProduct->quantity <= LOW_STOCK_ALERT_THRESHOLD) {
        printf("Low stock alert for %s! (Current Quantity: %d)\n", newProduct->name, newProduct->quantity);
    }
}

void sellProduct() {
    int productId;

    printf("Enter the ID of the product to sell: ");
    scanf("%d", &productId);
    clearBuffer();

    struct GroceryProduct* current = inventoryHead;
    struct GroceryProduct* prev = NULL;

    while (current != NULL && current->productId != productId) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Product not found with ID %d.\n", productId);
        return;
    }

    printf("Enter the quantity sold: ");
    int quantitySold;
    scanf("%d", &quantitySold);
    clearBuffer();

    if (quantitySold > current->quantity) {
        printf("Error: Quantity sold exceeds available quantity.\n");
        return;
    }

    current->quantity -= quantitySold;

    printf("Sale completed successfully.\n");

    if (current->quantity <= LOW_STOCK_ALERT_THRESHOLD) {
        printf("Low stock alert for %s! (Current Quantity: %d)\n", current->name, current->quantity);
    }

    if (current->quantity == 0) {
        if (prev == NULL) {
            inventoryHead = current->next;
            if (inventoryHead == NULL) {
                inventoryTail = NULL;
            }
        } else {
            prev->next = current->next;
            if (current->next == NULL) {
                inventoryTail = prev;
            }
        }

        free(current);
    }
}

void displayInventory() {
    struct GroceryProduct* current = inventoryHead;

    printf("\n========== Grocery Inventory ==========\n");
    printf("ID  | Name              | Category    | Quantity | Unit Price | Expiration Date\n");
    printf("--------------------------------------------------------------------------\n");

    while (current != NULL) {
        printf("%-3d | %-15s | %-10s | %-8d | $%-9.2f | %s\n",
               current->productId, current->name, current->category,
               current->quantity, current->unitPrice, current->expirationDate);

        current = current->next;
    }

    printf("=======================================\n");
}


void generateExpirationAlerts() {
    time_t currentTime = time(NULL);
    struct tm* localTime = localtime(&currentTime);

    printf("\n========== Expiration Alerts ==========\n");

    struct GroceryProduct* current = inventoryHead;

    while (current != NULL) {
        int day, month, year;
        sscanf(current->expirationDate, "%d-%d-%d", &day, &month, &year);

        struct tm expirationDate = {0};
        expirationDate.tm_year = year - 1900;
        expirationDate.tm_mon = month - 1;
        expirationDate.tm_mday = day;

        time_t expirationTime = mktime(&expirationDate);
        int daysDifference = difftime(expirationTime, currentTime) / (60 * 60 * 24);

        if (daysDifference <= DAYS_BEFORE_EXPIRATION_ALERT) {
            printf("Expiration alert for %s! (Days remaining: %d)\n", current->name, daysDifference+1);
        }

        current = current->next;
 }

    printf("=====================================\n");
}



void freeInventory() {
    struct GroceryProduct* current = inventoryHead;
    struct GroceryProduct* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    inventoryHead = NULL;
    inventoryTail = NULL;
}


struct bill{
	char *pname;
	int price;
	int quantity;
	struct bill *next;
};
typedef struct bill *Bill;



void insertproduct(Bill b,char *pname,int price,int quantity){
		Bill prod=(struct bill*)malloc(sizeof(struct bill));
		strcpy(prod->pname,pname);
		prod->price=price;
		prod->quantity=quantity;
		Bill temp=b;
		while(temp->next!=NULL){
			temp=temp->next;
		}
		temp->next=prod;
		prod->next=NULL;
	
}

void deleteproduct(Bill b,char *pname){
		if(strcmp(b->pname,pname)==0){
			Bill temp=b->next;
			b->next=temp->next;
			free(temp);
			return;
		}else{
			Bill temp=b;Bill preptr;
			while(temp->next!=NULL){
				preptr=temp;
				temp=temp->next;
			}
			if(strcmp(temp->pname,pname)==0){
				preptr->next=NULL;
				free(temp);
				return;
			}
			Bill temp2=b;Bill preptr2;
			while(temp2->next!=NULL && strcmp(temp->pname,pname)!=0){
				preptr2=temp2;
				temp2=temp2->next;
			}
			preptr2->next=temp2->next;
			free(temp2);
			return;
		}
}

int amount(Bill b){
	int res;
	Bill temp=b;
	while(temp!=NULL){
		res+=((temp->quantity)*(temp->price));
		temp=temp->next;
	}
	return res;
}

void generatebill(Bill b){
	printf("\n----------------------------");
	printf("\n        BILL            ");
	printf("\n-----------------------------");
	printf("\nProduct  | price  |quantity ");
	printf("\n------------------------------");
	Bill temp=b->next;
	while(temp!=NULL){
		printf("\n%-10s|%-10d|%-10d",
		temp->pname,temp->price,temp->quantity);
		temp=temp->next;
	}
	printf("\n------------------------------");
	printf("\n Total amount  =  %d",amount(b));
	printf("\n------------------------------");
}

int main() {
	char enteredUsername[20], enteredPassword[20];

    printf("Enter username: ");
    scanf("%s", enteredUsername);
    clearBuffer();
    printf("Enter password: ");
    scanf("%s", enteredPassword);
    clearBuffer();
    if (authenticateUser(enteredUsername,enteredPassword)) {
        int choice;

        do {
            printf("\n========== Grocery Stock Management System ==========\n");
            printf("1. Add Product\n");
            printf("2. Sell Product\n");
            printf("3. Display Inventory\n");
            printf("4. Generate Expiration Alerts\n");
            printf("5. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            clearBuffer();

            switch (choice) {
            case 1:
                addProduct();
                break;
            case 2:
                sellProduct();
                break;
            case 3:
                displayInventory();
                break;
            case 4:
                generateExpirationAlerts();
                break;
            case 5:
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
            }
        } while (choice != 5);

        freeInventory();
    }else if(authenticateUser2(enteredUsername,enteredPassword)){
		Bill b;char name[20];int price,quantity,op;
		b=(struct bill*)malloc(sizeof(struct bill));
		b->next=NULL;
		do{
			printf("\n----------------------------");
			printf("\n===== BILL GENERATION=====");
			printf("\n----------------------------");
			printf("\n 1.Add product");
			printf("\n 2.Remove product");
			printf("\n 3.Generate bill");
			printf("\n 4.Exit");
			printf("\n----------------------------");
			printf("\n Enter option: ");
			scanf("%d",&op);
			switch(op){
				case 1:
					printf("\n Product name:");
					scanf("%s",&name);
					//fgets(name,sizeof(name),stdin);
					printf("\n Product Price:");
					scanf("%d",&price);
					printf("\n Product Quantity:");
					scanf("%d",&quantity);
					insertproduct(b,name,price,quantity);
					printf("\n product added!!!");
					break;
				case 2:
					printf("\n Enter product name to remove:");
					scanf("%s",&name);
					//fgets(name,sizeof(name),stdin);
					deleteproduct(b,name);
					printf("\n Product removed!!!");
					break;
				case 3:
					generatebill(b);
					break;
				
			}
		}while(op!=4);
		Bill temp=b;Bill preptr;
		while(temp->next!=NULL){
			preptr=temp;
			temp=temp->next;
			free(preptr);
		}
	}else{
		printf("\n Authentication failed!!!");
		printf("\n Invalid Username and password!!!");
	}

    return 0;
}
