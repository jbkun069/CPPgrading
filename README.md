# Student Grade Management System

This C++ program is designed to manage student grades, providing functionalities to add students, view records, calculate statistics, sort, and filter data. It is a simple yet efficient tool for academic record-keeping.

## Features

- **Add Student**: Input student details including name, number of subjects, and marks.
- **View All Records**: Display all student records stored in the system.
- **Calculate Statistics**: Compute and display summary statistics such as class average, highest and lowest marks, and grade distribution.
- **Sort Records**: Sort and display student records by name or average.
- **Filter Records**: Filter and display records by grade or performance (above or below a certain average).

## Usage

1. **Compile the Program**:
    ```bash
    g++ -o student_grade_manager main.cpp
    ```
2. **Run the Program**:
    ```bash
    ./student_grade_manager
    ```
3. **Interact with the Menu**: Follow the on-screen menu to add students, view records, calculate statistics, sort, and filter data.

## File Structure

- `grades.txt`: Stores student records in a plain text format. Each student's data includes their name, marks, average, and grade.

## Dependencies

- **C++ Standard Library**: The program uses standard C++ libraries for input/output operations, file handling, and data structures.

## Contributing

Feel free to fork this repository and submit pull requests with improvements or additional features.

## License

This project is licensed under the MIT License. See the LICENSE file for details.