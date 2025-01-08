# Management-system
 
Command-line based management systems written in C for managing hospital patients and school records.

## Features

### Hospital Management System
- Patient record management with:
  - Unique ID tracking
  - Personal details (name, age, gender)
  - Disease information
  - Persistent storage in files
- CRUD operations:
  - Add new patients
  - View all patients in tabular format
  - Search patients by ID
  - Update existing records
  - Delete patient records
- Dynamic table display with auto-sizing columns
- Data persistence across sessions
- Input validation
- Memory-efficient dynamic allocation

### School Management System
- Dual management for staff and students
- Staff Management:
  - ID-based tracking
  - Subject assignment
  - Salary management
  - Performance tracking
- Student Management:
  - Academic records
  - Attendance tracking
  - Fee payment status
  - Grade management
- Features:
  - Add new records
  - View in formatted tables
  - Delete records
  - Auto-saving data
  - Memory management
  - Input validation

## Installation

```bash
# Clone the repository
git clone [repository-url]

# Compile Hospital Management System
gcc hospital-management.c -o hospital

# Compile School Management System
gcc school-management.c -o school
```

## Usage

### Hospital Management System
```bash
./hospital
```

Menu Options:
1. Add Patient
2. View All Patients
3. Search Patient
4. Update Patient
5. Delete Patient
6. Exit

### School Management System
```bash
./school
```

Menu Options:
1. Add Staff
2. View All Staff
3. Delete Staff
4. Add Student
5. View All Students
6. Delete Student
7. Exit

## Technical Details

- Language: C
- Data Storage: Text files
- Memory: Dynamic allocation
- Dependencies: Standard C libraries

## File Structure
```
.
├── hospital-management.c
├── school-management.c
├── patients.txt
├── staff.txt
└── students.txt
```

## Data Persistence
- Hospital: `patients.txt`
- School: `staff.txt`, `students.txt`

## Error Handling
- Memory allocation failures
- File I/O errors
- Invalid input validation
- Duplicate ID checking
- Record not found handling

## Contributing
1. Fork the repository
2. Create feature branch
3. Commit changes
4. Push to branch
5. Create Pull Request

## License
MIT License

## Authors
Aryan Mehta

## Version
1.0.0
