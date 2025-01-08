#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 50
#define DISEASE_LEN 50
#define PATIENTS_FILE "patients.txt"

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
    int age;
    char gender;
    char disease[DISEASE_LEN];
} Patient;

Patient* patients = NULL;
int patientCount = 0;

void loadPatientsFromFile();
void savePatientsToFile();
void addPatient();
void viewPatients();
void searchPatient();
void updatePatient();
void deletePatient();
int findPatientIndex(int id);

void menu() {
    int choice;
    while (1) {
        printf("\n--- Hospital Management System ---\n");
        printf("1. Add Patient\n");
        printf("2. View All Patients\n");
        printf("3. Search Patient\n");
        printf("4. Update Patient\n");
        printf("5. Delete Patient\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: addPatient(); break;
            case 2: viewPatients(); break;
            case 3: searchPatient(); break;
            case 4: updatePatient(); break;
            case 5: deletePatient(); break;
            case 6: 
                free(patients);
                exit(0);
            default: printf("Invalid choice! Please try again.\n");
        }
    }
}

void loadPatientsFromFile() {
    FILE *file = fopen(PATIENTS_FILE, "r");
    if (file == NULL) {
        printf("No existing patient database found. Starting fresh...\n");
        return;
    }

    fscanf(file, "%d", &patientCount);
    patients = (Patient*)malloc(patientCount * sizeof(Patient));
    if (patients == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }

    for (int i = 0; i < patientCount; i++) {
        fscanf(file, "%d\n", &patients[i].id);
        fgets(patients[i].name, NAME_LEN, file);
        patients[i].name[strcspn(patients[i].name, "\n")] = '\0';
        fscanf(file, "%d\n", &patients[i].age);
        fscanf(file, "%c\n", &patients[i].gender);
        fgets(patients[i].disease, DISEASE_LEN, file);
        patients[i].disease[strcspn(patients[i].disease, "\n")] = '\0';
    }
    fclose(file);
}

void savePatientsToFile() {
    FILE *file = fopen(PATIENTS_FILE, "w");
    if (file == NULL) {
        printf("Error saving patient data!\n");
        return;
    }

    fprintf(file, "%d\n", patientCount);
    for (int i = 0; i < patientCount; i++) {
        fprintf(file, "%d\n", patients[i].id);
        fprintf(file, "%s\n", patients[i].name);
        fprintf(file, "%d\n", patients[i].age);
        fprintf(file, "%c\n", patients[i].gender);
        fprintf(file, "%s\n", patients[i].disease);
    }
    fclose(file);
}

void addPatient() {
    Patient newPatient;
    printf("Enter Patient ID: ");
    scanf("%d", &newPatient.id);
    getchar();

    if (findPatientIndex(newPatient.id) != -1) {
        printf("Error: Patient with ID %d already exists!\n", newPatient.id);
        return;
    }

    printf("Enter Patient Name: ");
    fgets(newPatient.name, NAME_LEN, stdin);
    newPatient.name[strcspn(newPatient.name, "\n")] = '\0';

    printf("Enter Patient Age: ");
    scanf("%d", &newPatient.age);
    getchar();

    printf("Enter Patient Gender (M/F): ");
    fgets(&newPatient.gender, 2, stdin);
    getchar();

    printf("Enter Disease: ");
    fgets(newPatient.disease, DISEASE_LEN, stdin);
    newPatient.disease[strcspn(newPatient.disease, "\n")] = '\0';

    patients = (Patient*)realloc(patients, (patientCount + 1) * sizeof(Patient));
    if (patients == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }

    patients[patientCount] = newPatient;
    patientCount++;
    savePatientsToFile();
    printf("Patient added successfully!\n");
}

void viewPatients() {
    if (patientCount == 0) {
        printf("No patients in the database.\n");
        return;
    }

    char* headers[] = {"ID", "Name", "Age", "Gender", "Disease"};
    Table table;
    initTable(&table, headers, 5);
    
    for (int i = 0; i < patientCount; i++) {
        char* rowData[5];
        char idStr[20], ageStr[20];
        sprintf(idStr, "%d", patients[i].id);
        sprintf(ageStr, "%d", patients[i].age);
        
        rowData[0] = strdup(idStr);
        rowData[1] = strdup(patients[i].name);
        rowData[2] = strdup(ageStr);
        char* genderStr = (char*)malloc(2);
        genderStr[0] = patients[i].gender;
        genderStr[1] = '\0';
        rowData[3] = genderStr;
        rowData[4] = strdup(patients[i].disease);
        
        addRow(&table, rowData);
    }
    
    displayTable(&table);
    freeTable(&table);
}
void searchPatient() {
    int id;
    printf("Enter Patient ID to search: ");
    scanf("%d", &id);

    int index = findPatientIndex(id);
    if (index == -1) {
        printf("Patient with ID %d not found.\n", id);
    } else {
        printf("\nPatient found:\n");
        printf("ID: %d\n", patients[index].id);
        printf("Name: %s\n", patients[index].name);
        printf("Age: %d\n", patients[index].age);
        printf("Gender: %c\n", patients[index].gender);
        printf("Disease: %s\n", patients[index].disease);
    }
}

void updatePatient() {
    int id;
    printf("Enter Patient ID to update: ");
    scanf("%d", &id);

    int index = findPatientIndex(id);
    if (index == -1) {
        printf("Patient with ID %d not found.\n", id);
        return;
    }

    printf("Enter new name for Patient: ");
    getchar();
    fgets(patients[index].name, NAME_LEN, stdin);
    patients[index].name[strcspn(patients[index].name, "\n")] = '\0';

    printf("Enter new age: ");
    scanf("%d", &patients[index].age);

    printf("Enter new gender (M/F): ");
    getchar();
    scanf("%c", &patients[index].gender);

    printf("Enter new disease: ");
    getchar();
    fgets(patients[index].disease, DISEASE_LEN, stdin);
    patients[index].disease[strcspn(patients[index].disease, "\n")] = '\0';

    savePatientsToFile();
    printf("Patient updated successfully!\n");
}

void deletePatient() {
    int id;
    printf("Enter Patient ID to delete: ");
    scanf("%d", &id);

    int index = findPatientIndex(id);
    if (index == -1) {
        printf("Patient with ID %d not found.\n", id);
        return;
    }

    for (int i = index; i < patientCount - 1; i++) {
        patients[i] = patients[i + 1];
    }
    patientCount--;
    patients = (Patient*)realloc(patients, patientCount * sizeof(Patient));

    savePatientsToFile();
    printf("Patient deleted successfully!\n");
}

int findPatientIndex(int id) {
    for (int i = 0; i < patientCount; i++) {
        if (patients[i].id == id) {
            return i;
        }
    }
    return -1;
}

int main() {
    loadPatientsFromFile();
    menu();
    return 0;
}
