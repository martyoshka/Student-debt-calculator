/*
Compile: make pointer_list
Run: ./pointer_list
make pointer_list && ./pointer_list
*/

#include "pointer_list.h"

// Create a list node.
Node* new_node(void* value, Node* next) {
    Node* node = xcalloc(1, sizeof(Node));
    node->value = value;
    node->next = next;
    return node;
}

// Print the components of the given list. Use to_string to print each element.
void print_list(Node* list, ToStringFun to_string) {
    if (list == NULL) {
        printf("[]");
    } else {
        String s = to_string(list->value);
        printf("[%s", s);
        free(s);
        for (Node* n = list->next; n != NULL; n = n->next) {
            s = to_string(n->value);
            printf(", %s", s);
            free(s);
        }
        printf("]");
    }
}

// Print list followed by a newline. Use to_string to print each element.
void println_list(Node* list, ToStringFun to_string) {
    print_list(list, to_string);
    printsln("");
}

// Free all nodes of the list. Use free_element to free each element.
// If free_element is NULL, do not free the elements.
void free_list(Node* list, FreeFun free_element) {
    Node* node_next = NULL;
    for (Node* node = list; node != NULL; node = node_next) {
        node_next = node->next;
        if (free_element) free_element(node->value);
        free(node);
    }
}

// Number of elements of the list.
int length_list(Node* list) {
    int n = 0;
    for (Node* node = list; node != NULL; node = node->next) n++;
    return n;
}

// Check whether list contains element. Use equal 
// to compare elements. If equal is NULL compare pointers.
bool contains_list(Node* list, void* element, EqualFun equal) {
    if (equal == NULL) { // identity-based equality
        for (Node* node = list; node != NULL; node = node->next) {
            if (node->value == element) { // compare pointers
                return true;
            }
        }
    } else { // content-based equality
        for (Node* node = list; node != NULL; node = node->next) {
            if (equal(node->value, element)) {
                return true;
            }
        }
    }
    return false;
}

// Remove element at position index from list. Use free_element to release the element.
// If free_element is NULL, do not release the element if.
Node* remove_list(Node* list, int index, FreeFun free_element) {
    if (list == NULL || index < 0) return list;
    if (index == 0) { // remove first element of non-empty list
        Node* n = list->next;
        if (free_element != NULL) free_element(list->value);
        free(list);
        return n;
    }
    // return second or later element of non-empty list
    int i = 0;
    for (Node* node = list; node != NULL; node = node->next) {
        if (i + 1 == index) {
            Node* to_remove = node->next;
            if (to_remove == NULL) return list;
            Node* n = to_remove->next;
            if (free_element != NULL) free_element(to_remove->value);
            free(to_remove);
            node->next = n;
            return list;
        }
        i++;
    }
    return list;
}

// Prepend an element in front of the list.
Node* prepend_list(void* value, Node* list) {
    return new_node(value, list);
}

// Adds an element to the end of the list. Modifies the existing list.
Node* append_list(Node* list, void* value) {
    if (list == NULL) { // empty list
        return new_node(value, NULL);
    } else { // non-empty list
        Node* n = list;
        while (n->next != NULL) n = n->next; // find last element
        // assert: n != NULL && n->next == NULL
        n->next = new_node(value, NULL);
        return list;
    }
}

// Copies the list. Use copy_element to copy each element.
// If copy_element is NULL, just assign the pointer to the original element.
Node* copy_list(Node* list, CopyFun copy_element) {
    if (list == NULL) {
        return NULL;
    } else {
        if (copy_element == NULL) { // just reassign, do not copy elements
            Node* result = new_node(list->value, NULL);
            for (Node* n = result; list->next != NULL; n = n->next) {
                list = list->next;
                n->next = new_node(list->value, NULL);
            }
            return result;
        } else { // copy elements
            Node* result = new_node(copy_element(list->value), NULL);
            for (Node* n = result; list->next != NULL; n = n->next) {
                list = list->next;
                n->next = new_node(copy_element(list->value), NULL);
            }
            return result;
        }
    }
}

// Insert value in list at index.
Node* insert_list(Node* list, int index, void* value) {
    if (index < 0) return list; // invalid index, no change
    if (index == 0) { // insert at front
        return new_node(value, list);
    }
    // insert after first element
    Node *node = list;
    int i = 1;
    while (node != NULL && i < index) { // skip index - 1 nodes
        node = node->next;
        i++;
    }
    if (node != NULL) {
        node->next = new_node(value, node->next);
    }
    return list;
}

// Insert value at the right position. Use compare to compare elements.
Node* insert_ordered(Node* list, void* value, CompFun compare) {
    if (list == NULL) { // empty list
        return new_node(value, NULL);
    } else if (compare(value, list->value) < 0) { // insert before first
        return new_node(value, list);
    } else { // non-empty list, find insertion position after first node
        for (Node* n = list; n != NULL; n = n->next) {
            if (n->next == NULL) { // end of list?
                n->next = new_node(value, n->next);
                break;
            } else if (compare(value, n->next->value) < 0) { // found position?
                n->next = new_node(value, n->next);
                break;
            }
        }
        return list;
    }
}

#if 0
// Reverse the list. Do not modify the input list. Use copy_element to copy each element.
// If copy_element is NULL, just assign the pointer to the original element.
Node* reverse_list(Node* list, CopyFun copy_element) {
    Node* result = NULL;
    if (copy_element == NULL) {
        for (Node* n = list; n != NULL; n = n->next) {
            result = new_node(n->value, result);
        }
    } else {
        for (Node* n = list; n != NULL; n = n->next) {
            result = new_node(copy_element(n->value), result);
        }
    }
    return result;
}
#else
// Reverse the origial (!) list.
Node* reverse_list(Node* list) {
    if (list == NULL) {
        return NULL;
    } else {
        Node* first = list;
        Node* last = list;
        Node* next;
        for (Node* n = list->next; n != NULL; n = next) {
            next = n->next;
            n->next = first;
            first = n;
        }
        last->next = NULL;
        return first;
    }
}
#endif

// Find the first element in list for which pred(element, i, x) is true.
void* find_list(Node* list, FilterFun pred, void* x) {
    int i = 0;
    for (Node* node = list; node != NULL; node = node->next, i++) {
        if (pred(node->value, i, x)) {
            return node->value;
        }
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////

#if 0
Node* map_list(Node* list, MapFun f, void* x) {
    Node* result = NULL;
    int i = 0;
    for (Node* node = list; node != NULL; node = node->next, i++) {
        result = new_node(f(node->value, i, x), result);
    }
    return reverse_list(result);
}
#else
Node* map_list(Node* list, MapFun f, void* x) {
    if (list == NULL) return NULL;
    Node* first = new_node(f(list->value, 0, x), NULL);
    Node* last = first;
    int i = 1;
    for (Node* node = list->next; node != NULL; node = node->next, i++) {
        last->next = new_node(f(node->value, i, x), NULL);
        last = last->next;
    }
    return first;
}
#endif

#if 0
Node* filter_list(Node* list, FilterFun pred, void* x) {
    Node* result = NULL;
    int i = 0;
    for (Node* node = list; node != NULL; node = node->next, i++) {
        if (pred(node->value, i, x)) {
            result = new_node(node->value, result);
        }
    }
    return reverse_list(result);
}
#else
Node* filter_list(Node* list, FilterFun pred, void* x) {
    Node* first = NULL;
    Node* last = NULL;
    int i = 0;
    for (Node* node = list; node != NULL; node = node->next, i++) {
        if (pred(node->value, i, x)) {
            if (first == NULL) {
                first = new_node(node->value, NULL);
                last = first;
            } else {
                last->next = new_node(node->value, NULL);
                last = last->next;
            }
        }
    }
    return first;
}
#endif

Node* filter_map_list(Node* list, FilterFun pred, MapFun map, void* x) {
    Node* result = NULL;
    int i = 0;
    for (Node* node = list; node != NULL; node = node->next, i++) {
        if (pred(node->value, i, x)) {
            result = new_node(map(node->value, i, x), result);
        }
    }
    return reverse_list(result);
}

void reduce_list(Node* list, ReduceFun f, void* state) {
    int i = 0;
    for (Node* node = list; node != NULL; node = node->next, i++) {
        f(state, node->value, i);
    }
}
