#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
//#define MAX 100
void conquer(int low,int middle, int high); // function to merge arrays together
//creating an element for every array with its low ,midddle and high as parameters
typedef struct element {
    int low;
    int middle;
    int high;
    int *arr;
};
//int *temp[MAX];
//global variable pointer to array
int *temp;
int arraylength;


void * split(void *list)
{          //creating point to struct
    //pointer as it should point to the global array
    struct element *e=(struct element *) list;
    //at this case we don't initialize e->high or e->low as it points to elements in the array
    //e->low=temp[0];
    //  e->high=temp[arraylength];
    //   printf("%d  %d",e->low,e->high);
    //creating elements from struct for left and right child of any array
    //till we can no longer split the array
    e->middle = (e->low+e->high)/2;
    e->arr=temp;
    
    
    while(e->low < e->high && e->low!= e->high) {
        //printf("finished! \n");
        struct element left; //left subarray will hold elements from start "low" till middle
        left.low = e->low;
        left.high = e->middle;
        //right subarray will hold elements from middle till the end "high"
        struct element right;
        right.low = e->middle+1;
        right.high = e->high;
        //begininng of creating thread and start sorting
        //creating thread 1
        pthread_t t1;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        
        //creating thread 2
        pthread_t t2;
        int check =pthread_create(&t1,&attr, split, &left);
        if (check!=0){
            printf("error occured in creation of thread of tid1\n");
        }
        
        
        check=pthread_create(&t2,&attr, split, &right);
        if (check!=0){
            printf("error occured in creation of thread of tid2\n");
        }
        
        
        pthread_join(t1, NULL);
        //call the method conquer to start sorting each sub array
        //provided parameters are for subarray requested to be sorted
        conquer(e->low,e->middle, e->high);
        //second thread joining and conquer
        pthread_join(t2, NULL);
        conquer(e->low,e->middle, e->high);
        
        pthread_attr_destroy(&attr);
    }
}



void printArray (int *temp,int arraylength){  //printing array
    for (int k= 0; k <arraylength; k++)
        printf ("%d ", temp[k]);
    
}

void createMainthread(int *temp){
    int max=arraylength-1; //as not to neglect last element in array when sorting
    struct element origin;
    origin.low= 0;
    origin.high = max;
    origin.middle= (origin.high+origin.low)/2;
    origin.arr=temp;
    
    //create thread for this array that's our first conquer and we will split it further in the function
    pthread_t t0;
    //pthread_attr_t attr;
    int kiko=pthread_create(&t0, NULL, split, &origin);
    if (kiko){
        printf("error occured in creation of thread of tid0\n");
    }
    pthread_join(t0, NULL);
    
    
}
int main()
{
    int k=0; //index
    //pointer to file
    FILE *file=fopen("input.txt","r"); //open file for reading
    if (file == NULL) {
        printf("File not found");
        exit(0);
    }
    
    fscanf(file,"%d",&arraylength); //configure array size from file
    if(arraylength==0){
        printf("array is empty!");
        return 0;
    }
    //prepare allocation with same size
    temp=(int *)malloc(arraylength*sizeof(int));
    //inseeting elementss into prepared  to start
    while(k<arraylength){
        fscanf(file,"%d",&temp[k++]);
    }
    //printing array before sorting
    printf("Before sorting: \n");
    printArray(temp,arraylength);
    printf("\n");
    
    //create object from element struct and set the low and high to hold whole array
    //to initialize split function and start the sorting by splitting
    
    createMainthread(temp);
    fclose(file);   //close file
    //sleep(1);
    //print sorted array
    printf("After sorting: \n");
    printArray(temp,arraylength);
    printf("\n");
    return 0;
}
//method for sorting elements in arrays and sub-arrays for each thread
void conquer(int low,int middle, int high)
{
    
    int h=0;
    int low1 = low; //save it in a variable for later to swap arrays in line 186
    int elementmiddle = middle+1;
    //iterator for temp array for each thread
    int mc=0;
    //size of temp array
    int arlength=high-low+1;
    //temp array for each thread
    int newtemp[arlength];
    //compare first element in the array with every element after middle and vise versa
    for (h = 0; low <= middle && elementmiddle <= high;) {
        if (temp[low] > temp[elementmiddle]){
            //put elemnt in new temp and increment iterator and middle factor
            newtemp[mc] = temp[elementmiddle];
            mc+=1;
            elementmiddle++;
        }
        else{
            //else increment low factor
            newtemp[mc] = temp[low];
            mc+=1;
            low++;
        }
    }
    // put remaining of array in new temp to be comared in next thread if available
    for(h=0;low <= middle;low++) {
        newtemp[mc] = temp[low];
        mc+=1;
        
    }
    //loop for putting remaining elements in the array new tep
    //ready for next thread to sort
    for(h=0;elementmiddle <= high; elementmiddle++) {
        newtemp[mc] = temp[elementmiddle];
        mc+=1;
        
    }
    //transfer array of new temp to array temp which is our main array
    for (h = 0; h < arlength ; h++)
        temp[low1+h] = newtemp[h];
    //exit thread
    pthread_exit(NULL);
}

