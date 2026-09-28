# Social Media Platform Simulation (C)

A backend implementation of a social network simulated in C, utilizing custom data structures for user management, friendship networks, direct messaging, and ranked post feeds.

Developed for Queen's University ELEC 278 (Data Structures and Algorithms).

---

## Features

- **Dual-Indexed User Lookup**: Users are indexed simultaneously by username and unique email address using closed hash tables with linear probing for fast average lookup.
- **Friendship Graph**: Bidirectional friendship relationships maintained via singly linked lists per user, including mutual friend resolution.
- **Ranked Feed Generation**: Aggregates posts from a user and their friends, sorting the feed by engagement (likes descending, post ID ascending) using Merge Sort, and displaying the top 20 posts.
- **Direct Messaging (FIFO Queue)**: Chat sessions between mutual friends managed through circular message buffers retaining the 50 most recent messages.

---

## Data Structures & Algorithms

| Subsystem | Underlying Structure | Primary Operations & Complexities |
| :--- | :--- | :--- |
| **User Directory** | Closed Hash Table (`TABLE_SIZE = 10007`) with `djb2` hash function and linear probing | Insert: $O(1)$ avg<br>Lookup: $O(1)$ avg<br>Delete: $O(1)$ avg |
| **Friend Network** | Singly Linked List (`FriendNode`) | Add Friend: $O(N)$<br>Check Friends: $O(N)$<br>Mutual Friends: $O(N \times M)$ |
| **User Feed** | Singly Linked List (`PostNode`) merged into dynamically sized array | New Post: $O(1)$<br>Like Post: $O(L)$ (where $L$ is number of likes)<br>Display Feed: $O(K \log K)$ via Merge Sort |
| **Chat System** | Circular Array Buffer (Capacity: 50 messages) inside a Singly Linked List (`ChatNode`) | Enqueue / Dequeue: $O(1)$<br>Find Chat: $O(C)$ (where $C$ is number of active chats)<br>Display: $O(\min(C, M))$ |

---

## Repository Files

```text
.
├── functions.h    # Struct definitions (User, HashTable, FriendNode, Post, Chat, Message) and prototypes
├── functions.c    # Algorithm implementations and global state definitions
├── report.pdf     # Design specification and complexity analysis report
└── README.md      # Project overview and documentation
```

---

## Build & Integration

The project is structured as a library/module without a standalone `main()` entrypoint. You can compile the source files alongside your own test harness or driver:

### Compilation

```bash
gcc -Wall -Wextra -std=c99 -c functions.c -o functions.o
```

To compile with a custom driver (e.g., `main.c`):

```bash
gcc -Wall -Wextra -std=c99 main.c functions.c -o social_network
./social_network
```

---

## Key API Functions

### User Management
- `User* create_user(const char* name, const char* email)`: Allocates a new user, assigns an auto-incrementing ID, and inserts pointers into both `HT_NAME` and `HT_EMAIL`.
- `User* search_user_by_name(const char* name)` / `User* search_user_by_email(const char* email)`: Probes the corresponding hash table for the user pointer.
- `void print_users()`: Sorts all current usernames alphabetically using `qsort` and prints them comma-separated.
- `void delete_user(User* user)`: Clears hash table references and deallocates the user record.

### Friends
- `void add_friend(User* user1, User* user2)`: Establishes a mutual friendship link at the head of each user's friends list.
- `void delete_friend(User* user1, User* user2)`: Removes friendship links between two users.
- `User** mutual_friends(User* user1, User* user2)`: Returns a `NULL`-terminated dynamic array of shared friend pointers.

### Posts & Feed
- `Post* new_post(User* user, const char* content)`: Instantiates a post (up to 256 characters) and attaches it to the user's post list.
- `void add_like(Post* post, User* user)`: Adds a like to the post if the user is either the author or an authorized friend, preventing duplicate likes.
- `void display_feed(User* user1)`: Gathers posts from the user and all friends, sorts them by like count (descending) and post ID (ascending) via `mergeSort`, and prints the top 20.

### Messaging
- `Message* create_message(User* sender, User* receiver, const char* content)`: Creates a message between mutual friends. If the 50-message chat queue is full, the oldest message is dequeued to make room.
- `void display_chat(User* user1, User* user2)`: Prints active conversation history between two friends in FIFO order.
