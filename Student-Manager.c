#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Max students is a good constant to keep. 
#define MAX_STUDENTS 100
#define NAME_LEN 50
#define DATA_FILE "students_save.csv" // Shorter file name, why not?

// Data structure
typedef struct {
    char name[NAME_LEN];
    int age;
    float grade;
} Student;

// Let's just use globals, it's a C console app, keeps things simple.
Student roster[MAX_STUDENTS];
int count = 0; // Current number of students

// --- Function Prototypes (Keep these for good practice) ---
void show_menu();
void add_student();
void delete_student();
void search_students();
void print_list();
void show_stats();
void sort_roster();
void save_to_file();
void load_from_file();

// --- Input Cleanup Function (A necessary evil in C) ---
void clean_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// --- CORE FUNCTIONS ---

// 1. ADD NEW STUDENT (A little less rigorous input check)
void add_student() {
    if (count >= MAX_STUDENTS) {
        printf("\n[Warning] Roster is full! Can't add more than %d students.\n", MAX_STUDENTS);
        return;
    }

    Student s;
    printf("\n--- Adding a Student ---\n");

    // We'll trust the user to type in a decent name for now
    printf("Name: ");
    scanf("%49s", s.name);
    clean_input_buffer(); // Clear buffer after string input

    printf("Age (must be reasonable): ");
    // Human programmers sometimes forget to check scanf return value
    scanf("%d", &s.age);
    clean_input_buffer();

    // Quick check for grades
    printf("Grade (0.0-5.0): ");
    scanf("%f", &s.grade);
    clean_input_buffer();

    if (s.age <= 0 || s.grade < 0.0 || s.grade > 5.0) {
        printf("[Error] Invalid data detected (age<=0 or grade out of range). Aborting addition.\n");
        return; // Don't add invalid data
    }
    
    // Add to the main array
    roster[count++] = s;
    printf("\n[Success] %s added! We now have %d students.\n", s.name, count);
}

// 2. DELETE STUDENT BY NAME
void delete_student() {
    char name_to_delete[NAME_LEN];
    printf("\n--- Deleting a Student ---\n");
    printf("Enter Name of student to kick out: ");
    scanf("%49s", name_to_delete);
    clean_input_buffer();

    int found_idx = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(roster[i].name, name_to_delete) == 0) {
            found_idx = i;
            break;
        }
    }

    if (found_idx != -1) {
        // Shifting array elements manually. This is where you really feel the "C" in C.
        for (int i = found_idx; i < count - 1; i++) {
            roster[i] = roster[i + 1];
        }
        count--;
        printf("\n[Success] Student '%s' has been removed.\n", name_to_delete);
    } else {
        printf("\n[Info] Couldn't find '%s'. Check the name and try again.\n", name_to_delete);
    }
}

// 3. SEARCH STUDENT (Now we're combining search and print)
void search_students() {
    int choice;
    printf("\n--- Search ---\n");
    printf("1. By Name\n2. By Age\nEnter search mode: ");
    scanf("%d", &choice);
    clean_input_buffer();

    printf("\nSearch Results:\n");
    int found_count = 0;

    if (choice == 1) {
        char name_query[NAME_LEN];
        printf("Enter Name query: ");
        scanf("%49s", name_query);
        clean_input_buffer();
        
        for (int i = 0; i < count; i++) {
            // Using strstr for partial match, which is more user-friendly
            if (strstr(roster[i].name, name_query) != NULL) {
                printf("ID: %d | Name: %-15s | Age: %-3d | Grade: %.2f\n", i + 1, roster[i].name, roster[i].age, roster[i].grade);
                found_count++;
            }
        }
    } else if (choice == 2) {
        int age_query;
        printf("Enter Age: ");
        scanf("%d", &age_query);
        clean_input_buffer();
        
        for (int i = 0; i < count; i++) {
            if (roster[i].age == age_query) {
                printf("ID: %d | Name: %-15s | Age: %-3d | Grade: %.2f\n", i + 1, roster[i].name, roster[i].age, roster[i].grade);
                found_count++;
            }
        }
    } else {
        printf("[Error] Invalid search option.\n");
        return;
    }

    if (found_count == 0) {
        printf("Nothing matched that query.\n");
    }
}

// 4. VIEW ALL STUDENTS (Simple display function)
void print_list() {
    if (count == 0) {
        printf("\n[Info] The list is empty. Add some students first!\n");
        return;
    }

    printf("\n--- Full Roster (%d Students) ---\n", count);
    printf("--------------------------------------------------\n");
    printf("%-5s | %-15s | %-5s | %-5s\n", "ID", "Name", "Age", "Grade");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-5d | %-15s | %-5d | %-5.2f\n", i + 1, roster[i].name, roster[i].age, roster[i].grade);
    }
    printf("--------------------------------------------------\n");
}

// 5. CALCULATE STATISTICS
void show_stats() {
    if (count == 0) {
        printf("\n[Info] No students, no stats.\n");
        return;
    }

    float total_age = 0;
    float total_grade = 0;

    for (int i = 0; i < count; i++) {
        total_age += roster[i].age;
        total_grade += roster[i].grade;
    }

    printf("\n--- Roster Statistics ---\n");
    printf("Total Students: %d\n", count);
    printf("Average Age: %.1f\n", total_age / count);
    printf("Average Grade: %.2f\n", total_grade / count);
    printf("--------------------------\n");
}

// --- COMPARISON FUNCTIONS (Tucked here before the sort function) ---

int compare_name(const void *a, const void *b) {
    return strcmp(((Student *)a)->name, ((Student *)b)->name);
}

int compare_age(const void *a, const void *b) {
    return ((Student *)a)->age - ((Student *)b)->age;
}

int compare_grade(const void *a, const void *b) {
    // Float comparison is annoying, so we'll just check the difference
    float diff = ((Student *)a)->grade - ((Student *)b)->grade;
    if (diff < 0) return -1;
    if (diff > 0) return 1;
    return 0;
}

// 6. SORT LIST
void sort_roster() {
    if (count < 2) {
        printf("\n[Info] Need at least two students to sort.\n");
        return;
    }

    int choice;
    printf("\n--- Sort Options ---\n");
    printf("Sort by:\n1. Name\n2. Age\n3. Grade\nWhich one? ");
    scanf("%d", &choice);
    clean_input_buffer();

    switch (choice) {
        case 1:
            // qsort is the easiest way to sort an array of structs in C
            qsort(roster, count, sizeof(Student), compare_name);
            printf("\n[Sorted] List sorted by Name.\n");
            break;
        case 2:
            qsort(roster, count, sizeof(Student), compare_age);
            printf("\n[Sorted] List sorted by Age.\n");
            break;
        case 3:
            qsort(roster, count, sizeof(Student), compare_grade);
            printf("\n[Sorted] List sorted by Grade.\n");
            break;
        default:
            printf("[Error] That wasn't a valid option.\n");
            return;
    }

    print_list();
}

// 7. SAVE DATA (CSV)
void save_to_file() {
    FILE *file = fopen(DATA_FILE, "w");
    if (file == NULL) {
        perror("[Error] Couldn't open the file, RIP data");
        return;
    }

    // Write a simple header
    fprintf(file, "Name,Age,Grade\n");

    for (int i = 0; i < count; i++) {
        // Formatting to CSV: Name,Age,Grade
        fprintf(file, "%s,%d,%.2f\n", roster[i].name, roster[i].age, roster[i].grade);
    }

    fclose(file);
    printf("\n[Saved] Data backed up to %s.\n", DATA_FILE);
}

// 7. LOAD DATA (CSV)
void load_from_file() {
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        printf("\n[Info] No saved file found. Starting fresh.\n");
        return;
    }

    // A quick way to skip the header line
    char header_line[100];
    if (fgets(header_line, sizeof(header_line), file) == NULL) {
        printf("\n[Info] Data file was empty.\n");
        fclose(file);
        return;
    }

    count = 0; // Resetting count before loading
    Student s;
    int items_read;

    // Loop until we hit MAX_STUDENTS or end of file
    while (count < MAX_STUDENTS && 
           (items_read = fscanf(file, "%49[^,],%d,%f\n", s.name, &s.age, &s.grade)) == 3) {
        
        roster[count++] = s;
    }
    
    if (items_read != EOF && items_read != 3) {
        // If fscanf stopped without reading 3 items, something is wrong with the file structure
        printf("\n[Warning] Corrupted data found at the end of the file.\n");
    }

    fclose(file);
    printf("\n[Loaded] %d students loaded from file.\n", count);
}

// --- MAIN MENU AND PROGRAM ENTRY POINT ---

void show_menu() {
    printf("\n============================================\n");
    printf("        C Student Manager (Human Edition)   \n");
    printf("============================================\n");
    printf("1. Add Student\n");
    printf("2. Delete Student\n");
    printf("3. Search Students\n");
    printf("4. View Full List\n");
    printf("5. Show Statistics\n");
    printf("6. Sort List\n");
    printf("7. Save Data (CSV)\n");
    printf("8. Load Data (CSV)\n");
    printf("0. Exit\n");
    printf("--------------------------------------------\n");
    printf("What do you want to do? ");
}

int main() {
    int choice;

    // Load first, always a good idea
    load_from_file(); 

    do {
        show_menu();
        
        // This input loop is the bane of every C programmer's existence.
        if (scanf("%d", &choice) != 1) {
            printf("\n[Seriously, try again] Input must be a number.\n");
            clean_input_buffer();
            continue;
        }
        clean_input_buffer(); // Clean up after the integer read

        switch (choice) {
            case 1: add_student(); break;
            case 2: delete_student(); break;
            case 3: search_students(); break;
            case 4: print_list(); break;
            case 5: show_stats(); break;
            case 6: sort_roster(); break;
            case 7: save_to_file(); break;
            case 8: load_from_file(); break;
            case 0: printf("\nLogging off. Don't forget to save your work!\n"); break;
            default: printf("\n[Oops] Invalid choice. Range is 0-8.\n"); break;
        }
        
    } while (choice != 0);

    return 0;
}
