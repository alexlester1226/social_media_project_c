// functions.h
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define TABLE_SIZE 10007 // Prime number close to 10,000

extern int numUsers; // Global variable for total number of users
extern int numMessages; // Global variable for total number of messages
extern int numTotalPosts; // Global variable for total number of posts


typedef struct {
    // add attributes
    int id; // int id field
    char name[50]; // string for name with 49 characters + 1 for null character
    char email[50]; // string for email with 49 characters + 1 for null character
    struct FriendNode* friends;  // Pointer to the linked list of friends
    int numFriends; // int number of friends field
    struct PostNode* posts; // pointer to linked lists of posts made by the user
    int numPosts; // int number of posts field
    struct ChatNode* chats;  // pointer to linked lists of chats made by the user
} User;


typedef struct FriendNode {
    User* user;  // Pointer to the User struct (the friend)
    struct FriendNode* next;  // Pointer to the next friend in the list
} FriendNode;

typedef struct {
    User* table[TABLE_SIZE];  // Array to store pointers to User objects
    int size; // int number of users currently stored in the hash table
} HashTable;

// Declare hash tables
extern HashTable HT_NAME;
extern HashTable HT_EMAIL;

typedef struct {
    // add attributes
    int id; // int id field
    User* user; // pointer to user who created the post
    int numLikes; // int field for number of likes a post received
    FriendNode* whoLiked; // linked list of Users who liked the post
    char content[257]; // string of the content of the message 
} Post;

typedef struct PostNode {
    Post* post;  // Pointer to the Post  
    struct PostNode* next;  // Pointer to the next post in the list
} PostNode;

typedef struct {
    // add attributes
    int id; // int id field
    User* sender; // user pointer to the sender of the message 
    User* receiver; // user pointer to the receiver of the message 
    char content[257]; // string of the content of the message
} Message;

typedef struct {
    User* user1;             // Pointer to User 1
    User* user2;             // Pointer to User 2
    Message* messages[50];   // Fixed-size array to hold the 50 most recent messages
    int front;               // Index of the oldest message
    int count;               // Number of messages in the queue
} Chat;

typedef struct {
    Chat* chat; // pointer to chat
    struct ChatNode* next; // pointer to next ChatNode
} ChatNode;


User* create_user(const char* name, const char* email); // int user_id is auto-generated to be unique
unsigned int string_hash(const char *str); // Hash function
void user_insert(User* user, HashTable* ht_name, int type); // insert user into hashtable
int locate_user(User* user, int type);


FriendNode* create_friend_node(User* friend); // Creates and returns FriendNode
int check_friends(User* user1, User* user2); // Function to check if two users are friends
void add_friend(User* user1, User* user2); // users user1 and user2 are now friends
void delete_friend(User* user1, User* user2); // users user1 and user2 are no longer friends
void delete_user(User* user); // user is deleted
void print_users(); // prints all user names in ascending order
void change_user_name(User* user, char* new_name);
void change_user_email(User* user, char* new_email);
void print_friends(User* user); // prints user's friends in ascending order
User* search_user_by_name(const char* name);
User* search_user_by_email(const char* email);
User** mutual_friends(User* user1, User* user2); // returns an array of pointers to the mutual friends
void print_mutual_friends(User** friends); // prints mutual friends' user names in acsending order


Message* create_message(User* sender, User* receiver, const char* content); // int message_id is auto-generated to be unique
void enqueue(Message* msg, ChatNode* chatNode); // function to enqueue a Message to a messages queue
void dequeue(ChatNode* chatNode); // function to dequeue a Message from a messages queue


void print_message(Message* message);
void display_chat(User* user1, User* user2); // print messages in FIFO

Post* new_post(User* user, const char* content); // post id is auto-generated to be unique
void add_like(Post* post, User* user); // user is the individual who liked the post

void merge(Post* arr[], int left, int mid, int right); // Merges two subarrays of arr[].
void mergeSort(Post* arr[], int left, int right); // The subarray to be sorted is in the index range [left-right]
void display_feed(User* user1); // Function displays the feed of posts for a user
void testingParser(int arg1, char *arg2);


#endif
