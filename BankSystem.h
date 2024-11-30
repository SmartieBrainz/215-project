struct operation
{
    char op; // deposit 'd', withdraw 'w'
    double amount;
};

struct account
{
    long accNum;
    double balance;
    char holderName[50];
    char holderEmail[100];
    int numOfOper;
    struct operation oper[100];
};

void addAccount(const char *);
void updateAccount(const char *);
void deleteAccount(const char *);
void deleteHolderAccounts(const char *);
void searchAccount(const char*);
int displayAllAccounts(const char *);
void addOperation(const char*);
int checkName(const char *);
int checkEmail(const char *);
int checkAmount(double);