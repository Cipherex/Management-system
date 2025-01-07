#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 50
#define DISEASE_LEN 50
#define PATIENTS_FILE "patients.txt"

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

    for (int i = 0; i < patientCount; i++) {
        printf("\n-----------------\n");
        printf("ID: %d\n", patients[i].id);
        printf("Name: %s\n", patients[i].name);
        printf("Age: %d\n", patients[i].age);
        printf("Gender: %c\n", patients[i].gender);
        printf("Disease: %s\n", patients[i].disease);
    }
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
