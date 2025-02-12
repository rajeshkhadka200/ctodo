#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"

typedef struct {
    int id;
    char title[100];
    char description[200];
    char priority[10];
    char dueDate[20];
    char category[20];
    char status[20];  // Status will store either "Pending" or "Completed"
} Task;

int taskCount = 0;
Task taskList[100];

// Function prototypes
void displayMainMenu(int highlight);
void addTask();
void viewTasks();
void editTask();
void deleteTask();
void saveTaskToFile(Task task);
void loadTasksFromFile();
void saveAllTasksToFile();
void exitApp();
void displayTasks();

void displayMainMenu(int highlight) {
    system("cls");

    const char *menu[] = {
        "Add Task",
        "View Tasks",
        "Edit Task",
        "Delete Task",
        "Exit"
    };

    for (int i = 0; i < 5; i++) {
        if (i == highlight) {
            printf("-> %s%s%s\n", GREEN, menu[i], RESET);
        } else {
            printf("   %s\n", menu[i]);
        }
    }
}

// Function to add a task
void addTask() {
    Task newTask;

    printf("\nEnter Task Title: ");
    fgets(newTask.title, sizeof(newTask.title), stdin);
    newTask.title[strcspn(newTask.title, "\n")] = 0;

    printf("Enter Task Description: ");
    fgets(newTask.description, sizeof(newTask.description), stdin);
    newTask.description[strcspn(newTask.description, "\n")] = 0;

    printf("Enter Task Priority (High/Medium/Low): ");
    fgets(newTask.priority, sizeof(newTask.priority), stdin);
    newTask.priority[strcspn(newTask.priority, "\n")] = 0;

    printf("Enter Due Date (YYYY-MM-DD): ");
    fgets(newTask.dueDate, sizeof(newTask.dueDate), stdin);
    newTask.dueDate[strcspn(newTask.dueDate, "\n")] = 0;

    printf("Enter Task Category (Work/Personal/Health): ");
    fgets(newTask.category, sizeof(newTask.category), stdin);
    newTask.category[strcspn(newTask.category, "\n")] = 0;

    strcpy(newTask.status, "Pending");

    newTask.id = taskCount + 1;  // Unique ID based on taskCount
    taskList[taskCount] = newTask;
    taskCount++;

    saveAllTasksToFile();  // Save all tasks

    printf("\nTask added successfully!\n");

    printf("\nPress any key to return to the main menu...\n");
    _getch();
}

// Function to view tasks
void viewTasks() {
    if (taskCount == 0) {
        printf("\nNo tasks available.\n");
    } else {
        displayTasks();
    }

    printf("\nPress any key to return to the main menu...\n");
    _getch();
}

// Function to display tasks
void displayTasks() {
    printf("\n%-4s %-20s %-30s %-10s %-12s %-20s %-10s\n", "ID", "Title", "Description", "Priority", "Due Date", "Category", "Status");
    for (int i = 0; i < taskCount; i++) {
        printf("%-4d %-20s %-30s %-10s %-12s %-20s %-10s\n", 
            taskList[i].id, 
            taskList[i].title, 
            taskList[i].description, 
            taskList[i].priority, 
            taskList[i].dueDate, 
            taskList[i].category, 
            taskList[i].status);
    }
}

// Function to edit a task
void editTask() {
    int taskId;
    int found = 0;

    printf("\nEnter Task ID to edit: ");
    scanf("%d", &taskId);
    getchar();  // To consume the newline character left by scanf

    for (int i = 0; i < taskCount; i++) {
        if (taskList[i].id == taskId) {
            found = 1;

            printf("\nEditing Task ID %d\n", taskList[i].id);
            printf("Enter New Task Title (current: %s): ", taskList[i].title);
            fgets(taskList[i].title, sizeof(taskList[i].title), stdin);
            taskList[i].title[strcspn(taskList[i].title, "\n")] = 0;

            printf("Enter New Task Description (current: %s): ", taskList[i].description);
            fgets(taskList[i].description, sizeof(taskList[i].description), stdin);
            taskList[i].description[strcspn(taskList[i].description, "\n")] = 0;

            printf("Enter New Task Priority (current: %s): ", taskList[i].priority);
            fgets(taskList[i].priority, sizeof(taskList[i].priority), stdin);
            taskList[i].priority[strcspn(taskList[i].priority, "\n")] = 0;

            printf("Enter New Due Date (current: %s): ", taskList[i].dueDate);
            fgets(taskList[i].dueDate, sizeof(taskList[i].dueDate), stdin);
            taskList[i].dueDate[strcspn(taskList[i].dueDate, "\n")] = 0;

            printf("Enter New Task Category (current: %s): ", taskList[i].category);
            fgets(taskList[i].category, sizeof(taskList[i].category), stdin);
            taskList[i].category[strcspn(taskList[i].category, "\n")] = 0;

            // Ask if the user wants to mark the task as "Completed"
            char statusChoice;
            printf("Do you want to mark this task as Completed? (y/n): ");
            statusChoice = getchar();
            getchar(); // To consume the newline character

            if (statusChoice == 'y' || statusChoice == 'Y') {
                strcpy(taskList[i].status, "Completed");
            } else {
                strcpy(taskList[i].status, "Pending");
            }

            // After editing, save all tasks to the file
            saveAllTasksToFile();

            printf("\nTask edited successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("\nTask with ID %d not found.\n", taskId);
    }

    printf("\nPress any key to return to the main menu...\n");
    _getch();
}

// Function to delete a task
void deleteTask() {
    int taskId;
    int found = 0;

    printf("\nEnter Task ID to delete: ");
    scanf("%d", &taskId);
    getchar();  // To consume the newline character left by scanf

    for (int i = 0; i < taskCount; i++) {
        if (taskList[i].id == taskId) {
            found = 1;

            // Ask for confirmation
            char confirm;
            printf("\nAre you sure you want to delete Task ID %d (Title: %s)? (y/n): ", taskList[i].id, taskList[i].title);
            confirm = getchar();
            getchar();  // To consume the newline character after 'y' or 'n'

            if (confirm == 'y' || confirm == 'Y') {
                // Shift tasks down to remove the task
                for (int j = i; j < taskCount - 1; j++) {
                    taskList[j] = taskList[j + 1];
                }
                taskCount--;  // Decrease the task count

                // After deletion, save the remaining tasks back to the file
                saveAllTasksToFile();

                printf("\nTask deleted successfully!\n");
            } else {
                printf("\nTask deletion canceled.\n");
            }
            break;
        }
    }

    if (!found) {
        printf("\nTask with ID %d not found.\n", taskId);
    }

    printf("\nPress any key to return to the main menu...\n");
    _getch();
}

// Function to save all tasks to the file after deletion or editing
void saveAllTasksToFile() {
    FILE *file = fopen("tasks.csv", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < taskCount; i++) {
        fprintf(file, "%d,%s,%s,%s,%s,%s,%s\n", taskList[i].id, taskList[i].title, taskList[i].description, taskList[i].priority, taskList[i].dueDate, taskList[i].category, taskList[i].status);
    }

    fclose(file);
}

// Function to load tasks from the file
void loadTasksFromFile() {
    FILE *file = fopen("tasks.csv", "r");
    if (file == NULL) {
        return;
    }

    while (fscanf(file, "%d,%99[^,],%199[^,],%9[^,],%19[^,],%19[^,],%19[^,\n]\n", 
                  &taskList[taskCount].id,
                  taskList[taskCount].title,
                  taskList[taskCount].description,
                  taskList[taskCount].priority,
                  taskList[taskCount].dueDate,
                  taskList[taskCount].category,
                  taskList[taskCount].status) == 7) {
        taskCount++;
    }

    fclose(file);
}

void exitApp() {
    printf("\nExiting the program...\n");
}

int main() {
    loadTasksFromFile();  // Load tasks from file at the start
    int choice;
    int highlight = 0;

    while (1) {
        displayMainMenu(highlight);
        choice = _getch();

        if (choice == 224) {
            choice = _getch();  // Arrow key pressed
            if (choice == 72) {  // Up arrow
                highlight = (highlight == 0) ? 4 : highlight - 1;
            } else if (choice == 80) {  // Down arrow
                highlight = (highlight == 4) ? 0 : highlight + 1;
            }
        } else if (choice == 13) {  // Enter key
            if (highlight == 0) {
                addTask();
            } else if (highlight == 1) {
                viewTasks();
            } else if (highlight == 2) {
                editTask();
            } else if (highlight == 3) {
                deleteTask();
            } else if (highlight == 4) {
                exitApp();
                break;
            }
        }
    }

    return 0;
}
