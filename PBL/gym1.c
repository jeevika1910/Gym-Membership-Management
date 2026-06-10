#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

#define MAX 1000

typedef struct {

    int id;
    char name[50];
    int age;
    char phone[15];

    char membership[20];
    char package[30];

    float fee;

    char payment[20];

    int expDate;

} Member;

typedef struct {

    char username[20];
    char password[20];

} Admin;

// ---------- UTILITY ----------

void clearBuffer() {

    while (getchar() != '\n');
}

void toLowerCase(char str[]) {

    for (int i = 0; str[i]; i++) {

        str[i] = tolower(str[i]);
    }
}

void inputPassword(char pass[]) {

    int i = 0;
    char ch;

    while ((ch = getch()) != 13) {

        if (ch == 8 && i > 0) {

            i--;
            printf("\b \b");
        }

        else {

            pass[i++] = ch;
            printf("*");
        }
    }

    pass[i] = '\0';
}

int makeDate(int d, int m, int y) {

    return y * 10000 + m * 100 + d;
}

void printDate(int date) {

    int y = date / 10000;
    int m = (date / 100) % 100;
    int d = date % 100;

    printf("%04d-%02d-%02d", y, m, d);
}

// ---------- VALIDATION ----------

int validName(char name[]) {

    for (int i = 0; name[i]; i++) {

        if (!isalpha(name[i]) && name[i] != ' ')
            return 0;
    }

    return 1;
}

int validPhone(char phone[]) {

    if (strlen(phone) != 10)
        return 0;

    for (int i = 0; i < 10; i++) {

        if (!isdigit(phone[i]))
            return 0;
    }

    return 1;
}

int validMembership(char membership[]) {

    char temp[20];

    strcpy(temp, membership);

    toLowerCase(temp);

    if (strcmp(temp, "monthly") == 0 ||
        strcmp(temp, "annual") == 0)
        return 1;

    return 0;
}

int validPayment(char payment[]) {

    char temp[20];

    strcpy(temp, payment);

    toLowerCase(temp);

    if (strcmp(temp, "paid") == 0 ||
        strcmp(temp, "unpaid") == 0)
        return 1;

    return 0;
}

int validDate(int d, int m, int y) {

    if (y < 2000 || y > 2100)
        return 0;

    if (m < 1 || m > 12)
        return 0;

    if (d < 1 || d > 31)
        return 0;

    return 1;
}

// ---------- ADMIN ----------

void initializeAdmin() {

    FILE *fp = fopen("admin.dat", "rb");

    if (fp == NULL) {

        fp = fopen("admin.dat", "wb");

        Admin a = {"admin", "1234"};

        fwrite(&a, sizeof(a), 1, fp);
    }

    fclose(fp);
}

int authenticate() {

    FILE *fp = fopen("admin.dat", "rb");

    if (fp == NULL)
        return 0;

    Admin a;

    fread(&a, sizeof(a), 1, fp);

    fclose(fp);

    char user[20];
    char pass[20];

    int attempts = 3;

    while (attempts--) {

        printf("\n=========== LOGIN ===========\n");

        printf("Username: ");
        scanf("%s", user);

        printf("Password: ");
        inputPassword(pass);

        if (strcmp(user, a.username) == 0 &&
            strcmp(pass, a.password) == 0) {

            printf("\nLogin Successful\n");

            return 1;
        }

        printf("\nInvalid Credentials\n");
        printf("Attempts Left: %d\n", attempts);
    }

    return 0;
}

// ---------- DUPLICATE ID ----------

int isDuplicateID(int id) {

    FILE *fp = fopen("members.dat", "rb");

    Member m;

    if (fp == NULL)
        return 0;

    while (fread(&m, sizeof(m), 1, fp)) {

        if (m.id == id) {

            fclose(fp);
            return 1;
        }
    }

    fclose(fp);

    return 0;
}

// ---------- ADD MEMBER ----------

void addMember() {

    FILE *fp = fopen("members.dat", "ab");

    if (fp == NULL) {

        printf("File Error\n");
        return;
    }

    Member m;

    int d, mo, y;

    int choice;

    printf("\n========== ADD MEMBER ==========\n");

    while (1) {

        printf("Enter ID: ");

        if (scanf("%d", &m.id) != 1) {

            printf("ID must contain numbers only\n");
            clearBuffer();
        }

        else if (m.id <= 0) {

            printf("ID must be positive\n");
        }

        else if (isDuplicateID(m.id)) {

            printf("ID already exists\n");
        }

        else {

            break;
        }
    }

    clearBuffer();

    while (1) {

        printf("Enter Name: ");

        fgets(m.name, sizeof(m.name), stdin);

        m.name[strcspn(m.name, "\n")] = '\0';

        if (strlen(m.name) == 0) {

            printf("Name cannot be empty\n");
        }

        else if (!validName(m.name)) {

            printf("Name must contain alphabets only\n");
        }

        else {

            break;
        }
    }

    while (1) {

        printf("Enter Age: ");

        if (scanf("%d", &m.age) != 1) {

            printf("Age must contain numbers only\n");
            clearBuffer();
        }

        else if (m.age <= 0 || m.age > 100) {

            printf("Enter age between 1-100\n");
        }

        else {

            break;
        }
    }

    while (1) {

        printf("Enter Phone Number: ");

        scanf("%s", m.phone);

        if (!validPhone(m.phone)) {

            printf("Phone number must be exactly 10 digits\n");
        }

        else {

            break;
        }
    }

    while (1) {

        printf("Membership Type (Monthly/Annual): ");

        scanf("%s", m.membership);

        if (!validMembership(m.membership)) {

            printf("Enter only Monthly or Annual\n");
        }

        else {

            toLowerCase(m.membership);

            if (strcmp(m.membership, "monthly") == 0)
                strcpy(m.membership, "Monthly");

            else
                strcpy(m.membership, "Annual");

            break;
        }
    }

    printf("\n========== PACKAGE MENU ==========\n");

    printf("1. Trial Package         - Rs.100\n");
    printf("2. Strength Training     - Rs.500\n");
    printf("3. Cardio                - Rs.400\n");
    printf("4. Yoga                  - Rs.300\n");
    printf("5. Strength + Cardio     - Rs.800\n");
    printf("6. Full Access           - Rs.1200\n");

    while (1) {

        printf("Select Package: ");

        if (scanf("%d", &choice) != 1) {

            printf("Enter numbers only\n");
            clearBuffer();
            continue;
        }

        switch (choice) {

            case 1:
                strcpy(m.package, "Trial");
                m.fee = 100;
                break;

            case 2:
                strcpy(m.package, "Strength");
                m.fee = 500;
                break;

            case 3:
                strcpy(m.package, "Cardio");
                m.fee = 400;
                break;

            case 4:
                strcpy(m.package, "Yoga");
                m.fee = 300;
                break;

            case 5:
                strcpy(m.package, "Strength+Cardio");
                m.fee = 800;
                break;

            case 6:
                strcpy(m.package, "Full Access");
                m.fee = 1200;
                break;

            default:
                printf("Invalid Package\n");
                continue;
        }

        break;
    }

    if (strcmp(m.membership, "Annual") == 0) {

        if (strcmp(m.package, "Trial") == 0) {

            printf("Trial Package not available for Annual Membership\n");

            fclose(fp);
            return;
        }

        m.fee = m.fee * 12 * 0.9;
    }

    while (1) {

        printf("Payment Status (Paid/Unpaid): ");

        scanf("%s", m.payment);

        if (!validPayment(m.payment)) {

            printf("Enter only Paid or Unpaid\n");
        }

        else {

            toLowerCase(m.payment);

            if (strcmp(m.payment, "paid") == 0)
                strcpy(m.payment, "Paid");

            else
                strcpy(m.payment, "Unpaid");

            break;
        }
    }

    while (1) {

        printf("Enter Expiry Date (DD MM YYYY): ");

        if (scanf("%d %d %d", &d, &mo, &y) != 3) {

            printf("Date must contain numbers only\n");
            clearBuffer();
        }

        else if (!validDate(d, mo, y)) {

            printf("Invalid Date\n");
        }

        else {

            break;
        }
    }

    m.expDate = makeDate(d, mo, y);

    fwrite(&m, sizeof(m), 1, fp);

    fclose(fp);

    printf("\nMember Added Successfully\n");
}

// ---------- DISPLAY ----------

void displayMembers() {

    FILE *fp = fopen("members.dat", "rb");

    Member m;

    if (fp == NULL) {

        printf("No Records Found\n");
        return;
    }

    printf("\n====================================================================================================================\n");

    printf("%-5s %-15s %-5s %-12s %-12s %-20s %-10s %-10s %-15s\n",
           "ID",
           "NAME",
           "AGE",
           "PHONE",
           "MEMBERSHIP",
           "PACKAGE",
           "FEE",
           "PAYMENT",
           "EXPIRY DATE");

    printf("====================================================================================================================\n");

    while (fread(&m, sizeof(m), 1, fp)) {

        printf("%-5d %-15s %-5d %-12s %-12s %-20s %-10.2f %-10s ",
               m.id,
               m.name,
               m.age,
               m.phone,
               m.membership,
               m.package,
               m.fee,
               m.payment);

        printDate(m.expDate);

        printf("\n");
    }

    fclose(fp);
}

// ---------- SEARCH BY ID ----------

void searchByID() {

    FILE *fp = fopen("members.dat", "rb");

    Member m;

    int id;
    int found = 0;

    if (fp == NULL) {

        printf("No Records Found\n");
        return;
    }

    printf("Enter Member ID: ");
    scanf("%d", &id);

    while (fread(&m, sizeof(m), 1, fp)) {

        if (m.id == id) {

            printf("\nMember Found\n");

            printf("ID: %d\n", m.id);
            printf("Name: %s\n", m.name);
            printf("Age: %d\n", m.age);
            printf("Phone: %s\n", m.phone);
            printf("Membership: %s\n", m.membership);
            printf("Package: %s\n", m.package);
            printf("Fee: %.2f\n", m.fee);
            printf("Payment: %s\n", m.payment);

            printf("Expiry Date: ");
            printDate(m.expDate);

            printf("\n");

            found = 1;
            break;
        }
    }

    if (!found)
        printf("Member Not Found\n");

    fclose(fp);
}

// ---------- SEARCH BY PHONE ----------

void searchByPhone() {

    FILE *fp = fopen("members.dat", "rb");

    Member m;

    char phone[15];

    int found = 0;

    if (fp == NULL) {

        printf("No Records Found\n");
        return;
    }

    printf("Enter Phone Number: ");
    scanf("%s", phone);

    while (fread(&m, sizeof(m), 1, fp)) {

        if (strcmp(m.phone, phone) == 0) {

            printf("\nMember Found\n");

            printf("ID: %d\n", m.id);
            printf("Name: %s\n", m.name);
            printf("Age: %d\n", m.age);
            printf("Phone: %s\n", m.phone);
            printf("Membership: %s\n", m.membership);
            printf("Package: %s\n", m.package);
            printf("Fee: %.2f\n", m.fee);
            printf("Payment: %s\n", m.payment);

            printf("Expiry Date: ");
            printDate(m.expDate);

            printf("\n");

            found = 1;
        }
    }

    if (!found)
        printf("No Member Found\n");

    fclose(fp);
}

// ---------- SEARCH MEMBER ----------

void searchMember() {

    FILE *fp = fopen("members.dat", "rb");

    Member m;

    char key[50];
    char temp1[50];
    char temp2[50];

    int found = 0;

    if (fp == NULL) {

        printf("No Records Found\n");
        return;
    }

    clearBuffer();

    printf("Enter Name or Package: ");

    fgets(key, sizeof(key), stdin);

    key[strcspn(key, "\n")] = '\0';

    toLowerCase(key);

    while (fread(&m, sizeof(m), 1, fp)) {

        strcpy(temp1, m.name);
        strcpy(temp2, m.package);

        toLowerCase(temp1);
        toLowerCase(temp2);

        if (strcmp(temp1, key) == 0 ||
            strcmp(temp2, key) == 0) {

            printf("\n%d %s %d %s %s %s %.2f %s ",
                   m.id,
                   m.name,
                   m.age,
                   m.phone,
                   m.membership,
                   m.package,
                   m.fee,
                   m.payment);

            printDate(m.expDate);

            printf("\n");

            found = 1;
        }
    }

    if (!found)
        printf("No Matching Record Found\n");

    fclose(fp);
}

// ---------- SORT ----------

void sortMembers() {

    FILE *fp = fopen("members.dat", "rb");

    Member m[MAX];
    Member temp;

    int n = 0;

    int i, j;

    int choice;

    if (fp == NULL) {

        printf("No Records Found\n");
        return;
    }

    while (fread(&m[n], sizeof(Member), 1, fp)) {

        n++;
    }

    fclose(fp);

    printf("\n1. Sort by Age\n");
    printf("2. Sort by Expiry Date\n");
    printf("3. Sort by Name\n");
    printf("4. Sort by ID\n");

    printf("Enter Choice: ");

    scanf("%d", &choice);

    for (i = 0; i < n - 1; i++) {

        for (j = i + 1; j < n; j++) {

            if ((choice == 1 && m[i].age > m[j].age) ||

                (choice == 2 && m[i].expDate > m[j].expDate) ||

                (choice == 3 && strcmp(m[i].name, m[j].name) > 0) ||

                (choice == 4 && m[i].id > m[j].id)) {

                temp = m[i];
                m[i] = m[j];
                m[j] = temp;
            }
        }
    }

    fp = fopen("members.dat", "wb");

    fwrite(m, sizeof(Member), n, fp);

    fclose(fp);

    printf("Records Sorted Successfully\n");
}

// ---------- REMOVE EXPIRED ----------

void removeExpired() {

    FILE *fp = fopen("members.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    Member m;

    int d, mo, y;

    int today;

    if (fp == NULL || temp == NULL) {

        printf("File Error\n");
        return;
    }

    while (1) {

        printf("Enter Today's Date (DD MM YYYY): ");

        if (scanf("%d %d %d", &d, &mo, &y) != 3) {

            printf("Invalid Input\n");
            clearBuffer();
        }

        else if (!validDate(d, mo, y)) {

            printf("Invalid Date\n");
        }

        else {

            break;
        }
    }

    today = makeDate(d, mo, y);

    while (fread(&m, sizeof(m), 1, fp)) {

        if (m.expDate >= today) {

            fwrite(&m, sizeof(m), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("members.dat");

    rename("temp.dat", "members.dat");

    printf("Expired Members Removed Successfully\n");
}

// ---------- RENEW MEMBERSHIP ----------

void renewMembership() {

    FILE *fp = fopen("members.dat", "rb+");

    Member m;

    int id;

    int d, mo, y;

    int found = 0;

    if (fp == NULL) {

        printf("No Records Found\n");
        return;
    }

    printf("Enter Member ID: ");

    scanf("%d", &id);

    while (fread(&m, sizeof(m), 1, fp)) {

        if (m.id == id) {

            found = 1;

            while (1) {

                printf("Enter New Expiry Date (DD MM YYYY): ");

                if (scanf("%d %d %d", &d, &mo, &y) != 3) {

                    printf("Invalid Input\n");
                    clearBuffer();
                }

                else if (!validDate(d, mo, y)) {

                    printf("Invalid Date\n");
                }

                else {

                    break;
                }
            }

            m.expDate = makeDate(d, mo, y);

            fseek(fp, -sizeof(m), SEEK_CUR);

            fwrite(&m, sizeof(m), 1, fp);

            printf("Membership Renewed Successfully\n");

            break;
        }
    }

    if (!found)
        printf("Member ID Not Found\n");

    fclose(fp);
}

// ---------- UPDATE MEMBER ----------

void updateMember() {

    FILE *fp = fopen("members.dat", "rb+");

    Member m;

    int id;

    int choice;

    int found = 0;

    if (fp == NULL) {

        printf("No Records Found\n");
        return;
    }

    printf("Enter Member ID: ");
    scanf("%d", &id);

    while (fread(&m, sizeof(m), 1, fp)) {

        if (m.id == id) {

            found = 1;

            printf("\n1. Update Phone\n");
            printf("2. Update Package\n");
            printf("3. Update Payment Status\n");

            printf("Enter Choice: ");
            scanf("%d", &choice);

            if (choice == 1) {

                while (1) {

                    printf("Enter New Phone Number: ");
                    scanf("%s", m.phone);

                    if (!validPhone(m.phone)) {

                        printf("Phone number must be exactly 10 digits\n");
                    }

                    else {

                        break;
                    }
                }
            }

            else if (choice == 2) {

                int p;

                printf("\n========== PACKAGE MENU ==========\n");

                printf("1. Trial Package         - Rs.100\n");
                printf("2. Strength Training     - Rs.500\n");
                printf("3. Cardio                - Rs.400\n");
                printf("4. Yoga                  - Rs.300\n");
                printf("5. Strength + Cardio     - Rs.800\n");
                printf("6. Full Access           - Rs.1200\n");

                printf("Select Package: ");
                scanf("%d", &p);

                switch (p) {

                    case 1:
                        strcpy(m.package, "Trial");
                        m.fee = 100;
                        break;

                    case 2:
                        strcpy(m.package, "Strength");
                        m.fee = 500;
                        break;

                    case 3:
                        strcpy(m.package, "Cardio");
                        m.fee = 400;
                        break;

                    case 4:
                        strcpy(m.package, "Yoga");
                        m.fee = 300;
                        break;

                    case 5:
                        strcpy(m.package, "Strength+Cardio");
                        m.fee = 800;
                        break;

                    case 6:
                        strcpy(m.package, "Full Access");
                        m.fee = 1200;
                        break;

                    default:
                        printf("Invalid Package\n");
                        fclose(fp);
                        return;
                }

                if (strcmp(m.membership, "Annual") == 0) {

                    if (strcmp(m.package, "Trial") == 0) {

                        printf("Trial Package not allowed for Annual Membership\n");
                        fclose(fp);
                        return;
                    }

                    m.fee = m.fee * 12 * 0.9;
                }
            }

            else if (choice == 3) {

                while (1) {

                    printf("Enter Payment Status (Paid/Unpaid): ");

                    scanf("%s", m.payment);

                    if (!validPayment(m.payment)) {

                        printf("Enter only Paid or Unpaid\n");
                    }

                    else {

                        toLowerCase(m.payment);

                        if (strcmp(m.payment, "paid") == 0)
                            strcpy(m.payment, "Paid");

                        else
                            strcpy(m.payment, "Unpaid");

                        break;
                    }
                }
            }

            else {

                printf("Invalid Choice\n");
                fclose(fp);
                return;
            }

            fseek(fp, -sizeof(m), SEEK_CUR);

            fwrite(&m, sizeof(m), 1, fp);

            printf("Member Updated Successfully\n");

            break;
        }
    }

    if (!found)
        printf("Member ID Not Found\n");

    fclose(fp);
}

// ---------- REPORT ----------

void generateReport() {

    FILE *fp = fopen("members.dat", "rb");

    Member m;

    int total = 0;
    int active = 0;
    int paid = 0;
    int unpaid = 0;

    int d, mo, y;

    int today;

    if (fp == NULL) {

        printf("No Records Found\n");
        return;
    }

    printf("Enter Today's Date (DD MM YYYY): ");

    scanf("%d %d %d", &d, &mo, &y);

    today = makeDate(d, mo, y);

    while (fread(&m, sizeof(m), 1, fp)) {

        total++;

        if (m.expDate >= today)
            active++;

        if (strcmp(m.payment, "Paid") == 0)
            paid++;

        else
            unpaid++;
    }

    fclose(fp);

    printf("\n========== REPORT ==========\n");

    printf("Total Members  : %d\n", total);
    printf("Active Members : %d\n", active);
    printf("Paid Members   : %d\n", paid);
    printf("Unpaid Members : %d\n", unpaid);
}

// ---------- RESET ----------

void resetData() {

    char choice;

    printf("\nWARNING: This will delete all records.\n");

    printf("Continue? (Y/N): ");

    scanf(" %c", &choice);

    if (choice == 'Y' || choice == 'y') {

        remove("members.dat");
        remove("temp.dat");
        remove("admin.dat");

        printf("All Data Deleted Successfully\n");
        printf("Restart Program\n");

        exit(0);
    }

    else {

        printf("Reset Cancelled\n");
    }
}

// ---------- MAIN ----------

int main() {

    int choice;

    initializeAdmin();

    if (!authenticate())
        return 0;

    do {

        printf("\n====================================\n");
        printf("       GYM MANAGEMENT SYSTEM\n");
        printf("====================================\n");

        printf("1. Add Member\n");
        printf("2. Display Members\n");
        printf("3. Search by ID\n");
        printf("4. Search by Name/Package\n");
        printf("5. Search by Phone Number\n");
        printf("6. Sort Members\n");
        printf("7. Remove Expired Members\n");
        printf("8. Renew Membership\n");
        printf("9. Update Member Details\n");
        printf("10. Generate Report\n");
        printf("11. Reset Data\n");
        printf("0. Exit\n");

        printf("====================================\n");

        printf("Enter Choice: ");

        if (scanf("%d", &choice) != 1) {

            printf("Enter numbers only\n");
            clearBuffer();
            continue;
        }

        switch (choice) {

            case 1:
                addMember();
                break;

            case 2:
                displayMembers();
                break;

            case 3:
                searchByID();
                break;

            case 4:
                searchMember();
                break;

            case 5:
                searchByPhone();
                break;

            case 6:
                sortMembers();
                break;

            case 7:
                removeExpired();
                break;

            case 8:
                renewMembership();
                break;

            case 9:
                updateMember();
                break;

            case 10:
                generateReport();
                break;

            case 11:
                resetData();
                break;

            case 0:
                printf("Exiting Program...\n");
                break;

            default:
                printf("Invalid Choice\n");
        }

    } while (choice != 0);

    return 0;
}