/*************************************/
/***********Name : Jehad Amr**********/
/***********Date : 8/26/2024**********/
/***********C project*****************/
/**********Topic: Clinic System*******/
/***********Version : 1.0**************/

/* Libraries */
#include <stdio.h>
#include <stdlib.h> // for malloc

/**** Preprocessing definitions ***/
#define MAX_ATTEMPTS 3
#define SLOT_COUNT   5
#define PASSWORD 1234

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short int u16;
typedef unsigned long int u32;
typedef signed short int s16;
typedef signed long int s32;
typedef float f32;
typedef double f64;
typedef long double f96;

/*********** ERROR STATE ***********/
#define STD_TYPES_OK 1
#define STD_TYPES_NOK 0
#define NULL      ((void*)0)

// Node for storing information of patient
typedef struct patient {
    /** Data **/
    u8 name[60];
    u16 age;
    u8 gender;
    u16 id;
    /** Pointer to the next node **/
    struct patient *Next;
} Patient;

// Node for reservation
typedef struct reservation {
    u16 patientID;
    u16 slot;
    struct reservation *Next;
} Reservation;

// Initializing head pointer for each node with null to avoid wild pointers
static Patient *patientHead = NULL;
static Reservation *reservationHead = NULL;

// Array of strings representing available time slots
const char* slots[SLOT_COUNT] = {
    "2pm to 2:30pm",
    "2:30pm to 3pm",
    "3pm to 3:30pm",
    "4pm to 4:30pm",
    "4:30pm to 5pm"
};

// Function prototypes
void addPatient();
void editPatient();
void reserveSlot();
void cancelReservationByID(u16 id);

// User mode functions
void viewPatientRecord();
void viewTodaysReservations();

// Extra functions for organizing code
void displayAdminMenu();
int checkPassword();
void freeLists();
Patient* findPatientByID(u16 id);
Reservation* findReservationBySlot(u16 slot);

// Admin mode functions
int checkPassword() {
    u16 attempt = 0;
    u16 Entered_Password;

    while (attempt < MAX_ATTEMPTS) {
        printf("Enter admin password: ");
        scanf("%hu", &Entered_Password);
        if (Entered_Password == PASSWORD) {
            return 1;
        } else {
            attempt++;
            printf("Incorrect password. Attempts left: %d\n", MAX_ATTEMPTS - attempt);
        }
    }
    return 0;
}

/*******************************************************************/
// Search for existing patient node by ID
Patient* findPatientByID(u16 id) {
    Patient* temp = patientHead;
    while (temp != NULL) {
        if (temp->id == id) {
            return temp;
        }
        // Move to the next node
        temp = temp->Next;
    }
    // If not found, return NULL
    return NULL;
}

/******************************************************************/
void addPatient() {
    u16 id;
    printf("Enter patient ID: ");
    scanf("%hu", &id);

    if (findPatientByID(id) != NULL) { // ID exists
        printf("ID already exists.\n");
        return;
    } else { // ID does not exist
        // Create a new node
        Patient* client = (Patient*)malloc(sizeof(Patient));
        // Fill data
        client->id = id;

        printf("Enter patient name: ");
        getchar(); 
        scanf("%[^\n]", client->name); // Read until a newline character is encountered

        printf("Enter patient age: ");
        scanf("%hu", &client->age);
        printf("Enter patient gender (M/F): ");
        scanf(" %c", &client->gender);

        // New node points to the old head
        client->Next = patientHead;
        // Head points to the new node
        patientHead = client;
    }
}

/******************************************************************/
void editPatient() {
    u16 id;
    printf("Enter patient ID to edit: ");
    scanf("%hu", &id);
    Patient* temp = findPatientByID(id);
    if (temp == NULL) { // Patient ID not found
        printf("Patient ID not found.\n");
        return;
    } else { // Patient found
        printf("Enter patient name: ");
        getchar(); 
        scanf("%[^\n]", temp->name); // Read until a newline character is encountered

        printf("Enter patient age: ");
        scanf("%hu", &temp->age);
        printf("Enter patient gender (M/F): ");
        scanf(" %c", &temp->gender);
        printf("Operation successful.\n");
    }
}


/******************************************************************/
Reservation* findReservationBySlot(u16 slot) {
    Reservation* current = reservationHead;
    while (current != NULL) {
        if (current->slot == slot) { // Slot reserved
            return current;
        }
        // Move to the next node
        current = current->Next;
    }
    return NULL;
}

/******************************************************************/
void reserveSlot() {
    u16 id, slot;
    printf("Enter patient ID: ");
    scanf("%hu", &id);

    Patient* client = findPatientByID(id);
    if (client == NULL) { // Patient ID not found
        printf("Patient ID not found.\n");
        return;
    }

    u8 count = 0;
    printf("Available slots:\n");
    for (int i = 0; i < SLOT_COUNT; i++) {
        if (findReservationBySlot(i) == NULL) { // Slot is free
            printf("%d. %s\n", i + 1, slots[i]);
            count++;
        }
    }

    printf("Enter slot number (1-%d): ", count);
    scanf("%hu", &slot);
    slot--; // Adjust slot number to match array index

    if (slot < 0 || slot >= SLOT_COUNT || findReservationBySlot(slot) != NULL) {
        printf("Invalid or already reserved slot.\n");
        return;
    }

    // Create a new reservation node
    Reservation* newReservation = (Reservation*)malloc(sizeof(Reservation));
    // Fill data
    newReservation->slot = slot;
    newReservation->patientID = id;
    // New reservation node points to the old head
    newReservation->Next = reservationHead;
    // Head points to the new reservation node
    reservationHead = newReservation;

    printf("Slot reserved successfully.\n");
}

/******************************************************************/
// Remove a reservation node from the linked list
void cancelReservationByID(u16 id) {
    printf("Enter patient ID to cancel reservation: ");
    scanf("%hu", &id);
    Reservation* current = reservationHead;
    Reservation* temp = NULL;

    // Check if the head node holds the reservation
    if (current != NULL && current->patientID == id) {
        reservationHead = current->Next; // Move head to the next node
        free(current); // Free the old head
        printf("Reservation cancelled.\n");
        return;
    }

    // Search for the reservation node to be deleted
    while (current != NULL && current->patientID != id) {
        temp = current;
        current = current->Next;
    }

    if (current == NULL) { // Reservation not found
        printf("Reservation not found for the given patient ID.\n");
        return;
    }


    temp->Next = current->Next;
    free(current);
    printf("Reservation cancelled.\n");
}

/******************************************************************/
// User mode functions
void viewPatientRecord() {
    u16 id;
    printf("Enter patient ID to view: ");
    scanf("%hu", &id);
    // Search for the patient with this ID
    Patient* current = findPatientByID(id);
    if (current != NULL) { // Patient found
        printf("Patient ID: %hu\n", current->id);
        printf("Name: %s\n", current->name);
        printf("Age: %hu\n", current->age);
        printf("Gender: %c\n", current->gender);
    } else {
        printf("Patient ID not found.\n");
    }
}

/******************************************************************/
void viewTodaysReservations() {
    for (int i = 0; i < SLOT_COUNT; i++) {
        printf("%d. %s - ", i + 1, slots[i]);
        // Point to head to access the first node
        Reservation* current = reservationHead;
        int found = 0;
        while (current != NULL) { // Check if slot is reserved
            if (current->slot == i) {
                printf("Patient ID %hu\n", current->patientID);//%hufor unsigned integer
                found = 1;
                break;
            }
            current = current->Next;
        }

        if (found == 0) { // Slot is not reserved
            printf("Available\n");
        }
    }
}

/******************************************************************/
void displayAdminMenu() {
    printf("\nAdmin Menu:\n");
    printf("1. Add new patient record\n");
    printf("2. Edit patient record\n");
    printf("3. Reserve a slot with the doctor\n");
    printf("4. Cancel reservation\n");
    printf("5. Exit admin mode\n");
    printf("Choose an option: ");
}

/******************************************************************/
void freeLists() {
    // Free patient list
    Patient* patientCurrent = patientHead;
    while (patientCurrent != NULL) {
        Patient* temp = patientCurrent;
        patientCurrent = patientCurrent->Next;
        free(temp);
    }

    // Free reservation list
    Reservation* reservationCurrent = reservationHead;
    while (reservationCurrent != NULL) {
        Reservation* temp = reservationCurrent;
        reservationCurrent = reservationCurrent->Next;
        free(temp);
    }
}

/******************************************************************/
int main(void) {
    u8 mode;
    while (1) {
        printf("\nChoose mode:\n1. Admin\n2. User\n3. Exit\n");
        scanf("%hhu", &mode);//%hhu to scan unsigned char

        if (mode == 1) {
            // Admin mode
            if (!checkPassword()) {
                printf("Access denied.\n");
                continue;
            }

            u8 adminChoice;
            do {
                displayAdminMenu();
                scanf("%hhu", &adminChoice);

                switch (adminChoice) {
                    case 1:
                        addPatient();
                        break;
                    case 2:
                        editPatient();
                        break;
                    case 3:
                        reserveSlot();
                        break;
                    case 4:
                        cancelReservationByID(0);
                        break;
                    case 5:
                        printf("Exiting admin mode...\n");
                        break;
                    default:
                        printf("Invalid choice.\n");
                }
            } while (adminChoice != 5);

        } else if (mode == 2) {
            // User mode
            u8 userChoice;
            printf("\nUser Menu:\n");
            printf("1. View patient record\n");
            printf("2. View today's reservations\n");
            printf("Choose an option: ");
            scanf("%hhu", &userChoice);

            switch (userChoice) {
                case 1:
                    viewPatientRecord();
                    break;
                case 2:
                    viewTodaysReservations();
                    break;
                default:
                    printf("Invalid choice.\n");
            }
        } else if (mode == 3) {
            printf("Exiting program...\n");
            freeLists(); // Free allocated memory
            break;
        } else {
            printf("Invalid mode.\n");
        }
    }
    return 0;
}
