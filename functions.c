#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

// Utility function to test parser
void testingParser(int arg1, char *arg2) {
    printf("The parser was called with arguments: %d and %s\n", arg1, arg2);
}


HashTable HT_NAME;  // Define HT_NAME here
HashTable HT_EMAIL; // Define HT_EMAIL here
int numUsers = 0; // Global variable for total number of users
int numMessages = 0; // Global variable for total number of messages
int numTotalPosts = 0; // Global variable for total number of posts

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


// WRITE FUNCTIONS BELOW
int string_comparator(const void *a, const void *b) {
    const char **str_a = (const char **)a;
    const char **str_b = (const char **)b;
    return strcmp(*str_a, *str_b); // Lexicographical comparison
}

// int user_id is auto-generated to be unique
User* create_user(const char* name, const char* email) {
    // Dynamically allocate memory for a new User
    User* user = (User*)malloc(sizeof(User));
    if (user == NULL) {
        return NULL;  // Return NULL if memory allocation fails
    }

    // Generate unique ID number based on total number of users 
    user->id = numUsers;
    numUsers++;

    // Initially no friends
    user->friends = NULL;
    user->numFriends = 0;

    // Initially no posts
    user->posts = NULL;


    // Check if name is less than 50 characters (1 for null character) and if name is unique
    if (strlen(name) < 50) {
        if (search_user_by_name(name) == NULL) { // check if name is unique
            // add second if statment to check if unique name (search_name) and for email(search email)
            strncpy(user->name, name, sizeof(user->name) - 1);  // copy string to user->name
            user->name[sizeof(user->name) - 1] = '\0';  //  add null terminator
        } else { // if name is not unique return NULL and free user
            free(user);
            return NULL;
        }
    } else { // if name is not less than 50 characters return NULL and free user
        free(user);
        return NULL;
    }

    // Check if email is less than 50 characters (1 for null character)
    if (strlen(email) < 50) {
        if (search_user_by_email(email) == NULL) { // check if email is unique
            strncpy(user->email, email, sizeof(user->email) - 1); // copy string to user->email
            user->email[sizeof(user->email) - 1] = '\0'; // add null terminator
        } else { // if email is not unique return NULL and free user
            free(user);
            return NULL;
        }
    } else { // if email is not less than 50 characters return NULL and free user
        free(user);
        return NULL;
    }

    // insert user into both Hash Tables
    user_insert(user, &HT_NAME, 0);
    user_insert(user, &HT_EMAIL, 1);

    return user;
}

void user_insert(User* user, HashTable* ht, int type) {
    // Insert the user into the hash table
    unsigned int hash = 0;
    if (type == 0) {
        hash = string_hash(user->name);
    } else {
        hash = string_hash(user->email);
    }

    int original_index = hash;

    // Linear probing: search for an empty slot
    while (ht->table[hash] != NULL) {
        hash = (hash + 1) % TABLE_SIZE;  // Move to the next slot

        // Check if Hash table is full if so, stop probing
        if (hash == original_index) {
            return;
        }
    }

    // Insert user at the found empty slot
    ht->table[hash] = user;
    ht->size++;
}


unsigned int string_hash(const char *str) {
    // utilzes the djb2 string hash function
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = hash * 33 + c; // hash * 33 + c
    }

    return hash % TABLE_SIZE;
}

void print_users() {
    int counter = 0; // counter variable for users array
    char* names[HT_NAME.size];  // users array to store total users, dynamically allocated for each name


    // for loop going through hash table and storing each user in users array
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (HT_NAME.table[i] != NULL) {
            names[counter] = (char*)malloc(50 * sizeof(char));  // Allocate memory for each name

            if (names[counter] == NULL) {
                return;
            }
            strcpy(names[counter], HT_NAME.table[i]->name); // copy the name into the names array

            counter += 1;
        }

        // if statement to break from for loop early if all users have been located
        if (counter == HT_NAME.size) {
            break;
        }
    }

    // sorting the users array using quick sort
    qsort(names, counter, sizeof(char *), string_comparator);


    // for loop printing the sorted users 
    for (int i = 0; i < counter; i++) {
        printf("%s", names[i]);

        if (i + 1 == counter) {
            printf("\n");
        } else {
            printf(",");
        }

        // Free dynamically allocated memory for each name
        free(names[i]);
    }
}


int locate_user(User* user, int type) {
    if (type == 0) {
        int hash = string_hash(user->name);

        while(1) {
            if (HT_NAME.table[hash] == user) {
                break;
            } else {
                hash = (hash + 1) % TABLE_SIZE;  // Move to the next slot
            }
        }
        return hash;

    } else {
        int hash = string_hash(user->email);
        while(1) {
            if (HT_EMAIL.table[hash] == user) {
                break;
            } else {
                hash = (hash + 1) % TABLE_SIZE;  // Move to the next slot
            }
        }
        return hash;
    }
}

void delete_user(User* user){
    // get index of user by calling locate_user function
    int hash_name = locate_user(user, 0);
    int hash_email = locate_user(user, 1);

    // free the memory of the user and set index in both Hash Tables to NULL
    free(HT_NAME.table[hash_name]);
    HT_NAME.table[hash_name] = NULL;
    HT_EMAIL.table[hash_email] = NULL;

}

void change_user_name(User* user, char* new_name) {
    int hash_name = locate_user(user, 0); // locate the user using the the locate_user function
    int hash_email = locate_user(user, 1); // locate the user using the the locate_user function

    // remove the user from both hash_tables
    HT_NAME.table[hash_name] = NULL;
    HT_EMAIL.table[hash_email] = NULL;
    HT_NAME.size--;
    HT_EMAIL.size--;


    strcpy(user->name, new_name); // copy the new name into the users name

    // insert user into both Hash Tables
    user_insert(user, &HT_NAME, 0);
    user_insert(user, &HT_EMAIL, 1);
}


void change_user_email(User* user, char* new_email) {
    int hash_name = locate_user(user, 0); // locate the user using the the locate_user function
    int hash_email = locate_user(user, 1); // locate the user using the the locate_user function

    // remove the user from both hash_tables
    HT_NAME.table[hash_name] = NULL;
    HT_EMAIL.table[hash_email] = NULL;
    HT_NAME.size--;
    HT_EMAIL.size--;


    strcpy(user->email, new_email); // copy the new email into the users email

    // insert user into both Hash Tables
    user_insert(user, &HT_NAME, 0);
    user_insert(user, &HT_EMAIL, 1);
}

User* search_user_by_name(const char* name) {
    int counter = 0;
    int hash_name = string_hash(name);
    while(1) {
        // Increment counter on every loop iteration
        counter += 1;

        if (HT_NAME.table[hash_name] == NULL) {
            hash_name = (hash_name + 1) % TABLE_SIZE;  // Move to the next slot
        } else if (strcmp(HT_NAME.table[hash_name]->name, name) == 0) {
            return HT_NAME.table[hash_name];
        }

        // Break if counter exceeds a reasonable limit to avoid infinite loop
        if (counter == TABLE_SIZE) {
            return NULL;
        }

    }
}

User* search_user_by_email(const char* email) {
    int counter = 0;
    int hash_email = string_hash(email);
    while(1) {
        // Increment counter on every loop iteration
        counter += 1;

        if (HT_EMAIL.table[hash_email] == NULL) {
            hash_email = (hash_email + 1) % TABLE_SIZE;  // Move to the next slot
        } else if (strcmp(HT_EMAIL.table[hash_email]->email, email) == 0) {
            return HT_EMAIL.table[hash_email];
        }

        // Break if counter exceeds a reasonable limit to avoid infinite loop
        if (counter == TABLE_SIZE) {
            return NULL;
        }

    }
}
FriendNode* create_friend_node(User* friend) {
    // dynamically allocate memory to new_friend_node
    FriendNode* new_friend_node = (FriendNode*)malloc(sizeof(FriendNode));

    // set new_friend_node user pointer to friend and next to NULL
    new_friend_node->user = friend;
    new_friend_node->next = NULL;
    return new_friend_node; // return new_friend_node
}


int check_friends(User* user1, User* user2) {

    // initializes result variable and pointer to head of user1's friends linked list
    int result = 0;
    FriendNode* current = user1->friends;

    // iterates through linked list
    while (current != NULL) {
        if (current->user == user2) { // checks if current user is the same as user2
            result = 1; // if True sets result = 1
            break;
        }
        current = current->next; // moves the current node pointer down the linked list
    }
    return result; // returns result (0 = False, 1 = True)
}

void add_friend(User* user1, User* user2) {

    // if statement calls function to check if two users are already friends and the user1 and user2 are not the same user
    if (check_friends(user1, user2) == 0 && user1 != user2) {

        // creates new friend nodes
        FriendNode* new_friend_node_user1 = create_friend_node(user1);
        FriendNode* new_friend_node_user2 = create_friend_node(user2);

        // Add the new friend to the head of the both users linked lists 
        new_friend_node_user2->next = user1->friends;
        user1->friends = new_friend_node_user2;

        new_friend_node_user1->next = user2->friends;
        user2->friends = new_friend_node_user1;

        // Increment numFriends by one for both users
        user1->numFriends += 1;
        user2->numFriends += 1;
    }
}

void print_friends(User* user) { // prints user's friends in ascending order
    // initialize string array of names with the size of the number of friends the user has
    int size = user->numFriends;
    char* names[size];

    // create a current FriendNode that points to the head of the users friends linked list
    FriendNode* current = user->friends;

    // iterate from 0 to size
    for (int i = 0; i < size; i++) {
        names[i] = (char*)malloc(50 * sizeof(char));  // Allocate memory for each name

        if (names[i] == NULL) { // check if memory allocation failed
            return;
        }
        strcpy(names[i], current->user->name); // copy the name into the names array
        current = current->next; // move the pointer to the next node
    }

    // sorting the users array using quick sort
    qsort(names, size, sizeof(char *), string_comparator);

    // for loop printing the sorted users 
    for (int i = 0; i < size; i++) {
        printf("%s", names[i]);

        if (i + 1 == size) {
            printf("\n");
        } else {
            printf(",");
        }

        // Free dynamically allocated memory for each name
        free(names[i]);
    }

}
// users user1 and user2 are no longer friends
void delete_friend(User* user1, User* user2) {
    FriendNode* current_1 = user1->friends;
    FriendNode* prev_1 = NULL;

    FriendNode* current_2 = user2->friends;
    FriendNode* prev_2 = NULL;


    while (current_1 != NULL) {
        if (current_1->user == user2) {
            // Friend found, remove the node from the list
            if (prev_1 == NULL) {
                // If the friend to be removed is the first node in the list
                user1->friends = current_1->next;
            } else {
                // If the friend is in the middle or end of the list
                prev_1->next = current_1->next;
            }

            // Free the memory for the friend node
            free(current_1);
            user1->numFriends--;  // Decrease the friend count for user1
            break;  // Exit the function after removal
        }

        // Move to the next friend in the list
        prev_1 = current_1;
        current_1 = current_1->next;
    }


    while (current_2 != NULL) {
        if (current_2->user == user1) {
            // Friend found, remove the node from the list
            if (prev_2 == NULL) {
                // If the friend to be removed is the first node in the list
                user2->friends = current_2->next;
            } else {
                // If the friend is in the middle or end of the list
                prev_2->next = current_2->next;
            }

            // Free the memory for the friend node
            free(current_2);
            user2->numFriends--;  // Decrease the friend count for user1
            break;  // Exit the function after removal
        }

        // Move to the next friend in the list
        prev_2 = current_2;
        current_2 = current_2->next;
    }
}

User** mutual_friends(User* user1, User* user2) {
    if (user1->friends == NULL || user2->friends == NULL) {
        return NULL;  // No mutual friends if either has no friends
    }

    User** mutual_friends_arr = (User**)malloc(sizeof(User*));
    if (mutual_friends_arr == NULL) {
        return NULL;  // Memory allocation failed
    }

    int count = 0;  // Track the number of mutual friends

    FriendNode* current_1 = user1->friends;

    while (current_1 != NULL) {
        FriendNode* current_2 = user2->friends;

        while (current_2 != NULL) {
            if (current_1->user == current_2->user) {
                // Resize the array and check if realloc succeeded
                User** temp = realloc(mutual_friends_arr, (count + 1) * sizeof(User*));
                if (temp == NULL) {
                    free(mutual_friends_arr);
                    return NULL;  // Memory reallocation failed
                }
                mutual_friends_arr = temp;

                // Add the mutual friend to the array
                mutual_friends_arr[count] = current_1->user;
                count++;
            }
            current_2 = current_2->next;
        }

        current_1 = current_1->next;
    }

    // Handle case of no mutual friends
    if (count == 0) {
        free(mutual_friends_arr);
        return NULL;
    }

    // Add NULL terminator to the array
    User** temp = realloc(mutual_friends_arr, (count + 1) * sizeof(User*));
    if (temp == NULL) {
        free(mutual_friends_arr);
        return NULL;  // Memory reallocation failed
    }
    mutual_friends_arr = temp;
    mutual_friends_arr[count] = NULL;

    return mutual_friends_arr;
}


void print_mutual_friends(User** friends) {
    int size = sizeof(friends)/sizeof(User**);
    char* names[size];


    for (int i = 0; i < size+1; i++) {
        names[i] = friends[i]->name;
    }

    qsort(names, size, sizeof(char *), string_comparator);


    // for loop printing the sorted users 
    for (int i = 0; i < size; i++) {
        printf("%s", names[i]);

        if (i+1 == size) {
            printf("\n");
        } else {
            printf(", ");
        }

    }
}


void enqueue(Message* msg, ChatNode* chatNode) {
    // checks if queue is full
    if (chatNode->chat->count == 50) {
        return;
    }

    int rear = (chatNode->chat->front + chatNode->chat->count) % 50;  // Calculate the rear index
    chatNode->chat->messages[rear] = msg; // Insert the value
    chatNode->chat->count++; // Increment the count
}

void dequeue(ChatNode* chatNode) {
    if (chatNode->chat->count == 0) {
        return;
    }
    chatNode->chat->front = (chatNode->chat->front + 1) % 50; // Move front to the next position
    chatNode->chat->count--;                          // Decrease the count
}

Message* create_message(User* sender, User* receiver, const char* content) {
    if (check_friends(sender, receiver) == 1) { // if statement checks if two users are friends
        // initializes variables to check if a chat exists between the friends
        int if_chat = 0;
        ChatNode* current_sender = (ChatNode *) sender->chats;
        
        while (current_sender != NULL) { // iterates through linked list of chats
            // if statement checks if a chat exists between the two users
            if ((current_sender->chat->user1 == sender && current_sender->chat->user2 == receiver)
                || (current_sender->chat->user1 == receiver && current_sender->chat->user2 == sender)) {
                if_chat = 1; // sets if_chat = 1 (True)
                break;
            } else {
                current_sender = (ChatNode *) current_sender->next; // moves to next node
            }
        }

        if (if_chat == 0) { // if a chat doesn't exist
            
            // create new ChatNode and chat pointers
            ChatNode* new_chat_node_sender = (ChatNode*)malloc(sizeof(ChatNode));
            ChatNode* new_chat_node_receiver = (ChatNode*)malloc(sizeof(ChatNode));

            new_chat_node_sender->chat = (Chat*)malloc(sizeof(Chat));
            new_chat_node_receiver->chat = (Chat*)malloc(sizeof(Chat));


            // initialize chat variables
            new_chat_node_sender->chat->front = 0;
            new_chat_node_sender->chat->count = 0;
            new_chat_node_sender->chat->user1 = sender;
            new_chat_node_sender->chat->user2 = receiver;

            new_chat_node_receiver->chat->front = 0;
            new_chat_node_receiver->chat->count = 0;
            new_chat_node_receiver->chat->user1 = sender;
            new_chat_node_receiver->chat->user2 = receiver;

            // create a new Message pointer
            Message* msg = (Message*) malloc(sizeof(Message));

            // initialize message variables
            msg->sender = sender;
            msg->receiver = receiver;
            strcpy(msg->content, content); // copy the content to the msg content 
            msg->id = numMessages;
            numMessages++;

            // call enqueue function to add message to queue messages queue in the chat
            enqueue(msg, new_chat_node_sender
            );
            // call enqueue function to add message to queue messages queue in the chat
            enqueue(msg, new_chat_node_receiver);

            // Add the new ChatNode to the head of linked list
            new_chat_node_sender->next = sender->chats;
            sender->chats = (struct ChatNode *) new_chat_node_sender;

            new_chat_node_receiver->next = receiver->chats;
            receiver->chats = (struct ChatNode *) new_chat_node_receiver;
            
            return msg;

        } else {
            
            ChatNode* current_receiver = (ChatNode *) receiver->chats;

            while (current_receiver != NULL) { // iterates through linked list of chats
                // if statement checks if a chat exists between the two users
                if ((current_receiver->chat->user1 == sender && current_receiver->chat->user2 == receiver)
                    || (current_receiver->chat->user1 == receiver && current_receiver->chat->user2 == sender)) {
                    break;
                } else {
                    current_receiver = (ChatNode *) current_receiver->next; // moves to next node
                }
            }
            
            if (current_sender->chat->count < 50) {
                // create a new Message pointer
                Message* msg = (Message*) malloc(sizeof(Message));

                // initialize message variables
                msg->sender = sender;
                msg->receiver = receiver;
                strcpy(msg->content, content); // copy the content to the msg content 
                msg->id = numMessages;
                numMessages++;

                enqueue(msg, current_sender);
                enqueue(msg, current_receiver);
                
                return msg;

            } else {
                
                dequeue(current_sender);
                dequeue(current_receiver);


                // create a new Message pointer
                Message* msg = (Message*) malloc(sizeof(Message));

                // initialize message variables
                msg->sender = sender;
                msg->receiver = receiver;
                strcpy(msg->content, content); // copy the content to the msg content 

                msg->id = numMessages;
                numMessages++;
                
                enqueue(msg, current_sender);
                enqueue(msg, current_receiver);               
                
                return msg;

            }

        }

    } else {
        return NULL;

    }
}


void print_message(Message* message) {
    printf("%s\n", message->content);
}


void display_chat(User* user1, User* user2) { // print messages in FIFO
    if (check_friends(user1, user2) == 1) { // if statement checks if two users are friends

        // initializes variables to check if a chat exists between the friends
        int if_chat = 0;
        ChatNode* current = (ChatNode *) user1->chats;

        while (current != NULL) { // iterates through linked list of chats
            // if statement checks if a chat exists between the two users
            if ((current->chat->user1 == user1 && current->chat->user2 == user2)
                || (current->chat->user1 == user2 && current->chat->user2 == user1)) {
                if_chat = 1; // sets if_chat = 1 (True)
                break;
            } else {
                current = (ChatNode *) current->next; // moves to next node
            }
        }

        if (if_chat == 1) {
            for (int i = 0; i < current->chat->count; i++) {
                int index = (current->chat->front + i) % 50; // Wrap around using modulus
                if (i + 1 == current->chat->count) {
                    printf("[%s:]%s\n", current->chat->messages[index]->sender->name, current->chat->messages[index]->content);

                } else {
                    printf("[%s:]%s,", current->chat->messages[index]->sender->name, current->chat->messages[index]->content);
                }
            }
        }
    }
}

Post* new_post(User* user, const char* content) {

    // create a new post Pointer
    Post* post = (Post*)malloc(sizeof(Post));

    // set the user that created the post to user
    post->user = user;
    strcpy(post->content, content); // copy the content to the post content

    post->id = numTotalPosts; // give the post a unique id
    numTotalPosts++;

    // initially no likes on the post 
    post->numLikes = 0;
    post->whoLiked = NULL;

    // creat a new PostNode and set its post to the newly created post
    PostNode* postNode = (PostNode*) malloc(sizeof(PostNode));
    postNode->post = post;

    // Add the new PostNode to the head of users posts linked list
    postNode->next = user->posts;
    user->posts = postNode;

    user->numPosts++; // add one to numPosts of User

    return post; // return the created post
}


void add_like(Post* post, User* user) {

    // Function to check if two users are friends 
    if (check_friends(user, post->user) == 1 || user == post->user) {
        // create pointer to head of whoLiked linked list in the post 
        FriendNode* current = post->whoLiked;

        // iterating through list 
        while (current != NULL) {
            if (current->user == user) { // checks if user has already liked the post 
                return;
            }
            current = current->next; // moves to next node in linked list 
        }

        // creates new node and sets its user value to the user that liked the post
        FriendNode* like_user_node = (FriendNode*)malloc(sizeof(FriendNode));
        like_user_node->user = user;

        // adds the node to the head of the post's whoLiked linked list 
        like_user_node->next = post->whoLiked;
        post->whoLiked = like_user_node;

        // increment the number of posts by 1
        post->numLikes += 1;
    }
}



// Merges two subarrays of arr[].
// First subarray is arr[left..mid]
// Second subarray is arr[mid+1..right]
void merge(Post* arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    Post* leftArr[n1];
    Post* rightArr[n2];

    // Copy data to temporary arrays
    for (i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        rightArr[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr[left..right]
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        // Compare posts: first by numLikes (descending), then by id (ascending)
        if (leftArr[i]->numLikes > rightArr[j]->numLikes ||
            (leftArr[i]->numLikes == rightArr[j]->numLikes && leftArr[i]->id < rightArr[j]->id)) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of leftArr[], if any
    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    // Copy the remaining elements of rightArr[], if any
    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

// The subarray to be sorted is in the index range [left-right]
void mergeSort(Post* arr[], int left, int right) {
    if (left < right) {
        // Calculate the midpoint
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}


void display_feed(User* user1) {
    // set current_friend to head of user's friends linked list
    FriendNode* current_friend = user1->friends;
    int total_posts = user1->numPosts;

    // iterates through linked list and adds the number of post to total_posts
    while (current_friend != NULL) {
        total_posts += current_friend->user->numPosts;
        current_friend = current_friend->next;
    }

    // resets current_friend and creats a Post* array
    current_friend = user1->friends;
    Post* arr[total_posts];
    int count = 0;

    // iterates through all of the users posts and appends it to array 
    PostNode* posts_user = user1->posts;
    while (posts_user != NULL) {
        arr[count] = posts_user->post;
        count++;
        posts_user = posts_user->next;
    }
    
    // iterates through all of the users friends posts and appends it to array
    while (current_friend != NULL) {
        PostNode* posts = current_friend->user->posts;
        while (posts != NULL) {
            arr[count] = posts->post;
            count++;
            posts = posts->next;
        }
        current_friend = current_friend->next;
    }

    // calls merge sort to sort the array
    mergeSort(arr, 0, total_posts - 1);


    // checks if more than 20 posts in array
    int range;
    if (total_posts > 20) {
        range = 20;
    } else {
        range = total_posts;
    }

    // prints out the sorted arrary
    for (int i = 0; i < range; i++) {
        if (i + 1 == range) {
            printf("[%s]:%s\n", arr[i]->user->name, arr[i]->content);
        } else {
            printf("[%s]:%s,", arr[i]->user->name, arr[i]->content);
        }
    }
}

