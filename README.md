📚 Library Management System (C Language)

This project is a Library Management System implemented in C using structures, file handling, and modular functions. It allows users to manage books efficiently with options to add, search, issue, return, delete, update records, search by text, view statistics, and create backups. All data is saved permanently using .dat files.


---

✅ Features

📘 Book Management

Add new books

Display all books

Update book details

Delete a specific book


🔍 Search System

Search by Book ID

Search by Title/Author keyword


📝 Issue & Return

Issue a book to a user

Return a book

Calculates fine based on late days (₹5/day)


📊 Library Statistics

Total books

Issued books

Available books


💾 File Handling

Uses library.dat for storing book records

Creates backup.dat using backup option

Data remains safe even after closing the program


🔐 Admin Login

Password-based login (admin123)

Prevents unauthorized access



---

🛠 Technologies Used

C language

File handling (fopen, fread, fwrite, fseek)

Structures (typedef struct)

Modular functions

Basic string handling



---

📂 File Structure

library.c          # main source code
library.dat        # data file generated automatically
backup.dat         # backup file (optional)
README.md          # documentation

