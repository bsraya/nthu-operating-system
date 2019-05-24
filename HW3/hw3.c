#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct _Subarray{
    int first;
    int last;
    int* array_pointer;
} Subarray;

void *initialize_array(void *args);
void merge_sort(int array[], int left, int right);
void subarray_merger(int array[], int left, int middle, int right);

int main(int argc, char *argv[]){
    clock_t t; 
    t = clock(); 

    int i;    
    FILE *input_file, *output_file;
    char *line = NULL;
    size_t length = 0;

    input_file = fopen(argv[1], "r");
    input_file == NULL ? exit(EXIT_FAILURE) : input_file;
    output_file = fopen("output.txt", "w");

    while ((getline(&line, &length, input_file)) != -1){
        char *llc = malloc(strlen(line)+1);
        strcpy(llc, line);
        
        int i, count = 0;
        char* token;
        token = strtok(line, " ");

        while(token != NULL){
        	count ++;
        	token = strtok(NULL, " ");
        }

        int sortArray[count];

        token = strtok (llc, " ");
        count = 0;
        while(token != NULL){
        	sortArray[count] = atoi(token);
        	count++;
        	token = strtok(NULL, " ");
        }

        pthread_t thread1, thread2; 
        Subarray left_subarray, right_subarray;
        int middle = (sizeof(sortArray)/sizeof(sortArray[0]))/2;
        
        left_subarray.first = 0;
        left_subarray.last = middle;
        left_subarray.array_pointer = sortArray;

        right_subarray.first = middle + 1;
        right_subarray.last = (sizeof(sortArray)/sizeof(sortArray[0])) - 1;
        right_subarray.array_pointer = sortArray;
        
        pthread_create(&thread1, NULL, initialize_array, &left_subarray);
        pthread_create(&thread2, NULL, initialize_array, &right_subarray);
        
        (void) pthread_join(thread1, NULL);
        (void) pthread_join(thread2, NULL);

        subarray_merger(sortArray,0, middle, (sizeof(sortArray)/sizeof(sortArray[0])) - 1);

        for(i=0; i<(sizeof(sortArray)/sizeof(sortArray[0])); i++){
            printf("%d ", sortArray[i]);
            fprintf(output_file, "%d ",sortArray[i]);
        }
        printf("\n");
        fprintf(output_file, "\n");
    }
	free(line);
    fclose(input_file);
    fclose(output_file);

    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("It takes %f seconds to execute \n", time_taken); 
    return 0;
}

void *initialize_array(void *args){
    int first, last;
    int* sortArray;
    first = ((Subarray*)args) -> first;
    last = ((Subarray*)args) -> last;
    sortArray = ((Subarray*)args) -> array_pointer;
    merge_sort(sortArray, first, last);
}

void merge_sort(int array[], int left, int right){
    if(left < right){
        int middle = left + (right - left)/2;
        merge_sort(array, left, middle);
        merge_sort(array, middle + 1, right);   
        subarray_merger(array, left, middle, right);    
    } 
}

void subarray_merger(int array[], int left, int middle, int right){
    int i=0, j=0, k=left;
    int first_array_size = middle - left + 1, second_array_size = right - middle;
    int subarray_left[first_array_size], subarray_right[second_array_size];

    for(; i<first_array_size; i++) {
        subarray_left[i] = array[left+i];
    }
    for(; j<second_array_size; j++) {
        subarray_right[j] = array[middle+j+1];
    }

    i = 0;
    j = 0;

    while(i<first_array_size && j<second_array_size){
        if(subarray_left[i] > subarray_right[j]){
            array[k] = subarray_right[j];
            j++;
        } else {
            array[k] = subarray_left[i];
            i++;
        }
        k++; 
    }
    
    while(i<first_array_size){
        array[k] = subarray_left[i];
        i++;
        k++;
    }

    while(j<second_array_size){
        array[k] = subarray_right[j];
        j++;
        k++;
    }
 }