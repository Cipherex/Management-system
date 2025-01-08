#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 50
#define SUBJECT_LEN 30
#define STAFF_FILE "staff.txt"
#define STUDENT_FILE "students.txt"


#define MAX_COLS 10
#define CELL_PADDING 2

typedef struct {
    char* headers[MAX_COLS];
    char*** data;
    int* colWidths;
    int rows;
    int cols;
} Table;

void initTable(Table* table, char** headers, int cols) {
    table->cols = cols;
    table->rows = 0;
    table->colWidths = (int*)calloc(cols, sizeof(int));
    
    // Initialize headers and find initial column widths
    for (int i = 0; i < cols; i++) {
        table->headers[i] = strdup(headers[i]);
        table->colWidths[i] = strlen(headers[i]) + CELL_PADDING;
    }
    
    table->data = NULL;
}

void addRow(Table* table, char** rowData) {
    table->rows++;
    table->data = (char***)realloc(table->data, table->rows * sizeof(char**));
    table->data[table->rows - 1] = (char**)malloc(table->cols * sizeof(char*));
    
    for (int i = 0; i < table->cols; i++) {
        table->data[table->rows - 1][i] = strdup(rowData[i]);
        int cellLen = strlen(rowData[i]) + CELL_PADDING;
        if (cellLen > table->colWidths[i]) {
            table->colWidths[i] = cellLen;
        }
    }
}

void printHorizontalLine(Table* table) {
    printf("+");
    for (int i = 0; i < table->cols; i++) {
        for (int j = 0; j < table->colWidths[i]; j++) printf("-");
        printf("+");
    }
    printf("\n");
}

void printRow(Table* table, char** rowData) {
    printf("|");
    for (int i = 0; i < table->cols; i++) {
        printf("%-*s|", table->colWidths[i], rowData[i]);
    }
    printf("\n");
}

void displayTable(Table* table) {
    printHorizontalLine(table);
    
    // Print headers
    printf("|");
    for (int i = 0; i < table->cols; i++) {
        printf("%-*s|", table->colWidths[i], table->headers[i]);
    }
    printf("\n");
    
    printHorizontalLine(table);
    
    // Print data
    for (int i = 0; i < table->rows; i++) {
        printRow(table, table->data[i]);
    }
    
    printHorizontalLine(table);
}

void freeTable(Table* table) {
    for (int i = 0; i < table->cols; i++) {
        free(table->headers[i]);
    }
    
    for (int i = 0; i < table->rows; i++) {
        for (int j = 0; j < table->cols; j++) {
            free(table->data[i][j]);
        }
        free(table->data[i]);
    }
    
    free(table->data);
    free(table->colWidths);
}

// Utility function to convert int to string
char* itoa_custom(int value, char* result) {
    static char buffer[20];  // Static buffer for each call
    sprintf(buffer, "%d", value);
    return strdup(buffer);   // Return duplicated string
}

char* ftoa_custom(float value, char* result) {
    static char buffer[20];
    sprintf(buffer, "%.2f", value);
    return strdup(buffer);
}

typedef struct {
    int id;
    char name[NAME_LEN];
    char subject[SUBJECT_LEN];
    float salary;
} Staff;

typedef struct {
    int id;
    char name[NAME_LEN];
    float grade;
    int attendance;
    int feesPaid;
} Student;

Staff* staffList = NULL;
Student* studentList = NULL;
int staffCount = 0;
int studentCount = 0;

// Function declarations
void loadStaffFromFile();
void saveStaffToFile();
void loadStudentsFromFile();
void saveStudentsToFile();
void addStaff();
void viewStaff();
void deleteStaff();
void addStudent();
void viewStudents();
void deleteStudent();

void menu() {
    int choice;
    while (1) {
        printf("\n--- School Management System ---\n");
        printf("1. Add Staff\n");
        printf("2. View All Staff\n");
        printf("3. Delete Staff\n");
        printf("4. Add Student\n");
        printf("5. View All Students\n");
        printf("6. Delete Student\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: addStaff(); break;
            case 2: viewStaff(); break;
            case 3: deleteStaff(); break;
            case 4: addStudent(); break;
            case 5: viewStudents(); break;
            case 6: deleteStudent(); break;
            case 7: 
                free(staffList);
                free(studentList);
                exit(0);
            default: printf("Invalid choice! Please try again.\n");
        }
    }
}

void loadStaffFromFile() {
    FILE *file = fopen(STAFF_FILE, "r");
    if (file == NULL) {
        printf("No existing staff database found. Starting fresh...\n");
        return;
    }

    fscanf(file, "%d", &staffCount);
    staffList = (Staff*)malloc(staffCount * sizeof(Staff));
    if (staffList == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }

    for (int i = 0; i < staffCount; i++) {
        fscanf(file, "%d\n", &staffList[i].id);
        fgets(staffList[i].name, NAME_LEN, file);
        staffList[i].name[strcspn(staffList[i].name, "\n")] = '\0';
        fgets(staffList[i].subject, SUBJECT_LEN, file);
        staffList[i].subject[strcspn(staffList[i].subject, "\n")] = '\0';
        fscanf(file, "%f\n", &staffList[i].salary);
    }
    fclose(file);
}

void saveStaffToFile() {
    FILE *file = fopen(STAFF_FILE, "w");
    if (file == NULL) {
        printf("Error saving staff data!\n");
        return;
    }

    fprintf(file, "%d\n", staffCount);
    for (int i = 0; i < staffCount; i++) {
        fprintf(file, "%d\n", staffList[i].id);
        fprintf(file, "%s\n", staffList[i].name);
        fprintf(file, "%s\n", staffList[i].subject);
        fprintf(file, "%.2f\n", staffList[i].salary);
    }
    fclose(file);
}

void addStaff() {
    Staff newStaff;
    printf("Enter Staff ID: ");
    scanf("%d", &newStaff.id);
    getchar();

    printf("Enter Staff Name: ");
    fgets(newStaff.name, NAME_LEN, stdin);
    newStaff.name[strcspn(newStaff.name, "\n")] = '\0';

    printf("Enter Subject: ");
    fgets(newStaff.subject, SUBJECT_LEN, stdin);
    newStaff.subject[strcspn(newStaff.subject, "\n")] = '\0';

    printf("Enter Salary: ");
    scanf("%f", &newStaff.salary);

    staffList = (Staff*)realloc(staffList, (staffCount + 1) * sizeof(Staff));
    if (staffList == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }

    staffList[staffCount] = newStaff;
    staffCount++;
    saveStaffToFile();
    printf("Staff added successfully!\n");
}

void viewStaff() {
    if (staffCount == 0) {
        printf("No staff in the database.\n");
        return;
    }

    char* headers[] = {"ID", "Name", "Subject", "Salary"};
    Table table;
    initTable(&table, headers, 4);
    
    char buffer[20];
    for (int i = 0; i < staffCount; i++) {
        char* rowData[4];
        rowData[0] = itoa_custom(staffList[i].id, buffer);
        rowData[1] = staffList[i].name;
        rowData[2] = staffList[i].subject;
        rowData[3] = ftoa_custom(staffList[i].salary, buffer);
        addRow(&table, rowData);
    }
    
    displayTable(&table);
    freeTable(&table);
}

void deleteStaff() {
    int id;
    printf("Enter Staff ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < staffCount; i++) {
        if (staffList[i].id == id) {
            for (int j = i; j < staffCount - 1; j++) {
                staffList[j] = staffList[j + 1];
            }
            staffCount--;
            staffList = (Staff*)realloc(staffList, staffCount * sizeof(Staff));
            saveStaffToFile();
            printf("Staff deleted successfully!\n");
            return;
        }
    }
    printf("Staff with ID %d not found.\n", id);
}

void loadStudentsFromFile() {
    FILE *file = fopen(STUDENT_FILE, "r");
    if (file == NULL) {
        printf("No existing student database found. Starting fresh...\n");
        return;
    }

    fscanf(file, "%d", &studentCount);
    studentList = (Student*)malloc(studentCount * sizeof(Student));
    if (studentList == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }

    for (int i = 0; i < studentCount; i++) {
        fscanf(file, "%d\n", &studentList[i].id);
        fgets(studentList[i].name, NAME_LEN, file);
        studentList[i].name[strcspn(studentList[i].name, "\n")] = '\0';
        fscanf(file, "%f\n", &studentList[i].grade);
        fscanf(file, "%d\n", &studentList[i].attendance);
        fscanf(file, "%d\n", &studentList[i].feesPaid);
    }
    fclose(file);
}

void saveStudentsToFile() {
    FILE *file = fopen(STUDENT_FILE, "w");
    if (file == NULL) {
        printf("Error saving student data!\n");
        return;
    }

    fprintf(file, "%d\n", studentCount);
    for (int i = 0; i < studentCount; i++) {
        fprintf(file, "%d\n", studentList[i].id);
        fprintf(file, "%s\n", studentList[i].name);
        fprintf(file, "%.2f\n", studentList[i].grade);
        fprintf(file, "%d\n", studentList[i].attendance);
        fprintf(file, "%d\n", studentList[i].feesPaid);
    }
    fclose(file);
}

void addStudent() {
    Student newStudent;
    printf("Enter Student ID: ");
    scanf("%d", &newStudent.id);
    getchar();

    printf("Enter Student Name: ");
    fgets(newStudent.name, NAME_LEN, stdin);
    newStudent.name[strcspn(newStudent.name, "\n")] = '\0';

    printf("Enter Student Grade: ");
    scanf("%f", &newStudent.grade);
    getchar();

    printf("Enter Attendance: ");
    scanf("%d", &newStudent.attendance);
    getchar();

    printf("Has Fees been Paid? (1 for Yes, 0 for No): ");
    scanf("%d", &newStudent.feesPaid);
    getchar();

    studentList = (Student*)realloc(studentList, (studentCount + 1) * sizeof(Student));
    if (studentList == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }

    studentList[studentCount] = newStudent;
    studentCount++;
    saveStudentsToFile();
    printf("Student added successfully!\n");
}

void viewStudents() {
    if (studentCount == 0) {
        printf("No students in the database.\n");
        return;
    }

    char* headers[] = {"ID", "Name", "Grade", "Attendance", "Fees Paid"};
    Table table;
    initTable(&table, headers, 5);
    
    char buffer[20];
    for (int i = 0; i < studentCount; i++) {
        char* rowData[5];
        rowData[0] = itoa_custom(studentList[i].id, buffer);
        rowData[1] = studentList[i].name;
        rowData[2] = ftoa_custom(studentList[i].grade, buffer);
        rowData[3] = itoa_custom(studentList[i].attendance, buffer);
        rowData[4] = studentList[i].feesPaid ? "Yes" : "No";
        addRow(&table, rowData);
    }
    
    displayTable(&table);
    freeTable(&table);
}

void deleteStudent() {
    int id;
    printf("Enter Student ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < studentCount; i++) {
        if (studentList[i].id == id) {
            for (int j = i; j < studentCount - 1; j++) {
                studentList[j] = studentList[j + 1];
            }
            studentCount--;
            studentList = (Student*)realloc(studentList, studentCount * sizeof(Student));
            saveStudentsToFile();
            printf("Student deleted successfully!\n");
            return;
        }
    }
    printf("Student with ID %d not found.\n", id);
}

int main() {
    loadStaffFromFile();
    loadStudentsFromFile();
    menu();
    return 0;
}
