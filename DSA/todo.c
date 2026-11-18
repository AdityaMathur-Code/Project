/*
 * To-Do List and Task Manager
 * Tasks are stored in a linked list.
 *
 * Module 1 : add a task
 * Module 2 : edit, complete and delete a task
 * Module 3 : sort the tasks (bubble sort)
 * Module 4 : show all / pending / completed tasks and search
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

struct Date {
    int day;
    int month;
    int year;
};

struct Task {
    int id;
    char title[MAX];
    int priority;            /* 1 = low, 2 = medium, 3 = high */
    struct Date deadline;
    int completed;           /* 0 = pending, 1 = completed */
};

struct Node {
    struct Task task;
    struct Node *next;
};

struct Node *head = NULL;    /* first node of the list */
int nextId = 1;              /* id given to the next new task */


/* ---------- input helpers ---------- */

/* reads a whole number, asks again if the user types something else */
int readInt() {
    int n;
    int result;
    int c;

    while (1) {
        result = scanf("%d", &n);
        if (result == EOF) {
            printf("\nBye!\n");
            exit(0);
        }

        /* throw away the rest of the line */
        c = getchar();
        while (c != '\n' && c != EOF) {
            c = getchar();
        }

        if (result == 1) {
            return n;
        }
        printf("Please type a whole number: ");
    }
}

/* asks for a number and keeps asking until it is between min and max */
int askInt(char *message, int min, int max) {
    int n;

    printf("%s", message);
    n = readInt();
    while (n < min || n > max) {
        printf("Please enter a number from %d to %d: ", min, max);
        n = readInt();
    }
    return n;
}

/* reads a line of text (spaces are allowed) */
void readLine(char *s) {
    int c;

    if (fgets(s, MAX, stdin) == NULL) {
        printf("\nBye!\n");
        exit(0);
    }

    if (strchr(s, '\n') != NULL) {
        s[strcspn(s, "\n")] = '\0';
    } else {
        /* the line was too long for the buffer, so throw away the rest */
        c = getchar();
        while (c != '\n' && c != EOF) {
            c = getchar();
        }
    }
}

/* changes capital letters in s to small letters */
void toLower(char *s) {
    int i;

    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] = s[i] + 32;
        }
    }
}


/* ---------- date helpers ---------- */

/* how many days are in this month */
int daysInMonth(int month, int year) {
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
            return 29;
        }
        return 28;
    }
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    }
    return 31;
}

/* asks for a date and checks that it really exists */
struct Date askDate() {
    struct Date d;

    while (1) {
        d.day = askInt("  Day (1-31): ", 1, 31);
        d.month = askInt("  Month (1-12): ", 1, 12);
        d.year = askInt("  Year (1900-2100): ", 1900, 2100);

        if (d.day <= daysInMonth(d.month, d.year)) {
            return d;
        }
        printf("  That month does not have that many days. Try again.\n");
    }
}

/* turns a date into one number so two dates can be compared, e.g. 20261125 */
int dateNumber(struct Date d) {
    return d.year * 10000 + d.month * 100 + d.day;
}


/* ---------- printing ---------- */

void printHeader() {
    printf("\n%-4s %-8s %-10s %-10s %s\n", "ID", "PRIORITY", "DEADLINE", "STATUS", "TITLE");
    printf("----------------------------------------------------------------------\n");
}

void printTask(struct Task t) {
    char *priorityText;

    if (t.priority == 1) {
        priorityText = "Low";
    } else if (t.priority == 2) {
        priorityText = "Medium";
    } else {
        priorityText = "High";
    }

    printf("%-4d %-8s %02d-%02d-%04d %-10s %s\n",
           t.id, priorityText,
           t.deadline.day, t.deadline.month, t.deadline.year,
           t.completed ? "Completed" : "Pending",
           t.title);
}

/* finds a task by its id, returns NULL if it is not in the list */
struct Node *findTask(int id) {
    struct Node *cur = head;

    while (cur != NULL) {
        if (cur->task.id == id) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

/* status: -1 = show all, 0 = show pending, 1 = show completed */
void showTasks(int status) {
    struct Node *cur = head;
    int count = 0;

    printHeader();
    while (cur != NULL) {
        if (status == -1 || cur->task.completed == status) {
            printTask(cur->task);
            count++;
        }
        cur = cur->next;
    }

    if (count == 0) {
        printf("No tasks to show.\n");
    }
}


/* ===== Module 1: add a task at the end of the list ===== */

void addTask() {
    struct Node *newNode;
    struct Node *cur;

    newNode = malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Not enough memory.\n");
        return;
    }

    printf("Title: ");
    readLine(newNode->task.title);
    while (strlen(newNode->task.title) == 0) {
        printf("Title cannot be empty. Title: ");
        readLine(newNode->task.title);
    }

    newNode->task.id = nextId;
    nextId++;

    newNode->task.priority = askInt("Priority (1 = low, 2 = medium, 3 = high): ", 1, 3);

    printf("Deadline:\n");
    newNode->task.deadline = askDate();

    newNode->task.completed = askInt("Status (0 = pending, 1 = completed): ", 0, 1);
    newNode->next = NULL;

    /* put the new node at the end of the list */
    if (head == NULL) {
        head = newNode;
    } else {
        cur = head;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = newNode;
    }

    printf("Task %d added.\n", newNode->task.id);
}


/* ===== Module 2: edit, complete and delete ===== */

void editTask() {
    struct Node *node;
    int choice;

    printf("Enter task ID: ");
    node = findTask(readInt());
    if (node == NULL) {
        printf("Task not found.\n");
        return;
    }

    printf("1. Title\n");
    printf("2. Priority\n");
    printf("3. Deadline\n");
    choice = askInt("What do you want to change? ", 1, 3);

    if (choice == 1) {
        char newTitle[MAX];

        printf("New title (press Enter to keep the old one): ");
        readLine(newTitle);
        if (strlen(newTitle) > 0) {
            strcpy(node->task.title, newTitle);
        }
    } else if (choice == 2) {
        node->task.priority = askInt("New priority (1 = low, 2 = medium, 3 = high): ", 1, 3);
    } else {
        node->task.deadline = askDate();
    }

    printf("Task updated.\n");
}

void completeTask() {
    struct Node *node;

    printf("Enter task ID: ");
    node = findTask(readInt());
    if (node == NULL) {
        printf("Task not found.\n");
        return;
    }

    node->task.completed = 1;
    printf("Task marked as completed.\n");
}

void deleteTask() {
    struct Node *prev = NULL;
    struct Node *cur = head;
    int id;

    printf("Enter task ID: ");
    id = readInt();

    /* walk the list and keep the node before the one we want */
    while (cur != NULL && cur->task.id != id) {
        prev = cur;
        cur = cur->next;
    }

    if (cur == NULL) {
        printf("Task not found.\n");
        return;
    }

    if (prev == NULL) {
        head = cur->next;        /* deleting the first task */
    } else {
        prev->next = cur->next;  /* skip over the deleted node */
    }

    free(cur);
    printf("Task deleted.\n");
}


/* ===== Module 3: sorting ===== */

/* returns 1 if task a should come after task b */
int needsSwap(struct Task a, struct Task b, int sortBy) {
    char titleA[MAX];
    char titleB[MAX];

    if (sortBy == 1) {
        return a.priority < b.priority;     /* high priority first */
    }
    if (sortBy == 2) {
        return dateNumber(a.deadline) > dateNumber(b.deadline);   /* earliest first */
    }

    /* sort by title, ignoring capital letters */
    strcpy(titleA, a.title);
    strcpy(titleB, b.title);
    toLower(titleA);
    toLower(titleB);
    return strcmp(titleA, titleB) > 0;
}

void sortTasks() {
    struct Node *cur;
    struct Task temp;
    int sortBy;
    int swapped = 1;

    printf("Sort by: 1. Priority   2. Deadline   3. Title\n");
    sortBy = askInt("Choice: ", 1, 3);

    /* bubble sort: keep going through the list until nothing is swapped */
    while (swapped) {
        swapped = 0;
        cur = head;

        while (cur != NULL && cur->next != NULL) {
            if (needsSwap(cur->task, cur->next->task, sortBy)) {
                temp = cur->task;
                cur->task = cur->next->task;
                cur->next->task = temp;
                swapped = 1;
            }
            cur = cur->next;
        }
    }

    printf("Tasks sorted.\n");
    showTasks(-1);
}

/* ===== Module 4: show and search ===== */
void searchTasks() {
    char keyword[MAX];
    char lowerKeyword[MAX];
    char lowerTitle[MAX];
    struct Node *cur = head;
    int count = 0;

    printf("Enter a word to search for: ");
    readLine(keyword);

    strcpy(lowerKeyword, keyword);
    toLower(lowerKeyword);

    printHeader();
    while (cur != NULL) {
        strcpy(lowerTitle, cur->task.title);
        toLower(lowerTitle);

        if (strstr(lowerTitle, lowerKeyword) != NULL) {
            printTask(cur->task);
            count++;
        }
        cur = cur->next;
    }

    if (count == 0) {
        printf("No tasks found with that word.\n");
    }
}


/* ---------- main menu ---------- */

void freeAll() {
    struct Node *cur = head;

    while (cur != NULL) {
        struct Node *next = cur->next;
        free(cur);
        cur = next;
    }
    head = NULL;
}

void printMenu() {
    printf("\n1. Add a task\n");
    printf("2. Show all tasks\n");
    printf("3. Show pending tasks\n");
    printf("4. Show completed tasks\n");
    printf("5. Search tasks by a word\n");
    printf("6. Edit a task\n");
    printf("7. Mark a task as completed\n");
    printf("8. Delete a task\n");
    printf("9. Sort tasks\n");
    printf("0. Exit\n");
}

int main() {
    int choice;

    printf("===== TO-DO LIST AND TASK MANAGER =====\n");

    while (1) {
        printMenu();
        choice = askInt("Enter your choice: ", 0, 9);

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            addTask();
        } else if (choice == 2) {
            showTasks(-1);
        } else if (choice == 3) {
            showTasks(0);
        } else if (choice == 4) {
            showTasks(1);
        } else if (choice == 5) {
            searchTasks();
        } else if (choice == 6) {
            editTask();
        } else if (choice == 7) {
            completeTask();
        } else if (choice == 8) {
            deleteTask();
        } else {
            sortTasks();
        }
    }

    freeAll();
    printf("Bye!\n");
    return 0;
}
