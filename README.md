# C Student Manager: Imperative Console App 🧑‍💻

## Project Overview
This is a straightforward command-line application built entirely in **C** (the Imperative Paradigm project for Group 1). The goal was to build a system that manages student records (Name, Age, Grade) using core C concepts like **structs** and **arrays of structs**.

This project demonstrates the **imperative programming style** by focusing on explicit state change (modifying the global student array) and using functions for every distinct operation. We implemented manual array manipulation for tasks like deletion, which is characteristic of imperative C programming.

## Getting Started: Installation & Setup 🛠️

To get this running, you'll need a standard C compiler installed (like **GCC** or **Clang**). If you're on Windows, using Git Bash or installing a development environment like MinGW is usually the easiest way to go.

1.  **Save the Code:** Ensure your source code is saved as `student_manager.c` in your working directory.
2.  **Compile:** Open your terminal/command prompt and run the standard GCC command. We're compiling it into an executable named `manager`:
    ```bash
    gcc student_manager.c -o manager
    ```
3.  **Run:** Execute the compiled file:
    ```bash
    ./manager
    ```
    *(Note: The program attempts to load data from `students_save.csv` automatically on startup.)*

## Key Features

This application implements the full list of required functions:

* **Student CRUD:** Add (with **input validation**), Delete (by Name), and Search (by Name or Age).
* **Data Analysis:** Calculate the **average age** and **average grade** across the roster.
* **List Manipulation:** Sorting by Name, Age, or Grade. We used the standard C `qsort()` function for an efficient approach.
* **Persistence:** We chose to use a simple **CSV text file** (`students_save.csv`) for saving and loading data, fulfilling the file I/O requirement.

## Execution Examples: Testing the Core Logic 🧪

The following scenarios walk through the required functions and confirm they work correctly.

### Scenario 1: Initial Setup & Statistics Check

This confirms data input works, that input validation passes, and the statistical math is correct.

| Action | Input | Expected Output |
| :--- | :--- | :--- |
| **Run Program** | `./manager` | `[INFO] Data file... not found. Starting with an empty list.` |
| **Add Student 1** | Menu `1`, Name: `Alice`, Age: `20`, Grade: `4.5` | `[SUCCESS] Student 'Alice' added successfully.` |
| **Test Validation**| Menu `1`, Name: `Invalid`, Age: `0`, Grade: `5.0` | `[ERROR] Invalid or non-positive age entered. Aborting addition.` |
| **Add Student 2** | Menu `1`, Name: `Bob`, Age: `19`, Grade: `3.0` | `[SUCCESS] Student 'Bob' added successfully. Total students: 2` |
| **Calculate Stats**| Menu `5` | `Average Age: 19.50 years`, `Average Grade: 3.75` |
| **View List** | Menu `4` | Shows Alice (ID 1) and Bob (ID 2). |

---

### Scenario 2: Sorting and Deletion

We confirm the list can be manipulated and elements can be removed.

| Action | Input | Expected Output |
| :--- | :--- | :--- |
| **Sort by Grade**| Menu `6`, Choice: `3` (Grade) | List sorted by Grade: Bob (3.0), Alice (4.5). |
| **Search by Age** | Menu `3`, Choice: `2` (Age), Age: `20` | Prints only Alice's record. |
| **Delete Student**| Menu `2`, Name: `Alice` | `[SUCCESS] Student 'Alice' has been removed.` |
| **View List** | Menu `4` | Shows only **Bob** remaining. Total students: 1. |

---

### Scenario 3: Data Persistence Check (Save/Load)

This is the most critical test, verifying that file I/O works.

| Action | Input | Expected Output |
| :--- | :--- | :--- |
| **Save Data** | Menu `7` | `[SUCCESS] Data saved to students_save.csv (1 students).` |
| **Exit** | Menu `0` | `Exiting Student Manager. Goodbye!` |
| **Rerun Program**| `./manager` | **Crucial:** Automatically prints `[SUCCESS] Data loaded from students_save.csv. Total students: 1` on startup. |
| **Verify Loaded** | Menu `4` | Shows only **Bob**'s record, confirming the data was correctly loaded from the file after a full restart. |
