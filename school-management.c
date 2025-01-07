#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 50
#define SUBJECT_LEN 30
#define STAFF_FILE "staff.txt"
#define STUDENT_FILE "students.txt"

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

    for (int i = 0; i < staffCount; i++) {
        printf("\n-----------------\n");
        printf("ID: %d\n", staffList[i].id);
        printf("Name: %s\n", staffList[i].name);
        printf("Subject: %s\n", staffList[i].subject);
        printf("Salary: %.2f\n", staffList[i].salary);
    }
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

    for (int i = 0; i < studentCount; i++) {
        printf("\n-----------------\n");
        printf("ID: %d\n", studentList[i].id);
        printf("Name: %s\n", studentList[i].name);
        printf("Grade: %.2f\n", studentList[i].grade);
        printf("Attendance: %d\n", studentList[i].attendance);
        printf("Fees paid: %s\n", studentList[i].feesPaid ? "Yes" : "No");
    }
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
