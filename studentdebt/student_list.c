// make student_list && ./student_list

#include "pointer_list.h"

// Data definition for a student
typedef struct {
	String name;
	String sex;
	int mat_nr;
	double debts;
} Student;



// String, String, int, double -> City*
// Create a student (constructor).
Student* new_student(String name, String sex, int mat_nr, double debts) {
    Student* s = xcalloc(1, sizeof(Student));
    s->name = s_copy(name);
    s->sex = s_copy(sex);
	s->mat_nr = mat_nr;
    s->debts = debts;
	return s;
}

// Student* -> Student*
// Create a copy of student.
void* copy_student(void* x) {
    Student* s = (Student*) x;
    return new_student(s->name, s->sex, s->mat_nr, s->debts);
}

// Student* -> String
// String representation of student in the form:
// s is a hard working student who is in debt for debts €. The matriculations number is mat_nr and the students sex is sex
String student_to_string(void* x) {
    Student* s = (Student*) x;
    int n = s_length(s->name) + s_length(s->sex) + 150;
    String text = xmalloc(n);
    snprintf(text, n, "%s is a hard working student who is in debt for %.2f Euro. The matriculation number is %d and the student's sex is %s", s->name, s->debts, s->mat_nr, s->sex);
    return text;
}

// Student* -> void
// Releases memory of a student.
void free_student(void* x) {
    Student* s = (Student*)x;
    free(s->name);
    free(s->sex);
    free(s);
}

// Student* -> bool
// Returns true if the amount of debts is above 20000.
bool poor_student(void* element, int i, void* x) {
    Student* s = (Student*)element;
    return s->debts > 20000;
}

// Student* -> bool
// Returns true if the matriculation number is shorter than *x.
bool mat_nr_less_digits_than(void* element, int i, void* x) {
    Student* s = (Student*)element; // Typecast the element to a Student*
    int* a = (int*)x; // Typecast x of type void* to int* a
	int counter = 0; // Initializing a counter to 0

	for(int i = s->mat_nr; i > 0; i = i / 10 )// initialize i with the mat_nr do the loop while i > 0 and devide i by ten after each iteration
		counter++; // increase the counter
    return (counter < (*a)); // Compare the counter to the value in a / x
}

// Student* -> String
// Maps a student to its name.
void* student_name(void* element, int i, void* x) {
	Student* s = (Student*)element;
    return s->name;
}

// Student*, int, double* -> Student*
// Creates a new student with less debts. x is in range of [0, 1].
void* pay_debts(void* element, int i, void* x){
	// todo: implement
    Student* s = copy_student(element);
    s->debts *= 1 - *(double*)x;
    return s;
}

// double*, Student*, int -> void
// Adds students debts to state.
void add_debts(void* state, void* element, int index) {
    double* a = (double*)state; // Typecast to the double value of all depts
    Student* s = (Student*)element; // typecast of the student
    *a = *a + s->debts; // add dept to total depts
}

typedef struct {
	int n;
	double debts;
}Student_stats;

// Student_stats**, Student*, int -> void
// Computes the average debts for students.
void student_stats(void* state, void* element, int index) {
    // todo: explain each line
    Student_stats** s = (Student_stats**) state; // Typecast to a pointer to the pointer to the struct
    Student* e = (Student*) element; //typecast of the element to student
    if(*s == NULL){ // check if there is no struct yet
		*s = xmalloc(sizeof(Student_stats)); // memory for studen_stats
		(*s)->n = 0; //set number to 0
		(*s)->debts = 0.0; // detps to 0 aswell
	}
	(*s)->n++; // increase number of students
	(*s)->debts += e->debts; // add dept of that student to total
}

// Student*, Student* -> { <0, 0, >0 }
// Returns =0 if x and y are equal, <0 if x smaller than y, >0 otherwise.
int cmp_student_mat_nr(void* x, void* y) {
    // todo: implement
    return ((Student*)x)->mat_nr - ((Student*)y)->mat_nr;
}

// String -> String
// Copies a string for printing (and later release).
String string_to_string(void* x) {
    return s_copy(x);
}

// String -> void
// Releases a string.
void free_string(void* x) {
    free(x);
}

int main(void) {
    report_memory_leaks(true);
	
	Student* s1 = new_student("Mina", "not binary", 2090, 15000.00);
    Student* s2 = new_student("Mike", "male", 208080, 23607.56);
	Student* s3 = new_student("Anna", "female", 42, 406.26);
	Student* s4 = new_student("Jimmy", "male", 133, 8000.90);
	
    Node* list = new_node(s1, new_node(s2, new_node(s3, new_node(s4, NULL))));
    println_list(list, student_to_string);
    printf("------------------------------------------------------------------------------------------------\n");
    
    // find first poor student, use find_list
    // print result, if any
    printsln("first poor student:");
    Student* found_student = find_list(list, poor_student, NULL);
    if(found_student){
        String s = student_to_string(found_student);
        printf("found: %s\n", s);
        free(s);
    }

    
    // find the first student with a matriculation number with a length less than max_digits, use find_list
    // print result, if any
    printsln("first student with mat_nr digit count less than max_digits:");
    int max_digits = 8;
    printf("Max digits: %d\n", max_digits);
    // todo: implement
    found_student = find_list(list,mat_nr_less_digits_than, &max_digits);
    if(found_student){
        String s = student_to_string(found_student);
        printf("found: %s\n",s);
        free(s);
    }


    // map the list of students to a list of student names
    printsln("student names:");
    Node* names = map_list(list, student_name, NULL);
    println_list(names, string_to_string);
    free_list(names, NULL);

    // reduce the amount of debts for all students by 15%
    // new list, do not modify original list elements, use map_list
    printsln("reduce debts:");
    double factor = 0.15;
    // todo: implement
    Node* list2 = map_list(list, pay_debts, &factor);
    println_list(list2, student_to_string);
    free_list(list2, free_student);
    
    // find all students with matriculation numbers with less than 4 digits, use filter_list
    printsln("short matriculation numbers:");
    max_digits = 4;
	Node* selected_students = filter_list(list,mat_nr_less_digits_than, &max_digits);
    free_list(selected_students, NULL);

    // produce a list of the names students with short matricultion numbers, use filter_map_list
    printsln("names of students with short matriculation numbers:");
    names = filter_map_list(list, mat_nr_less_digits_than, student_name, &max_digits);
    print_list(names, string_to_string);
    free_list(names, NULL);

    // compute the sum of the debts of all students, use reduce_list
    printsln("total debts:");
    double total_debts = 0;
    reduce_list(list, add_debts, &total_debts);
    printdln(total_debts);

	
    // calculate the average debts, use reduce_list
    Student_stats* stats = NULL;
    reduce_list(list, student_stats, &stats);
    if (stats != NULL) {
        printf("The average debts are: %.2f\n", stats->debts/stats->n);
        free(stats);
    }
    
    // sort the students by their matriculation number, in increasing order
    // use insert_ordered, do not modify the original list, do not copy the students
    Node* sorted_list = NULL;
    // todo: implement
    for (Node* n = list; n != NULL; n = n->next) {
        sorted_list = insert_ordered(sorted_list, n->value, cmp_student_mat_nr);
    }
    printsln("sorted students:");
    if (sorted_list != NULL) {
        println_list(sorted_list, student_to_string);
        free_list(sorted_list, NULL);
    }

    // free all memory
    free_list(list, free_student);
    
    return 0;
}
