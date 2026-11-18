# To-Do List and Task Manager (C)

A simple to-do list program written in C. Tasks are stored in a linked list.

## What it can do

- Add a task (title, priority, deadline and status)
- Show all tasks, only pending tasks, or only completed tasks
- Search tasks by a word
- Edit a task, mark it as completed, or delete it
- Sort tasks by priority, deadline or title

## Files

- `todo.c` - the whole program
- `README.md` - this file

## How to compile and run

Linux / macOS:

```
gcc -Wall todo.c -o todo
./todo
```

Windows (MinGW):

```
gcc -Wall todo.c -o todo.exe
todo.exe
```

## Headers used

- `stdio.h` - for input and output (`printf`, `scanf`, `fgets`)
- `stdlib.h` - for `malloc`, `free` and `exit`
- `string.h` - for string functions (`strcpy`, `strlen`, `strstr`, ...)

## Modules

The program has four modules, as in the project list.

| Module | What it does | Main functions |
|--------|--------------|----------------|
| 1. Task creation | Adds a new task at the end of the list | `addTask()` |
| 2. Update and delete | Edits, completes or deletes a task | `editTask()`, `completeTask()`, `deleteTask()` |
| 3. Sorting | Sorts the list by priority, deadline or title | `needsSwap()`, `sortTasks()` |
| 4. Display and filter | Shows tasks and searches by a word | `showTasks()`, `searchTasks()` |

## How the linked list works

Every task is a node. A node has the task data and a `next` pointer that points to the next node. The `head` pointer points to the first node, and the last node has `next = NULL`.

```
head -> [Task 1 | next] -> [Task 2 | next] -> [Task 3 | next] -> NULL
```

- **Add:** walk to the last node and set its `next` to the new node.
- **Delete:** keep the node before the one we want, and make it point past the deleted node. Then `free` the deleted node.
- **Find:** walk from `head` until the ID matches.

## How sorting works

The sort uses **bubble sort**. It walks through the list and swaps two neighbouring tasks if they are in the wrong order. It keeps doing this until a full walk makes no swaps. Bubble sort is simple to understand, but it is slow for very large lists (it takes about n × n steps). A to-do list is small, so this is fine here.

## Main menu

```
1. Add a task
2. Show all tasks
3. Show pending tasks
4. Show completed tasks
5. Search tasks by a word
6. Edit a task
7. Mark a task as completed
8. Delete a task
9. Sort tasks
0. Exit
```

## Example output

```
ID   PRIORITY DEADLINE   STATUS     TITLE
----------------------------------------------------------------------
1    High     25-11-2026 Pending    Submit DSA project
2    Medium   01-10-2026 Pending    Read chapter 5
3    Low      16-11-2026 Pending    Buy groceries
4    Medium   30-11-2026 Completed  Gym session
```

## Things to know

- Dates are checked, so `31-02` and `29-02-2025` are rejected. `29-02-2024` is accepted because 2024 is a leap year.
- Titles can be up to 99 characters. Longer titles are cut short.
- Keyword search and title sorting ignore capital letters.
- Tasks are only kept in memory. They are lost when the program closes.
- The program does not show "overdue" tasks, because that needs the current date.
