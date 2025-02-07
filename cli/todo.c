#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>

#define MAX_TASKS 100
#define MAX_TASK_LENGTH 100
#define FILENAME "tasks.txt"

// Windows console colors
#define COLOR_RED FOREGROUND_RED | FOREGROUND_INTENSITY
#define COLOR_GREEN FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define COLOR_CYAN FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define COLOR_WHITE FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY

// Structure to represent a task
struct Task {
    char des[MAX_TASK_LENGTH];
    bool status;
    time_t createdAt;
    time_t completedAt;
};

// Global variables
struct Task tasks[MAX_TASKS];
int num_tasks = 0;
HANDLE hConsole;

// Function prototypes
void setConsoleColor(WORD color);
void resetConsoleColor();
void addTask(const char *des);
void deleteTask(int index);
void markDone(int index);
void markUndone(int index);
void listTasks(bool showTime);
void printUsage();
void loadTasks();
void saveTasks();
void aboutus();
void displayMenu();

int main() {
    // Get console handle for colors
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Load tasks from file
    loadTasks();

    int choice;
    char input[MAX_TASK_LENGTH];
    int index;
    bool showTime = false;

    while (1) {
        displayMenu();
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline character

        switch (choice) {
            case 1: // Add task
                printf("Enter task description: ");
                fgets(input, MAX_TASK_LENGTH, stdin);
                input[strcspn(input, "\n")] = '\0'; // Remove newline character
                addTask(input);
                break;

            case 2: // Delete task
                printf("Enter task index to delete: ");
                scanf("%d", &index);
                deleteTask(index);
                break;

            case 3: // Mark task as done
                printf("Enter task index to mark as done: ");
                scanf("%d", &index);
                markDone(index);
                break;

            case 4: // Mark task as undone
                printf("Enter task index to mark as undone: ");
                scanf("%d", &index);
                markUndone(index);
                break;

            case 5: // List tasks
                printf("Show timestamps? (1 for yes, 0 for no): ");
                scanf("%d", &index);
                listTasks(index == 1);
                break;

            case 6: // About us
                aboutus();
                break;

            case 7: // Exit
                saveTasks();
                setConsoleColor(COLOR_GREEN);
                printf("\nExiting the program. Goodbye!\n");
                resetConsoleColor();
                return 0;

            default:
                setConsoleColor(COLOR_RED);
                printf("\nInvalid choice. Please try again.\n");
                resetConsoleColor();
                break;
        }

        // Save tasks after each operation
        saveTasks();
    }

    return 0;
}

void displayMenu() {
    setConsoleColor(COLOR_CYAN);
    printf("\n📝 Todo App Menu\n");
    resetConsoleColor();
    printf("1. Add Task\n");
    printf("2. Delete Task\n");
    printf("3. Mark Task as Done\n");
    printf("4. Mark Task as Undone\n");
    printf("5. List Tasks\n");
    printf("6. About Us\n");
    printf("7. Exit\n");
}

void setConsoleColor(WORD color) {
    SetConsoleTextAttribute(hConsole, color);
}

void resetConsoleColor() {
    SetConsoleTextAttribute(hConsole, COLOR_WHITE);
}

void addTask(const char *des) {
    if (num_tasks >= MAX_TASKS) {
        setConsoleColor(COLOR_RED);
        printf("Error: Cannot add more tasks. Task list full.\n");
        resetConsoleColor();
        return;
    }

    struct Task newTask;
    strncpy(newTask.des, des, MAX_TASK_LENGTH - 1);
    newTask.des[MAX_TASK_LENGTH - 1] = '\0';
    newTask.status = false;
    newTask.createdAt = time(NULL);
    newTask.completedAt = 0;

    tasks[num_tasks++] = newTask;

    setConsoleColor(COLOR_GREEN);
    printf("\nTask added successfully!\n");
    resetConsoleColor();
}

void deleteTask(int index) {
    if (index < 1 || index > num_tasks) {
        setConsoleColor(COLOR_RED);
        printf("\nError: Invalid task index\n");
        resetConsoleColor();
        return;
    }

    for (int i = index - 1; i < num_tasks - 1; i++) {
        tasks[i] = tasks[i + 1];
    }
    num_tasks--;

    setConsoleColor(COLOR_GREEN);
    printf("\nTask deleted successfully!\n");
    resetConsoleColor();
}

void listTasks(bool showTime) {
    if (num_tasks == 0) {
        printf("\nNo tasks available.\n");
        return;
    }

    setConsoleColor(COLOR_CYAN);
    printf("\nTasks:\n");
    resetConsoleColor();

    for (int i = 0; i < num_tasks; i++) {
        char dateStr[26];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M:%S", localtime(&tasks[i].createdAt));
        
        printf("\n%d. [", i + 1);
        if (tasks[i].status) {
            setConsoleColor(COLOR_GREEN);
            printf("X");
        } else {
            printf(" ");
        }
        resetConsoleColor();
        printf("] %s", tasks[i].des);

        if (showTime) {
            printf(" - Created at: %s", dateStr);
            if (tasks[i].status && tasks[i].completedAt != 0) {
                strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M:%S", localtime(&tasks[i].completedAt));
                printf(", Completed at: %s", dateStr);
            }
        }
        printf("\n");
    }
}

void loadTasks() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("\nFile does not exist. Creating a new file.\n");
        saveTasks();
        return;
    }

    num_tasks = 0;
    while (fread(&tasks[num_tasks], sizeof(struct Task), 1, file) == 1) {
        num_tasks++;
    }

    fclose(file);
}

void saveTasks() {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        setConsoleColor(COLOR_RED);
        printf("Error: Failed to open file for saving tasks.\n");
        resetConsoleColor();
        return;
    }

    for (int i = 0; i < num_tasks; i++) {
        fwrite(&tasks[i], sizeof(struct Task), 1, file);
    }

    fclose(file);
}

void markDone(int index) {
    if (index < 1 || index > num_tasks) {
        setConsoleColor(COLOR_RED);
        printf("Error: Invalid task index.\n");
        resetConsoleColor();
        return;
    }

    tasks[index - 1].status = true;
    tasks[index - 1].completedAt = time(NULL);
    
    setConsoleColor(COLOR_GREEN);
    printf("\nTask marked as done.\n");
    resetConsoleColor();
}

void markUndone(int index) {
    if (index < 1 || index > num_tasks) {
        setConsoleColor(COLOR_RED);
        printf("Error: Invalid task index.\n");
        resetConsoleColor();
        return;
    }

    tasks[index - 1].status = false;
    tasks[index - 1].completedAt = 0;
    
    setConsoleColor(COLOR_GREEN);
    printf("\nTask marked as undone.\n");
    resetConsoleColor();
}

void aboutus() {
    setConsoleColor(COLOR_CYAN);
    printf("\n📝 Welcome to Ctodo - Your Command Line Todo App\n");
    resetConsoleColor();
    printf("\nVersion: 1.0\n");
    printf("Developed by: Utsav Bhattarai & Loblesh Bhartal\n");
}